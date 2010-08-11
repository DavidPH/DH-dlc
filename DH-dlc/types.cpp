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
	2010/02/03 - Added LevelObjectName class.
	2010/02/16 - Conversions from any_t need to be changed to reflect
		recent changes to LevelObjectData.
	2010/05/06 - Replaced the converter macros with templates. Also added a
		num_from_string template function to use instead of basing
		everything on to_real_l(string_t).
	2010/05/07 - Removed bounds checking from converters. Not sure how
		useful they were to begin with. If someone comes up with a
		convincing reason, they'll be added back as an option.
*/

#include "types.hpp"

#include "global_object.hpp"
#include "options.hpp"

#include "exceptions/ParsingException.hpp"

#include "types/binary.hpp"
#include "types/int_t.hpp"
#include "types/real_t.hpp"
#include "types/string_t.hpp"

#include "../common/foreach.hpp"

#include <iostream>
#include <stdexcept>



template<typename T>
bool_t bool_from_string(T const & value)
{
	if (value == T("false") || value == T("FALSE")) return false;
	if (value == T("true")  || value == T("TRUE"))  return true;

	return value != T();
}

template<typename T>
T from_obj_t(obj_t o)
{
	if (o->getData().getType() == any_t::OBJMAP_T)
		return convert<T, int_s_t>(get_object_index(o));
	else
		return convert<T, any_t>(o->getData());
}

template<class T>
std::string make_string(T const & in);
template<>
std::string make_string<bool_t>(bool_t const & in)
{
	return in ? misc_name_true() : misc_name_false();
}
template<>
std::string make_string<string_t>(string_t const & in)
{
	return in.makeString();
}
template<>
std::string make_string<string8_t>(string8_t const & in)
{
	return in.makeString();
}
template<>
std::string make_string<string16_t>(string16_t const & in)
{
	return in.makeString();
}
template<>
std::string make_string<string32_t>(string32_t const & in)
{
	return in.makeString();
}
template<>
std::string make_string<string80_t>(string80_t const & in)
{
	return in.makeString();
}
template<>
std::string make_string<string320_t>(string320_t const & in)
{
	return in.makeString();
}

template <class T>
T num_from_char(char c)
{
	switch (c)
	{
		case '0': return T( 0);
		case '1': return T( 1);
		case '2': return T( 2);
		case '3': return T( 3);
		case '4': return T( 4);
		case '5': return T( 5);
		case '6': return T( 6);
		case '7': return T( 7);
		case '8': return T( 8);
		case '9': return T( 9);
		case 'A': return T(10);
		case 'B': return T(11);
		case 'C': return T(12);
		case 'D': return T(13);
		case 'E': return T(14);
		case 'F': return T(15);
		case 'a': return T(10);
		case 'b': return T(11);
		case 'c': return T(12);
		case 'd': return T(13);
		case 'e': return T(14);
		case 'f': return T(15);

		default: throw ParsingException("invalid digit:'" + make_string(c) + "'");
	}
}

template <class T>
T num_from_char(char c, int base)
{
	T digit = num_from_char<T>(c);

	if (digit >= T(base))
		throw ParsingException("invalid digit:" + make_string(digit) + ":for base:" + make_string(base));

	return digit;
}

template <class T>
T num_from_string(std::string const & value)
{
	if (value.empty()) return T(0);

	// May get numbers in the form of fractions at times. This should only
	// happen with mpq_class, so not concerned about loss of precision from
	// the operation.
	size_t pos = value.find_first_of('/');
	if (pos != std::string::npos)
		return num_from_string<T>(value.substr(0, pos)) / num_from_string<T>(value.substr(pos+1));

	int base(10);
	std::string num_str;
	std::string exp_str;

	if (value[0] == '0')
	{
		switch (value.size() >= 2 ? value[1] : 0)
		{
		case 'x':
		case 'X':
			base    = 16;
			num_str = value.substr(2);
			break;

		case 'd':
		case 'D':
			base    = 10;
			num_str = value.substr(2);
			break;

		case 'o':
		case 'O':
			base    = 8;
			num_str = value.substr(2);
			break;

		case 'b':
		case 'B':
			base    = 2;
			num_str = value.substr(2);
			break;

		case '.':
			base    = 10;
			num_str = value.substr(1);
			break;

		default:
			base    = 8;
			num_str = value.substr(1);
			break;
		}
	}

	if (num_str.empty())
		num_str = value;

	// base 15 = 0123456789ABCDE
	pos = num_str.find_first_of(base < 15 ? "eE" : "+-");

	if (pos != std::string::npos && pos != num_str.size()-1)
	{
		// For future reference, we NEED THE SIGN!
		exp_str = num_str.substr(   pos + (base < 15 ? 1 : 0));
		num_str = num_str.substr(0, pos - (base < 15 ? 0 : 1));

		if (exp_str[0] == '+')
			exp_str.erase(0, 1);
	}

	T num(0);

	pos = 0;

	char num_char = 0;

	while (pos < num_str.size())
	{
		num_char = num_str[pos++];

		if (num_char == '.') break;

		num *= T(base);
		num += num_from_char<T>(num_char, base);
	}

	if (num_char == '.')
	{
		pos = num_str.size();
		T num_part(0);

		while ((num_char = num_str[--pos]) != '.')
		{
			num_part += num_from_char<T>(num_char, base);

			num_part /= T(base);
		}

		num += num_part;
	}

	if (!exp_str.empty())
	{
		int_s_t exp = num_from_string<int_s_t>(exp_str);

		if (exp < 0)
		{
			for (; exp; ++exp)
				num /= T(base);
		}
		else
		{
			for (; exp; --exp)
				num *= T(base);
		}
	}

	return num;
}

template<> any_t convert<any_t, any_t>      (any_t       const & value) {return value;}
template<> any_t convert<any_t, bool_t>     (bool_t      const & value) {return value;}
template<> any_t convert<any_t, int_s_t>    (int_s_t     const & value) {return value;}
template<> any_t convert<any_t, int_t>      (int_t       const & value) {return value;}
template<> any_t convert<any_t, int_l_t>    (int_l_t     const & value) {return value;}
template<> any_t convert<any_t, obj_t>      (obj_t       const & value) {return from_obj_t<any_t>(value);}
template<> any_t convert<any_t, real_s_t>   (real_s_t    const & value) {return value;}
template<> any_t convert<any_t, real_t>     (real_t      const & value) {return value;}
template<> any_t convert<any_t, real_l_t>   (real_l_t    const & value) {return value;}
template<> any_t convert<any_t, string_t>   (string_t    const & value) {return value;}
template<> any_t convert<any_t, string8_t>  (string8_t   const & value) {return value;}
template<> any_t convert<any_t, string16_t> (string16_t  const & value) {return value;}
template<> any_t convert<any_t, string32_t> (string32_t  const & value) {return value;}
template<> any_t convert<any_t, string80_t> (string80_t  const & value) {return value;}
template<> any_t convert<any_t, string320_t>(string320_t const & value) {return value;}
template<> any_t convert<any_t, ubyte_t>    (ubyte_t     const & value) {return value;}
template<> any_t convert<any_t, sword_t>    (sword_t     const & value) {return value;}
template<> any_t convert<any_t, uword_t>    (uword_t     const & value) {return value;}
template<> any_t convert<any_t, sdword_t>   (sdword_t    const & value) {return value;}
template<> any_t convert<any_t, udword_t>   (udword_t    const & value) {return value;}

