/*
    Copyright 2009, 2010 David Hill

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
	This is a header for easily adding command line options to a C/C++
	program. It is fully functional with both, but in C++, the STL is used
	for certain option types giving it some additional features.

	If the C behaviour is desired, define PROCESS_OPTION_C before including
	this header.

	The file process_options.c contains the definitions for certain
	functions, and should either be included by a source file of your
	project or built and linked.

	NOTE: Two option names are reserved: arg and end. option_arg is used to
	store leftover arguments that aren't options or use by options.
	option_end indicates an explicit end of options via --. The names short
	and long are also used internally.

	Crude outline of program flow with default handlers:

	PROCESS_OPTIONS
	|->process_option(const char* opt, const char* arg)
	   |->process_option_arg(const char* arg, bool barg)
	   |->process_option_long(const char* opt, const char* arg, bool barg)
	   |  |->process_option_*(const char* arg, bool barg)
	   |->process_option_short(char opt, const char* arg, bool barg)
	      |->process_option_*(const char* arg, bool barg)

	This handles all of the normal GNU option conventions:
		--opt
		--opt arg
		--opt=arg

		-o -p
		-op
		-o -p arg
		-op arg
		-o -parg
		-oparg

	-- signals the end of options.

	--opt=arg and -oarg are handled in process_option before being handed
	to process_option_long or process_option_short. As a result, opt is not
	always null terminated (it may be '=' terminated).

	If reading options from a file (or similiar mechanism) and you require
	--opt=arg or -oarg form, you can just say process_option(opt, NULL).

	As a part of the default process_option function, options are given a
	boolean sense (which is true by default). Long options can be prefixed
	with --do- or --no- (which is stripped). Short options are true if
	lower case or non-alphabetic, false if uppercase. The opt char is
	passed in its original state.

	Any command line elements that are neither an option or an argument to
	an option get sent to process_option_arg. The default behaviour is to
	append it to option_arg for later reference by the program.

	When using the default handler macros and a user error occurs, the
	macro PROCESS_OPTION_USER_ERROR is invoked. By default, it is defined
	as exit(2);.



	Types:

	Many of the following definitions make mention of type. For the
	purposes of this documentation, these are the following types, and the
	actual type they represent (C/C++):
		bool   = bool / bool
		float  = float / float
		int    = int / int
		string = char* / std::string

		bool_multi   = bool* / std::vector<bool>
		float_multi  = float* / std::vector<float>
		int_multi    = int* / std::vector<int>
		string_multi = char** / std::vector<std::string>

	They are typedefed as *_opt_t, so that code can declare
	correct variables. However, when a macro calls for a type, it expects
	the short version (int, not int_opt_t).



	Configuration defines/macros:

	These macros affect how option processing works. They should be defined
	before including the header and/or source file(s) are included. Certain
	exceptions exist that are not errors, but can make awkward code.

	PROCESS_OPTION_C
	PROCESS_OPTION_LONG_T
	PROCESS_OPTION_USER_ERROR



	PROCESS_OPTION_C

	If this is defined, no C++ features are used.

	If this macro isn't defined and compiling as C, it is defined.



	PROCESS_OPTION_LONG_T

	If this is defined, larger types are used for options. This is
	reflected in the typedefs.



	PROCESS_OPTION_USER_ERROR

	This macro is invoked during option processing when a user error is
	detected. It is also used in the definitions of certain functions.

	It is generally safe to redefine this macro, but this will introduce
	inconsistencies in the generated code.

	If this macro isn't defined, it is defined as exit(2);.



	Functions:

	cmp_opt(const char* s1, const char* s2)

	fput_opt(const char* opt, FILE* out)

	put_opt(std::ostream& out, const char* opt) (C++ only)

	process_option_string_to_float(const char* str, int base)
	process_option_string_to_int(const char* str, int base)



	int cmp_opt(const char* s1, const char* s2)

	s1 and s2 can be either null or '=' terminated.

	If s1 == s2, returns length+1.
	If s1 starts with s2, returns the length of s2.
	If s2 starts with s1, returns the length of s1.
	Else, returns -1.



	int fput_opt(constchar* opt, FILE* out)

	Works like fputs, but opt can be terminated by a '=' character.

	Returns number of characters printed, or -1 if an error occurs.

	This function is available in C++ if cstdio is included.



	std::ostream& put_opt(std::ostream& out, const char* opt)

	Like fput_opt, but for std::ostream.

	Returns out.

	This function is only available in C++.



	float_opt_t process_option_string_to_float(const char* str, int base)
	int_opt_t   process_option_string_to_int(const char* str, int base)

	Converts str to a number. If base is 0, str can be prefixed to be with
	0 or 0x to indicate base.

	In C++, base has a default value of 0.

	If str does not represent a valid number or base is unsupported, prints
	an error message and invokes the macro PROCESS_OPTION_USER_ERROR. If
	the macro fails to terminate the program, the function will return with
	whatever number it was able to extract so far.



	Macros:

	PROCESS_OPTION_EXTERN_type(name);
	PROCESS_OPTION_DECLARE_type(name, default) { ... }
	PROCESS_OPTION_DEFINE_type(name, default)

	PROCESS_OPTION_LONG_EXTERN;
	PROCESS_OPTION_LONG_DECLARE { ... }
	PROCESS_OPTION_SHORT_EXTERN;
	PROCESS_OPTION_SHORT_DECLARE { ... }

	PROCESS_OPTION_REQUIRE_ARGUMENT_LONG();
	PROCESS_OPTION_REQUIRE_ARGUMENT_SHORT();

	PROCESS_OPTION_CALL_type(name);

	PROCESS_OPTION_HANDLE_LONG_type(name, string, min_match_length);
	PROCESS_OPTION_HANDLE_SHORT_type(name, character);

	PROCESS_OPTION_HANDLE_LONG_UNKNOWN();
	PROCESS_OPTION_HANDLE_SHORT_UNKNOWN();

	PROCESS_OPTION_ARG_EXTERN;
	PROCESS_OPTION_ARG_DECLARE { ... }
	PROCESS_OPTION_ARG_DEFINE

	PROCESS_OPTION_EXTERN;
	PROCESS_OPTION_DECLARE { ... }
	PROCESS_OPTION_DEFINE

	PROCESS_OPTIONS;



	PROCESS_OPTION_EXTERN_type(name);
	PROCESS_OPTION_DECLARE_type(name, default) { ... }
	PROCESS_OPTION_DEFINE_type(name, default)

	*_multi types do not take the default argument.

	Declares/defines the following variables and function:
		bool   option_name_default = true;
		size_t option_name_size    = 0;
		type   option_name         = default;

		int process_option_name(const char* arg, bool barg)

	_size is only declared for *_multi types.

	The function is expected to return 2 if it uses arg, and 1 if not.
	Whether barg is used or not is unimportant.



	PROCESS_OPTION_LONG_EXTERN;
	PROCESS_OPTION_LONG_DECLARE { ... }
	PROCESS_OPTION_SHORT_EXTERN;
	PROCESS_OPTION_SHORT_DECLARE { ... }

	Declares/defines one of the following functions:
		int process_option_long(const char* opt, const char* arg, bool barg)
		int process_option_short(     char  opt, const char* arg, bool barg)

	The functions are expected to return 2 if arg is used, and 1 if not.
	Whether barg is used or not is unimportant.



	PROCESS_OPTION_REQUIRE_ARGUMENT_LONG();
	PROCESS_OPTION_REQUIRE_ARGUMENT_SHORT();

	If arg is NULL, prints an error message and invokes the macro
	PROCESS_OPTION_USER_ERROR.



	PROCESS_OPTION_CALL_type(name);

	Calls the option processor for name and returns the result. Is
	equivalent to:
		return process_option_name(arg, barg)



	PROCESS_OPTION_HANDLE_LONG_type(name, string, min_match_length);
	PROCESS_OPTION_HANDLE_SHORT_type(name, character, name);

	Calls the option processor for name if opt equals string or character.
	min_match_length can be used to allow shorthand long options (as in
	--dir instead of --directory). If no shorthand is desired, say length
	of the string + 1.

	Except for bool types, this implies PROCESS_OPTION_REQUIRE_ARGUMENT_*.



	PROCESS_OPTION_HANDLE_LONG_UNKNOWN();
	PROCESS_OPTION_HANDLE_SHORT_UNKNOWN();

	Prints an error message and invoked the macro
	PROCESS_OPTION_USER_ERROR.



	PROCESS_OPTION_ARG_EXTERN;
	PROCESS_OPTION_ARG_DECLARE { ... }
	PROCESS_OPTION_ARG_DEFINE

	Essentially the same as PROCESS_OPTION_*_string_multi(arg). This option
	is used to store excess arguments from the command line.



	PROCESS_OPTION_EXTERN;
	PROCESS_OPTION_DECLARE { ... }
	PROCESS_OPTION_DEFINE

	Declares/defines the function:
		int process_option(const char* opt, const char* arg)

	The function is expected to return 2 if it uses arg, and 1 if not.



	PROCESS_OPTIONS;

	Iterates over argv, calling process_option as needed.



	Example:

#include "process_options.c"

PROCESS_OPTION_DEFINE_bool(action, true)
PROCESS_OPTION_DEFINE_int(verbosity, 0)
PROCESS_OPTION_DEFINE_string(dir, "")

PROCESS_OPTION_LONG_DECLARE
{
	PROCESS_OPTION_HANDLE_LONG_bool(action, "action", 7);
	PROCESS_OPTION_HANDLE_LONG_int(verbosity, "verbosity", 10);
	PROCESS_OPTION_HANDLE_LONG_string(dir, "directory", 3);

	PROCESS_OPTION_HANDLE_LONG_UNKNOWN;
}

PROCESS_OPTION_SHORT_DECLARE
{
	PROCESS_OPTION_HANDLE_SHORT_bool(action, 'a');
	PROCESS_OPTION_HANDLE_SHORT_int(verbosity, 'v');
	PROCESS_OPTION_HANDLE_SHORT_string(dir, 'd');

	PROCESS_OPTION_HANDLE_SHORT_UNKNOWN;
}

PROCESS_OPTION_ARG_DEFINE

PROCESS_OPTION_DEFINE

int main(int argc, char** argv)
{
	PROCESS_OPTIONS;

	return 0;
}
*/

