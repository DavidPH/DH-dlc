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
#include "exceptions/UnknownFunctionException.hpp"

#include "parsing/FunctionHandler.hpp"

#include "types/binary.hpp"
#include "types/int_t.hpp"
#include "types/real_t.hpp"
#include "types/string_t.hpp"

#include "../common/string_funcs.hpp"



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



template<typename T> T    parse         (std::string const & value);
template<typename T> T    parse         (SourceScannerDHLX & sc);
template<typename T> T    parse_const   (std::string const & function);
template<typename T> bool parse_function(ParsingDataDDL<T> & data);
template<typename T> void parse_init    (T & data);
template<typename T> T    parse_part    (SourceScannerDHLX & sc);
template<typename T> bool parse_typecast(ParsingDataDDL<T> & data);
template<typename T> void parse_typecast(T & data, type_t const type, SourceScannerDHLX & sc);
template<typename T> bool parse_unary   (ParsingDataDDL<T> & data);
template<typename T> T    parse_unary   (std::string const & function, SourceScannerDHLX & sc);
template<typename T> T    parse_unary   (std::string const & function, std::string const & value);
template<typename T> bool parse_value   (ParsingDataDDL<T> & data);



/* parse -> const -> bool */
template<typename T>
inline T parse_const__bool(std::string const & function)
{
	if (function == function_name_random())
		return random<int_s_t>(0, 1) != 0;

	throw UnknownFunctionException(function);
}


/* parse -> const -> int */
template<typename T>
inline T parse_const__int(std::string const & function)
{
	throw UnknownFunctionException(function);
}


/* parse -> const -> real */
template<typename T>
inline T parse_const__real(std::string const & function)
{
	if (function == function_name_pi())
		return convert<T, real_t>(pi());

	if (function == function_name_random())
		return random<T>();

	throw UnknownFunctionException(function);
}


/* parse -> const -> string */
template<typename T>
inline T parse_const__string(std::string const & function)
{
	if (function == function_name_mapname())
		return T(option_map_name);

	throw UnknownFunctionException(function);
}


/* parse -> const */
template<typename T>
inline T parse_const(std::string const & function)
{
	throw UnknownFunctionException(function);
}

/* parse -> const <bool_t> */
template<>
inline bool_t parse_const<bool_t>(std::string const & function)
{
	return parse_const__bool<bool_t>(function);
}

/* parse -> const <int_s_t> */
template<>
inline int_s_t parse_const<int_s_t>(std::string const & function)
{
	return parse_const__int<int_s_t>(function);
}

/* parse -> const <int_t> */
template<>
inline int_t parse_const<int_t>(std::string const & function)
{
	return parse_const__int<int_t>(function);
}

/* parse -> const <int_l_t> */
template<>
inline int_l_t parse_const<int_l_t>(std::string const & function)
{
	return parse_const__int<int_l_t>(function);
}

/* parse -> const <real_s_t> */
template<>
inline real_s_t parse_const<real_s_t>(std::string const & function)
{
	return parse_const__real<real_s_t>(function);
}

/* parse -> const <real_t> */
template<>
inline real_t parse_const<real_t>(std::string const & function)
{
	return parse_const__real<real_t>(function);
}

/* parse -> const <real_l_t> */
template<>
inline real_l_t parse_const<real_l_t>(std::string const & function)
{
	return parse_const__real<real_l_t>(function);
}

/* parse -> const <string_t> */
template<>
inline string_t parse_const<string_t>(std::string const & function)
{
	return parse_const__string<string_t>(function);
}

/* parse -> const <string8_t> */
template<>
inline string8_t parse_const<string8_t>(std::string const & function)
{
	return parse_const__string<string8_t>(function);
}

/* parse -> const <string16_t> */
template<>
inline string16_t parse_const<string16_t>(std::string const & function)
{
	return parse_const__string<string16_t>(function);
}

/* parse -> const <string32_t> */
template<>
inline string32_t parse_const<string32_t>(std::string const & function)
{
	return parse_const__string<string32_t>(function);
}

/* parse -> const <string80_t> */
template<>
inline string80_t parse_const<string80_t>(std::string const & function)
{
	return parse_const__string<string80_t>(function);
}

/* parse -> const <string320t> */
template<>
inline string320_t parse_const<string320_t>(std::string const & function)
{
	return parse_const__string<string320_t>(function);
}

/* parse -> const <ubyte_t> */
template<>
inline ubyte_t parse_const<ubyte_t>(std::string const & function)
{
	return parse_const__int<ubyte_t>(function);
}

/* parse -> const <sword_t> */
template<>
inline sword_t parse_const<sword_t>(std::string const & function)
{
	return parse_const__int<sword_t>(function);
}

/* parse -> const <uword_t> */
template<>
inline uword_t parse_const<uword_t>(std::string const & function)
{
	return parse_const__int<uword_t>(function);
}

/* parse -> const <sdword_t> */
template<>
inline sdword_t parse_const<sdword_t>(std::string const & function)
{
	return parse_const__int<sdword_t>(function);
}

/* parse -> const <udword_t> */
template<>
inline udword_t parse_const<udword_t>(std::string const & function)
{
	return parse_const__int<udword_t>(function);
}



/* parse -> function */
template<typename T>
inline bool parse_function(ParsingDataDDL<T> & data)
{
	if ((data.value.size() == 0) || (data.value[0] != '<')) return false;

	size_t bracketIndex = data.value.find('>');

	if (bracketIndex == std::string::npos) return false;

	if (bracketIndex == (data.value.size() - 1))
		throw FunctionException("missing argument list");

	std::string function(data.value, 1, bracketIndex - 1);
	std::string value(data.value, bracketIndex + 2, (data.value.size() - 2) - (bracketIndex + 2));

	std::vector<std::string> args(parse_args(value));

	data.valueReturn = FunctionHandler<T>::get_function(function)(args);

	return true;
}



