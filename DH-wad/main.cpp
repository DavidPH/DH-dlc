/*
    Copyright 2009, 2010 David Hill

    This file is part of DH-wad.

    DH-wad is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DH-wad is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DH-wad.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
	2010/02/03 - Update for new process_options.h.
	2010/05/08 - When determining the default output, don't just check
		option_arg. Must also check option_directory and option_map.
*/

#include <fstream>
#include <iostream>
#include <list>

#include "Lump.hpp"
#include "main.hpp"
#include "options.hpp"
#include "process_directory.hpp"
#include "process_file.hpp"
#include "process_map.hpp"
#include "../common/foreach.hpp"
#include "../common/IO.hpp"



#ifdef TARGET_OS_WIN32
#define PATHSEP '\\'
#else
#define PATHSEP '/'
#endif



std::list<Lump> lump_list;



uint32_t get_lump_list_length()
{
	uint32_t length = 0;

	FOREACH_T(std::list<Lump>, it, lump_list)
		length+= it->length();

	return length;
}



void usage()
{
	std::cerr <<
		"usage: DH-wad [--output] OUTPUT [SOURCE]\n"
		"       DH-wad --map SOURCE OUTPUT\n"
		"\n"
		"If --output option is not used, default depends on remaining arguments.\n"
		"If more than one argument, the first is used for target file.\n"
		"Otherwise, if the argument ends in .wad, then source defaults to the argument\n"
		"  stripped of the extension.\n"
		"If only one argument and it does not end with .ddl, it is used for the source.\n"
		"In this last case, the output file is assumed to be the argument with the .wad\n"
		"  extension added.\n"
		"\n"
		"Options:\n"
		"  -h, --help     displays this text and exits\n"
		"      --version  displays version and exits\n"
		"\n"
		"  -d, --directory  processes the specified directory\n"
		"  -e, --explicit   processes arguments as an explicit list of files\n"
		"      --iwad       outputs an IWAD instead of a PWAD\n"
		"  -m, --map        processes the specified directory as a map\n"
		"  -o, --output     sets the output directory\n"
		"  -u, --unwad      writes out directory instead of a wad\n"
		"  -w, --wad        read the specified file as a wad file and add its lumps\n"
	;
}

void version()
{
	std::cerr << "DH-wad 1.0\n";
}



