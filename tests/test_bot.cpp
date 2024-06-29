#include <gtest/gtest.h>

// TODO: something in gtest sets these macros which breaks inline assembly in the Source SDK
#undef and
#undef or

#include "bot.h"
#include "outsourced.h"

TEST(CBot, kill) {
	helpers = new outsourced::MockServerPluginHelpers();
	engine = new outsourced::MockEngine();
	
    CBot *pBot = new CBot();
	pBot->kill();

    EXPECT_TRUE(pBot != nullptr);

	delete engine;
	delete helpers;
	delete pBot;
}