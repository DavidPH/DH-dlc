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
	2010/02/04 - Added command_name_deletevolatile().
	2010/02/06 - Added --debug-token option for debugging.
	2010/02/06 - Added command_name_return().
	2010/02/15 - Added command_name_break(), command_name_continue(),
		command_name_for(), and command_name_do().
	2010/06/24 - Added several type_*().
	2010/06/24 - Added option_lib_udmf_strict.
*/

#ifndef OPTIONS_H
#define OPTIONS_H

#include "../common/process_options.h"



PROCESS_OPTION_EXTERN_bool(debug);
PROCESS_OPTION_EXTERN_bool(debug_dump);
PROCESS_OPTION_EXTERN_bool(debug_seed);
PROCESS_OPTION_EXTERN_bool(debug_time);
PROCESS_OPTION_EXTERN_bool(debug_token);

PROCESS_OPTION_EXTERN_bool(case_sensitive);
PROCESS_OPTION_EXTERN_bool(case_upper);

PROCESS_OPTION_EXTERN_bool(force_default_types);

PROCESS_OPTION_EXTERN_bool(lib_std);
PROCESS_OPTION_EXTERN_bool(lib_udmf);
PROCESS_OPTION_EXTERN_bool(lib_udmf_strict);
PROCESS_OPTION_EXTERN_bool(lib_usdf);
PROCESS_OPTION_EXTERN_bool(lib_usdf_strict);

PROCESS_OPTION_EXTERN_bool(output_any);
PROCESS_OPTION_EXTERN_bool(output_doom);
PROCESS_OPTION_EXTERN_bool(output_extradata);
PROCESS_OPTION_EXTERN_bool(output_heretic);
PROCESS_OPTION_EXTERN_bool(output_hexen);
PROCESS_OPTION_EXTERN_bool(output_strife);
PROCESS_OPTION_EXTERN_bool(output_udmf);
PROCESS_OPTION_EXTERN_bool(output_usdf);

PROCESS_OPTION_EXTERN_bool(strict_strings);
PROCESS_OPTION_EXTERN_bool(strict_types);

PROCESS_OPTION_EXTERN_bool(use_file_extensions);

PROCESS_OPTION_EXTERN_int(error_limit);
PROCESS_OPTION_EXTERN_int(precision);
PROCESS_OPTION_EXTERN_int(seed);

PROCESS_OPTION_EXTERN_string(directory);
PROCESS_OPTION_EXTERN_string(map_name);
PROCESS_OPTION_EXTERN_string(script_acs);
PROCESS_OPTION_EXTERN_string(script_extradata);

PROCESS_OPTION_EXTERN_string_multi(include);



PROCESS_OPTION_LONG_EXTERN;
PROCESS_OPTION_SHORT_EXTERN;

PROCESS_OPTION_ARG_EXTERN;

PROCESS_OPTION_EXTERN;



void set_precision();
void set_precision(int);



#define COUNT_INFO (void) 0
#define COUNT_DEBUG (void) 0
#define COUNT_WARNING (void) 0
#define COUNT_ERROR					\
if (option_error_limit && --option_error_limit == 0)	\
{							\
	std::cerr << "error limit reached\n";		\
	exit(1);					\
}							\
else (void) 0

#define PRINT_INFO(MSG)	\
std::cerr << MSG

#define PRINT_DEBUG(MSG)		\
if (option_debug)			\
	std::cerr << "debug:" << MSG;	\
else (void) 0

#define PRINT_WARNING(MSG)	\
std::cerr << "warning:" << MSG

#define PRINT_ERROR(MSG)	\
std::cerr << "error:" << MSG

#define PRINT_AND_COUNT_INFO(MSG)	\
if (true)				\
{					\
	PRINT_INFO(MSG);		\
	COUNT_INFO;			\
}					\
else (void) 0

