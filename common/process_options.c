/*
    Copyright 2009 David Hill

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
	This file contains functions declared in process_options.h. For usage,
	see that file.

	IMPORTANT: Any configuration macros must be defined BEFORE including
	this file. Failure to do so can result in discrepencies between the
	header and the output.
*/

#include "process_options.h"



#if defined(PROCESS_OPTION_C) || ( defined(__FILE_defined) && defined(EOF) )
int fput_opt(const char* opt, FILE* out)
{
	size_t index = 0;

	for (index; opt[index] && opt[index] != '='; ++index)
	{
		if (fputc(opt[index], out) != opt[index])
			return -1;
	}

	return index;
}
#endif

#ifndef PROCESS_OPTION_C
std::ostream& put_opt(std::ostream& out, const char* opt)
{
	size_t index = 0;

	for (index = 0; opt[index] && opt[index] != '='; ++index)
		out << opt[index];

	return out;
}
#endif

int cmp_opt(const char* s1, const char* s2)
{
	size_t index;

	for (index = 0; s1[index] && s1[index] != '='; ++index)
	{
		if (s2[index] == 0 || s2[index] == '=')
			return index;

		if (s1[index] != s2[index])
			return -1;
	}

	if (s2[index] == 0 || s2[index] == '=')
		return index+1;

	return index;
}

#ifdef PROCESS_OPTION_C
#define ERROR_BAD_BASE						\
if (1)								\
{								\
	fprintf(stderr, "unsupported base: %i\n", base);	\
								\
	PROCESS_OPTION_USER_ERROR;				\
								\
	return negVal ? -value : value;				\
}								\
else (void) 0
#else
#define ERROR_BAD_BASE						\
if (1)								\
{								\
	std::cerr << "unsupported base: " << base << "\n";	\
								\
	PROCESS_OPTION_USER_ERROR;				\
								\
	return negVal ? -value : value;				\
}								\
else (void) 0
#endif

#ifdef PROCESS_OPTION_C
#define ERROR_BAD_NUM(EXPR)					\
if (EXPR)							\
{								\
	fprintf(stderr, "expected a number, got: '%s'\n", str);	\
								\
	PROCESS_OPTION_USER_ERROR;				\
								\
	return negVal ? -value : value;				\
}								\
else (void) 0
#else
#define ERROR_BAD_NUM(EXPR)						\
if (EXPR)								\
{									\
	std::cerr << "expected a number, got: '" << str << "'\n";	\
									\
	PROCESS_OPTION_USER_ERROR;					\
									\
	return negVal ? -value : value;					\
}									\
else (void) 0
#endif

#define ERROR_BAD_NUM_8 ERROR_BAD_NUM(!isdigit(strChar) || strChar == '8' || strChar == '9')
#define ERROR_BAD_NUM_10 ERROR_BAD_NUM(!isdigit(strChar))
#define ERROR_BAD_NUM_16 ERROR_BAD_NUM(!isxdigit(strChar))

#define CHAR_TO_NUM_8(C) ((C) - '0')
#define CHAR_TO_NUM_10(C) ((C) - '0')
#define CHAR_TO_NUM_16(C) ((C) <= '9' ? ((C) - '0') : (toupper(C) - 'A'))

#define INCR_BY_CHAR_8(VAR, C)		\
if (1)					\
{					\
	ERROR_BAD_NUM_8;		\
	VAR *= 8;			\
	VAR += CHAR_TO_NUM_8(C);	\
}					\
else (void) 0

#define INCR_BY_CHAR_10(VAR, C)		\
if (1)					\
{					\
	ERROR_BAD_NUM_10;		\
	VAR *= 10;			\
	VAR += CHAR_TO_NUM_10(C);	\
}					\
else (void) 0

#define INCR_BY_CHAR_16(VAR, C)		\
if (1)					\
{					\
	ERROR_BAD_NUM_16;		\
	VAR *= 16;			\
	VAR += CHAR_TO_NUM_16(C);	\
}					\
else (void) 0

#define CHECK_BASE				\
if (base == 0)					\
{						\
	if (strChar == '0')			\
	{					\
		strChar = *++strSub;		\
						\
		if (strChar == 'x')		\
		{				\
			base = 16;		\
			strChar = *++strSub;	\
		}				\
		else if (strChar == 0)		\
		{				\
			return 0;		\
		}				\
		else				\
		{				\
			base = 8;		\
		}				\
	}					\
	else					\
	{					\
		base = 10;			\
	}					\
}						\
else (void) 0

#define CHECK_SIGN(VAR)		\
if (strChar == '-')		\
{				\
	VAR = true;		\
	strChar = *++strSub;	\
}				\
else if (strChar == '+')	\
{				\
	strChar = *++strSub;	\
}				\
else (void) 0

float_opt_t process_option_string_to_float(const char* str, int base)
{
	bool        negVal  = false;
	char        strChar = *str;
	const char* strSub  = str;
	float_opt_t value   = 0;

	CHECK_SIGN(negVal);
	CHECK_BASE;

	if (base == 10)
	{
		for (; strChar && strChar != '.' && strChar != 'e' && strChar != 'E'; strChar = *++strSub)
			INCR_BY_CHAR_10(value, strChar);

		if (strChar == '.')
		{
			size_t index = 1;

			strChar = *++strSub;

			for (; strChar && strChar != 'e' && strChar != 'E'; strChar = *++strSub, ++index)
			{
				ERROR_BAD_NUM_10;

				{
					size_t      index2 = index;
					float_opt_t value2 = CHAR_TO_NUM_10(strChar);

					for (; index2; --index2)
						value2 /= 10;

					value += value2;
				}
			}
		}

		if (strChar == 'e' || strChar == 'E')
		{
			int_opt_t exp    = 0;
			bool                 negExp = false;

			strChar = *++strSub;

			CHECK_SIGN(negExp);

			for (; strChar; strChar = *++strSub)
				INCR_BY_CHAR_10(exp, strChar);

			if (negExp) while (exp--) value /= 10;
			else        while (exp--) value *= 10;
		}
	}
	else
		ERROR_BAD_BASE;

	return negVal ? -value : value;
}

int_opt_t process_option_string_to_int(const char* str, int base)
{
	bool        negVal  = false;
	char        strChar = *str;
	const char* strSub  = str;
	int_opt_t   value   = 0;

	CHECK_SIGN(negVal);
	CHECK_BASE;

	if (base == 10) for (; strChar; strChar = *++strSub)
		INCR_BY_CHAR_10(value, strChar);

	else if (base == 8) for (; strChar; strChar = *++strSub)
		INCR_BY_CHAR_8(value, strChar);

	else if (base == 16) for (; strChar; strChar = *++strSub)
		INCR_BY_CHAR_16(value, strChar);

	else
		ERROR_BAD_BASE;

	return negVal ? -value : value;
}

/*
	These macros exist purely to generate the above functions.
*/

#undef ERROR_BAD_BASE

#undef ERROR_BAD_NUM
#undef ERROR_BAD_NUM_8
#undef ERROR_BAD_NUM_10
#undef ERROR_BAD_NUM_16

#undef CHAR_TO_NUM_8
#undef CHAR_TO_NUM_10
#undef CHAR_TO_NUM_16

#undef INCR_BY_CHAR_8
#undef INCR_BY_CHAR_10
#undef INCR_BY_CHAR_16

#undef CHECK_BASE
#undef CHECK_SIGN