/* parse -> init */
template<typename T>
inline void parse_init(T & data)
{

}

/* parse -> init <bool_t> */
template<>
inline void parse_init<bool_t>(bool_t & data)
{
	data = false;
}

/* parse -> init <int_s_t> */
template<>
inline void parse_init<int_s_t>(int_s_t & data)
{
	data = 0;
}

/* parse -> init <real_s_t> */
template<>
inline void parse_init<real_s_t>(real_s_t & data)
{
	data = 0;
}



/* parse -> math -> bool */
template<typename T>
inline bool parse_math__bool(ParsingDataDDL<T> & data)
{
	switch (data.operatorChar)
	{
		case -2: return false;

		case '&': data.valueReturn = parse<T>(data.valueLeft) && parse<T>(data.valueRight); return true;
		case '|': data.valueReturn = parse<T>(data.valueLeft) || parse<T>(data.valueRight); return true;

		default: throw ParsingException(std::string("non-bool operator:") + (char)data.operatorChar);
	}
}
template<typename T>
inline bool parse_math__bool(T & data, SourceScannerDHLX & sc)
{
	SourceTokenDHLX st(sc.get());

	switch (st.getType())
	{
	case SourceTokenDHLX::TT_OP_AND2:
		data = data && parse_part<T>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_PIPE2:
		data = data || parse_part<T>(sc);
		return true;

	default:
		sc.unget(st);
		return false;
	}
}


/* parse -> math -> int */
template<typename T>
inline bool parse_math__int(ParsingDataDDL<T> & data)
{
	switch (data.operatorChar)
	{
		case -2: return false;

		case '*': data.valueReturn = parse<T>(data.valueLeft) * parse<T>(data.valueRight); return true;
		case '/': data.valueReturn = parse<T>(data.valueLeft) / parse<T>(data.valueRight); return true;
		case '%': data.valueReturn = parse<T>(data.valueLeft) % parse<T>(data.valueRight); return true;
		case '+': data.valueReturn = parse<T>(data.valueLeft) + parse<T>(data.valueRight); return true;
		case '-': data.valueReturn = parse<T>(data.valueLeft) - parse<T>(data.valueRight); return true;
		case '&': data.valueReturn = parse<T>(data.valueLeft) & parse<T>(data.valueRight); return true;
		case '|': data.valueReturn = parse<T>(data.valueLeft) | parse<T>(data.valueRight); return true;

		default: throw ParsingException(std::string("non-int operator:") + (char)data.operatorChar);
	}
}
template<typename T>
inline bool parse_math__int(T & data, SourceScannerDHLX & sc)
{
	SourceTokenDHLX st(sc.get());

	switch (st.getType())
	{
	case SourceTokenDHLX::TT_OP_AND:
		data &= parse_part<T>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_ASTERIX:
		data *= parse_part<T>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_MINUS:
		data -= parse_part<T>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_PERCENT:
		data %= parse_part<T>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_PIPE:
		data |= parse_part<T>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_PLUS:
		data += parse_part<T>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_SLASH:
		data /= parse_part<T>(sc);
		return true;

	default:
		sc.unget(st);
		return false;
	}
}


/* parse -> math -> real */
template<typename T>
inline bool parse_math__real(ParsingDataDDL<T> & data)
{
	switch (data.operatorChar)
	{
		case -2: return false;

		case '*': data.valueReturn = parse<T>(data.valueLeft) * parse<T>(data.valueRight); return true;
		case '/': data.valueReturn = parse<T>(data.valueLeft) / parse<T>(data.valueRight); return true;
		case '+': data.valueReturn = parse<T>(data.valueLeft) + parse<T>(data.valueRight); return true;
		case '-': data.valueReturn = parse<T>(data.valueLeft) - parse<T>(data.valueRight); return true;

		default: throw ParsingException(std::string("non-float operator:") + (char)data.operatorChar);
	}
}
template<typename T>
inline bool parse_math__real(T & data, SourceScannerDHLX & sc)
{
	SourceTokenDHLX st(sc.get());

	switch (st.getType())
	{
	case SourceTokenDHLX::TT_OP_ASTERIX:
		data *= parse_part<T>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_MINUS:
		data -= parse_part<T>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_PLUS:
		data += parse_part<T>(sc);
		return true;

	case SourceTokenDHLX::TT_OP_SLASH:
		data /= parse_part<T>(sc);
		return true;

	default:
		sc.unget(st);
		return false;
	}
}


/* parse -> math -> string */
template<typename T>
inline bool parse_math__string(ParsingDataDDL<T> & data)
{
	switch (data.operatorChar)
	{
		case -2: return false;

		case '+': data.valueReturn = parse<T>(data.valueLeft) + parse<T>(data.valueRight); return true;

		default: throw ParsingException(std::string("non-string operator:") + (char)data.operatorChar);
	}
}
template<typename T>
inline bool parse_math__string(T & data, SourceScannerDHLX & sc)
{
	SourceTokenDHLX st(sc.get());

	switch (st.getType())
	{
	case SourceTokenDHLX::TT_OP_PLUS:
		data += parse_part<T>(sc);
		return true;

	default:
		sc.unget(st);
		return false;
	}
}


/* parse -> math */
template<typename T>
inline bool parse_math(ParsingDataDDL<T> & data)
{
	switch (data.operatorChar)
	{
		case -2: return false;

		default: throw ParsingException(std::string("non-string operator:") + (char)data.operatorChar);
	}
}
template<typename T>
inline bool parse_math(T & data, SourceScannerDHLX & sc)
{
	return false;
}

