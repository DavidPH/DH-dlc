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

#ifndef INT_T_H
#define INT_T_H

#include "types_limits.hpp"

#include "../math.hpp"
#include "../types.hpp"

#include <ostream>
#if USE_GMPLIB
#include <gmpxx.h>
#else
#include <cmath>
#endif



typedef ptrdiff_t int_s_t;
class int_t;
class int_l_t;
typedef double real_s_t;
class real_t;
class real_l_t;



class int_t
{
	public:
		explicit int_t() : _data(0) {}
		         int_t(int_t          const & v) : _data(v._data) {cap_data();}
		#if USE_GMPLIB
		explicit int_t(mpz_class      const & v) : _data(v) {cap_data();}
		#else
		explicit int_t(sint_biggest_t const & v) : _data(v) {cap_data();}
		#endif

		void encodeText(std::ostream & out);

		float_biggest_t makeFloat() const;
		sint_biggest_t  makeInt()  const;

		int_t& operator &= (const int_t& v) {_data &= v._data; cap_data(); return *this;}
		int_t& operator |= (const int_t& v) {_data |= v._data; cap_data(); return *this;}
		int_t& operator *= (const int_t& v) {_data *= v._data; cap_data(); return *this;}
		int_t& operator /= (const int_t& v) {_data /= v._data; cap_data(); return *this;}
		int_t& operator %= (const int_t& v) {_data %= v._data; cap_data(); return *this;}
		int_t& operator += (const int_t& v) {_data += v._data; cap_data(); return *this;}
		int_t& operator -= (const int_t& v) {_data -= v._data; cap_data(); return *this;}
		int_t& operator  = (const int_t& v) {_data  = v._data; cap_data(); return *this;}



		friend int_t abs(int_t const &);

		friend int cmp(int_t const &, int_t const &);

		friend std::ostream& operator << (std::ostream&, const int_t&);

		friend int_t random<int_t>(int_t const &);

		friend int_t sqrt(int_t const &);

		friend int_l_t  convert<int_l_t,  int_t>(int_t const &);
		friend real_t   convert<real_t,   int_t>(int_t const &);
		friend real_l_t convert<real_l_t, int_t>(int_t const &);



		static void resetLimits();

		#if USE_GMPLIB
		static mpz_class  int_t_max;
		static mpz_class  int_t_min;
		static mpz_class uint_t_max;
		#else
		static sint_biggest_t  int_t_max;
		static sint_biggest_t  int_t_min;
		static uint_biggest_t uint_t_max;
		#endif

	private:
		void cap_data();

		#if USE_GMPLIB
		mpz_class _data;
		#else
		sint_biggest_t _data;
		#endif
};

class int_l_t
{
	public:
		explicit int_l_t() : _data(0) {}
		         int_l_t(int_l_t        const & v) : _data(v._data) {}
		#if USE_GMPLIB
		explicit int_l_t(mpz_class      const & v) : _data(v) {}
		#else
		explicit int_l_t(sint_biggest_t const & v) : _data(v) {}
		#endif

		void encodeText(std::ostream & out);

		float_biggest_t makeFloat() const;
		sint_biggest_t  makeInt()  const;

		int_l_t& operator &= (const int_l_t& v) {_data &= v._data; return *this;}
		int_l_t& operator |= (const int_l_t& v) {_data |= v._data; return *this;}
		int_l_t& operator *= (const int_l_t& v) {_data *= v._data; return *this;}
		int_l_t& operator /= (const int_l_t& v) {_data /= v._data; return *this;}
		int_l_t& operator %= (const int_l_t& v) {_data %= v._data; return *this;}
		int_l_t& operator += (const int_l_t& v) {_data += v._data; return *this;}
		int_l_t& operator -= (const int_l_t& v) {_data -= v._data; return *this;}
		int_l_t& operator  = (const int_l_t& v) {_data  = v._data; return *this;}


		friend int_l_t abs(int_l_t const &);

		friend int cmp(int_l_t const &, int_l_t const &);

		friend std::ostream& operator << (std::ostream&, const int_l_t&);

		friend int_l_t random<int_l_t>(int_l_t const &);

		friend int_l_t sqrt(int_l_t const &);

		friend int_t    convert<int_t,    int_l_t>(int_l_t const &);
		friend real_t   convert<real_t,   int_l_t>(int_l_t const &);
		friend real_l_t convert<real_l_t, int_l_t>(int_l_t const &);

	private:
		#if USE_GMPLIB
		mpz_class _data;
		#else
		sint_biggest_t _data;
		#endif
};



int_t   abs(int_t   const &);
int_l_t abs(int_l_t const &);

int cmp(int_t   const &, int_t   const &);
int cmp(int_l_t const &, int_l_t const &);

int_t   hypot(int_t   const &, int_t   const &);
int_l_t hypot(int_l_t const &, int_l_t const &);

