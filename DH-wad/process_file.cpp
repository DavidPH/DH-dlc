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



bool process_file(std::string const & nameDir, std::string const & nameFile, std::string nameLump, bool addAlways)
{
	if (nameLump.empty())
	{
		// Strips file extension.
		nameLump = nameFile.substr(0, nameFile.find_first_of('.'));
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

	if (((wadData[0] != 'I') && (wadData[0] != 'P')) || (wadData[1] != 'W') || (wadData[2] != 'A') || (wadData[3] != 'D'))
	{
		std::cerr << "not a wad:" << wadName << '\n';
		return;
	}

	// Without the first cast, a value >127 may be interpreted as signed.
	uint32_t headCount(0);
	headCount += uint32_t((unsigned char)wadData[ 4]) <<  0;
	headCount += uint32_t((unsigned char)wadData[ 5]) <<  8;
	headCount += uint32_t((unsigned char)wadData[ 6]) << 16;
	headCount += uint32_t((unsigned char)wadData[ 7]) << 24;

	uint32_t headIndex(0);
	headIndex += uint32_t((unsigned char)wadData[ 8]) <<  0;
	headIndex += uint32_t((unsigned char)wadData[ 9]) <<  8;
	headIndex += uint32_t((unsigned char)wadData[10]) << 16;
	headIndex += uint32_t((unsigned char)wadData[11]) << 24;

	while (headCount != 0)
	{
		if ((headIndex+16) > wadSize)
		{
			std::cerr << "invalid head index:" << wadName << ':' << headIndex << ':' << wadSize << ':' << headCount << '\n';
			return;
		}

		uint32_t lumpStart(0);
		lumpStart  += uint32_t((unsigned char)wadData[headIndex + 0]) <<  0;
		lumpStart  += uint32_t((unsigned char)wadData[headIndex + 1]) <<  8;
		lumpStart  += uint32_t((unsigned char)wadData[headIndex + 2]) << 16;
		lumpStart  += uint32_t((unsigned char)wadData[headIndex + 3]) << 24;

		uint32_t lumpLength(0);
		lumpLength += uint32_t((unsigned char)wadData[headIndex + 4]) <<  0;
		lumpLength += uint32_t((unsigned char)wadData[headIndex + 5]) <<  8;
		lumpLength += uint32_t((unsigned char)wadData[headIndex + 6]) << 16;
		lumpLength += uint32_t((unsigned char)wadData[headIndex + 7]) << 24;

		std::string lumpName(wadData, headIndex+8, 8);

		std::string lumpData(wadData, lumpStart, lumpLength);

		lump_list.push_back(Lump(lumpData, lumpName));

		headIndex += 16;
		headCount -=  1;
	}
}



