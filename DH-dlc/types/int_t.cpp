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
	2010/05/05 - Original version.
*/

#include "int_t.hpp"

#include "../options.hpp"

#if USE_GMPLIB
mpz_class int_t:: int_t_max = 0;
mpz_class int_t:: int_t_min = 0;
mpz_class int_t::uint_t_max = 0;
#else
sint_biggest_t int_t:: int_t_max = 0;
sint_biggest_t int_t:: int_t_min = 0;
uint_biggest_t int_t::uint_t_max = 0;
#endif

void int_t::cap_data()
{
	if (_data > int_t_MAX || _data < int_t_MIN)
	{
		#if USE_GMPLIB
		int msb = mpz_tstbit(_data.get_mpz_t(), option_precision-1);

		_data &= int_t_MAX;

		if (msb) _data += int_t_MIN;
		#else
		int msb = (_data >> option_precision) & 1;

		_data &= int_t_MAX;

		if (msb) _data += int_t_MIN;
		#endif
	}
}

float_biggest_t int_t::makeFloat() const
{
	#if USE_GMPLIB
	return _data.get_d();
	#else
	return _data;
	#endif
}

sint_biggest_t int_t::makeInt() const
{
	#if USE_GMPLIB
	return _data.get_si();
	#else
	return _data;
	#endif
}



void int_t::resetLimits()
{
	uint_t_max = 0;

	// The algorithm looks funny because it has to be able to handle non-GMP
	// numbers at their limit. Like a precision of 32 with 32 bit ints.
	for (int i = option_precision - 1; i; --i)
	{
		++uint_t_max;
		uint_t_max <<= 1U;
	}

	++uint_t_max;

	int_t_max = uint_t_max >> 1;
	int_t_min = -int_t_max - 1;
}



float_biggest_t int_l_t::makeFloat() const
{
	#if USE_GMPLIB
	return _data.get_d();
	#else
	return _data;
	#endif
}

sint_biggest_t int_l_t::makeInt() const
{
	#if USE_GMPLIB
	return _data.get_si();
	#else
	return _data;
	#endif
}



int_t abs(int_t const & value)
{
	#if USE_GMPLIB
	return int_t(abs(value._data));
	#else
	if (value._data < 0) return int_t(-value._data);

	return value;
	#endif
}
int_l_t abs(int_l_t const & value)
{
	#if USE_GMPLIB
	return int_l_t(abs(value._data));
	#else
	if (value._data < 0) return int_l_t(-value._data);

	return value;
	#endif
}

int cmp(int_t const & l, int_t const & r)
{
	#if USE_GMPLIB
	return cmp(l._data, r._data);
	#else
	if (l._data > r._data) return +1;
	if (l._data < r._data) return -1;

	return 0;
	#endif
}
int cmp(int_l_t const & l, int_l_t const & r)
{
	#if USE_GMPLIB
	return cmp(l._data, r._data);
	#else
	if (l._data > r._data) return +1;
	if (l._data < r._data) return -1;

	return 0;
	#endif
}