#ifndef PROCESS_OPTIONS_H
#define PROCESS_OPTIONS_H

#if !defined(__cplusplus) && !defined(PROCESS_OPTION_C)
#define PROCESS_OPTION_C
#endif

#ifdef PROCESS_OPTION_C
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#else
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#endif

#ifndef PROCESS_OPTION_USER_ERROR
#define PROCESS_OPTION_USER_ERROR exit(2);
#endif



typedef bool  bool_opt_t;

#ifdef PROCESS_OPTION_LONG_T
typedef double   float_opt_t;
typedef long int int_opt_t;
#else
typedef float float_opt_t;
typedef int   int_opt_t;
#endif

#ifdef PROCESS_OPTION_C
typedef char*            string_opt_t;

typedef bool_opt_t*   bool_multi_opt_t;
typedef float_opt_t*  float_multi_opt_t;
typedef int_opt_t*    int_multi_opt_t;
typedef string_opt_t* string_multi_opt_t;

extern int fput_opt(const char* opt, FILE* out);

extern float_opt_t process_option_string_to_float(const char* str, int base);
extern int_opt_t   process_option_string_to_int(const char* str, int base);
#else
typedef std::string               string_opt_t;

typedef std::vector<bool_opt_t>   bool_multi_opt_t;
typedef std::vector<float_opt_t>  float_multi_opt_t;
typedef std::vector<int_opt_t>    int_multi_opt_t;
typedef std::vector<string_opt_t> string_multi_opt_t;

