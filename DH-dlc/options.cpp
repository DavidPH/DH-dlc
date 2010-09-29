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
	2010/02/03 - Update for new process_options.h.
	2010/02/06 - Added --debug-token option for debugging.
	2010/06/24 - Added option_lib_udmf_strict.
*/

#define PROCESS_OPTION_USER_ERROR usage(); exit(2);

#include "options.hpp"

#include "main.hpp"
#include "types.hpp"

#include "types/real_t.hpp"

#include "../common/process_options.c"

#include <cstdlib>
#include <iostream>



PROCESS_OPTION_DEFINE_bool(debug,       false)
PROCESS_OPTION_DEFINE_bool(debug_dump,  false)
PROCESS_OPTION_DEFINE_bool(debug_seed,  false)
PROCESS_OPTION_DEFINE_bool(debug_time,  false)
PROCESS_OPTION_DEFINE_bool(debug_token, false)

PROCESS_OPTION_DEFINE_bool(case_sensitive, true)
PROCESS_OPTION_DEFINE_bool(case_upper,     false)

PROCESS_OPTION_DEFINE_bool(force_default_types, false)

PROCESS_OPTION_DEFINE_bool(lib_std,         true)
PROCESS_OPTION_DEFINE_bool(lib_udmf,        true)
PROCESS_OPTION_DEFINE_bool(lib_udmf_strict, false)
PROCESS_OPTION_DEFINE_bool(lib_usdf,        false)
PROCESS_OPTION_DEFINE_bool(lib_usdf_strict, false)

PROCESS_OPTION_DEFINE_bool(output_any,       true)
PROCESS_OPTION_DEFINE_bool(output_doom,      false)
PROCESS_OPTION_DEFINE_bool(output_extradata, false)
PROCESS_OPTION_DEFINE_bool(output_heretic,   false)
PROCESS_OPTION_DEFINE_bool(output_hexen,     false)
PROCESS_OPTION_DEFINE_bool(output_strife,    false)
PROCESS_OPTION_DEFINE_bool(output_udmf,      true)
PROCESS_OPTION_DEFINE_bool(output_usdf,      false)

PROCESS_OPTION_DEFINE_bool(strict_strings, true)
PROCESS_OPTION_DEFINE_bool(strict_types,   false)

PROCESS_OPTION_DEFINE_bool(use_file_extensions, false)

PROCESS_OPTION_DEFINE_int(error_limit, 1)
PROCESS_OPTION_DECLARE_int(precision, POINTER_BIT*2)
{
	option_precision_default = false;
	option_precision         = atoi(arg);

	set_precision();

	return 2;
}
PROCESS_OPTION_DEFINE_int(seed, 0)

PROCESS_OPTION_DEFINE_string(directory,        "")
PROCESS_OPTION_DEFINE_string(map_name,         "")
PROCESS_OPTION_DEFINE_string(script_acs,       "SCRIPTS")
PROCESS_OPTION_DEFINE_string(script_extradata, "EXTRADAT")

PROCESS_OPTION_DEFINE_string_multi(include)



