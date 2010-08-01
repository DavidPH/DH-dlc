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



template <size_t S>
stringf_t<S>::stringf_t()
{
	clear();
}
template <size_t S>
stringf_t<S>::stringf_t(stringf_t<S> const & value)
{
	for (size_t index = 0; index < S; ++index)
		_data[index] = value._data[index];
}
template <size_t S>
stringf_t<S>::stringf_t(char const * value)
{
	// In case value is less than eight characters.
	clear();

	for (size_t index = 0; value[index] && index < S; ++index)
		_data[index] = value[index];
}
template <size_t S>
stringf_t<S>::stringf_t(std::string const & value)
{
	// In case value is less than eight characters.
	clear();

	for (size_t index = 0; index < value.size() && index < S; ++index)
		_data[index] = value[index];
}

template <size_t S>
void stringf_t<S>::clear()
{
	for (size_t index = 0; index < S; ++index)
		_data[index] = 0;
}

template <size_t S>
void stringf_t<S>::encodeBinary(std::ostream & out)
{
	for (size_t index = 0; index < S; ++index)
		out.put(_data[index]);
}
template <size_t S>
void stringf_t<S>::encodeText(std::ostream & out)
{
	out.put('"');

	for (size_t index = 0; index < S; ++index)
	{
		int c = _data[index];

		switch (c)
		{
		case '\\':
		case '"':
			out.put('\\');
			out.put(c);
			break;

		default:
			out.put(c);
			break;
		}
	}

	out.put('"');
}

template <size_t S>
stringf_t<S> & stringf_t<S>::operator += (stringf_t<S> const & value)
{
	size_t index = 0;

	for (; _data[index] && index < S; ++index);

	for (size_t index2 = 0; index < S; ++index, ++index2)
		_data[index] = value._data[index2];

	return *this;
}

template <size_t S>
stringf_t<S> & stringf_t<S>::operator = (stringf_t<S> const & value)
{
	for (size_t index = 0; index < S; ++index)
		_data[index] = value._data[index];

	return *this;
}

template class stringf_t<  8U>;
template class stringf_t< 16U>;
template class stringf_t< 32U>;
template class stringf_t< 80U>;
template class stringf_t<320U>;

ubyte_t::ubyte_t() : _data(0) {}
ubyte_t::ubyte_t(ubyte_t const & value) : _data(value._data) {}
ubyte_t::ubyte_t(unsigned char value) : _data(value) {}

void ubyte_t::encodeBinary(std::ostream & out)
{
	out.put(_data & 0xFF);
}
void ubyte_t::encodeText(std::ostream & out)
{
	out << _data;
}

sword_t::sword_t() : _data(0) {}
sword_t::sword_t(sword_t const & value) : _data(value._data) {}
sword_t::sword_t(signed short int value) : _data(value) {}

void sword_t::encodeBinary(std::ostream & out)
{
	out.put((_data     ) & 0xFF);
	out.put((_data >> 8) & 0xFF);
}
void sword_t::encodeText(std::ostream & out)
{
	out << _data;
}

uword_t::uword_t() : _data(0) {}
uword_t::uword_t(uword_t const & value) : _data(value._data) {}
uword_t::uword_t(unsigned short int value) : _data(value) {}

void uword_t::encodeBinary(std::ostream & out)
{
	out.put((_data     ) & 0xFF);
	out.put((_data >> 8) & 0xFF);
}
void uword_t::encodeText(std::ostream & out)
{
	out << _data;
}



template <size_t S>
int cmp(stringf_t<S> const & l, stringf_t<S> const & r)
{
	for (size_t index = 0; index < S; ++index)
	{
		if (l[index] < r[index]) return -1;
		if (l[index] > r[index]) return +1;
	}

	return 0;
}
template int cmp<  8U>(stringf_t<  8U> const & l, stringf_t<  8U> const & r);
template int cmp< 16U>(stringf_t< 16U> const & l, stringf_t< 16U> const & r);
template int cmp< 32U>(stringf_t< 32U> const & l, stringf_t< 32U> const & r);
template int cmp< 80U>(stringf_t< 80U> const & l, stringf_t< 80U> const & r);
template int cmp<320U>(stringf_t<320U> const & l, stringf_t<320U> const & r);
int cmp(ubyte_t const & l, ubyte_t const & r)
{
	if (l._data < r._data) return -1;
	if (l._data > r._data) return +1;

	return 0;
}
int cmp(sword_t const & l, sword_t const & r)
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

template <size_t S>
std::ostream & operator << (std::ostream & out, stringf_t<S> const & in)
{
	for (size_t index = 0; index < S; ++index)
		out << in[index];

	return out;
}
template std::ostream & operator << <  8U> (std::ostream & out, stringf_t<  8U> const & r);
template std::ostream & operator << < 16U> (std::ostream & out, stringf_t< 16U> const & r);
template std::ostream & operator << < 32U> (std::ostream & out, stringf_t< 32U> const & r);
template std::ostream & operator << < 80U> (std::ostream & out, stringf_t< 80U> const & r);
template std::ostream & operator << <320U> (std::ostream & out, stringf_t<320U> const & r);
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