/* parse -> math <bool_t> */
template<>
inline bool parse_math<bool_t>(ParsingDataDDL<bool_t> & data)
{
	return parse_math__bool<bool_t>(data);
}
template<>
inline bool parse_math<bool_t>(bool_t & data, SourceScannerDHLX & sc)
{
	return parse_math__bool<bool_t>(data, sc);
}

/* parse -> math <int_s_t> */
template<>
inline bool parse_math<int_s_t>(ParsingDataDDL<int_s_t> & data)
{
	return parse_math__int<int_s_t>(data);
}
template<>
inline bool parse_math<int_s_t>(int_s_t & data, SourceScannerDHLX & sc)
{
	return parse_math__int<int_s_t>(data, sc);
}

/* parse -> math <int_t> */
template<>
inline bool parse_math<int_t>(ParsingDataDDL<int_t> & data)
{
	return parse_math__int<int_t>(data);
}
template<>
inline bool parse_math<int_t>(int_t & data, SourceScannerDHLX & sc)
{
	return parse_math__int<int_t>(data, sc);
}

/* parse -> math <int_l_t> */
template<>
inline bool parse_math<int_l_t>(ParsingDataDDL<int_l_t> & data)
{
	return parse_math__int<int_l_t>(data);
}
template<>
inline bool parse_math<int_l_t>(int_l_t & data, SourceScannerDHLX & sc)
{
	return parse_math__int<int_l_t>(data, sc);
}

/* parse -> math <real_s_t> */
template<>
inline bool parse_math<real_s_t>(ParsingDataDDL<real_s_t> & data)
{
	return parse_math__real<real_s_t>(data);
}
template<>
inline bool parse_math<real_s_t>(real_s_t & data, SourceScannerDHLX & sc)
{
	return parse_math__real<real_s_t>(data, sc);
}

/* parse -> math <real_t> */
template<>
inline bool parse_math<real_t>(ParsingDataDDL<real_t> & data)
{
	return parse_math__real<real_t>(data);
}
template<>
inline bool parse_math<real_t>(real_t & data, SourceScannerDHLX & sc)
{
	return parse_math__real<real_t>(data, sc);
}

/* parse -> math <real_l_t> */
template<>
inline bool parse_math<real_l_t>(ParsingDataDDL<real_l_t> & data)
{
	return parse_math__real<real_l_t>(data);
}
template<>
inline bool parse_math<real_l_t>(real_l_t & data, SourceScannerDHLX & sc)
{
	return parse_math__real<real_l_t>(data, sc);
}

/* parse -> math <string_t> */
template<>
inline bool parse_math<string_t>(ParsingDataDDL<string_t> & data)
{
	return parse_math__string<string_t>(data);
}
template<>
inline bool parse_math<string_t>(string_t & data, SourceScannerDHLX & sc)
{
	return parse_math__string<string_t>(data, sc);
}

/* parse -> math <string8_t> */
template<>
inline bool parse_math<string8_t>(ParsingDataDDL<string8_t> & data)
{
	return parse_math__string<string8_t>(data);
}
template<>
inline bool parse_math<string8_t>(string8_t & data, SourceScannerDHLX & sc)
{
	return parse_math__string<string8_t>(data, sc);
}

/* parse -> math <string16_t> */
template<>
inline bool parse_math<string16_t>(ParsingDataDDL<string16_t> & data)
{
	return parse_math__string<string16_t>(data);
}
template<>
inline bool parse_math<string16_t>(string16_t & data, SourceScannerDHLX & sc)
{
	return parse_math__string<string16_t>(data, sc);
}

/* parse -> math <string32_t> */
template<>
inline bool parse_math<string32_t>(ParsingDataDDL<string32_t> & data)
{
	return parse_math__string<string32_t>(data);
}
template<>
inline bool parse_math<string32_t>(string32_t & data, SourceScannerDHLX & sc)
{
	return parse_math__string<string32_t>(data, sc);
}

/* parse -> math <string80_t> */
template<>
inline bool parse_math<string80_t>(ParsingDataDDL<string80_t> & data)
{
	return parse_math__string<string80_t>(data);
}
template<>
inline bool parse_math<string80_t>(string80_t & data, SourceScannerDHLX & sc)
{
	return parse_math__string<string80_t>(data, sc);
}

/* parse -> math <string320_t> */
template<>
inline bool parse_math<string320_t>(ParsingDataDDL<string320_t> & data)
{
	return parse_math__string<string320_t>(data);
}
template<>
inline bool parse_math<string320_t>(string320_t & data, SourceScannerDHLX & sc)
{
	return parse_math__string<string320_t>(data, sc);
}

/* parse -> math <ubyte_t> */
template<>
inline bool parse_math<ubyte_t>(ParsingDataDDL<ubyte_t> & data)
{
	return parse_math__int<ubyte_t>(data);
}
template<>
inline bool parse_math<ubyte_t>(ubyte_t & data, SourceScannerDHLX & sc)
{
	return parse_math__int<ubyte_t>(data, sc);
}

/* parse -> math <sword_t> */
template<>
inline bool parse_math<sword_t>(ParsingDataDDL<sword_t> & data)
{
	return parse_math__int<sword_t>(data);
}
template<>
inline bool parse_math<sword_t>(sword_t & data, SourceScannerDHLX & sc)
{
	return parse_math__int<sword_t>(data, sc);
}

