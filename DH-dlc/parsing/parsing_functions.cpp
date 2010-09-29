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

#include "FunctionHandlerNative.hpp"

#include "../math.hpp"
#include "../options.hpp"
#include "../SourceScanner.hpp"
#include "../types.hpp"

#include "../exceptions/FunctionException.hpp"
#include "../exceptions/UnknownFunctionException.hpp"

#include "../LevelObject/LevelObject.hpp"
#include "../LevelObject/LevelObjectName.hpp"
#include "../LevelObject/LevelObjectPointer.hpp"

#include "../types/binary.hpp"
#include "../types/int_t.hpp"
#include "../types/real_t.hpp"
#include "../types/string_t.hpp"

#include "../../common/string_funcs.hpp"



#define ADD_FUNCTION_BOTH_2(TYPE1, TYPE2, FUNC, NAME) \
FunctionHandler<TYPE1##_t> const * \
parse_function_##TYPE1##_##TYPE2##_##FUNC##_##NAME = \
FunctionHandler<TYPE1##_t>::add_function(#NAME, new FunctionHandlerNative<TYPE1##_t>(parse_function_DDL_##FUNC<TYPE2##_t>, parse_function_DHLX_##FUNC<TYPE2##_t>))

#define ADD_FUNCTION_BOTH(TYPE, FUNC, NAME) \
ADD_FUNCTION_BOTH_2(TYPE, TYPE, FUNC, NAME)

#define ADD_FUNCTION_BOTH_INT(FUNC, NAME) \
ADD_FUNCTION_BOTH(int_s,  FUNC, NAME); \
ADD_FUNCTION_BOTH(int,    FUNC, NAME); \
ADD_FUNCTION_BOTH(int_l,  FUNC, NAME); \
ADD_FUNCTION_BOTH(ubyte,  FUNC, NAME); \
ADD_FUNCTION_BOTH(sword,  FUNC, NAME); \
ADD_FUNCTION_BOTH(uword,  FUNC, NAME); \
ADD_FUNCTION_BOTH(sdword, FUNC, NAME); \
ADD_FUNCTION_BOTH(udword, FUNC, NAME)

#define ADD_FUNCTION_BOTH_NUMBER(FUNC, NAME) \
ADD_FUNCTION_BOTH_INT (FUNC, NAME); \
ADD_FUNCTION_BOTH_REAL(FUNC, NAME)

#define ADD_FUNCTION_BOTH_REAL(FUNC, NAME) \
ADD_FUNCTION_BOTH(real_s, FUNC, NAME); \
ADD_FUNCTION_BOTH(real,   FUNC, NAME); \
ADD_FUNCTION_BOTH(real_l, FUNC, NAME)

#define ADD_FUNCTION_BOTH_STRING(FUNC, NAME) \
ADD_FUNCTION_BOTH(string,    FUNC, NAME); \
ADD_FUNCTION_BOTH(string8,   FUNC, NAME); \
ADD_FUNCTION_BOTH(string16,  FUNC, NAME); \
ADD_FUNCTION_BOTH(string32,  FUNC, NAME); \
ADD_FUNCTION_BOTH(string80,  FUNC, NAME); \
ADD_FUNCTION_BOTH(string320, FUNC, NAME)


#define ADD_FUNCTION_DDL_2(TYPE1, TYPE2, FUNC, NAME) \
FunctionHandler<TYPE1##_t> const * \
parse_function_##TYPE1##_##TYPE2##_##FUNC##_##NAME = \
FunctionHandler<TYPE1##_t>::add_function(#NAME, new FunctionHandlerNative<TYPE1##_t>(parse_function_DDL_##FUNC<TYPE2##_t>))

#define ADD_FUNCTION_DDL(TYPE, FUNC, NAME) \
ADD_FUNCTION_DDL_2(TYPE, TYPE, FUNC, NAME)

