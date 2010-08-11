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
	2010/02/14 - get_z_range is 0..max-1, not 0..max. Oops.
	2010/02/14 - Made generating the seed independant of the size of long.
	2010/06/24 - Added binary type RNGs.
*/

#include "math.hpp"

#include <climits>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "options.hpp"
#include "types.hpp"
#include "types/binary.hpp"
#include "types/int_t.hpp"
#include "types/real_t.hpp"
#include "types/string_t.hpp"



#ifdef USE_GMPLIB
gmp_randclass random_source(gmp_randinit_default);
#endif



real_t pi_make()
{
	// Gauss-Legrende algorithm

	real_t a0(1);
	real_t b0(real_t(1) / sqrt(real_t(2)));
	real_t t0(0.25);
	real_t p0(1);

	real_t a1, b1, t1, p1;

	// index is halved each time because the precision doubles per iteration.
	for (int index = option_precision + 2; index > 0; index /= 2)
	{
		a1 = (a0 + b0) / real_t(2);
		b1 = sqrt(real_t(a0 * b0));
		t1 = t0 - (p0 * (a0-a1) * (a0-a1));
		p1 = p0 * real_t(2);

		a0 = a1;
		b0 = b1;
		t0 = t1;
		p0 = p1;
	}

	return ((a0 + b0) * (a0 + b0)) / (t0 * real_t(4));
}
const real_t& pi()
{
	static real_t current_pi        = pi_make();
	static int    current_precision = option_precision;

	if (current_precision != option_precision)
		current_pi = pi_make();

	return current_pi;
}



template<typename T> inline T random__int(T const & max)
{
	sint_biggest_t random_sink = 0;

	for (size_t i = sizeof(sint_biggest_t); i; --i)
	{
		random_sink <<= CHAR_BIT;
		random_sink += rand();
	}

	// If this number is negative, it causes problems.
	random_sink &= SINT_BIGGEST_MAX;

	// +1 because this function produces 0..max inclusive.
	random_sink %= max.makeInt() + 1;

	return T(random_sink);
}
template<> inline int_s_t random__int(int_s_t const & max)
{
	int_s_t random_sink = 0;

	for (size_t i = sizeof(int_s_t); i; --i)
	{
		random_sink <<= CHAR_BIT;
		random_sink += rand();
	}

	// If this number is negative, it causes problems.
	random_sink &= int_s_t_MAX;

	// +1 because this function produces 0..max inclusive.
	random_sink %= max + 1;

	return int_s_t(random_sink);
}

template<> real_s_t random<real_s_t>()
{
	return real_s_t(rand()) / real_s_t(RAND_MAX);
}
template<> real_s_t random<real_s_t>(real_s_t const & max)
{
	return random<real_s_t>() * max;
}
template<> real_s_t random<real_s_t>(real_s_t const & min, real_s_t const & max)
{
	return (random<real_s_t>() * (max-min)) + min;
}

template<> real_t random<real_t>()
{
	#if USE_GMPLIB
	return real_t(random_source.get_f(option_precision));
	#else
	return real_t(rand()) / real_t(RAND_MAX);
	#endif
}
template<> real_t random<real_t>(real_t const & max)
{
	return random<real_t>() * max;
}
template<> real_t random<real_t>(real_t const & min, real_t const & max)
{
	return (random<real_t>() * (max-min)) + min;
}

template<> real_l_t random<real_l_t>()
{
	#if USE_GMPLIB
	return real_l_t(random_source.get_f(option_precision));
	#else
	return real_l_t(rand()) / real_l_t(RAND_MAX);
	#endif
}
template<> real_l_t random<real_l_t>(real_l_t const & max)
{
	return random<real_l_t>() * max;
}
template<> real_l_t random<real_l_t>(real_l_t const & min, real_l_t const & max)
{
	return (random<real_l_t>() * (max-min)) + min;
}

template<> int_s_t random<int_s_t>(int_s_t const & max)
{
	return random__int<int_s_t>(max);
}
template<> int_s_t random<int_s_t>(int_s_t const & min, int_s_t const & max)
{
	return random<int_s_t>(max-min) + min;
}

template<> int_t random<int_t>(int_t const & max)
{
	#if USE_GMPLIB
	return int_t(random_source.get_z_range(max._data + 1));
	#else
	return random__int<int_t>(max);
	#endif
}
template<> int_t random<int_t>(int_t const & min, int_t const & max)
{
	return random<int_t>(max-min) + min;
}

template<> int_l_t random<int_l_t>(int_l_t const & max)
{
	#if USE_GMPLIB
	return int_l_t(random_source.get_z_range(max._data + 1));
	#else
	return random__int<int_l_t>(max);
	#endif
}
template<> int_l_t random<int_l_t>(int_l_t const & min, int_l_t const & max)
{
	return random<int_l_t>(max-min) + min;
}

template<> ubyte_t random<ubyte_t>(ubyte_t const & max)
{
	return random__int<ubyte_t>(max);
}
template<> ubyte_t random<ubyte_t>(ubyte_t const & min, ubyte_t const & max)
{
	return random<ubyte_t>(max-min) + min;
}

template<> sword_t random<sword_t>(sword_t const & max)
{
	return random__int<sword_t>(max);
}
template<> sword_t random<sword_t>(sword_t const & min, sword_t const & max)
{
	return random<sword_t>(max-min) + min;
}

template<> uword_t random<uword_t>(uword_t const & max)
{
	return random__int<uword_t>(max);
}
template<> uword_t random<uword_t>(uword_t const & min, uword_t const & max)
{
	return random<uword_t>(max-min) + min;
}

template<> sdword_t random<sdword_t>(sdword_t const & max)
{
	return random__int<sdword_t>(max);
}
template<> sdword_t random<sdword_t>(sdword_t const & min, sdword_t const & max)
{
	return random<sdword_t>(max-min) + min;
}

template<> udword_t random<udword_t>(udword_t const & max)
{
	return random__int<udword_t>(max);
}
template<> udword_t random<udword_t>(udword_t const & min, udword_t const & max)
{
	return random<udword_t>(max-min) + min;
}



