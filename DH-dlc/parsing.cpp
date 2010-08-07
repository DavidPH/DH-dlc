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
struct ParsingDataDDL
{
	ParsingDataDDL(std::string const & valueIn) :
	valueReturn(), value(valueIn), valueLeft(), valueRight(),
	operatorChar(-2),
	hasBracket(0), hasExponent(0)
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
			else if (indexChar == '[' || indexChar == '<')
			{
				++bracketCount;
			}
			else if (indexChar == ')')
			{
				--bracketCount;
				hasBracket = true;
			}
			else if (indexChar == ']' || indexChar == '>')
			{
				--bracketCount;
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
					hasExponent = true;
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

	unsigned hasBracket  : 1;
	unsigned hasExponent : 1;
};

template <class T, T (Tfunc)(std::string const &, std::vector<std::string> const &)>
static bool check_function(ParsingDataDDL<T> * data)
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
static bool check_math_bool(ParsingDataDDL<T> * data)
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
static bool check_math_int(ParsingDataDDL<T> * data)
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
static bool check_math_real(ParsingDataDDL<T> * data)
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
static bool check_math_string(ParsingDataDDL<T> * data)
{
	switch (data->operatorChar)
	{
		case -2: return false;

		case '+': data->valueReturn = Tparse(data->valueLeft) + Tparse(data->valueRight); return true;

		default: throw ParsingException(std::string("non-string operator:") + (char)data->operatorChar);
	}
}

template <class T, T (Tparse)(std::string const &)>
static bool check_math(ParsingDataDDL<T> * data)
{
	return check_math_int<T, Tparse>(data);
}
template <class T, T (Tparse)(std::string const &)>
static bool check_math(ParsingDataDDL<bool_t> * data)
{
	return check_math_bool<bool_t, parse_bool>(data);
}
template <class T, T (Tparse)(std::string const &)>
static bool check_math(ParsingDataDDL<real_s_t> * data)
{
	return check_math_real<real_s_t, parse_real_s>(data);
}
template <class T, T (Tparse)(std::string const &)>
static bool check_math(ParsingDataDDL<real_t> * data)
{
	return check_math_real<real_t, parse_real>(data);
}
template <class T, T (Tparse)(std::string const &)>
static bool check_math(ParsingDataDDL<real_l_t> * data)
{
	return check_math_real<real_l_t, parse_real_l>(data);
}
template <class T, T (Tparse)(std::string const &)>
static bool check_math(ParsingDataDDL<string_t> * data)
{
	return check_math_string<string_t, parse_string>(data);
}
template <class T, T (Tparse)(std::string const &)>
static bool check_math(ParsingDataDDL<string8_t> * data)
{
	return check_math_string<string8_t, parse_string8>(data);
}
template <class T, T (Tparse)(std::string const &)>
static bool check_math(ParsingDataDDL<string16_t> * data)
{
	return check_math_string<string16_t, parse_string16>(data);
}
template <class T, T (Tparse)(std::string const &)>
static bool check_math(ParsingDataDDL<string32_t> * data)
{
	return check_math_string<string32_t, parse_string32>(data);
}
template <class T, T (Tparse)(std::string const &)>
static bool check_math(ParsingDataDDL<string80_t> * data)
{
	return check_math_string<string80_t, parse_string80>(data);
}
template <class T, T (Tparse)(std::string const &)>
static bool check_math(ParsingDataDDL<string320_t> * data)
{
	return check_math_string<string320_t, parse_string320>(data);
}

template <class T, T (Tconv)(any_t const &)>
static bool check_typecast(ParsingDataDDL<T> * data)
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
		data->valueReturn = Tconv(parse_bool(newValue));
		return true;
	}

	if (opString == type_name_shortint())
	{
		data->valueReturn = Tconv(parse_int_s(newValue));
		return true;
	}
	if (opString == type_name_int())
	{
		data->valueReturn = Tconv(parse_int(newValue));
		return true;
	}
	if (opString == type_name_longint())
	{
		data->valueReturn = Tconv(parse_int_l(newValue));
		return true;
	}

	if (opString == type_name_shortfloat())
	{
		data->valueReturn = Tconv(parse_real_s(newValue));
		return true;
	}
	if (opString == type_name_float())
	{
		data->valueReturn = Tconv(parse_real(newValue));
		return true;
	}
	if (opString == type_name_longfloat())
	{
		data->valueReturn = Tconv(parse_real_l(newValue));
		return true;
	}

	if (opString == type_name_string())
	{
		data->valueReturn = Tconv(parse_string(newValue));
		return true;
	}

	return false;
}

