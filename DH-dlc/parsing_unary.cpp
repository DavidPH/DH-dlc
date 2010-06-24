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

#include "parsing.hpp"

#include <sstream>

#include "global_object.hpp"
#include "LevelObjectName.hpp"
#include "math.hpp"
#include "options.hpp"
#include "types.hpp"
#include "../common/string_funcs.hpp"
#include "exceptions/UnknownFunctionException.hpp"
#include "types/binary.hpp"
#include "types/int_t.hpp"
#include "types/real_t.hpp"
#include "types/string_t.hpp"



#define CONST_PI(TYPE)			\
if (opString == function_name_pi())	\
	return to_##TYPE(pi());		\
else (void) 0

#define CONST_RANDOM(TYPE)		\
if (opString == function_name_random())	\
	return random_##TYPE();		\
else (void) 0



#define CONST_common(TYPE)	\
(void) 0

#define CONST_common_int(TYPE)	\
CONST_common(TYPE)

#define CONST_common_real(TYPE)	\
CONST_common(TYPE);		\
CONST_PI(TYPE);			\
CONST_RANDOM(TYPE)



#define UNARY_basic(TYPE, NAME)			\
if (opString == function_name_##NAME())		\
	return NAME(parse_##TYPE(value));	\
else (void) 0

#define UNARY_ABS(TYPE)  UNARY_basic(TYPE, abs)
#define UNARY_ACOS(TYPE) UNARY_basic(TYPE, acos)
#define UNARY_ASIN(TYPE) UNARY_basic(TYPE, asin)
#define UNARY_ATAN(TYPE) UNARY_basic(TYPE, atan)
#define UNARY_COS(TYPE)  UNARY_basic(TYPE, cos)
#define UNARY_SIN(TYPE)  UNARY_basic(TYPE, sin)
#define UNARY_SQRT(TYPE) UNARY_basic(TYPE, sqrt)
#define UNARY_TAN(TYPE)  UNARY_basic(TYPE, tan)

#define UNARY_clamp(TYPE, NAME, MAX, CALC_TYPE)			\
if (opString == function_name_##NAME())				\
{								\
	CALC_TYPE##_t clampValue =				\
		to_##CALC_TYPE(parse_##TYPE(value));		\
								\
	while (clampValue >= CALC_TYPE##_t(MAX))       clampValue -= CALC_TYPE##_t(MAX);	\
	while (clampValue < CALC_TYPE##_t(int_s_t(0))) clampValue += CALC_TYPE##_t(MAX);	\
								\
	return to_##TYPE(clampValue);				\
}								\
else (void) 0

#define UNARY_BYTEANGLE(TYPE) UNARY_clamp(TYPE, byteangle, int_s_t(256), TYPE)
#define UNARY_DEGREES(TYPE)   UNARY_clamp(TYPE, degrees,   int_s_t(360), TYPE)
#define UNARY_RADIANS(TYPE)   UNARY_clamp(TYPE, radians,   to_real_l(pi()*real_t(2)), real_l)

#define UNARY_convert(TYPE, NAME, EXPR)						\
if (opString == function_name_##NAME())						\
	return to_##TYPE(real_t( to_real(parse_##TYPE(value)) * (EXPR) ));	\
else (void) 0

#define UNARY_BYTE2DEG(TYPE) UNARY_convert(TYPE, byte2deg, (real_t(360.0) / real_t(256.0)))
#define UNARY_BYTE2RAD(TYPE) UNARY_convert(TYPE, byte2rad, (pi()          / real_t(128.0)))
#define UNARY_DEG2BYTE(TYPE) UNARY_convert(TYPE, deg2byte, (real_t(256.0) / real_t(360.0)))
#define UNARY_DEG2RAD(TYPE)  UNARY_convert(TYPE, deg2rad,  (pi()          / real_t(180.0)))
#define UNARY_RAD2BYTE(TYPE) UNARY_convert(TYPE, rad2byte, (real_t(128.0) / pi()))
#define UNARY_RAD2DEG(TYPE)  UNARY_convert(TYPE, rad2deg,  (real_t(180.0) / pi()))

#define UNARY_RANDOM(TYPE)				\
if (opString == function_name_random())			\
	return random_##TYPE(parse_##TYPE(value));	\
else (void) 0

#define UNARY_ROUND(TYPE) \
if (opString == function_name_round()) \
	return to_##TYPE(to_int_l(TYPE##_t(parse_##TYPE(value) + TYPE##_t(0.5)))); \
else (void) 0

#define UNARY_common(TYPE)	\
UNARY_ABS(TYPE);		\
UNARY_BYTEANGLE(TYPE);		\
UNARY_BYTE2DEG(TYPE);		\
UNARY_BYTE2RAD(TYPE);		\
UNARY_DEG2BYTE(TYPE);		\
UNARY_DEG2RAD(TYPE);		\
UNARY_DEGREES(TYPE);		\
UNARY_RAD2BYTE(TYPE);		\
UNARY_RAD2DEG(TYPE);		\
UNARY_RADIANS(TYPE);		\
UNARY_RANDOM(TYPE);		\
UNARY_SQRT(TYPE)

#define UNARY_common_int(TYPE)	\
UNARY_common(TYPE)

#define UNARY_common_real(TYPE)	\
UNARY_common(TYPE);		\
UNARY_ACOS(TYPE);		\
UNARY_ASIN(TYPE);		\
UNARY_ATAN(TYPE);		\
UNARY_COS(TYPE);		\
UNARY_ROUND(TYPE);		\
UNARY_SIN(TYPE);		\
UNARY_TAN(TYPE)



bool_t parse_bool_unary(std::string const & opString)
{
	if (opString == function_name_random())
		return random_int_s(0, 1) != 0;

	throw UnknownFunctionException("["+opString+"]");
}
bool_t parse_bool_unary(std::string const & opString, std::string const & value)
{
	if (opString == function_name_exists())
		return has_object(name_t(value));

	if (opString == function_name_not())
		return !parse_bool(value);

	throw UnknownFunctionException("["+opString+"]");
}



int_s_t parse_int_s_unary(std::string const & opString)
{
	CONST_common_int(int_s);

	throw UnknownFunctionException("["+opString+"]");
}
int_s_t parse_int_s_unary(std::string const & opString, std::string const & value)
{
	UNARY_common_int(int_s);

	throw UnknownFunctionException("["+opString+"]");
}

int_t parse_int_unary(std::string const & opString)
{
	CONST_common_int(int);

	throw UnknownFunctionException("["+opString+"]");
}
int_t parse_int_unary(std::string const & opString, std::string const & value)
{
	UNARY_common_int(int);

	throw UnknownFunctionException("["+opString+"]");
}

int_l_t parse_int_l_unary(std::string const & opString)
{
	CONST_common_int(int_l);

	throw UnknownFunctionException("["+opString+"]");
}
int_l_t parse_int_l_unary(std::string const & opString, std::string const & value)
{
	UNARY_common_int(int_l);

	throw UnknownFunctionException("["+opString+"]");
}



real_s_t parse_real_s_unary(std::string const & opString)
{
	CONST_common_real(real_s);

	throw UnknownFunctionException("["+opString+"]");
}
real_s_t parse_real_s_unary(std::string const & opString, std::string const & value)
{
	UNARY_common_real(real_s);

	throw UnknownFunctionException("["+opString+"]");
}

real_t parse_real_unary(std::string const & opString)
{
	CONST_common_real(real);

	throw UnknownFunctionException("["+opString+"]");
}
real_t parse_real_unary(std::string const & opString, std::string const & value)
{
	UNARY_common_real(real);

	throw UnknownFunctionException("["+opString+"]");
}

real_l_t parse_real_l_unary(std::string const & opString)
{
	CONST_common_real(real_l);

	throw UnknownFunctionException("["+opString+"]");
}
real_l_t parse_real_l_unary(std::string const & opString, std::string const & value)
{
	UNARY_common_real(real_l);

	throw UnknownFunctionException("["+opString+"]");
}



string_t parse_string_unary(std::string const & opString)
{
	if (opString == function_name_mapname())
		return string_t(option_map_name);

	throw UnknownFunctionException("["+opString+"]");
}
string_t parse_string_unary(std::string const & opString, std::string const & value)
{
	if (opString == function_name_lower())
		return string_t(tolower(value));

	// Makes the string suitable for putting into an embedded script.
	// Note that this is different from the quoting done for UDMF output.
	// This is intentional, this has to catch more than the UDMF quoting so
	// as to be more generic.
	if (opString == function_name_quote())
	{
		std::ostringstream oss;
		std::istringstream iss(value);

		oss.put('"');

		for (int c = iss.get(); c != -1; c = iss.get())
		{
			switch (c)
			{
				case '\\':
				case '"':
					oss.put('\\');
					oss.put(c);
					break;

				case '\r':
					oss.put('\\');
					oss.put('r');
					break;

				case '\n':
					oss.put('\\');
					oss.put('n');
					break;

				case '\t':
					oss.put('\\');
					oss.put('t');
					break;

				default:
					oss.put(c);
					break;
			}
		}

		oss.put('"');

		return string_t(oss.str());
	}

	if (opString == function_name_upper())
		return string_t(toupper(value));

	throw UnknownFunctionException("["+opString+"]");
}

string8_t parse_string8_unary(std::string const & opString)
{
	if (opString == function_name_mapname())
		return string8_t(option_map_name);

	throw UnknownFunctionException("["+opString+"]");
}
string8_t parse_string8_unary(std::string const & opString, std::string const & value)
{
	if (opString == function_name_lower())
		return string8_t(tolower(value));

	if (opString == function_name_upper())
		return string8_t(toupper(value));

	throw UnknownFunctionException("["+opString+"]");
}

sword_t parse_sword_unary(std::string const & opString)
{
	CONST_common_int(sword);

	throw UnknownFunctionException("["+opString+"]");
}
sword_t parse_sword_unary(std::string const & opString, std::string const & value)
{
	throw UnknownFunctionException("["+opString+"]");
}

ubyte_t parse_ubyte_unary(std::string const & opString)
{
	CONST_common_int(ubyte);

	throw UnknownFunctionException("["+opString+"]");
}
ubyte_t parse_ubyte_unary(std::string const & opString, std::string const & value)
{
	throw UnknownFunctionException("["+opString+"]");
}

uword_t parse_uword_unary(std::string const & opString)
{
	CONST_common_int(uword);

	throw UnknownFunctionException("["+opString+"]");
}
uword_t parse_uword_unary(std::string const & opString, std::string const & value)
{
	throw UnknownFunctionException("["+opString+"]");
}