#define ADD_FUNCTION_DDL_ALL(FUNC, NAME) \
ADD_FUNCTION_DDL_BOOL  (FUNC, NAME); \
ADD_FUNCTION_DDL_NUMBER(FUNC, NAME); \
ADD_FUNCTION_DDL_STRING(FUNC, NAME)

#define ADD_FUNCTION_DDL_BOOL(FUNC, NAME) \
ADD_FUNCTION_DDL(bool,   FUNC, NAME)

#define ADD_FUNCTION_DDL_INT(FUNC, NAME) \
ADD_FUNCTION_DDL(int_s,  FUNC, NAME); \
ADD_FUNCTION_DDL(int,    FUNC, NAME); \
ADD_FUNCTION_DDL(int_l,  FUNC, NAME); \
ADD_FUNCTION_DDL(ubyte,  FUNC, NAME); \
ADD_FUNCTION_DDL(sword,  FUNC, NAME); \
ADD_FUNCTION_DDL(uword,  FUNC, NAME); \
ADD_FUNCTION_DDL(sdword, FUNC, NAME); \
ADD_FUNCTION_DDL(udword, FUNC, NAME)

#define ADD_FUNCTION_DDL_NUMBER(FUNC, NAME) \
ADD_FUNCTION_DDL_INT (FUNC, NAME); \
ADD_FUNCTION_DDL_REAL(FUNC, NAME)

#define ADD_FUNCTION_DDL_REAL(FUNC, NAME) \
ADD_FUNCTION_DDL(real_s, FUNC, NAME); \
ADD_FUNCTION_DDL(real,   FUNC, NAME); \
ADD_FUNCTION_DDL(real_l, FUNC, NAME)

#define ADD_FUNCTION_DDL_STRING(FUNC, NAME) \
ADD_FUNCTION_DDL(string,    FUNC, NAME); \
ADD_FUNCTION_DDL(string8,   FUNC, NAME); \
ADD_FUNCTION_DDL(string16,  FUNC, NAME); \
ADD_FUNCTION_DDL(string32,  FUNC, NAME); \
ADD_FUNCTION_DDL(string80,  FUNC, NAME); \
ADD_FUNCTION_DDL(string320, FUNC, NAME)


#define ADD_FUNCTION_DHLX_2(TYPE1, TYPE2, FUNC, NAME) \
FunctionHandler<TYPE1##_t> const * \
parse_function_##TYPE1##_##TYPE2##_##FUNC##_##NAME = \
FunctionHandler<TYPE1##_t>::add_function(#NAME, new FunctionHandlerNative<TYPE1##_t>(parse_function_DHLX_##FUNC<TYPE2##_t>))

#define ADD_FUNCTION_DHLX(TYPE, FUNC, NAME) \
ADD_FUNCTION_DHLX_2(TYPE, TYPE, FUNC, NAME)

#define ADD_FUNCTION_DHLX_NUMBER(FUNC, NAME) \
ADD_FUNCTION_DHLX_INT (FUNC, NAME); \
ADD_FUNCTION_DHLX_REAL(FUNC, NAME)


#define DEFINE_FUNCTION_DDL_2(TYPE, FUNC) \
template <class T> \
TYPE parse_function_DDL_##FUNC(std::vector<std::string> const & args)

#define DEFINE_FUNCTION_DDL(FUNC) \
DEFINE_FUNCTION_DDL_2(T, FUNC)

#define DEFINE_FUNCTION_DHLX_2(TYPE, FUNC) \
template <class T> \
TYPE parse_function_DHLX_##FUNC(SourceScannerDHLX & sc)

#define DEFINE_FUNCTION_DHLX(FUNC) \
DEFINE_FUNCTION_DHLX_2(T, FUNC)



