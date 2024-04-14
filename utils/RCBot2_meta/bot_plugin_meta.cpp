/**
 * vim: set ts=4 sw=4 tw=99 noet :
 * ======================================================
 * Metamod:Source Sample Plugin
 * Written by AlliedModders LLC.
 * ======================================================
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from 
 * the use of this software.
 *
 * This sample plugin is public domain.
 */

#include <cstdio>

#include "bot_plugin_meta.h"

#include "filesystem.h"
#include "interface.h"

#ifdef __linux__
#include "shake.h"    //bir3yk
#endif

#include "ndebugoverlay.h"
#include "irecipientfilter.h"

#include "bot_cvars.h"

#if SOURCE_ENGINE <= SE_DARKMESSIAH
#include "tier1/bitbuf.h"
#endif

#ifdef _WIN32
	#include <ctime>
#endif

// for IServerTools
#include "bot.h"
#include "bot_configfile.h"
#include "bot_globals.h"
#include "bot_profile.h"
#include "bot_menu.h"
#include "bot_getprop.h"
#include "bot_event.h"
#include "bot_profiling.h"
#include "bot_wpt_dist.h"
#include "bot_squads.h"
#include "bot_accessclient.h"
#include "bot_weapons.h"
#include "bot_waypoint_visibility.h"
#include "bot_kv.h"
#include "bot_sigscan.h"
//#include "bot_mods.h"

#include "tier0/icommandline.h"

#include "rcbot/logging.h"

#include <cstring>

SH_DECL_HOOK6(IServerGameDLL, LevelInit, SH_NOATTRIB, 0, bool, char const *, char const *, char const *, char const *, bool, bool);
SH_DECL_HOOK3_void(IServerGameDLL, ServerActivate, SH_NOATTRIB, 0, edict_t *, int, int);
SH_DECL_HOOK1_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool);
SH_DECL_HOOK0_void(IServerGameDLL, LevelShutdown, SH_NOATTRIB, 0);
SH_DECL_HOOK2_void(IServerGameClients, ClientActive, SH_NOATTRIB, 0, edict_t *, bool);
SH_DECL_HOOK1_void(IServerGameClients, ClientDisconnect, SH_NOATTRIB, 0, edict_t *);
SH_DECL_HOOK2_void(IServerGameClients, ClientPutInServer, SH_NOATTRIB, 0, edict_t *, char const *);
SH_DECL_HOOK1_void(IServerGameClients, SetCommandClient, SH_NOATTRIB, 0, int);
SH_DECL_HOOK5(IServerGameClients, ClientConnect, SH_NOATTRIB, 0, bool, edict_t *, const char*, const char *, char *, int);
SH_DECL_HOOK2(IGameEventManager2, FireEvent, SH_NOATTRIB, 0, bool, IGameEvent *, bool);

#if SOURCE_ENGINE >= SE_ORANGEBOX
SH_DECL_HOOK2_void(IServerGameClients, NetworkIDValidated, SH_NOATTRIB, 0, const char *, const char *);
SH_DECL_HOOK2_void(IServerGameClients, ClientCommand, SH_NOATTRIB, 0, edict_t *, const CCommand &);
#else
SH_DECL_HOOK1_void(IServerGameClients, ClientCommand, SH_NOATTRIB, 0, edict_t *);
#endif

SH_DECL_MANUALHOOK2_void(MHook_PlayerRunCmd, 0, 0, 0, CUserCmd*, IMoveHelper*); 

IServerGameDLL *server = nullptr;
IGameEventManager2 *gameevents = nullptr;
IServerPluginCallbacks *vsp_callbacks = nullptr;
ICvar *icvar = nullptr;
IVEngineServer *engine = nullptr;  // helper functions (messaging clients, loading content, making entities, running commands, etc)
IFileSystem *filesystem = nullptr;  // file I/O 
IGameEventManager2 *gameeventmanager = nullptr;
IGameEventManager *gameeventmanager1 = nullptr;  // game events interface
IPlayerInfoManager *playerinfomanager = nullptr;  // game dll interface to interact with players
IServerPluginHelpers *helpers = nullptr;  // special 3rd party plugin helpers from the engine
IServerGameClients* gameclients = nullptr;
IEngineTrace *enginetrace = nullptr;
IEffects *g_pEffects = nullptr;
IBotManager *g_pBotManager = nullptr;
CGlobalVars *gpGlobals = nullptr;
IVDebugOverlay *debugoverlay = nullptr;
IServerGameEnts *servergameents = nullptr; // for accessing the server game entities
IServerGameDLL *servergamedll = nullptr;
IServerTools *servertools = nullptr;

