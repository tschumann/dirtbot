//========= Copyright © 2008-2024, Team Sandpit, All rights reserved. ============
//
// Purpose: Bot mtrand tests
//
// $NoKeywords: $
//================================================================================

#include "base_test.h"
#include "bot_mtrand.h"

TEST(mtrand, randomInt)
{
    EXPECT_EQ(0, randomInt(0, 0));
}