PROCESS_OPTION_LONG_DECLARE
{
	if (cmp_opt(opt, "help") > 4)
	{
		usage();
		exit(0);
	}

	if (cmp_opt(opt, "limits") > 6)
	{
		limits();
		exit(0);
	}

	if (cmp_opt(opt, "version") > 7)
	{
		version();
		exit(0);
	}

	PROCESS_OPTION_HANDLE_LONG_bool(debug,       "debug",        6);
	PROCESS_OPTION_HANDLE_LONG_bool(debug_dump,  "debug-dump",  11);
	PROCESS_OPTION_HANDLE_LONG_bool(debug_seed,  "debug-seed",  11);
	PROCESS_OPTION_HANDLE_LONG_bool(debug_time,  "debug-time",  11);
	PROCESS_OPTION_HANDLE_LONG_bool(debug_token, "debug-token", 12);

	PROCESS_OPTION_HANDLE_LONG_bool(case_sensitive, "case-sensitive", 15);
	PROCESS_OPTION_HANDLE_LONG_bool(case_upper,     "case-upper",     11);

	PROCESS_OPTION_HANDLE_LONG_bool(force_default_types, "force-default-types", 20);

	PROCESS_OPTION_HANDLE_LONG_bool(lib_std,         "lib-std",          8);
	PROCESS_OPTION_HANDLE_LONG_bool(lib_udmf,        "lib-udmf",         9);
	PROCESS_OPTION_HANDLE_LONG_bool(lib_udmf_strict, "lib-udmf-strict", 16);
	PROCESS_OPTION_HANDLE_LONG_bool(lib_usdf,        "lib-usdf",         9);
	PROCESS_OPTION_HANDLE_LONG_bool(lib_usdf_strict, "lib-usdf-strict", 16);

	PROCESS_OPTION_HANDLE_LONG_bool(output_any,       "output-any",       11);
	PROCESS_OPTION_HANDLE_LONG_bool(output_doom,      "output-doom",      12);
	PROCESS_OPTION_HANDLE_LONG_bool(output_extradata, "output-extradata", 17);
	PROCESS_OPTION_HANDLE_LONG_bool(output_heretic,   "output-heretic",   15);
	PROCESS_OPTION_HANDLE_LONG_bool(output_hexen,     "output-hexen",     13);
	PROCESS_OPTION_HANDLE_LONG_bool(output_strife,    "output-strife",    14);
	PROCESS_OPTION_HANDLE_LONG_bool(output_udmf,      "output-udmf",      12);
	PROCESS_OPTION_HANDLE_LONG_bool(output_usdf,      "output-usdf",      12);

	PROCESS_OPTION_HANDLE_LONG_bool(strict_strings, "strict-strings", 15);
	PROCESS_OPTION_HANDLE_LONG_bool(strict_types,   "strict-types",   13);

	PROCESS_OPTION_HANDLE_LONG_bool(use_file_extensions, "extensions", 3);

	PROCESS_OPTION_HANDLE_LONG_int(error_limit, "error-limit", 12);
	PROCESS_OPTION_HANDLE_LONG_int(precision,   "precision",    4);
	PROCESS_OPTION_HANDLE_LONG_int(seed,        "seed",         5);

	PROCESS_OPTION_HANDLE_LONG_string(directory,        "directory",         3);
	PROCESS_OPTION_HANDLE_LONG_string(map_name,         "map-name",          3);
	PROCESS_OPTION_HANDLE_LONG_string(script_acs,       "script-acs",       11);
	PROCESS_OPTION_HANDLE_LONG_string(script_extradata, "script-extradata", 17);

	PROCESS_OPTION_HANDLE_LONG_string_multi(include, "include", 3);

	PROCESS_OPTION_HANDLE_LONG_UNKNOWN();
}

PROCESS_OPTION_SHORT_DECLARE
{
	if (opt == 'h')
	{
		usage();
		exit(0);
	}

	PROCESS_OPTION_HANDLE_SHORT_bool(case_sensitive, 'c');

	PROCESS_OPTION_HANDLE_SHORT_int(error_limit, 'e');
	PROCESS_OPTION_HANDLE_SHORT_int(precision,   'p');

	PROCESS_OPTION_HANDLE_SHORT_string(directory, 'd');
	PROCESS_OPTION_HANDLE_SHORT_string(map_name,  'm');

	PROCESS_OPTION_HANDLE_SHORT_string_multi(include, 'i');

	PROCESS_OPTION_HANDLE_SHORT_UNKNOWN();
}



PROCESS_OPTION_ARG_DEFINE

PROCESS_OPTION_DEFINE



void set_precision()
{
	#if USE_GMPLIB
	mpf_set_default_prec(option_precision);
	#endif
}
void set_precision(int new_precision)
{
	option_precision = new_precision;

	set_precision();
}



