//========= Copyright © 2008-2024, Team Sandpit, All rights reserved. ============
//
// Purpose: Bot tests
//
// $NoKeywords: $
//================================================================================

#include <gtest/gtest.h>
#include <memory>

#include "base_test.h"
#include "bot.h"
#include "outsourced.h"

TEST(CBot, kill) {
	helpers = new outsourced::FakeServerPluginHelpers();
	engine = new outsourced::FakeVEngineServer();
	
    std::unique_ptr<CBot> pBot = std::make_unique<CBot>();
	pBot->kill();

	EXPECT_STREQ("kill\n", outsourced::gEngine.clientCommands.back().c_str());

	delete engine;
	delete helpers;
}