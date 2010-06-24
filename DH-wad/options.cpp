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
	03/02/2010 - Update for new process_options.h.
*/

#include <cstdlib>
#include <iostream>

#define PROCESS_OPTION_USER_ERROR usage(); exit(2);

#include "main.hpp"
#include "options.hpp"
#include "../common/process_options.c"



PROCESS_OPTION_DEFINE_bool(iwad, false)

PROCESS_OPTION_DEFINE_string(output, "")

PROCESS_OPTION_DEFINE_string_multi(directory)
PROCESS_OPTION_DEFINE_string_multi(map)



PROCESS_OPTION_LONG_DECLARE
{
	if (cmp_opt(opt, "help") >= 4)
	{
		usage();
		exit(0);
	}

	if (cmp_opt(opt, "version") >= 7)
	{
		version();
		exit(0);
	}

	PROCESS_OPTION_HANDLE_LONG_bool(iwad, "iwad", 3);

	PROCESS_OPTION_HANDLE_LONG_string(output, "output", 3);

	PROCESS_OPTION_HANDLE_LONG_string_multi(map,       "map",       3);
	PROCESS_OPTION_HANDLE_LONG_string_multi(directory, "directory", 3);

	PROCESS_OPTION_HANDLE_LONG_UNKNOWN();
}

PROCESS_OPTION_SHORT_DECLARE
{
	if (opt == 'h')
	{
		usage();
		exit(0);
	}

	PROCESS_OPTION_HANDLE_SHORT_string(output, 'o');

	PROCESS_OPTION_HANDLE_SHORT_string_multi(map,       'm');
	PROCESS_OPTION_HANDLE_SHORT_string_multi(directory, 'd');

	PROCESS_OPTION_HANDLE_SHORT_UNKNOWN();
}



PROCESS_OPTION_ARG_DEFINE

PROCESS_OPTION_DEFINE



