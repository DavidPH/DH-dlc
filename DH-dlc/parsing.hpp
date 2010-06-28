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

#include "types.hpp"

#include <string>
#include <vector>



std::vector<std::string> parse_args(std::string const &);

bool_t parse_bool(std::string const &);
bool_t parse_bool_function(std::string const &, std::vector<std::string> const &);
bool_t parse_bool_unary(std::string const &);
bool_t parse_bool_unary(std::string const &, std::string const &);

int_s_t parse_int_s(std::string const &);
int_s_t parse_int_s_function(std::string const &, std::vector<std::string> const &);
int_s_t parse_int_s_unary(std::string const &);
int_s_t parse_int_s_unary(std::string const &, std::string const &);
int_t parse_int(std::string const &);
int_t parse_int_function(std::string const &, std::vector<std::string> const &);
int_t parse_int_unary(std::string const &);
int_t parse_int_unary(std::string const &, std::string const &);
int_l_t parse_int_l(std::string const &);
int_l_t parse_int_l_function(std::string const &, std::vector<std::string> const &);
int_l_t parse_int_l_unary(std::string const &);
int_l_t parse_int_l_unary(std::string const &, std::string const &);

obj_t parse_obj(std::string const &, std::string const &);

real_s_t parse_real_s(std::string const &);
real_s_t parse_real_s_function(std::string const &, std::vector<std::string> const &);
real_s_t parse_real_s_unary(std::string const &);
real_s_t parse_real_s_unary(std::string const &, std::string const &);
real_t parse_real(std::string const &);
real_t parse_real_function(std::string const &, std::vector<std::string> const &);
real_t parse_real_unary(std::string const &);
real_t parse_real_unary(std::string const &, std::string const &);
real_l_t parse_real_l(std::string const &);
real_l_t parse_real_l_function(std::string const &, std::vector<std::string> const &);
real_l_t parse_real_l_unary(std::string const &);
real_l_t parse_real_l_unary(std::string const &, std::string const &);

string_t parse_string(std::string const &);
string_t parse_string_function(std::string const &, std::vector<std::string> const &);
string_t parse_string_unary(std::string const &);
string_t parse_string_unary(std::string const &, std::string const &);
string8_t parse_string8(std::string const &);
string8_t parse_string8_function(std::string const &, std::vector<std::string> const &);
string8_t parse_string8_unary(std::string const &);
string8_t parse_string8_unary(std::string const &, std::string const &);

sword_t parse_sword(std::string const &);
sword_t parse_sword_function(std::string const &, std::vector<std::string> const &);
sword_t parse_sword_unary(std::string const &);
sword_t parse_sword_unary(std::string const &, std::string const &);

ubyte_t parse_ubyte(std::string const &);
ubyte_t parse_ubyte_function(std::string const &, std::vector<std::string> const &);
ubyte_t parse_ubyte_unary(std::string const &);
ubyte_t parse_ubyte_unary(std::string const &, std::string const &);

uword_t parse_uword(std::string const &);
uword_t parse_uword_function(std::string const &, std::vector<std::string> const &);
uword_t parse_uword_unary(std::string const &);
uword_t parse_uword_unary(std::string const &, std::string const &);



#endif /* PARSING_H */



