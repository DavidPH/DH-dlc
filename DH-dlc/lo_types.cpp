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
	2010/02/28 - Update for enum lo_type.
*/

#include "lo_types.hpp"

#include <map>

#include "exceptions/NoDefaultTypeException.hpp"



// default type handling

std::map< std::string, std::map<std::string, std::string> > dt_map;

void add_default_type(std::string const & name, std::string const & type, std::string const & context)
{
	dt_map[name][context] = type;
}

std::string const & get_default_type(std::string const & name, std::string const & context)
{
	if (!has_default_type(name, context))
	{
		if (context.empty())
			throw NoDefaultTypeException(name);
		else
			throw NoDefaultTypeException(name + " in " + context);
	}

	return dt_map[name][context];
}

bool has_default_type(std::string const & name, std::string const & context)
{
	return !dt_map[name][context].empty();
}



// lo_type functions

std::map<std::string, lo_type> lo_type_map;

void add_lo_type(std::string const & type_name, lo_type type)
{
	lo_type_map[type_name] = type;
}

lo_type get_lo_type(std::string const & type_name)
{
	return lo_type_map[type_name];
}



// type redirection handling

std::map<std::string, std::string> lo_type_redirects;

void add_lo_type_redirect(std::string const & new_type, std::string const & old_type)
{
	if (new_type.empty()) return;

	lo_type_redirects[new_type] = old_type;
}

std::string const & get_lo_type_redirect(std::string const & type)
{
	if (type.empty()) return type;

	if (is_lo_type_redirect(type))
		return lo_type_redirects[type];

	return type;
}

bool is_lo_type_redirect(std::string const & type)
{
	return !lo_type_redirects[type].empty();
}



