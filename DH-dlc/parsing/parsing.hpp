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

#include "../SourceScanner.hpp"
#include "../types.hpp"

#include <string>
#include <vector>



std::vector<std::string> parse_args(std::string const &);

name_t parse_name(SourceScannerDHLX &);

obj_t parse_obj(std::string const &, type_t const);

template<typename T>
T parse(SourceScannerDHLX &);
template<typename T>
T parse(std::string const &);



#endif /* PARSING_H */



