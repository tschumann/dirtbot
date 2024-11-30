/*
 *    This file is part of RCBot.
 *
 *    RCBot by Paul Murphy adapted from Botman's HPB Bot 2 template.
 *
 *    RCBot is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    RCBot is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RCBot; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */
#ifndef __DOD_RCBOT_H__
#define __DOD_RCBOT_H__

enum : std::uint8_t
{
	TEAM_ALLIES = 2,
	TEAM_AXIS = 3
};

#define MAX_GREN_THROW_DIST 1024.0f

#define SMOKE_RADIUS 150.0f
#define DOD_BOMB_EXPLODE_IMMINENT_TIME 7.0f

class CBroadcastBombEvent : public IBotFunction
{
public:
	CBroadcastBombEvent ( int iEvent, int iCP, int iTeam ) 
	{ 
		m_iEvent = iEvent; m_iCP = iCP; m_iTeam = iTeam; 
	}

	void execute (CBot *pBot) override;
private:
	int m_iCP;
	int m_iTeam;
	int m_iEvent;
};

typedef enum : std::uint8_t
{
	DOD_VC_GOGOGO = 0,
	DOD_VC_YES = 1,
	DOD_VC_DROPWEAP = 2,
    DOD_VC_HOLD = 3,
	DOD_VC_NO = 4,
	DOD_VC_DISPLACE = 5,
    DOD_VC_GO_LEFT = 6,
    DOD_VC_NEED_BACKUP = 7,
	DOD_VC_MGAHEAD = 8,
    DOD_VC_GO_RIGHT = 9,
    DOD_VC_FIRE_IN_THE_HOLE =10,
    DOD_VC_ENEMY_BEHIND = 11,
    DOD_VC_STICK_TOGETHER = 12,
	DOD_VC_USE_GRENADE = 13,
	DOD_VC_ENEMY_DOWN = 14,
    DOD_VC_COVERING_FIRE = 15,
    DOD_VC_SNIPER = 16,
    DOD_VC_NEED_MG = 17,
    DOD_VC_SMOKE = 18,
    DOD_VC_NICE_SHOT = 19,
    DOD_VC_NEED_AMMO = 20,
    DOD_VC_GRENADE2 = 21,
    DOD_VC_THANKS = 22,
    DOD_VC_USE_BAZOOKA = 23,
	DOD_VC_CEASEFIRE = 24,
	DOD_VC_AREA_CLEAR = 25,
	DOD_VC_BAZOOKA = 26,
	DOD_VC_INVALID = 27
}eDODVoiceCMD;

typedef struct
{
	eDODVoiceCMD id;
	const char *pcmd;
}eDODVoiceCommand_t;

typedef enum : std::uint8_t
{
	DOD_CLASS_RIFLEMAN = 1,
	DOD_CLASS_ASSAULT = 2,
	DOD_CLASS_SUPPORT = 3,
	DOD_CLASS_SNIPER = 4,
	DOD_CLASS_MACHINEGUNNER = 5,
	DOD_CLASS_ROCKET = 6
}DOD_Class;

enum : std::uint8_t
{
	DOD_BOMB_STATE_UNAVAILABLE = 0,
	DOD_BOMB_STATE_AVAILABLE = 1,
	DOD_BOMB_STATE_ACTIVE = 2
};

enum : std::uint8_t
{
	DOD_BOMB_EXPLODED = 0,
	DOD_BOMB_DEFUSE = 1,
	DOD_BOMB_PLANT = 2,
	DOD_BOMB_PATH_PLANT = 3,
	DOD_BOMB_PATH_DEFUSE = 4,
	DOD_POINT_CAPTURED = 5
};

#define DOD_CLASSNAME_CONTROLPOINT "dod_control_point"
#define DOD_CLASSNAME_BOMBTARGET "dod_bomb_target"

typedef struct
{
	float fLastTime;
	float fProb;
	bool bVisible;
	bool bInSmoke;
}smoke_t;

// bot for DOD
class CDODBot : public CBot
{
public:

	CDODBot();

	bool isDOD () override { return true; }

	bool withinTeammate ( ) const;

	bool hasBomb () const { return m_bHasBomb; }
	void removeBomb () { m_bHasBomb = false; }
	void bombEvent ( int iEvent, int iCP, int iTeam );

	void friendlyFire ( edict_t *pEdict ) override;

	void modThink () override;

	void init (bool bVarInit=false) override;
	void setup () override;

	void freeMapMemory () override;

	//Vector getAimVector ( edict_t *pEntity );
	void modAim ( edict_t *pEntity, Vector &v_origin, 
	              Vector *v_desired_offset, Vector &v_size,
	              float fDist, float fDist2D) override;

	bool startGame () override;