template<> bool_t convert<bool_t, any_t>      (any_t       const & value) {return value.toBool();}
template<> bool_t convert<bool_t, bool_t>     (bool_t      const & value) {return value;}
template<> bool_t convert<bool_t, int_s_t>    (int_s_t     const & value) {return value != int_s_t();}
template<> bool_t convert<bool_t, int_t>      (int_t       const & value) {return value != int_t();}
template<> bool_t convert<bool_t, int_l_t>    (int_l_t     const & value) {return value != int_l_t();}
template<> bool_t convert<bool_t, obj_t>      (obj_t       const & value) {return from_obj_t<bool_t>(value);}
template<> bool_t convert<bool_t, real_s_t>   (real_s_t    const & value) {return value != int_s_t();}
template<> bool_t convert<bool_t, real_t>     (real_t      const & value) {return value != real_t();}
template<> bool_t convert<bool_t, real_l_t>   (real_l_t    const & value) {return value != real_l_t();}
template<> bool_t convert<bool_t, string_t>   (string_t    const & value) {return bool_from_string(value);}
template<> bool_t convert<bool_t, string8_t>  (string8_t   const & value) {return bool_from_string(value);}
template<> bool_t convert<bool_t, string16_t> (string16_t  const & value) {return bool_from_string(value);}
template<> bool_t convert<bool_t, string32_t> (string32_t  const & value) {return bool_from_string(value);}
template<> bool_t convert<bool_t, string80_t> (string80_t  const & value) {return bool_from_string(value);}
template<> bool_t convert<bool_t, string320_t>(string320_t const & value) {return bool_from_string(value);}
template<> bool_t convert<bool_t, ubyte_t>    (ubyte_t     const & value) {return value != ubyte_t();}
template<> bool_t convert<bool_t, sword_t>    (sword_t     const & value) {return value != sword_t();}
template<> bool_t convert<bool_t, uword_t>    (uword_t     const & value) {return value != uword_t();}
template<> bool_t convert<bool_t, sdword_t>   (sdword_t    const & value) {return value != sdword_t();}
template<> bool_t convert<bool_t, udword_t>   (udword_t    const & value) {return value != udword_t();}

template<> int_s_t convert<int_s_t, any_t>      (any_t       const & value) {return value.toIntShort();}
template<> int_s_t convert<int_s_t, bool_t>     (bool_t      const & value) {return int_s_t(value);}
template<> int_s_t convert<int_s_t, int_s_t>    (int_s_t     const & value) {return value;}
template<> int_s_t convert<int_s_t, int_t>      (int_t       const & value) {return int_s_t(value.makeInt());}
template<> int_s_t convert<int_s_t, int_l_t>    (int_l_t     const & value) {return int_s_t(value.makeInt());}
template<> int_s_t convert<int_s_t, obj_t>      (obj_t       const & value) {return from_obj_t<int_s_t>(value);}
template<> int_s_t convert<int_s_t, real_s_t>   (real_s_t    const & value) {return int_s_t(value);}
template<> int_s_t convert<int_s_t, real_t>     (real_t      const & value) {return int_s_t(value.makeInt());}
template<> int_s_t convert<int_s_t, real_l_t>   (real_l_t    const & value) {return int_s_t(value.makeInt());}
template<> int_s_t convert<int_s_t, string_t>   (string_t    const & value) {return num_from_string<int_s_t>(value.makeString());}
template<> int_s_t convert<int_s_t, string8_t>  (string8_t   const & value) {return num_from_string<int_s_t>(value.makeString());}
template<> int_s_t convert<int_s_t, string16_t> (string16_t  const & value) {return num_from_string<int_s_t>(value.makeString());}
template<> int_s_t convert<int_s_t, string32_t> (string32_t  const & value) {return num_from_string<int_s_t>(value.makeString());}
template<> int_s_t convert<int_s_t, string80_t> (string80_t  const & value) {return num_from_string<int_s_t>(value.makeString());}
template<> int_s_t convert<int_s_t, string320_t>(string320_t const & value) {return num_from_string<int_s_t>(value.makeString());}
template<> int_s_t convert<int_s_t, ubyte_t>    (ubyte_t     const & value) {return int_s_t(value.makeInt());}
template<> int_s_t convert<int_s_t, sword_t>    (sword_t     const & value) {return int_s_t(value.makeInt());}
template<> int_s_t convert<int_s_t, uword_t>    (uword_t     const & value) {return int_s_t(value.makeInt());}
template<> int_s_t convert<int_s_t, sdword_t>   (sdword_t    const & value) {return int_s_t(value.makeInt());}
template<> int_s_t convert<int_s_t, udword_t>   (udword_t    const & value) {return int_s_t(value.makeInt());}

