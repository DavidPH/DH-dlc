/*
    Copyright 2009, 2011 David Hill

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

#include <string>

#include "Lump.hpp"
#include "main.hpp"
#include "options.hpp"



Lump::Lump(const std::string& lumpData, const std::string& lumpName) : _lumpData(lumpData), _lumpName(lumpName), _lumpStart(get_lump_list_length()+12)
{
	transName();
}
Lump::Lump(const std::string& lumpData, const std::string& lumpName, uint32_t lumpStart) : _lumpData(lumpData), _lumpName(lumpName), _lumpStart(lumpStart)
{
	transName();
}

std::string Lump::encodeBody() const
{
	return _lumpData;
}

std::string Lump::encodeHead() const
{
	std::string encodedData(16, 0);

	// start
	encodedData[ 0] = (_lumpStart >>  0) & 0xFF;
	encodedData[ 1] = (_lumpStart >>  8) & 0xFF;
	encodedData[ 2] = (_lumpStart >> 16) & 0xFF;
	encodedData[ 3] = (_lumpStart >> 24) & 0xFF;

	// length
	encodedData[ 4] = (_lumpData.size() >>  0) & 0xFF;
	encodedData[ 5] = (_lumpData.size() >>  8) & 0xFF;
	encodedData[ 6] = (_lumpData.size() >> 16) & 0xFF;
	encodedData[ 7] = (_lumpData.size() >> 24) & 0xFF;

	// name
	encodedData[ 8] = _lumpName[0];
	encodedData[ 9] = _lumpName[1];
	encodedData[10] = _lumpName[2];
	encodedData[11] = _lumpName[3];
	encodedData[12] = _lumpName[4];
	encodedData[13] = _lumpName[5];
	encodedData[14] = _lumpName[6];
	encodedData[15] = _lumpName[7];

	if (option_trans_bslash)
	{
		for (int i = 8; i < 16; ++i)
			if (encodedData[i] == '^')
				encodedData[i] = '\\';
	}

	return encodedData;
}

std::string Lump::getData() const
{
	return _lumpData;
}
std::string Lump::getName() const
{
	return std::string(_lumpName.c_str());
}

uint32_t Lump::length() const
{
	return _lumpData.length();
}

void Lump::transName()
{
	while (_lumpName.size() < 8)
		_lumpName += '\0';

	if (option_trans_bslash)
	{
		for (int i = 0; i < 8; ++i)
			if (_lumpName[i] == '\\')
				_lumpName[i] = '^';
	}
}

