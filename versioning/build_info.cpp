#include "build_info.h"

// generated with versioning/generate.py, AMBuild handles this
#include <build_version_auto.h>

const char* build_info::authors = "tschumann";
const char* build_info::url = "https://www.teamsandpit.com/";

const char* build_info::long_version = "alpha-" GIT_COMMIT_HASH;
const char* build_info::short_version = "alpha-" GIT_COMMIT_SHORT_HASH;

const char* build_info::date = __DATE__;