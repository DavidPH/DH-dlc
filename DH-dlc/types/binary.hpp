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
	The types used in binary map formats. Mostly intended to help clean up
	LevelObject_encode.cpp. (It's not really "messy" right now, but I'd
	like to be able to generalize it.) But I'll probably eventually expose
	them to users for no adequately explained reason.

	I use classes instead of stdint.h for portability. And so that I can
	maintain better type differentiation. And so I can have an encodeBinary
	member function. And...

	2010/05/09 - Original version.
	2010/06/24 - Added abs, sqrt, and hypot for the numeric classes so that
		they can be treated like the other int classes.
*/

#ifndef TYPES__BINARY_H
#define TYPES__BINARY_H

#include <cmath>
#include <ostream>
#include <string>



/*
	Fixed length string. (Doom)
*/
template <size_t S>
class stringf_t
{
	public:
		explicit stringf_t();
		         stringf_t(stringf_t<S> const &);
		explicit stringf_t(char const *);
		explicit stringf_t(std::string const &);

		void clear();

		void encodeBinary(std::ostream & out);
		void encodeText  (std::ostream & out);

		std::string makeString() const {return std::string(_data, S);}

		char const & operator [] (size_t index) const {return _data[index];}
		char       & operator [] (size_t index)       {return _data[index];}

		stringf_t & operator += (stringf_t<S> const &);
		stringf_t & operator  = (stringf_t<S> const &);

	private:
		char _data[S];
};
/*
	8-char string. (Doom)
*/
typedef stringf_t<8U> string8_t;
/*
	16-char string. (Strife)
*/
typedef stringf_t<16U> string16_t;
/*
	32-char string. (Strife)
*/
typedef stringf_t<32U> string32_t;
/*
	80-char string. (Strife)
*/
typedef stringf_t<80U> string80_t;
/*
	320-char string. (Strife)
*/
typedef stringf_t<320U> string320_t;

/*
	8-bit unsigned. (Hexen)
*/
class ubyte_t
{
	public:
		explicit ubyte_t();
		         ubyte_t(ubyte_t const &);
		explicit ubyte_t(unsigned char);

		void encodeBinary(std::ostream & out);
		void encodeText  (std::ostream & out);

		unsigned char makeInt() const {return _data;}

		ubyte_t & operator &= (ubyte_t const & v) {_data &= v._data; return *this;}
		ubyte_t & operator |= (ubyte_t const & v) {_data |= v._data; return *this;}
		ubyte_t & operator *= (ubyte_t const & v) {_data *= v._data; return *this;}
		ubyte_t & operator /= (ubyte_t const & v) {_data /= v._data; return *this;}
		ubyte_t & operator %= (ubyte_t const & v) {_data %= v._data; return *this;}
		ubyte_t & operator += (ubyte_t const & v) {_data += v._data; return *this;}
		ubyte_t & operator -= (ubyte_t const & v) {_data -= v._data; return *this;}
		ubyte_t & operator  = (ubyte_t const & v) {_data  = v._data; return *this;}



		friend int cmp(ubyte_t const &, ubyte_t const &);

		friend std::ostream& operator << (std::ostream &, ubyte_t const &);

	private:
		unsigned _data : 8;
};

/*
	16-bit signed. (Doom)
*/
class sword_t
{
	public:
		explicit sword_t();
		         sword_t(sword_t const &);
		explicit sword_t(signed short int);

		void encodeBinary(std::ostream & out);
		void encodeText  (std::ostream & out);

		signed short int makeInt() const {return _data;}

		sword_t & operator &= (sword_t const & v) {_data &= v._data; return *this;}
		sword_t & operator |= (sword_t const & v) {_data |= v._data; return *this;}
		sword_t & operator *= (sword_t const & v) {_data *= v._data; return *this;}
		sword_t & operator /= (sword_t const & v) {_data /= v._data; return *this;}
		sword_t & operator %= (sword_t const & v) {_data %= v._data; return *this;}
		sword_t & operator += (sword_t const & v) {_data += v._data; return *this;}
		sword_t & operator -= (sword_t const & v) {_data -= v._data; return *this;}
		sword_t & operator  = (sword_t const & v) {_data  = v._data; return *this;}