/* parse -> math <uword_t> */
template<>
inline bool parse_math<uword_t>(ParsingDataDDL<uword_t> & data)
{
	return parse_math__int<uword_t>(data);
}
template<>
inline bool parse_math<uword_t>(uword_t & data, SourceScannerDHLX & sc)
{
	return parse_math__int<uword_t>(data, sc);
}

/* parse -> math <sdword_t> */
template<>
inline bool parse_math<sdword_t>(ParsingDataDDL<sdword_t> & data)
{
	return parse_math__int<sdword_t>(data);
}
template<>
inline bool parse_math<sdword_t>(sdword_t & data, SourceScannerDHLX & sc)
{
	return parse_math__int<sdword_t>(data, sc);
}

/* parse -> math <udword_t> */
template<>
inline bool parse_math<udword_t>(ParsingDataDDL<udword_t> & data)
{
	return parse_math__int<udword_t>(data);
}
template<>
inline bool parse_math<udword_t>(udword_t & data, SourceScannerDHLX & sc)
{
	return parse_math__int<udword_t>(data, sc);
}



/* parse -> part -> IDENTIFIER */
template<typename T>
inline void parse_part_IDENTIFIER(T & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	sc.unget(st);
	data = convert<T, obj_t>(get_object(parse_name(sc)));
}

/* parse -> part -> IDENTIFIER <bool_t> */
template<>
inline void parse_part_IDENTIFIER<bool_t>(bool_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
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


/* parse -> part -> NUMBER */
template<typename T>
inline void parse_part_NUMBER(T & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	sc.unget(st);
}

/* parse -> part -> NUMBER <int_s_t> */
template<>
inline void parse_part_NUMBER<int_s_t>(int_s_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<int_s_t>(st.getData());
}

/* parse -> part -> NUMBER <int_t> */
template<>
inline void parse_part_NUMBER<int_t>(int_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<int_t>(st.getData());
}

/* parse -> part -> NUMBER <int_l_t> */
template<>
inline void parse_part_NUMBER<int_l_t>(int_l_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<int_l_t>(st.getData());
}

/* parse -> part -> NUMBER <real_s_t> */
template<>
inline void parse_part_NUMBER<real_s_t>(real_s_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<real_s_t>(st.getData());
}

/* parse -> part -> NUMBER <real_t> */
template<>
inline void parse_part_NUMBER<real_t>(real_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<real_t>(st.getData());
}

/* parse -> part -> NUMBER <real_l_t> */
template<>
inline void parse_part_NUMBER<real_l_t>(real_l_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<real_l_t>(st.getData());
}

/* parse -> part -> NUMBER <ubyte_t> */
template<>
inline void parse_part_NUMBER<ubyte_t>(ubyte_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<ubyte_t>(st.getData());
}

/* parse -> part -> NUMBER <sword_t> */
template<>
inline void parse_part_NUMBER<sword_t>(sword_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<sword_t>(st.getData());
}

/* parse -> part -> NUMBER <uword_t> */
template<>
inline void parse_part_NUMBER<uword_t>(uword_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<uword_t>(st.getData());
}

/* parse -> part -> NUMBER <sdword_t> */
template<>
inline void parse_part_NUMBER<sdword_t>(sdword_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<sdword_t>(st.getData());
}

/* parse -> part -> NUMBER <udword_t> */
template<>
inline void parse_part_NUMBER<udword_t>(udword_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = num_from_string<udword_t>(st.getData());
}


/* parse -> part -> STRING */
template<typename T>
inline void parse_part_STRING(T & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	sc.unget(st);
}

/* parse -> part -> STRING <string_t> */
template<>
inline void parse_part_STRING<string_t>(string_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = string_t(st.getData());
}

/* parse -> part -> STRING <string8_t> */
template<>
inline void parse_part_STRING<string8_t>(string8_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = string8_t(st.getData());
}

/* parse -> part -> STRING <string16_t> */
template<>
inline void parse_part_STRING<string16_t>(string16_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = string16_t(st.getData());
}

/* parse -> part -> STRING <string32_t> */
template<>
inline void parse_part_STRING<string32_t>(string32_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = string32_t(st.getData());
}

/* parse -> part -> STRING <string80_t> */
template<>
inline void parse_part_STRING<string80_t>(string80_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = string80_t(st.getData());
}

/* parse -> part -> STRING <string320_t> */
template<>
inline void parse_part_STRING<string320_t>(string320_t & data, SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	data = string320_t(st.getData());
}


/* parse -> part */
template<typename T>
inline T parse_part(SourceScannerDHLX & sc)
{
	T data;

	parse_init<T>(data);

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
				parse_typecast<T>(data, type_t::get_type(function), sc);
			else
				data = FunctionHandler<T>::get_function(function)(sc);

			sc.get(SourceTokenDHLX::TT_OP_PARENTHESIS_C);
			break;
		}
		else
		{
			sc.unget(st2);
		}
	}
		parse_part_IDENTIFIER<T>(data, st, sc);
		break;

	case SourceTokenDHLX::TT_NUMBER:
		parse_part_NUMBER<T>(data, st, sc);
		break;

	case SourceTokenDHLX::TT_STRING:
		parse_part_STRING<T>(data, st, sc);
		break;

	case SourceTokenDHLX::TT_OP_BRACKET_O:
		st = sc.get(SourceTokenDHLX::TT_IDENTIFIER);
		sc.get(SourceTokenDHLX::TT_OP_BRACKET_C);
		data = parse_const<T>(st.getData());
		break;

	case SourceTokenDHLX::TT_OP_CMP_LT:
		st = sc.get(SourceTokenDHLX::TT_IDENTIFIER);
		sc.get(SourceTokenDHLX::TT_OP_CMP_GT);
		data = parse_unary<T>(st.getData(), sc);
		break;

	case SourceTokenDHLX::TT_OP_PARENTHESIS_O:
	{
		SourceTokenDHLX st2(sc.get());
		SourceTokenDHLX st3(sc.get());

		if ((st2.getType() == SourceTokenDHLX::TT_IDENTIFIER) && (st3.getType() == SourceTokenDHLX::TT_OP_PARENTHESIS_C) && type_t::has_type(st2.getData()))
		{
			parse_typecast<T>(data, type_t::get_type(st2.getData()), sc);
			break;
		}
		else
		{
			sc.unget(st3);
			sc.unget(st2);
		}
	}
		data = parse<T>(sc);
		sc.get(SourceTokenDHLX::TT_OP_PARENTHESIS_C);
		break;

	default:
		sc.unget(st);
		break;
	}

	return data;
}



