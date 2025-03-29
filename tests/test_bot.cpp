//========= Copyright © 2008-2024, Team Sandpit, All rights reserved. ============
//
// Purpose: Bot tests
//
// $NoKeywords: $
//================================================================================

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

TEST(MathUtils, minimum) {
	EXPECT_EQ(1, MathUtils::minimum(1, 2));
	EXPECT_EQ(1, MathUtils::minimum(2, 1));
}

TEST(MathUtils, maximum) {
	EXPECT_EQ(2, MathUtils::maximum(1, 2));
	EXPECT_EQ(2, MathUtils::maximum(2, 1));
}