bool operator == (int_t   const &, int_t   const &);
bool operator == (int_l_t const &, int_l_t const &);
bool operator != (int_t   const &, int_t   const &);
bool operator != (int_l_t const &, int_l_t const &);
bool operator >= (int_t   const &, int_t   const &);
bool operator >= (int_l_t const &, int_l_t const &);
bool operator >  (int_t   const &, int_t   const &);
bool operator >  (int_l_t const &, int_l_t const &);
bool operator <= (int_t   const &, int_t   const &);
bool operator <= (int_l_t const &, int_l_t const &);
bool operator <  (int_t   const &, int_t   const &);
bool operator <  (int_l_t const &, int_l_t const &);

int_t   operator & (int_t   const &, int_t   const &);
int_l_t operator & (int_l_t const &, int_l_t const &);
int_t   operator | (int_t   const &, int_t   const &);
int_l_t operator | (int_l_t const &, int_l_t const &);
int_t   operator * (int_t   const &, int_t   const &);
int_l_t operator * (int_l_t const &, int_l_t const &);
int_t   operator / (int_t   const &, int_t   const &);
int_l_t operator / (int_l_t const &, int_l_t const &);
int_t   operator % (int_t   const &, int_t   const &);
int_l_t operator % (int_l_t const &, int_l_t const &);
int_t   operator + (int_t   const &, int_t   const &);
int_l_t operator + (int_l_t const &, int_l_t const &);
int_t   operator - (int_t   const &, int_t   const &);
int_l_t operator - (int_l_t const &, int_l_t const &);

std::ostream& operator << (std::ostream&, int_t   const &);
std::ostream& operator << (std::ostream&, int_l_t const &);

int_t   sqrt(int_t   const &);
int_l_t sqrt(int_l_t const &);



inline int_t hypot(int_t const & x, int_t const & y)
{
	return sqrt((x * x) + (y * y));
}
inline int_l_t hypot(int_l_t const & x, int_l_t const & y)
{
	return sqrt((x * x) + (y * y));
}

inline std::ostream& operator << (std::ostream& out, int_t const & in)
{
	return out << in._data;
}
inline std::ostream& operator << (std::ostream& out, int_l_t const & in)
{
	return out << in._data;
}

inline int_t sqrt(int_t const & value)
{
	return int_t(sqrt(value._data));
}
inline int_l_t sqrt(int_l_t const & value)
{
	return int_l_t(sqrt(value._data));
}



inline bool operator == (int_t   const & l, int_t   const & r) {return cmp(l, r) == 0;}
inline bool operator == (int_l_t const & l, int_l_t const & r) {return cmp(l, r) == 0;}
inline bool operator != (int_t   const & l, int_t   const & r) {return cmp(l, r) != 0;}
inline bool operator != (int_l_t const & l, int_l_t const & r) {return cmp(l, r) != 0;}
inline bool operator >= (int_t   const & l, int_t   const & r) {return cmp(l, r) >= 0;}
inline bool operator >= (int_l_t const & l, int_l_t const & r) {return cmp(l, r) >= 0;}
inline bool operator >  (int_t   const & l, int_t   const & r) {return cmp(l, r) >  0;}
inline bool operator >  (int_l_t const & l, int_l_t const & r) {return cmp(l, r) >  0;}
inline bool operator <= (int_t   const & l, int_t   const & r) {return cmp(l, r) <= 0;}
inline bool operator <= (int_l_t const & l, int_l_t const & r) {return cmp(l, r) <= 0;}
inline bool operator <  (int_t   const & l, int_t   const & r) {return cmp(l, r) <  0;}
inline bool operator <  (int_l_t const & l, int_l_t const & r) {return cmp(l, r) <  0;}

inline int_t   operator & (int_t   const & l, int_t   const & r) {return int_t  (l) &= r;}
inline int_l_t operator & (int_l_t const & l, int_l_t const & r) {return int_l_t(l) &= r;}
inline int_t   operator | (int_t   const & l, int_t   const & r) {return int_t  (l) |= r;}
inline int_l_t operator | (int_l_t const & l, int_l_t const & r) {return int_l_t(l) |= r;}
inline int_t   operator * (int_t   const & l, int_t   const & r) {return int_t  (l) *= r;}
inline int_l_t operator * (int_l_t const & l, int_l_t const & r) {return int_l_t(l) *= r;}
inline int_t   operator / (int_t   const & l, int_t   const & r) {return int_t  (l) /= r;}
inline int_l_t operator / (int_l_t const & l, int_l_t const & r) {return int_l_t(l) /= r;}
inline int_t   operator % (int_t   const & l, int_t   const & r) {return int_t  (l) %= r;}
inline int_l_t operator % (int_l_t const & l, int_l_t const & r) {return int_l_t(l) %= r;}
inline int_t   operator + (int_t   const & l, int_t   const & r) {return int_t  (l) += r;}
inline int_l_t operator + (int_l_t const & l, int_l_t const & r) {return int_l_t(l) += r;}
inline int_t   operator - (int_t   const & l, int_t   const & r) {return int_t  (l) -= r;}
inline int_l_t operator - (int_l_t const & l, int_l_t const & r) {return int_l_t(l) -= r;}



#endif /* INT_T_H */



