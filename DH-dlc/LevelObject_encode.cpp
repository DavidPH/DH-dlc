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
	2010/02/28 - Update for lo_type enum.
*/

#include "LevelObject.hpp"

#include <climits>
#include <sstream>

#include "global_object.hpp"
#include "LevelObjectName.hpp"
#include "lo_types.hpp"
#include "options.hpp"
#include "types.hpp"
#include "../common/foreach.hpp"
#include "exceptions/InvalidTypeException.hpp"
#include "types/int_t.hpp"
#include "types/real_t.hpp"
#include "types/string_t.hpp"



std::string LevelObject::encode(bool topLevel)
{
	switch (data.getType())
	{
		case any_t::BOOL_T:

		case any_t::INT_S_T:
		case any_t::INT_T:
		case any_t::INT_L_T:

		case any_t::REAL_S_T:
		case any_t::REAL_T:
		case any_t::REAL_L_T:
			if (topLevel)
				return "";

			return make_string(data);

		case any_t::OBJMAP_T:
		{
			// TODO: LO_TYPE_INLINE handling.
			if (get_lo_type(type) != LO_TYPE_OBJECT)
				return "";

			std::ostringstream oss;

			if (topLevel)
			{
				oss << type << " // " << get_object_index(this) << "\n{\n";

				FOREACH_T(objmap_t, it, data.getObjMap())
					oss << '\t' << it->first << '=' << it->second->encode(false) << ";\n";

				oss << "}";
			}
			else
			{
				oss << get_object_index(this);
			}

			return oss.str();
		}

		case any_t::STRING_T:
		{
			if (topLevel)
				return "";

			std::ostringstream oss;
			std::istringstream iss(data.getString().makeString());

			oss.put('"');

			for (int c = iss.get(); c != -1; c = iss.get())
			{
				switch (c)
				{
					case '\\':
					case '"':
						oss.put('\\');
						oss.put(c);
						break;

					default:
						oss.put(c);
						break;
				}
			}

			oss.put('"');

			return oss.str();
		}

		default:
			return "";
	}
}
std::string LevelObject::encode(LevelObjectBinaryType binType)
{
	const char* NULL_STRING = "-\0\0\0\0\0\0\0";

	std::string binReturn;

	if (
		data.getType() == any_t::BOOL_T   ||
		data.getType() == any_t::REAL_S_T || data.getType() == any_t::REAL_T || data.getType() == any_t::REAL_L_T ||
		data.getType() == any_t::INT_S_T  || data.getType() == any_t::INT_T  || data.getType() == any_t::INT_L_T)
	{
		int_s_t intValue = to_int_s(data);

		// No need to handle S and U different, since -1 should always output 0xFFFF.
		if (binType == BIN_OBJECT || binType == USHORT || binType == SSHORT)
		{
			binReturn.resize(2);

			binReturn[0] = (intValue     ) & 0xFF;
			binReturn[1] = (intValue >> 8) & 0xFF;

			return binReturn;
		}

		if (binType == UBYTE)
		{
			binReturn.resize(1);

			binReturn[0] = intValue & 0xFF;

			return binReturn;
		}

		throw InvalidTypeException("cannot encode BOOL/FLOAT/INT");
	}

	if (data.getType() == any_t::STRING_T)
	{
		if (binType == BIN_STRING)
		{
			binReturn = make_string(data);

			binReturn.resize(8);

			return binReturn;
		}

		throw InvalidTypeException("cannot encode STRING");
	}

	if (data.getType() != any_t::OBJMAP_T)
		throw InvalidTypeException("internal error:unknown enum value for 'data.getType()'");

	#define CHECKFLAG(FLAG, VALUE)				\
	if (hasObject(FLAG) && to_bool(getObject(FLAG)))	\
		flags |= VALUE;					\
	else (void) 0

	#define CHECKFLAGNOT(FLAG, VALUE)			\
	if (!hasObject(FLAG) || !to_bool(getObject(FLAG)))	\
		flags |= VALUE;					\
	else (void) 0

	#define CHECKVALUE(VALUE, TYPE, DEFAULT, LENGTH)	\
	if (hasObject(VALUE))					\
		binReturn += getObject(VALUE)->encode(TYPE);	\
	else							\
		binReturn.append(DEFAULT, LENGTH)

	if (binType == BIN_OBJECT)
	{
		binReturn.resize(2);

		int index = get_object_index(this);

		binReturn[0] = (index     ) & 0xFF;
		binReturn[1] = (index >> 8) & 0xFF;

		return binReturn;	
	}

	if (binType == LINEDEF)
	{
		if (option_out_binary_hexen)
		{
			binReturn.reserve(16);

			binReturn += getObject("v1")->encode(BIN_OBJECT); // 0-1
			binReturn += getObject("v2")->encode(BIN_OBJECT); // 2-3

			// 4-5
			if (hasObject("flags"))
				binReturn += getObject("flags")->encode(USHORT);
			else
			{
				int flags = 0;

				CHECKFLAG("blocking",      0x0001);
				CHECKFLAG("blockmonsters", 0x0002);
				CHECKFLAG("twosided",      0x0004);
				CHECKFLAG("dontpegtop",    0x0008);
				CHECKFLAG("dontpegbottom", 0x0010);
				CHECKFLAG("secret",        0x0020);
				CHECKFLAG("blocksound",    0x0040);
				CHECKFLAG("dontdraw",      0x0080);
				CHECKFLAG("mapped",        0x0100);
				CHECKFLAG("repeatspecial", 0x0200);
				CHECKFLAG("monsteruse",    0x2000);
				CHECKFLAG("blocking",      0x4000);

				if (hasObject("playeruse") && to_bool(getObject("playeruse")))
					if (hasObject("playercross") && to_bool(getObject("playercross")))
						flags |= 0x1800;
					else
						flags |= 0x0400;
				else if (hasObject("monstercross") && to_bool(getObject("monstercross")))
					flags |= 0x0800;
				else if (hasObject("impact") && to_bool(getObject("impact")))
					flags |= 0x0C00;
				else if (hasObject("playerpush") && to_bool(getObject("playerpush")))
					flags |= 0x1000;
				else if (hasObject("missilecross") && to_bool(getObject("missilecross")))
					flags |= 0x1400;

				binReturn += (flags     ) & 0xFF;
				binReturn += (flags >> 8) & 0xFF;
			}

			CHECKVALUE("special",   UBYTE,      "\x00",     1); // 06
			CHECKVALUE("arg0",      UBYTE,      "\x00",     1); // 07
			CHECKVALUE("arg1",      UBYTE,      "\x00",     1); // 08
			CHECKVALUE("arg2",      UBYTE,      "\x00",     1); // 09
			CHECKVALUE("arg3",      UBYTE,      "\x00",     1); // 10
			CHECKVALUE("arg4",      UBYTE,      "\x00",     1); // 11
			CHECKVALUE("sidefront", BIN_OBJECT, "\xFF\xFF", 2); // 12-13
			CHECKVALUE("sideback",  BIN_OBJECT, "\xFF\xFF", 2); // 14-15

			int_s_t flags = 0;
			if (hasObject("moreflags"))
				flags = to_int_s(getObject("moreflags"));
			else
			{
				CHECKFLAG("zoneboundry",      0x01);
				CHECKFLAG("jumpover",         0x02);
				CHECKFLAG("blockfloaters",    0x04);
				CHECKFLAG("clipmidtex",       0x08);
				CHECKFLAG("wrapmidtex",       0x10);
				CHECKFLAG("midtex3d",         0x20);
				CHECKFLAG("checkswitchrange", 0x40);
				CHECKFLAG("firstsideonly",    0x80);
			}

			int_s_t id = 0;
			if (hasObject("id"))
				id = to_int_s(getObject("id"));

			int flagArg = -1;
			int id1Arg  = -1;
			int id2Arg  = -1; // It's a ZDoom thing, but useful.

			switch (binReturn[6] & 0xFF)
			{
				case 0:   // no special
					if (id || flags)
					{
						binReturn[6] = 121;
						flagArg = 1;
						id1Arg  = 0;
						id2Arg  = 4;
					}
					break;

				case 1:   // PolyObj_StartLine
					id1Arg = 3;
					break;

				case 5:   // PolyObj_ExplicitLine
					id1Arg = 4;
					break;

				case 121: // Line_SetIdentification
					flagArg = 1;
					id1Arg  = 0;
					id2Arg  = 4;
					break;

				case 208: // TranslucentLine
					flagArg = 3;
					id1Arg  = 0;
					break;

				case 215: // Teleport_Line
					id1Arg = 0;
					break;

				case 222: // Scroll_Texture_Model
					id1Arg = 0;
					break;
			}

			if (flagArg != -1)
				binReturn[7+flagArg] = flags & 0xFF;

			if (id1Arg != -1 && hasObject("id"))
				binReturn[7+id1Arg] = id & 0xFF;

			if (id2Arg != -1 && hasObject("id"))
				binReturn[7+id2Arg] = (id >> 8) & 0xFF;

			return binReturn;
		}
		else
		{
			binReturn.reserve(14);

			binReturn += getObject("v1")->encode(BIN_OBJECT); // 0-1
			binReturn += getObject("v2")->encode(BIN_OBJECT); // 2-3

			// 4-5
			if (hasObject("flags"))
				binReturn += getObject("flags")->encode(USHORT);
			else
			{
				int flags = 0;

				CHECKFLAG("blocking",      0x0001);
				CHECKFLAG("blockmonsters", 0x0002);
				CHECKFLAG("twosided",      0x0004);
				CHECKFLAG("dontpegtop",    0x0008);
				CHECKFLAG("dontpegbottom", 0x0010);
				CHECKFLAG("secret",        0x0020);
				CHECKFLAG("blocksound",    0x0040);
				CHECKFLAG("dontdraw",      0x0080);
				CHECKFLAG("mapped",        0x0100);

				binReturn += (flags     ) & 0xFF;
				binReturn += (flags >> 8) & 0xFF;
			}

			CHECKVALUE("id",        USHORT,     "\x00\x00", 2); // 06-07
			CHECKVALUE("special",   USHORT,     "\x00\x00", 2); // 08-09
			CHECKVALUE("sidefront", BIN_OBJECT, "\xFF\xFF", 2); // 10-11
			CHECKVALUE("sideback",  BIN_OBJECT, "\xFF\xFF", 2); // 12-13

			return binReturn;
		}
	}

	if (binType == SECTOR)
	{
		binReturn.reserve(26);

		CHECKVALUE("heightfloor",    SSHORT,     "\x00\x00",  2); // 00-01
		CHECKVALUE("heightceiling",  SSHORT,     "\x00\x00",  2); // 02-03
		CHECKVALUE("texturefloor",   BIN_STRING, NULL_STRING, 8); // 04-11
		CHECKVALUE("textureceiling", BIN_STRING, NULL_STRING, 8); // 12-19
		CHECKVALUE("lightlevel",     USHORT,     "\xA0\x00",  2); // 20-21
		CHECKVALUE("special",        USHORT,     "\x00\x00",  2); // 22-23
		CHECKVALUE("id",             USHORT,     "\x00\x00",  2); // 24-25

		return binReturn;
	}

	if (binType == SIDEDEF)
	{
		binReturn.reserve(30);

		CHECKVALUE("offsetx",       SSHORT,     "\x00\x00",  2); // 00-01
		CHECKVALUE("offsety",       SSHORT,     "\x00\x00",  2); // 02-03
		CHECKVALUE("texturetop",    BIN_STRING, NULL_STRING, 8); // 04-11
		CHECKVALUE("texturebottom", BIN_STRING, NULL_STRING, 8); // 12-19
		CHECKVALUE("texturemiddle", BIN_STRING, NULL_STRING, 8); // 20-27
		binReturn += getObject("sector")->encode(BIN_OBJECT);    // 28-29

		return binReturn;
	}

	if (binType == THING)
	{
		if (option_out_binary_hexen)
		{
			binReturn.reserve(20);

			CHECKVALUE("id",     USHORT, "\x00\x00", 2);    // 00-01
			binReturn += getObject("x")->encode(SSHORT);    // 02-03
			binReturn += getObject("y")->encode(SSHORT);    // 04-05
			CHECKVALUE("height", USHORT, "\x00\x00", 2);    // 06-07
			CHECKVALUE("angle",  USHORT, "\x00\x00", 2);    // 08-09
			binReturn += getObject("type")->encode(SSHORT); // 10-11

			// 12-13
			if (hasObject("flags"))
				binReturn += getObject("flags")->encode(USHORT);
			if (option_out_binary_hexen)
			{
				int flags = 0;

				CHECKFLAG("skill1",      0x0001);
				CHECKFLAG("skill2",      0x0001);
				CHECKFLAG("skill3",      0x0002);
				CHECKFLAG("skill4",      0x0004);
				CHECKFLAG("skill5",      0x0004);
				CHECKFLAG("ambush",      0x0008);
				CHECKFLAG("dormant",     0x0010);
				CHECKFLAG("single",      0x0100);
				CHECKFLAG("coop",        0x0200);
				CHECKFLAG("dm",          0x0400);
				CHECKFLAG("translucent", 0x0800);
				CHECKFLAG("invisible",   0x1000);
				CHECKFLAG("friend",      0x2000);
				CHECKFLAG("standing",    0x4000);

				binReturn += (flags >> 8) & 0xFF;
				binReturn += (flags     ) & 0xFF;
			}

			CHECKVALUE("special", UBYTE, "\x00", 1); // 14
			CHECKVALUE("arg0",    UBYTE, "\x00", 1); // 15
			CHECKVALUE("arg1",    UBYTE, "\x00", 1); // 16
			CHECKVALUE("arg2",    UBYTE, "\x00", 1); // 17
			CHECKVALUE("arg3",    UBYTE, "\x00", 1); // 18
			CHECKVALUE("arg4",    UBYTE, "\x00", 1); // 19

			return binReturn;
		}
		else
		{
			binReturn.reserve(10);

			binReturn += getObject("x")->encode(SSHORT);    // 0-1
			binReturn += getObject("y")->encode(SSHORT);    // 2-3
			CHECKVALUE("angle", USHORT, "\x00\x00", 2);     // 4-5
			binReturn += getObject("type")->encode(SSHORT); // 6-7

			// 8-9
			if (hasObject("flags"))
				binReturn += getObject("flags")->encode(USHORT);
			else if (option_out_binary_strife)
			{
				int flags = 0;

				CHECKFLAG   ("skill1",      0x0001);
				CHECKFLAG   ("skill2",      0x0001);
				CHECKFLAG   ("skill3",      0x0002);
				CHECKFLAG   ("skill4",      0x0004);
				CHECKFLAG   ("skill5",      0x0004);
				CHECKFLAG   ("standing",    0x0008);
				CHECKFLAGNOT("single",      0x0010);
				CHECKFLAG   ("ambush",      0x0020);
				CHECKFLAG   ("friendly",    0x0040);
				CHECKFLAG   ("translucent", 0x0100);
				CHECKFLAG   ("invisible",   0x0200);

				binReturn += (flags >> 8) & 0xFF;
				binReturn += (flags     ) & 0xFF;
			}
			else
			{
				int flags = 0;

				CHECKFLAG   ("skill1",      0x0001);
				CHECKFLAG   ("skill2",      0x0001);
				CHECKFLAG   ("skill3",      0x0002);
				CHECKFLAG   ("skill4",      0x0004);
				CHECKFLAG   ("skill5",      0x0004);
				CHECKFLAG   ("ambush",      0x0008);
				CHECKFLAGNOT("single",      0x0010);
				CHECKFLAGNOT("dm",          0x0020);
				CHECKFLAGNOT("coop",        0x0040);
				CHECKFLAG   ("friend",      0x0080);

				binReturn += (flags >> 8) & 0xFF;
				binReturn += (flags     ) & 0xFF;
			}

			return binReturn;
		}
	}

	if (binType == VERTEX)
	{
		binReturn.reserve(4);

		binReturn += getObject("x")->encode(SSHORT);
		binReturn += getObject("y")->encode(SSHORT);

		return binReturn;
	}

	throw InvalidTypeException("internal error:unknown enum value for 'binType'");

	#undef CHECKFLAG
	#undef CHECKFLAGNOT
	#undef CHECKVALUE
}



