// Cross_Platform_Lua_Test.cpp : Defines the entry point for the application.
//

#include "Cross_Platform_Lua_Test.h"

// Utility functions to distinguish between Lua and C/C++ console outputs
namespace ConsoleUtility
{
	enum Lang
	{
		C,
		Lua
	};

	template<typename T, typename U>
	void Display(int lang, const T& var, const U& var2)
	{
		if (lang == C)
		{
			std::cout << "[ C ]: " << var << " " << var2 << '\n';
		}
		else
		{
			std::cout << "[ LUA ]: " << var << " " << var2 << '\n';
		}
	}

	template<typename T>
	void Display(int lang, const T& var)
	{
		if (lang == C)
		{
			std::cout << "[ C ]: " << var << '\n';
		}
		else
		{
			std::cout << "[ LUA ]: " << var << '\n';
		}
	}
}

using namespace ConsoleUtility;

std::ofstream CreateFile(std::string path)
{
	std::ofstream file(path);
	file.close();

	return file;
}

void WriteFile(std::ofstream& file, std::string filePath, std::string toWrite)
{
	// Check if file is open for writing
	if (!file.is_open())
	{
		// Open file in order to write to it
		file.open(filePath);
	}
	file << toWrite << std::endl;
	file.close();
}

// Creates a file and optionally writes to it.
// If the directory for said file is missing, it'll create one and put the file inside it.
std::ofstream CreateFileWithDirectory(std::filesystem::path directoryPath, std::string filePath, std::string write = "")
{
	std::ofstream file{};

	// Check if directory does not exist
	if (!std::filesystem::exists(directoryPath))
	{
		Display(C, "Failed to find directory", directoryPath.string());

		// Create a new directory, and any missing directories leading to our directory
		std::filesystem::create_directories(directoryPath);
		Display(C, "Created directory: ", directoryPath);

		// Create file
		file = CreateFile(filePath);

		// Write text into it
		WriteFile(file, filePath, write);
	}
	else
	{
		// Check if file exists
		if (!std::filesystem::exists(filePath))
		{
			Display(C, "File missing: ", filePath);

			file = CreateFile(filePath);

			WriteFile(file, filePath, write);
		}
	}

	return file;
}

int main()
{
	Display(C, "Hello CMake.");
	Print();

	
	// Setup lua interpreter

	lua_State* L = luaL_newstate();


	// Load common lua libraries

	luaL_openlibs(L);


	// Create any missing lua files, along with their directory if missing

	CreateFileWithDirectory("./scripts", "./scripts/TestLua.lua", "a = 5 + math.random(0, 5)");
	CreateFileWithDirectory("./scripts/autorun", "./scripts/autorun/PrintLua.lua", "print('Hello from autorun lua!')");
	CreateFileWithDirectory("./scripts/autorun", "./scripts/autorun/PrintLua2.lua", "print('Hello from autorun lua2!')");


	// Iterate through autorun directory and attempt to run every .lua file that is found

	for (const auto& dirEntry : std::filesystem::recursive_directory_iterator("./scripts/autorun/"))
	{
		std::filesystem::path filePath = dirEntry.path();
		Display(C, "Running lua script: ", filePath.string());


		// continue iterating the directory if the file is missing the .lua file extension

		if (!(filePath.extension() == ".lua"))
		{
			Display(C, filePath.string(), " is not a lua file");
			continue;
		}


		// Attempt tu run the lua script

		if (!luaL_dofile(L, filePath.string().c_str()))
		{
			// extract variables, etc
		}
		else
		{
			Display(C, "Error running lua script");

			// Raise an error if the lua file is integral to our application
			//luaL_error(L, lua_tostring(L, -1));
		}
	}


	// attempt to run the given script

	if (!luaL_dofile(L, "./scripts/TestLua.lua"))
	{
		Display(C, "Executed Lua script");


		// push the desired variable to the top of the stack

		lua_getglobal(L, "a");


		// check if the variable we pushed to the top of the stack is a number(we expect it to be a number)

		if (lua_isnumber(L, -1))
		{

			// if it is a number, then store it as a floating point in C(a number in lua is a float)

			float a = (float)lua_tonumber(L, -1);
			Display(Lua, "variable is ", a);
		}
		else
		{
			Display(C, "Lua variable is not a number");
		}
	}
	else
	{
		Display(C, "Error running lua script");

		// Raise an error, as the lua file is integral to our application
		//luaL_error(L, lua_tostring(L, -1));
	}



	getchar();
	lua_close(L);
	return 0;
}
