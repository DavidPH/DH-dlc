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
	Declares fundamental internal types. Also declares the functions for
	converting between one another.

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

#include "types_limits.hpp"

#include <cfloat>
#include <climits>
#include <sstream>
#include <string>



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
class LevelObjectMap;
class LevelObjectName;
class LevelObjectPointer;
class LevelObjectType;

template <size_t S>
class stringf_t;

typedef LevelObjectData    any_t;
typedef bool               bool_t;
typedef ptrdiff_t          int_s_t;
class                      int_t;
class                      int_l_t;
typedef LevelObjectPointer obj_t;
typedef LevelObjectMap     objmap_t;
typedef double             real_s_t;
class                      real_t;
class                      real_l_t;
class                      string_t;
typedef stringf_t<  8U>    string8_t;
typedef stringf_t< 16U>    string16_t;
typedef stringf_t< 32U>    string32_t;
typedef stringf_t< 80U>    string80_t;
typedef stringf_t<320U>    string320_t;
class                      ubyte_t;
class                      sword_t;
class                      uword_t;
class                      sdword_t;
class                      udword_t;

typedef LevelObjectName name_t;
typedef LevelObjectType type_t;



template<class T>
std::string make_string(T const & in)
{
	std::ostringstream oss;

	oss.precision(255);

	oss << in;

	return oss.str();
}
template<>
std::string make_string<bool_t>(bool_t const & in);
template<>
std::string make_string<string_t>(string_t const & in);
template<>
std::string make_string<string8_t>(string8_t const & in);
template<>
std::string make_string<string16_t>(string16_t const & in);
template<>
std::string make_string<string32_t>(string32_t const & in);
template<>
std::string make_string<string80_t>(string80_t const & in);
template<>
std::string make_string<string320_t>(string320_t const & in);

template <class T>
T num_from_string(std::string const & value);

#define DECLARE_CONVERT(TYPE) \
TYPE##_t to_##TYPE(any_t       const &); \
TYPE##_t to_##TYPE(bool_t      const &); \
TYPE##_t to_##TYPE(int_s_t     const &); \
TYPE##_t to_##TYPE(int_t       const &); \
TYPE##_t to_##TYPE(int_l_t     const &); \
TYPE##_t to_##TYPE(obj_t              ); \
TYPE##_t to_##TYPE(real_s_t    const &); \
TYPE##_t to_##TYPE(real_t      const &); \
TYPE##_t to_##TYPE(real_l_t    const &); \
TYPE##_t to_##TYPE(string_t    const &); \
TYPE##_t to_##TYPE(string8_t   const &); \
TYPE##_t to_##TYPE(string16_t  const &); \
TYPE##_t to_##TYPE(string32_t  const &); \
TYPE##_t to_##TYPE(string80_t  const &); \
TYPE##_t to_##TYPE(string320_t const &); \
TYPE##_t to_##TYPE(ubyte_t     const &); \
TYPE##_t to_##TYPE(sword_t     const &); \
TYPE##_t to_##TYPE(uword_t     const &); \
TYPE##_t to_##TYPE(sdword_t    const &); \
TYPE##_t to_##TYPE(udword_t    const &); \

DECLARE_CONVERT(any)
DECLARE_CONVERT(bool)
DECLARE_CONVERT(int_s)
DECLARE_CONVERT(int)
DECLARE_CONVERT(int_l)
DECLARE_CONVERT(obj)
DECLARE_CONVERT(real_s)
DECLARE_CONVERT(real)
DECLARE_CONVERT(real_l)
DECLARE_CONVERT(string)
DECLARE_CONVERT(string8)
DECLARE_CONVERT(string16)
DECLARE_CONVERT(string32)
DECLARE_CONVERT(string80)
DECLARE_CONVERT(string320)
DECLARE_CONVERT(ubyte)
DECLARE_CONVERT(sword)
DECLARE_CONVERT(uword)
DECLARE_CONVERT(sdword)
DECLARE_CONVERT(udword)