	void died ( edict_t *pKiller, const char *pszWeapon ) override;
	void killed ( edict_t *pVictim, char *weapon ) override;

	void spawnInit () override;

	float getEnemyFactor ( edict_t *pEnemy ) override;

	bool isEnemy ( edict_t *pEdict,bool bCheckWeapons = true ) override;

	float getArmorPercent () const { return 0.01f * static_cast<float>(m_pPlayerInfo->GetArmorValue()); }

	void getTasks (unsigned int iIgnore) override;

	bool executeAction ( CBotUtility *util );

	void updateConditions () override;

	void selectedClass ( int iClass );

	bool setVisible ( edict_t *pEntity, bool bVisible ) override;

	bool select_CWeapon ( CWeapon *pWeapon ) override;

	bool selectBotWeapon ( CBotWeapon *pBotWeapon ) override;

	bool canGotoWaypoint (Vector vPrevWaypoint, CWaypoint *pWaypoint, CWaypoint *pPrev = nullptr) override;

	void defending () override;

	bool handleAttack ( CBotWeapon *pWeapon, edict_t *pEnemy ) override;

	void hearVoiceCommand ( edict_t *pPlayer, byte cmd ) override;

	void handleWeapons () override;

	void reachedCoverSpot (int flags) override;

	void touchedWpt ( CWaypoint *pWaypoint, int iNextWaypoint = -1, int iPrevWaypoint = -1 ) override;

	bool checkStuck () override;

	bool hasMG () const;
	CBotWeapon *getMG() const;
	CBotWeapon *getSniperRifle () const;
	bool hasSniperRifle () const;

	void voiceCommand (int cmd) override;

	unsigned int maxEntityIndex ( ) override { return gpGlobals->maxEntities; }

	void seeFriendlyDie ( edict_t *pDied, edict_t *pKiller, CWeapon *pWeapon ) override;
	void seeFriendlyKill ( edict_t *pTeamMate, edict_t *pDied, CWeapon *pWeapon ) override;

	bool isVisibleThroughSmoke ( edict_t *pSmoke, edict_t *pCheck );

	void grenadeThrown () override { addVoiceCommand(DOD_VC_FIRE_IN_THE_HOLE); }

	void chooseClass( bool bIsChangingClass ); // updates m_iDesiredClass
	void changeClass(); // uses m_iDesiredClass

	void setNearestBomb ( edict_t *pBomb ) { m_pNearestBomb = pBomb; }
	void prone ();
	void unProne();

	bool wantToListenToPlayerAttack ( edict_t *pPlayer, int iWeaponID = -1) override;

	bool walkingTowardsWaypoint ( CWaypoint *pWaypoint, bool *bOffsetApplied, Vector &vOffset ) override;

	void listenForPlayers () override;

	void signal ( const char *signal ) const;

	void sayInPosition () override;

	void sayMoveOut () override;

	void areaClear() override;

	void dropAmmo ();

private:

	int m_iSelectedClass;

	MyEHandle m_pCurrentWeapon;

	CBaseHandle *m_Weapons;

	float m_fFixWeaponTime;
	float m_flSprintTime;

	float m_flStamina;
	bool m_bProne;

	int m_iClip1;
	int m_iClip2;

	int m_iTeam; // either 2 / 3 TEAM_ALLIES/TEAM_AXIS
	int m_iEnemyTeam; // is the opposite of m_iTeam to check for enemy things

	MyEHandle m_pNearestFlag;
	MyEHandle m_pGoalFlag;

	DOD_Class m_iClass;
	float m_fShootTime;
	float m_fZoomOrDeployTime;

	float m_fProneTime;

	// EHandles cos they will be destroyed soon
	MyEHandle m_pEnemyRocket;
	float m_fShoutRocket;
	MyEHandle m_pEnemyGrenade;
	float m_fShoutGrenade;
	MyEHandle m_pOwnGrenade;
	MyEHandle m_pNearestSmokeToEnemy;

	float m_fChangeClassTime;
	bool m_bCheckClass;
	bool m_bHasBomb;

	smoke_t m_CheckSmoke[RCBOT_MAXPLAYERS];

	float m_fDeployMachineGunTime;
	MyEHandle m_pNearestBomb; // "capture" bomb
	MyEHandle m_pNearestPathBomb; // blocking path bomb
	MyEHandle m_pNearestBreakable;
	MyEHandle m_pNearestWeapon;

	CPerceptron *m_pWantToProne;

	float m_fLastRunForCover;

	eDODVoiceCMD m_LastHearVoiceCommand;

	float m_fLastCaptureEvent;
	float m_fNextCheckNeedAmmo;
	float m_fNextCheckAlone; // to see if bot should tell others to stick together

	bool m_bDroppedAmmoThisRound;

	MyEHandle m_pNearestDeadTeamMate;
};

#endif