/* parse -> typecast */
template<typename T>
inline bool parse_typecast(ParsingDataDDL<T> & data)
{
	if ((data.value.size() == 0) || (data.value[0] != '(')) return false;

	size_t bracketIndex = data.value.find(')');

	if (bracketIndex == std::string::npos) return false;

	std::string typeString(data.value, 1, bracketIndex-1);

	if (!type_t::has_type(typeString)) return false;

	if (bracketIndex == data.value.size()-1) return true;

	type_t type(type_t::get_type(typeString));

	std::string value(data.value, bracketIndex + 1);

	     if (type == type_t::type_bool())       data.valueReturn = convert<T, bool_t>(parse<bool_t>(value));

	else if (type == type_t::type_shortint())   data.valueReturn = convert<T, int_s_t>(parse<int_s_t>(value));
	else if (type == type_t::type_int())        data.valueReturn = convert<T, int_t>(parse<int_t>(value));
	else if (type == type_t::type_longint())    data.valueReturn = convert<T, int_l_t>(parse<int_l_t>(value));

	else if (type == type_t::type_shortfloat()) data.valueReturn = convert<T, real_s_t>(parse<real_s_t>(value));
	else if (type == type_t::type_float())      data.valueReturn = convert<T, real_t>(parse<real_t>(value));
	else if (type == type_t::type_longfloat())  data.valueReturn = convert<T, real_l_t>(parse<real_l_t>(value));

	else if (type == type_t::type_string())     data.valueReturn = convert<T, string_t>(parse<string_t>(value));
	else if (type == type_t::type_string8())    data.valueReturn = convert<T, string8_t>(parse<string8_t>(value));
	else if (type == type_t::type_string16())   data.valueReturn = convert<T, string16_t>(parse<string16_t>(value));
	else if (type == type_t::type_string32())   data.valueReturn = convert<T, string32_t>(parse<string32_t>(value));
	else if (type == type_t::type_string80())   data.valueReturn = convert<T, string80_t>(parse<string80_t>(value));
	else if (type == type_t::type_string320())  data.valueReturn = convert<T, string320_t>(parse<string320_t>(value));

	else if (type == type_t::type_ubyte())      data.valueReturn = convert<T, ubyte_t>(parse<ubyte_t>(value));
	else if (type == type_t::type_sword())      data.valueReturn = convert<T, sword_t>(parse<sword_t>(value));
	else if (type == type_t::type_uword())      data.valueReturn = convert<T, uword_t>(parse<uword_t>(value));
	else if (type == type_t::type_sdword())     data.valueReturn = convert<T, sdword_t>(parse<sdword_t>(value));
	else if (type == type_t::type_udword())     data.valueReturn = convert<T, udword_t>(parse<udword_t>(value));

	else
		throw ParsingException("unknown typecast:" + type.makeString());

	return true;
}
template<typename T>
inline void parse_typecast(T & data, type_t const type, SourceScannerDHLX & sc)
{
	     if (type == type_t::type_bool())       data = convert<T, bool_t>(parse<bool_t>(sc));

	else if (type == type_t::type_shortint())   data = convert<T, int_s_t>(parse<int_s_t>(sc));
	else if (type == type_t::type_int())        data = convert<T, int_t>(parse<int_t>(sc));
	else if (type == type_t::type_longint())    data = convert<T, int_l_t>(parse<int_l_t>(sc));

	else if (type == type_t::type_shortfloat()) data = convert<T, real_s_t>(parse<real_s_t>(sc));
	else if (type == type_t::type_float())      data = convert<T, real_t>(parse<real_t>(sc));
	else if (type == type_t::type_longfloat())  data = convert<T, real_l_t>(parse<real_l_t>(sc));

	else if (type == type_t::type_string())     data = convert<T, string_t>(parse<string_t>(sc));
	else if (type == type_t::type_string8())    data = convert<T, string8_t>(parse<string8_t>(sc));
	else if (type == type_t::type_string16())   data = convert<T, string16_t>(parse<string16_t>(sc));
	else if (type == type_t::type_string32())   data = convert<T, string32_t>(parse<string32_t>(sc));
	else if (type == type_t::type_string80())   data = convert<T, string80_t>(parse<string80_t>(sc));
	else if (type == type_t::type_string320())  data = convert<T, string320_t>(parse<string320_t>(sc));

	else if (type == type_t::type_ubyte())      data = convert<T, ubyte_t>(parse<ubyte_t>(sc));
	else if (type == type_t::type_sword())      data = convert<T, sword_t>(parse<sword_t>(sc));
	else if (type == type_t::type_uword())      data = convert<T, uword_t>(parse<uword_t>(sc));
	else if (type == type_t::type_sdword())     data = convert<T, sdword_t>(parse<sdword_t>(sc));
	else if (type == type_t::type_udword())     data = convert<T, udword_t>(parse<udword_t>(sc));

	else
		throw ParsingException("unknown typecast:" + type.makeString());
}



