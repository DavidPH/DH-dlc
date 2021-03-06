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
	2010/06/24 - Added binary type RNGs.
*/

#ifndef LO_MATH_H
#define LO_MATH_H

#include "types.hpp"

#include <cmath>
#if USE_GMPLIB
#include <gmpxx.h>
#endif



#if USE_GMPLIB
extern gmp_randclass random_source;
#endif



template<typename T>
T clamp(T value, T const & min, T const & max);

int cmp(bool_t, bool_t);
int cmp(int_s_t, int_s_t);
int cmp(real_s_t, real_s_t);
int cmp(const std::string&, const std::string&);

int isoperator(int);

const real_t& pi();

template<typename T> T random();
template<typename T> T random(T const &);
template<typename T> T random(T const &, T const &);

template<typename T> T round(T const &);

real_l_t sqrt(const real_l_t&);



template<typename T>
inline T clamp(T value, T const & min, T const & max)
{
	while (value <  min) value += max;
	while (value >= max) value -= max;

	return value;
}

inline int cmp(bool_t val1, bool_t val2)
{
	return val1 - val2;
}
inline int cmp(int_s_t l, int_s_t r)
{
	if (l > r) return +1;
	if (l < r) return -1;

	return 0;
}
inline int cmp(real_s_t l, real_s_t r)
{
	if (l > r) return +1;
	if (l < r) return -1;

	return 0;
}
inline int cmp(const std::string& val1, const std::string& val2)
{
	for (size_t index = 0; index < val1.size() && index < val2.size(); ++index)
		if (val1[index] != val2[index])
			return val1[index] - val2[index];

	if (val1.size() != val2.size())
		return val1.size() - val2.size();

	return 0;
}

inline int isoperator(int c)
{
	switch (c)
	{
		case '*':
		case '/':
		case '%':
		case '+':
		case '-':
		case '&':
		case '|':
			return true;

		default:
			return false;
	}
}

template<typename T> T round(T const & value)
{
	return floor(value + T(0.5));
}



#endif /* LO_MATH_H */