template <class T, T (Tconst)(std::string const &), T (Tunary)(std::string const &, std::string const &)>
static bool check_unary(ParsingDataDDL<T> * data)
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
static bool check_all(ParsingDataDDL<T> * data)
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



template<TEMPLATE_TAKE_DHLX> T parse__base(SourceScannerDHLX &);

// This will be removed once all LevelObjectData types are class'd.
template<typename T>
inline void parse__base_init(T & data)
{

}
template<>
inline void parse__base_init<bool_t>(bool_t & data)
{
	data = false;
}
template<>
inline void parse__base_init<int_s_t>(int_s_t & data)
{
	data = 0;
}
template<>
inline void parse__base_init<real_s_t>(real_s_t & data)
{
	data = 0;
}
template<TEMPLATE_TAKE_DHLX>
inline void parse__base_typecast(T & data, type_t const type, SourceScannerDHLX & sc)
{
	     if (type == type_t::type_bool())       data = convert<T, bool_t>(parse_bool(sc));

	else if (type == type_t::type_shortint())   data = convert<T, int_s_t>(parse_int_s(sc));
	else if (type == type_t::type_int())        data = convert<T, int_t>(parse_int(sc));
	else if (type == type_t::type_longint())    data = convert<T, int_l_t>(parse_int_l(sc));

	else if (type == type_t::type_shortfloat()) data = convert<T, real_s_t>(parse_real_s(sc));
	else if (type == type_t::type_float())      data = convert<T, real_t>(parse_real(sc));
	else if (type == type_t::type_longfloat())  data = convert<T, real_l_t>(parse_real_l(sc));

	else if (type == type_t::type_string())     data = convert<T, string_t>(parse_string(sc));
	else if (type == type_t::type_string8())    data = convert<T, string8_t>(parse_string8(sc));
	else if (type == type_t::type_string16())   data = convert<T, string16_t>(parse_string16(sc));
	else if (type == type_t::type_string32())   data = convert<T, string32_t>(parse_string32(sc));
	else if (type == type_t::type_string80())   data = convert<T, string80_t>(parse_string80(sc));
	else if (type == type_t::type_string320())  data = convert<T, string320_t>(parse_string320(sc));

	else if (type == type_t::type_ubyte())      data = convert<T, ubyte_t>(parse_ubyte(sc));
	else if (type == type_t::type_sword())      data = convert<T, sword_t>(parse_sword(sc));
	else if (type == type_t::type_uword())      data = convert<T, uword_t>(parse_uword(sc));
	else if (type == type_t::type_sdword())     data = convert<T, sdword_t>(parse_sdword(sc));
	else if (type == type_t::type_udword())     data = convert<T, udword_t>(parse_udword(sc));

	else
		throw ParsingException("unknown typecast:" + type.makeString());
}
template<TEMPLATE_TAKE_DHLX>
inline void parse__base_part_IDENTIFIER(T & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	sc.unget(st);
	data = convert<T, obj_t>(get_object(parse_name(sc)));
}
template<>
inline void parse__base_part_IDENTIFIER<TEMPLATE_PUSH_type(bool)>(bool_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	if (st.getData() == "true")
		data = true;
	else if (st.getData() == "false")
		data = false;
	else
	{
		sc.unget(st);
		data = convert<bool_t, obj_t>(get_object(parse_name(sc)));
	}
}
template<TEMPLATE_TAKE_DHLX>
inline void parse__base_part_NUMBER(T & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	sc.unget(st);
}
template<>
inline void parse__base_part_NUMBER<TEMPLATE_PUSH_type(int_s)>(int_s_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<int_s_t>(st.getData());
}
template<>
inline void parse__base_part_NUMBER<TEMPLATE_PUSH_type(int)>(int_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<int_t>(st.getData());
}
template<>
inline void parse__base_part_NUMBER<TEMPLATE_PUSH_type(int_l)>(int_l_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<int_l_t>(st.getData());
}
template<>
inline void parse__base_part_NUMBER<TEMPLATE_PUSH_type(real_s)>(real_s_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<real_s_t>(st.getData());
}
template<>
inline void parse__base_part_NUMBER<TEMPLATE_PUSH_type(real)>(real_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<real_t>(st.getData());
}
template<>
inline void parse__base_part_NUMBER<TEMPLATE_PUSH_type(real_l)>(real_l_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<real_l_t>(st.getData());
}
template<>
inline void parse__base_part_NUMBER<TEMPLATE_PUSH_type(ubyte)>(ubyte_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<ubyte_t>(st.getData());
}
template<>
inline void parse__base_part_NUMBER<TEMPLATE_PUSH_type(sword)>(sword_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<sword_t>(st.getData());
}
template<>
inline void parse__base_part_NUMBER<TEMPLATE_PUSH_type(uword)>(uword_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<uword_t>(st.getData());
}
template<>
inline void parse__base_part_NUMBER<TEMPLATE_PUSH_type(sdword)>(sdword_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<sdword_t>(st.getData());
}
template<>
inline void parse__base_part_NUMBER<TEMPLATE_PUSH_type(udword)>(udword_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<udword_t>(st.getData());
}
template<TEMPLATE_TAKE_DHLX>
inline void parse__base_part_STRING(T & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	sc.unget(st);
}
template<>
inline void parse__base_part_STRING<TEMPLATE_PUSH_type(string)>(string_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = string_t(st.getData());
}
template<>
inline void parse__base_part_STRING<TEMPLATE_PUSH_type(string8)>(string8_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = string8_t(st.getData());
}
template<>
inline void parse__base_part_STRING<TEMPLATE_PUSH_type(string16)>(string16_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = string16_t(st.getData());
}
template<>
inline void parse__base_part_STRING<TEMPLATE_PUSH_type(string32)>(string32_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = string32_t(st.getData());
}
template<>
inline void parse__base_part_STRING<TEMPLATE_PUSH_type(string80)>(string80_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = string80_t(st.getData());
}
template<>
inline void parse__base_part_STRING<TEMPLATE_PUSH_type(string320)>(string320_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = string320_t(st.getData());
}
template<TEMPLATE_TAKE_DHLX>
inline T parse__base_part(SourceScannerDHLX & sc)
{
	T data;

	parse__base_init<T>(data);

	SourceTokenDHLX st(sc.get());

	switch (st.getType())
	{
	case SourceTokenDHLX::TT_IDENTIFIER:
	{
		SourceTokenDHLX st2(sc.get());

		if (st2.getType() == SourceTokenDHLX::TT_OP_PARENTHESIS_O)
		{
			std::string function(st.getData());

			if (type_t::has_type(function))
				parse__base_typecast<TEMPLATE_PUSH>(data, type_t::get_type(function), sc);
			else
				data = Tfunc(function, sc);

			sc.get(SourceTokenDHLX::TT_OP_PARENTHESIS_C);
			break;
		}
		else
		{
			sc.unget(st2);
		}
	}
		parse__base_part_IDENTIFIER<TEMPLATE_PUSH>(data, st, sc);
		break;

	case SourceTokenDHLX::TT_NUMBER:
		parse__base_part_NUMBER<TEMPLATE_PUSH>(data, st, sc);
		break;

	case SourceTokenDHLX::TT_STRING:
		parse__base_part_STRING<TEMPLATE_PUSH>(data, st, sc);
		break;

	case SourceTokenDHLX::TT_OP_BRACKET_O:
		st = sc.get(SourceTokenDHLX::TT_IDENTIFIER);
		sc.get(SourceTokenDHLX::TT_OP_BRACKET_C);
		data = Tconst(st.getData());
		break;

	case SourceTokenDHLX::TT_OP_CMP_LT:
		st = sc.get(SourceTokenDHLX::TT_IDENTIFIER);
		sc.get(SourceTokenDHLX::TT_OP_CMP_GT);
		data = Tunary(st.getData(), sc);
		break;

	case SourceTokenDHLX::TT_OP_PARENTHESIS_O:
	{
		SourceTokenDHLX st2(sc.get());
		SourceTokenDHLX st3(sc.get());

		if ((st2.getType() == SourceTokenDHLX::TT_IDENTIFIER) && (st3.getType() == SourceTokenDHLX::TT_OP_PARENTHESIS_C) && type_t::has_type(st2.getData()))
		{
			parse__base_typecast<TEMPLATE_PUSH>(data, type_t::get_type(st2.getData()), sc);
			break;
		}
		else
		{
			sc.unget(st3);
			sc.unget(st2);
		}
	}
		data = parse__base<TEMPLATE_PUSH>(sc);
		sc.get(SourceTokenDHLX::TT_OP_PARENTHESIS_C);
		break;

	default:
		sc.unget(st);
		break;
	}

	return data;
}
template<TEMPLATE_TAKE_DHLX>
inline bool parse__base_math__bool(T & data, SourceScannerDHLX & sc)
{
	SourceTokenDHLX st(sc.get());

	switch (st.getType())
	{
	case SourceTokenDHLX::TT_OP_AND2:
		data = data && parse__base_part<TEMPLATE_PUSH>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_PIPE2:
		data = data || parse__base_part<TEMPLATE_PUSH>(sc);
		return true;

	default:
		sc.unget(st);
		return false;
	}
}
template<TEMPLATE_TAKE_DHLX>
inline bool parse__base_math__int(T & data, SourceScannerDHLX & sc)
{
	SourceTokenDHLX st(sc.get());

	switch (st.getType())
	{
	case SourceTokenDHLX::TT_OP_AND:
		data &= parse__base_part<TEMPLATE_PUSH>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_ASTERIX:
		data *= parse__base_part<TEMPLATE_PUSH>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_MINUS:
		data -= parse__base_part<TEMPLATE_PUSH>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_PERCENT:
		data %= parse__base_part<TEMPLATE_PUSH>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_PIPE:
		data |= parse__base_part<TEMPLATE_PUSH>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_PLUS:
		data += parse__base_part<TEMPLATE_PUSH>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_SLASH:
		data /= parse__base_part<TEMPLATE_PUSH>(sc);
		return true;

	default:
		sc.unget(st);
		return false;
	}
}
template<TEMPLATE_TAKE_DHLX>
inline bool parse__base_math__real(T & data, SourceScannerDHLX & sc)
{
	SourceTokenDHLX st(sc.get());

	switch (st.getType())
	{
	case SourceTokenDHLX::TT_OP_ASTERIX:
		data *= parse__base_part<TEMPLATE_PUSH>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_MINUS:
		data -= parse__base_part<TEMPLATE_PUSH>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_PLUS:
		data += parse__base_part<TEMPLATE_PUSH>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_SLASH:
		data /= parse__base_part<TEMPLATE_PUSH>(sc);
		return true;

	default:
		sc.unget(st);
		return false;
	}
}
template<TEMPLATE_TAKE_DHLX>
inline bool parse__base_math__string(T & data, SourceScannerDHLX & sc)
{
	SourceTokenDHLX st(sc.get());

	switch (st.getType())
	{
	case SourceTokenDHLX::TT_OP_PLUS:
		data += parse__base_part<TEMPLATE_PUSH>(sc);
		return true;

	default:
		sc.unget(st);
		return false;
	}
}
template<TEMPLATE_TAKE_DHLX>
inline bool parse__base_math(T & data, SourceScannerDHLX & sc)
{
	return false;
}
template<>
inline bool parse__base_math<TEMPLATE_PUSH_type(bool)>(bool_t & data, SourceScannerDHLX & sc)
{
	return parse__base_math__bool<TEMPLATE_PUSH_type(bool)>(data, sc);
}
template<>
inline bool parse__base_math<TEMPLATE_PUSH_type(int_s)>(int_s_t & data, SourceScannerDHLX & sc)
{
	return parse__base_math__int<TEMPLATE_PUSH_type(int_s)>(data, sc);
}
template<>
inline bool parse__base_math<TEMPLATE_PUSH_type(int)>(int_t & data, SourceScannerDHLX & sc)
{
	return parse__base_math__int<TEMPLATE_PUSH_type(int)>(data, sc);
}
template<>
inline bool parse__base_math<TEMPLATE_PUSH_type(int_l)>(int_l_t & data, SourceScannerDHLX & sc)
{
	return parse__base_math__int<TEMPLATE_PUSH_type(int_l)>(data, sc);
}
template<>
inline bool parse__base_math<TEMPLATE_PUSH_type(real_s)>(real_s_t & data, SourceScannerDHLX & sc)
{
	return parse__base_math__real<TEMPLATE_PUSH_type(real_s)>(data, sc);
}
template<>
inline bool parse__base_math<TEMPLATE_PUSH_type(real)>(real_t & data, SourceScannerDHLX & sc)
{
	return parse__base_math__real<TEMPLATE_PUSH_type(real)>(data, sc);
}
template<>
inline bool parse__base_math<TEMPLATE_PUSH_type(real_l)>(real_l_t & data, SourceScannerDHLX & sc)
{
	return parse__base_math__real<TEMPLATE_PUSH_type(real_l)>(data, sc);
}
template<>
inline bool parse__base_math<TEMPLATE_PUSH_type(string)>(string_t & data, SourceScannerDHLX & sc)
{
	return parse__base_math__string<TEMPLATE_PUSH_type(string)>(data, sc);
}
template<>
inline bool parse__base_math<TEMPLATE_PUSH_type(string8)>(string8_t & data, SourceScannerDHLX & sc)
{
	return parse__base_math__string<TEMPLATE_PUSH_type(string8)>(data, sc);
}
template<>
inline bool parse__base_math<TEMPLATE_PUSH_type(string16)>(string16_t & data, SourceScannerDHLX & sc)
{
	return parse__base_math__string<TEMPLATE_PUSH_type(string16)>(data, sc);
}
template<>
inline bool parse__base_math<TEMPLATE_PUSH_type(string32)>(string32_t & data, SourceScannerDHLX & sc)
{
	return parse__base_math__string<TEMPLATE_PUSH_type(string32)>(data, sc);
}
template<>
inline bool parse__base_math<TEMPLATE_PUSH_type(string80)>(string80_t & data, SourceScannerDHLX & sc)
{
	return parse__base_math__string<TEMPLATE_PUSH_type(string80)>(data, sc);
}
template<>
inline bool parse__base_math<TEMPLATE_PUSH_type(string320)>(string320_t & data, SourceScannerDHLX & sc)
{
	return parse__base_math__string<TEMPLATE_PUSH_type(string320)>(data, sc);
}
template<>
inline bool parse__base_math<TEMPLATE_PUSH_type(ubyte)>(ubyte_t & data, SourceScannerDHLX & sc)
{
	return parse__base_math__int<TEMPLATE_PUSH_type(ubyte)>(data, sc);
}
template<>
inline bool parse__base_math<TEMPLATE_PUSH_type(sword)>(sword_t & data, SourceScannerDHLX & sc)
{
	return parse__base_math__int<TEMPLATE_PUSH_type(sword)>(data, sc);
}
template<>
inline bool parse__base_math<TEMPLATE_PUSH_type(uword)>(uword_t & data, SourceScannerDHLX & sc)
{
	return parse__base_math__int<TEMPLATE_PUSH_type(uword)>(data, sc);
}
template<>
inline bool parse__base_math<TEMPLATE_PUSH_type(sdword)>(sdword_t & data, SourceScannerDHLX & sc)
{
	return parse__base_math__int<TEMPLATE_PUSH_type(sdword)>(data, sc);
}
template<>
inline bool parse__base_math<TEMPLATE_PUSH_type(udword)>(udword_t & data, SourceScannerDHLX & sc)
{
	return parse__base_math__int<TEMPLATE_PUSH_type(udword)>(data, sc);
}
template<TEMPLATE_TAKE_DHLX>
inline T parse__base(SourceScannerDHLX & sc)
{
	T data;

	parse__base_init<T>(data);

	data = parse__base_part<TEMPLATE_PUSH>(sc);

	while (parse__base_math<TEMPLATE_PUSH>(data, sc));

	return data;
}



