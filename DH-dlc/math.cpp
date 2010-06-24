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
*/

#include "math.hpp"

#include <climits>
#include <cmath>
#include <cstdlib>
#include <fstream>

#include "options.hpp"
#include "types.hpp"
#include "types/int_t.hpp"
#include "types/real_t.hpp"
#include "types/string_t.hpp"



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



real_s_t random_real_s()
{
	return to_real_s(random_real());
}
real_s_t random_real_s(const real_s_t& max)
{
	return random_real_s() * max;
}
real_s_t random_real_s(const real_s_t& min, const real_s_t& max)
{
	return (random_real_s() * (max-min)) + min;
}

real_t random_real()
{
	#if USE_GMPLIB
	static gmp_randclass random_source(gmp_randinit_default);
	static unsigned long random_seed(0UL);

	if (random_seed == 0UL)
	{
		std::ifstream ifs("/dev/urandom");

		if (ifs)
		{
			for (size_t index = sizeof(random_seed); index; --index)
			{
				random_seed <<= CHAR_BIT;
				random_seed += (unsigned long)ifs.get();
			}
		}
		else
			random_seed = time(NULL);

		ifs.close();

		random_source.seed(random_seed);
	}

	return real_t(random_source.get_f(option_precision));
	#else
	return to_real(random_int_l(int_l_t(SINT_BIGGEST_MAX))) / real_t(SINT_BIGGEST_MAX);
	#endif
}
real_t random_real(const real_t& max)
{
	return random_real() * max;
}
real_t random_real(const real_t& min, const real_t& max)
{
	return (random_real() * (max-min)) + min;
}

real_l_t random_real_l()
{
	return to_real_l(random_real());
}
real_l_t random_real_l(const real_l_t& max)
{
	return random_real_l() * max;
}
real_l_t random_real_l(const real_l_t& min, const real_l_t& max)
{
	return (random_real_l() * (max-min)) + min;
}

int_s_t random_int_s(const int_s_t& max)
{
	return to_int_s(random_int_l(to_int_l(max)));
}
int_s_t random_int_s(const int_s_t& min, const int_s_t& max)
{
	return to_int_s(random_int_l(to_int_l(min), to_int_l(max)));
}

int_t random_int(const int_t& max)
{
	return to_int(random_int_l(to_int_l(max)));
}
int_t random_int(const int_t& min, const int_t& max)
{
	return to_int(random_int_l(to_int_l(min), to_int_l(max)));
}

int_l_t random_int_l(const int_l_t& max)
{
	#if USE_GMPLIB
	static gmp_randclass random_source(gmp_randinit_default);
	static unsigned long random_seed(0UL);

	if (random_seed == 0UL)
	{
		std::ifstream ifs("/dev/urandom");

		if (ifs)
		{
			for (size_t index = sizeof(random_seed); index; --index)
			{
				random_seed <<= CHAR_BIT;
				random_seed += (unsigned long)ifs.get();
			}
		}
		else
			random_seed = time(NULL);

		ifs.close();

		random_source.seed(random_seed);
	}

	return int_l_t(random_source.get_z_range(max._data + 1));
	#else
	static unsigned int random_seed(0U);

	if (random_seed == 0U)
	{
		while (random_seed == 0U)
		{
			std::ifstream ifs("/dev/urandom");

			if (ifs)
			{
				for (size_t index = sizeof(random_seed); index; --index)
				{
					random_seed <<= CHAR_BIT;
					random_seed += (unsigned int)ifs.get();
				}

				ifs.close();

				break;
			}

			ifs.close();
			ifs.clear();
			ifs.open("/dev/random");

			if (ifs)
			{
				for (size_t index = sizeof(random_seed); index; --index)
				{
					random_seed <<= CHAR_BIT;
					random_seed += (unsigned int)ifs.get();
				}

				ifs.close();

				break;
			}

			ifs.close();

			random_seed = time(NULL);
		}

		srand(random_seed);
	}

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

	return int_l_t(random_sink);

	#endif
}
int_l_t random_int_l(const int_l_t& min, const int_l_t& max)
{
	return random_int_l(max-min) + min;
}



