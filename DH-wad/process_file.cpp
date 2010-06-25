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
	2010/06/24 - Altered interface to allow using different file name and
		lump name.
*/

#include <fstream>
#include <string>

#include "Lump.hpp"
#include "main.hpp"



bool process_file(std::string const & nameDir, std::string const & nameFile, std::string const & nameLump_, bool addAlways)
{
	std::string nameLump;

	if (nameLump_.empty())
	{
		// Strips file extension.
		nameLump = nameFile.substr(0, nameFile.find_first_of('.'));
	}
	else
	{
		nameLump = nameLump_;
	}

	if (nameFile.empty())
	{
		if (addAlways)
		{
			lump_list.push_back(Lump("", nameLump));
			return true;
		}

		return false;
	}


	std::ifstream lumpStream((nameDir + nameFile).c_str(), std::ios_base::in | std::ios_base::binary | std::ios_base::ate);

	if (!lumpStream)
	{
		if (addAlways)
		{
			lump_list.push_back(Lump("", nameLump));
			return true;
		}

		return false;
	}

	size_t lumpSize = lumpStream.tellg();
	lumpStream.seekg(0);

	std::string lumpData;
	lumpData.reserve(lumpSize);

	char lumpChar;
	while (lumpStream.get(lumpChar))
		lumpData += lumpChar;

	lump_list.push_back(Lump(lumpData, nameLump));

	lumpStream.close();

	return true;
}



