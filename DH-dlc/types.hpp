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
	Defines fundamental internal types. Also defines the functions for
	using them like normal types, and converting between one another.

	2010/02/03 - Added name_t class to try and make name handling simpler.
	2010/02/19 - Use the more standard ptrdiff_t for int_s_t instead of
		ssize_t to improve portability.
	2010/05/04 - Remove str_t typedef. Replace all uses with either
		std::string or the new string_t class. Because this obviously
		affects almost all of the code, it will only be noted here. I
		am putting an end to the use of "str" and also much of the
		overlap between user-exposed types and internal types.
	2010/05/05 - Moved int_t to its own file.
	2010/05/06 - No more inline functions here, mostly because of the new
		types. (Circular dependencies make me weep.)
*/

#ifndef TYPES_H
#define TYPES_H

//#define __STDC_LIMIT_MACROS

#include <cfloat>
#include <climits>
#include <map>
#include <ostream>
#include <sstream>
#include <string>

#include "types_limits.hpp"



#define  int_s_t_MAX SPOINTER_MAX
#define  int_s_t_MIN SPOINTER_MIN
#define uint_s_t_MAX UPOINTER_MAX

#define  int_t_MAX int_t::int_t_max
#define  int_t_MIN int_t::int_t_min
#define uint_t_MAX int_t::uint_t_max

#if USE_GMPLIB
#define  int_l_t_MAX 0
#define  int_l_t_MIN 0
#define uint_l_t_MAX 0
#else
#define  int_l_t_MAX SINT_BIGGEST_MAX
#define  int_l_t_MIN SINT_BIGGEST_MIN
#define uint_l_t_MAX UINT_BIGGEST_MAX
#endif

#define  real_s_t_MAX   DBL_MAX
#define  real_s_t_MIN (-DBL_MAX)
#define ureal_s_t_MAX   DBL_MAX

#define  real_t_MAX  real_l_t_MAX
#define  real_t_MIN  real_l_t_MIN
#define ureal_t_MAX ureal_l_t_MAX

#if USE_GMPLIB
#define  real_l_t_MAX 0
#define  real_l_t_MIN 0
#define ureal_l_t_MAX 0
#else
#define  real_l_t_MAX   FLOAT_BIGGEST_MAX
#define  real_l_t_MIN (-FLOAT_BIGGEST_MAX)
#define ureal_l_t_MAX   FLOAT_BIGGEST_MAX
#endif



class LevelObjectData;
class LevelObjectName;
class LevelObjectPointer;
class LevelObjectType;

typedef LevelObjectData    any_t;
typedef bool               bool_t;
typedef ptrdiff_t          int_s_t;
class                      int_t;
class                      int_l_t;
typedef LevelObjectPointer obj_t;
typedef double             real_s_t;
class                      real_t;
class                      real_l_t;
class                      string_t;
class                      string8_t;
class                      sword_t;
class                      ubyte_t;
class                      uword_t;

typedef LevelObjectName name_t;
typedef LevelObjectType type_t;

typedef std::map<name_t, obj_t>  objmap_t;



template<class T>
std::string make_string(T const & in)
{
	std::ostringstream oss;

	oss.precision(255);

	oss << in;

	return oss.str();
}
std::string make_string(bool);

any_t to_any(any_t     const &);
any_t to_any(bool_t    const &);
any_t to_any(int_s_t   const &);
any_t to_any(int_t     const &);
any_t to_any(int_l_t   const &);
any_t to_any(obj_t            );
any_t to_any(real_s_t  const &);
any_t to_any(real_t    const &);
any_t to_any(real_l_t  const &);
any_t to_any(string_t  const &);
any_t to_any(string8_t const &);
any_t to_any(sword_t   const &);
any_t to_any(ubyte_t   const &);
any_t to_any(uword_t   const &);

