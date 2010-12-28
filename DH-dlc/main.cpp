/*
    Copyright 2009, 2010 David Hill

    This file is part of DH-dlc.

    DH-dlc is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DH-dlc is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DH-dlc.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
	2010/02/03 - Update for new process_options.h.
	2010/05/05 - Update limits() to account for *_MAX and *_MIN defines
		always being defined.
*/

#include "main.hpp"

#include "global_object.hpp"
#include "math.hpp"
#include "options.hpp"
#include "process_file.hpp"
#include "process_stream.hpp"
#include "scripts.hpp"
#include "SourceStream.hpp"
#include "types.hpp"

#include "exceptions/CompilerException.hpp"

#include "types/int_t.hpp"
#include "types/real_t.hpp"

#include "../common/foreach.hpp"
#include "../common/IO.hpp"
#include "../common/process_options.h"

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>



void limits()
{
	#define NO_LIMIT "NO LIMIT"

	#define PRINT_LIMIT_INT(TYPE) \
	if (TYPE##_t_MAX != 0) \
		std::cerr << #TYPE "_t_MAX  = " << TYPE##_t_MAX << '\n'; \
	else \
		std::cerr << #TYPE "_t_MAX  = " << NO_LIMIT << '\n'; \
	\
	if (TYPE##_t_MIN != 0) \
		std::cerr << #TYPE "_t_MIN  = " << TYPE##_t_MIN << '\n'; \
	else \
		std::cerr << #TYPE "_t_MIN  = " << NO_LIMIT << '\n'; \
	\
	if (u##TYPE##_t_MAX != 0) \
		std::cerr << "u" #TYPE "_t_MAX  = " << u##TYPE##_t_MAX << '\n'; \
	else \
		std::cerr << "u" #TYPE "_t_MAX  = " << NO_LIMIT << '\n';

	#define PRINT_LIMIT_REAL(TYPE) \
	if (TYPE##_t_MAX != 0) \
		std::cerr << #TYPE "_t_MAX  = " << TYPE##_t_MAX << '\n'; \
	else \
		std::cerr << #TYPE "_t_MAX  = " << NO_LIMIT << '\n'; \
	\
	if (TYPE##_t_MIN != 0) \
		std::cerr << #TYPE "_t_MIN  = " << TYPE##_t_MIN << '\n'; \
	else \
		std::cerr << #TYPE "_t_MIN  = " << NO_LIMIT << '\n';

	PRINT_LIMIT_INT(int_s)
	PRINT_LIMIT_INT(int)
	PRINT_LIMIT_INT(int_l)

	std::cerr << '\n';

	PRINT_LIMIT_REAL(real_s)
	PRINT_LIMIT_REAL(real)
	PRINT_LIMIT_REAL(real_l)
}

void usage()
{
	std::cerr <<
		"usage: DH-dlc [OPTION [...]] [--directory] TARGET [SOURCE [...]]\n"
		"\n"
		"If --directory option is not used, default depends on remaining arguments.\n"
		"If more than one argument, the first is used for target directory.\n"
		"Otherwise, if the argument ends in .ddl or .dhlx, then directory defaults to the\n"
		"  argument stripped of the extension.\n"
		"If only one argument and it does not end with .ddl, it is used for directory.\n"
		"In this last case, the source file is assumed to be the argument with the .ddl\n"
		"  extension added.\n"
		"\n"
		"The Library options (lib-*) are additive (except where noted).\n"
		"lib-udmf-strict will replace lib-udmf if both are enabled.\n"
		"lib-usdf-strict will replace lib-usdf if both are enabled.\n"
		"\n"
		"The Output options (output-*) are mutually exclusive.\n"
		"If more than one is selected, the first listed here is used.\n"
		"output-extradata and output-usdf are additive.\n"
		"\n"
		"Options:\n"
		"  -h, --help     displays this text and exits\n"
		"      --limits   displays value limits and exits\n"
		"      --version  displays version and exits\n"
		"\n"
		"  -d, --directory      sets the output directory\n"
		"  -e, --error-limit    sets the number of errors that can occur before\n"
		"                       terminating\n"
		"      --do-extensions  makes output files have extensions\n"
		"  -i, --include        adds to the list of directories to search for files in\n"
		"  -m, --map-name       sets the map name\n"
		#if USE_GMPLIB
		"  -p, --precision      sets the precision for floats in bits [default: " << (POINTER_BIT*2) << "]\n"
		#else
		"  -p, --precision      sets the precision for floats in bits [default: " << (POINTER_BIT) << "]\n"
		#endif
		"\n"
		"Scripts:\n"
		"      --script-acs        sets the output name for ACS scripts\n"
		"                          [default: SCRIPTS]\n"
		"      --script-extradata  sets the output name for ExtraData scripts\n"
		"                          [default: EXTRADAT]\n"
		"\n"
		"Libraries:\n"
		"      --no-lib-std          do not automatically include lib-std.ddl\n"
		"      --no-lib-udmf         do not automatically include lib-udmf.ddl\n"
		"      --do-lib-udmf-strict  automatically include lib-udmf-strict.ddl\n"
		"      --no-lib-usdf         automatically include lib-usdf.ddl\n"
		"      --do-lib-usdf-strict  automatically include lib-usdf-strict.ddl\n"
		"\n"
		"Input:\n"
		"  -C, --no-case-sensitive  reads source files as case insensitive\n"
		"\n"
		"Output:\n"
		"      --no-output-any        disables any output\n"
		"      --do-output-extradata  output ExtraData files\n"
		"      --do-output-usdf       output USDF files\n"
		"      --do-output-hexen      output files in Hexen format\n"
		"      --do-output-strife     output files in Strife format\n"
		"      --do-output-heretic    output files in Heretic format\n"
		"      --do-output-doom       output files in Doom format\n"
		"      --do-output-udmf       output files in UDMF format [default]\n"
		"\n"
		"Debugging:\n"
		"      --debug        enables debugging messages\n"
		"      --debug-dump   prints every object at the end of program\n"
		"                     WARNING: will go into an infinite loop if an object\n"
		"                     references itself, directly or otherwise\n"
		"      --debug-token  prints every token read.\n"
	;
}

void version()
{
	std::cerr << "DH-dlc 1.0\n";
}

int main(int argc, char** argv)
{
	clock_t clock_start(clock());
	clock_t clock_total(0);

	PROCESS_OPTIONS();

	if (option_arg.size() == 0)
	{
		usage();

		return 0;
	}

	if (option_arg.size() == 1 && option_arg[0] == "-")
	{
		if (option_error_limit_default) option_error_limit = 0;
		if (option_output_any_default)  option_output_any  = false;

		if (option_directory_default)
		{
			option_directory_default = false;
			option_directory         = "./";
		}
	}



	set_precision();

	if (option_directory_default)
	{
		if (option_arg.size() == 1)
		{
			if (option_arg[0].size() >= 4 && option_arg[0].compare(option_arg[0].size()-4, 4, ".ddl", 4) == 0)
			{
				option_directory = option_arg[0].substr(0, option_arg[0].size()-4);
			}
			if (option_arg[0].size() >= 5 && option_arg[0].compare(option_arg[0].size()-5, 5, ".dhlx", 5) == 0)
			{
				option_directory = option_arg[0].substr(0, option_arg[0].size()-5);
			}
			else
			{
				option_directory = option_arg[0];
				option_arg[0] += ".ddl";
			}
		}
		else
		{
			option_directory = option_arg[0];
			option_arg[0].clear();
		}
	}

	if (!option_directory.empty() && option_directory[option_directory.size()-1] != PATH_SEP)
		option_directory += PATH_SEP;

	if (option_map_name_default)
	{
		size_t lastSep = option_directory.find_last_of(PATH_SEP, option_directory.size()-2);

		option_map_name = option_directory.substr(lastSep+1, (option_directory.size() - lastSep) - 2);
	}

	if (getenv("DH_DLC_PATH"))
	{
		char* token = strtok(getenv("DH_DLC_PATH"), ":");

		if (token)
		{
			option_include.push_back(token);

			while ((token = strtok(NULL, ":")) != NULL)
				option_include.push_back(token);
		}
	}

	FOREACH_T(std::vector<std::string>, it, option_include)
	{
		if (!it->empty() && (*it)[it->size()-1] != PATH_SEP)
			*it += PATH_SEP;
	}

	if (option_seed_default)
	{
		std::ifstream ifs("/dev/urandom");

		if (ifs)
		{
			for (size_t index = sizeof(option_seed); index; --index)
			{
				option_seed <<= CHAR_BIT;
				option_seed += ifs.get();
			}

			option_seed_default = false;
		}
	}
	if (option_seed_default)
	{
		std::ifstream ifs("/dev/random");

		if (ifs)
		{
			for (size_t index = sizeof(option_seed); index; --index)
			{
				option_seed <<= CHAR_BIT;
				option_seed += ifs.get();
			}

			option_seed_default = false;
		}
	}
	if (option_seed_default)
	{
		option_seed = time(NULL);

		option_seed_default = false;
	}



	srand(option_seed);
	#if USE_GMPLIB
	random_source.seed(option_seed);
	#endif



	if (option_debug_seed)
		std::cerr << "seed:" << option_seed << ";\n";



	clock_t clock_options((clock() - clock_start) - clock_total);
	clock_total += clock_options;
	if (option_debug_time)
	{
		std::cerr.precision(8);
		std::cerr << "Toptions = " << (clock_options / double(CLOCKS_PER_SEC)) << ";\n";
	}



	if (option_lib_std)
		process_file("lib-std.ddl");

	if (option_lib_udmf_strict)
		process_file("lib-udmf-strict.ddl");
	else if (option_lib_udmf)
		process_file("lib-udmf.ddl");

	if (option_lib_usdf_strict)
		process_file("lib-usdf-strict.ddl");
	else if (option_lib_usdf)
		process_file("lib-usdf.ddl");

	FOREACH_T(std::vector<std::string>, it, option_arg)
	{
		size_t lastSep = it->find_last_of(PATH_SEP);

		// This ensures that files can always be included from the same directory.
		if (lastSep != std::string::npos)
			option_include.push_back(it->substr(0, lastSep+1));

		if (*it == "-")
		{
			SourceStream ss(std::cin);
			process_stream<SourceTokenDDL>(ss, "stdin");
		}
		else
			process_file(*it);
	}



	clock_t clock_compile((clock() - clock_start) - clock_total);
	clock_total += clock_compile;
	if (option_debug_time)
	{
		std::cerr.precision(8);
		std::cerr << "Tcompile = " << (clock_compile / double(CLOCKS_PER_SEC)) << ";\n";
	}



	if (option_debug_dump)
	{
		std::cerr << "global=";
		global_object->printOn(std::cerr);

		FOREACH_T(global_object_map_t, mapIt, global_object_map)
		{
			FOREACH_T(global_object_list_t, listIt, mapIt->second)
			{
				(*listIt)->printOn(std::cerr);
			}
		}
	}



	if (!option_output_any)
	{
		clock_t clock_output((clock() - clock_start) - clock_total);
		clock_total += clock_output;

		if (option_debug_time)
		{
			std::cerr.precision(8);
			std::cerr << "Toutput  = " << (clock_output  / double(CLOCKS_PER_SEC)) << ";\n";
			std::cerr << "Ttotal   = " << (clock_total   / double(CLOCKS_PER_SEC)) << ";\n";
		}

		return 0;
	}

	#define OPENFILE(NAME, EXT) \
	std::string name##NAME(#NAME); \
	if (option_use_file_extensions) \
		name##NAME += EXT; \
	\
	std::ofstream file##NAME( \
		(option_directory + name##NAME).c_str(), \
		std::ios_base::out | std::ios_base::binary); \
	\
	if (!file##NAME) \
	{ \
		std::cerr << "unable to open:" << name##NAME << "\n"; \
		exit(1); \
	}

	IO::mkdir(option_directory, true);

	if (option_output_hexen)
	{
		OPENFILE(LINEDEFS, ".lmp");
		OPENFILE(SECTORS,  ".lmp");
		OPENFILE(SIDEDEFS, ".lmp");
		OPENFILE(THINGS,   ".lmp");
		OPENFILE(VERTEXES, ".lmp");

		try
		{
			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_linedef()])
				(*it)->encodeHexen(fileLINEDEFS);

			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_sector()])
				(*it)->encodeHexen(fileSECTORS);

			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_sidedef()])
				(*it)->encodeHexen(fileSIDEDEFS);

			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_thing()])
				(*it)->encodeHexen(fileTHINGS);

			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_vertex()])
				(*it)->encodeHexen(fileVERTEXES);
		}
		catch (CompilerException & e)
		{
			std::cerr << e << '\n';
		}

		fileLINEDEFS.close();
		fileSECTORS.close();
		fileSIDEDEFS.close();
		fileTHINGS.close();
		fileVERTEXES.close();

		// ZDoom requires a BEHAVIOR lump to signify a Hexen map.
		// TODO: Make this an option.
		std::string nameBEHAVIOR("BEHAVIOR");
		if (option_use_file_extensions)
			nameBEHAVIOR += ".o";

		// Opened as append so as to not delete existing content, if any.
		std::ofstream fileBEHAVIOR((option_directory + nameBEHAVIOR).c_str(), std::ios_base::app);

		fileBEHAVIOR.close();
	}
	else if (option_output_strife)
	{
		OPENFILE(LINEDEFS, ".lmp");
		OPENFILE(SECTORS,  ".lmp");
		OPENFILE(SIDEDEFS, ".lmp");
		OPENFILE(THINGS,   ".lmp");
		OPENFILE(VERTEXES, ".lmp");

		try
		{
			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_linedef()])
				(*it)->encodeStrife(fileLINEDEFS);

			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_sector()])
				(*it)->encodeStrife(fileSECTORS);

			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_sidedef()])
				(*it)->encodeStrife(fileSIDEDEFS);

			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_thing()])
				(*it)->encodeStrife(fileTHINGS);

			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_vertex()])
				(*it)->encodeStrife(fileVERTEXES);
		}
		catch (CompilerException & e)
		{
			std::cerr << e << '\n';
		}

		fileLINEDEFS.close();
		fileSECTORS.close();
		fileSIDEDEFS.close();
		fileTHINGS.close();
		fileVERTEXES.close();
	}
	else if (option_output_heretic)
	{
		OPENFILE(LINEDEFS, ".lmp");
		OPENFILE(SECTORS,  ".lmp");
		OPENFILE(SIDEDEFS, ".lmp");
		OPENFILE(THINGS,   ".lmp");
		OPENFILE(VERTEXES, ".lmp");

		try
		{
			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_linedef()])
				(*it)->encodeHeretic(fileLINEDEFS);

			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_sector()])
				(*it)->encodeHeretic(fileSECTORS);

			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_sidedef()])
				(*it)->encodeHeretic(fileSIDEDEFS);

			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_thing()])
				(*it)->encodeHeretic(fileTHINGS);

			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_vertex()])
				(*it)->encodeHeretic(fileVERTEXES);
		}
		catch (CompilerException & e)
		{
			std::cerr << e << '\n';
		}

		fileLINEDEFS.close();
		fileSECTORS.close();
		fileSIDEDEFS.close();
		fileTHINGS.close();
		fileVERTEXES.close();
	}
	else if (option_output_doom)
	{
		OPENFILE(LINEDEFS, ".lmp");
		OPENFILE(SECTORS,  ".lmp");
		OPENFILE(SIDEDEFS, ".lmp");
		OPENFILE(THINGS,   ".lmp");
		OPENFILE(VERTEXES, ".lmp");

		try
		{
			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_linedef()])
				(*it)->encodeDoom(fileLINEDEFS);

			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_sector()])
				(*it)->encodeDoom(fileSECTORS);

			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_sidedef()])
				(*it)->encodeDoom(fileSIDEDEFS);

			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_thing()])
				(*it)->encodeDoom(fileTHINGS);

			FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_vertex()])
				(*it)->encodeDoom(fileVERTEXES);
		}
		catch (CompilerException & e)
		{
			std::cerr << e << '\n';
		}

		fileLINEDEFS.close();
		fileSECTORS.close();
		fileSIDEDEFS.close();
		fileTHINGS.close();
		fileVERTEXES.close();
	}
	else if (option_output_udmf)
	{
		OPENFILE(TEXTMAP, ".txt");

		fileTEXTMAP << "/* Compiled by DH-dlc. */\n\n";

		// TODO make a command for this
		if (global_object->hasObject(name_t("namespace")))
		{
			obj_t namespaceObj = global_object->getObject(name_t("namespace"));

			if (namespaceObj->getType() == type_t::type_string())
				fileTEXTMAP << "namespace = "; namespaceObj->encodeUDMF(fileTEXTMAP, 1); fileTEXTMAP << ";\n\n";
		}

		try
		{
			FOREACH_T(global_object_map_t, mapIt, global_object_map)
			{
				FOREACH_T(global_object_list_t, listIt, mapIt->second)
					(*listIt)->encodeUDMF(fileTEXTMAP);
			}
		}
		catch (CompilerException & e)
		{
			std::cerr << e << '\n';
		}

		fileTEXTMAP.close();
	}

	if (option_output_usdf)
	{
		OPENFILE(DIALOG, ".txt");

		fileDIALOG << "/* Compiled by DH-dlc. */\n\n";

		// TODO make a command for this
		if (global_object->hasObject(name_t("namespace")))
		{
			obj_t namespaceObj = global_object->getObject(name_t("namespace"));

			if (namespaceObj->getType() == type_t::type_string())
				fileDIALOG << "namespace = "; namespaceObj->encodeUSDF(fileDIALOG, 1); fileDIALOG << ";\n\n";
		}

		try
		{
			FOREACH_T(global_object_map_t, mapIt, global_object_map)
			{
				FOREACH_T(global_object_list_t, listIt, mapIt->second)
					(*listIt)->encodeUSDF(fileDIALOG);
			}
		}
		catch (CompilerException & e)
		{
			std::cerr << e << '\n';
		}

		fileDIALOG.close();
	}

	#undef OPENFILE

	if (option_output_extradata)
	{
		std::string nameExtraData(option_script_extradata);

		std::ofstream fileExtraData((option_directory + nameExtraData).c_str());

		if (!fileExtraData)
		{
			std::cerr << "unable to open:" << nameExtraData << '\n';
			return 1;
		}

		FOREACH_T(global_object_map_t, mapIt, global_object_map)
		{
			FOREACH_T(global_object_list_t, listIt, mapIt->second)
			{
				obj_t thisObj(*listIt);

				try
				{
					thisObj->encodeExtraData(fileExtraData);
				}
				catch (CompilerException& e)
				{
					std::cerr << thisObj->getType().makeString() << ':' << get_object_index(thisObj) << ':' << e << '\n';
				}
			}
		}

		fileExtraData.close();
	}

	FOREACH_T(scripts_data_type, it, scripts_data)
	{
		std::string nameSCRIPT = it->first;
		if (option_use_file_extensions && nameSCRIPT.find('.') == std::string::npos)
			nameSCRIPT += ".txt";

		std::ofstream fileSCRIPT((option_directory + it->first).c_str());

		if (!fileSCRIPT)
		{
			std::cerr << "unable to write script:\n  " << it->first << '\n';
			continue;
		}

		fileSCRIPT << it->second;

		fileSCRIPT.close();
	}



	clock_t clock_output((clock() - clock_start) - clock_total);
	clock_total += clock_output;
	if (option_debug_time)
	{
		std::cerr.precision(8);
		std::cerr << "Toutput  = " << (clock_output  / double(CLOCKS_PER_SEC)) << ";\n";
	}



	if (option_debug_time)
	{
		std::cerr.precision(8);
		std::cerr << "Ttotal   = " << (clock_total   / double(CLOCKS_PER_SEC)) << ";\n";
	}



	return 0;
}