bool_t parse_bool(SourceScannerDHLX & sc)
{
	return parse__base<TEMPLATE_PUSH_type(bool)>(sc);
}
bool_t parse_bool(std::string const & value)
{
	if (value.empty())
		return false;

	if (value == "true" || value == "TRUE")
		return true;

	if (value == "false" || value == "FALSE")
		return false;

	ParsingDataDDL<bool_t> data(value);

	if (check_all<bool_t, parse_bool, convert<bool_t, any_t>, parse_bool_const, parse_bool_unary, parse_bool_function>(&data))
		return data.valueReturn;

	return convert<bool_t, obj_t>(get_object(name_t(value)));
}

template <class T, T (Tparse)(std::string const &), T (Tconv)(any_t const &), T (Tconst)(std::string const &), T (Tunary)(std::string const &, std::string const &), T (Tfunc)(std::string const &, std::vector<std::string> const &)>
static T parse_num_base(std::string const & value)
{
	if (value.empty()) return T();

	ParsingDataDDL<T> data(value);

	if (check_all<T, Tparse, Tconv, Tconst, Tunary, Tfunc>(&data))
		return data.valueReturn;

	if (data.hasExponent)
	{
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
	}

	if (isdigit(value[0]))
		return Tconv(string_t(value));

	return Tconv(get_object(name_t(value)));
}

