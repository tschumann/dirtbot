#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base_test.h"
#include "build_info.h"

// TODO: these are here because adding versioning/build_info.cpp to the build causes an error because it's generated by another ambuild step
const char* build_info::authors = "Cheeseh, RoboCop, nosoop, caxanga334";
const char* build_info::url = "http://rcbot.bots-united.com/";
const char* build_info::long_version = "APGRoboCop/rcbot2@";
const char* build_info::short_version = "1.7-beta2 (apg-nosoop-caxanga334)-";
const char* build_info::date = __DATE__;

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}