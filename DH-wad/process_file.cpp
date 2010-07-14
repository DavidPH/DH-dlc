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

#include "process_file.hpp"

#include "Lump.hpp"
#include "main.hpp"

#include <fstream>
#include <iostream>
#include <string>



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

void process_file_wad(std::string const & wadName)
{
	std::ifstream wadStream(wadName.c_str(), std::ios_base::in | std::ios_base::binary | std::ios_base::ate);

	if (!wadStream)
	{
		std::cerr << "unable to open:" << wadName << '\n';
		return;
	}

	size_t wadSize = wadStream.tellg();
	wadStream.seekg(0);

	std::string wadData;
	wadData.reserve(wadSize);

	char wadChar;
	while (wadStream.get(wadChar))
		wadData += wadChar;

	if (wadSize != wadData.size())
	{
		std::cerr << "unexpected size of wad: " << wadName << ':' << wadSize << "!=" << wadData.size() << '\n';
		return;
	}

	if (wadSize < 12)
	{
		std::cerr << "invalid size of wad:" << wadName << ':' << wadSize << '\n';
		return;
	}

	if (((wadData[0] != 'P') && (wadData[0] != 'I')) || (wadData[1] != 'W') || (wadData[2] != 'A') || (wadData[3] != 'D'))
	{
		std::cerr << "not a wad:" << wadName << '\n';
		return;
	}

	uint32_t lumpCount = (wadData[4] << 0) + (wadData[5] << 8) + (wadData[ 6] << 16) + (wadData[ 7] << 24);
	uint32_t lumpIndex = (wadData[8] << 0) + (wadData[9] << 8) + (wadData[10] << 16) + (wadData[11] << 24);

	while (lumpCount)
	{
		if ((lumpIndex+16) >= wadSize)
		{
			std::cerr << "invalid lump index:" << wadName << ':' << lumpIndex << '\n';
			return;
		}

		uint32_t lumpStart  = (wadData[lumpIndex+0] << 0) + (wadData[lumpIndex+1] << 8) + (wadData[lumpIndex+2] << 16) + (wadData[lumpIndex+3] << 24);
		uint32_t lumpLength = (wadData[lumpIndex+4] << 0) + (wadData[lumpIndex+5] << 8) + (wadData[lumpIndex+6] << 16) + (wadData[lumpIndex+7] << 24);

		std::string lumpName(wadData, lumpIndex+8, 8);

		std::string lumpData(wadData, lumpStart, lumpLength);

		lump_list.push_back(Lump(lumpData, lumpName));

		lumpIndex += 16;
		lumpCount -=  1;
	}
}



