//========= Copyright © 2008-2024, Team Sandpit, All rights reserved. ============
//
// Purpose: Bot utils tests
//
// $NoKeywords: $
//================================================================================

#include "base_test.h"
#include "bot.h"
#include "utils.h"

TEST(rcbot2utils, IsValidEdict)
{
    EXPECT_EQ(false, rcbot2utils::IsValidEdict(nullptr));
}