template<> int_t convert<int_t, any_t>      (any_t       const & value) {return value.toInt();}
template<> int_t convert<int_t, bool_t>     (bool_t      const & value) {return int_t((int_s_t)value);}
template<> int_t convert<int_t, int_s_t>    (int_s_t     const & value) {return int_t(value);}
template<> int_t convert<int_t, int_t>      (int_t       const & value) {return value;}
template<> int_t convert<int_t, int_l_t>    (int_l_t     const & value) {return int_t(value._data);}
template<> int_t convert<int_t, obj_t>      (obj_t       const & value) {return from_obj_t<int_t>(value);}
template<> int_t convert<int_t, real_s_t>   (real_s_t    const & value) {return int_t(value);}
template<> int_t convert<int_t, real_t>     (real_t      const & value) {return int_t(value._data);}
template<> int_t convert<int_t, real_l_t>   (real_l_t    const & value) {return int_t(value._data);}
template<> int_t convert<int_t, string_t>   (string_t    const & value) {return num_from_string<int_t>(value.makeString());}
template<> int_t convert<int_t, string8_t>  (string8_t   const & value) {return num_from_string<int_t>(value.makeString());}
template<> int_t convert<int_t, string16_t> (string16_t  const & value) {return num_from_string<int_t>(value.makeString());}
template<> int_t convert<int_t, string32_t> (string32_t  const & value) {return num_from_string<int_t>(value.makeString());}
template<> int_t convert<int_t, string80_t> (string80_t  const & value) {return num_from_string<int_t>(value.makeString());}
template<> int_t convert<int_t, string320_t>(string320_t const & value) {return num_from_string<int_t>(value.makeString());}
template<> int_t convert<int_t, ubyte_t>    (ubyte_t     const & value) {return int_t(value.makeInt());}
template<> int_t convert<int_t, sword_t>    (sword_t     const & value) {return int_t(value.makeInt());}
template<> int_t convert<int_t, uword_t>    (uword_t     const & value) {return int_t(value.makeInt());}
template<> int_t convert<int_t, sdword_t>   (sdword_t    const & value) {return int_t(value.makeInt());}
template<> int_t convert<int_t, udword_t>   (udword_t    const & value) {return int_t(value.makeInt());}

template<> int_l_t convert<int_l_t, any_t>      (any_t       const & value) {return value.toIntLong();}
template<> int_l_t convert<int_l_t, bool_t>     (bool_t      const & value) {return int_l_t((int_s_t)value);}
template<> int_l_t convert<int_l_t, int_s_t>    (int_s_t     const & value) {return int_l_t(value);}
template<> int_l_t convert<int_l_t, int_t>      (int_t       const & value) {return int_l_t(value._data);}
template<> int_l_t convert<int_l_t, int_l_t>    (int_l_t     const & value) {return value;}
template<> int_l_t convert<int_l_t, obj_t>      (obj_t       const & value) {return from_obj_t<int_l_t>(value);}
template<> int_l_t convert<int_l_t, real_s_t>   (real_s_t    const & value) {return int_l_t(value);}
template<> int_l_t convert<int_l_t, real_t>     (real_t      const & value) {return int_l_t(value._data);}
template<> int_l_t convert<int_l_t, real_l_t>   (real_l_t    const & value) {return int_l_t(value._data);}
template<> int_l_t convert<int_l_t, string_t>   (string_t    const & value) {return num_from_string<int_l_t>(value.makeString());}
template<> int_l_t convert<int_l_t, string8_t>  (string8_t   const & value) {return num_from_string<int_l_t>(value.makeString());}
template<> int_l_t convert<int_l_t, string16_t> (string16_t  const & value) {return num_from_string<int_l_t>(value.makeString());}
template<> int_l_t convert<int_l_t, string32_t> (string32_t  const & value) {return num_from_string<int_l_t>(value.makeString());}
template<> int_l_t convert<int_l_t, string80_t> (string80_t  const & value) {return num_from_string<int_l_t>(value.makeString());}
template<> int_l_t convert<int_l_t, string320_t>(string320_t const & value) {return num_from_string<int_l_t>(value.makeString());}
template<> int_l_t convert<int_l_t, ubyte_t>    (ubyte_t     const & value) {return int_l_t(value.makeInt());}
template<> int_l_t convert<int_l_t, sword_t>    (sword_t     const & value) {return int_l_t(value.makeInt());}
template<> int_l_t convert<int_l_t, uword_t>    (uword_t     const & value) {return int_l_t(value.makeInt());}
template<> int_l_t convert<int_l_t, sdword_t>   (sdword_t    const & value) {return int_l_t(value.makeInt());}
template<> int_l_t convert<int_l_t, udword_t>   (udword_t    const & value) {return int_l_t(value.makeInt());}

template<> obj_t convert<obj_t, any_t>      (any_t       const & value) {throw std::invalid_argument("to_obj(any_t)");}
template<> obj_t convert<obj_t, bool_t>     (bool_t      const & value) {throw std::invalid_argument("to_obj(bool_t)");}
template<> obj_t convert<obj_t, int_s_t>    (int_s_t     const & value) {throw std::invalid_argument("to_obj(int_s_t)");}
template<> obj_t convert<obj_t, int_t>      (int_t       const & value) {throw std::invalid_argument("to_obj(int_t)");}
template<> obj_t convert<obj_t, int_l_t>    (int_l_t     const & value) {throw std::invalid_argument("to_obj(int_l_t)");}
template<> obj_t convert<obj_t, obj_t>      (obj_t       const & value) {return value;}
template<> obj_t convert<obj_t, real_s_t>   (real_s_t    const & value) {throw std::invalid_argument("to_obj(real_s_t)");}
template<> obj_t convert<obj_t, real_t>     (real_t      const & value) {throw std::invalid_argument("to_obj(real_t)");}
template<> obj_t convert<obj_t, real_l_t>   (real_l_t    const & value) {throw std::invalid_argument("to_obj(real_l_t)");}
template<> obj_t convert<obj_t, string_t>   (string_t    const & value) {throw std::invalid_argument("to_obj(string_t)");}
template<> obj_t convert<obj_t, string8_t>  (string8_t   const & value) {throw std::invalid_argument("to_obj(string8_t)");}
template<> obj_t convert<obj_t, string16_t> (string16_t  const & value) {throw std::invalid_argument("to_obj(string16_t)");}
template<> obj_t convert<obj_t, string32_t> (string32_t  const & value) {throw std::invalid_argument("to_obj(string32_t)");}
template<> obj_t convert<obj_t, string80_t> (string80_t  const & value) {throw std::invalid_argument("to_obj(string80_t)");}
template<> obj_t convert<obj_t, string320_t>(string320_t const & value) {throw std::invalid_argument("to_obj(string320_t)");}
template<> obj_t convert<obj_t, ubyte_t>    (ubyte_t     const & value) {throw std::invalid_argument("to_obj(ubyte_t)");}
template<> obj_t convert<obj_t, sword_t>    (sword_t     const & value) {throw std::invalid_argument("to_obj(sword_t)");}
template<> obj_t convert<obj_t, uword_t>    (uword_t     const & value) {throw std::invalid_argument("to_obj(uword_t)");}
template<> obj_t convert<obj_t, sdword_t>   (sdword_t    const & value) {throw std::invalid_argument("to_obj(sdword_t)");}
template<> obj_t convert<obj_t, udword_t>   (udword_t    const & value) {throw std::invalid_argument("to_obj(udword_t)");}

