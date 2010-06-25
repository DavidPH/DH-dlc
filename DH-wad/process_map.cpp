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

*/

#include "process_directory.hpp"
#include "process_file.hpp"
#include "process_map.hpp"



#define PROCESS_SPECIAL(TAG, NAME) \
if (lumpName == #TAG || lumpName == #TAG "_START" || lumpName == NAME) \
{ \
	\
	if (!process_file(dirnameBase + dirname + '/', #TAG "_START.txt", "")) \
	if (!process_file(dirnameBase + dirname + '/', #TAG "_START.lmp", "")) \
	process_file(dirnameBase + dirname + '/', #TAG "_START", "", true); \
	\
	process_directory(dirnameBase + dirname + '/', false); \
	\
	if (!process_file(dirnameBase + dirname + '/', #TAG "_END.txt", "")) \
	if (!process_file(dirnameBase + dirname + '/', #TAG "_END.lmp", "")) \
	process_file(dirnameBase + dirname + '/', #TAG "_END", "", true); \
	\
	return; \
} \
else (void) 0



void process_map(const std::string& dirnameBase, const std::string& dirname)
{
	const static char* mapLumps[] =
	{
		// TODO do check such that extension is ignored (maybe).

		// Must be in this order to load right.

		"TEXTMAP",  "TEXTMAP.txt",   // O
		"THINGS",   "THINGS.lmp",    // *
		"LINEDEFS", "LINEDEFS.lmp",  // *
		"SIDEDEFS", "SIDEDEFS.lmp",  // *
		"VERTEXES", "VERTEXES.lmp",  // *
		"SEGS",     "SEGS.lmp",      // N
		"SSECTORS", "SSECTORS.lmp",  // N
		"NODES",    "NODES.lmp",     // N
		"SECTORS",  "SECTORS.lmp",   // *
		"REJECT",   "REJECT.lmp",    // O
		"BLOCKMAP", "BLOCKMAP.lmp",  // O
		"BEHAVIOR", "BEHAVIOR.o",    // O
		"SCRIPTS",  "SCRIPTS.acs",   // O
		NULL

		// * required
		// N node-related
		// O other
	};

	// Strips file extension.
	std::string lumpName = dirname.substr(0, dirname.find_first_of('.'));

	PROCESS_SPECIAL(A,  "ACS");
	PROCESS_SPECIAL(FF, "flats");
	PROCESS_SPECIAL(HI, "hires");
	PROCESS_SPECIAL(SS, "sprites");
	PROCESS_SPECIAL(TX, "textures");

	if (!process_file(dirnameBase + dirname + '/', lumpName+".txt", ""))
		if (!process_file(dirnameBase + dirname + '/', lumpName+".lmp", ""))
			process_file(dirnameBase + dirname + '/', lumpName, "", true);

	for (int index = 0; mapLumps[index] != NULL; ++index)
		process_file(dirnameBase + dirname + '/', mapLumps[index], "");

	if (!process_file(dirnameBase + dirname + '/', "ENDMAP.txt", ""))
		if (!process_file(dirnameBase + dirname + '/', "ENDMAP.lmp", ""))
			process_file(dirnameBase + dirname + '/', "ENDMAP", "", true);
}



