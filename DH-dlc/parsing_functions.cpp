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
	2010/06/24 - Replaced most of the macros with templates.
*/

#include "parsing.hpp"

#include "math.hpp"
#include "options.hpp"
#include "types.hpp"

#include "exceptions/FunctionException.hpp"
#include "exceptions/UnknownFunctionException.hpp"

#include "LevelObject/LevelObject.hpp"
#include "LevelObject/LevelObjectName.hpp"
#include "LevelObject/LevelObjectPointer.hpp"

#include "types/binary.hpp"
#include "types/int_t.hpp"
#include "types/real_t.hpp"
#include "types/string_t.hpp"

#include "../common/string_funcs.hpp"



static std::map< std::string, std::map<std::string, std::string> > custom_function_map;

typedef bool_t      (*bool_func_t)      (std::vector<std::string> const &);

typedef int_s_t     (*int_s_func_t)     (std::vector<std::string> const &);
typedef int_t       (*int_func_t)       (std::vector<std::string> const &);
typedef int_l_t     (*int_l_func_t)     (std::vector<std::string> const &);

typedef real_s_t    (*real_s_func_t)    (std::vector<std::string> const &);
typedef real_t      (*real_func_t)      (std::vector<std::string> const &);
typedef real_l_t    (*real_l_func_t)    (std::vector<std::string> const &);

typedef string_t    (*string_func_t)    (std::vector<std::string> const &);
typedef string8_t   (*string8_func_t)   (std::vector<std::string> const &);
typedef string16_t  (*string16_func_t)  (std::vector<std::string> const &);
typedef string32_t  (*string32_func_t)  (std::vector<std::string> const &);
typedef string80_t  (*string80_func_t)  (std::vector<std::string> const &);
typedef string320_t (*string320_func_t) (std::vector<std::string> const &);

typedef ubyte_t     (*ubyte_func_t)     (std::vector<std::string> const &);
typedef sword_t     (*sword_func_t)     (std::vector<std::string> const &);
typedef uword_t     (*uword_func_t)     (std::vector<std::string> const &);
typedef sdword_t    (*sdword_func_t)    (std::vector<std::string> const &);
typedef udword_t    (*udword_func_t)    (std::vector<std::string> const &);

std::map<std::string, bool_func_t>      bool_func_map;

std::map<std::string, int_s_func_t>     int_s_func_map;
std::map<std::string, int_func_t>       int_func_map;
std::map<std::string, int_l_func_t>     int_l_func_map;

std::map<std::string, real_s_func_t>    real_s_func_map;
std::map<std::string, real_func_t>      real_func_map;
std::map<std::string, real_l_func_t>    real_l_func_map;

std::map<std::string, string_func_t>    string_func_map;
std::map<std::string, string8_func_t>   string8_func_map;
std::map<std::string, string16_func_t>  string16_func_map;
std::map<std::string, string32_func_t>  string32_func_map;
std::map<std::string, string80_func_t>  string80_func_map;
std::map<std::string, string320_func_t> string320_func_map;

std::map<std::string, ubyte_func_t>     ubyte_func_map;
std::map<std::string, sword_func_t>     sword_func_map;
std::map<std::string, uword_func_t>     uword_func_map;
std::map<std::string, sdword_func_t>    sdword_func_map;
std::map<std::string, udword_func_t>    udword_func_map;



template<class FUNC_T>
class parsing_FunctionAdder
{
	public:
	parsing_FunctionAdder(std::map<std::string, FUNC_T> & func_map, std::string const & name, FUNC_T func)
	{
		func_map[        name ] = func;
		func_map[toupper(name)] = func;
		func_map[tolower(name)] = func;
	}
};

#define ADD_FUNC(TYPE, FUNC) \
ADD_FUNC2(TYPE, FUNC, FUNC)

