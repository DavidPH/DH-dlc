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
	2010/06/24 - Update for explicit name_t constructors.
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



static const name_t name_ambush          ("ambush");
static const name_t name_angle           ("angle");
static const name_t name_arg0            ("arg0");
static const name_t name_arg1            ("arg1");
static const name_t name_arg2            ("arg2");
static const name_t name_arg3            ("arg3");
static const name_t name_arg4            ("arg4");
static const name_t name_blockfloaters   ("blockfloaters");
static const name_t name_blocking        ("blocking");
static const name_t name_blockmonsters   ("blockmonsters");
static const name_t name_blocksound      ("blocksound");
static const name_t name_checkswitchrange("checkswitchrange");
static const name_t name_clipmidtex      ("clipmidtext");
static const name_t name_coop            ("coop");
static const name_t name_dm              ("dm");
static const name_t name_dontdraw        ("dontdraw");
static const name_t name_dontpegbottom   ("dontpegbottom");
static const name_t name_dontpegtop      ("dontpegtop");
static const name_t name_dormant         ("dormant");
static const name_t name_firstsideonly   ("firstsideonly");
static const name_t name_flags           ("flags");
static const name_t name_flags2          ("flags2");
static const name_t name_friend          ("friend");
static const name_t name_height          ("height");
static const name_t name_heightceiling   ("heightceiling");
static const name_t name_heightfloor     ("heightfloor");
static const name_t name_id              ("id");
static const name_t name_impact          ("impact");
static const name_t name_invisible       ("invisible");
static const name_t name_jumpover        ("jumpover");
static const name_t name_lightlevel      ("lightlevel");
static const name_t name_mapped          ("mapped");
static const name_t name_midtex3d        ("midtex3d");
static const name_t name_missilecross    ("missilecross");
static const name_t name_monstercross    ("monstercross");
static const name_t name_monsteruse      ("monsteruse");
static const name_t name_offsetx         ("offsetx");
static const name_t name_offsety         ("offsety");
static const name_t name_playercross     ("playercross");
static const name_t name_playerpush      ("playerpush");
static const name_t name_playeruse       ("playeruse");
static const name_t name_repeatspecial   ("repeatspecial");
static const name_t name_secret          ("secret");
static const name_t name_sector          ("sector");
static const name_t name_sideback        ("sideback");
static const name_t name_sidefront       ("sidefront");
static const name_t name_single          ("single");
static const name_t name_skill1          ("skill1");
static const name_t name_skill2          ("skill2");
static const name_t name_skill3          ("skill3");
static const name_t name_skill4          ("skill4");
static const name_t name_skill5          ("skill5");
static const name_t name_special         ("special");
static const name_t name_standing        ("standing");
static const name_t name_texturebottom   ("texturebottom");
static const name_t name_textureceiling  ("textureceiling");
static const name_t name_texturefloor    ("texturefloor");
static const name_t name_texturemiddle   ("texturemiddle");
static const name_t name_texturetop      ("texturetop");
static const name_t name_translucent     ("translucent");
static const name_t name_twosided        ("twosided");
static const name_t name_type            ("type");
static const name_t name_v1              ("v1");
static const name_t name_v2              ("v2");
static const name_t name_wrapmidtex      ("wrapmidtext");
static const name_t name_x               ("x");
static const name_t name_y               ("y");
static const name_t name_zoneboundry     ("zoneboundry");



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
		if (option_output_hexen)
		{
			binReturn.reserve(16);

			binReturn += getObject(name_v1)->encode(BIN_OBJECT); // 0-1
			binReturn += getObject(name_v2)->encode(BIN_OBJECT); // 2-3

			// 4-5
			if (hasObject(name_flags))
				binReturn += getObject(name_flags)->encode(USHORT);
			else
			{
				int flags = 0;

				CHECKFLAG(name_blocking,      0x0001);
				CHECKFLAG(name_blockmonsters, 0x0002);
				CHECKFLAG(name_twosided,      0x0004);
				CHECKFLAG(name_dontpegtop,    0x0008);
				CHECKFLAG(name_dontpegbottom, 0x0010);
				CHECKFLAG(name_secret,        0x0020);
				CHECKFLAG(name_blocksound,    0x0040);
				CHECKFLAG(name_dontdraw,      0x0080);
				CHECKFLAG(name_mapped,        0x0100);
				CHECKFLAG(name_repeatspecial, 0x0200);
				CHECKFLAG(name_monsteruse,    0x2000);
				CHECKFLAG(name_blocking,      0x4000);

				if (hasObject(name_playeruse) && to_bool(getObject(name_playeruse)))
					if (hasObject(name_playercross) && to_bool(getObject(name_playercross)))
						flags |= 0x1800;
					else
						flags |= 0x0400;
				else if (hasObject(name_monstercross) && to_bool(getObject(name_monstercross)))
					flags |= 0x0800;
				else if (hasObject(name_impact) && to_bool(getObject(name_impact)))
					flags |= 0x0C00;
				else if (hasObject(name_playerpush) && to_bool(getObject(name_playerpush)))
					flags |= 0x1000;
				else if (hasObject(name_missilecross) && to_bool(getObject(name_missilecross)))
					flags |= 0x1400;

				binReturn += (flags     ) & 0xFF;
				binReturn += (flags >> 8) & 0xFF;
			}

			CHECKVALUE(name_special,   UBYTE,      "\x00",     1); // 06
			CHECKVALUE(name_arg0,      UBYTE,      "\x00",     1); // 07
			CHECKVALUE(name_arg1,      UBYTE,      "\x00",     1); // 08
			CHECKVALUE(name_arg2,      UBYTE,      "\x00",     1); // 09
			CHECKVALUE(name_arg3,      UBYTE,      "\x00",     1); // 10
			CHECKVALUE(name_arg4,      UBYTE,      "\x00",     1); // 11
			CHECKVALUE(name_sidefront, BIN_OBJECT, "\xFF\xFF", 2); // 12-13
			CHECKVALUE(name_sideback,  BIN_OBJECT, "\xFF\xFF", 2); // 14-15

			int_s_t flags = 0;
			if (hasObject(name_flags2))
				flags = to_int_s(getObject(name_flags2));
			else
			{
				CHECKFLAG(name_zoneboundry,      0x01);
				CHECKFLAG(name_jumpover,         0x02);
				CHECKFLAG(name_blockfloaters,    0x04);
				CHECKFLAG(name_clipmidtex,       0x08);
				CHECKFLAG(name_wrapmidtex,       0x10);
				CHECKFLAG(name_midtex3d,         0x20);
				CHECKFLAG(name_checkswitchrange, 0x40);
				CHECKFLAG(name_firstsideonly,    0x80);
			}

			int_s_t id = 0;
			if (hasObject(name_id))
				id = to_int_s(getObject(name_id));

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

			if (id1Arg != -1 && hasObject(name_id))
				binReturn[7+id1Arg] = id & 0xFF;

			if (id2Arg != -1 && hasObject(name_id))
				binReturn[7+id2Arg] = (id >> 8) & 0xFF;

			return binReturn;
		}
		else
		{
			binReturn.reserve(14);

			binReturn += getObject(name_v1)->encode(BIN_OBJECT); // 0-1
			binReturn += getObject(name_v2)->encode(BIN_OBJECT); // 2-3

			// 4-5
			if (hasObject(name_flags))
				binReturn += getObject(name_flags)->encode(USHORT);
			else
			{
				int flags = 0;

				CHECKFLAG(name_blocking,      0x0001);
				CHECKFLAG(name_blockmonsters, 0x0002);
				CHECKFLAG(name_twosided,      0x0004);
				CHECKFLAG(name_dontpegtop,    0x0008);
				CHECKFLAG(name_dontpegbottom, 0x0010);
				CHECKFLAG(name_secret,        0x0020);
				CHECKFLAG(name_blocksound,    0x0040);
				CHECKFLAG(name_dontdraw,      0x0080);
				CHECKFLAG(name_mapped,        0x0100);

				binReturn += (flags     ) & 0xFF;
				binReturn += (flags >> 8) & 0xFF;
			}

			CHECKVALUE(name_id,        USHORT,     "\x00\x00", 2); // 06-07
			CHECKVALUE(name_special,   USHORT,     "\x00\x00", 2); // 08-09
			CHECKVALUE(name_sidefront, BIN_OBJECT, "\xFF\xFF", 2); // 10-11
			CHECKVALUE(name_sideback,  BIN_OBJECT, "\xFF\xFF", 2); // 12-13

			return binReturn;
		}
	}

	if (binType == SECTOR)
	{
		binReturn.reserve(26);

		CHECKVALUE(name_heightfloor,    SSHORT,     "\x00\x00",  2); // 00-01
		CHECKVALUE(name_heightceiling,  SSHORT,     "\x00\x00",  2); // 02-03
		CHECKVALUE(name_texturefloor,   BIN_STRING, NULL_STRING, 8); // 04-11
		CHECKVALUE(name_textureceiling, BIN_STRING, NULL_STRING, 8); // 12-19
		CHECKVALUE(name_lightlevel,     USHORT,     "\xA0\x00",  2); // 20-21
		CHECKVALUE(name_special,        USHORT,     "\x00\x00",  2); // 22-23
		CHECKVALUE(name_id,             USHORT,     "\x00\x00",  2); // 24-25

		return binReturn;
	}

	if (binType == SIDEDEF)
	{
		binReturn.reserve(30);

		CHECKVALUE(name_offsetx,       SSHORT,     "\x00\x00",  2); // 00-01
		CHECKVALUE(name_offsety,       SSHORT,     "\x00\x00",  2); // 02-03
		CHECKVALUE(name_texturetop,    BIN_STRING, NULL_STRING, 8); // 04-11
		CHECKVALUE(name_texturebottom, BIN_STRING, NULL_STRING, 8); // 12-19
		CHECKVALUE(name_texturemiddle, BIN_STRING, NULL_STRING, 8); // 20-27
		binReturn += getObject(name_sector)->encode(BIN_OBJECT);    // 28-29

		return binReturn;
	}

	if (binType == THING)
	{
		if (option_output_hexen)
		{
			binReturn.reserve(20);

			CHECKVALUE(name_id,     USHORT, "\x00\x00", 2);    // 00-01
			binReturn += getObject(name_x)->encode(SSHORT);    // 02-03
			binReturn += getObject(name_y)->encode(SSHORT);    // 04-05
			CHECKVALUE(name_height, USHORT, "\x00\x00", 2);    // 06-07
			CHECKVALUE(name_angle,  USHORT, "\x00\x00", 2);    // 08-09
			binReturn += getObject(name_type)->encode(SSHORT); // 10-11

			// 12-13
			if (hasObject(name_flags))
				binReturn += getObject(name_flags)->encode(USHORT);
			if (option_output_hexen)
			{
				int flags = 0;

				CHECKFLAG(name_skill1,      0x0001);
				CHECKFLAG(name_skill2,      0x0001);
				CHECKFLAG(name_skill3,      0x0002);
				CHECKFLAG(name_skill4,      0x0004);
				CHECKFLAG(name_skill5,      0x0004);
				CHECKFLAG(name_ambush,      0x0008);
				CHECKFLAG(name_dormant,     0x0010);
				CHECKFLAG(name_single,      0x0100);
				CHECKFLAG(name_coop,        0x0200);
				CHECKFLAG(name_dm,          0x0400);
				CHECKFLAG(name_translucent, 0x0800);
				CHECKFLAG(name_invisible,   0x1000);
				CHECKFLAG(name_friend,      0x2000);
				CHECKFLAG(name_standing,    0x4000);

				binReturn += (flags >> 8) & 0xFF;
				binReturn += (flags     ) & 0xFF;
			}

			CHECKVALUE(name_special, UBYTE, "\x00", 1); // 14
			CHECKVALUE(name_arg0,    UBYTE, "\x00", 1); // 15
			CHECKVALUE(name_arg1,    UBYTE, "\x00", 1); // 16
			CHECKVALUE(name_arg2,    UBYTE, "\x00", 1); // 17
			CHECKVALUE(name_arg3,    UBYTE, "\x00", 1); // 18
			CHECKVALUE(name_arg4,    UBYTE, "\x00", 1); // 19

			return binReturn;
		}
		else
		{
			binReturn.reserve(10);

			binReturn += getObject(name_x)->encode(SSHORT);    // 0-1
			binReturn += getObject(name_y)->encode(SSHORT);    // 2-3
			CHECKVALUE(name_angle, USHORT, "\x00\x00", 2);     // 4-5
			binReturn += getObject(name_type)->encode(SSHORT); // 6-7

			// 8-9
			if (hasObject(name_flags))
				binReturn += getObject(name_flags)->encode(USHORT);
			else if (option_output_strife)
			{
				int flags = 0;

				CHECKFLAG   (name_skill1,      0x0001);
				CHECKFLAG   (name_skill2,      0x0001);
				CHECKFLAG   (name_skill3,      0x0002);
				CHECKFLAG   (name_skill4,      0x0004);
				CHECKFLAG   (name_skill5,      0x0004);
				CHECKFLAG   (name_standing,    0x0008);
				CHECKFLAGNOT(name_single,      0x0010);
				CHECKFLAG   (name_ambush,      0x0020);
				CHECKFLAG   (name_friend,      0x0040);
				CHECKFLAG   (name_translucent, 0x0100);
				CHECKFLAG   (name_invisible,   0x0200);

				binReturn += (flags >> 8) & 0xFF;
				binReturn += (flags     ) & 0xFF;
			}
			else
			{
				int flags = 0;

				CHECKFLAG   (name_skill1,      0x0001);
				CHECKFLAG   (name_skill2,      0x0001);
				CHECKFLAG   (name_skill3,      0x0002);
				CHECKFLAG   (name_skill4,      0x0004);
				CHECKFLAG   (name_skill5,      0x0004);
				CHECKFLAG   (name_ambush,      0x0008);
				CHECKFLAGNOT(name_single,      0x0010);
				CHECKFLAGNOT(name_dm,          0x0020);
				CHECKFLAGNOT(name_coop,        0x0040);
				CHECKFLAG   (name_friend,      0x0080);

				binReturn += (flags >> 8) & 0xFF;
				binReturn += (flags     ) & 0xFF;
			}

			return binReturn;
		}
	}

	if (binType == VERTEX)
	{
		binReturn.reserve(4);

		binReturn += getObject(name_x)->encode(SSHORT);
		binReturn += getObject(name_y)->encode(SSHORT);

		return binReturn;
	}

	throw InvalidTypeException("internal error:unknown enum value for 'binType'");

	#undef CHECKFLAG
	#undef CHECKFLAGNOT
	#undef CHECKVALUE
}

std::string LevelObject::encodeExtraData()
{
	if (data.getType() != any_t::OBJMAP_T)
		return "";

	if (get_lo_type(type) != LO_TYPE_OBJECT)
		return "";

	return "";
}