# if defined(__FILE_defined) && defined(EOF)
extern int fput_opt(const char* opt, FILE* out);
# endif

extern std::ostream& put_opt(std::ostream& out, const char* opt);

extern float_opt_t process_option_string_to_float(const char* str, int base = 0);
extern int_opt_t   process_option_string_to_int(const char* str, int base = 0);
#endif

extern int cmp_opt(const char* s1, const char* s2);



/*
	Use these in your headers.
*/

#define PROCESS_OPTION_EXTERN_base(OPTION, TYPE)	\
extern bool         option_##OPTION##_default;	\
extern TYPE##_opt_t option_##OPTION;			\
							\
extern int process_option##OPTION(const char*, bool)

#define PROCESS_OPTION_EXTERN_base_multi(OPTION, TYPE)	\
extern bool         option_##OPTION##_default;	\
extern size_t       option_##OPTION##_size;		\
extern TYPE##_opt_t option_##OPTION;			\
							\
extern int process_option_##OPTION(const char*, bool)

#define PROCESS_OPTION_EXTERN_bool(OPTION)	\
PROCESS_OPTION_EXTERN_base(OPTION, bool)

#define PROCESS_OPTION_EXTERN_bool_multi(OPTION)	\
PROCESS_OPTION_EXTERN_base_multi(OPTION, bool_multi)