bool_t to_bool(any_t     const &);
bool_t to_bool(bool_t    const &);
bool_t to_bool(int_s_t   const &);
bool_t to_bool(int_t     const &);
bool_t to_bool(int_l_t   const &);
bool_t to_bool(obj_t            );
bool_t to_bool(real_s_t  const &);
bool_t to_bool(real_t    const &);
bool_t to_bool(real_l_t  const &);
bool_t to_bool(string_t  const &);
bool_t to_bool(string8_t const &);
bool_t to_bool(sword_t   const &);
bool_t to_bool(ubyte_t   const &);
bool_t to_bool(uword_t   const &);

int_s_t to_int_s(any_t     const &);
int_s_t to_int_s(bool_t    const &);
int_s_t to_int_s(int_s_t   const &);
int_s_t to_int_s(int_t     const &);
int_s_t to_int_s(int_l_t   const &);
int_s_t to_int_s(obj_t            );
int_s_t to_int_s(real_s_t  const &);
int_s_t to_int_s(real_t    const &);
int_s_t to_int_s(real_l_t  const &);
int_s_t to_int_s(string_t  const &);
int_s_t to_int_s(string8_t const &);
int_s_t to_int_s(sword_t   const &);
int_s_t to_int_s(ubyte_t   const &);
int_s_t to_int_s(uword_t   const &);

int_t to_int(any_t     const &);
int_t to_int(bool_t    const &);
int_t to_int(int_s_t   const &);
int_t to_int(int_t     const &);
int_t to_int(int_l_t   const &);
int_t to_int(obj_t            );
int_t to_int(real_s_t  const &);
int_t to_int(real_t    const &);
int_t to_int(real_l_t  const &);
int_t to_int(string_t  const &);
int_t to_int(string8_t const &);
int_t to_int(sword_t   const &);
int_t to_int(ubyte_t   const &);
int_t to_int(uword_t   const &);

int_l_t to_int_l(any_t     const &);
int_l_t to_int_l(bool_t    const &);
int_l_t to_int_l(int_s_t   const &);
int_l_t to_int_l(int_t     const &);
int_l_t to_int_l(int_l_t   const &);
int_l_t to_int_l(obj_t            );
int_l_t to_int_l(real_s_t  const &);
int_l_t to_int_l(real_t    const &);
int_l_t to_int_l(real_l_t  const &);
int_l_t to_int_l(string_t  const &);
int_l_t to_int_l(string8_t const &);
int_l_t to_int_l(sword_t   const &);
int_l_t to_int_l(ubyte_t   const &);
int_l_t to_int_l(uword_t   const &);

obj_t to_obj(any_t     const &);
obj_t to_obj(bool_t    const &);
obj_t to_obj(int_s_t   const &);
obj_t to_obj(int_t     const &);
obj_t to_obj(int_l_t   const &);
obj_t to_obj(obj_t            );
obj_t to_obj(real_s_t  const &);
obj_t to_obj(real_t    const &);
obj_t to_obj(real_l_t  const &);
obj_t to_obj(string_t  const &);
obj_t to_obj(string8_t const &);
obj_t to_obj(sword_t   const &);
obj_t to_obj(ubyte_t   const &);
obj_t to_obj(uword_t   const &);

real_s_t to_real_s(any_t     const &);
real_s_t to_real_s(bool_t    const &);
real_s_t to_real_s(int_s_t   const &);
real_s_t to_real_s(int_t     const &);
real_s_t to_real_s(int_l_t   const &);
real_s_t to_real_s(obj_t            );
real_s_t to_real_s(real_s_t  const &);
real_s_t to_real_s(real_t    const &);
real_s_t to_real_s(real_l_t  const &);
real_s_t to_real_s(string_t  const &);
real_s_t to_real_s(string8_t const &);
real_s_t to_real_s(sword_t   const &);
real_s_t to_real_s(ubyte_t   const &);
real_s_t to_real_s(uword_t   const &);

real_t to_real(any_t     const &);
real_t to_real(bool_t    const &);
real_t to_real(int_s_t   const &);
real_t to_real(int_t     const &);
real_t to_real(int_l_t   const &);
real_t to_real(obj_t            );
real_t to_real(real_s_t  const &);
real_t to_real(real_t    const &);
real_t to_real(real_l_t  const &);
real_t to_real(string_t  const &);
real_t to_real(string8_t const &);
real_t to_real(sword_t   const &);
real_t to_real(ubyte_t   const &);
real_t to_real(uword_t   const &);

