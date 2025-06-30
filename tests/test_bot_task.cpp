//========= Copyright © 2008-2024, Team Sandpit, All rights reserved. ============
//
// Purpose: Bot task tests
//
// $NoKeywords: $
//================================================================================

#include <cmath>

#include "base_test.h"
#include "bot.h"
#include "bot_task.h"

TEST(CBotTask, getGrenadeAngle)
{
    float fa1 = 0.0;
	float fa2 = 0.0;

	getGrenadeAngle(0.0, 0.0, 0.0, 0.0, &fa1, &fa2);

    EXPECT_TRUE(isnan(fa1));
	EXPECT_TRUE(isnan(fa2));
}