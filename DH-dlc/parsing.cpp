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
	2010/02/14 - If an invalid type cast in requested, ignore it. Otherwise
		(1+(float)([pi])) demands that 1+(float be a valid cast. If it
		is genuinely an attempt to use a bad type for casting, it will
		still be an error anyways. I'll re-add the check if I can think
		of a good way of doing it.
	2010/05/25 - Replaced the function generating macros with templates.
*/

#include "parsing.hpp"

#include <cmath>

#include "global_object.hpp"
#include "LevelObjectData.hpp"
#include "LevelObjectName.hpp"
#include "math.hpp"
#include "options.hpp"
#include "types.hpp"
#include "exceptions/FunctionException.hpp"
#include "exceptions/InvalidTypeException.hpp"
#include "exceptions/ParsingException.hpp"
#include "types/binary.hpp"
#include "types/int_t.hpp"
#include "types/real_t.hpp"
#include "types/string_t.hpp"



template <class T>
struct ParsingData
{
	ParsingData(std::string const & valueIn) :
	valueReturn(), value(valueIn), valueLeft(), valueRight(),
	operatorChar(-2),
	hasBracket(0)
	{
		int bracketCount = 0;

		for (size_t index = value.size()-1; ; --index)
		{
			char indexChar = value[index];

			if (indexChar == '(')
			{
				++bracketCount;
				hasBracket = true;
			}
			else if (indexChar == ')')
			{
				--bracketCount;
				hasBracket = true;
			}
			else if (isoperator(indexChar))
			{
				// Don't break on exponent operator.
				if (
				(indexChar == '+' || indexChar == '-')           &&
				(index >= 2 && index < value.size()-1)           &&
				isdigit(value[index-2])                          &&
				(value[index-1] == 'e' || value[index-1] == 'E') &&
				isdigit(value[index+1])
				)
				{

				}
				else if (bracketCount == 0)
				{
					operatorChar = indexChar;

					valueLeft  = value.substr(0, index);
					valueRight = value.substr(index+1);
				}
			}

			if (operatorChar != -2 || index == 0)
				break;
		}

		if (bracketCount != 0)
			throw ParsingException("unbalanced parentheses:" + value);
	}

	T valueReturn;

	std::string value;
	std::string valueLeft;
	std::string valueRight;

	int operatorChar;

	unsigned hasBracket : 1;
};

template <class T, T (Tfunc)(std::string const &, std::vector<std::string> const &)>
static bool check_function(ParsingData<T> * data)
{
	if (data->value[0] != '<') return false;

	size_t bracketIndex = data->value.find('>');

	if (bracketIndex == std::string::npos) return false;

	if (bracketIndex == data->value.size()-1)
		throw FunctionException("missing argument list");

	std::string opString = data->value.substr(1, bracketIndex-1);
	std::string newValue = data->value.substr(bracketIndex+1);

	std::vector<std::string> args = parse_args(newValue.substr(1, newValue.size()-2));

	data->valueReturn = Tfunc(opString, args);

	return true;
}

template <class T, T (Tparse)(std::string const &)>
static bool check_math_bool(ParsingData<T> * data)
{
	switch (data->operatorChar)
	{
		case -2: return false;

		case '&': data->valueReturn = Tparse(data->valueLeft) && Tparse(data->valueRight); return true;
		case '|': data->valueReturn = Tparse(data->valueLeft) || Tparse(data->valueRight); return true;

		default: throw ParsingException(std::string("non-bool operator:") + (char)data->operatorChar);
	}
}
template <class T, T (Tparse)(std::string const &)>
static bool check_math_int(ParsingData<T> * data)
{
	switch (data->operatorChar)
	{
		case -2: return false;

		case '*': data->valueReturn = Tparse(data->valueLeft) * Tparse(data->valueRight); return true;
		case '/': data->valueReturn = Tparse(data->valueLeft) / Tparse(data->valueRight); return true;
		case '%': data->valueReturn = Tparse(data->valueLeft) % Tparse(data->valueRight); return true;
		case '+': data->valueReturn = Tparse(data->valueLeft) + Tparse(data->valueRight); return true;
		case '-': data->valueReturn = Tparse(data->valueLeft) - Tparse(data->valueRight); return true;
		case '&': data->valueReturn = Tparse(data->valueLeft) & Tparse(data->valueRight); return true;
		case '|': data->valueReturn = Tparse(data->valueLeft) | Tparse(data->valueRight); return true;

		default: throw ParsingException(std::string("non-int operator:") + (char)data->operatorChar);
	}
}
template <class T, T (Tparse)(std::string const &)>
static bool check_math_real(ParsingData<T> * data)
{
	switch (data->operatorChar)
	{
		case -2: return false;

		case '*': data->valueReturn = Tparse(data->valueLeft) * Tparse(data->valueRight); return true;
		case '/': data->valueReturn = Tparse(data->valueLeft) / Tparse(data->valueRight); return true;
		case '+': data->valueReturn = Tparse(data->valueLeft) + Tparse(data->valueRight); return true;
		case '-': data->valueReturn = Tparse(data->valueLeft) - Tparse(data->valueRight); return true;

		default: throw ParsingException(std::string("non-float operator:") + (char)data->operatorChar);
	}
}
template <class T, T (Tparse)(std::string const &)>
static bool check_math_string(ParsingData<T> * data)
{
	switch (data->operatorChar)
	{
		case -2: return false;

		case '+': data->valueReturn = Tparse(data->valueLeft) + Tparse(data->valueRight); return true;

		default: throw ParsingException(std::string("non-string operator:") + (char)data->operatorChar);
	}
}

template <class T, T (Tparse)(std::string const &)>
static bool check_math(ParsingData<T> * data)
{
	return check_math_int<T, Tparse>(data);
}
template <class T, T (Tparse)(std::string const &)>
static bool check_math(ParsingData<bool_t> * data)
{
	return check_math_bool<bool_t, parse_bool>(data);
}
template <class T, T (Tparse)(std::string const &)>
static bool check_math(ParsingData<real_s_t> * data)
{
	return check_math_real<real_s_t, parse_real_s>(data);
}
template <class T, T (Tparse)(std::string const &)>
static bool check_math(ParsingData<real_t> * data)
{
	return check_math_real<real_t, parse_real>(data);
}
template <class T, T (Tparse)(std::string const &)>
static bool check_math(ParsingData<real_l_t> * data)
{
	return check_math_real<real_l_t, parse_real_l>(data);
}
template <class T, T (Tparse)(std::string const &)>
static bool check_math(ParsingData<string_t> * data)
{
	return check_math_string<string_t, parse_string>(data);
}
template <class T, T (Tparse)(std::string const &)>
static bool check_math(ParsingData<string8_t> * data)
{
	return check_math_string<string8_t, parse_string8>(data);
}

template <class T, T (Tconv)(any_t const &)>
static bool check_typecast(ParsingData<T> * data)
{
	if (data->value[0] != '(') return false;

	size_t bracketIndex = data->value.find(')');

	if (bracketIndex == std::string::npos) return false;

	std::string opString = data->value.substr(1, bracketIndex-1);
	std::string newValue = "";

	// Assign newValue only if there actually is one.
	if (bracketIndex != data->value.size()-1)
		newValue = data->value.substr(bracketIndex+1);

	if (opString == type_name_bool())
	{
		data->valueReturn = Tconv(to_any(parse_bool(newValue)));
		return true;
	}

	if (opString == type_name_shortint())
	{
		data->valueReturn = Tconv(to_any(parse_int_s(newValue)));
		return true;
	}
	if (opString == type_name_int())
	{
		data->valueReturn = Tconv(to_any(parse_int(newValue)));
		return true;
	}
	if (opString == type_name_longint())
	{
		data->valueReturn = Tconv(to_any(parse_int_l(newValue)));
		return true;
	}

	if (opString == type_name_shortfloat())
	{
		data->valueReturn = Tconv(to_any(parse_real_s(newValue)));
		return true;
	}
	if (opString == type_name_float())
	{
		data->valueReturn = Tconv(to_any(parse_real(newValue)));
		return true;
	}
	if (opString == type_name_longfloat())
	{
		data->valueReturn = Tconv(to_any(parse_real_l(newValue)));
		return true;
	}

	if (opString == type_name_string())
	{
		data->valueReturn = Tconv(to_any(parse_string(newValue)));
		return true;
	}

	return false;
}

template <class T, T (Tconst)(std::string const &), T (Tunary)(std::string const &, std::string const &)>
static bool check_unary(ParsingData<T> * data)
{
	if (data->value[0] != '[') return false;

	size_t bracketIndex = data->value.find(']');

	if (bracketIndex == std::string::npos) return false;

	std::string opString = data->value.substr(1, bracketIndex-1);

	if (bracketIndex != data->value.size()-1)
		data->valueReturn = Tunary(opString, data->value.substr(bracketIndex+1));
	else
		data->valueReturn = Tconst(opString);

	return true;
}

template <class T, T (Tparse)(std::string const &), T (Tconv)(any_t const &), T (Tconst)(std::string const &), T (Tunary)(std::string const &, std::string const &), T (Tfunc)(std::string const &, std::vector<std::string> const &)>
static bool check_all(ParsingData<T> * data)
{
	if (check_math<T, Tparse>(data))
		return true;

	if (check_typecast<T, Tconv>(data))
		return true;

	if (check_unary<T, Tconst, Tunary>(data))
		return true;

	if (check_function<T, Tfunc>(data))
		return true;

	if (data->hasBracket)
	{
		data->valueReturn = Tparse(data->value.substr(1, data->value.length()-2));
		return true;
	}

	return false;
}



std::vector<std::string> parse_args(std::string const & value)
{
	std::vector<std::string> args;

	int    bracketCount = 0;
	size_t lastSep      = 0;

	for (size_t index = 0; index < value.size(); ++index)
	{
		if (value[index] == '(')
			++bracketCount;

		else if (value[index] == ')')
			--bracketCount;

		if (value[index] == ',' && bracketCount == 0)
		{
			args.push_back( value.substr(lastSep, index-lastSep) );

			lastSep = index+1;
		}
	}

	if (lastSep < value.size())
		args.push_back( value.substr(lastSep) );

	return args;
}



bool_t parse_bool(std::string const & value)
{
	if (value.empty())
		return false;

	if (value == "true" || value == "TRUE")
		return true;

	if (value == "false" || value == "FALSE")
		return false;

	ParsingData<bool_t> data(value);

	if (check_all<bool_t, parse_bool, to_bool, parse_bool_unary, parse_bool_unary, parse_bool_function>(&data))
		return data.valueReturn;

	return to_bool(get_object(name_t(value)));
}

template <class T, T (Tparse)(std::string const &), T (Tconv)(any_t const &), T (Tconst)(std::string const &), T (Tunary)(std::string const &, std::string const &), T (Tfunc)(std::string const &, std::vector<std::string> const &)>
static T parse_num_base(std::string const & value)
{
	if (value.empty()) return T();

	ParsingData<T> data(value);

	if (check_all<T, Tparse, Tconv, Tconst, Tunary, Tfunc>(&data))
		return data.valueReturn;

	size_t pos = value.find_first_of("+-");

	if (pos != std::string::npos)
	{
		if (isdigit(value[0]))
			return Tconv(string_t(value));

		std::string valueLeft(value, 0, pos);
		std::string valueRight(value, pos+1);

		if (value[pos] == '+')
			return Tparse(valueLeft) + Tparse(valueRight);

		if (value[pos] == '-')
			return Tparse(valueLeft) - Tparse(valueRight);
	}

	if (isdigit(value[0]))
		return Tconv(string_t(value));

	return Tconv(get_object(name_t(value)));
}

int_s_t parse_int_s(std::string const & value)
{
	return parse_num_base<int_s_t, parse_int_s, to_int_s, parse_int_s_unary, parse_int_s_unary, parse_int_s_function>(value);
}
int_t parse_int(std::string const & value)
{
	return parse_num_base<int_t, parse_int, to_int, parse_int_unary, parse_int_unary, parse_int_function>(value);
}
int_l_t parse_int_l(std::string const & value)
{
	return parse_num_base<int_l_t, parse_int_l, to_int_l, parse_int_l_unary, parse_int_l_unary, parse_int_l_function>(value);
}

obj_t parse_obj(std::string const & value, type_t const type)
{
	if (has_object(name_t(value)))
		return get_object(name_t(value), type);

	return get_object(parse_int_s(value), type);
}

real_s_t parse_real_s(std::string const & value)
{
	return parse_num_base<real_s_t, parse_real_s, to_real_s, parse_real_s_unary, parse_real_s_unary, parse_real_s_function>(value);
}
real_t parse_real(std::string const & value)
{
	return parse_num_base<real_t, parse_real, to_real, parse_real_unary, parse_real_unary, parse_real_function>(value);
}
real_l_t parse_real_l(std::string const & value)
{
	return parse_num_base<real_l_t, parse_real_l, to_real_l, parse_real_l_unary, parse_real_l_unary, parse_real_l_function>(value);
}

template <class T, T (Tparse)(std::string const &), T (Tconv)(any_t const &), T (Tconst)(std::string const &), T (Tunary)(std::string const &, std::string const &), T (Tfunc)(std::string const &, std::vector<std::string> const &)>
static T parse_string_base(std::string const & value)
{
	if (value.empty())
		return T(value);

	if (value[0] == '$')
		return T(value.substr(1));

	ParsingData<T> data(value);

	if (check_all<T, Tparse, Tconv, Tconst, Tunary, Tfunc>(&data))
		return data.valueReturn;

	// This is required to be able to parse UDMF.
	if (!option_strict_strings && !has_object(name_t(value)))
		return T(value);

	return Tconv(get_object(name_t(value)));
}

string_t parse_string(std::string const & value)
{
	return parse_string_base<string_t, parse_string, to_string, parse_string_unary, parse_string_unary, parse_string_function>(value);
}
string8_t parse_string8(std::string const & value)
{
	return parse_string_base<string8_t, parse_string8, to_string8, parse_string8_unary, parse_string8_unary, parse_string8_function>(value);
}

sword_t parse_sword(std::string const & value)
{
	return parse_num_base<sword_t, parse_sword, to_sword, parse_sword_unary, parse_sword_unary, parse_sword_function>(value);
}

ubyte_t parse_ubyte(std::string const & value)
{
	return parse_num_base<ubyte_t, parse_ubyte, to_ubyte, parse_ubyte_unary, parse_ubyte_unary, parse_ubyte_function>(value);
}

uword_t parse_uword(std::string const & value)
{
	return parse_num_base<uword_t, parse_uword, to_uword, parse_uword_unary, parse_uword_unary, parse_uword_function>(value);
}