RCBotPluginMeta g_RCBotPluginMeta;

PLUGIN_EXPOSE(RCBotPluginMeta, g_RCBotPluginMeta);

static ConVar rcbot2_ver_cvar("dirtbot_ver", "0.1", FCVAR_REPLICATED, "Dirtbot version");

CON_COMMAND(rcbotd, "access the bot commands on a server")
{
#if SOURCE_ENGINE <= SE_DARKMESSIAH
	CCommand args;
#endif
	if (!engine->IsDedicatedServer() || !CBotGlobals::IsMapRunning())
	{
		logger->Log(LogLevel::ERROR, "Error, no map running or not dedicated server");
		return;
	}

	//iResult = CBotGlobals::m_pCommands->execute(NULL,engine->Cmd_Argv(1),engine->Cmd_Argv(2),engine->Cmd_Argv(3),engine->Cmd_Argv(4),engine->Cmd_Argv(5),engine->Cmd_Argv(6));
	eBotCommandResult iResult = CBotGlobals::m_pCommands->execute(nullptr, args.Arg(1), args.Arg(2), args.Arg(3),
	                                                              args.Arg(4), args.Arg(5), args.Arg(6));

	if (iResult == COMMAND_ACCESSED)
	{
		// ok
	}
	else if (iResult == COMMAND_REQUIRE_ACCESS)
	{
		logger->Log(LogLevel::ERROR, "You do not have access to this command");
	}
	else if (iResult == COMMAND_NOT_FOUND)
	{
		logger->Log(LogLevel::ERROR, "bot command not found");
	}
	else if (iResult == COMMAND_ERROR)
	{
		logger->Log(LogLevel::ERROR, "bot command returned an error");
	}
}

class CBotRecipientFilter : public IRecipientFilter
{
public:
	CBotRecipientFilter(edict_t *pPlayer)
	{
		m_iPlayerSlot = ENTINDEX(pPlayer);
	}

	bool IsReliable() const override { return false; }
	bool IsInitMessage() const override { return false; }

	int	GetRecipientCount() const override { return 1; }
	int	GetRecipientIndex(int slot) const override { return m_iPlayerSlot; }

private:
	int m_iPlayerSlot;
};

class CClientBroadcastRecipientFilter : public IRecipientFilter
{
public:

	CClientBroadcastRecipientFilter() {
		m_iMaxCount = 0;

		for (int i = 0; i < RCBOT_MAXPLAYERS; ++i) {
			const CClient* client = CClients::get(i);

			if (client->isUsed()) {
				IPlayerInfo *p = playerinfomanager->GetPlayerInfo(client->getPlayer());

				if (p->IsConnected() && !p->IsFakeClient()) {
					m_iPlayerSlot[m_iMaxCount] = i;
					m_iMaxCount++;
				}
			}
		}
	}

	bool IsReliable() const override { return false; }
	bool IsInitMessage() const override { return false; }

	int	GetRecipientCount() const override { return m_iMaxCount; }
	int	GetRecipientIndex(int slot) const override { return m_iPlayerSlot[slot] + 1; }

private:

	int m_iMaxCount;
	int m_iPlayerSlot[RCBOT_MAXPLAYERS];
};

///////////////
// hud message
///////////////
void RCBotPluginMeta::HudTextMessage(edict_t *pEntity, const char *szMessage)
{
	int msgid = 0;
	int imsgsize = 0;
	char msgbuf[64];
	bool bOK;

	int hint = -1;
	int say = -1;

	while ((bOK = servergamedll->GetUserMessageInfo(msgid, msgbuf, 63, imsgsize)) == true)
	{
		if (std::strcmp(msgbuf, "HintText") == 0)
			hint = msgid;
		else if (std::strcmp(msgbuf, "SayText") == 0)
			say = msgid;

		msgid++;
	}

	if (msgid == 0)
		return;

	// if (!bOK)
	// return;

	CBotRecipientFilter *filter = new CBotRecipientFilter(pEntity);

	bf_write *buf;

	if (hint > 0) {
		buf = engine->UserMessageBegin(filter, hint);
		buf->WriteString(szMessage);
		engine->MessageEnd();
	}

	if (say > 0) {
		char chatline[128];
		snprintf(chatline, sizeof(chatline), "\x01\x04[RCBot2]\x01 %s\n", szMessage);

		buf = engine->UserMessageBegin(filter, say);
		buf->WriteString(chatline);
		engine->MessageEnd();
	}

	delete filter;
}

