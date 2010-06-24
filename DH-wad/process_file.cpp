/*
    Copyright 2009 David Hill

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

#include <fstream>
#include <string>

#include "Lump.hpp"
#include "main.hpp"



bool process_file(const std::string& dirname, const std::string& filename, bool addEmpty)
{
	std::ifstream lumpStream((dirname + filename).c_str(), std::ios_base::in | std::ios_base::binary | std::ios_base::ate);

	if (!lumpStream)
	{
		if (addEmpty)
		{
			lump_list.push_back(Lump("", filename));
			return true;
		}

		return false;
	}

	// Strips file extension.
	std::string lumpName = filename.substr(0, filename.find_first_of('.'));

	size_t lumpSize = lumpStream.tellg();
	lumpStream.seekg(0);

	std::string lumpData;
	lumpData.reserve(lumpSize);

	char lumpChar;
	while (lumpStream.get(lumpChar))
		lumpData += lumpChar;

	lump_list.push_back(Lump(lumpData, lumpName));

	lumpStream.close();

	return true;
}



