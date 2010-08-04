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
	2010/05/10 - Add declarations for parsing the new binary types.
*/

#ifndef PARSING_H
#define PARSING_H

#include "SourceScanner.hpp"
#include "types.hpp"

#include <string>
#include <vector>



void add_function(std::string const & type, std::string const & name, std::string const & data);
std::string const & get_function(std::string const & type, std::string const & name);
bool has_function(std::string const & type, std::string const & name);

std::vector<std::string> parse_args(std::string const &);

name_t parse_name(SourceScannerDHLX &);

obj_t parse_obj(std::string const &, type_t const);

#define DECLARE_PARSE(TYPE) \
TYPE##_t parse_##TYPE(SourceScannerDHLX &); \
TYPE##_t parse_##TYPE(std::string const &); \
TYPE##_t parse_##TYPE##_function(std::string const &, SourceScannerDHLX &);              \
TYPE##_t parse_##TYPE##_function(std::string const &, std::vector<std::string> const &); \
TYPE##_t parse_##TYPE##_const(std::string const &); \
TYPE##_t parse_##TYPE##_unary(std::string const &, SourceScannerDHLX &); \
TYPE##_t parse_##TYPE##_unary(std::string const &, std::string const &);

DECLARE_PARSE(bool);

DECLARE_PARSE(int_s);
DECLARE_PARSE(int);
DECLARE_PARSE(int_l);

DECLARE_PARSE(real_s);
DECLARE_PARSE(real);
DECLARE_PARSE(real_l);

DECLARE_PARSE(string);
DECLARE_PARSE(string8);
DECLARE_PARSE(string16);
DECLARE_PARSE(string32);
DECLARE_PARSE(string80);
DECLARE_PARSE(string320);

DECLARE_PARSE(ubyte);
DECLARE_PARSE(sword);
DECLARE_PARSE(uword);
DECLARE_PARSE(sdword);
DECLARE_PARSE(udword);

#undef DECLARE_PARSE



#endif /* PARSING_H */