//////////////////////////
// chat broadcast message
//////////////////////////
void RCBotPluginMeta::BroadcastTextMessage(const char *szMessage)
{
	int msgid = 0;
	int imsgsize = 0;
	char msgbuf[64];
	bool bOK;

	int hint = -1;
	int say = -1;

	while ((bOK = servergamedll->GetUserMessageInfo(msgid, msgbuf, 63, imsgsize)) == true)
	{
		if (std::strcmp(msgbuf, "HintText") == 0)
			hint = msgid;
		else if (std::strcmp(msgbuf, "SayText") == 0)
			say = msgid;

		msgid++;
	}

	if (msgid == 0)
		return;

	CClientBroadcastRecipientFilter *filter = new CClientBroadcastRecipientFilter();

	if (say > 0) {
		char chatline[128];
		snprintf(chatline, sizeof(chatline), "\x01\x04[RCBot2]\x01 %s\n", szMessage);

		bf_write* buf = engine->UserMessageBegin(filter, say);
		buf->WriteString(chatline);
		engine->MessageEnd();
	}

	delete filter;
}

void RCBotPluginMeta::Hook_PlayerRunCmd(CUserCmd *ucmd, IMoveHelper *moveHelper)
{
	static CBot *pBot;

	CBaseEntity *pPlayer = META_IFACEPTR(CBaseEntity);

	edict_t *pEdict = servergameents->BaseEntityToEdict(pPlayer);

	pBot = CBots::getBotPointer(pEdict);
	
	if ( pBot )
	{
		static CBotCmd *cmd;
		
		cmd = pBot->getUserCMD();

		// put the bot's commands into this move frame
		ucmd->buttons = cmd->buttons;
		ucmd->forwardmove = cmd->forwardmove;
		ucmd->impulse = cmd->impulse;
		ucmd->sidemove = cmd->sidemove;
		ucmd->upmove = cmd->upmove;
		ucmd->viewangles = cmd->viewangles;
		ucmd->weaponselect = cmd->weaponselect;
		ucmd->weaponsubtype = cmd->weaponsubtype;
		ucmd->tick_count = cmd->tick_count;
		ucmd->command_number = cmd->command_number;
	}
	RETURN_META(MRES_IGNORED);
}

/** 
 * Something like this is needed to register cvars/CON_COMMANDs.
 */
class BaseAccessor : public IConCommandBaseAccessor
{
public:
	bool RegisterConCommandBase(ConCommandBase *pCommandBase) override
	{
		/* Always call META_REGCVAR instead of going through the engine. */
		return META_REGCVAR(pCommandBase);
	}
} s_BaseAccessor;

bool RCBotPluginMeta::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	extern MTRand_int32 irand;

	PLUGIN_SAVEVARS();

	GET_V_IFACE_CURRENT(GetEngineFactory, enginetrace, IEngineTrace, INTERFACEVERSION_ENGINETRACE_SERVER);	
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer, INTERFACEVERSION_VENGINESERVER);
	GET_V_IFACE_CURRENT(GetEngineFactory, gameevents, IGameEventManager2, INTERFACEVERSION_GAMEEVENTSMANAGER2);
	GET_V_IFACE_CURRENT(GetEngineFactory, helpers, IServerPluginHelpers, INTERFACEVERSION_ISERVERPLUGINHELPERS);
	GET_V_IFACE_CURRENT(GetEngineFactory, icvar, ICvar, CVAR_INTERFACE_VERSION);

	GET_V_IFACE_ANY(GetEngineFactory, filesystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION)

	GET_V_IFACE_ANY(GetServerFactory, servergameents, IServerGameEnts, INTERFACEVERSION_SERVERGAMEENTS);
	GET_V_IFACE_ANY(GetServerFactory, server, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL);
	GET_V_IFACE_ANY(GetServerFactory, gameclients, IServerGameClients, INTERFACEVERSION_SERVERGAMECLIENTS);
	GET_V_IFACE_ANY(GetServerFactory, playerinfomanager, IPlayerInfoManager, INTERFACEVERSION_PLAYERINFOMANAGER);

	GET_V_IFACE_ANY(GetServerFactory, g_pEffects, IEffects, IEFFECTS_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetServerFactory, g_pBotManager, IBotManager, INTERFACEVERSION_PLAYERBOTMANAGER);
#if SOURCE_ENGINE > SE_DARKMESSIAH
	GET_V_IFACE_ANY(GetServerFactory, servertools, IServerTools, VSERVERTOOLS_INTERFACE_VERSION);
#endif

#ifndef __linux__
	GET_V_IFACE_CURRENT(GetEngineFactory,debugoverlay, IVDebugOverlay, VDEBUG_OVERLAY_INTERFACE_VERSION);
