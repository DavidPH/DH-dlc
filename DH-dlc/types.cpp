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

#include <iostream>
#include <stdexcept>

#include "global_object.hpp"
#include "LevelObject.hpp"
#include "LevelObjectName.hpp"
#include "options.hpp"
#include "parsing.hpp"
#include "../common/foreach.hpp"
#include "exceptions/ParsingException.hpp"
#include "types/binary.hpp"
#include "types/int_t.hpp"
#include "types/real_t.hpp"
#include "types/string_t.hpp"



std::string make_string(bool in)
{
	return in ? misc_name_true() : misc_name_false();
}

template <class T, T (Ca)(any_t const &), T (Ci)(int_s_t const &)>
T from_obj_t(obj_t o)
{
	if (o->getData().getType() == any_t::OBJMAP_T)
		return Ci(get_object_index(o));
	else
		return Ca(o->getData());
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

	int base(10);
	std::string num_str;
	std::string exp_str;

	// May get numbers in the form of fractions at times. This should only
	// happen with mpq_class, so not concerned about loss of precision from
	// the operation.
	size_t pos = value.find_first_of('/');
	if (pos != std::string::npos)
		return num_from_string<T>(value.substr(0, pos)) / num_from_string<T>(value.substr(pos+1));

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
		size_t base_pos = pos-1;

		while (pos < num_str.size())
		{
			num_char = num_str[pos++];

			T num_part = num_from_char<T>(num_char, base);

			for (size_t i = pos - base_pos; i; --i)
				num_part /= T(base);

			num += num_part;
		}
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

any_t to_any(any_t     const & value) {return value;}
any_t to_any(bool_t    const & value) {return value;}
any_t to_any(int_s_t   const & value) {return value;}
any_t to_any(int_t     const & value) {return value;}
any_t to_any(int_l_t   const & value) {return value;}
any_t to_any(obj_t             value) {return from_obj_t<any_t, to_any, to_any>(value);}
any_t to_any(real_s_t  const & value) {return value;}
any_t to_any(real_t    const & value) {return value;}
any_t to_any(real_l_t  const & value) {return value;}
any_t to_any(string_t  const & value) {return value;}
any_t to_any(string8_t const & value) {return value;}
any_t to_any(sword_t   const & value) {return value;}
any_t to_any(ubyte_t   const & value) {return value;}
any_t to_any(uword_t   const & value) {return value;}

bool_t to_bool(any_t     const & value) {return value.toBool();}
bool_t to_bool(bool_t    const & value) {return value;}
bool_t to_bool(int_s_t   const & value) {return value != int_s_t();}
bool_t to_bool(int_t     const & value) {return value != int_t();}
bool_t to_bool(int_l_t   const & value) {return value != int_l_t();}
bool_t to_bool(obj_t             value) {return from_obj_t<bool_t, to_bool, to_bool>(value);}
bool_t to_bool(real_s_t  const & value) {return value != int_s_t();}
bool_t to_bool(real_t    const & value) {return value != real_t();}
bool_t to_bool(real_l_t  const & value) {return value != real_l_t();}
bool_t to_bool(string_t  const & value)
{
	if (value.makeString() == "false" || value.makeString() == "FALSE") return false;
	if (value.makeString() == "true"  || value.makeString() == "TRUE")  return true;

	return value != string_t();
}
bool_t to_bool(string8_t const & value)
{
	if (value == string8_t("false") || value == string8_t("FALSE")) return false;
	if (value == string8_t("true")  || value == string8_t("TRUE"))  return true;

	return value != string8_t();
}
bool_t to_bool(sword_t   const & value) {return value != sword_t();}
bool_t to_bool(ubyte_t   const & value) {return value != ubyte_t();}
bool_t to_bool(uword_t   const & value) {return value != uword_t();}

int_s_t to_int_s(any_t     const & value) {return value.toIntShort();}
int_s_t to_int_s(bool_t    const & value) {return int_s_t(value);}
int_s_t to_int_s(int_s_t   const & value) {return value;}
int_s_t to_int_s(int_t     const & value) {return int_s_t(value.makeInt());}
int_s_t to_int_s(int_l_t   const & value) {return int_s_t(value.makeInt());}
int_s_t to_int_s(obj_t             value) {return from_obj_t<int_s_t, to_int_s, to_int_s>(value);}
int_s_t to_int_s(real_s_t  const & value) {return int_s_t(value);}
int_s_t to_int_s(real_t    const & value) {return int_s_t(value.makeInt());}
int_s_t to_int_s(real_l_t  const & value) {return int_s_t(value.makeInt());}
int_s_t to_int_s(string_t  const & value) {return num_from_string<int_s_t>(value.makeString());}
int_s_t to_int_s(string8_t const & value) {return num_from_string<int_s_t>(value.makeString());}
int_s_t to_int_s(sword_t   const & value) {return int_s_t(value.makeInt());}
int_s_t to_int_s(ubyte_t   const & value) {return int_s_t(value.makeInt());}
int_s_t to_int_s(uword_t   const & value) {return int_s_t(value.makeInt());}

int_t to_int(any_t     const & value) {return value.toInt();}
int_t to_int(bool_t    const & value) {return int_t((int_s_t)value);}
int_t to_int(int_s_t   const & value) {return int_t(value);}
int_t to_int(int_t     const & value) {return value;}
int_t to_int(int_l_t   const & value) {return int_t(value._data);}
int_t to_int(obj_t             value) {return from_obj_t<int_t, to_int, to_int>(value);}
int_t to_int(real_s_t  const & value) {return int_t(value);}
int_t to_int(real_t    const & value) {return int_t(value._data);}
int_t to_int(real_l_t  const & value) {return int_t(value._data);}
int_t to_int(string_t  const & value) {return num_from_string<int_t>(value.makeString());}
int_t to_int(string8_t const & value) {return num_from_string<int_t>(value.makeString());}
int_t to_int(sword_t   const & value) {return int_t(value.makeInt());}
int_t to_int(ubyte_t   const & value) {return int_t(value.makeInt());}
int_t to_int(uword_t   const & value) {return int_t(value.makeInt());}

int_l_t to_int_l(any_t     const & value) {return value.toIntLong();}
int_l_t to_int_l(bool_t    const & value) {return int_l_t((int_s_t)value);}
int_l_t to_int_l(int_s_t   const & value) {return int_l_t(value);}
int_l_t to_int_l(int_t     const & value) {return int_l_t(value._data);}
int_l_t to_int_l(int_l_t   const & value) {return value;}
int_l_t to_int_l(obj_t             value) {return from_obj_t<int_l_t, to_int_l, to_int_l>(value);}
int_l_t to_int_l(real_s_t  const & value) {return int_l_t(value);}
int_l_t to_int_l(real_t    const & value) {return int_l_t(value._data);}
int_l_t to_int_l(real_l_t  const & value) {return int_l_t(value._data);}
int_l_t to_int_l(string_t  const & value) {return num_from_string<int_l_t>(value.makeString());}
int_l_t to_int_l(string8_t const & value) {return num_from_string<int_l_t>(value.makeString());}
int_l_t to_int_l(sword_t   const & value) {return int_l_t(value.makeInt());}
int_l_t to_int_l(ubyte_t   const & value) {return int_l_t(value.makeInt());}
int_l_t to_int_l(uword_t   const & value) {return int_l_t(value.makeInt());}

obj_t to_obj(any_t     const & value) {throw std::invalid_argument("to_obj(any_t)");}
obj_t to_obj(bool_t    const & value) {throw std::invalid_argument("to_obj(bool_t)");}
obj_t to_obj(int_s_t   const & value) {throw std::invalid_argument("to_obj(int_s_t)");}
obj_t to_obj(int_t     const & value) {throw std::invalid_argument("to_obj(int_t)");}
obj_t to_obj(int_l_t   const & value) {throw std::invalid_argument("to_obj(int_l_t)");}
obj_t to_obj(obj_t             value) {return value;}
obj_t to_obj(real_s_t  const & value) {throw std::invalid_argument("to_obj(real_s_t)");}
obj_t to_obj(real_t    const & value) {throw std::invalid_argument("to_obj(real_t)");}
obj_t to_obj(real_l_t  const & value) {throw std::invalid_argument("to_obj(real_l_t)");}
obj_t to_obj(string_t  const & value) {throw std::invalid_argument("to_obj(string_t)");}
obj_t to_obj(string8_t const & value) {throw std::invalid_argument("to_obj(string8_t)");}
obj_t to_obj(sword_t   const & value) {throw std::invalid_argument("to_obj(sword_t)");}
obj_t to_obj(ubyte_t   const & value) {throw std::invalid_argument("to_obj(ubyte_t)");}
obj_t to_obj(uword_t   const & value) {throw std::invalid_argument("to_obj(uword_t)");}

real_s_t to_real_s(any_t     const & value) {return value.toRealShort();}
real_s_t to_real_s(bool_t    const & value) {return (int_s_t)value;}
real_s_t to_real_s(int_s_t   const & value) {return value;}
real_s_t to_real_s(int_t     const & value) {return value.makeFloat();}
real_s_t to_real_s(int_l_t   const & value) {return value.makeFloat();}
real_s_t to_real_s(obj_t             value) {return from_obj_t<real_s_t, to_real_s, to_real_s>(value);}
real_s_t to_real_s(real_s_t  const & value) {return value;}
real_s_t to_real_s(real_t    const & value) {return value.makeFloat();}
real_s_t to_real_s(real_l_t  const & value) {return value.makeFloat();}
real_s_t to_real_s(string_t  const & value) {return num_from_string<real_s_t>(value.makeString());}
real_s_t to_real_s(string8_t const & value) {return num_from_string<real_s_t>(value.makeString());}
real_s_t to_real_s(sword_t   const & value) {return real_s_t(value.makeInt());}
real_s_t to_real_s(ubyte_t   const & value) {return real_s_t(value.makeInt());}
real_s_t to_real_s(uword_t   const & value) {return real_s_t(value.makeInt());}

real_t to_real(any_t     const & value) {return value.toReal();}
real_t to_real(bool_t    const & value) {return real_t((int)value);}
real_t to_real(int_s_t   const & value) {return real_t(value);}
real_t to_real(int_t     const & value) {return real_t(value._data);}
real_t to_real(int_l_t   const & value) {return real_t(value._data);}
real_t to_real(obj_t             value) {return from_obj_t<real_t, to_real, to_real>(value);}
real_t to_real(real_s_t  const & value) {return real_t(value);}
real_t to_real(real_t    const & value) {return value;}
real_t to_real(real_l_t  const & value) {return real_t(value._data);}
real_t to_real(string_t  const & value) {return num_from_string<real_t>(value.makeString());}
real_t to_real(string8_t const & value) {return num_from_string<real_t>(value.makeString());}
real_t to_real(sword_t   const & value) {return real_t(value.makeInt());}
real_t to_real(ubyte_t   const & value) {return real_t(value.makeInt());}
real_t to_real(uword_t   const & value) {return real_t(value.makeInt());}

real_l_t to_real_l(any_t     const & value) {return value.toRealLong();}
real_l_t to_real_l(bool_t    const & value) {return real_l_t((int)value);}
real_l_t to_real_l(int_s_t   const & value) {return real_l_t(value);}
real_l_t to_real_l(int_t     const & value) {return real_l_t(value._data);}
real_l_t to_real_l(int_l_t   const & value) {return real_l_t(value._data);}
real_l_t to_real_l(obj_t             value) {return from_obj_t<real_l_t, to_real_l, to_real_l>(value);}
real_l_t to_real_l(real_s_t  const & value) {return real_l_t(value);}
real_l_t to_real_l(real_t    const & value) {return real_l_t(value._data);}
real_l_t to_real_l(real_l_t  const & value) {return value;}
real_l_t to_real_l(string_t  const & value) {return num_from_string<real_l_t>(value.makeString());}
real_l_t to_real_l(string8_t const & value) {return num_from_string<real_l_t>(value.makeString());}
real_l_t to_real_l(sword_t   const & value) {return real_l_t(value.makeInt());}
real_l_t to_real_l(ubyte_t   const & value) {return real_l_t(value.makeInt());}
real_l_t to_real_l(uword_t   const & value) {return real_l_t(value.makeInt());}

string_t to_string(any_t     const & value) {return value.toString();}
string_t to_string(bool_t    const & value) {return string_t(make_string(value));}
string_t to_string(int_s_t   const & value) {return string_t(make_string(value));}
string_t to_string(int_t     const & value) {return string_t(make_string(value));}
string_t to_string(int_l_t   const & value) {return string_t(make_string(value));}
string_t to_string(obj_t             value) {return from_obj_t<string_t, to_string, to_string>(value);}
string_t to_string(real_s_t  const & value) {return string_t(make_string(value));}
string_t to_string(real_t    const & value) {return string_t(make_string(value));}
string_t to_string(real_l_t  const & value) {return string_t(make_string(value));}
string_t to_string(string_t  const & value) {return value;}
string_t to_string(string8_t const & value) {return string_t(value.makeString());}
string_t to_string(sword_t   const & value) {return string_t(make_string(value));}
string_t to_string(ubyte_t   const & value) {return string_t(make_string(value));}
string_t to_string(uword_t   const & value) {return string_t(make_string(value));}

string8_t to_string8(any_t     const & value) {return value.toString8();}
string8_t to_string8(bool_t    const & value) {return string8_t(make_string(value).c_str());}
string8_t to_string8(int_s_t   const & value) {return string8_t(make_string(value).c_str());}
string8_t to_string8(int_t     const & value) {return string8_t(make_string(value).c_str());}
string8_t to_string8(int_l_t   const & value) {return string8_t(make_string(value).c_str());}
string8_t to_string8(obj_t             value) {return from_obj_t<string8_t, to_string8, to_string8>(value);}
string8_t to_string8(real_s_t  const & value) {return string8_t(make_string(value).c_str());}
string8_t to_string8(real_t    const & value) {return string8_t(make_string(value).c_str());}
string8_t to_string8(real_l_t  const & value) {return string8_t(make_string(value).c_str());}
string8_t to_string8(string_t  const & value) {return string8_t(value.makeString().c_str());}
string8_t to_string8(string8_t const & value) {return value;}
string8_t to_string8(sword_t   const & value) {return string8_t(make_string(value).c_str());}
string8_t to_string8(ubyte_t   const & value) {return string8_t(make_string(value).c_str());}
string8_t to_string8(uword_t   const & value) {return string8_t(make_string(value).c_str());}

sword_t to_sword(any_t     const & value) {return value.toSWord();}
sword_t to_sword(bool_t    const & value) {return sword_t(value);}
sword_t to_sword(int_s_t   const & value) {return sword_t(value);}
sword_t to_sword(int_t     const & value) {return sword_t(value.makeInt());}
sword_t to_sword(int_l_t   const & value) {return sword_t(value.makeInt());}
sword_t to_sword(obj_t             value) {return from_obj_t<sword_t, to_sword, to_sword>(value);}
sword_t to_sword(real_s_t  const & value) {return sword_t(value);}
sword_t to_sword(real_t    const & value) {return sword_t(value.makeInt());}
sword_t to_sword(real_l_t  const & value) {return sword_t(value.makeInt());}
sword_t to_sword(string_t  const & value) {return num_from_string<sword_t>(value.makeString());}
sword_t to_sword(string8_t const & value) {return num_from_string<sword_t>(value.makeString());}
sword_t to_sword(sword_t   const & value) {return value;}
sword_t to_sword(ubyte_t   const & value) {return sword_t(value.makeInt());}
sword_t to_sword(uword_t   const & value) {return sword_t(value.makeInt());}

ubyte_t to_ubyte(any_t     const & value) {return value.toUByte();}
ubyte_t to_ubyte(bool_t    const & value) {return ubyte_t(value);}
ubyte_t to_ubyte(int_s_t   const & value) {return ubyte_t(value);}
ubyte_t to_ubyte(int_t     const & value) {return ubyte_t(value.makeInt());}
ubyte_t to_ubyte(int_l_t   const & value) {return ubyte_t(value.makeInt());}
ubyte_t to_ubyte(obj_t             value) {return from_obj_t<ubyte_t, to_ubyte, to_ubyte>(value);}
ubyte_t to_ubyte(real_s_t  const & value) {return ubyte_t(value);}
ubyte_t to_ubyte(real_t    const & value) {return ubyte_t(value.makeInt());}
ubyte_t to_ubyte(real_l_t  const & value) {return ubyte_t(value.makeInt());}
ubyte_t to_ubyte(string_t  const & value) {return num_from_string<ubyte_t>(value.makeString());}
ubyte_t to_ubyte(string8_t const & value) {return num_from_string<ubyte_t>(value.makeString());}
ubyte_t to_ubyte(sword_t   const & value) {return ubyte_t(value.makeInt());}
ubyte_t to_ubyte(ubyte_t   const & value) {return value;}
ubyte_t to_ubyte(uword_t   const & value) {return ubyte_t(value.makeInt());}

uword_t to_uword(any_t     const & value) {return value.toUWord();}
uword_t to_uword(bool_t    const & value) {return uword_t(value);}
uword_t to_uword(int_s_t   const & value) {return uword_t(value);}
uword_t to_uword(int_t     const & value) {return uword_t(value.makeInt());}
uword_t to_uword(int_l_t   const & value) {return uword_t(value.makeInt());}
uword_t to_uword(obj_t             value) {return from_obj_t<uword_t, to_uword, to_uword>(value);}
uword_t to_uword(real_s_t  const & value) {return uword_t(value);}
uword_t to_uword(real_t    const & value) {return uword_t(value.makeInt());}
uword_t to_uword(real_l_t  const & value) {return uword_t(value.makeInt());}
uword_t to_uword(string_t  const & value) {return num_from_string<uword_t>(value.makeString());}
uword_t to_uword(string8_t const & value) {return num_from_string<uword_t>(value.makeString());}
uword_t to_uword(sword_t   const & value) {return uword_t(value.makeInt());}
uword_t to_uword(ubyte_t   const & value) {return uword_t(value.makeInt());}
uword_t to_uword(uword_t   const & value) {return value;}