/* parse -> unary -> bool */
template<typename T>
inline T parse_unary__bool(std::string const & function, std::string const & value)
{
	if (function == function_name_exists())
		return has_object(name_t(value));

	if (function == function_name_not())
		return !parse<T>(value);

	throw UnknownFunctionException(function);
}


/* parse -> unary -> byte */
template<typename T>
inline T parse_unary__byte(std::string const & function, std::string const & value)
{
	// Bytes are too small for certain operations.

	if (function == function_name_abs())
		return abs(parse<T>(value));

	if (function == function_name_random())
		return random<T>(parse<T>(value));

	if (function == function_name_sqrt())
		return sqrt(parse<T>(value));

	throw UnknownFunctionException(function);
}


/* parse -> unary -> int */
template<typename T>
inline T parse_unary__int(std::string const & function, std::string const & value)
{
	if (function == function_name_abs())
		return abs(parse<T>(value));

	if (function == function_name_byte2deg())
		return (parse<T>(value) * T(360)) / T(256);

	if (function == function_name_byteangle())
		return clamp<T>(parse<T>(value), T(0), T(256));

	if (function == function_name_deg2byte())
		return (parse<T>(value) * T(256)) / T(360);

	if (function == function_name_degrees())
		return clamp<T>(parse<T>(value), T(0), T(360));

	if (function == function_name_random())
		return random<T>(parse<T>(value));

	if (function == function_name_sqrt())
		return sqrt(parse<T>(value));

	throw UnknownFunctionException(function);
}


/* parse -> unary -> real */
template<typename T>
inline T parse_unary__real(std::string const & function, std::string const & value)
{
	if (function == function_name_abs())
		return abs(parse<T>(value));

	if (function == function_name_acos())
		return acos(parse<T>(value));

	if (function == function_name_asin())
		return asin(parse<T>(value));

	if (function == function_name_atan())
		return atan(parse<T>(value));

	if (function == function_name_byte2deg())
		return (parse<T>(value) * T(360)) / T(256);

	if (function == function_name_byte2rad())
		return (parse<T>(value) * convert<T, real_t>(pi())) / T(128);

	if (function == function_name_byteangle())
		return clamp<T>(parse<T>(value), T(0), T(256));

	if (function == function_name_cos())
		return cos(parse<T>(value));

	if (function == function_name_deg2byte())
		return (parse<T>(value) * T(256)) / T(360);

	if (function == function_name_deg2rad())
		return (parse<T>(value) * convert<T, real_t>(pi())) / T(180);

	if (function == function_name_degrees())
		return clamp<T>(parse<T>(value), T(0), T(360));

	if (function == function_name_rad2byte())
		return (parse<T>(value) * T(128)) / convert<T, real_t>(pi());

	if (function == function_name_rad2deg())
		return (parse<T>(value) * T(180)) / convert<T, real_t>(pi());

	if (function == function_name_radians())
		return clamp<T>(parse<T>(value), T(0), convert<T, real_t>(pi() * real_t(2)));

	if (function == function_name_random())
		return random<T>(parse<T>(value));

	// TODO
	if (function == function_name_round())
		return convert<T, int_l_t>(convert<int_l_t, T>(parse<T>(value) + T(0.5)));
	//	return round(parse<T>(value));

	if (function == function_name_sin())
		return sin(parse<T>(value));

	if (function == function_name_sqrt())
		return sqrt(parse<T>(value));

	if (function == function_name_tan())
		return tan(parse<T>(value));

	throw UnknownFunctionException(function);
}


/* parse -> unary -> string */
template<typename T>
inline T parse_unary__string(std::string const & function, std::string const & value)
{
	if (function == function_name_lower())
		return T(tolower(parse<T>(value).makeString()));

	if (function == function_name_quote())
	{
		std::ostringstream oss;
		std::istringstream iss(parse<T>(value).makeString());

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

		return T(oss.str());
	}

	if (function == function_name_upper())
		return T(toupper(parse<T>(value).makeString()));

	throw UnknownFunctionException(function);
}


/* parse -> unary */
template<typename T>
inline bool parse_unary(ParsingDataDDL<T> & data)
{
	if ((data.value.size() == 0) || (data.value[0] != '[')) return false;

	size_t bracketIndex = data.value.find(']');

	if (bracketIndex == std::string::npos) return false;

	std::string function = data.value.substr(1, bracketIndex-1);

	if (bracketIndex == (data.value.size() - 1))
		data.valueReturn = parse_const<T>(function);
	else
		data.valueReturn = parse_unary<T>(function, data.value.substr(bracketIndex + 1));

	return true;
}
template<typename T>
inline T parse_unary(std::string const & function, SourceScannerDHLX & sc)
{
	throw UnknownFunctionException(function);
}
template<typename T>
inline T parse_unary(std::string const & function, std::string const & value)
{
	throw UnknownFunctionException(function);
}

/* parse -> unary <bool_t> */
template<>
inline bool_t parse_unary<bool_t>(std::string const & function, std::string const & value)
{
	return parse_unary__bool<bool_t>(function, value);
}

/* parse -> unary <int_s_t> */
template<>
inline int_s_t parse_unary<int_s_t>(std::string const & function, std::string const & value)
{
	return parse_unary__int<int_s_t>(function, value);
}

/* parse -> unary <int_t> */
template<>
inline int_t parse_unary<int_t>(std::string const & function, std::string const & value)
{
	return parse_unary__int<int_t>(function, value);
}