#endif
	GET_V_IFACE_ANY(GetServerFactory, servergamedll, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL);
	GET_V_IFACE_ANY(GetServerFactory, gameclients, IServerGameClients, INTERFACEVERSION_SERVERGAMECLIENTS);

	gpGlobals = ismm->GetCGlobals();

	META_LOG(g_PLAPI, "Starting plugin.");

	/* Load the VSP listener.  This is usually needed for IServerPluginHelpers. */
	ismm->AddListener(this, this);
	if ((vsp_callbacks = ismm->GetVSPInfo(nullptr)) == nullptr)
	{
		ismm->EnableVSPListener();
	}

#if SOURCE_ENGINE > SE_DARKMESSIAH
	SH_ADD_HOOK_MEMFUNC(IServerGameDLL, LevelInit, server, this, &RCBotPluginMeta::Hook_LevelInit, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameDLL, ServerActivate, server, this, &RCBotPluginMeta::Hook_ServerActivate, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameDLL, GameFrame, server, this, &RCBotPluginMeta::Hook_GameFrame, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameDLL, LevelShutdown, server, this, &RCBotPluginMeta::Hook_LevelShutdown, false);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientActive, gameclients, this, &RCBotPluginMeta::Hook_ClientActive, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientDisconnect, gameclients, this, &RCBotPluginMeta::Hook_ClientDisconnect, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientPutInServer, gameclients, this, &RCBotPluginMeta::Hook_ClientPutInServer, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientConnect, gameclients, this, &RCBotPluginMeta::Hook_ClientConnect, false);
#endif
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientCommand, gameclients, this, &RCBotPluginMeta::Hook_ClientCommand, false);
#if SOURCE_ENGINE > SE_DARKMESSIAH
	//Hook FireEvent to our function - unstable for TF2? [APG]RoboCop[CL]
	SH_ADD_HOOK_MEMFUNC(IGameEventManager2, FireEvent, gameevents, this, &RCBotPluginMeta::FireGameEvent, false);
#endif

#if SOURCE_ENGINE >= SE_ORANGEBOX
	g_pCVar = icvar;
	ConVar_Register(0, &s_BaseAccessor);
#else
	ConCommandBaseMgr::OneTimeInit(&s_BaseAccessor);
#endif

#if SOURCE_ENGINE > SE_DARKMESSIAH
	// read loglevel from startup param for early logging
	ConVarRef rcbot_loglevel("rcbot_loglevel");
	rcbot_loglevel.SetValue(CommandLine()->ParmValue("+rcbot_loglevel", rcbot_loglevel.GetInt()));
#endif

	// Read Signatures and Offsets
	CBotGlobals::initModFolder();
#if SOURCE_ENGINE > SE_DARKMESSIAH
	CBotGlobals::readRCBotFolder();
#endif

	char filename[512];
	// Load RCBOT2 hook data
	CBotGlobals::buildFileName(filename, "hookinfo", BOT_CONFIG_FOLDER, "ini");

	std::fstream fp(filename, std::fstream::in);

	CRCBotKeyValueList kvl;

	if (fp)
		kvl.parseFile(fp);

	void *gameServerFactory = reinterpret_cast<void*>(ismm->GetServerFactory(false));

	int val;

#ifdef _WIN32
	if (kvl.getInt("runplayermove_dods_win", &val))
		rcbot_runplayercmd_dods.SetValue(val);
	if (kvl.getInt("gamerules_win", &val))
		rcbot_gamerules_offset.SetValue(val);
	if (kvl.getInt("runplayermove_synergy_win", &val))
		rcbot_runplayercmd_syn.SetValue(val);
	if (kvl.getInt("getdatadescmap_win", &val))
		rcbot_datamap_offset.SetValue(val);
#else
	if (kvl.getInt("runplayermove_dods_linux", &val))
		rcbot_runplayercmd_dods.SetValue(val);
	if (kvl.getInt("runplayermove_synergy_linux", &val))
		rcbot_runplayercmd_syn.SetValue(val);
	if (kvl.getInt("getdatadescmap_linux", &val))
		rcbot_datamap_offset.SetValue(val);
#endif

	g_pGameRules_Obj = new CGameRulesObject(kvl, gameServerFactory);
	g_pGameRules_Create_Obj = new CCreateGameRulesObject(kvl, gameServerFactory);

	if (fp)
		fp.close();

	if (!CBotGlobals::gameStart())
		return false;

	CBotMod *pMod = CBotGlobals::getCurrentMod();