template<> real_s_t convert<real_s_t, any_t>      (any_t       const & value) {return value.toRealShort();}
template<> real_s_t convert<real_s_t, bool_t>     (bool_t      const & value) {return (int_s_t)value;}
template<> real_s_t convert<real_s_t, int_s_t>    (int_s_t     const & value) {return value;}
template<> real_s_t convert<real_s_t, int_t>      (int_t       const & value) {return value.makeFloat();}
template<> real_s_t convert<real_s_t, int_l_t>    (int_l_t     const & value) {return value.makeFloat();}
template<> real_s_t convert<real_s_t, obj_t>      (obj_t       const & value) {return from_obj_t<real_s_t>(value);}
template<> real_s_t convert<real_s_t, real_s_t>   (real_s_t    const & value) {return value;}
template<> real_s_t convert<real_s_t, real_t>     (real_t      const & value) {return value.makeFloat();}
template<> real_s_t convert<real_s_t, real_l_t>   (real_l_t    const & value) {return value.makeFloat();}
template<> real_s_t convert<real_s_t, string_t>   (string_t    const & value) {return num_from_string<real_s_t>(value.makeString());}
template<> real_s_t convert<real_s_t, string8_t>  (string8_t   const & value) {return num_from_string<real_s_t>(value.makeString());}
template<> real_s_t convert<real_s_t, string16_t> (string16_t  const & value) {return num_from_string<real_s_t>(value.makeString());}
template<> real_s_t convert<real_s_t, string32_t> (string32_t  const & value) {return num_from_string<real_s_t>(value.makeString());}
template<> real_s_t convert<real_s_t, string80_t> (string80_t  const & value) {return num_from_string<real_s_t>(value.makeString());}
template<> real_s_t convert<real_s_t, string320_t>(string320_t const & value) {return num_from_string<real_s_t>(value.makeString());}
template<> real_s_t convert<real_s_t, ubyte_t>    (ubyte_t     const & value) {return real_s_t(value.makeInt());}
template<> real_s_t convert<real_s_t, sword_t>    (sword_t     const & value) {return real_s_t(value.makeInt());}
template<> real_s_t convert<real_s_t, uword_t>    (uword_t     const & value) {return real_s_t(value.makeInt());}
template<> real_s_t convert<real_s_t, sdword_t>   (sdword_t    const & value) {return real_s_t(value.makeInt());}
template<> real_s_t convert<real_s_t, udword_t>   (udword_t    const & value) {return real_s_t(value.makeInt());}

template<> real_t convert<real_t, any_t>      (any_t       const & value) {return value.toReal();}
template<> real_t convert<real_t, bool_t>     (bool_t      const & value) {return real_t((int)value);}
template<> real_t convert<real_t, int_s_t>    (int_s_t     const & value) {return real_t(value);}
template<> real_t convert<real_t, int_t>      (int_t       const & value) {return real_t(value._data);}
template<> real_t convert<real_t, int_l_t>    (int_l_t     const & value) {return real_t(value._data);}
template<> real_t convert<real_t, obj_t>      (obj_t       const & value) {return from_obj_t<real_t>(value);}
template<> real_t convert<real_t, real_s_t>   (real_s_t    const & value) {return real_t(value);}
template<> real_t convert<real_t, real_t>     (real_t      const & value) {return value;}
template<> real_t convert<real_t, real_l_t>   (real_l_t    const & value) {return real_t(value._data);}
template<> real_t convert<real_t, string_t>   (string_t    const & value) {return num_from_string<real_t>(value.makeString());}
template<> real_t convert<real_t, string8_t>  (string8_t   const & value) {return num_from_string<real_t>(value.makeString());}
template<> real_t convert<real_t, string16_t> (string16_t  const & value) {return num_from_string<real_t>(value.makeString());}
template<> real_t convert<real_t, string32_t> (string32_t  const & value) {return num_from_string<real_t>(value.makeString());}
template<> real_t convert<real_t, string80_t> (string80_t  const & value) {return num_from_string<real_t>(value.makeString());}
template<> real_t convert<real_t, string320_t>(string320_t const & value) {return num_from_string<real_t>(value.makeString());}
template<> real_t convert<real_t, ubyte_t>    (ubyte_t     const & value) {return real_t(value.makeInt());}
template<> real_t convert<real_t, sword_t>    (sword_t     const & value) {return real_t(value.makeInt());}
template<> real_t convert<real_t, uword_t>    (uword_t     const & value) {return real_t(value.makeInt());}
template<> real_t convert<real_t, sdword_t>   (sdword_t    const & value) {return real_t(value.makeInt());}
template<> real_t convert<real_t, udword_t>   (udword_t    const & value) {return real_t(value.makeInt());}

template<> real_l_t convert<real_l_t, any_t>      (any_t       const & value) {return value.toRealLong();}
template<> real_l_t convert<real_l_t, bool_t>     (bool_t      const & value) {return real_l_t((int)value);}
template<> real_l_t convert<real_l_t, int_s_t>    (int_s_t     const & value) {return real_l_t(value);}
template<> real_l_t convert<real_l_t, int_t>      (int_t       const & value) {return real_l_t(value._data);}
template<> real_l_t convert<real_l_t, int_l_t>    (int_l_t     const & value) {return real_l_t(value._data);}
template<> real_l_t convert<real_l_t, obj_t>      (obj_t       const & value) {return from_obj_t<real_l_t>(value);}
template<> real_l_t convert<real_l_t, real_s_t>   (real_s_t    const & value) {return real_l_t(value);}
template<> real_l_t convert<real_l_t, real_t>     (real_t      const & value) {return real_l_t(value._data);}
template<> real_l_t convert<real_l_t, real_l_t>   (real_l_t    const & value) {return value;}
template<> real_l_t convert<real_l_t, string_t>   (string_t    const & value) {return num_from_string<real_l_t>(value.makeString());}
template<> real_l_t convert<real_l_t, string8_t>  (string8_t   const & value) {return num_from_string<real_l_t>(value.makeString());}
template<> real_l_t convert<real_l_t, string16_t> (string16_t  const & value) {return num_from_string<real_l_t>(value.makeString());}
template<> real_l_t convert<real_l_t, string32_t> (string32_t  const & value) {return num_from_string<real_l_t>(value.makeString());}
template<> real_l_t convert<real_l_t, string80_t> (string80_t  const & value) {return num_from_string<real_l_t>(value.makeString());}
template<> real_l_t convert<real_l_t, string320_t>(string320_t const & value) {return num_from_string<real_l_t>(value.makeString());}
template<> real_l_t convert<real_l_t, ubyte_t>    (ubyte_t     const & value) {return real_l_t(value.makeInt());}
template<> real_l_t convert<real_l_t, sword_t>    (sword_t     const & value) {return real_l_t(value.makeInt());}
template<> real_l_t convert<real_l_t, uword_t>    (uword_t     const & value) {return real_l_t(value.makeInt());}
template<> real_l_t convert<real_l_t, sdword_t>   (sdword_t    const & value) {return real_l_t(value.makeInt());}
template<> real_l_t convert<real_l_t, udword_t>   (udword_t    const & value) {return real_l_t(value.makeInt());}

