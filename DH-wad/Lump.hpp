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

#ifndef LUMP_H
#define LUMP_H

#include <stdint.h>
#include <string>



class Lump
{
	public:
		Lump(const std::string&, const std::string&);
		Lump(const std::string&, const std::string&, uint32_t);

		std::string encodeBody() const;
		std::string encodeHead() const;

		uint32_t length() const;

	private:
		std::string _lumpData;
		std::string _lumpName;
		uint32_t    _lumpStart;
};



#endif /* LUMP_H */