		friend int cmp(sword_t const &, sword_t const &);

		friend std::ostream& operator << (std::ostream &, sword_t const &);

	private:
		signed _data : 16;
};

/*
	16-bit unsigned. (Doom)
*/
class uword_t
{
	public:
		explicit uword_t();
		         uword_t(uword_t const &);
		explicit uword_t(unsigned short int);

		void encodeBinary(std::ostream & out);
		void encodeText  (std::ostream & out);

		unsigned short int makeInt() const {return _data;}

		uword_t & operator &= (uword_t const & v) {_data &= v._data; return *this;}
		uword_t & operator |= (uword_t const & v) {_data |= v._data; return *this;}
		uword_t & operator *= (uword_t const & v) {_data *= v._data; return *this;}
		uword_t & operator /= (uword_t const & v) {_data /= v._data; return *this;}
		uword_t & operator %= (uword_t const & v) {_data %= v._data; return *this;}
		uword_t & operator += (uword_t const & v) {_data += v._data; return *this;}
		uword_t & operator -= (uword_t const & v) {_data -= v._data; return *this;}
		uword_t & operator  = (uword_t const & v) {_data  = v._data; return *this;}



		friend int cmp(uword_t const &, uword_t const &);

		friend std::ostream& operator << (std::ostream &, uword_t const &);

	private:
		unsigned _data : 16;
};

/*
	32-bit signed. (Strife)
*/
class sdword_t
{
	public:
		explicit sdword_t();
		         sdword_t(sdword_t const &);
		explicit sdword_t(signed long int);

		void encodeBinary(std::ostream & out);
		void encodeText  (std::ostream & out);

		signed long int makeInt() const {return _data;}

		sdword_t & operator &= (sdword_t const & v) {_data &= v._data; return *this;}
		sdword_t & operator |= (sdword_t const & v) {_data |= v._data; return *this;}
		sdword_t & operator *= (sdword_t const & v) {_data *= v._data; return *this;}
		sdword_t & operator /= (sdword_t const & v) {_data /= v._data; return *this;}
		sdword_t & operator %= (sdword_t const & v) {_data %= v._data; return *this;}
		sdword_t & operator += (sdword_t const & v) {_data += v._data; return *this;}
		sdword_t & operator -= (sdword_t const & v) {_data -= v._data; return *this;}
		sdword_t & operator  = (sdword_t const & v) {_data  = v._data; return *this;}



		friend int cmp(sdword_t const &, sdword_t const &);

		friend std::ostream & operator << (std::ostream &, sdword_t const &);

	private:
		signed _data : 32;
};

/*
	32-bit unsigned. (Strife)
*/
class udword_t
{
	public:
		explicit udword_t();
		         udword_t(udword_t const &);
		explicit udword_t(unsigned long int);

		void encodeBinary(std::ostream & out);
		void encodeText  (std::ostream & out);

		unsigned long int makeInt() const {return _data;}

		udword_t & operator &= (udword_t const & v) {_data &= v._data; return *this;}
		udword_t & operator |= (udword_t const & v) {_data |= v._data; return *this;}
		udword_t & operator *= (udword_t const & v) {_data *= v._data; return *this;}
		udword_t & operator /= (udword_t const & v) {_data /= v._data; return *this;}
		udword_t & operator %= (udword_t const & v) {_data %= v._data; return *this;}
		udword_t & operator += (udword_t const & v) {_data += v._data; return *this;}
		udword_t & operator -= (udword_t const & v) {_data -= v._data; return *this;}
		udword_t & operator  = (udword_t const & v) {_data  = v._data; return *this;}



		friend int cmp(udword_t const &, udword_t const &);

		friend std::ostream & operator << (std::ostream &, udword_t const &);

	private:
		unsigned _data : 32;
};



ubyte_t  abs(ubyte_t  const &);
sword_t  abs(sword_t  const &);
uword_t  abs(uword_t  const &);
sdword_t abs(sdword_t const &);
udword_t abs(udword_t const &);