#undef DECLARE_CONVERT



#define TEMPLATE_TAKE_Tconv \
T(Tconv_any)      (any_t       const &), \
T(Tconv_bool)     (bool_t      const &), \
T(Tconv_int_s)    (int_s_t     const &), \
T(Tconv_int)      (int_t       const &), \
T(Tconv_int_l)    (int_l_t     const &), \
T(Tconv_obj)      (obj_t              ), \
T(Tconv_real_s)   (real_s_t    const &), \
T(Tconv_real)     (real_t      const &), \
T(Tconv_real_l)   (real_l_t    const &), \
T(Tconv_string)   (string_t    const &), \
T(Tconv_string8)  (string8_t   const &), \
T(Tconv_string16) (string16_t  const &), \
T(Tconv_string32) (string32_t  const &), \
T(Tconv_string80) (string80_t  const &), \
T(Tconv_string320)(string320_t const &), \
T(Tconv_ubyte)    (ubyte_t     const &), \
T(Tconv_sword)    (sword_t     const &), \
T(Tconv_uword)    (uword_t     const &), \
T(Tconv_sdword)   (sdword_t    const &), \
T(Tconv_udword)   (udword_t    const &)
#define TEMPLATE_PUSH_Tconv \
Tconv_any,       \
Tconv_bool,      \
Tconv_int_s,     \
Tconv_int,       \
Tconv_int_l,     \
Tconv_obj,       \
Tconv_real_s,    \
Tconv_real,      \
Tconv_real_l,    \
Tconv_string,    \
Tconv_string8,   \
Tconv_string16,  \
Tconv_string32,  \
Tconv_string80,  \
Tconv_string320, \
Tconv_ubyte,     \
Tconv_sword,     \
Tconv_uword,     \
Tconv_sdword,    \
Tconv_udword
#define TEMPLATE_PUSH_Tconv_type(TYPE) \
to_##TYPE, \
to_##TYPE, \
to_##TYPE, \
to_##TYPE, \
to_##TYPE, \
to_##TYPE, \
to_##TYPE, \
to_##TYPE, \
to_##TYPE, \
to_##TYPE, \
to_##TYPE, \
to_##TYPE, \
to_##TYPE, \
to_##TYPE, \
to_##TYPE, \
to_##TYPE, \
to_##TYPE, \
to_##TYPE, \
to_##TYPE, \
to_##TYPE

#define TEMPLATE_TAKE_TfuncDDL \
T(Tfunc)(std::string const &, std::vector<std::string> const &), \
T(Tunary)(std::string const &, std::string const &),             \
T(Tconst)(std::string const &)
#define TEMPLATE_TAKE_TfuncDHLX \
T(Tfunc)(std::string const &, SourceScannerDHLX &),  \
T(Tunary)(std::string const &, SourceScannerDHLX &), \
T(Tconst)(std::string const &)
#define TEMPLATE_PUSH_Tfunc \
Tfunc,  \
Tunary, \
Tconst
#define TEMPLATE_PUSH_Tfunc_type(TYPE) \
parse_##TYPE##_function, \
parse_##TYPE##_unary,    \
parse_##TYPE##_const

#define TEMPLATE_TAKE_DDL \
typename T,          \
TEMPLATE_TAKE_Tconv, \
TEMPLATE_TAKE_TfuncDDL
#define TEMPLATE_TAKE_DHLX \
typename T,          \
TEMPLATE_TAKE_Tconv, \
TEMPLATE_TAKE_TfuncDHLX
#define TEMPLATE_PUSH \
T,                   \
TEMPLATE_PUSH_Tconv, \
TEMPLATE_PUSH_Tfunc
#define TEMPLATE_PUSH_type(TYPE) \
TYPE##_t,                       \
TEMPLATE_PUSH_Tconv_type(TYPE), \
TEMPLATE_PUSH_Tfunc_type(TYPE)



#endif /* TYPES_H */



