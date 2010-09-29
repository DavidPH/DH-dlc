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

#ifndef COMPOUND_OBJECTS_H
#define COMPOUND_OBJECTS_H

#include "SourceScanner.hpp"
#include "types.hpp"

#include "LevelObject/LevelObjectPointer.hpp"

#include <string>



void add_compound_object(std::string const & type, SourceScannerDHLX & sc);
void add_compound_object(std::string const & type, std::string const & data);

void do_compound_object(std::string const & type, obj_t const & object);



#endif /* COMPOUND_OBJECTS_H */