template <size_t S>
int cmp(stringf_t<S> const &, stringf_t<S> const &);
int cmp(ubyte_t      const &, ubyte_t      const &);
int cmp(sword_t      const &, sword_t      const &);
int cmp(uword_t      const &, uword_t      const &);
int cmp(sdword_t     const &, sdword_t     const &);
int cmp(udword_t     const &, udword_t     const &);

ubyte_t  hypot(ubyte_t  const &, ubyte_t  const &);
sword_t  hypot(sword_t  const &, sword_t  const &);
uword_t  hypot(uword_t  const &, uword_t  const &);
sdword_t hypot(sdword_t const &, sdword_t const &);
udword_t hypot(udword_t const &, udword_t const &);

template <size_t S>
bool operator == (stringf_t<S> const &, stringf_t<S> const &);
bool operator == (ubyte_t      const &, ubyte_t      const &);
bool operator == (sword_t      const &, sword_t      const &);
bool operator == (uword_t      const &, uword_t      const &);
bool operator == (sdword_t     const &, sdword_t     const &);
bool operator == (udword_t     const &, udword_t     const &);

template <size_t S>
bool operator != (stringf_t<S> const &, stringf_t<S> const &);
bool operator != (ubyte_t      const &, ubyte_t      const &);
bool operator != (sword_t      const &, sword_t      const &);
bool operator != (uword_t      const &, uword_t      const &);
bool operator != (sdword_t     const &, sdword_t     const &);
bool operator != (udword_t     const &, udword_t     const &);

template <size_t S>
bool operator >= (stringf_t<S> const &, stringf_t<S> const &);
bool operator >= (ubyte_t      const &, ubyte_t      const &);
bool operator >= (sword_t      const &, sword_t      const &);
bool operator >= (uword_t      const &, uword_t      const &);
bool operator >= (sdword_t     const &, sdword_t     const &);
bool operator >= (udword_t     const &, udword_t     const &);

template <size_t S>
bool operator >  (stringf_t<S> const &, stringf_t<S> const &);
bool operator >  (ubyte_t      const &, ubyte_t      const &);
bool operator >  (sword_t      const &, sword_t      const &);
bool operator >  (uword_t      const &, uword_t      const &);
bool operator >  (sdword_t     const &, sdword_t     const &);
bool operator >  (udword_t     const &, udword_t     const &);

template <size_t S>
bool operator <= (stringf_t<S> const &, stringf_t<S> const &);
bool operator <= (ubyte_t      const &, ubyte_t      const &);
bool operator <= (sword_t      const &, sword_t      const &);
bool operator <= (uword_t      const &, uword_t      const &);
bool operator <= (sdword_t     const &, sdword_t     const &);
bool operator <= (udword_t     const &, udword_t     const &);

template <size_t S>
bool operator <  (stringf_t<S> const &, stringf_t<S> const &);
bool operator <  (ubyte_t      const &, ubyte_t      const &);
bool operator <  (sword_t      const &, sword_t      const &);
bool operator <  (uword_t      const &, uword_t      const &);
bool operator <  (sdword_t     const &, sdword_t     const &);
bool operator <  (udword_t     const &, udword_t     const &);

ubyte_t      operator & (ubyte_t      const &, ubyte_t      const &);
sword_t      operator & (sword_t      const &, sword_t      const &);
uword_t      operator & (uword_t      const &, uword_t      const &);
sdword_t     operator & (sdword_t     const &, sdword_t     const &);
udword_t     operator & (udword_t     const &, udword_t     const &);

ubyte_t      operator | (ubyte_t      const &, ubyte_t      const &);
sword_t      operator | (sword_t      const &, sword_t      const &);
uword_t      operator | (uword_t      const &, uword_t      const &);
sdword_t     operator | (sdword_t     const &, sdword_t     const &);
udword_t     operator | (udword_t     const &, udword_t     const &);

ubyte_t      operator * (ubyte_t      const &, ubyte_t      const &);
sword_t      operator * (sword_t      const &, sword_t      const &);
uword_t      operator * (uword_t      const &, uword_t      const &);
sdword_t     operator * (sdword_t     const &, sdword_t     const &);
udword_t     operator * (udword_t     const &, udword_t     const &);

