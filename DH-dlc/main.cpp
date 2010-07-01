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

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

#include "global_object.hpp"
#include "LevelObject.hpp"
#include "LevelObjectName.hpp"
#include "lo_types.hpp"
#include "options.hpp"
#include "process_file.hpp"
#include "process_stream.hpp"
#include "scripts.hpp"
#include "SourceStream.hpp"
#include "types.hpp"
#include "../common/foreach.hpp"
#include "../common/process_options.h"
#include "exceptions/CompilerException.hpp"
#include "types/int_t.hpp"
#include "types/real_t.hpp"
#include "types/string_t.hpp"



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
		"Otherwise, if the argument ends in .ddl, then directory defaults to the argument\n"
		"  stripped of the extension.\n"
		"If only one argument and it does not end with .ddl, it is used for directory.\n"
		"In this last case, the source file is assumed to be the argument with the .ddl\n"
		"  extension added.\n"
		"\n"
		"The Library options (lib-*) are additive (except where noted).\n"
		"lib-udmf-strict will replace lib-udmf if both are enabled.\n"
		"\n"
		"The Output options (output-*) are mutually exclusive.\n"
		"If more than one is selected, the first listed here is used.\n"
		"Except for output-extradata which is additive for certain outputs.\n"
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
		"\n"
		"Input:\n"
		"  -C, --no-case-sensitive  reads source files as case insensitive\n"
		"\n"
		"Output:\n"
		"      --no-output-any        disables any output\n"
		"      --do-output-extradata  output files in ExtraData format\n"
		"      --do-output-hexen      output files in Hexen format\n"
		"      --do-output-strife     output files in Strife format\n"
		"      --do-output-heretic    output files in Heretic format\n"
		"      --do-output-doom       output files in Doom format\n"
		"      --do-output-udmf       output files in UDMF format [default]\n"
		"\n"
		"Debugging:\n"
		"      --debug       enables debugging messages\n"
		"      --debug-dump  prints every object at the end of program\n"
		"                    WARNING: will go into an infinite loop if an object\n"
		"                    references itself, directly or otherwise\n"
	;
}

void version()
{
	std::cerr << "DH-dlc 1.0\n";
}

