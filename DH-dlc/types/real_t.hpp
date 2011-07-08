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

#ifndef REAL_T_H
#define REAL_T_H

#include <ostream>

#include "../types.hpp"

#if USE_GMPLIB
#include <gmpxx.h>
#else
#include <cmath>
#endif



typedef long long int int_s_t;
class int_t;
class int_l_t;
typedef double real_s_t;
class real_t;
class real_l_t;



class real_t
{
	public:
		explicit real_t() : _data(0) {}
		         real_t(real_t      const & v) : _data(v._data) {}
		#if USE_GMPLIB
		explicit real_t(mpf_class   const & v) : _data(v) {}
		#else
		explicit real_t(long double const & v) : _data(v) {}
		#endif

		void encodeText(std::ostream & out);

		long double makeFloat() const;
		long long int makeInt() const;

		real_t & operator *= (real_t const & v) {_data *= v._data; return *this;}
		real_t & operator /= (real_t const & v) {_data /= v._data; return *this;}
		real_t & operator += (real_t const & v) {_data += v._data; return *this;}
		real_t & operator -= (real_t const & v) {_data -= v._data; return *this;}
		real_t & operator  = (real_t const & v) {_data  = v._data; return *this;}


		friend real_t abs(real_t const &);

		friend int cmp(real_t const &, real_t const &);

		friend real_t floor(real_t const &);

		friend std::ostream& operator << (std::ostream&, const real_t&);

		friend real_t sqrt(real_t const &);

		friend int_t    convert<int_t,    real_t>(real_t const &);
		friend int_l_t  convert<int_l_t,  real_t>(real_t const &);
		friend real_l_t convert<real_l_t, real_t>(real_t const &);

		friend real_t acos(real_t const &);
		friend real_t asin(real_t const &);
		friend real_t atan(real_t const &);
		friend real_t  cos(real_t const &);
		friend real_t  sin(real_t const &);
		friend real_t  tan(real_t const &);

	private:
		#if USE_GMPLIB
		mpf_class _data;
		#else
		long double _data;
		#endif
};

class real_l_t
{
	public:
		explicit real_l_t() : _data(0) {}
		         real_l_t(real_l_t    const & v) : _data(v._data) {}
		#if USE_GMPLIB
		explicit real_l_t(mpq_class   const & v) : _data(v) {}
		#else
		explicit real_l_t(long double const & v) : _data(v) {}
		#endif

		void encodeText(std::ostream & out);

		long double makeFloat() const;
		long long int makeInt() const;

		real_l_t & operator *= (real_l_t const & v) {_data *= v._data; return *this;}
		real_l_t & operator /= (real_l_t const & v) {_data /= v._data; return *this;}
		real_l_t & operator += (real_l_t const & v) {_data += v._data; return *this;}
		real_l_t & operator -= (real_l_t const & v) {_data -= v._data; return *this;}
		real_l_t & operator  = (real_l_t const & v) {_data  = v._data; return *this;}


		friend real_l_t abs(real_l_t const &);

		friend int cmp(real_l_t const &, real_l_t const &);

		friend real_l_t floor(real_l_t const &);

		friend std::ostream& operator << (std::ostream&, const real_l_t&);

		friend real_l_t sqrt(real_l_t const &);

		friend int_t   convert<int_t,   real_l_t>(real_l_t const &);
		friend int_l_t convert<int_l_t, real_l_t>(real_l_t const &);
		friend real_t  convert<real_t,  real_l_t>(real_l_t const &);

		friend real_l_t acos(real_l_t const &);
		friend real_l_t asin(real_l_t const &);
		friend real_l_t atan(real_l_t const &);
		friend real_l_t  cos(real_l_t const &);
		friend real_l_t  sin(real_l_t const &);
		friend real_l_t  tan(real_l_t const &);

	private:
		#if USE_GMPLIB
		mpq_class _data;
		#else
		long double _data;
		#endif
};



real_t   abs(real_t   const &);
real_l_t abs(real_l_t const &);

int cmp(real_t   const &, real_t   const &);
int cmp(real_l_t const &, real_l_t const &);

real_t   floor(real_t   const &);
real_l_t floor(real_l_t const &);

std::ostream & operator << (std::ostream &, real_t   const &);
std::ostream & operator << (std::ostream &, real_l_t const &);

real_t   hypot(real_t   const &, real_t   const &);
real_l_t hypot(real_l_t const &, real_l_t const &);

