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
	2010/05/08 - Attempt to add Windows directory iteration.
*/

#include "process_directory.hpp"

#include "process_file.hpp"
#include "process_map.hpp"

#include "../common/IO.hpp"
#include "../common/foreach.hpp"

#include <string>
#include <vector>

#include <iostream>



void process_directory(std::string const & dirName, bool primeDir)
{
	std::vector<std::string> filelist(IO::lsdir(dirName.c_str()));

	FOREACH_T(std::vector<std::string>, it, filelist)
	{
		std::string & filename = *it;

		if (IO::isdir(filename.c_str()))
		{
			if (primeDir)
			{
				process_map(dirName, filename);
			}
			else
			{
				process_directory(filename, false);
			}
		}
		else // TODO: IO::isfile
		{
			process_file(dirName, filename, "");
		}
	}
}



