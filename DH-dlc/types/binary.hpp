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
class string8_t
{
	public:
		explicit string8_t();
		         string8_t(string8_t const &);
		explicit string8_t(char const *);
		explicit string8_t(std::string const &);

		void clear();

		void encodeBinary(std::ostream & out);

		std::string makeString() const {return std::string(_data, 8);}

		char const operator [] (size_t index) const {return _data[index];}
		char       operator [] (size_t index)       {return _data[index];}

		string8_t & operator += (string8_t const &);
		string8_t & operator  = (string8_t const &);

	private:
		char _data[8];
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
	8-bit unsigned. (Hexen)
*/
class ubyte_t
{
	public:
		explicit ubyte_t();
		         ubyte_t(ubyte_t const &);
		explicit ubyte_t(unsigned char);

		void encodeBinary(std::ostream & out);

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
	16-bit unsigned. (Doom)
*/
class uword_t
{
	public:
		explicit uword_t();
		         uword_t(uword_t const &);
		explicit uword_t(unsigned short int);

		void encodeBinary(std::ostream & out);

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



sword_t abs(sword_t const &);
ubyte_t abs(ubyte_t const &);
uword_t abs(uword_t const &);

int cmp(string8_t const &, string8_t const &);
int cmp(sword_t   const &, sword_t   const &);
int cmp(ubyte_t   const &, ubyte_t   const &);
int cmp(uword_t   const &, uword_t   const &);

sword_t hypot(sword_t const &, sword_t const &);
ubyte_t hypot(ubyte_t const &, ubyte_t const &);
uword_t hypot(uword_t const &, uword_t const &);

bool operator == (string8_t const &, string8_t const &);
bool operator == (sword_t   const &, sword_t   const &);
bool operator == (ubyte_t   const &, ubyte_t   const &);
bool operator == (uword_t   const &, uword_t   const &);

bool operator != (string8_t const &, string8_t const &);
bool operator != (sword_t   const &, sword_t   const &);
bool operator != (ubyte_t   const &, ubyte_t   const &);
bool operator != (uword_t   const &, uword_t   const &);

bool operator >= (string8_t const &, string8_t const &);
bool operator >= (sword_t   const &, sword_t   const &);
bool operator >= (ubyte_t   const &, ubyte_t   const &);
bool operator >= (uword_t   const &, uword_t   const &);

bool operator >  (string8_t const &, string8_t const &);
bool operator >  (sword_t   const &, sword_t   const &);
bool operator >  (ubyte_t   const &, ubyte_t   const &);
bool operator >  (uword_t   const &, uword_t   const &);

bool operator <= (string8_t const &, string8_t const &);
bool operator <= (sword_t   const &, sword_t   const &);
bool operator <= (ubyte_t   const &, ubyte_t   const &);
bool operator <= (uword_t   const &, uword_t   const &);

bool operator <  (string8_t const &, string8_t const &);
bool operator <  (sword_t   const &, sword_t   const &);
bool operator <  (ubyte_t   const &, ubyte_t   const &);
bool operator <  (uword_t   const &, uword_t   const &);

sword_t   operator & (sword_t   const &, sword_t   const &);
ubyte_t   operator & (ubyte_t   const &, ubyte_t   const &);
uword_t   operator & (uword_t   const &, uword_t   const &);

sword_t   operator | (sword_t   const &, sword_t   const &);
ubyte_t   operator | (ubyte_t   const &, ubyte_t   const &);
uword_t   operator | (uword_t   const &, uword_t   const &);

sword_t   operator * (sword_t   const &, sword_t   const &);
ubyte_t   operator * (ubyte_t   const &, ubyte_t   const &);
uword_t   operator * (uword_t   const &, uword_t   const &);

sword_t   operator / (sword_t   const &, sword_t   const &);
ubyte_t   operator / (ubyte_t   const &, ubyte_t   const &);
uword_t   operator / (uword_t   const &, uword_t   const &);

sword_t   operator % (sword_t   const &, sword_t   const &);
ubyte_t   operator % (ubyte_t   const &, ubyte_t   const &);
uword_t   operator % (uword_t   const &, uword_t   const &);

string8_t operator + (string8_t const &, string8_t const &);
sword_t   operator + (sword_t   const &, sword_t   const &);
ubyte_t   operator + (ubyte_t   const &, ubyte_t   const &);
uword_t   operator + (uword_t   const &, uword_t   const &);

sword_t   operator - (sword_t   const &, sword_t   const &);
ubyte_t   operator - (ubyte_t   const &, ubyte_t   const &);
uword_t   operator - (uword_t   const &, uword_t   const &);

std::ostream& operator << (std::ostream &, string8_t const &);
std::ostream& operator << (std::ostream &, sword_t   const &);
std::ostream& operator << (std::ostream &, ubyte_t   const &);
std::ostream& operator << (std::ostream &, uword_t   const &);

sword_t sqrt(sword_t const &);
ubyte_t sqrt(ubyte_t const &);
uword_t sqrt(uword_t const &);



inline sword_t abs(sword_t const & i) {return (i < sword_t(0)) ? (i * sword_t(-1)) : (i);}
inline ubyte_t abs(ubyte_t const & i) {return i;}
inline uword_t abs(uword_t const & i) {return i;}

inline sword_t hypot(sword_t const & l, sword_t const & r) {return sqrt((l * l) + (r * r));}
inline ubyte_t hypot(ubyte_t const & l, ubyte_t const & r) {return sqrt((l * l) + (r * r));}
inline uword_t hypot(uword_t const & l, uword_t const & r) {return sqrt((l * l) + (r * r));}

inline bool operator == (string8_t const & l, string8_t const & r) {return cmp(l, r) == 0;}
inline bool operator == (sword_t   const & l, sword_t   const & r) {return cmp(l, r) == 0;}
inline bool operator == (ubyte_t   const & l, ubyte_t   const & r) {return cmp(l, r) == 0;}
inline bool operator == (uword_t   const & l, uword_t   const & r) {return cmp(l, r) == 0;}

inline bool operator != (string8_t const & l, string8_t const & r) {return cmp(l, r) != 0;}
inline bool operator != (sword_t   const & l, sword_t   const & r) {return cmp(l, r) != 0;}
inline bool operator != (ubyte_t   const & l, ubyte_t   const & r) {return cmp(l, r) != 0;}
inline bool operator != (uword_t   const & l, uword_t   const & r) {return cmp(l, r) != 0;}

inline bool operator >= (string8_t const & l, string8_t const & r) {return cmp(l, r) >= 0;}
inline bool operator >= (sword_t   const & l, sword_t   const & r) {return cmp(l, r) >= 0;}
inline bool operator >= (ubyte_t   const & l, ubyte_t   const & r) {return cmp(l, r) >= 0;}
inline bool operator >= (uword_t   const & l, uword_t   const & r) {return cmp(l, r) >= 0;}

inline bool operator >  (string8_t const & l, string8_t const & r) {return cmp(l, r) >  0;}
inline bool operator >  (sword_t   const & l, sword_t   const & r) {return cmp(l, r) >  0;}
inline bool operator >  (ubyte_t   const & l, ubyte_t   const & r) {return cmp(l, r) >  0;}
inline bool operator >  (uword_t   const & l, uword_t   const & r) {return cmp(l, r) >  0;}

inline bool operator <= (string8_t const & l, string8_t const & r) {return cmp(l, r) <= 0;}
inline bool operator <= (sword_t   const & l, sword_t   const & r) {return cmp(l, r) <= 0;}
inline bool operator <= (ubyte_t   const & l, ubyte_t   const & r) {return cmp(l, r) <= 0;}
inline bool operator <= (uword_t   const & l, uword_t   const & r) {return cmp(l, r) <= 0;}

inline bool operator <  (string8_t const & l, string8_t const & r) {return cmp(l, r) <  0;}
inline bool operator <  (sword_t   const & l, sword_t   const & r) {return cmp(l, r) <  0;}
inline bool operator <  (ubyte_t   const & l, ubyte_t   const & r) {return cmp(l, r) <  0;}
inline bool operator <  (uword_t   const & l, uword_t   const & r) {return cmp(l, r) <  0;}

inline sword_t   operator & (sword_t   const & l, sword_t   const & r) {return sword_t  (l) &= r;}
inline ubyte_t   operator & (ubyte_t   const & l, ubyte_t   const & r) {return ubyte_t  (l) &= r;}
inline uword_t   operator & (uword_t   const & l, uword_t   const & r) {return uword_t  (l) &= r;}

inline sword_t   operator | (sword_t   const & l, sword_t   const & r) {return sword_t  (l) |= r;}
inline ubyte_t   operator | (ubyte_t   const & l, ubyte_t   const & r) {return ubyte_t  (l) |= r;}
inline uword_t   operator | (uword_t   const & l, uword_t   const & r) {return uword_t  (l) |= r;}

inline sword_t   operator * (sword_t   const & l, sword_t   const & r) {return sword_t  (l) *= r;}
inline ubyte_t   operator * (ubyte_t   const & l, ubyte_t   const & r) {return ubyte_t  (l) *= r;}
inline uword_t   operator * (uword_t   const & l, uword_t   const & r) {return uword_t  (l) *= r;}

inline sword_t   operator / (sword_t   const & l, sword_t   const & r) {return sword_t  (l) /= r;}
inline ubyte_t   operator / (ubyte_t   const & l, ubyte_t   const & r) {return ubyte_t  (l) /= r;}
inline uword_t   operator / (uword_t   const & l, uword_t   const & r) {return uword_t  (l) /= r;}

inline sword_t   operator % (sword_t   const & l, sword_t   const & r) {return sword_t  (l) %= r;}
inline ubyte_t   operator % (ubyte_t   const & l, ubyte_t   const & r) {return ubyte_t  (l) %= r;}
inline uword_t   operator % (uword_t   const & l, uword_t   const & r) {return uword_t  (l) %= r;}

inline string8_t operator + (string8_t const & l, string8_t const & r) {return string8_t(l) += r;}
inline sword_t   operator + (sword_t   const & l, sword_t   const & r) {return sword_t  (l) += r;}
inline ubyte_t   operator + (ubyte_t   const & l, ubyte_t   const & r) {return ubyte_t  (l) += r;}
inline uword_t   operator + (uword_t   const & l, uword_t   const & r) {return uword_t  (l) += r;}

inline sword_t   operator - (sword_t   const & l, sword_t   const & r) {return sword_t  (l) -= r;}
inline ubyte_t   operator - (ubyte_t   const & l, ubyte_t   const & r) {return ubyte_t  (l) -= r;}
inline uword_t   operator - (uword_t   const & l, uword_t   const & r) {return uword_t  (l) -= r;}

inline sword_t sqrt(sword_t const & i) {return sword_t(sqrt(i.makeInt()));}
inline ubyte_t sqrt(ubyte_t const & i) {return ubyte_t(sqrt(i.makeInt()));}
inline uword_t sqrt(uword_t const & i) {return uword_t(sqrt(i.makeInt()));}



#endif /* TYPES__BINARY_H */