bool operator == (real_t   const &, real_t   const &);
bool operator == (real_l_t const &, real_l_t const &);
bool operator != (real_t   const &, real_t   const &);
bool operator != (real_l_t const &, real_l_t const &);
bool operator >= (real_t   const &, real_t   const &);
bool operator >= (real_l_t const &, real_l_t const &);
bool operator >  (real_t   const &, real_t   const &);
bool operator >  (real_l_t const &, real_l_t const &);
bool operator <= (real_t   const &, real_t   const &);
bool operator <= (real_l_t const &, real_l_t const &);
bool operator <  (real_t   const &, real_t   const &);
bool operator <  (real_l_t const &, real_l_t const &);

real_t   operator * (real_t   const &, real_t   const &);
real_l_t operator * (real_l_t const &, real_l_t const &);
real_t   operator / (real_t   const &, real_t   const &);
real_l_t operator / (real_l_t const &, real_l_t const &);
real_t   operator + (real_t   const &, real_t   const &);
real_l_t operator + (real_l_t const &, real_l_t const &);
real_t   operator - (real_t   const &, real_t   const &);
real_l_t operator - (real_l_t const &, real_l_t const &);

real_t   sqrt(real_t   const &);
real_l_t sqrt(real_l_t const &);

real_t   acos(real_t   const &);
real_l_t acos(real_l_t const &);
real_t   asin(real_t   const &);
real_l_t asin(real_l_t const &);
real_t   atan(real_t   const &);
real_l_t atan(real_l_t const &);
real_t    cos(real_t   const &);
real_l_t  cos(real_l_t const &);
real_t    sin(real_t   const &);
real_l_t  sin(real_l_t const &);
real_t    tan(real_t   const &);
real_l_t  tan(real_l_t const &);



inline real_t   floor(real_t   const & x) {return real_t  (floor(x._data));}

inline real_t   hypot(real_t   const & x, real_t   const & y) {return sqrt((x*x) + (y*y));}
inline real_l_t hypot(real_l_t const & x, real_l_t const & y) {return sqrt((x*x) + (y*y));}

inline bool operator == (real_t   const & l, real_t   const & r) {return cmp(l, r) == 0;}
inline bool operator == (real_l_t const & l, real_l_t const & r) {return cmp(l, r) == 0;}
inline bool operator != (real_t   const & l, real_t   const & r) {return cmp(l, r) != 0;}
inline bool operator != (real_l_t const & l, real_l_t const & r) {return cmp(l, r) != 0;}
inline bool operator >= (real_t   const & l, real_t   const & r) {return cmp(l, r) >= 0;}
inline bool operator >= (real_l_t const & l, real_l_t const & r) {return cmp(l, r) >= 0;}
inline bool operator >  (real_t   const & l, real_t   const & r) {return cmp(l, r) >  0;}
inline bool operator >  (real_l_t const & l, real_l_t const & r) {return cmp(l, r) >  0;}
inline bool operator <= (real_t   const & l, real_t   const & r) {return cmp(l, r) <= 0;}
inline bool operator <= (real_l_t const & l, real_l_t const & r) {return cmp(l, r) <= 0;}
inline bool operator <  (real_t   const & l, real_t   const & r) {return cmp(l, r) <  0;}
inline bool operator <  (real_l_t const & l, real_l_t const & r) {return cmp(l, r) <  0;}

inline real_t   operator * (real_t   const & l, real_t   const & r) {return real_t  (l) *= r;}
inline real_l_t operator * (real_l_t const & l, real_l_t const & r) {return real_l_t(l) *= r;}
inline real_t   operator / (real_t   const & l, real_t   const & r) {return real_t  (l) /= r;}
inline real_l_t operator / (real_l_t const & l, real_l_t const & r) {return real_l_t(l) /= r;}
inline real_t   operator + (real_t   const & l, real_t   const & r) {return real_t  (l) += r;}
inline real_l_t operator + (real_l_t const & l, real_l_t const & r) {return real_l_t(l) += r;}
inline real_t   operator - (real_t   const & l, real_t   const & r) {return real_t  (l) -= r;}
inline real_l_t operator - (real_l_t const & l, real_l_t const & r) {return real_l_t(l) -= r;}

inline real_t sqrt(real_t const & x) {return real_t(sqrt(x._data));}



#endif /* REAL_T_H */



