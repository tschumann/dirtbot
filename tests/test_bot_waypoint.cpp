//========= Copyright © 2008-2024, Team Sandpit, All rights reserved. ============
//
// Purpose: Bot waypoint tests
//
// $NoKeywords: $
//================================================================================

#include <gtest/gtest.h>

#include "base_test.h"
#include "bot_waypoint.h"
#include "bot_wpt_color.h"

TEST(CWaypointType, getBits) {
    CWaypointType waypointType = CWaypointType(0, "", 0, WptColor(), 0, 0);

    EXPECT_EQ(0, waypointType.getBits());
}