template<> string_t convert<string_t, any_t>      (any_t       const & value) {return value.toString();}
template<> string_t convert<string_t, bool_t>     (bool_t      const & value) {return string_t(make_string(value));}
template<> string_t convert<string_t, int_s_t>    (int_s_t     const & value) {return string_t(make_string(value));}
template<> string_t convert<string_t, int_t>      (int_t       const & value) {return string_t(make_string(value));}
template<> string_t convert<string_t, int_l_t>    (int_l_t     const & value) {return string_t(make_string(value));}
template<> string_t convert<string_t, obj_t>      (obj_t       const & value) {return from_obj_t<string_t>(value);}
template<> string_t convert<string_t, real_s_t>   (real_s_t    const & value) {return string_t(make_string(value));}
template<> string_t convert<string_t, real_t>     (real_t      const & value) {return string_t(make_string(value));}
template<> string_t convert<string_t, real_l_t>   (real_l_t    const & value) {return string_t(make_string(value));}
template<> string_t convert<string_t, string_t>   (string_t    const & value) {return value;}
template<> string_t convert<string_t, string8_t>  (string8_t   const & value) {return string_t(value.makeString());}
template<> string_t convert<string_t, string16_t> (string16_t  const & value) {return string_t(value.makeString());}
template<> string_t convert<string_t, string32_t> (string32_t  const & value) {return string_t(value.makeString());}
template<> string_t convert<string_t, string80_t> (string80_t  const & value) {return string_t(value.makeString());}
template<> string_t convert<string_t, string320_t>(string320_t const & value) {return string_t(value.makeString());}
template<> string_t convert<string_t, ubyte_t>    (ubyte_t     const & value) {return string_t(make_string(value));}
template<> string_t convert<string_t, sword_t>    (sword_t     const & value) {return string_t(make_string(value));}
template<> string_t convert<string_t, uword_t>    (uword_t     const & value) {return string_t(make_string(value));}
template<> string_t convert<string_t, sdword_t>   (sdword_t    const & value) {return string_t(make_string(value));}
template<> string_t convert<string_t, udword_t>   (udword_t    const & value) {return string_t(make_string(value));}

template<> string8_t convert<string8_t, any_t>      (any_t       const & value) {return value.toString8();}
template<> string8_t convert<string8_t, bool_t>     (bool_t      const & value) {return string8_t(make_string(value));}
template<> string8_t convert<string8_t, int_s_t>    (int_s_t     const & value) {return string8_t(make_string(value));}
template<> string8_t convert<string8_t, int_t>      (int_t       const & value) {return string8_t(make_string(value));}
template<> string8_t convert<string8_t, int_l_t>    (int_l_t     const & value) {return string8_t(make_string(value));}
template<> string8_t convert<string8_t, obj_t>      (obj_t       const & value) {return from_obj_t<string8_t>(value);}
template<> string8_t convert<string8_t, real_s_t>   (real_s_t    const & value) {return string8_t(make_string(value));}
template<> string8_t convert<string8_t, real_t>     (real_t      const & value) {return string8_t(make_string(value));}
template<> string8_t convert<string8_t, real_l_t>   (real_l_t    const & value) {return string8_t(make_string(value));}
template<> string8_t convert<string8_t, string_t>   (string_t    const & value) {return string8_t(value.makeString());}
template<> string8_t convert<string8_t, string8_t>  (string8_t   const & value) {return value;}
template<> string8_t convert<string8_t, string16_t> (string16_t  const & value) {return string8_t(value.makeString());}
template<> string8_t convert<string8_t, string32_t> (string32_t  const & value) {return string8_t(value.makeString());}
template<> string8_t convert<string8_t, string80_t> (string80_t  const & value) {return string8_t(value.makeString());}
template<> string8_t convert<string8_t, string320_t>(string320_t const & value) {return string8_t(value.makeString());}
template<> string8_t convert<string8_t, ubyte_t>    (ubyte_t     const & value) {return string8_t(make_string(value));}
template<> string8_t convert<string8_t, sword_t>    (sword_t     const & value) {return string8_t(make_string(value));}
template<> string8_t convert<string8_t, uword_t>    (uword_t     const & value) {return string8_t(make_string(value));}
template<> string8_t convert<string8_t, sdword_t>   (sdword_t    const & value) {return string8_t(make_string(value));}
template<> string8_t convert<string8_t, udword_t>   (udword_t    const & value) {return string8_t(make_string(value));}

template<> string16_t convert<string16_t, any_t>      (any_t       const & value) {return value.toString16();}
template<> string16_t convert<string16_t, bool_t>     (bool_t      const & value) {return string16_t(make_string(value));}
template<> string16_t convert<string16_t, int_s_t>    (int_s_t     const & value) {return string16_t(make_string(value));}
template<> string16_t convert<string16_t, int_t>      (int_t       const & value) {return string16_t(make_string(value));}
template<> string16_t convert<string16_t, int_l_t>    (int_l_t     const & value) {return string16_t(make_string(value));}
template<> string16_t convert<string16_t, obj_t>      (obj_t       const & value) {return from_obj_t<string16_t>(value);}
template<> string16_t convert<string16_t, real_s_t>   (real_s_t    const & value) {return string16_t(make_string(value));}
template<> string16_t convert<string16_t, real_t>     (real_t      const & value) {return string16_t(make_string(value));}
template<> string16_t convert<string16_t, real_l_t>   (real_l_t    const & value) {return string16_t(make_string(value));}
template<> string16_t convert<string16_t, string_t>   (string_t    const & value) {return string16_t(value.makeString());}
template<> string16_t convert<string16_t, string8_t>  (string8_t   const & value) {return string16_t(value.makeString());}
template<> string16_t convert<string16_t, string16_t> (string16_t  const & value) {return value;}
template<> string16_t convert<string16_t, string32_t> (string32_t  const & value) {return string16_t(value.makeString());}
template<> string16_t convert<string16_t, string80_t> (string80_t  const & value) {return string16_t(value.makeString());}
template<> string16_t convert<string16_t, string320_t>(string320_t const & value) {return string16_t(value.makeString());}
template<> string16_t convert<string16_t, ubyte_t>    (ubyte_t     const & value) {return string16_t(make_string(value));}
template<> string16_t convert<string16_t, sword_t>    (sword_t     const & value) {return string16_t(make_string(value));}
template<> string16_t convert<string16_t, uword_t>    (uword_t     const & value) {return string16_t(make_string(value));}
template<> string16_t convert<string16_t, sdword_t>   (sdword_t    const & value) {return string16_t(make_string(value));}
template<> string16_t convert<string16_t, udword_t>   (udword_t    const & value) {return string16_t(make_string(value));}

