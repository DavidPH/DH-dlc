//-----------------------------------------------------------------------------
//
// Copyright(C) 2009-2012 David Hill
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------------
//
// Declares fundamental internal types. Also declares the functions for
// converting between one another.
//
//-----------------------------------------------------------------------------

#ifndef HPP_types_
#define HPP_types_

#include <cfloat>
#include <climits>
#include <sstream>
#include <string>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define  int_s_t_MAX  LLONG_MAX
#define  int_s_t_MIN  LLONG_MIN
#define uint_s_t_MAX ULLONG_MAX

#define  int_t_MAX  int_l_t_MAX
#define  int_t_MIN  int_l_t_MIN
#define uint_t_MAX uint_l_t_MAX

#if USE_GMPLIB
#define  int_l_t_MAX 0
#define  int_l_t_MIN 0
#define uint_l_t_MAX 0
#else
#define  int_l_t_MAX  LLONG_MAX
#define  int_l_t_MIN  LLONG_MIN
#define uint_l_t_MAX ULLONG_MAX
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
#define  real_l_t_MAX   LDBL_MAX
#define  real_l_t_MIN (-LDBL_MAX)
#define ureal_l_t_MAX   LDBL_MAX
#endif


//----------------------------------------------------------------------------|
// Types                                                                      |
//

class LevelObject;
class LevelObjectData;
class LevelObjectMap;
class LevelObjectName;
class LevelObjectType;

template<typename T> class CounterPointer;

template <size_t S>
class stringf_t;

typedef LevelObjectData    any_t;
typedef bool               bool_t;
typedef long long int      int_s_t;
class                      int_t;
class                      int_l_t;
typedef CounterPointer<LevelObject> obj_t;
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


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

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



template<typename Tto, typename Tfrom>
Tto convert(Tfrom const &);

#endif//HPP_types_