#define ADD_FUNC2(TYPE, FUNC, NAME) \
static parsing_FunctionAdder<TYPE##_func_t> \
parsing_FunctionAdder_##TYPE##_##FUNC##_##NAME \
(TYPE##_func_map, #NAME, &parse_##TYPE##_function_##FUNC)

#define ADD_FUNCTION2(TYPE1, TYPE2, FUNC, NAME) \
static parsing_FunctionAdder<TYPE1##_func_t> \
parsing_FunctionAdder_##TYPE1##_##TYPE2##_##FUNC##_##NAME \
(TYPE1##_func_map, #NAME, &parse_function_##FUNC<TYPE2##_t, to_##TYPE2, parse_##TYPE2>)

#define ADD_FUNCTION(TYPE, FUNC, NAME) \
ADD_FUNCTION2(TYPE, TYPE, FUNC, NAME)

#define ADD_FUNC_ALL(FUNC, NAME) \
ADD_FUNC_BOOL  (FUNC, NAME); \
ADD_FUNC_NUMBER(FUNC, NAME); \
ADD_FUNC_STRING(FUNC, NAME)

#define ADD_FUNC_BOOL(FUNC, NAME) \
ADD_FUNCTION(bool,   FUNC, NAME)

#define ADD_FUNC_INT(FUNC, NAME) \
ADD_FUNCTION(int_s,  FUNC, NAME); \
ADD_FUNCTION(int,    FUNC, NAME); \
ADD_FUNCTION(int_l,  FUNC, NAME); \
ADD_FUNCTION(ubyte,  FUNC, NAME); \
ADD_FUNCTION(sword,  FUNC, NAME); \
ADD_FUNCTION(uword,  FUNC, NAME); \
ADD_FUNCTION(sdword, FUNC, NAME); \
ADD_FUNCTION(udword, FUNC, NAME)

#define ADD_FUNC_NUMBER(FUNC, NAME) \
ADD_FUNC_INT (FUNC, NAME); \
ADD_FUNC_REAL(FUNC, NAME)

#define ADD_FUNC_REAL(FUNC, NAME) \
ADD_FUNCTION(real_s, FUNC, NAME); \
ADD_FUNCTION(real,   FUNC, NAME); \
ADD_FUNCTION(real_l, FUNC, NAME)

#define ADD_FUNC_STRING(FUNC, NAME) \
ADD_FUNCTION(string,    FUNC, NAME); \
ADD_FUNCTION(string8,   FUNC, NAME); \
ADD_FUNCTION(string16,  FUNC, NAME); \
ADD_FUNCTION(string32,  FUNC, NAME); \
ADD_FUNCTION(string80,  FUNC, NAME); \
ADD_FUNCTION(string320, FUNC, NAME)

#define DEFINE_FUNCTION2(TYPE, FUNC) \
template <class T, T(Tconv)(any_t const &), T(Tparse)(std::string const &)> \
TYPE parse_function_##FUNC(std::vector<std::string> const & args)

#define DEFINE_FUNCTION(FUNC) \
DEFINE_FUNCTION2(T, FUNC)



DEFINE_FUNCTION(cmp)
{
	if (args.size() != 2)
		throw FunctionException("2 args required");

	return Tconv(int_s_t( cmp(Tparse(args[0]), Tparse(args[1])) ));
}
ADD_FUNC_NUMBER(cmp, cmp);
ADD_FUNC_STRING(cmp, cmp);

DEFINE_FUNCTION2(bool_t, bcmp)
{
	if (args.size() != 3)
		throw FunctionException("3 args required");

	int cmpResult = cmp(Tparse(args[0]), Tparse(args[2]));

	if (args[1] == cmp_name_eq()) return cmpResult == 0;
	if (args[1] == cmp_name_ne()) return cmpResult != 0;
	if (args[1] == cmp_name_gt()) return cmpResult >  0;
	if (args[1] == cmp_name_ge()) return cmpResult >= 0;
	if (args[1] == cmp_name_lt()) return cmpResult <  0;
	if (args[1] == cmp_name_le()) return cmpResult <= 0;

	throw FunctionException("unknown cmp:" + args[1]);
}
ADD_FUNCTION2(bool, int_s,  bcmp, cmpis);
ADD_FUNCTION2(bool, int,    bcmp, cmpi);
ADD_FUNCTION2(bool, int_l,  bcmp, cmpil);
ADD_FUNCTION2(bool, real_s, bcmp, cmpfs);
ADD_FUNCTION2(bool, real,   bcmp, cmpf);
ADD_FUNCTION2(bool, real_l, bcmp, cmpfl);
ADD_FUNCTION2(bool, real_l, bcmp, cmp);
ADD_FUNCTION2(bool, string, bcmp, cmps);

DEFINE_FUNCTION(distance)
{
	T x1, y1, x2, y2;

	if (args.size() == 2)
	{
		obj_t p1 = parse_obj(args[0], type_t::type_vertex());
		obj_t p2 = parse_obj(args[1], type_t::type_vertex());

		x1 = Tconv(p1->getObject(name_t(key_name_x())));
		y1 = Tconv(p1->getObject(name_t(key_name_y())));
		x2 = Tconv(p2->getObject(name_t(key_name_x())));
		y2 = Tconv(p2->getObject(name_t(key_name_y())));
	}
	else if (args.size() == 4)
	{
		x1 = Tparse(args[0]);
		y1 = Tparse(args[1]);
		x2 = Tparse(args[2]);
		y2 = Tparse(args[3]);
	}
	else
		throw FunctionException("2 or 4 args required");

	return hypot(x1 - x2, y1 - y2);
}
ADD_FUNC_NUMBER(distance, distance);

DEFINE_FUNCTION(facing)
{
	T x1, y1, x2, y2;

	/* ([VERTEX] p1, [VERTEX] p2) */
	if (args.size() == 2)
	{
		obj_t p1 = parse_obj(args[0], type_t::type_vertex());
		obj_t p2 = parse_obj(args[1], type_t::type_vertex());

		x1 = Tconv(p1->getObject(name_t(key_name_x())));
		x2 = Tconv(p2->getObject(name_t(key_name_x())));

		y1 = Tconv(p1->getObject(name_t(key_name_y())));
		y2 = Tconv(p2->getObject(name_t(key_name_y())));
	}
	/* ([float] x1, [float] y1, [float] x2, [float] y2) */
	else if (args.size() == 4)
	{
		x1 = Tparse(args[0]);
		x2 = Tparse(args[2]);

		y1 = Tparse(args[1]);
		y2 = Tparse(args[3]);
	}
	else
		throw FunctionException("2 or 4 args required");

	T length1;
	T length2 = y2 - y1;
	T length3 = x2 - x1;

	length1 = hypot(length2, length3);

	/* Not sure what else to do... */
	if (length1 == T(0)) return T(0);

	T angle1 = (T(180) / Tconv(pi()));
	T angle2 = asin(T(length2 / length1)) * angle1;
	T angle3 = asin(T(length3 / length1)) * angle1;

	/* up-right */
	if (angle2 >= T(0) && angle3 >= T(0))
		return angle2;

	/* up-left */
	else if (angle2 >= T(0) && angle3 < T(0))
		return (T(90) - angle2) + T(90);

	/* down right is +270, left is +180 and +90 for negative */
	else
		return angle3 + T(270);
}
ADD_FUNC_REAL(facing, facing);

DEFINE_FUNCTION(hypot)
{
	if (args.size() != 2)
		throw FunctionException("2 args required");

	return hypot(Tparse(args[0]), Tparse(args[1]));
}
ADD_FUNC_NUMBER(hypot, hypot);

#define FUNCTION_RANDOM(TYPE) \
TYPE##_t parse_##TYPE##_function_random(std::vector<std::string> const & args) \
{ \
	if (args.size() != 2) \
		throw FunctionException("2 args required"); \
	\
	return random_##TYPE(parse_##TYPE(args[0]), parse_##TYPE(args[1])); \
} \
ADD_FUNC(TYPE, random)
FUNCTION_RANDOM(int_s);
FUNCTION_RANDOM(int);
FUNCTION_RANDOM(int_l);
FUNCTION_RANDOM(real_s);
FUNCTION_RANDOM(real);
FUNCTION_RANDOM(real_l);
FUNCTION_RANDOM(sword);
FUNCTION_RANDOM(ubyte);
FUNCTION_RANDOM(uword);



void add_function(std::string const & type, std::string const & name, std::string const & data)
{
	custom_function_map[type][name] = data;
}

std::string const & get_function(std::string const & type, std::string const & name)
{
	return custom_function_map[type][name];
}

bool has_function(std::string const & type, std::string const & name)
{
	std::map< std::string, std::map<std::string, std::string> >::iterator itType = custom_function_map.find(type);

	if (itType == custom_function_map.end()) return false;

	std::map<std::string, std::string>::iterator itName = itType->second.find(name);

	if (itName == itType->second.end()) return false;

	return true;
}



#define PARSE_FUNCTION(TYPE, TYPE_NAME) \
TYPE##_t parse_##TYPE##_function(std::string const & function, SourceScannerDHLX & sc) \
{ \
	throw UnknownFunctionException(function); \
} \
TYPE##_t parse_##TYPE##_function(std::string const & opString, std::vector<std::string> const & args) \
{ \
	return parse__function<TYPE##_t, TYPE##_func_t, to_##TYPE>(opString, args, TYPE##_func_map, TYPE_NAME); \
}
template <class T, class Tfunc, T(Tconv)(any_t const &)>
T parse__function(std::string const & opString, std::vector<std::string> const & args, std::map<std::string, Tfunc> & TfuncMap, char const * Tname)
{
	if (has_function(Tname, opString))
	{
		obj_t funcObj = LevelObject::create();

		obj_t returnType = LevelObject::create(type_t::type_string(), string_t(Tname));
		funcObj->addObject(name_t::name_return_type, returnType);

		obj_t argcObj = LevelObject::create(type_t::type_shortint(), int_s_t(args.size()));
		funcObj->addObject(name_t(key_name_argc()), argcObj);

		for (int_s_t index = 0; (size_t)index < args.size(); ++index)
		{
			obj_t argObj = LevelObject::create(type_t::type_string(), string_t(args[index]));

			name_t argName(std::string(key_name_arg()) + make_string(index));

			funcObj->addObject(argName, argObj);
		}

		funcObj->addData(get_function(Tname, opString));

		obj_t returnValue = funcObj->getObject(name_t::name_return_value);
		return Tconv(returnValue);
	}

	Tfunc func = TfuncMap[opString];

	if (func != NULL) return (*func)(args);

	throw UnknownFunctionException(opString);
}

PARSE_FUNCTION(bool, type_name_bool());

PARSE_FUNCTION(int_s, type_name_shortint());
PARSE_FUNCTION(int,   type_name_int());
PARSE_FUNCTION(int_l, type_name_longint());

PARSE_FUNCTION(real_s, type_name_shortfloat());
PARSE_FUNCTION(real,   type_name_float());
PARSE_FUNCTION(real_l, type_name_longfloat());

PARSE_FUNCTION(string,    type_name_string());
PARSE_FUNCTION(string8,   type_name_string8());
PARSE_FUNCTION(string16,  type_name_string16());
PARSE_FUNCTION(string32,  type_name_string32());
PARSE_FUNCTION(string80,  type_name_string80());
PARSE_FUNCTION(string320, type_name_string320());

PARSE_FUNCTION(ubyte,  type_name_ubyte());
PARSE_FUNCTION(sword,  type_name_sword());
PARSE_FUNCTION(uword,  type_name_uword());
PARSE_FUNCTION(sdword, type_name_sdword());
PARSE_FUNCTION(udword, type_name_udword());