template<> string32_t convert<string32_t, any_t>      (any_t       const & value) {return value.toString32();}
template<> string32_t convert<string32_t, bool_t>     (bool_t      const & value) {return string32_t(make_string(value));}
template<> string32_t convert<string32_t, int_s_t>    (int_s_t     const & value) {return string32_t(make_string(value));}
template<> string32_t convert<string32_t, int_t>      (int_t       const & value) {return string32_t(make_string(value));}
template<> string32_t convert<string32_t, int_l_t>    (int_l_t     const & value) {return string32_t(make_string(value));}
template<> string32_t convert<string32_t, obj_t>      (obj_t       const & value) {return from_obj_t<string32_t>(value);}
template<> string32_t convert<string32_t, real_s_t>   (real_s_t    const & value) {return string32_t(make_string(value));}
template<> string32_t convert<string32_t, real_t>     (real_t      const & value) {return string32_t(make_string(value));}
template<> string32_t convert<string32_t, real_l_t>   (real_l_t    const & value) {return string32_t(make_string(value));}
template<> string32_t convert<string32_t, string_t>   (string_t    const & value) {return string32_t(value.makeString());}
template<> string32_t convert<string32_t, string8_t>  (string8_t   const & value) {return string32_t(value.makeString());}
template<> string32_t convert<string32_t, string16_t> (string16_t  const & value) {return string32_t(value.makeString());}
template<> string32_t convert<string32_t, string32_t> (string32_t  const & value) {return value;}
template<> string32_t convert<string32_t, string80_t> (string80_t  const & value) {return string32_t(value.makeString());}
template<> string32_t convert<string32_t, string320_t>(string320_t const & value) {return string32_t(value.makeString());}
template<> string32_t convert<string32_t, ubyte_t>    (ubyte_t     const & value) {return string32_t(make_string(value));}
template<> string32_t convert<string32_t, sword_t>    (sword_t     const & value) {return string32_t(make_string(value));}
template<> string32_t convert<string32_t, uword_t>    (uword_t     const & value) {return string32_t(make_string(value));}
template<> string32_t convert<string32_t, sdword_t>   (sdword_t    const & value) {return string32_t(make_string(value));}
template<> string32_t convert<string32_t, udword_t>   (udword_t    const & value) {return string32_t(make_string(value));}

template<> string80_t convert<string80_t, any_t>      (any_t       const & value) {return value.toString80();}
template<> string80_t convert<string80_t, bool_t>     (bool_t      const & value) {return string80_t(make_string(value));}
template<> string80_t convert<string80_t, int_s_t>    (int_s_t     const & value) {return string80_t(make_string(value));}
template<> string80_t convert<string80_t, int_t>      (int_t       const & value) {return string80_t(make_string(value));}
template<> string80_t convert<string80_t, int_l_t>    (int_l_t     const & value) {return string80_t(make_string(value));}
template<> string80_t convert<string80_t, obj_t>      (obj_t       const & value) {return from_obj_t<string80_t>(value);}
template<> string80_t convert<string80_t, real_s_t>   (real_s_t    const & value) {return string80_t(make_string(value));}
template<> string80_t convert<string80_t, real_t>     (real_t      const & value) {return string80_t(make_string(value));}
template<> string80_t convert<string80_t, real_l_t>   (real_l_t    const & value) {return string80_t(make_string(value));}
template<> string80_t convert<string80_t, string_t>   (string_t    const & value) {return string80_t(value.makeString());}
template<> string80_t convert<string80_t, string8_t>  (string8_t   const & value) {return string80_t(value.makeString());}
template<> string80_t convert<string80_t, string16_t> (string16_t  const & value) {return string80_t(value.makeString());}
template<> string80_t convert<string80_t, string32_t> (string32_t  const & value) {return string80_t(value.makeString());}
template<> string80_t convert<string80_t, string80_t> (string80_t  const & value) {return value;}
template<> string80_t convert<string80_t, string320_t>(string320_t const & value) {return string80_t(value.makeString());}
template<> string80_t convert<string80_t, ubyte_t>    (ubyte_t     const & value) {return string80_t(make_string(value));}
template<> string80_t convert<string80_t, sword_t>    (sword_t     const & value) {return string80_t(make_string(value));}
template<> string80_t convert<string80_t, uword_t>    (uword_t     const & value) {return string80_t(make_string(value));}
template<> string80_t convert<string80_t, sdword_t>   (sdword_t    const & value) {return string80_t(make_string(value));}
template<> string80_t convert<string80_t, udword_t>   (udword_t    const & value) {return string80_t(make_string(value));}

template<> string320_t convert<string320_t, any_t>      (any_t       const & value) {return value.toString320();}
template<> string320_t convert<string320_t, bool_t>     (bool_t      const & value) {return string320_t(make_string(value));}
template<> string320_t convert<string320_t, int_s_t>    (int_s_t     const & value) {return string320_t(make_string(value));}
template<> string320_t convert<string320_t, int_t>      (int_t       const & value) {return string320_t(make_string(value));}
template<> string320_t convert<string320_t, int_l_t>    (int_l_t     const & value) {return string320_t(make_string(value));}
template<> string320_t convert<string320_t, obj_t>      (obj_t       const & value) {return from_obj_t<string320_t>(value);}
template<> string320_t convert<string320_t, real_s_t>   (real_s_t    const & value) {return string320_t(make_string(value));}
template<> string320_t convert<string320_t, real_t>     (real_t      const & value) {return string320_t(make_string(value));}
template<> string320_t convert<string320_t, real_l_t>   (real_l_t    const & value) {return string320_t(make_string(value));}
template<> string320_t convert<string320_t, string_t>   (string_t    const & value) {return string320_t(value.makeString());}
template<> string320_t convert<string320_t, string8_t>  (string8_t   const & value) {return string320_t(value.makeString());}
template<> string320_t convert<string320_t, string16_t> (string16_t  const & value) {return string320_t(value.makeString());}
template<> string320_t convert<string320_t, string32_t> (string32_t  const & value) {return string320_t(value.makeString());}
template<> string320_t convert<string320_t, string80_t> (string80_t  const & value) {return string320_t(value.makeString());}
template<> string320_t convert<string320_t, string320_t>(string320_t const & value) {return value;}
template<> string320_t convert<string320_t, ubyte_t>    (ubyte_t     const & value) {return string320_t(make_string(value));}
template<> string320_t convert<string320_t, sword_t>    (sword_t     const & value) {return string320_t(make_string(value));}
template<> string320_t convert<string320_t, uword_t>    (uword_t     const & value) {return string320_t(make_string(value));}
template<> string320_t convert<string320_t, sdword_t>   (sdword_t    const & value) {return string320_t(make_string(value));}
template<> string320_t convert<string320_t, udword_t>   (udword_t    const & value) {return string320_t(make_string(value));}