#define PROCESS_OPTION_EXTERN_float(OPTION)	\
PROCESS_OPTION_EXTERN_base(OPTION, float)

#define PROCESS_OPTION_EXTERN_float_multi(OPTION)	\
PROCESS_OPTION_EXTERN_base_multi(OPTION, float_multi)

#define PROCESS_OPTION_EXTERN_int(OPTION)	\
PROCESS_OPTION_EXTERN_base(OPTION, int)

#define PROCESS_OPTION_EXTERN_int_multi(OPTION)	\
PROCESS_OPTION_EXTERN_base_multi(OPTION, int_multi)

#define PROCESS_OPTION_EXTERN_string(OPTION)	\
PROCESS_OPTION_EXTERN_base(OPTION, string)

#define PROCESS_OPTION_EXTERN_string_multi(OPTION)	\
PROCESS_OPTION_EXTERN_base_multi(OPTION, string_multi)



/*
	Use these to define custom handlers.
*/

#define PROCESS_OPTION_DECLARE_base(OPTION, DEFAULT, TYPE)	\
bool         option_##OPTION##_default = true;		\
TYPE##_opt_t option_##OPTION           = DEFAULT;	\
								\
int process_option_##OPTION(const char* arg, bool barg)

#ifdef PROCESS_OPTION_C
#define PROCESS_OPTION_DECLARE_base_multi(OPTION, TYPE)	\
bool         option_##OPTION##_default = true;		\
size_t       option_##OPTION##_size    = 0;		\
TYPE##_opt_t option_##OPTION           = NULL;		\
							\
int process_option_##OPTION(const char* arg, bool barg)
#else
#define PROCESS_OPTION_DECLARE_base_multi(OPTION, TYPE)	\
bool         option_##OPTION##_default = true;		\
size_t       option_##OPTION##_size    = 0;		\
TYPE##_opt_t option_##OPTION;				\
							\
int process_option_##OPTION(const char* arg, bool barg)
#endif

#define PROCESS_OPTION_DECLARE_bool(OPTION, DEFAULT)	\
PROCESS_OPTION_DECLARE_base(OPTION, DEFAULT, bool)

#define PROCESS_OPTION_DECLARE_bool_multi(OPTION)	\
PROCESS_OPTION_DECLARE_base_multi(OPTION, bool_multi)

#define PROCESS_OPTION_DECLARE_float(OPTION, DEFAULT)	\
PROCESS_OPTION_DECLARE_base(OPTION, DEFAULT, float)

#define PROCESS_OPTION_DECLARE_float_multi(OPTION)	\
PROCESS_OPTION_DECLARE_base_multi(OPTION, float_multi)

#define PROCESS_OPTION_DECLARE_int(OPTION, DEFAULT)	\
PROCESS_OPTION_DECLARE_base(OPTION, DEFAULT, int)

#define PROCESS_OPTION_DECLARE_int_multi(OPTION)	\
PROCESS_OPTION_DECLARE_base_multi(OPTION, int_multi)

#define PROCESS_OPTION_DECLARE_string(OPTION, DEFAULT)	\
PROCESS_OPTION_DECLARE_base(OPTION, DEFAULT, string)

#define PROCESS_OPTION_DECLARE_string_multi(OPTION)	\
PROCESS_OPTION_DECLARE_base_multi(OPTION, string_multi)



/*
	Use these for default handlers.
*/