int main(int argc, char** argv)
{
	PROCESS_OPTIONS();

	if (option_arg.size() == 0 && option_map.size() == 0 && option_directory.size() == 0 && option_wad.size() == 0)
	{
		usage();
		return 0;
	}

	if (option_output_default)
	{
		if ((option_arg.size() > 1) || (option_arg.size() == 1 && (!option_directory_default || !option_map_default)))
		{
			option_output = option_arg[0];
			option_arg[0].clear();
		}
		else if (option_arg.size() == 1)
		{
			size_t lastSep = option_arg[0].find_first_of('.');

			if (lastSep == std::string::npos)
			{
				option_output = option_arg[0] + ".wad";
			}
			else
			{
				option_output = option_arg[0];
				option_arg[0] = option_arg[0].substr(0, lastSep);
			}
		}
		else
		{
			usage();
			return 0;
		}
	}

	if (option_explicit)
	{
		for (size_t index = 0; index+1 < option_arg.size(); index += 2)
		{
			std::string const & nameFile = option_arg[index+0];
			std::string const & nameLump = option_arg[index+1];

			process_file("./", nameFile, nameLump, true);
		}
	}
	else
	{
		FOREACH_T(string_multi_opt_t, it, option_arg)
		{
			if (it->empty()) continue;

			if ((*it)[it->size()-1] != PATHSEP)
				(*it) += PATHSEP;

			process_directory(it->c_str());
		}

		FOREACH_T(string_multi_opt_t, it, option_directory)
		{
			if (it->empty()) continue;

			if ((*it)[it->size()-1] != PATHSEP)
				(*it) += PATHSEP;

			process_directory(it->c_str(), false);
		}

		FOREACH_T(string_multi_opt_t, it, option_map)
		{
			if (it->empty()) continue;

			// Actually needs to NOT have the trailing path seperator...
			if ((*it)[it->size()-1] == PATHSEP)
				(*it) = it->substr(0, it->size()-1);

			std::string base;
			std::string name(*it);

			size_t lastSep = it->find_last_of(PATHSEP);

			if (lastSep != std::string::npos)
			{
				// Must include the seperator.
				base = it->substr(0, lastSep+1);
				name = it->substr(lastSep+1);
			}

			process_map(base.c_str(), name.c_str());
		}

		FOREACH_T(string_multi_opt_t, it, option_wad)
		{
			if (it->empty()) continue;

			process_file_wad(*it);
		}
	}

	if (option_unwad)
	{
		std::string dirBase(option_output);

		if ((dirBase.size() != 0) && (dirBase[dirBase.size()-1] != PATHSEP))
			dirBase += PATHSEP;

		std::string dirSub;
		bool binaryMapHack(false);

		FOREACH_T(std::list<Lump>, lumpIt, lump_list)
		{
			std::string lumpData(lumpIt->getData());
			std::string lumpName(lumpIt->getName());

			if (dirSub.empty())
			{
				     if (lumpName == "A_START")
					dirSub = lumpName + PATHSEP;

				else if (lumpName == "F_START")
					dirSub = lumpName + PATHSEP;

				else if (lumpName == "FF_START")
					dirSub = lumpName + PATHSEP;

				else if (lumpName == "HI_START")
					dirSub = lumpName + PATHSEP;

				else if (lumpName == "P_START")
					dirSub = lumpName + PATHSEP;

				else if (lumpName == "S_START")
					dirSub = lumpName + PATHSEP;

				else if (lumpName == "SS_START")
					dirSub = lumpName + PATHSEP;

				else if (lumpName == "TX_START")
					dirSub = lumpName + PATHSEP;

				else if (lumpName == "VX_START")
					dirSub = lumpName + PATHSEP;

				else
				{
					std::list<Lump>::iterator lumpIt2 = lumpIt;
					++lumpIt2;

					if (lumpIt2 == lump_list.end())
						;

					else if (lumpIt2->getName() == "TEXTMAP")
					{
						binaryMapHack = false;
						dirSub = lumpName + PATHSEP;
					}
					else if (lumpIt2->getName() == "THINGS")
					{
						binaryMapHack = true;
						dirSub = lumpName + PATHSEP;
					}
				}
			}

			IO::mkdir(dirBase+dirSub, true);

			if (!lumpData.empty())
			{
				std::ofstream lumpStream((dirBase+dirSub+lumpName).c_str(), std::ios_base::out | std::ios_base::binary);

				if (!lumpStream)
				{
					std::cerr << "Unable to open:" << (dirBase+dirSub+lumpName) << '\n';

					return 1;
				}

				lumpStream << lumpData;

				lumpStream.close();
			}

			if (!dirSub.empty())
			{
				     if (lumpName == "A_END")
					dirSub.clear();

				else if (lumpName == "F_END")
					dirSub.clear();

				else if (lumpName == "FF_END")
					dirSub.clear();

				else if (lumpName == "HI_END")
					dirSub.clear();

				else if (lumpName == "P_END")
					dirSub.clear();

				else if (lumpName == "S_END")
					dirSub.clear();

				else if (lumpName == "SS_END")
					dirSub.clear();

				else if (lumpName == "TX_END")
					dirSub.clear();

				else if (lumpName == "VX_END")
					dirSub.clear();

				else if (lumpName == "ENDMAP")
					dirSub.clear();

				else if (binaryMapHack)
				{
					std::list<Lump>::iterator lumpIt2 = lumpIt;
					std::string lumpName2((++lumpIt2)->getName());

					if ((binaryMapHack == 1) &&
						(lumpName2 != "THINGS")   &&
						(lumpName2 != "LINEDEFS") &&
						(lumpName2 != "SIDEDEFS") &&
						(lumpName2 != "VERTEXES") &&
						(lumpName2 != "SEGS")     &&
						(lumpName2 != "SSECTORS") &&
						(lumpName2 != "NODES")    &&
						(lumpName2 != "SECTORS")  &&
						(lumpName2 != "REJECT")   &&
						(lumpName2 != "BLOCKMAP") &&
						(lumpName2 != "BEHAVIOR") &&
						(lumpName2 != "SCRIPTS")  &&
						(lumpName2 != "DIALOGUE")
					)
					{
						binaryMapHack = false;
						dirSub.clear();
					}
				}
			}
		}
	}
	else
	{
		std::ofstream fileWAD(option_output.c_str(), std::ios_base::out | std::ios_base::binary);

		fileWAD << (option_iwad ? "IWAD" : "PWAD");

		// lump count
		uint32_t lumpCount = lump_list.size();

		fileWAD << char((lumpCount >>  0) & 0xFF);
		fileWAD << char((lumpCount >>  8) & 0xFF);
		fileWAD << char((lumpCount >> 16) & 0xFF);
		fileWAD << char((lumpCount >> 24) & 0xFF);

		// first lump header index
		uint32_t lumpHeadIndex = get_lump_list_length()+12;

		fileWAD << char((lumpHeadIndex >>  0) & 0xFF);
		fileWAD << char((lumpHeadIndex >>  8) & 0xFF);
		fileWAD << char((lumpHeadIndex >> 16) & 0xFF);
		fileWAD << char((lumpHeadIndex >> 24) & 0xFF);

		FOREACH_T(std::list<Lump>, it, lump_list)
			fileWAD << it->encodeBody();

		FOREACH_T(std::list<Lump>, it, lump_list)
			fileWAD << it->encodeHead();
	}
}



