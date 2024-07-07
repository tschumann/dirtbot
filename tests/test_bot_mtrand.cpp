//========= Copyright © 2008-2024, Team Sandpit, All rights reserved. ============
//
// Purpose: Bot mtrand tests
//
// $NoKeywords: $
//================================================================================

#include <gtest/gtest.h>

#include "base_test.h"
#include "bot_waypoint.h"
#include "bot_wpt_color.h"

TEST(mtrand, randomInt) {
    EXPECT_EQ(0, randomInt(0, 0));
}