#define PRINT_AND_COUNT_DEBUG(MSG)	\
if (option_debug)			\
{					\
	PRINT_DEBUG(MSG);		\
	COUNT_DEBUG;			\
}					\
else (void) 0

#define PRINT_AND_COUNT_WARNING(MSG)	\
if (true)				\
{					\
	PRINT_WARNING(MSG);		\
	COUNT_WARNING;			\
}					\
else (void) 0

#define PRINT_AND_COUNT_ERROR(MSG)	\
if (true)				\
{					\
	PRINT_ERROR(MSG);		\
	COUNT_ERROR;			\
}					\
else (void) 0



/*
	These functions make it so I don't have to worry so much about the case
	sensitivity issue.
*/

#define NAME_FUNC(TYPE, NAME, STR_TRUE, STR_UPPER, STR_LOWER)	\
inline const char* TYPE##_name_##NAME()				\
{								\
	return option_case_sensitive ? STR_TRUE :		\
		(option_case_upper ? STR_UPPER : STR_LOWER);	\
}

// command names
NAME_FUNC(command, defaulttype, "defaulttype", "DEFAULTTYPE", "defaultype")
NAME_FUNC(command, define,      "define",      "DEFINE",      "define")
NAME_FUNC(command, include,     "include",     "INCLUDE",     "include")
NAME_FUNC(command, precision,   "precision",   "PRECISION",   "precision")
NAME_FUNC(command, typedef,     "typedef",     "TYPEDEF",     "typedef")
NAME_FUNC(command, typedefnew,  "typedefnew",  "TYPEDEFNEW",  "typedefnew")

NAME_FUNC(command, break,            "break",            "BREAK",            "break")
NAME_FUNC(command, changetype,       "changetype",       "CHANGETYPE",       "changetype")
NAME_FUNC(command, compound,         "compound",         "COMPOUND",         "compound")
NAME_FUNC(command, continue,         "continue",         "CONTINUE",         "continue")
NAME_FUNC(command, debug,            "debug",            "DEBUG",            "debug")
NAME_FUNC(command, delete,           "delete",           "DELETE",           "delete")
NAME_FUNC(command, deletevolatile,   "deletevolatile",   "DELETEVOLATILE",   "deletevolatile")
NAME_FUNC(command, delete_,          "delete_",          "DELETE_",          "delete_")
NAME_FUNC(command, do,               "do",               "DO",               "do")
NAME_FUNC(command, else,             "else",             "ELSE",             "else")
NAME_FUNC(command, elseifcmp,        "elseifcmp",        "ELSEIFCMP",        "elseifcmp")
NAME_FUNC(command, elseif,           "elseif",           "ELSEIF",           "elseif")
NAME_FUNC(command, error,            "error",            "ERROR",            "error")
NAME_FUNC(command, function,         "function",         "FUNCTION",         "function")
NAME_FUNC(command, for,              "for",              "FOR",              "for")
NAME_FUNC(command, ifcmp,            "ifcmp",            "IFCMP",            "ifcmp")
NAME_FUNC(command, if,               "if",               "IF",               "if")
NAME_FUNC(command, info,             "info",             "INFO",             "info")
NAME_FUNC(command, return,           "return",           "RETURN",           "return")
NAME_FUNC(command, script,           "script",           "SCRIPT",           "script")
NAME_FUNC(command, script_acs,       "script_acs",       "SCRIPT_ACS",       "script_acs")
NAME_FUNC(command, script_extradata, "script_extradata", "SCRIPT_EXTRADATA", "script_extradata")
NAME_FUNC(command, script_fraggle,   "script_fraggle",   "SCRIPT_FRAGGLE",   "script_fraggle")
NAME_FUNC(command, warn,             "warn",             "WARN",             "warn")
NAME_FUNC(command, while,            "while",            "WHILE",            "while")

