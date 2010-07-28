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
	2010/05/06 - Original version.
*/

#include <cmath>

#include "real_t.hpp"



void real_t::encodeText(std::ostream & out)
{
	out.precision(256);
	out << _data;
}

float_biggest_t real_t::makeFloat() const
{
	#if USE_GMPLIB
	return _data.get_d();
	#else
	return _data;
	#endif
}

sint_biggest_t real_t::makeInt() const
{
	#if USE_GMPLIB
	return _data.get_si();
	#else
	return _data;
	#endif
}



void real_l_t::encodeText(std::ostream & out)
{
	out.precision(256);
	#if USE_GMPLIB
	out << mpf_class(_data);
	#else
	out << _data;
	#endif
}

float_biggest_t real_l_t::makeFloat() const
{
	#if USE_GMPLIB
	return _data.get_d();
	#else
	return _data;
	#endif
}

sint_biggest_t real_l_t::makeInt() const
{
	#if USE_GMPLIB
	return mpf_class(_data).get_si();
	#else
	return _data;
	#endif
}



real_t abs(real_t const & x)
{
	#if USE_GMPLIB
	return real_t(abs(x._data));
	#else
	if (x._data < 0) return real_t(-x._data);

	return x;
	#endif
}
real_l_t abs(real_l_t const & x)
{
	#if USE_GMPLIB
	return real_l_t(abs(x._data));
	#else
	if (x._data < 0) return real_l_t(-x._data);

	return x;
	#endif
}

int cmp(real_t const & l, real_t const & r)
{
	#if USE_GMPLIB
	return cmp(l._data, r._data);
	#else
	if (l._data > r._data) return +1;
	if (l._data < r._data) return -1;

	return 0;
	#endif
}
int cmp(real_l_t const & l, real_l_t const & r)
{
	#if USE_GMPLIB
	return cmp(l._data, r._data);
	#else
	if (l._data > r._data) return +1;
	if (l._data < r._data) return -1;

	return 0;
	#endif
}

std::ostream & operator << (std::ostream & out, real_t const & in)
{
	return out << in._data;
}
std::ostream & operator << (std::ostream & out, real_l_t const & in)
{
	#if USE_GMPLIB
	return out << mpf_class(in._data);
	#else
	return out << in._data;
	#endif
}

real_l_t sqrt(const real_l_t& x)
{
	#if USE_GMPLIB
	return real_l_t(sqrt(mpf_class(x._data)));

	/*
	real_t epsilon(1, option_precision * 8);

	real_t guess(1);

	while (abs((guess * guess) - x) >= epsilon)
		guess = (x / guess + guess) / 2;

	return guess;
	*/
	#else
	return real_l_t(sqrt(x._data));
	#endif
}

real_t acos(real_t const & x)
{
	#if USE_GMPLIB
	return real_t(acos(x._data.get_d()));
	#else
	return real_t(acos(x._data));
	#endif
}
real_l_t acos(real_l_t const & x)
{
	#if USE_GMPLIB
	return real_l_t(acos(x._data.get_d()));
	#else
	return real_l_t(acos(x._data));
	#endif
}

real_t asin(real_t const & x)
{
	#if USE_GMPLIB
	return real_t(asin(x._data.get_d()));
	#else
	return real_t(asin(x._data));
	#endif
}
real_l_t asin(real_l_t const & x)
{
	#if USE_GMPLIB
	return real_l_t(asin(x._data.get_d()));
	#else
	return real_l_t(asin(x._data));
	#endif
}

real_t atan(real_t const & x)
{
	#if USE_GMPLIB
	return real_t(atan(x._data.get_d()));
	#else
	return real_t(atan(x._data));
	#endif
}
real_l_t atan(real_l_t const & x)
{
	#if USE_GMPLIB
	return real_l_t(atan(x._data.get_d()));
	#else
	return real_l_t(atan(x._data));
	#endif
}

real_t cos(real_t const & x)
{
	#if USE_GMPLIB
	return real_t(cos(x._data.get_d()));
	#else
	return real_t(cos(x._data));
	#endif
}
real_l_t cos(real_l_t const & x)
{
	#if USE_GMPLIB
	return real_l_t(cos(x._data.get_d()));
	#else
	return real_l_t(cos(x._data));
	#endif
}

real_t sin(real_t const & x)
{
	#if USE_GMPLIB
	return real_t(sin(x._data.get_d()));
	#else
	return real_t(sin(x._data));
	#endif
}
real_l_t sin(real_l_t const & x)
{
	#if USE_GMPLIB
	return real_l_t(sin(x._data.get_d()));
	#else
	return real_l_t(sin(x._data));
	#endif
}

real_t tan(real_t const & x)
{
	#if USE_GMPLIB
	return real_t(tan(x._data.get_d()));
	#else
	return real_t(tan(x._data));
	#endif
}
real_l_t tan(real_l_t const & x)
{
	#if USE_GMPLIB
	return real_l_t(tan(x._data.get_d()));
	#else
	return real_l_t(tan(x._data));
	#endif
}