DEFINE_FUNCTION_DDL(cmp)
{
	if (args.size() != 2)
		throw FunctionException("2 args required");

	return convert<T, int_s_t>(int_s_t( cmp(parse<T>(args[0]), parse<T>(args[1])) ));
}
DEFINE_FUNCTION_DHLX(cmp)
{
	T arg0(parse<T>(sc));
	sc.get(SourceTokenDHLX::TT_OP_COMMA);
	T arg1(parse<T>(sc));

	return convert<T, int_s_t>(int_s_t( cmp(arg0, arg1) ));
}
ADD_FUNCTION_BOTH_NUMBER(cmp, cmp);
ADD_FUNCTION_BOTH_STRING(cmp, cmp);

DEFINE_FUNCTION_DDL_2(bool_t, bcmp)
{
	if (args.size() != 3)
		throw FunctionException("3 args required");

	int cmpResult = cmp(parse<T>(args[0]), parse<T>(args[2]));

	if (args[1] == cmp_name_eq()) return cmpResult == 0;
	if (args[1] == cmp_name_ne()) return cmpResult != 0;
	if (args[1] == cmp_name_gt()) return cmpResult >  0;
	if (args[1] == cmp_name_ge()) return cmpResult >= 0;
	if (args[1] == cmp_name_lt()) return cmpResult <  0;
	if (args[1] == cmp_name_le()) return cmpResult <= 0;

	throw FunctionException("unknown cmp:" + args[1]);
}
DEFINE_FUNCTION_DHLX_2(bool_t, bcmp)
{
	T arg0(parse<T>(sc));
	sc.get(SourceTokenDHLX::TT_OP_COMMA);
	std::string arg1(sc.get(SourceTokenDHLX::TT_IDENTIFIER).getData());
	sc.get(SourceTokenDHLX::TT_OP_COMMA);
	T arg2(parse<T>(sc));

	int cmpResult(cmp(arg0, arg2));

	if (arg1 == cmp_name_eq()) return cmpResult == 0;
	if (arg1 == cmp_name_ne()) return cmpResult != 0;
	if (arg1 == cmp_name_gt()) return cmpResult >  0;
	if (arg1 == cmp_name_ge()) return cmpResult >= 0;
	if (arg1 == cmp_name_lt()) return cmpResult <  0;
	if (arg1 == cmp_name_le()) return cmpResult <= 0;

	throw FunctionException("unknown cmp:" + arg1);
}
ADD_FUNCTION_BOTH_2(bool, int_s,  bcmp, cmpis);
ADD_FUNCTION_BOTH_2(bool, int,    bcmp, cmpi);
ADD_FUNCTION_BOTH_2(bool, int_l,  bcmp, cmpil);
ADD_FUNCTION_BOTH_2(bool, real_s, bcmp, cmpfs);
ADD_FUNCTION_BOTH_2(bool, real,   bcmp, cmpf);
ADD_FUNCTION_BOTH_2(bool, real_l, bcmp, cmpfl);
ADD_FUNCTION_BOTH_2(bool, real_l, bcmp, cmp);
ADD_FUNCTION_BOTH_2(bool, string, bcmp, cmps);

DEFINE_FUNCTION_DDL(distance)
{
	T x1, y1, x2, y2;

	if (args.size() == 2)
	{
		obj_t p1 = parse_obj(args[0], type_t::type_vertex());
		obj_t p2 = parse_obj(args[1], type_t::type_vertex());

		x1 = convert<T, obj_t>(p1->getObject(name_t(key_name_x())));
		y1 = convert<T, obj_t>(p1->getObject(name_t(key_name_y())));
		x2 = convert<T, obj_t>(p2->getObject(name_t(key_name_x())));
		y2 = convert<T, obj_t>(p2->getObject(name_t(key_name_y())));
	}
	else if (args.size() == 4)
	{
		x1 = parse<T>(args[0]);
		y1 = parse<T>(args[1]);
		x2 = parse<T>(args[2]);
		y2 = parse<T>(args[3]);
	}
	else
		throw FunctionException("2 or 4 args required");

	return hypot(x1 - x2, y1 - y2);
}
ADD_FUNCTION_DDL_NUMBER(distance, distance);