#define PROCESS_OPTION_DEFINE_base(OPTION, DEFAULT, TYPE)	\
PROCESS_OPTION_DECLARE_##TYPE(OPTION, DEFAULT)			\
{								\
	option_##OPTION##_default = false;

#define PROCESS_OPTION_DEFINE_base_multi(OPTION, TYPE)	\
PROCESS_OPTION_DECLARE_##TYPE(OPTION)			\
{							\
	option_##OPTION##_default = false;

#define PROCESS_OPTION_DEFINE_bool(OPTION, DEFAULT)	\
PROCESS_OPTION_DEFINE_base(OPTION, DEFAULT, bool)	\
	option_##OPTION = barg;				\
	return 1;					\
}

#define PROCESS_OPTION_DEFINE_bool_multi(OPTION)			\
PROCESS_OPTION_DEFINE_base_multi(OPTION, bool_multi)			\
	PROCESS_OPTIONS_APPEND_ARRAY(option_##OPTION, barg, bool);	\
	return 1;							\
}

#define PROCESS_OPTION_DEFINE_float(OPTION, DEFAULT)			\
PROCESS_OPTION_DEFINE_base(OPTION, DEFAULT, float)			\
	option_##OPTION = process_option_string_to_float(arg, 0);	\
	return 2;							\
}

#define PROCESS_OPTION_DEFINE_float_multi(OPTION)							\
PROCESS_OPTION_DEFINE_base_multi(OPTION, float_multi)							\
	PROCESS_OPTIONS_APPEND_ARRAY(option_##OPTION, process_option_string_to_float(arg, 0), float);	\
	return 2;											\
}

#define PROCESS_OPTION_DEFINE_int(OPTION, DEFAULT)		\
PROCESS_OPTION_DEFINE_base(OPTION, DEFAULT, int)		\
	option_##OPTION = process_option_string_to_int(arg, 0);	\
	return 2;						\
}

#define PROCESS_OPTION_DEFINE_int_multi(OPTION)								\
PROCESS_OPTION_DEFINE_base_multi(OPTION, int_multi)							\
	PROCESS_OPTIONS_APPEND_ARRAY(option_##OPTION, process_option_string_to_int(arg, 0), int);	\
	return 2;											\
}

#define PROCESS_OPTION_DEFINE_string(OPTION, DEFAULT)		\
PROCESS_OPTION_DEFINE_base(OPTION, DEFAULT, string)		\
	PROCESS_OPTION_ASSIGN_string(option_##OPTION, arg);	\
	return 2;						\
}

#define PROCESS_OPTION_DEFINE_string_multi(OPTION)			\
PROCESS_OPTION_DEFINE_base_multi(OPTION, string_multi)			\
	PROCESS_OPTIONS_APPEND_ARRAY(option_##OPTION, arg, string);	\
	return 2;							\
}



/*
	Use these to require having an argument.
*/

#ifdef PROCESS_OPTION_C
#define PROCESS_OPTION_REQUIRE_ARGUMENT_LONG()	\
if (arg == NULL)				\
{						\
	fputs("option '--", stderr);		\
	fput_opt(opt, stderr);			\
	fputs("' requires argument\n", stderr);	\
						\
	PROCESS_OPTION_USER_ERROR;		\
}						\
else (void) 0

#define PROCESS_OPTION_REQUIRE_ARGUMENT_SHORT()				\
if (arg == NULL)							\
{									\
	fprintf(stderr, "option '-%c' requires argument\n", opt);	\
	PROCESS_OPTION_USER_ERROR;					\
}									\
else (void) 0
#else
#define PROCESS_OPTION_REQUIRE_ARGUMENT_LONG()			\
if (arg == NULL)						\
{								\
	std::cerr << "option '--";				\
	put_opt(std::cerr, opt) << "' requires argument\n";	\
								\
	PROCESS_OPTION_USER_ERROR;				\
}								\
else (void) 0

#define PROCESS_OPTION_REQUIRE_ARGUMENT_SHORT()				\
if (arg == NULL)							\
{									\
	std::cerr << "option '-" << opt << "' requires argument\n";	\
	PROCESS_OPTION_USER_ERROR;					\
}									\
else (void) 0
#endif



/*
	Use these for custom checking but default calling.
*/

#define PROCESS_OPTION_CALL_base(OPTION)	\
return process_option_##OPTION(arg, barg)

#define PROCESS_OPTION_CALL_bool(OPTION)	\
PROCESS_OPTION_CALL_base(OPTION)

#define PROCESS_OPTION_CALL_bool_multi(OPTION)	\
PROCESS_OPTION_CALL_base(OPTION)

#define PROCESS_OPTION_CALL_float(OPTION)	\
PROCESS_OPTION_CALL_base(OPTION)

#define PROCESS_OPTION_CALL_float_multi(OPTION)	\
PROCESS_OPTION_CALL_base(OPTION)