/* parse -> unary <int_l_t> */
template<>
inline int_l_t parse_unary<int_l_t>(std::string const & function, std::string const & value)
{
	return parse_unary__int<int_l_t>(function, value);
}

/* parse -> unary <real_s_t> */
template<>
inline real_s_t parse_unary<real_s_t>(std::string const & function, std::string const & value)
{
	return parse_unary__real<real_s_t>(function, value);
}

/* parse -> unary <real_t> */
template<>
inline real_t parse_unary<real_t>(std::string const & function, std::string const & value)
{
	return parse_unary__real<real_t>(function, value);
}

/* parse -> unary <real_l_t> */
template<>
inline real_l_t parse_unary<real_l_t>(std::string const & function, std::string const & value)
{
	return parse_unary__real<real_l_t>(function, value);
}

/* parse -> unary <string_t> */
template<>
inline string_t parse_unary<string_t>(std::string const & function, std::string const & value)
{
	return parse_unary__string<string_t>(function, value);
}

/* parse -> unary <string8_t> */
template<>
inline string8_t parse_unary<string8_t>(std::string const & function, std::string const & value)
{
	return parse_unary__string<string8_t>(function, value);
}

/* parse -> unary <string16_t> */
template<>
inline string16_t parse_unary<string16_t>(std::string const & function, std::string const & value)
{
	return parse_unary__string<string16_t>(function, value);
}

/* parse -> unary <string32_t> */
template<>
inline string32_t parse_unary<string32_t>(std::string const & function, std::string const & value)
{
	return parse_unary__string<string32_t>(function, value);
}

/* parse -> unary <string80_t> */
template<>
inline string80_t parse_unary<string80_t>(std::string const & function, std::string const & value)
{
	return parse_unary__string<string80_t>(function, value);
}

/* parse -> unary <string320t> */
template<>
inline string320_t parse_unary<string320_t>(std::string const & function, std::string const & value)
{
	return parse_unary__string<string320_t>(function, value);
}

/* parse -> unary <ubyte_t> */
template<>
inline ubyte_t parse_unary<ubyte_t>(std::string const & function, std::string const & value)
{
	return parse_unary__byte<ubyte_t>(function, value);
}

/* parse -> unary <sword_t> */
template<>
inline sword_t parse_unary<sword_t>(std::string const & function, std::string const & value)
{
	return parse_unary__int<sword_t>(function, value);
}

/* parse -> unary <uword_t> */
template<>
inline uword_t parse_unary<uword_t>(std::string const & function, std::string const & value)
{
	return parse_unary__int<uword_t>(function, value);
}

/* parse -> unary <sdword_t> */
template<>
inline sdword_t parse_unary<sdword_t>(std::string const & function, std::string const & value)
{
	return parse_unary__int<sdword_t>(function, value);
}

/* parse -> unary <udword_t> */
template<>
inline udword_t parse_unary<udword_t>(std::string const & function, std::string const & value)
{
	return parse_unary__int<udword_t>(function, value);
}



/* parse -> value -> bool */
template<typename T>
inline bool parse_value__bool(ParsingDataDDL<T> & data)
{
	if ((data.value == "true") || (data.value == "TRUE"))
	{
		data.valueReturn = true;
		return true;
	}

	if ((data.value == "false") || (data.value == "FALSE"))
	{
		data.valueReturn = false;
		return true;
	}

	return false;
}


/* parse -> value -> number */
template<typename T>
inline bool parse_value__number(ParsingDataDDL<T> & data)
{
	if ((data.value.size() == 0) || (data.value[0] != '0'))
		return false;

	data.valueReturn = num_from_string<T>(data.value);

	return true;
}


/* parse -> value -> string */
template<typename T>
inline bool parse_value__string(ParsingDataDDL<T> & data)
{
	if (data.value.size() == 0)
		return false;

	if (data.value[0] == '$')
	{
		data.valueReturn = T(data.value);
		return true;
	}

	if (!option_strict_strings && !has_object(name_t(data.value)))
	{
		data.valueReturn = T(data.value);
		return true;
	}

	return false;
}


/* parse -> value */
template<typename T>
inline bool parse_value(ParsingDataDDL<T> & data)
{
	return false;
}

/* parse -> value <bool_t> */
template<>
inline bool parse_value<bool_t>(ParsingDataDDL<bool_t> & data)
{
	return parse_value__bool<bool_t>(data);
}

/* parse -> value <int_s_t> */
template<>
inline bool parse_value<int_s_t>(ParsingDataDDL<int_s_t> & data)
{
	return parse_value__number<int_s_t>(data);
}

/* parse -> value <int_t> */
template<>
inline bool parse_value<int_t>(ParsingDataDDL<int_t> & data)
{
	return parse_value__number<int_t>(data);
}

/* parse -> value <int_l_t> */
template<>
inline bool parse_value<int_l_t>(ParsingDataDDL<int_l_t> & data)
{
	return parse_value__number<int_l_t>(data);
}

/* parse -> value <real_s_t> */
template<>
inline bool parse_value<real_s_t>(ParsingDataDDL<real_s_t> & data)
{
	return parse_value__number<real_s_t>(data);
}

/* parse -> value <real_t> */
template<>
inline bool parse_value<real_t>(ParsingDataDDL<real_t> & data)
{
	return parse_value__number<real_t>(data);
}

/* parse -> value <real_l_t> */
template<>
inline bool parse_value<real_l_t>(ParsingDataDDL<real_l_t> & data)
{
	return parse_value__number<real_l_t>(data);
}

/* parse -> value <string_t> */
template<>
inline bool parse_value<string_t>(ParsingDataDDL<string_t> & data)
{
	return parse_value__string<string_t>(data);
}