#ifdef OVERRIDE_RUNCMD
	// TODO figure out a more robust gamedata fix instead of vtable
	#if SOURCE_ENGINE == SE_DODS
	SH_MANUALHOOK_RECONFIGURE(MHook_PlayerRunCmd, rcbot_runplayercmd_dods.GetInt(), 0, 0);
	#elif SOURCE_ENGINE == SE_SDK2013
#endif

#endif

	ENGINE_CALL(LogPrint)("All hooks started!\n");

	//MathLib_Init( 2.2f, 2.2f, 0.0f, 2.0f );
	//ConVar_Register( 0 );
	//InitCVars( interfaceFactory ); // register any cvars we have defined

	std::srand( static_cast<unsigned>(time(nullptr)) );  // initialize the random seed
	irand.seed( static_cast<unsigned>(time(nullptr)) );

	// Find the RCBOT2 Path from metamod VDF
	extern IFileSystem *filesystem;
	KeyValues *mainkv = new KeyValues("metamodplugin");
	
	const char *rcbot2path;
	logger->Log(LogLevel::INFO, "Reading rcbot2 path from VDF...");
#if SOURCE_ENGINE > SE_DARKMESSIAH
	mainkv->LoadFromFile(filesystem, "addons/metamod/rcbot2.vdf", "MOD");
	
	mainkv = mainkv->FindKey("Metamod Plugin");

	if (mainkv)
		rcbot2path = mainkv->GetString("rcbot2path", "\0");

	mainkv->deleteThis(); //mainkv possible redundant? [APG]RoboCop[CL]
	//eventListener2 = new CRCBotEventListener();
#endif
	// Initialize bot variables
	CBotProfiles::setupProfiles();

	//CBotEvents::setupEvents();
	CWaypointTypes::setup();
	CWaypoints::setupVisibility();

	CBotConfigFile::reset();	
	CBotConfigFile::load();

	CBotMenuList::setupMenus();

	//CRCBotPlugin::ShowLicense();	

	//RandomSeed((unsigned int)time(NULL));

	CClassInterface::init();

#if SOURCE_ENGINE > SE_DARKMESSIAH
	RCBOT2_Cvar_setup(g_pCVar);
#endif

	// Bot Quota Settings
	char bq_line[128];

	int bot_count = 0;
	int human_count = 0;

	for (int i = 0; i < RCBOT_MAXPLAYERS; ++i) {
		m_iTargetBots[i] = 0;
	}

	CBotGlobals::buildFileName(filename, "bot_quota", BOT_CONFIG_FOLDER, "ini");
	fp = std::fstream(filename, std::fstream::in);

	std::memset(bq_line, 0, sizeof(bq_line));

	if (fp) {
		while (fp.getline(bq_line, sizeof(bq_line))) {
			if (bq_line[0] == '#')
				continue;

			for (int i = 0; i < sizeof(bq_line); ++i) {
				if (bq_line[i] == '\0')
					break;

				if (!isdigit(bq_line[i]))
					bq_line[i] = ' ';
			}

			if (std::sscanf(bq_line, "%d %d", &human_count, &bot_count) == 2) {
				if (human_count < 0 || human_count > 32) {
					logger->Log(LogLevel::WARN, "Bot Quota - Invalid Human Count %d", human_count);
					continue;
				}

				if (bot_count < 0 || bot_count > 32) {
					logger->Log(LogLevel::WARN, "Bot Quota - Invalid Bot Count %d", bot_count);
					continue;
				}

				m_iTargetBots[human_count] = bot_count;
				logger->Log(LogLevel::INFO, "Bot Quota - Humans: %d, Bots: %d", human_count, bot_count);
			}
		}
	}

	return true;
}

bool RCBotPluginMeta::FireGameEvent(IGameEvent * pevent, bool bDontBroadcast)
{
	CBotEvents::executeEvent(pevent,TYPE_IGAMEEVENT);

	RETURN_META_VALUE(MRES_IGNORED, true);
}

