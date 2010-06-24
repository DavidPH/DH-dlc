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



std::map<std::string, std::string> compound_object_defines;

void add_compound_object(std::string const & type, std::string const & data)
{
	std::map<std::string, std::string>::iterator it = compound_object_defines.find(type);

	if (it != compound_object_defines.end())
		throw InvalidTypeException("already defined compound object:" + type);

	compound_object_defines[type] = data;
}

std::string const & get_compound_object(std::string const & type)
{
	std::map<std::string, std::string>::iterator it = compound_object_defines.find(type);

	if (it == compound_object_defines.end())
		throw InvalidTypeException("undefined compound object:" + type);

	return compound_object_defines[type];
}