template<> ubyte_t convert<ubyte_t, any_t>      (any_t       const & value) {return value.toUByte();}
template<> ubyte_t convert<ubyte_t, bool_t>     (bool_t      const & value) {return ubyte_t(value);}
template<> ubyte_t convert<ubyte_t, int_s_t>    (int_s_t     const & value) {return ubyte_t(value);}
template<> ubyte_t convert<ubyte_t, int_t>      (int_t       const & value) {return ubyte_t(value.makeInt());}
template<> ubyte_t convert<ubyte_t, int_l_t>    (int_l_t     const & value) {return ubyte_t(value.makeInt());}
template<> ubyte_t convert<ubyte_t, obj_t>      (obj_t       const & value) {return from_obj_t<ubyte_t>(value);}
template<> ubyte_t convert<ubyte_t, real_s_t>   (real_s_t    const & value) {return ubyte_t(value);}
template<> ubyte_t convert<ubyte_t, real_t>     (real_t      const & value) {return ubyte_t(value.makeInt());}
template<> ubyte_t convert<ubyte_t, real_l_t>   (real_l_t    const & value) {return ubyte_t(value.makeInt());}
template<> ubyte_t convert<ubyte_t, string_t>   (string_t    const & value) {return num_from_string<ubyte_t>(value.makeString());}
template<> ubyte_t convert<ubyte_t, string8_t>  (string8_t   const & value) {return num_from_string<ubyte_t>(value.makeString());}
template<> ubyte_t convert<ubyte_t, string16_t> (string16_t  const & value) {return num_from_string<ubyte_t>(value.makeString());}
template<> ubyte_t convert<ubyte_t, string32_t> (string32_t  const & value) {return num_from_string<ubyte_t>(value.makeString());}
template<> ubyte_t convert<ubyte_t, string80_t> (string80_t  const & value) {return num_from_string<ubyte_t>(value.makeString());}
template<> ubyte_t convert<ubyte_t, string320_t>(string320_t const & value) {return num_from_string<ubyte_t>(value.makeString());}
template<> ubyte_t convert<ubyte_t, ubyte_t>    (ubyte_t     const & value) {return value;}
template<> ubyte_t convert<ubyte_t, sword_t>    (sword_t     const & value) {return ubyte_t(value.makeInt());}
template<> ubyte_t convert<ubyte_t, uword_t>    (uword_t     const & value) {return ubyte_t(value.makeInt());}
template<> ubyte_t convert<ubyte_t, sdword_t>   (sdword_t    const & value) {return ubyte_t(value.makeInt());}
template<> ubyte_t convert<ubyte_t, udword_t>   (udword_t    const & value) {return ubyte_t(value.makeInt());}

template<> sword_t convert<sword_t, any_t>      (any_t       const & value) {return value.toSWord();}
template<> sword_t convert<sword_t, bool_t>     (bool_t      const & value) {return sword_t(value);}
template<> sword_t convert<sword_t, int_s_t>    (int_s_t     const & value) {return sword_t(value);}
template<> sword_t convert<sword_t, int_t>      (int_t       const & value) {return sword_t(value.makeInt());}
template<> sword_t convert<sword_t, int_l_t>    (int_l_t     const & value) {return sword_t(value.makeInt());}
template<> sword_t convert<sword_t, obj_t>      (obj_t       const & value) {return from_obj_t<sword_t>(value);}
template<> sword_t convert<sword_t, real_s_t>   (real_s_t    const & value) {return sword_t(value);}
template<> sword_t convert<sword_t, real_t>     (real_t      const & value) {return sword_t(value.makeInt());}
template<> sword_t convert<sword_t, real_l_t>   (real_l_t    const & value) {return sword_t(value.makeInt());}
template<> sword_t convert<sword_t, string_t>   (string_t    const & value) {return num_from_string<sword_t>(value.makeString());}
template<> sword_t convert<sword_t, string8_t>  (string8_t   const & value) {return num_from_string<sword_t>(value.makeString());}
template<> sword_t convert<sword_t, string16_t> (string16_t  const & value) {return num_from_string<sword_t>(value.makeString());}
template<> sword_t convert<sword_t, string32_t> (string32_t  const & value) {return num_from_string<sword_t>(value.makeString());}
template<> sword_t convert<sword_t, string80_t> (string80_t  const & value) {return num_from_string<sword_t>(value.makeString());}
template<> sword_t convert<sword_t, string320_t>(string320_t const & value) {return num_from_string<sword_t>(value.makeString());}
template<> sword_t convert<sword_t, ubyte_t>    (ubyte_t     const & value) {return sword_t(value.makeInt());}
template<> sword_t convert<sword_t, sword_t>    (sword_t     const & value) {return value;}
template<> sword_t convert<sword_t, uword_t>    (uword_t     const & value) {return sword_t(value.makeInt());}
template<> sword_t convert<sword_t, sdword_t>   (sdword_t    const & value) {return sword_t(value.makeInt());}
template<> sword_t convert<sword_t, udword_t>   (udword_t    const & value) {return sword_t(value.makeInt());}