bool RCBotPluginMeta::Unload(char *error, size_t maxlen)
{	
	//CBots::kickRandomBot(RCBOT_MAXPLAYERS); //breaks the bot quota system? [APG]RoboCop[CL]
	
	SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, LevelInit, server, this, &RCBotPluginMeta::Hook_LevelInit, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, ServerActivate, server, this, &RCBotPluginMeta::Hook_ServerActivate, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, GameFrame, server, this, &RCBotPluginMeta::Hook_GameFrame, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, LevelShutdown, server, this, &RCBotPluginMeta::Hook_LevelShutdown, false);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientActive, gameclients, this, &RCBotPluginMeta::Hook_ClientActive, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientDisconnect, gameclients, this, &RCBotPluginMeta::Hook_ClientDisconnect, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientPutInServer, gameclients, this, &RCBotPluginMeta::Hook_ClientPutInServer, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientConnect, gameclients, this, &RCBotPluginMeta::Hook_ClientConnect, false);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientCommand, gameclients, this, &RCBotPluginMeta::Hook_ClientCommand, false);
	
	//SH_REMOVE_MANUALHOOK(MHook_PlayerRunCmd, player_vtable, SH_STATIC(Hook_Function2), false);

	// if another instance is running dont run through this
	//if ( !bInitialised )
	//	return;
	
	CBots::freeAllMemory();
	CStrings::freeAllMemory();
	CBotMods::freeMemory();
	CAccessClients::freeMemory();
	CBotEvents::freeMemory();
	CWaypoints::freeMemory();
	CWaypointTypes::freeMemory();
	CBotProfiles::deleteProfiles();
	CWeapons::freeMemory();
	CBotMenuList::freeMemory();
	//unloadSignatures();

	//UnhookPlayerRunCommand();
	//UnhookGiveNamedItem();

	//ConVar_Unregister();

	//if ( gameevents )
	//	gameevents->RemoveListener(this);

#if SOURCE_ENGINE > SE_DARKMESSIAH
	ConVar_Unregister( );
#endif

	return true;
}

void RCBotPluginMeta::OnVSPListening(IServerPluginCallbacks *iface)
{
	vsp_callbacks = iface;
}

void RCBotPluginMeta::Hook_ServerActivate(edict_t *pEdictList, int edictCount, int clientMax)
{
	META_LOG(g_PLAPI, "ServerActivate() called: edictCount = %d, clientMax = %d", edictCount, clientMax);

	CAccessClients::load();

	CBotGlobals::setClientMax(clientMax);
}

void RCBotPluginMeta::AllPluginsLoaded()
{
	/* This is where we'd do stuff that relies on the mod or other plugins 
	 * being initialized (for example, cvars added and events registered).
	 */
}

void RCBotPluginMeta::Hook_ClientActive(edict_t *pEntity, bool bLoadGame)
{
	META_LOG(g_PLAPI, "Hook_ClientActive(%d, %d)", IndexOfEdict(pEntity), bLoadGame);

	CClients::clientActive(pEntity);
}

#if SOURCE_ENGINE >= SE_ORANGEBOX
void RCBotPluginMeta::Hook_ClientCommand(edict_t *pEntity, const CCommand &args)
#else
void RCBotPluginMeta::Hook_ClientCommand(edict_t *pEntity)
#endif
{
	static CBotMod *pMod = nullptr;

#if SOURCE_ENGINE <= SE_DARKMESSIAH
	CCommand args;
#endif

	const char *pcmd = args.Arg(0);

	if (!pEntity || pEntity->IsFree())
	{
		return;
	}

	CClient *pClient = CClients::get(pEntity);

	// is bot command?
	if ( CBotGlobals::m_pCommands->isCommand(pcmd) )
	{		
		//eBotCommandResult iResult = CBotGlobals::m_pCommands->execute(pClient,engine->Cmd_Argv(1),engine->Cmd_Argv(2),engine->Cmd_Argv(3),engine->Cmd_Argv(4),engine->Cmd_Argv(5),engine->Cmd_Argv(6));
		eBotCommandResult iResult = CBotGlobals::m_pCommands->execute(pClient,args.Arg(1),args.Arg(2),args.Arg(3),args.Arg(4),args.Arg(5),args.Arg(6));

		if ( iResult == COMMAND_ACCESSED )
		{
			// ok
		}
		else if ( iResult == COMMAND_REQUIRE_ACCESS )
		{
			CBotGlobals::botMessage(pEntity,0,"You do not have access to this command");
		}
		else if ( iResult == COMMAND_NOT_FOUND )
		{
			CBotGlobals::botMessage(pEntity,0,"bot command not found");	
		}
		else if ( iResult == COMMAND_ERROR )
		{
			CBotGlobals::botMessage(pEntity,0,"bot command returned an error");	
		}

		RETURN_META(MRES_SUPERCEDE);
	}
	if ( std::strncmp(pcmd,"menuselect",10) == 0 ) // menu command
	{
		if ( pClient->isUsingMenu() )
		{
			int iCommand = std::atoi(args.Arg(1));

			// format is 1.2.3.4.5.6.7.8.9.0
			if ( iCommand == 0 )
				iCommand = 9;
			else
				iCommand --;

			pClient->getCurrentMenu()->selectedMenu(pClient,iCommand);
		}
	}

	// command capturing
	pMod = CBotGlobals::getCurrentMod();

	// capture some client commands e.g. voice commands
	pMod->clientCommand(pEntity,args.ArgC(),pcmd,args.Arg(1),args.Arg(2));

	RETURN_META(MRES_IGNORED); 
}

