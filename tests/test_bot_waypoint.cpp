#include <gtest/gtest.h>

// TODO: something in gtest sets these macros which breaks inline assembly in the Source SDK
#undef and
#undef or

#include "bot_waypoint.h"
#include "bot_wpt_color.h"

TEST(CWaypointType, getBits) {
    CWaypointType waypointType = CWaypointType(0, "", 0, WptColor(), 0, 0);

    EXPECT_EQ(0, waypointType.getBits());
}