DEFINE_FUNCTION_DDL(facing)
{
	T x1, y1, x2, y2;

	/* ([VERTEX] p1, [VERTEX] p2) */
	if (args.size() == 2)
	{
		obj_t p1 = parse_obj(args[0], type_t::type_vertex());
		obj_t p2 = parse_obj(args[1], type_t::type_vertex());

		x1 = convert<T, obj_t>(p1->getObject(name_t(key_name_x())));
		x2 = convert<T, obj_t>(p2->getObject(name_t(key_name_x())));

		y1 = convert<T, obj_t>(p1->getObject(name_t(key_name_y())));
		y2 = convert<T, obj_t>(p2->getObject(name_t(key_name_y())));
	}
	/* ([float] x1, [float] y1, [float] x2, [float] y2) */
	else if (args.size() == 4)
	{
		x1 = parse<T>(args[0]);
		x2 = parse<T>(args[2]);

		y1 = parse<T>(args[1]);
		y2 = parse<T>(args[3]);
	}
	else
		throw FunctionException("2 or 4 args required");

	T length1;
	T length2 = y2 - y1;
	T length3 = x2 - x1;

	length1 = hypot(length2, length3);

	/* Not sure what else to do... */
	if (length1 == T(0)) return T(0);

	T angle1 = (T(180) / convert<T, real_t>(pi()));
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
ADD_FUNCTION_DDL_REAL(facing, facing);

DEFINE_FUNCTION_DDL(hypot)
{
	if (args.size() != 2)
		throw FunctionException("2 args required");

	return hypot(parse<T>(args[0]), parse<T>(args[1]));
}
ADD_FUNCTION_DDL_NUMBER(hypot, hypot);

DEFINE_FUNCTION_DDL(randomf)
{
	if (args.size() == 0)
	{
		return random<T>();
	}
	else if (args.size() == 1)
	{
		return random<T>(parse<T>(args[0]));
	}
	else if (args.size() == 2)
	{
		return random<T>(parse<T>(args[0]), parse<T>(args[1]));
	}
	else
	{
		throw FunctionException("0-2 args required");
	}
}
DEFINE_FUNCTION_DDL(randomi)
{
	if (args.size() == 1)
	{
		return random<T>(parse<T>(args[0]));
	}
	else if (args.size() == 2)
	{
		return random<T>(parse<T>(args[0]), parse<T>(args[1]));
	}
	else
	{
		throw FunctionException("1-2 args required");
	}
}
DEFINE_FUNCTION_DHLX(randomf)
{
	SourceTokenDHLX argTerm(sc.get());
	sc.unget(argTerm);

	if (argTerm.getType() == SourceTokenDHLX::TT_OP_PARENTHESIS_C)
	{
		return random<T>();
	}

	T arg0(parse<T>(sc));

	argTerm = sc.get();
	sc.unget(argTerm);

	if (argTerm.getType() == SourceTokenDHLX::TT_OP_PARENTHESIS_C)
	{
		return random<T>(arg0);
	}

	sc.get(SourceTokenDHLX::TT_OP_COMMA);

	T arg1(parse<T>(sc));

	return random<T>(arg0, arg1);
	
}
DEFINE_FUNCTION_DHLX(randomi)
{
	SourceTokenDHLX argTerm(sc.get());
	sc.unget(argTerm);

	T arg0(parse<T>(sc));

	argTerm = sc.get();
	sc.unget(argTerm);

	if (argTerm.getType() == SourceTokenDHLX::TT_OP_PARENTHESIS_C)
	{
		return random<T>(arg0);
	}

	sc.get(SourceTokenDHLX::TT_OP_COMMA);

	T arg1(parse<T>(sc));

	return random<T>(arg0, arg1);
	
}
ADD_FUNCTION_BOTH_INT(randomi, random);
ADD_FUNCTION_BOTH_REAL(randomf, random);