int_s_t parse_int_s(SourceScannerDHLX & sc)
{
	return parse__base<TEMPLATE_PUSH_type(int_s)>(sc);
}
int_s_t parse_int_s(std::string const & value)
{
	return parse_num_base<int_s_t, parse_int_s, convert<int_s_t, any_t>, parse_int_s_const, parse_int_s_unary, parse_int_s_function>(value);
}
int_t parse_int(SourceScannerDHLX & sc)
{
	return parse__base<TEMPLATE_PUSH_type(int)>(sc);
}
int_t parse_int(std::string const & value)
{
	return parse_num_base<int_t, parse_int, convert<int_t, any_t>, parse_int_const, parse_int_unary, parse_int_function>(value);
}
int_l_t parse_int_l(SourceScannerDHLX & sc)
{
	return parse__base<TEMPLATE_PUSH_type(int_l)>(sc);
}
int_l_t parse_int_l(std::string const & value)
{
	return parse_num_base<int_l_t, parse_int_l, convert<int_l_t, any_t>, parse_int_l_const, parse_int_l_unary, parse_int_l_function>(value);
}

name_t parse_name(SourceScannerDHLX & sc)
{
	std::vector<std::string> nameVector;

	SourceTokenDHLX nameToken(sc.get(SourceTokenDHLX::TT_IDENTIFIER));

	std::string nameElement(nameToken.getData());

	bool hasDynamic(false);

	while (true)
	{
		nameToken = sc.get();

		if (nameToken.getType() == SourceTokenDHLX::TT_OP_PERIOD)
		{
			nameVector.push_back(nameElement);
			nameElement = sc.get(SourceTokenDHLX::TT_IDENTIFIER).getData();
			hasDynamic  = false;
		}
		else if (nameToken.getType() == SourceTokenDHLX::TT_OP_BRACKET_O)
		{
			if (hasDynamic)
				nameElement += '.';
			nameElement += make_string(parse_int_s(sc));
			hasDynamic   = true;

			sc.get(SourceTokenDHLX::TT_OP_BRACKET_C);
		}
		else if (nameToken.getType() == SourceTokenDHLX::TT_OP_CMP_LT)
		{
			if (hasDynamic)
				nameElement += '.';
			nameElement += make_string(parse_string(sc));
			hasDynamic   = true;

			sc.get(SourceTokenDHLX::TT_OP_CMP_GT);
		}
		else
		{
			sc.unget(nameToken);
			break;
		}
	}

	nameVector.push_back(nameElement);

	return name_t(nameVector);
}