int main(int argc, char** argv)
{
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



	if (option_lib_std)
		process_file("lib-std.ddl");

	if (option_lib_udmf_strict)
		process_file("lib-udmf-strict.ddl");
	else if (option_lib_udmf)
		process_file("lib-udmf.ddl");

	FOREACH_T(std::vector<std::string>, it, option_arg)
	{
		size_t lastSep = it->find_last_of(PATH_SEP);

		// This ensures that files can always be included from the same directory.
		if (lastSep != std::string::npos)
			option_include.push_back(it->substr(0, lastSep+1));

		if (*it == "-")
		{
			SourceStream ss(std::cin);
			process_stream(ss, "stdin");
		}
		else
			process_file(*it);
	}



	if (option_debug_dump)
	{
		std::cerr << "global=";
		global_object->printOn(std::cerr);

		FOREACH_T(std::list<obj_t>, it, global_object_list)
			(*it)->printOn(std::cerr);
	}



	if (!option_output_any) return 0;

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

	if (option_output_heretic || option_output_hexen || option_output_strife)
	{
		OPENFILE(LINEDEFS, ".lmp");
		OPENFILE(SECTORS,  ".lmp");
		OPENFILE(SIDEDEFS, ".lmp");
		OPENFILE(THINGS,   ".lmp");
		OPENFILE(VERTEXES, ".lmp");

		std::ofstream fileExtraData;

		if (option_output_extradata)
		{
			std::string nameExtraData(option_script_extradata);

			fileExtraData.open((option_directory + nameExtraData).c_str());
		}

		FOREACH_T(std::list<obj_t>, it, global_object_list)
		{
			obj_t thisObj(*it);
			std::string thisType(get_lo_type_redirect(thisObj->getType()));

			try
			{
				if (thisType == type_name_linedef())
				{
					fileLINEDEFS << thisObj->encode(LINEDEF);
				}
				else if (thisType == type_name_sector())
				{
					fileSECTORS  << thisObj->encode(SECTOR);
				}
				else if (thisType == type_name_sidedef())
				{
					fileSIDEDEFS << thisObj->encode(SIDEDEF);
				}
				else if (thisType == type_name_thing())
				{
					fileTHINGS   << thisObj->encode(THING);
				}
				else if (thisType == type_name_vertex())
				{
					fileVERTEXES << thisObj->encode(VERTEX);
				}

				if (option_output_extradata)
				{
					thisObj->encodeExtraData(fileExtraData);
				}
			}
			catch (CompilerException& e)
			{
				std::cerr << thisType << ':' << get_object_index(thisObj) << ':' << e << '\n';
			}
		}

		fileLINEDEFS.close();
		fileSECTORS.close();
		fileSIDEDEFS.close();
		fileTHINGS.close();
		fileVERTEXES.close();
		fileExtraData.close();

		// ZDoom requires a BEHAVIOR lump to signify a Hexen map.
		// TODO: Make this an option.
		if (option_output_hexen)
		{
			std::string nameBEHAVIOR("BEHAVIOR");
			if (option_use_file_extensions)
				nameBEHAVIOR += ".o";

			// Opened as append so as to not delete existing content, if any.
			std::ofstream fileBEHAVIOR(
				(option_directory + nameBEHAVIOR).c_str(),
				std::ios_base::app);

			fileBEHAVIOR.close();
		}
	}
	else if (option_output_doom)
	{
		OPENFILE(LINEDEFS, ".lmp");
		OPENFILE(SECTORS,  ".lmp");
		OPENFILE(SIDEDEFS, ".lmp");
		OPENFILE(THINGS,   ".lmp");
		OPENFILE(VERTEXES, ".lmp");

		FOREACH_T(std::list<obj_t>, it, global_object_list)
		{
			obj_t thisObj(*it);
			std::string thisType(get_lo_type_redirect(thisObj->getType()));

			try
			{
				if (thisType == type_name_linedef())
					thisObj->encodeDoom(fileLINEDEFS);

				else if (thisType == type_name_sector())
					thisObj->encodeDoom(fileSECTORS);

				else if (thisType == type_name_sidedef())
					thisObj->encodeDoom(fileSIDEDEFS);

				else if (thisType == type_name_thing())
					thisObj->encodeDoom(fileTHINGS);

				else if (thisType == type_name_vertex())
					thisObj->encodeDoom(fileVERTEXES);
			}
			catch (CompilerException& e)
			{
				std::cerr << thisType << ':' << get_object_index(thisObj) << ':' << e << '\n';
			}
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

		fileTEXTMAP << "// Compiled by DH-dlc.\n\n";

		// TODO make a command for this
		if (global_object->hasObject(name_t("namespace")))
		{
			obj_t namespaceObj = global_object->getObject(name_t("namespace"));

			if (namespaceObj->getType() == type_name_string())
				fileTEXTMAP << "namespace = " << namespaceObj->encode(false) << ";\n\n";
		}

		FOREACH_T(std::list<obj_t>, it, global_object_list)
		{
			std::string encString((*it)->encode());

			if (!encString.empty())
				fileTEXTMAP << encString << "\n\n";
		}
	}

	#undef OPENFILE

	if (option_output_extradata)
	{

		std::ofstream fileExtraData;

		if (option_output_extradata)
		{
			std::string nameExtraData(option_script_extradata);

			fileExtraData.open((option_directory + nameExtraData).c_str());
		}

		FOREACH_T(std::list<obj_t>, it, global_object_list)
		{
			obj_t thisObj(*it);
			std::string thisType(get_lo_type_redirect(thisObj->getType()));

			try
			{
				thisObj->encodeExtraData(fileExtraData);
			}
			catch (CompilerException& e)
			{
				std::cerr << thisType << ':' << get_object_index(thisObj) << ':' << e << '\n';
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

	return 0;
}



