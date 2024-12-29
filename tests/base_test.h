//========= Copyright © 2008-2024, Team Sandpit, All rights reserved. ============
//
// Purpose: Base test stuff
//
// $NoKeywords: $
//================================================================================

#ifndef _BASE_TEST_H
#define _BASE_TEST_H

#pragma push_macro("and")
#pragma push_macro("or")

// TODO: something in gtest sets "and" and "or" (maybe some compatability thing for && and or?) which breaks inline assembly in the Source SDK
#include <gtest/gtest.h>

#pragma pop_macro("and")
#pragma pop_macro("or")

#endif // _BASE_TEST_H