obj_t parse_obj(std::string const & value, type_t const type)
{
	if (has_object(name_t(value)))
		return get_object(name_t(value), type);

	return get_object(parse_int_s(value), type);
}

real_s_t parse_real_s(SourceScannerDHLX & sc)
{
	return parse__base<TEMPLATE_PUSH_type(real_s)>(sc);
}
real_s_t parse_real_s(std::string const & value)
{
	return parse_num_base<real_s_t, parse_real_s, convert<real_s_t, any_t>, parse_real_s_const, parse_real_s_unary, parse_real_s_function>(value);
}
real_t parse_real(SourceScannerDHLX & sc)
{
	return parse__base<TEMPLATE_PUSH_type(real)>(sc);
}
real_t parse_real(std::string const & value)
{
	return parse_num_base<real_t, parse_real, convert<real_t, any_t>, parse_real_const, parse_real_unary, parse_real_function>(value);
}
real_l_t parse_real_l(SourceScannerDHLX & sc)
{
	return parse__base<TEMPLATE_PUSH_type(real_l)>(sc);
}
real_l_t parse_real_l(std::string const & value)
{
	return parse_num_base<real_l_t, parse_real_l, convert<real_l_t, any_t>, parse_real_l_const, parse_real_l_unary, parse_real_l_function>(value);
}