real_l_t to_real_l(any_t     const &);
real_l_t to_real_l(bool_t    const &);
real_l_t to_real_l(int_s_t   const &);
real_l_t to_real_l(int_t     const &);
real_l_t to_real_l(int_l_t   const &);
real_l_t to_real_l(obj_t            );
real_l_t to_real_l(real_s_t  const &);
real_l_t to_real_l(real_t    const &);
real_l_t to_real_l(real_l_t  const &);
real_l_t to_real_l(string_t  const &);
real_l_t to_real_l(string8_t const &);
real_l_t to_real_l(sword_t   const &);
real_l_t to_real_l(ubyte_t   const &);
real_l_t to_real_l(uword_t   const &);

string_t to_string(any_t     const &);
string_t to_string(bool_t    const &);
string_t to_string(int_s_t   const &);
string_t to_string(int_t     const &);
string_t to_string(int_l_t   const &);
string_t to_string(obj_t            );
string_t to_string(real_s_t  const &);
string_t to_string(real_t    const &);
string_t to_string(real_l_t  const &);
string_t to_string(string_t  const &);
string_t to_string(string8_t const &);
string_t to_string(sword_t   const &);
string_t to_string(ubyte_t   const &);
string_t to_string(uword_t   const &);

string8_t to_string8(any_t     const &);
string8_t to_string8(bool_t    const &);
string8_t to_string8(int_s_t   const &);
string8_t to_string8(int_t     const &);
string8_t to_string8(int_l_t   const &);
string8_t to_string8(obj_t            );
string8_t to_string8(real_s_t  const &);
string8_t to_string8(real_t    const &);
string8_t to_string8(real_l_t  const &);
string8_t to_string8(string_t  const &);
string8_t to_string8(string8_t const &);
string8_t to_string8(sword_t   const &);
string8_t to_string8(ubyte_t   const &);
string8_t to_string8(uword_t   const &);

sword_t to_sword(any_t     const &);
sword_t to_sword(bool_t    const &);
sword_t to_sword(int_s_t   const &);
sword_t to_sword(int_t     const &);
sword_t to_sword(int_l_t   const &);
sword_t to_sword(obj_t            );
sword_t to_sword(real_s_t  const &);
sword_t to_sword(real_t    const &);
sword_t to_sword(real_l_t  const &);
sword_t to_sword(string_t  const &);
sword_t to_sword(string8_t const &);
sword_t to_sword(sword_t   const &);
sword_t to_sword(ubyte_t   const &);
sword_t to_sword(uword_t   const &);

ubyte_t to_ubyte(any_t     const &);
ubyte_t to_ubyte(bool_t    const &);
ubyte_t to_ubyte(int_s_t   const &);
ubyte_t to_ubyte(int_t     const &);
ubyte_t to_ubyte(int_l_t   const &);
ubyte_t to_ubyte(obj_t            );
ubyte_t to_ubyte(real_s_t  const &);
ubyte_t to_ubyte(real_t    const &);
ubyte_t to_ubyte(real_l_t  const &);
ubyte_t to_ubyte(string_t  const &);
ubyte_t to_ubyte(string8_t const &);
ubyte_t to_ubyte(sword_t   const &);
ubyte_t to_ubyte(ubyte_t   const &);
ubyte_t to_ubyte(uword_t   const &);

uword_t to_uword(any_t     const &);
uword_t to_uword(bool_t    const &);
uword_t to_uword(int_s_t   const &);
uword_t to_uword(int_t     const &);
uword_t to_uword(int_l_t   const &);
uword_t to_uword(obj_t            );
uword_t to_uword(real_s_t  const &);
uword_t to_uword(real_t    const &);
uword_t to_uword(real_l_t  const &);
uword_t to_uword(string_t  const &);
uword_t to_uword(string8_t const &);
uword_t to_uword(sword_t   const &);
uword_t to_uword(ubyte_t   const &);
uword_t to_uword(uword_t   const &);



#endif /* TYPES_H */