template<> uword_t convert<uword_t, any_t>      (any_t       const & value) {return value.toUWord();}
template<> uword_t convert<uword_t, bool_t>     (bool_t      const & value) {return uword_t(value);}
template<> uword_t convert<uword_t, int_s_t>    (int_s_t     const & value) {return uword_t(value);}
template<> uword_t convert<uword_t, int_t>      (int_t       const & value) {return uword_t(value.makeInt());}
template<> uword_t convert<uword_t, int_l_t>    (int_l_t     const & value) {return uword_t(value.makeInt());}
template<> uword_t convert<uword_t, obj_t>      (obj_t       const & value) {return from_obj_t<uword_t>(value);}
template<> uword_t convert<uword_t, real_s_t>   (real_s_t    const & value) {return uword_t(value);}
template<> uword_t convert<uword_t, real_t>     (real_t      const & value) {return uword_t(value.makeInt());}
template<> uword_t convert<uword_t, real_l_t>   (real_l_t    const & value) {return uword_t(value.makeInt());}
template<> uword_t convert<uword_t, string_t>   (string_t    const & value) {return num_from_string<uword_t>(value.makeString());}
template<> uword_t convert<uword_t, string8_t>  (string8_t   const & value) {return num_from_string<uword_t>(value.makeString());}
template<> uword_t convert<uword_t, string16_t> (string16_t  const & value) {return num_from_string<uword_t>(value.makeString());}
template<> uword_t convert<uword_t, string32_t> (string32_t  const & value) {return num_from_string<uword_t>(value.makeString());}
template<> uword_t convert<uword_t, string80_t> (string80_t  const & value) {return num_from_string<uword_t>(value.makeString());}
template<> uword_t convert<uword_t, string320_t>(string320_t const & value) {return num_from_string<uword_t>(value.makeString());}
template<> uword_t convert<uword_t, ubyte_t>    (ubyte_t     const & value) {return uword_t(value.makeInt());}
template<> uword_t convert<uword_t, sword_t>    (sword_t     const & value) {return uword_t(value.makeInt());}
template<> uword_t convert<uword_t, uword_t>    (uword_t     const & value) {return value;}
template<> uword_t convert<uword_t, sdword_t>   (sdword_t    const & value) {return uword_t(value.makeInt());}
template<> uword_t convert<uword_t, udword_t>   (udword_t    const & value) {return uword_t(value.makeInt());}

template<> sdword_t convert<sdword_t, any_t>      (any_t       const & value) {return value.toSDWord();}
template<> sdword_t convert<sdword_t, bool_t>     (bool_t      const & value) {return sdword_t(value);}
template<> sdword_t convert<sdword_t, int_s_t>    (int_s_t     const & value) {return sdword_t(value);}
template<> sdword_t convert<sdword_t, int_t>      (int_t       const & value) {return sdword_t(value.makeInt());}
template<> sdword_t convert<sdword_t, int_l_t>    (int_l_t     const & value) {return sdword_t(value.makeInt());}
template<> sdword_t convert<sdword_t, obj_t>      (obj_t       const & value) {return from_obj_t<sdword_t>(value);}
template<> sdword_t convert<sdword_t, real_s_t>   (real_s_t    const & value) {return sdword_t(value);}
template<> sdword_t convert<sdword_t, real_t>     (real_t      const & value) {return sdword_t(value.makeInt());}
template<> sdword_t convert<sdword_t, real_l_t>   (real_l_t    const & value) {return sdword_t(value.makeInt());}
template<> sdword_t convert<sdword_t, string_t>   (string_t    const & value) {return num_from_string<sdword_t>(value.makeString());}
template<> sdword_t convert<sdword_t, string8_t>  (string8_t   const & value) {return num_from_string<sdword_t>(value.makeString());}
template<> sdword_t convert<sdword_t, string16_t> (string16_t  const & value) {return num_from_string<sdword_t>(value.makeString());}
template<> sdword_t convert<sdword_t, string32_t> (string32_t  const & value) {return num_from_string<sdword_t>(value.makeString());}
template<> sdword_t convert<sdword_t, string80_t> (string80_t  const & value) {return num_from_string<sdword_t>(value.makeString());}
template<> sdword_t convert<sdword_t, string320_t>(string320_t const & value) {return num_from_string<sdword_t>(value.makeString());}
template<> sdword_t convert<sdword_t, ubyte_t>    (ubyte_t     const & value) {return sdword_t(value.makeInt());}
template<> sdword_t convert<sdword_t, sword_t>    (sword_t     const & value) {return sdword_t(value.makeInt());}
template<> sdword_t convert<sdword_t, uword_t>    (uword_t     const & value) {return sdword_t(value.makeInt());}
template<> sdword_t convert<sdword_t, sdword_t>   (sdword_t    const & value) {return value;}
template<> sdword_t convert<sdword_t, udword_t>   (udword_t    const & value) {return sdword_t(value.makeInt());}

template<> udword_t convert<udword_t, any_t>      (any_t       const & value) {return value.toUDWord();}
template<> udword_t convert<udword_t, bool_t>     (bool_t      const & value) {return udword_t(value);}
template<> udword_t convert<udword_t, int_s_t>    (int_s_t     const & value) {return udword_t(value);}
template<> udword_t convert<udword_t, int_t>      (int_t       const & value) {return udword_t(value.makeInt());}
template<> udword_t convert<udword_t, int_l_t>    (int_l_t     const & value) {return udword_t(value.makeInt());}
template<> udword_t convert<udword_t, obj_t>      (obj_t       const & value) {return from_obj_t<udword_t>(value);}
template<> udword_t convert<udword_t, real_s_t>   (real_s_t    const & value) {return udword_t(value);}
template<> udword_t convert<udword_t, real_t>     (real_t      const & value) {return udword_t(value.makeInt());}
template<> udword_t convert<udword_t, real_l_t>   (real_l_t    const & value) {return udword_t(value.makeInt());}
template<> udword_t convert<udword_t, string_t>   (string_t    const & value) {return num_from_string<udword_t>(value.makeString());}
template<> udword_t convert<udword_t, string8_t>  (string8_t   const & value) {return num_from_string<udword_t>(value.makeString());}
template<> udword_t convert<udword_t, string16_t> (string16_t  const & value) {return num_from_string<udword_t>(value.makeString());}
template<> udword_t convert<udword_t, string32_t> (string32_t  const & value) {return num_from_string<udword_t>(value.makeString());}
template<> udword_t convert<udword_t, string80_t> (string80_t  const & value) {return num_from_string<udword_t>(value.makeString());}
template<> udword_t convert<udword_t, string320_t>(string320_t const & value) {return num_from_string<udword_t>(value.makeString());}
template<> udword_t convert<udword_t, ubyte_t>    (ubyte_t     const & value) {return udword_t(value.makeInt());}
template<> udword_t convert<udword_t, sword_t>    (sword_t     const & value) {return udword_t(value.makeInt());}
template<> udword_t convert<udword_t, uword_t>    (uword_t     const & value) {return udword_t(value.makeInt());}
template<> udword_t convert<udword_t, sdword_t>   (sdword_t    const & value) {return udword_t(value.makeInt());}
template<> udword_t convert<udword_t, udword_t>   (udword_t    const & value) {return value;}



