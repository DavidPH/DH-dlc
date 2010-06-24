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
	2010/05/04 - Original version.
*/

#include "string_t.hpp"



string_t::string_t()                         : _data()           {}
string_t::string_t(string_t const & data)    : _data(data._data) {}
string_t::string_t(char const * data)        : _data(data)       {}
string_t::string_t(std::string const & data) : _data(data)       {}

string_t::~string_t() {}



void string_t::clear()
{
	_data.clear();
}

bool string_t::empty() const
{
	return _data.empty();
}

std::string const & string_t::makeString() const
{
	return _data;
}

char & string_t::operator [] (size_t index)
{
	return _data[index];
}
char const & string_t::operator [] (size_t index) const
{
	return _data[index];
}

string_t & string_t::operator += (string_t const & other)
{
	_data += other._data;

	return *this;
}

size_t string_t::size() const
{
	return _data.size();
}



int cmp(string_t const & l, string_t const & r)
{
	for (size_t index = 0; index < l.size() && index < r.size(); ++index)
		if (l[index] != r[index])
			return l[index] - r[index];

	if (l.size() != r.size())
		return l.size() - r.size();

	return 0;
}

string_t operator + (string_t const & l, string_t const & r)
{
	return string_t(l) += r;
}