template <class T, T (Tparse)(std::string const &), T (Tconv)(any_t const &), T (Tconst)(std::string const &), T (Tunary)(std::string const &, std::string const &), T (Tfunc)(std::string const &, std::vector<std::string> const &)>
static T parse_string_base(std::string const & value)
{
	if (value.empty())
		return T(value);

	if (value[0] == '$')
		return T(value.substr(1));

	ParsingDataDDL<T> data(value);

	if (check_all<T, Tparse, Tconv, Tconst, Tunary, Tfunc>(&data))
		return data.valueReturn;

	// This is required to be able to parse UDMF.
	if (!option_strict_strings && !has_object(name_t(value)))
		return T(value);

	return Tconv(get_object(name_t(value)));
}

string_t parse_string(SourceScannerDHLX & sc)
{
	return parse__base<TEMPLATE_PUSH_type(string)>(sc);
}
string_t parse_string(std::string const & value)
{
	return parse_string_base<string_t, parse_string, convert<string_t, any_t>, parse_string_const, parse_string_unary, parse_string_function>(value);
}
string8_t parse_string8(SourceScannerDHLX & sc)
{
	return parse__base<TEMPLATE_PUSH_type(string8)>(sc);
}
string8_t parse_string8(std::string const & value)
{
	return parse_string_base<string8_t, parse_string8, convert<string8_t, any_t>, parse_string8_const, parse_string8_unary, parse_string8_function>(value);
}
string16_t parse_string16(SourceScannerDHLX & sc)
{
	return parse__base<TEMPLATE_PUSH_type(string16)>(sc);
}
string16_t parse_string16(std::string const & value)
{
	return parse_string_base<string16_t, parse_string16, convert<string16_t, any_t>, parse_string16_const, parse_string16_unary, parse_string16_function>(value);
}
string32_t parse_string32(SourceScannerDHLX & sc)
{
	return parse__base<TEMPLATE_PUSH_type(string32)>(sc);
}
string32_t parse_string32(std::string const & value)
{
	return parse_string_base<string32_t, parse_string32, convert<string32_t, any_t>, parse_string32_const, parse_string32_unary, parse_string32_function>(value);
}
string80_t parse_string80(SourceScannerDHLX & sc)
{
	return parse__base<TEMPLATE_PUSH_type(string80)>(sc);
}
string80_t parse_string80(std::string const & value)
{
	return parse_string_base<string80_t, parse_string80, convert<string80_t, any_t>, parse_string80_const, parse_string80_unary, parse_string80_function>(value);
}
string320_t parse_string320(SourceScannerDHLX & sc)
{
	return parse__base<TEMPLATE_PUSH_type(string320)>(sc);
}
string320_t parse_string320(std::string const & value)
{
	return parse_string_base<string320_t, parse_string320, convert<string320_t, any_t>, parse_string320_const, parse_string320_unary, parse_string320_function>(value);
}