ubyte_t      operator / (ubyte_t      const &, ubyte_t      const &);
sword_t      operator / (sword_t      const &, sword_t      const &);
uword_t      operator / (uword_t      const &, uword_t      const &);
sdword_t     operator / (sdword_t     const &, sdword_t     const &);
udword_t     operator / (udword_t     const &, udword_t     const &);

ubyte_t      operator % (ubyte_t      const &, ubyte_t      const &);
sword_t      operator % (sword_t      const &, sword_t      const &);
uword_t      operator % (uword_t      const &, uword_t      const &);
sdword_t     operator % (sdword_t     const &, sdword_t     const &);
udword_t     operator % (udword_t     const &, udword_t     const &);

template <size_t S>
stringf_t<S> operator + (stringf_t<S> const &, stringf_t<S> const &);
ubyte_t      operator + (ubyte_t      const &, ubyte_t      const &);
sword_t      operator + (sword_t      const &, sword_t      const &);
uword_t      operator + (uword_t      const &, uword_t      const &);
sdword_t     operator + (sdword_t     const &, sdword_t     const &);
udword_t     operator + (udword_t     const &, udword_t     const &);

ubyte_t      operator - (ubyte_t      const &, ubyte_t      const &);
sword_t      operator - (sword_t      const &, sword_t      const &);
uword_t      operator - (uword_t      const &, uword_t      const &);
sdword_t     operator - (sdword_t     const &, sdword_t     const &);
udword_t     operator - (udword_t     const &, udword_t     const &);

template <size_t S>
std::ostream & operator << (std::ostream &, stringf_t<S> const &);
std::ostream & operator << (std::ostream &, ubyte_t      const &);
std::ostream & operator << (std::ostream &, sword_t      const &);
std::ostream & operator << (std::ostream &, uword_t      const &);
std::ostream & operator << (std::ostream &, sdword_t     const &);
std::ostream & operator << (std::ostream &, udword_t     const &);

ubyte_t  sqrt(ubyte_t  const &);
sword_t  sqrt(sword_t  const &);
uword_t  sqrt(uword_t  const &);
sdword_t sqrt(sdword_t const &);
udword_t sqrt(udword_t const &);



inline ubyte_t  abs(ubyte_t const & i)  {return i;}
inline sword_t  abs(sword_t const & i)  {return (i < sword_t (0)) ? (i * sword_t (-1)) : (i);}
inline uword_t  abs(uword_t const & i)  {return i;}
inline sdword_t abs(sdword_t const & i) {return (i < sdword_t(0)) ? (i * sdword_t(-1)) : (i);}
inline udword_t abs(udword_t const & i) {return i;}

inline ubyte_t  hypot(ubyte_t  const & l, ubyte_t  const & r) {return sqrt((l * l) + (r * r));}
inline sword_t  hypot(sword_t  const & l, sword_t  const & r) {return sqrt((l * l) + (r * r));}
inline uword_t  hypot(uword_t  const & l, uword_t  const & r) {return sqrt((l * l) + (r * r));}
inline sdword_t hypot(sdword_t const & l, sdword_t const & r) {return sqrt((l * l) + (r * r));}
inline udword_t hypot(udword_t const & l, udword_t const & r) {return sqrt((l * l) + (r * r));}

template <size_t S>
inline bool operator == (stringf_t<S> const & l, stringf_t<S> const & r) {return cmp(l, r) == 0;}
inline bool operator == (ubyte_t      const & l, ubyte_t      const & r) {return cmp(l, r) == 0;}
inline bool operator == (sword_t      const & l, sword_t      const & r) {return cmp(l, r) == 0;}
inline bool operator == (uword_t      const & l, uword_t      const & r) {return cmp(l, r) == 0;}
inline bool operator == (sdword_t     const & l, sdword_t     const & r) {return cmp(l, r) == 0;}
inline bool operator == (udword_t     const & l, udword_t     const & r) {return cmp(l, r) == 0;}

