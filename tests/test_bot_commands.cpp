//========= Copyright © 2008-2024, Team Sandpit, All rights reserved. ============
//
// Purpose: Bot command tests
//
// $NoKeywords: $
//================================================================================

#include "base_test.h"
#include "bot.h"
#include "bot_globals.h"
#include "bot_commands.h"
#include "outsourced.h"

TEST(CBotSubcommands, printCommand) {
	engine = new outsourced::FakeVEngineServer();

    CBotGlobals::m_pCommands->printCommand(engine->CreateEdict(), 0);

	// it's quite verbose so don't bother checking the whole thing
	EXPECT_EQ("[RCBot] [rcbot]", outsourced::gEngine.clientPrintf.substr(0, 15));
}

TEST(CBotSubcommands, isContainer) {
    EXPECT_EQ(true, CBotGlobals::m_pCommands->isContainer());
}