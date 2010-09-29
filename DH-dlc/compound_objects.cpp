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

*/

#include "compound_objects.hpp"

#include <map>

#include "types.hpp"
#include "exceptions/InvalidTypeException.hpp"



std::map<std::string, std::string> compound_object_defines_DDL;
std::map<std::string, SourceScannerDHLX> compound_object_defines_DHLX;

void add_compound_object(std::string const & type, SourceScannerDHLX & sc)
{
	compound_object_defines_DHLX[type] = sc.getblock(SourceTokenDHLX::TT_OP_BRACE_O, SourceTokenDHLX::TT_OP_BRACE_C);
}
void add_compound_object(std::string const & type, std::string const & data)
{
	compound_object_defines_DDL[type] = data;
}

void do_compound_object(std::string const & type, obj_t const & object)
{
	std::map<std::string, std::string>::iterator itDDL = compound_object_defines_DDL.find(type);

	if (itDDL != compound_object_defines_DDL.end())
	{
		object->addData(itDDL->second, type);
		return;
	}

	std::map<std::string, SourceScannerDHLX>::iterator itDHLX = compound_object_defines_DHLX.find(type);

	if (itDHLX != compound_object_defines_DHLX.end())
	{
		SourceScannerDHLX data(itDHLX->second);

		object->addData(data);

		return;
	}

	throw InvalidTypeException("undefined compound object:" + type);
}