// compare strings
NAME_FUNC(cmp, eq, "eq", "EQ", "eq")
NAME_FUNC(cmp, ne, "ne", "NE", "ne")
NAME_FUNC(cmp, lt, "lt", "LT", "lt")
NAME_FUNC(cmp, le, "le", "LE", "le")
NAME_FUNC(cmp, gt, "gt", "GT", "gt")
NAME_FUNC(cmp, ge, "ge", "GE", "ge")

// functions
NAME_FUNC(function, abs,       "abs",       "ABS",       "abs")
NAME_FUNC(function, acos,      "acos",      "ACOS",      "acos")
NAME_FUNC(function, angle,     "angle",     "ANGLE",     "angle")
NAME_FUNC(function, asin,      "asin",      "ASIN",      "asin")
NAME_FUNC(function, atan,      "atan",      "ATAN",      "atan")
NAME_FUNC(function, byte2deg,  "byte2deg",  "BYTE2DEG",  "byte2deg")
NAME_FUNC(function, byte2rad,  "byte2rad",  "BYTE2RAD",  "byte2rad")
NAME_FUNC(function, byteangle, "byteangle", "BYTEANGLE", "byteangle")
NAME_FUNC(function, cmp,       "cmp",       "CMP",       "cmp")
NAME_FUNC(function, cmpfs,     "cmpfs",     "CMPFS",     "cmpfs")
NAME_FUNC(function, cmpf,      "cmpf",      "CMPF",      "cmpf")
NAME_FUNC(function, cmpfl,     "cmpfl",     "CMPFL",     "cmpfl")
NAME_FUNC(function, cmpis,     "cmpis",     "CMPIS",     "cmpis")
NAME_FUNC(function, cmpi,      "cmpi",      "CMPI",      "cmpi")
NAME_FUNC(function, cmpil,     "cmpil",     "CMPIL",     "cmpil")
NAME_FUNC(function, cmps,      "cmps",      "CMPS",      "cmps")
NAME_FUNC(function, cos,       "cos",       "COS",       "cos")
NAME_FUNC(function, deg2byte,  "deg2byte",  "DEG2BYTE",  "deg2byte")
NAME_FUNC(function, deg2rad,   "deg2rad",   "DEG2RAD",   "deg2rad")
NAME_FUNC(function, degrees,   "degrees",   "DEGREES",   "degrees")
NAME_FUNC(function, distance,  "distance",  "DISTANCE",  "distance")
NAME_FUNC(function, exists,    "exists",    "EXISTS",    "exists")
NAME_FUNC(function, facing,    "facing",    "FACING",    "facing")
NAME_FUNC(function, hypot,     "hypot",     "HYPOT",     "hypot")
NAME_FUNC(function, lower,     "lower",     "LOWER",     "lower")
NAME_FUNC(function, mapname,   "mapname",   "MAPNAME",   "mapname")
NAME_FUNC(function, not,       "not",       "NOT",       "not")
NAME_FUNC(function, pi,        "pi",        "PI",        "pi")
NAME_FUNC(function, quote,     "quote",     "QUOTE",     "quote")
NAME_FUNC(function, rad2byte,  "rad2byte",  "RAD2BYTE",  "rad2byte")
NAME_FUNC(function, rad2deg,   "rad2deg",   "RAD2DEG",   "rad2deg")
NAME_FUNC(function, radians,   "radians",   "RADIANS",   "radians")
NAME_FUNC(function, random,    "random",    "RANDOM",    "random")
NAME_FUNC(function, round,     "round",     "ROUND",     "round")
NAME_FUNC(function, sin,       "sin",       "SIN",       "sin")
NAME_FUNC(function, sqrt,      "sqrt",      "SQRT",      "sqrt")
NAME_FUNC(function, tan,       "tan",       "TAN",       "tan")
NAME_FUNC(function, upper,     "upper",     "UPPER",     "upper")

