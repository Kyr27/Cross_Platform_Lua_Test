# CMakeList.txt : CMake project for Cross_Platform_Lua_Test, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Cross_Platform_Lua_Test "Cross_Platform_Lua_Test.cpp" "Cross_Platform_Lua_Test.h" "printJunk.cpp" "printJunk.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Cross_Platform_Lua_Test PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
# Make sure your .dll is in the same directory as your .exe on Windows(Linux doesnt have this issue)

# Best method, figures out the file extension on its own and sets variables for use
set (Lua "${CMAKE_SOURCE_DIR}/libs/Lua")	# Set variable Lua, to be the path to the library
find_library(LuaLib							# Sets the library path if it finds the library its looking for
	NAMES lua54	liblua54.a					# Set the names of the library its looking for, in this case lua54, and liblua54.a(for linux, explicitly stating that we want the static library as per official lua recommendations)
	PATHS ${Lua})							# Set possible paths the library can be found in

if (LuaLib)
	include_directories(Cross_Platform_Lua_Test ${Lua}/include/)
	target_link_libraries(Cross_Platform_Lua_Test PUBLIC "${LuaLib}")
	#target_link_libraries(Cross_Platform_Lua_Test ${CMAKE_SOURCE_DIR}/libs/Lua/lua54.lib) # good 2nd method
else()
	message(FATAL_ERROR, "Lua library not found")
endif()