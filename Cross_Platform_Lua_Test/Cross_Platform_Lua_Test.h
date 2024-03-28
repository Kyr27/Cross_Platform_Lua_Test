// Cross_Platform_Lua_Test.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

// TODO: Reference additional headers your program requires here.
#include "printJunk.h"
#include "lua.hpp"	// these include errors messed with me for way too long, only to realize they were coming from another config-target(x64-debug, x86-debug), and not the current one x64-release...

#include <filesystem>
#include <fstream>