template <size_t S>
inline bool operator != (stringf_t<S> const & l, stringf_t<S> const & r) {return cmp(l, r) != 0;}
inline bool operator != (ubyte_t      const & l, ubyte_t      const & r) {return cmp(l, r) != 0;}
inline bool operator != (sword_t      const & l, sword_t      const & r) {return cmp(l, r) != 0;}
inline bool operator != (uword_t      const & l, uword_t      const & r) {return cmp(l, r) != 0;}
inline bool operator != (sdword_t     const & l, sdword_t     const & r) {return cmp(l, r) != 0;}
inline bool operator != (udword_t     const & l, udword_t     const & r) {return cmp(l, r) != 0;}

template <size_t S>
inline bool operator >= (stringf_t<S> const & l, stringf_t<S> const & r) {return cmp(l, r) >= 0;}
inline bool operator >= (ubyte_t      const & l, ubyte_t      const & r) {return cmp(l, r) >= 0;}
inline bool operator >= (sword_t      const & l, sword_t      const & r) {return cmp(l, r) >= 0;}
inline bool operator >= (uword_t      const & l, uword_t      const & r) {return cmp(l, r) >= 0;}
inline bool operator >= (sdword_t     const & l, sdword_t     const & r) {return cmp(l, r) >= 0;}
inline bool operator >= (udword_t     const & l, udword_t     const & r) {return cmp(l, r) >= 0;}

template <size_t S>
inline bool operator >  (stringf_t<S> const & l, stringf_t<S> const & r) {return cmp(l, r) >  0;}
inline bool operator >  (ubyte_t      const & l, ubyte_t      const & r) {return cmp(l, r) >  0;}
inline bool operator >  (sword_t      const & l, sword_t      const & r) {return cmp(l, r) >  0;}
inline bool operator >  (uword_t      const & l, uword_t      const & r) {return cmp(l, r) >  0;}
inline bool operator >  (sdword_t     const & l, sdword_t     const & r) {return cmp(l, r) >  0;}
inline bool operator >  (udword_t     const & l, udword_t     const & r) {return cmp(l, r) >  0;}

template <size_t S>
inline bool operator <= (stringf_t<S> const & l, stringf_t<S> const & r) {return cmp(l, r) <= 0;}
inline bool operator <= (ubyte_t      const & l, ubyte_t      const & r) {return cmp(l, r) <= 0;}
inline bool operator <= (sword_t      const & l, sword_t      const & r) {return cmp(l, r) <= 0;}
inline bool operator <= (uword_t      const & l, uword_t      const & r) {return cmp(l, r) <= 0;}
inline bool operator <= (sdword_t     const & l, sdword_t     const & r) {return cmp(l, r) <= 0;}
inline bool operator <= (udword_t     const & l, udword_t     const & r) {return cmp(l, r) <= 0;}

template <size_t S>
inline bool operator <  (stringf_t<S> const & l, stringf_t<S> const & r) {return cmp(l, r) <  0;}
inline bool operator <  (ubyte_t      const & l, ubyte_t      const & r) {return cmp(l, r) <  0;}
inline bool operator <  (sword_t      const & l, sword_t      const & r) {return cmp(l, r) <  0;}
inline bool operator <  (uword_t      const & l, uword_t      const & r) {return cmp(l, r) <  0;}
inline bool operator <  (sdword_t     const & l, sdword_t     const & r) {return cmp(l, r) <  0;}
inline bool operator <  (udword_t     const & l, udword_t     const & r) {return cmp(l, r) <  0;}

inline ubyte_t      operator & (ubyte_t      const & l, ubyte_t      const & r) {return ubyte_t     (l) &= r;}
inline sword_t      operator & (sword_t      const & l, sword_t      const & r) {return sword_t     (l) &= r;}
inline uword_t      operator & (uword_t      const & l, uword_t      const & r) {return uword_t     (l) &= r;}
inline sdword_t     operator & (sdword_t     const & l, sdword_t     const & r) {return sdword_t    (l) &= r;}
inline udword_t     operator & (udword_t     const & l, udword_t     const & r) {return udword_t    (l) &= r;}