bool RCBotPluginMeta::Hook_ClientConnect(edict_t *pEntity,
									const char *pszName,
									const char *pszAddress,
									char *reject,
									int maxrejectlen)
{
	META_LOG(g_PLAPI, "Hook_ClientConnect(%d, \"%s\", \"%s\")", IndexOfEdict(pEntity), pszName, pszAddress);

	CClients::init(pEntity);

	return true;
}

void RCBotPluginMeta::Hook_ClientPutInServer(edict_t *pEntity, char const *playername)
{
	CBaseEntity *pEnt = servergameents->EdictToBaseEntity(pEntity);
	const bool is_Rcbot = false;

	CClient *pClient = CClients::clientConnected(pEntity);

	if ( !is_Rcbot && pClient )
	{
		if ( !engine->IsDedicatedServer() )
		{
			if ( CClients::noListenServerClient() )
			{
				// give listenserver client all access to bot commands
				CClients::setListenServerClient(pClient);
				pClient->setAccessLevel(CMD_ACCESS_ALL);
				pClient->resetMenuCommands();
			}
		}
	}

	CBotMod *pMod = CBotGlobals::getCurrentMod();

	pMod->playerSpawned(pEntity);

#ifdef OVERRIDE_RUNCMD
	if ( pEnt )
	{
		SH_ADD_MANUALHOOK_MEMFUNC(MHook_PlayerRunCmd, pEnt, this, &RCBotPluginMeta::Hook_PlayerRunCmd, false);
	}
#endif
}

void RCBotPluginMeta::Hook_ClientDisconnect(edict_t *pEntity)
{
	CBaseEntity *pEnt = servergameents->EdictToBaseEntity(pEntity);

#ifdef OVERRIDE_RUNCMD
	if ( pEnt )
	{
		SH_REMOVE_MANUALHOOK_MEMFUNC(MHook_PlayerRunCmd, pEnt, this, &RCBotPluginMeta::Hook_PlayerRunCmd, false);
	}
#endif

	CClients::clientDisconnected(pEntity);

	META_LOG(g_PLAPI, "Hook_ClientDisconnect(%d)", IndexOfEdict(pEntity));
}

void RCBotPluginMeta::Hook_GameFrame(bool simulating)
{
	/**
	 * simulating:
	 * ***********
	 * true  | game is ticking
	 * false | game is not ticking
	 */

	if ( simulating && CBotGlobals::IsMapRunning() )
	{
		static CBotMod *currentmod;

		CBots::botThink();
		CClients::clientThink();

		if ( CWaypoints::getVisiblity()->needToWorkVisibility() )
		{
			CWaypoints::getVisiblity()->workVisibility();
		}

		// Profiling
#ifdef _DEBUG
		if ( CClients::clientsDebugging(BOT_DEBUG_PROFILE) )
		{
			CProfileTimers::updateAndDisplay();
		}
#endif

		// Config Commands
		CBotConfigFile::doNextCommand();
		currentmod = CBotGlobals::getCurrentMod();

		currentmod->modFrame();

		// Bot Quota
		if (rcbot_bot_quota_interval.GetInt() > 0) {
			BotQuotaCheck();
		}
	}
}