#define PROCESS_OPTION_CALL_int(OPTION)	\
PROCESS_OPTION_CALL_base(OPTION)

#define PROCESS_OPTION_CALL_int_multi(OPTION)	\
PROCESS_OPTION_CALL_base(OPTION)

#define PROCESS_OPTION_CALL_string(OPTION)	\
PROCESS_OPTION_CALL_base(OPTION)

#define PROCESS_OPTION_CALL_string_multi(OPTION)	\
PROCESS_OPTION_CALL_base(OPTION)



/*
	Use these for default checking and calling in process_option_long.
*/

#define PROCESS_OPTION_HANDLE_LONG_base(NAME, MIN_MATCH, OPTION, TYPE, RETURN)	\
if (cmp_opt(opt, NAME) >= MIN_MATCH)						\
{										\
	if (RETURN == 2)							\
	{									\
		PROCESS_OPTION_REQUIRE_ARGUMENT_LONG();				\
	}									\
										\
	PROCESS_OPTION_CALL_##TYPE(OPTION);					\
}										\
else (void) 0

#define PROCESS_OPTION_HANDLE_LONG_bool(OPTION, NAME, MIN_MATCH)	\
PROCESS_OPTION_HANDLE_LONG_base(NAME, MIN_MATCH, OPTION, bool, 1)

#define PROCESS_OPTION_HANDLE_LONG_bool_multi(OPTION, NAME, MIN_MATCH)	\
PROCESS_OPTION_HANDLE_LONG_base(NAME, MIN_MATCH, OPTION, bool_multi, 1)

#define PROCESS_OPTION_HANDLE_LONG_float(OPTION, NAME, MIN_MATCH)	\
PROCESS_OPTION_HANDLE_LONG_base(NAME, MIN_MATCH, OPTION, float, 2)

#define PROCESS_OPTION_HANDLE_LONG_float_multi(OPTION, NAME, MIN_MATCH)	\
PROCESS_OPTION_HANDLE_LONG_base(NAME, MIN_MATCH, OPTION, float_multi, 2)

#define PROCESS_OPTION_HANDLE_LONG_int(OPTION, NAME, MIN_MATCH)	\
PROCESS_OPTION_HANDLE_LONG_base(NAME, MIN_MATCH, OPTION, int, 2)

#define PROCESS_OPTION_HANDLE_LONG_int_multi(OPTION, NAME, MIN_MATCH)	\
PROCESS_OPTION_HANDLE_LONG_base(NAME, MIN_MATCH, OPTION, int_multi, 2)

#define PROCESS_OPTION_HANDLE_LONG_string(OPTION, NAME, MIN_MATCH)	\
PROCESS_OPTION_HANDLE_LONG_base(NAME, MIN_MATCH, OPTION, string, 2)

#define PROCESS_OPTION_HANDLE_LONG_string_multi(OPTION, NAME, MIN_MATCH)	\
PROCESS_OPTION_HANDLE_LONG_base(NAME, MIN_MATCH, OPTION, string_multi, 2)



/*
	Use these for default checking and calling in process_option_short.
*/

#define PROCESS_OPTION_HANDLE_SHORT_base(NAME, OPTION, TYPE, RETURN)	\
if (tolower(opt) == NAME)						\
{									\
	if (RETURN == 2)						\
	{								\
		PROCESS_OPTION_REQUIRE_ARGUMENT_SHORT();		\
	}								\
									\
	PROCESS_OPTION_CALL_##TYPE(OPTION);				\
}									\
else (void) 0

#define PROCESS_OPTION_HANDLE_SHORT_bool(OPTION, NAME)	\
PROCESS_OPTION_HANDLE_SHORT_base(NAME, OPTION, bool, 1)

#define PROCESS_OPTION_HANDLE_SHORT_bool_multi(OPTION, NAME)	\
PROCESS_OPTION_HANDLE_SHORT_base(NAME, OPTION, bool_multi, 1)

#define PROCESS_OPTION_HANDLE_SHORT_float(OPTION, NAME)	\
PROCESS_OPTION_HANDLE_SHORT_base(NAME, OPTION, float, 2)

#define PROCESS_OPTION_HANDLE_SHORT_float_multi(OPTION, NAME)	\
PROCESS_OPTION_HANDLE_SHORT_base(NAME, OPTION, float_multi, 2)

