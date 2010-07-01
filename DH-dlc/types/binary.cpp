/*
    Copyright 2010 David Hill

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
	2010/05/09 - Original version.
*/

#include "binary.hpp"



string8_t::string8_t()
{
	clear();
}
string8_t::string8_t(string8_t const & value)
{
	for (size_t index = 0; index < 8; ++index)
		_data[index] = value._data[index];
}
string8_t::string8_t(char const * value)
{
	// In case value is less than eight characters.
	clear();

	for (size_t index = 0; value[index] && index < 8; ++index)
		_data[index] = value[index];
}
string8_t::string8_t(std::string const & value)
{
	// In case value is less than eight characters.
	clear();

	for (size_t index = 0; index < value.size() && index < 8; ++index)
		_data[index] = value[index];
}

void string8_t::clear()
{
	for (size_t index = 0; index < 8; ++index)
		_data[index] = 0;
}

std::string string8_t::encodeBinary()
{
	std::string s(8, 0);

	for (size_t index = 0; index < 8; ++index)
		s[index] = _data[index];

	return s;
}
void string8_t::encodeBinary(std::ostream & out)
{
	for (size_t index = 0; index < 8; ++index)
		out.put(_data[index]);
}

string8_t & string8_t::operator += (string8_t const & value)
{
	size_t index = 0;

	for (; _data[index] && index < 8; ++index);

	for (size_t index2 = 0; index < 8; ++index, ++index2)
		_data[index] = value._data[index2];

	return *this;
}

string8_t & string8_t::operator = (string8_t const & value)
{
	for (size_t index = 0; index < 8; ++index)
		_data[index] = value._data[index];

	return *this;
}

sword_t::sword_t() : _data(0) {}
sword_t::sword_t(sword_t const & value) : _data(value._data) {}
sword_t::sword_t(signed short int value) : _data(value) {}

std::string sword_t::encodeBinary()
{
	std::string s(2, 0);

	s[0] = (_data     ) & 0xFF;
	s[1] = (_data >> 8) & 0xFF;

	return s;
}
void sword_t::encodeBinary(std::ostream & out)
{
	out.put((_data     ) & 0xFF);
	out.put((_data >> 8) & 0xFF);
}

ubyte_t::ubyte_t() : _data(0) {}
ubyte_t::ubyte_t(ubyte_t const & value) : _data(value._data) {}
ubyte_t::ubyte_t(unsigned char value) : _data(value) {}

std::string ubyte_t::encodeBinary()
{
	std::string s(1, 0);

	s[0] = _data;

	return s;
}
void ubyte_t::encodeBinary(std::ostream & out)
{
	out.put(_data & 0xFF);
}

uword_t::uword_t() : _data(0) {}
uword_t::uword_t(uword_t const & value) : _data(value._data) {}
uword_t::uword_t(unsigned short int value) : _data(value) {}

std::string uword_t::encodeBinary()
{
	std::string s(2, 0);

	s[0] = (_data     ) & 0xFF;
	s[1] = (_data >> 8) & 0xFF;

	return s;
}
void uword_t::encodeBinary(std::ostream & out)
{
	out.put((_data     ) & 0xFF);
	out.put((_data >> 8) & 0xFF);
}



int cmp(string8_t const & l, string8_t const & r)
{
	for (size_t index = 0; index < 8; ++index)
	{
		if (l[index] < r[index]) return -1;
		if (l[index] > r[index]) return +1;
	}

	return 0;
}
int cmp(sword_t const & l, sword_t const & r)
{
	if (l._data < r._data) return -1;
	if (l._data > r._data) return +1;

	return 0;
}
int cmp(ubyte_t const & l, ubyte_t const & r)
{
	if (l._data < r._data) return -1;
	if (l._data > r._data) return +1;

	return 0;
}
int cmp(uword_t const & l, uword_t const & r)
{
	if (l._data < r._data) return -1;
	if (l._data > r._data) return +1;

	return 0;
}

std::ostream & operator << (std::ostream & out, string8_t const & in)
{
	for (size_t index = 0; index < 8; ++index)
		out << in[index];

	return out;
}
std::ostream & operator << (std::ostream & out, sword_t const & in)
{
	return out << in._data;
}
std::ostream & operator << (std::ostream & out, ubyte_t const & in)
{
	return out << in._data;
}
std::ostream & operator << (std::ostream & out, uword_t const & in)
{
	return out << in._data;
}