// iftype strings
NAME_FUNC(iftype, bool,   "bool",   "BOOL",   "bool")
NAME_FUNC(iftype, exists, "exists", "EXISTS", "exists")
NAME_FUNC(iftype, false,  "false",  "FALSE",  "false")
NAME_FUNC(iftype, global, "global", "GLOBAL", "global")
NAME_FUNC(iftype, local,  "local",  "LOCAL",  "local")
NAME_FUNC(iftype, not,    "not",    "NOT",    "not")
NAME_FUNC(iftype, true,   "true",   "TRUE",   "true")

// internal key names
NAME_FUNC(key, arg,  "arg",  "ARG",  "arg")
NAME_FUNC(key, argc, "argc", "ARGC", "argc")
NAME_FUNC(key, x,    "x",    "X",    "x")
NAME_FUNC(key, y,    "y",    "Y",    "y")

// internal value types
NAME_FUNC(type, bool,       "bool",       "BOOL",       "bool")
NAME_FUNC(type, float,      "float",      "FLOAT",      "float")
NAME_FUNC(type, int,        "int",        "INT",        "int")
NAME_FUNC(type, long,       "long",       "LONG",       "long")
NAME_FUNC(type, longfloat,  "longfloat",  "LONGFLOAT",  "longfloat")
NAME_FUNC(type, longint,    "longint",    "LONGINT",    "longint")
NAME_FUNC(type, returntype, "returntype", "RETURNTYPE", "returntype")
NAME_FUNC(type, short,      "short",      "SHORT",      "short")
NAME_FUNC(type, shortfloat, "shortfloat", "SHORTFLOAT", "shortfloat")
NAME_FUNC(type, shortint,   "shortint",   "SHORTINT",   "shortint")
NAME_FUNC(type, string,     "string",     "STRING",     "string")
NAME_FUNC(type, string8,    "string8",    "STRING8",    "string8")
NAME_FUNC(type, string16,   "string16",   "STRING16",   "string16")
NAME_FUNC(type, string32,   "string32",   "STRING32",   "string32")
NAME_FUNC(type, string80,   "string80",   "STRING80",   "string80")
NAME_FUNC(type, string320,  "string320",  "STRING320",  "string320")
NAME_FUNC(type, type,       "objtype",    "OBJTYPE",    "objtype")
NAME_FUNC(type, ubyte,      "ubyte",      "UBYTE",      "ubyte")
NAME_FUNC(type, sword,      "sword",      "SWORD",      "sword")
NAME_FUNC(type, uword,      "uword",      "UWORD",      "uword")
NAME_FUNC(type, sdword,     "sdword",     "SDWORD",     "sdword")
NAME_FUNC(type, udword,     "udword",     "UDWORD",     "udword")

// internal object types
NAME_FUNC(type, choice,       "CHOICE",       "CHOICE",       "choice")
NAME_FUNC(type, conversation, "CONVERSATION", "CONVERSATION", "conversation")
NAME_FUNC(type, cost,         "COST",         "COST",         "cost")
NAME_FUNC(type, ifitem,       "IFITEM",       "IFITEM",       "ifitem")
NAME_FUNC(type, linedef,      "LINEDEF",      "LINEDEF",      "linedef")
NAME_FUNC(type, page,         "PAGE",         "PAGE",         "page")
NAME_FUNC(type, sector,       "SECTOR",       "SECTOR",       "sector")
NAME_FUNC(type, sidedef,      "SIDEDEF",      "SIDEDEF",      "sidedef")
NAME_FUNC(type, thing,        "THING",        "THING",        "thing")
NAME_FUNC(type, vertex,       "VERTEX",       "VERTEX",       "vertex")

// miscellaneous
NAME_FUNC(misc, false,  "false",  "FALSE",  "false")
NAME_FUNC(misc, global, "global", "GLOBAL", "global")
NAME_FUNC(misc, this,   "this",   "THIS",   "this")
NAME_FUNC(misc, true,   "true",   "TRUE",   "true")

#undef NAME_FUNC



#endif /* OPTIONS_H */