inline ubyte_t      operator | (ubyte_t      const & l, ubyte_t      const & r) {return ubyte_t     (l) |= r;}
inline sword_t      operator | (sword_t      const & l, sword_t      const & r) {return sword_t     (l) |= r;}
inline uword_t      operator | (uword_t      const & l, uword_t      const & r) {return uword_t     (l) |= r;}
inline sdword_t     operator | (sdword_t     const & l, sdword_t     const & r) {return sdword_t    (l) |= r;}
inline udword_t     operator | (udword_t     const & l, udword_t     const & r) {return udword_t    (l) |= r;}

inline ubyte_t      operator * (ubyte_t      const & l, ubyte_t      const & r) {return ubyte_t     (l) *= r;}
inline sword_t      operator * (sword_t      const & l, sword_t      const & r) {return sword_t     (l) *= r;}
inline uword_t      operator * (uword_t      const & l, uword_t      const & r) {return uword_t     (l) *= r;}
inline sdword_t     operator * (sdword_t     const & l, sdword_t     const & r) {return sdword_t    (l) *= r;}
inline udword_t     operator * (udword_t     const & l, udword_t     const & r) {return udword_t    (l) *= r;}

inline ubyte_t      operator / (ubyte_t      const & l, ubyte_t      const & r) {return ubyte_t     (l) /= r;}
inline sword_t      operator / (sword_t      const & l, sword_t      const & r) {return sword_t     (l) /= r;}
inline uword_t      operator / (uword_t      const & l, uword_t      const & r) {return uword_t     (l) /= r;}
inline sdword_t     operator / (sdword_t     const & l, sdword_t     const & r) {return sdword_t    (l) /= r;}
inline udword_t     operator / (udword_t     const & l, udword_t     const & r) {return udword_t    (l) /= r;}

inline ubyte_t      operator % (ubyte_t      const & l, ubyte_t      const & r) {return ubyte_t     (l) %= r;}
inline sword_t      operator % (sword_t      const & l, sword_t      const & r) {return sword_t     (l) %= r;}
inline uword_t      operator % (uword_t      const & l, uword_t      const & r) {return uword_t     (l) %= r;}
inline sdword_t     operator % (sdword_t     const & l, sdword_t     const & r) {return sdword_t    (l) %= r;}
inline udword_t     operator % (udword_t     const & l, udword_t     const & r) {return udword_t    (l) %= r;}

template <size_t S>
inline stringf_t<S> operator + (stringf_t<S> const & l, stringf_t<S> const & r) {return stringf_t<S>(l) += r;}
inline ubyte_t      operator + (ubyte_t      const & l, ubyte_t      const & r) {return ubyte_t     (l) += r;}
inline sword_t      operator + (sword_t      const & l, sword_t      const & r) {return sword_t     (l) += r;}
inline uword_t      operator + (uword_t      const & l, uword_t      const & r) {return uword_t     (l) += r;}
inline sdword_t     operator + (sdword_t     const & l, sdword_t     const & r) {return sdword_t    (l) += r;}
inline udword_t     operator + (udword_t     const & l, udword_t     const & r) {return udword_t    (l) += r;}

inline ubyte_t      operator - (ubyte_t      const & l, ubyte_t      const & r) {return ubyte_t     (l) -= r;}
inline sword_t      operator - (sword_t      const & l, sword_t      const & r) {return sword_t     (l) -= r;}
inline uword_t      operator - (uword_t      const & l, uword_t      const & r) {return uword_t     (l) -= r;}
inline sdword_t     operator - (sdword_t     const & l, sdword_t     const & r) {return sdword_t    (l) -= r;}
inline udword_t     operator - (udword_t     const & l, udword_t     const & r) {return udword_t    (l) -= r;}

inline ubyte_t  sqrt(ubyte_t const & i)  {return ubyte_t (sqrt(i.makeInt()));}
inline sword_t  sqrt(sword_t const & i)  {return sword_t (sqrt(i.makeInt()));}
inline uword_t  sqrt(uword_t const & i)  {return uword_t (sqrt(i.makeInt()));}
inline sdword_t sqrt(sdword_t const & i) {return sdword_t(sqrt(i.makeInt()));}
inline udword_t sqrt(udword_t const & i) {return udword_t(sqrt(i.makeInt()));}



#endif /* TYPES__BINARY_H */



