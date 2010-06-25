/*
    Copyright 2009, 2010 David Hill

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

/*
	2010/06/24 - Altered interface to allow using different file name and
		lump name.
*/

#ifndef PROCESS_FILE_H
#define PROCESS_FILE_H

#include <string>



/*
	Returns true if lump added, else false.

	If nameFile is empty and addAlways is true, add an empty lump by
	nameLump.

	If nameLump is empty, use nameFile.
*/
bool process_file(std::string const & nameDir, std::string const & nameFile, std::string const & nameLump, bool addAlways = false);



#endif /* PROCESS_FILE_H */