#define PROCESS_OPTION_HANDLE_SHORT_int(OPTION, NAME)	\
PROCESS_OPTION_HANDLE_SHORT_base(NAME, OPTION, int, 2)

#define PROCESS_OPTION_HANDLE_SHORT_int_multi(OPTION, NAME)	\
PROCESS_OPTION_HANDLE_SHORT_base(NAME, OPTION, int_multi, 2)

#define PROCESS_OPTION_HANDLE_SHORT_string(OPTION, NAME)	\
PROCESS_OPTION_HANDLE_SHORT_base(NAME, OPTION, string, 2)

#define PROCESS_OPTION_HANDLE_SHORT_string_multi(OPTION, NAME)	\
PROCESS_OPTION_HANDLE_SHORT_base(NAME, OPTION, string_multi, 2)



/*
	Use this for default unknown option handling.
*/

#ifdef PROCESS_OPTION_C
#define PROCESS_OPTION_HANDLE_LONG_UNKNOWN()	\
if (1)						\
{						\
	fputs("unknown option '--", stderr);	\
	fput_opt(opt, stderr);			\
	fputs("'\n", stderr);			\
						\
	PROCESS_OPTION_USER_ERROR;		\
}						\
else (void) 0

#define PROCESS_OPTION_HANDLE_SHORT_UNKNOWN()		\
if (1)							\
{							\
	fprintf(stderr, "unknown option '-%c'\n", opt);	\
	PROCESS_OPTION_USER_ERROR;			\
}							\
else (void) 0
#else
#define PROCESS_OPTION_HANDLE_LONG_UNKNOWN()	\
if (1)						\
{						\
	std::cerr << "unknown option '--"; 	\
	put_opt(std::cerr, opt) << "'\n";	\
						\
	PROCESS_OPTION_USER_ERROR;		\
}						\
else (void) 0

#define PROCESS_OPTION_HANDLE_SHORT_UNKNOWN()			\
if (1)								\
{								\
	std::cerr << "unknown option '-" << opt << "'\n";	\
	PROCESS_OPTION_USER_ERROR;				\
}								\
else (void) 0
#endif




/*
	Processes individual options, called by PROCESS_OPTION.
*/

#define PROCESS_OPTION_LONG_EXTERN	\
extern int process_option_long(const char*, const char*, bool)

#define PROCESS_OPTION_LONG_DECLARE	\
int process_option_long(const char* opt, const char* arg, bool barg)

#define PROCESS_OPTION_SHORT_EXTERN	\
extern int process_option_short(char, const char*, bool)

#define PROCESS_OPTION_SHORT_DECLARE	\
int process_option_short(char opt, const char* arg, bool barg)



/*
	Processes unused arguments, called by PROCESS_OPTION.
*/

#define PROCESS_OPTION_ARG_EXTERN	\
PROCESS_OPTION_EXTERN_string_multi(arg)

#define PROCESS_OPTION_ARG_DECLARE	\
PROCESS_OPTION_DECLARE_string_multi(arg)

#define PROCESS_OPTION_ARG_DEFINE	\
PROCESS_OPTION_DEFINE_string_multi(arg)



/*
	Processes individual options, called by PROCESS_OPTIONS.
*/

#define PROCESS_OPTION_EXTERN	\
extern bool option_end;		\
				\
extern int process_option(const char*, const char*)

#define PROCESS_OPTION_DECLARE	\
bool option_end = false;	\
				\
int process_option(const char* opt, const char* arg)

