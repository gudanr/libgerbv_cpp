// libgerbv.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <string>
#include "gerbv.h"
#include "main.h"
#include "config.h"

namespace fs = std::filesystem;


int main(int argc, char* argv[])
{
	const std::string settings_schema_env("GSETTINGS_SCHEMA_DIR");

	/* On Windows executable can be not in bin/ dir */
	const std::string settings_schema_fallback_dir("share/glib-2.0/schemas" G_SEARCHPATH_SEPARATOR_S "../share/glib-2.0/schemas");

	std::string env_val;

	mainProject = gerbv_create_project();
	mainProject->execname = _strdup(argv[0]);
	mainProject->execpath = g_path_get_dirname(argv[0]);

	/* Add "fallback" directory with settings schema file from this
	 * executable path */
	if (NULL == getenv(settings_schema_env.c_str())) {
		/* Empty env var */
		env_val = settings_schema_env;
	}
	else {
		/* Not empty env var */
		env_val = getenv(settings_schema_env.c_str());
	}

	env_val += G_SEARCHPATH_SEPARATOR_S;
	env_val += mainProject->execpath;
	env_val += G_DIR_SEPARATOR_S;
	env_val += settings_schema_fallback_dir;

	int env_result = _putenv(env_val.c_str());

	const char* sample_proj = ".\\bin\\example\\sample_proj.gvp";
	const char* gerber_file = ".\\bin\\example\\cslk\\cslk.gbx";

    main_open_project_from_filename(mainProject, (char*)sample_proj);

	gerbv_open_layer_from_filename(mainProject, (char*)gerber_file);

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
