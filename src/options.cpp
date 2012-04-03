//-----------------------------------------------------------------------------
//
// Copyright(C) 2009-2012 David Hill
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------------
//
// Global program options.
//
//-----------------------------------------------------------------------------

#include "options.hpp"

#include "option.hpp"


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

static option::option_dptr<bool> option_debug_handler
('\0', "debug", "debugging", "Enables debugging messages.", NULL,
 &option_debug);

static option::option_dptr<bool> option_debug_dump_handler
('\0', "debug-dump", "debugging",
 "Prints every object at the end of program.\nWARNING: Will go into an "
 "infinite loop if an object references itself, directly or otherwise.", NULL,
 &option_debug_dump);

static option::option_dptr<bool> option_debug_seed_handler
('\0', "debug-seed", "debugging", "Prints the starting seed.", NULL,
 &option_debug_seed);

static option::option_dptr<bool> option_debug_time_handler
('\0', "debug-time", "debugging", "Prints the compile times.", NULL,
 &option_debug_time);

static option::option_dptr<bool> option_debug_token_handler
('\0', "debug-token", "debugging", NULL, NULL, &option_debug_token);

static option::option_dptr<bool> option_case_sensitive_handler
('\0', "case-sensitive", "input", "Enables case sensitivity. On by default.",
 NULL, &option_case_sensitive);

static option::option_dptr<bool> option_case_upper_handler
('\0', "case-upper", "input", NULL, NULL, &option_case_upper);

static option::option_dptr<bool> option_force_default_types_handler
('\0', "force-default-types", "features", NULL, NULL,
 &option_force_default_types);

static option::option_dptr<bool> option_lib_std_handler
('\0', "lib-std", "libraries",
 "Automatically includes lib-std.ddl. On by default.", NULL, &option_lib_std);

static option::option_dptr<bool> option_lib_udmf_handler
('\0', "lib-udmf", "libraries",
 "Automatically includes lib-udmf.ddl. On by default.", NULL, &option_lib_udmf);

static option::option_dptr<bool> option_lib_udmf_strict_handler
('\0', "lib-udmf-strict", "libraries",
 "Automatically includes lib-udmf-strict.ddl.", NULL, &option_lib_udmf);

static option::option_dptr<bool> option_lib_usdf_handler
('\0', "lib-usdf", "libraries",
 "Automatically includes lib-usdf.ddl.", NULL, &option_lib_usdf);

static option::option_dptr<bool> option_lib_usdf_strict_handler
('\0', "lib-usdf-strict", "libraries",
 "Automatically includes lib-usdf-strict.ddl.", NULL, &option_lib_usdf);

static option::option_dptr<bool> option_output_any_handler
('\0', "output-any", "output", "Enables any output. On by default.", NULL,
 &option_output_any);

static option::option_dptr<bool> option_output_doom_handler
('\0', "output-doom", "output", "Enables Doom output.", NULL,
 &option_output_doom);

static option::option_dptr<bool> option_output_extradata_handler
('\0', "output-extradata", "output", "Enables ExtraData output.", NULL,
 &option_output_extradata);

static option::option_dptr<bool> option_output_heretic_handler
('\0', "output-heretic", "output", "Enables Heretic output.", NULL,
 &option_output_heretic);

static option::option_dptr<bool> option_output_hexen_handler
('\0', "output-hexen", "output", "Enables Hexen output.", NULL,
 &option_output_hexen);

static option::option_dptr<bool> option_output_strife_handler
('\0', "output-strife", "output", "Enables Strife output.", NULL,
 &option_output_strife);

static option::option_dptr<bool> option_output_udmf_handler
('\0', "output-udmf", "output", "Enables UDMF output. On by default", NULL,
 &option_output_udmf);

static option::option_dptr<bool> option_output_usdf_handler
('\0', "output-usdf", "output", "Enables USDF output.", NULL,
 &option_output_usdf);

static option::option_dptr<bool> option_strict_strings_handler
('\0', "strict-strings", "features", NULL, NULL, &option_strict_strings);

static option::option_dptr<bool> option_strict_types_handler
('\0', "strict-types", "features", NULL, NULL, &option_strict_types);

static option::option_dptr<bool> option_use_file_extensions_handler
('\0', "file-extensions", "output", "Adds file-extensions to output files.",
 NULL, &option_use_file_extensions);

static option::option_dptr<int> option_error_limit_handler
('\0', "error-limit", "input",
 "Sets the number of errors that can occur before terminating. 1 by default.",
 NULL, &option_error_limit);

static option::option_dptr<int> option_seed_handler
('\0', "seed", "features", "Sets the seed for the random number generator.",
 NULL, &option_seed);

static option::option_dptr<std::string> option_directory_handler
('\0', "directory", "output", "Sets the output directory.", NULL,
 &option_directory);
static option::option_copy option_directory_handler_copy
('o', "output", &option_directory_handler);

static option::option_dptr<std::string> option_map_name_handler
('\0', "map-name", "scripts", "Sets the map name.", NULL, &option_map_name);

static option::option_dptr<std::string> option_script_acs_handler
('\0', "script-acs", "scripts",
 "Sets the output file for ACS. SCRIPTS by default.", NULL,
 &option_script_acs);

static option::option_dptr<std::string> option_script_extradata_handler
('\0', "script-extradata", "scripts",
 "Sets the output file for ExtraData. EXTRADAT by default.", NULL,
 &option_script_extradata);


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

bool option_debug       = false;
bool option_debug_dump  = false;
bool option_debug_seed  = false;
bool option_debug_time  = false;
bool option_debug_token = false;

bool option_case_sensitive = true;
bool option_case_upper     = false;

bool option_force_default_types = false;

bool option_lib_std         = true;
bool option_lib_udmf        = true;
bool option_lib_udmf_strict = false;
bool option_lib_usdf        = false;
bool option_lib_usdf_strict = false;

bool option_output_any       = true;
bool option_output_doom      = false;
bool option_output_extradata = false;
bool option_output_heretic   = false;
bool option_output_hexen     = false;
bool option_output_strife    = false;
bool option_output_udmf      = true;
bool option_output_usdf      = false;

bool option_strict_strings = true;
bool option_strict_types   = false;

bool option_use_file_extensions = false;

int option_error_limit = 1;
int option_seed        = 0;

std::string option_directory;
std::string option_map_name;
std::string option_script_acs       = "SCRIPTS";
std::string option_script_extradata = "EXTRADAT";

bool &option_map_name_handled = option_map_name_handler.handled;
bool &option_seed_handled = option_seed_handler.handled;

// EOF