#define PROCESS_OPTION_DEFINE						\
PROCESS_OPTION_DECLARE							\
{									\
	int         argUsed = 1;					\
	bool        barg;						\
	const char* carg;						\
	size_t      index;						\
									\
	if (option_end || opt[0] != '-' || opt[1] == 0)			\
	{								\
		process_option_arg(opt, true);				\
		return 1;						\
	}								\
									\
	/* If arg is an option, don't use it. */			\
	if (arg && arg[0] == '-' && arg[1]) arg = NULL;			\
									\
	if (opt[1] == '-')						\
	{								\
		const char* optOffset;					\
									\
		/* -- end of options */					\
		if (opt[2] == 0) 					\
		{							\
			option_end = true;				\
			return 1;					\
		}							\
									\
		/* --* long option */					\
		barg = true;						\
		carg = arg;						\
									\
		for (index = 2; opt[index]; ++index)			\
		{							\
			if (opt[index] == '=')				\
			{						\
				carg = opt+index+1;			\
				break;					\
			}						\
		}							\
									\
		optOffset = opt + 2;					\
									\
		if (opt[2] == 'n' && opt[3] == 'o' && opt[4] == '-')	\
		{							\
			barg = false;					\
			optOffset += 3;					\
		}							\
		else if (opt[2] == 'd' && opt[3] == 'o' && opt[4] == '-')\
		{							\
			barg = true;					\
			optOffset += 3;					\
		}							\
									\
		argUsed = process_option_long(optOffset, carg, barg);	\
									\
		if (carg == NULL || carg != arg)			\
			argUsed = 1;					\
									\
		return argUsed;						\
	}								\
									\
	/* -* short option(s) */					\
	for (index = 1; opt[index]; ++index)				\
	{								\
		if (opt[index+1] == 0)					\
			carg = arg;					\
		else							\
			carg = opt+index+1;				\
									\
		/* This makes non-alphabetics true. */			\
		barg = !isupper(opt[index]);				\
									\
		argUsed = process_option_short(opt[index], carg, barg);	\
									\
		/* We've used the rest of opt, so we're done. */	\
		/* But we did not use up arg, so indicate as such. */	\
		if (argUsed == 2 && carg != arg)			\
		{							\
			argUsed = 1;					\
			break;						\
		}							\
	}								\
									\
	return argUsed;							\
}



/*
	Processes all options, must be used in main.
*/

#define PROCESS_OPTIONS()							\
if (1)										\
{										\
	int index = 1;							\
										\
	for ( ; index < argc; )							\
	{									\
		int argUsed;							\
										\
		if (index != argc-1)						\
			argUsed = process_option(argv[index], argv[index+1]);	\
		else								\
			argUsed = process_option(argv[index], NULL);		\
										\
		index += argUsed;						\
	}									\
}										\
else (void) 0



/*
	Internally used macros, not for external use.
*/

#ifdef PROCESS_OPTION_C
#define PROCESS_OPTIONS_APPEND_ARRAY(ARRAY_VAR, NEW_VAR, TYPE)		\
if (1)									\
{									\
	if (ARRAY_VAR == NULL)						\
	{								\
		ARRAY_VAR = (TYPE##_multi_opt_t)			\
			malloc(sizeof(TYPE##_opt_t));			\
									\
		ARRAY_VAR##_size = 1;					\
	}								\
	else								\
	{								\
		ARRAY_VAR = (TYPE##_multi_opt_t) realloc(ARRAY_VAR,	\
			sizeof(TYPE##_opt_t) * (++ARRAY_VAR##_size));	\
	}								\
									\
	if (ARRAY_VAR == NULL)						\
		{fprintf(stderr, "out of memory\n"); exit(1);}		\
									\
	PROCESS_OPTION_ASSIGN_##TYPE(					\
		ARRAY_VAR[ARRAY_VAR##_size-1], NEW_VAR);		\
}									\
else (void) 0
#else
#define PROCESS_OPTIONS_APPEND_ARRAY(ARRAY_VAR, NEW_VAR, TYPE)	\
if (1)								\
{								\
	ARRAY_VAR.push_back(NEW_VAR);				\
	ARRAY_VAR##_size = ARRAY_VAR.size();			\
}								\
else (void) 0
#endif

#define PROCESS_OPTION_ASSIGN_bool(VAR, VAL)	\
VAR = VAL

#define PROCESS_OPTION_ASSIGN_int(VAR, VAL)	\
VAR = VAL

#ifdef PROCESS_OPTION_C
#define PROCESS_OPTION_ASSIGN_string(VAR, STRING)	\
if (1)							\
{							\
	size_t index;					\
							\
	for (index = 0; STRING[index]; ++index);	\
							\
	VAR = (char*) malloc(index);			\
							\
	if (VAR == NULL)				\
	{						\
		fprintf(stderr, "out of memory\n");	\
		exit(1);				\
	}						\
							\
	for (index = 0; STRING[index]; ++index)		\
		VAR[index] = STRING[index];		\
							\
	VAR[index] = 0;					\
}							\
else (void) 0
#else
#define PROCESS_OPTION_ASSIGN_string(VAR, STRING)	\
VAR = STRING
#endif



#endif /* PROCESS_OPTIONS_H */