void RCBotPluginMeta::BotQuotaCheck() {
	// this is configured with config/bot_quota.ini
	if (rcbot_bot_quota_interval.GetInt() <= 0) {
		return;
	}

	if (m_fBotQuotaTimer < 1.0f) {
		m_fBotQuotaTimer = engine->Time() + 10.0f; // Sleep 10 seconds
	}

	if (m_fBotQuotaTimer < engine->Time() - rcbot_bot_quota_interval.GetInt()) {
		m_fBotQuotaTimer = engine->Time();

		// Target Bot Count
		int bot_target = 0; //not used? [APG]RoboCop[CL]

		// Change Notification
		bool notify = false;

		// Current Bot Count
		int bot_count = 0;
		int human_count = 0;

		// Count Players
		for (int i = 0; i < RCBOT_MAXPLAYERS; ++i) {
			const CClient* client = CClients::get(i);
			const CBot* bot = CBots::get(i);

			if (bot != nullptr && bot->getEdict() != nullptr && bot->inUse()) {
				IPlayerInfo *p = playerinfomanager->GetPlayerInfo(bot->getEdict());

				if (p->IsConnected() && p->IsFakeClient() && !p->IsHLTV()) {
					bot_count++;
				}
			}

			if (client != nullptr && client->getPlayer() != nullptr && client->isUsed()) {
				IPlayerInfo *p = playerinfomanager->GetPlayerInfo(client->getPlayer());

				if (p->IsConnected() && !p->IsFakeClient() && !p->IsHLTV()) {
					human_count++;
				}
			}
		}

		if (human_count >= RCBOT_MAXPLAYERS) {
			human_count = 0;
		}

		// Get Bot Quota
		bot_target = m_iTargetBots[human_count];

		// Change Bot Quota
		if (bot_count > bot_target) {
			CBots::kickRandomBot(bot_count - bot_target);
			notify = true;
		} else if (bot_target > bot_count) {
			const int bot_diff = bot_target - bot_count;

			for (int i = 0; i < bot_diff; ++i) {
				CBots::createBot("", "", "");
				break; // Bug-Fix, only add one bot at a time
			}

			notify = true;
		}

		if (notify) {
			char chatmsg[128];
			snprintf(chatmsg, sizeof(chatmsg), "[Bot Quota] Humans: %d, Bots: %d", human_count, bot_target);
			logger->Log(LogLevel::INFO, chatmsg);
			// RCBotPluginMeta::BroadcastTextMessage(chatmsg);
		}
	}
}

bool RCBotPluginMeta::Hook_LevelInit(const char *pMapName,
								char const *pMapEntities,
								char const *pOldLevel,
								char const *pLandmarkName,
								bool loadGame,
								bool background)
{
	META_LOG(g_PLAPI, "Hook_LevelInit(%s)", pMapName);

		//CClients::initall();
	// Must set this
	CBotGlobals::setMapName(pMapName);

	logger->Log(LogLevel::INFO, "Level \"%s\" has been loaded", pMapName);

	CWaypoints::precacheWaypointTexture();

	CWaypointDistances::reset();

	CProfileTimers::reset();

	CWaypoints::init();
	CWaypoints::load();

	CBotGlobals::setMapRunning(true);
	CBotConfigFile::reset();

#if SOURCE_ENGINE <= SE_DARKMESSIAH
	if (mp_teamplay != nullptr)
		CBotGlobals::setTeamplay(mp_teamplay->GetBool());
	else
		CBotGlobals::setTeamplay(false);
#else
	if ( mp_teamplay.IsValid() )
		CBotGlobals::setTeamplay(mp_teamplay.GetBool());
	else
		CBotGlobals::setTeamplay(false);
#endif

	CBotEvents::setupEvents();

	CBots::mapInit();

	CBotMod *pMod = CBotGlobals::getCurrentMod();
	
	if ( pMod )
		pMod->mapInit();

	CBotSquads::FreeMemory();

	CClients::setListenServerClient(nullptr);

	// Setup game rules
	extern void **g_pGameRules;

	if (g_pGameRules_Obj && g_pGameRules_Obj->found())
	{
		g_pGameRules = g_pGameRules_Obj->getGameRules();
	}
	else if (g_pGameRules_Create_Obj && g_pGameRules_Create_Obj->found())
	{
		g_pGameRules = g_pGameRules_Create_Obj->getGameRules();
	}

	return true;
}

void RCBotPluginMeta::Hook_LevelShutdown()
{
	META_LOG(g_PLAPI, "Hook_LevelShutdown()");

	CClients::initall();
	CWaypointDistances::save();

	CBots::freeMapMemory();	
	CWaypoints::init();

	CBotGlobals::setMapRunning(false);
	CBotEvents::freeMemory();
}

bool RCBotPluginMeta::Pause(char *error, size_t maxlen)
{
	return true;
}

bool RCBotPluginMeta::Unpause(char *error, size_t maxlen)
{
	return true;
}

const char *RCBotPluginMeta::GetLicense()
{
	return "Affero GPL 3.0";
}

const char *RCBotPluginMeta::GetVersion()
{
	return "0.1";
}

const char *RCBotPluginMeta::GetDate()
{
	return __DATE__;
}

const char *RCBotPluginMeta::GetLogTag()
{
	return "DIRTBOT";
}

const char *RCBotPluginMeta::GetAuthor()
{
	return "cheeseh, APGRoboCop, tschumann";
}

const char *RCBotPluginMeta::GetDescription()
{
	return "Bot for HL2DM, TF2 and DOD:S";
}

const char *RCBotPluginMeta::GetName()
{
	return "Dirtbot";
}

const char *RCBotPluginMeta::GetURL()
{
	return "http://www.teamsandpit.com/";
}