ubyte_t parse_ubyte(SourceScannerDHLX & sc)
{
	return parse__base<TEMPLATE_PUSH_type(ubyte)>(sc);
}
ubyte_t parse_ubyte(std::string const & value)
{
	return parse_num_base<ubyte_t, parse_ubyte, convert<ubyte_t, any_t>, parse_ubyte_const, parse_ubyte_unary, parse_ubyte_function>(value);
}
sword_t parse_sword(SourceScannerDHLX & sc)
{
	return parse__base<TEMPLATE_PUSH_type(sword)>(sc);
}
sword_t parse_sword(std::string const & value)
{
	return parse_num_base<sword_t, parse_sword, convert<sword_t, any_t>, parse_sword_const, parse_sword_unary, parse_sword_function>(value);
}
uword_t parse_uword(SourceScannerDHLX & sc)
{
	return parse__base<TEMPLATE_PUSH_type(uword)>(sc);
}
uword_t parse_uword(std::string const & value)
{
	return parse_num_base<uword_t, parse_uword, convert<uword_t, any_t>, parse_uword_const, parse_uword_unary, parse_uword_function>(value);
}
sdword_t parse_sdword(SourceScannerDHLX & sc)
{
	return parse__base<TEMPLATE_PUSH_type(sdword)>(sc);
}
sdword_t parse_sdword(std::string const & value)
{
	return parse_num_base<sdword_t, parse_sdword, convert<sdword_t, any_t>, parse_sdword_const, parse_sdword_unary, parse_sdword_function>(value);
}
udword_t parse_udword(SourceScannerDHLX & sc)
{
	return parse__base<TEMPLATE_PUSH_type(udword)>(sc);
}
udword_t parse_udword(std::string const & value)
{
	return parse_num_base<udword_t, parse_udword, convert<udword_t, any_t>, parse_udword_const, parse_udword_unary, parse_udword_function>(value);
}



