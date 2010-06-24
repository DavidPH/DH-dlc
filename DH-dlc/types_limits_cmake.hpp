/*
    Copyright 2009 David Hill

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
	Defines some limits for types.hpp. This file is meant to be configured
	by CMake.
*/

#ifndef TYPES_LIMITS_H
#define TYPES_LIMITS_H



typedef @SINT_BIGGEST@ sint_biggest_t;
typedef @UINT_BIGGEST@ uint_biggest_t;

#define SINT_BIGGEST_MAX ((sint_biggest_t)(@SINT_BIGGEST_MAX@))
#define SINT_BIGGEST_MIN ((sint_biggest_t)(@SINT_BIGGEST_MIN@))
#define UINT_BIGGEST_MAX                  (@UINT_BIGGEST_MAX@)

typedef @FLOAT_BIGGEST@ float_biggest_t;

#define FLOAT_BIGGEST_MAX (@FLOAT_BIGGEST_MAX@)
#define FLOAT_BIGGEST_MIN (@FLOAT_BIGGEST_MIN@)

#define POINTER_BIT (@POINTER_BIT@)

#define SPOINTER_MAX ((int_s_t)(@SPOINTER_MAX@))
#define SPOINTER_MIN ((int_s_t)(@SPOINTER_MIN@))
#define UPOINTER_MAX           (@UPOINTER_MAX@)



#define PATH_SEP @PATH_SEP@



#endif /* TYPES_LIMITS_H */