/* parse -> value <string8_t> */
template<>
inline bool parse_value<string8_t>(ParsingDataDDL<string8_t> & data)
{
	return parse_value__string<string8_t>(data);
}

/* parse -> value <string16_t> */
template<>
inline bool parse_value<string16_t>(ParsingDataDDL<string16_t> & data)
{
	return parse_value__string<string16_t>(data);
}

/* parse -> value <string32_t> */
template<>
inline bool parse_value<string32_t>(ParsingDataDDL<string32_t> & data)
{
	return parse_value__string<string32_t>(data);
}

/* parse -> value <string80_t> */
template<>
inline bool parse_value<string80_t>(ParsingDataDDL<string80_t> & data)
{
	return parse_value__string<string80_t>(data);
}

/* parse -> value <string320_t> */
template<>
inline bool parse_value<string320_t>(ParsingDataDDL<string320_t> & data)
{
	return parse_value__string<string320_t>(data);
}

/* parse -> value <ubyte_t> */
template<>
inline bool parse_value<ubyte_t>(ParsingDataDDL<ubyte_t> & data)
{
	return parse_value__number<ubyte_t>(data);
}

/* parse -> value <sword_t> */
template<>
inline bool parse_value<sword_t>(ParsingDataDDL<sword_t> & data)
{
	return parse_value__number<sword_t>(data);
}

/* parse -> value <uword_t> */
template<>
inline bool parse_value<uword_t>(ParsingDataDDL<uword_t> & data)
{
	return parse_value__number<uword_t>(data);
}

/* parse -> value <sdword_t> */
template<>
inline bool parse_value<sdword_t>(ParsingDataDDL<sdword_t> & data)
{
	return parse_value__number<sdword_t>(data);
}

/* parse -> value <udword_t> */
template<>
inline bool parse_value<udword_t>(ParsingDataDDL<udword_t> & data)
{
	return parse_value__number<udword_t>(data);
}



/* parse */
template<typename T>
T parse(std::string const & value)
{
	ParsingDataDDL<T> data(value);

	if (parse_math<T>(data))
		return data.valueReturn;

	if (parse_typecast<T>(data))
		return data.valueReturn;

	if (parse_unary<T>(data))
		return data.valueReturn;

	if (parse_function<T>(data))
		return data.valueReturn;

	if (data.hasBracket)
		return parse<T>(data.value.substr(1, data.value.length()-2));

	if (parse_value<T>(data))
		return data.valueReturn;

	return convert<T, obj_t>(get_object(name_t(value)));
}
template<typename T>
T parse(SourceScannerDHLX & sc)
{
	T data;

	parse_init<T>(data);

	data = parse_part<T>(sc);

	while (parse_math<T>(data, sc));

	return data;
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
			nameElement += make_string(parse<int_s_t>(sc));
			hasDynamic   = true;

			sc.get(SourceTokenDHLX::TT_OP_BRACKET_C);
		}
		else if (nameToken.getType() == SourceTokenDHLX::TT_OP_CMP_LT)
		{
			if (hasDynamic)
				nameElement += '.';
			nameElement += make_string(parse<string_t>(sc));
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

	return get_object(parse<int_s_t>(value), type);
}



template bool_t      parse<bool_t>     (std::string const & value);
template bool_t      parse<bool_t>     (SourceScannerDHLX & sc);
template int_s_t     parse<int_s_t>    (std::string const & value);
template int_s_t     parse<int_s_t>    (SourceScannerDHLX & sc);
template int_t       parse<int_t>      (std::string const & value);
template int_t       parse<int_t>      (SourceScannerDHLX & sc);
template int_l_t     parse<int_l_t>    (std::string const & value);
template int_l_t     parse<int_l_t>    (SourceScannerDHLX & sc);
template real_s_t    parse<real_s_t>   (std::string const & value);
template real_s_t    parse<real_s_t>   (SourceScannerDHLX & sc);
template real_t      parse<real_t>     (std::string const & value);
template real_t      parse<real_t>     (SourceScannerDHLX & sc);
template real_l_t    parse<real_l_t>   (std::string const & value);
template real_l_t    parse<real_l_t>   (SourceScannerDHLX & sc);
template string_t    parse<string_t>   (std::string const & value);
template string_t    parse<string_t>   (SourceScannerDHLX & sc);
template string8_t   parse<string8_t>  (std::string const & value);
template string8_t   parse<string8_t>  (SourceScannerDHLX & sc);
template string16_t  parse<string16_t> (std::string const & value);
template string16_t  parse<string16_t> (SourceScannerDHLX & sc);
template string32_t  parse<string32_t> (std::string const & value);
template string32_t  parse<string32_t> (SourceScannerDHLX & sc);
template string80_t  parse<string80_t> (std::string const & value);
template string80_t  parse<string80_t> (SourceScannerDHLX & sc);
template string320_t parse<string320_t>(std::string const & value);
template string320_t parse<string320_t>(SourceScannerDHLX & sc);
template ubyte_t     parse<ubyte_t>    (std::string const & value);
template ubyte_t     parse<ubyte_t>    (SourceScannerDHLX & sc);
template sword_t     parse<sword_t>    (std::string const & value);
template sword_t     parse<sword_t>    (SourceScannerDHLX & sc);
template uword_t     parse<uword_t>    (std::string const & value);
template uword_t     parse<uword_t>    (SourceScannerDHLX & sc);
template sdword_t    parse<sdword_t>   (std::string const & value);
template sdword_t    parse<sdword_t>   (SourceScannerDHLX & sc);
template udword_t    parse<udword_t>   (std::string const & value);
template udword_t    parse<udword_t>   (SourceScannerDHLX & sc);



