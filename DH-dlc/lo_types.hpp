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
	2010/02/03 - Update for name_t.
	2010/02/28 - Use an enumeration rather than lo_type_* functions.
*/

#ifndef LO_TYPES_H
#define LO_TYPES_H

#include <string>



enum lo_type
{
	// For if(get_lo_type()) logic.
	LO_TYPE_NONE=0,
	LO_TYPE_VALUE,
	LO_TYPE_OBJECT,
	LO_TYPE_COMPOUNDOBJECT,
	LO_TYPE_INLINE
};



// default type handling

void                add_default_type(std::string const & name, std::string const & type, std::string const & context = "");
std::string const & get_default_type(std::string const & name, std::string const & context = "");
bool                has_default_type(std::string const & name, std::string const & context = "");



// lo_type functions

void add_lo_type(std::string const & type_name, lo_type type);
lo_type get_lo_type(std::string const & type_name);



// type redirection handling

void add_lo_type_redirect(const std::string&, const std::string&);

std::string const & get_lo_type_redirect(std::string const &);

bool is_lo_type_redirect(const std::string&);



#endif /* LO_TYPES_H */



