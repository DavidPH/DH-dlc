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
#include "LevelObjectMap.hpp"
#include "LevelObjectName.hpp"
#include "options.hpp"
#include "types.hpp"
#include "../common/foreach.hpp"
#include "exceptions/InvalidTypeException.hpp"
#include "types/binary.hpp"
#include "types/int_t.hpp"
#include "types/real_t.hpp"
#include "types/string_t.hpp"



static const name_t name_ambush           ("ambush");
static const name_t name_angle            ("angle");
static const name_t name_arg0             ("arg0");
static const name_t name_arg1             ("arg1");
static const name_t name_arg2             ("arg2");
static const name_t name_arg3             ("arg3");
static const name_t name_arg4             ("arg4");
static const name_t name_blockeverything  ("blockeverything");
static const name_t name_blockfloaters    ("blockfloaters");
static const name_t name_blocking         ("blocking");
static const name_t name_blockmonsters    ("blockmonsters");
static const name_t name_blockplayers     ("blockplayers");
static const name_t name_blocksound       ("blocksound");
static const name_t name_checkswitchrange ("checkswitchrange");
static const name_t name_clipmidtex       ("clipmidtext");
static const name_t name_coop             ("coop");
static const name_t name_dm               ("dm");
static const name_t name_dontdraw         ("dontdraw");
static const name_t name_dontpegbottom    ("dontpegbottom");
static const name_t name_dontpegtop       ("dontpegtop");
static const name_t name_dormant          ("dormant");
static const name_t name_extradata_flags  ("extradata_flags");
static const name_t name_extradata_special("extradata_flags");
static const name_t name_extradata_tag    ("extradata_tag");
static const name_t name_extradata_type   ("extradata_type");
static const name_t name_firstsideonly    ("firstsideonly");
static const name_t name_flags            ("flags");
static const name_t name_flags2           ("flags2");
static const name_t name_friend           ("friend");
static const name_t name_height           ("height");
static const name_t name_heightceiling    ("heightceiling");
static const name_t name_heightfloor      ("heightfloor");
static const name_t name_id               ("id");
static const name_t name_impact           ("impact");
static const name_t name_invisible        ("invisible");
static const name_t name_jumpover         ("jumpover");
static const name_t name_lightlevel       ("lightlevel");
static const name_t name_mapped           ("mapped");
static const name_t name_midtex3d         ("midtex3d");
static const name_t name_missilecross     ("missilecross");
static const name_t name_monsteractivate  ("monsteractivate");
static const name_t name_monstercross     ("monstercross");
static const name_t name_monsterpush      ("monsterpush");
static const name_t name_monsteruse       ("monsteruse");
static const name_t name_offsetx          ("offsetx");
static const name_t name_offsety          ("offsety");
static const name_t name_passuse          ("passuse");
static const name_t name_playercross      ("playercross");
static const name_t name_playerpush       ("playerpush");
static const name_t name_playeruse        ("playeruse");
static const name_t name_railing          ("railing");
static const name_t name_repeatspecial    ("repeatspecial");
static const name_t name_secret           ("secret");
static const name_t name_sector           ("sector");
static const name_t name_sideback         ("sideback");
static const name_t name_sidefront        ("sidefront");
static const name_t name_single           ("single");
static const name_t name_skill1           ("skill1");
static const name_t name_skill2           ("skill2");
static const name_t name_skill3           ("skill3");
static const name_t name_skill4           ("skill4");
static const name_t name_skill5           ("skill5");
static const name_t name_special          ("special");
static const name_t name_standing         ("standing");
static const name_t name_texturebottom    ("texturebottom");
static const name_t name_textureceiling   ("textureceiling");
static const name_t name_texturefloor     ("texturefloor");
static const name_t name_texturemiddle    ("texturemiddle");
static const name_t name_texturetop       ("texturetop");
static const name_t name_translucent      ("translucent");
static const name_t name_twosided         ("twosided");
static const name_t name_type             ("type");
static const name_t name_v1               ("v1");
static const name_t name_v2               ("v2");
static const name_t name_wrapmidtex       ("wrapmidtext");
static const name_t name_x                ("x");
static const name_t name_y                ("y");
static const name_t name_zoneboundry      ("zoneboundry");

#define CHECKFLAG(ASSIGN, FLAG, VALUE) \
if (hasObject(FLAG) && to_bool(getObject(FLAG))) \
	ASSIGN |= VALUE; \
else (void) 0

#define CHECKFLAGNOT(ASSIGN, FLAG, VALUE) \
if (!hasObject(FLAG) || !to_bool(getObject(FLAG))) \
	ASSIGN |= VALUE; \
else (void) 0

#define CHECKVALUE1_ASSIGN(ASSIGN, NAME, TYPE) \
if (hasObject(NAME)) \
	ASSIGN = to_##TYPE(getObject(NAME)); \
else

#define CHECKVALUE2_ASSIGN(ASSIGN, NAME, TYPE, DEFAULT) \
CHECKVALUE1_ASSIGN(ASSIGN, NAME, TYPE) \
	ASSIGN = TYPE##_t(DEFAULT)

#define CHECKVALUE1_BINARY(NAME, TYPE) \
if (hasObject(NAME)) \
	to_##TYPE(getObject(NAME)).encodeBinary(out); \
else

#define CHECKVALUE2_BINARY(NAME, TYPE, DEFAULT) \
CHECKVALUE1_BINARY(NAME, TYPE) \
	TYPE##_t(DEFAULT).encodeBinary(out)



void LevelObject::encodeDoom(std::ostream & out)
{
	if (_data.getType() != any_t::OBJMAP_T) return;

	if (_type == type_t::type_linedef())
	{
		uword_t v1;        // 00-01
		uword_t v2;        // 02-03
		uword_t flags;     // 04-05
		uword_t id;        // 06-07
		uword_t special;   // 08-09
		uword_t sidefront; // 10-11
		uword_t sideback;  // 12-13

		CHECKVALUE2_ASSIGN(v1,        name_v1,        uword, -1); // 00-01
		CHECKVALUE2_ASSIGN(v2,        name_v2,        uword, -1); // 02-03
		CHECKVALUE1_ASSIGN(flags,     name_flags,     uword    )  // 04-05
		{
			CHECKFLAG(flags, name_blocking,      uword_t(0x0001U)); // Doom
			CHECKFLAG(flags, name_blockmonsters, uword_t(0x0002U)); // Doom
			CHECKFLAG(flags, name_twosided,      uword_t(0x0004U)); // Doom
			CHECKFLAG(flags, name_dontpegtop,    uword_t(0x0008U)); // Doom
			CHECKFLAG(flags, name_dontpegbottom, uword_t(0x0010U)); // Doom
			CHECKFLAG(flags, name_secret,        uword_t(0x0020U)); // Doom
			CHECKFLAG(flags, name_blocksound,    uword_t(0x0040U)); // Doom
			CHECKFLAG(flags, name_dontdraw,      uword_t(0x0080U)); // Doom
			CHECKFLAG(flags, name_mapped,        uword_t(0x0100U)); // Doom
			CHECKFLAG(flags, name_passuse,       uword_t(0x0200U)); // Boom
		}
		CHECKVALUE2_ASSIGN(id,        name_id,        uword,  0); // 06-07
		CHECKVALUE2_ASSIGN(special,   name_special,   uword,  0); // 08-09
		CHECKVALUE2_ASSIGN(sidefront, name_sidefront, uword, -1); // 10-11
		CHECKVALUE2_ASSIGN(sideback,  name_sideback,  uword, -1); // 12-13

		if (option_output_extradata && (special != uword_t(270)))
		{
			if (hasObject(name_special))
				addObject(name_extradata_special, getObject(name_special));

			if (hasObject(name_id))
				addObject(name_extradata_tag, getObject(name_id));

			special = uword_t(270);
			id      = uword_t(_index);

			addObject(name_special, create(type_t::type_uword(), make_string(special)));
		}

		v1.encodeBinary(out);
		v2.encodeBinary(out);
		flags.encodeBinary(out);
		id.encodeBinary(out);
		special.encodeBinary(out);
		sidefront.encodeBinary(out);
		sideback.encodeBinary(out);

		return;
	}

	if (_type == type_t::type_sector())
	{
		CHECKVALUE2_BINARY(name_heightfloor,    sword,     0); // 00-01
		CHECKVALUE2_BINARY(name_heightceiling,  sword,     0); // 02-03
		CHECKVALUE2_BINARY(name_texturefloor,   string8,  ""); // 04-11
		CHECKVALUE2_BINARY(name_textureceiling, string8,  ""); // 12-19
		CHECKVALUE2_BINARY(name_lightlevel,     uword,   160); // 20-21
		CHECKVALUE2_BINARY(name_special,        uword,     0); // 22-23
		CHECKVALUE2_BINARY(name_id,             uword,     0); // 24-25

		return;
	}

	if (_type == type_t::type_sidedef())
	{
		CHECKVALUE2_BINARY(name_offsetx,       sword,    0); // 00-01
		CHECKVALUE2_BINARY(name_offsety,       sword,    0); // 02-03
		CHECKVALUE2_BINARY(name_texturetop,    string8, ""); // 04-11
		CHECKVALUE2_BINARY(name_texturebottom, string8, ""); // 12-19
		CHECKVALUE2_BINARY(name_texturemiddle, string8, ""); // 20-27
		CHECKVALUE2_BINARY(name_sector,        uword,   -1); // 28-29

		return;
	}

	if (_type == type_t::type_thing())
	{
		sword_t x;     // 00-01
		sword_t y;     // 02-03
		uword_t angle; // 04-05
		uword_t type;  // 06-07
		uword_t flags; // 08-09

		CHECKVALUE2_ASSIGN(x,     name_x,     sword, 0); // 00-01
		CHECKVALUE2_ASSIGN(y,     name_y,     sword, 0); // 02-03
		CHECKVALUE2_ASSIGN(angle, name_angle, uword, 0); // 04-05
		CHECKVALUE2_ASSIGN(type,  name_type,  uword, 0); // 06-07
		CHECKVALUE1_ASSIGN(flags, name_flags, uword   )  // 08-09
		{
			CHECKFLAG   (flags, name_skill1, uword_t(0x0001U)); // Doom
			CHECKFLAG   (flags, name_skill2, uword_t(0x0001U)); // Doom
			CHECKFLAG   (flags, name_skill3, uword_t(0x0002U)); // Doom
			CHECKFLAG   (flags, name_skill4, uword_t(0x0004U)); // Doom
			CHECKFLAG   (flags, name_skill5, uword_t(0x0004U)); // Doom
			CHECKFLAG   (flags, name_ambush, uword_t(0x0008U)); // Doom
			CHECKFLAGNOT(flags, name_single, uword_t(0x0010U)); // Doom
			CHECKFLAGNOT(flags, name_dm,     uword_t(0x0020U)); // Boom
			CHECKFLAGNOT(flags, name_coop,   uword_t(0x0040U)); // Boom
			CHECKFLAG   (flags, name_friend, uword_t(0x0080U)); // Boom
		}

		if (option_output_extradata && (type != uword_t(5004)))
		{
			if (hasObject(name_type))
				addObject(name_extradata_type, getObject(name_type));

			if (hasObject(name_flags))
				addObject(name_extradata_flags, getObject(name_flags));

			type  = uword_t(5004);
			flags = uword_t(_index);

			addObject(name_type, create(type_t::type_uword(), make_string(type)));
		}

		x.encodeBinary(out);
		y.encodeBinary(out);
		angle.encodeBinary(out);
		type.encodeBinary(out);
		flags.encodeBinary(out);

		return;
	}

	if (_type == type_t::type_vertex())
	{
		CHECKVALUE2_BINARY(name_x, sword, 0); // 00-01
		CHECKVALUE2_BINARY(name_y, sword, 0); // 02-03

		return;
	}
}

void LevelObject::encodeExtraData(std::ostream & out)
{
	if (_data.getType() != any_t::OBJMAP_T)
		return;

	if (_type == type_t::type_linedef())
	{
		int_l_t special = to_int_l(getObject(name_special));

		if (special != int_l_t(270)) return;

		out << "linedef\n{\n";

		out << "\trecordnum " << _index << '\n';

		if (hasObject(name_extradata_special))
		{
			obj_t extradata_special = getObject(name_extradata_special);

			out << "\tspecial ";

			if (extradata_special->_data.getType() == any_t::STRING_T)
				out << to_string(extradata_special);
			else
				out << to_int_l(extradata_special);

			out << "\n";
		}

		if (hasObject(name_extradata_tag))
			out << "\ttag " << to_int_l(getObject(name_extradata_tag)) << '\n';

		if (hasObject(name_extradata_flags))
		{
			obj_t extradata_flags = getObject(name_extradata_flags);

			out << "\textflags ";

			if (extradata_flags->_data.getType() == any_t::STRING_T)
				out << to_string(extradata_flags);
			else
				out << to_int_l(extradata_flags);

			out << "\n";
		}
		else
		{
			out << "\textflags 0";

			if (hasObject(name_monstercross) && to_bool(getObject(name_monstercross)))
				out << "|CROSS";
			if (hasObject(name_playercross) && to_bool(getObject(name_playercross)))
				out << "|CROSS";

			if (hasObject(name_monsteruse) && to_bool(getObject(name_monsteruse)))
				out << "|USE";
			if (hasObject(name_playeruse) && to_bool(getObject(name_playeruse)))
				out << "|USE";

			if (hasObject(name_impact) && to_bool(getObject(name_impact)))
				out << "|IMPACT";

			if (hasObject(name_monsterpush) && to_bool(getObject(name_monsterpush)))
				out << "|PUSH";
			if (hasObject(name_playerpush) && to_bool(getObject(name_playerpush)))
				out << "|PUSH";

			if (hasObject(name_playercross) && to_bool(getObject(name_playercross)))
				out << "|PLAYER";
			if (hasObject(name_playeruse) && to_bool(getObject(name_playeruse)))
				out << "|PLAYER";
			if (hasObject(name_playerpush) && to_bool(getObject(name_playerpush)))
				out << "|PLAYER";

			if (hasObject(name_monsteractivate) && to_bool(getObject(name_monsteractivate)))
				out << "|MONSTER";

			if (hasObject(name_missilecross) && to_bool(getObject(name_missilecross)))
				out << "|MISSILE";

			if (hasObject(name_repeatspecial) && to_bool(getObject(name_repeatspecial)))
				out << "|REPEAT";

			if (hasObject(name_firstsideonly) && to_bool(getObject(name_firstsideonly)))
				out << "|1SONLY";

			out << "\n";
		}

		out << "\targs {";

		if (hasObject(name_arg0))
			out << to_int_l(getObject(name_arg0));
		else
			out << "0";

		out << ", ";

		if (hasObject(name_arg1))
			out << to_int_l(getObject(name_arg1));
		else
			out << "0";

		out << ", ";

		if (hasObject(name_arg2))
			out << to_int_l(getObject(name_arg2));
		else
			out << "0";

		out << ", ";

		if (hasObject(name_arg3))
			out << to_int_l(getObject(name_arg3));
		else
			out << "0";

		out << ", ";

		if (hasObject(name_arg4))
			out << to_int_l(getObject(name_arg4));
		else
			out << "0";

		out << "}\n";

		out << "}\n\n";

		return;
	}

	if (_type == type_t::type_thing())
	{
		int_l_t type = to_int_l(getObject(name_type));

		if (type != int_l_t(5004)) return;

		out << "mapthing\n{\n";

		out << "\trecordnum " << _index << '\n';

		if (hasObject(name_extradata_type))
		{
			obj_t extradata_type = getObject(name_extradata_type);

			out << "\ttype ";

			if (extradata_type->_data.getType() == any_t::STRING_T)
				out << to_string(extradata_type);
			else
				out << to_int_l(extradata_type);

			out << "\n";
		}

		if (hasObject(name_extradata_flags))
		{
			obj_t extradata_flags = getObject(name_extradata_flags);

			out << "\toptions ";

			if (extradata_flags->_data.getType() == any_t::STRING_T)
				out << to_string(extradata_flags);
			else
				out << to_int_l(extradata_flags);

			out << "\n";
		}
		else
		{
			out << "\toptions 0";

			if (hasObject(name_skill1) && to_bool(getObject(name_skill1)))
				out << "|EASY";
			if (hasObject(name_skill2) && to_bool(getObject(name_skill2)))
				out << "|EASY";

			if (hasObject(name_skill3) && to_bool(getObject(name_skill3)))
				out << "|NORMAL";

			if (hasObject(name_skill4) && to_bool(getObject(name_skill4)))
				out << "|HARD";
			if (hasObject(name_skill5) && to_bool(getObject(name_skill5)))
				out << "|HARD";

			if (hasObject(name_ambush) && to_bool(getObject(name_ambush)))
				out << "|AMBUSH";

			if (!hasObject(name_single) || !to_bool(getObject(name_single)))
				out << "|NOTSINGLE";

			if (!hasObject(name_dm) || !to_bool(getObject(name_dm)))
				out << "|NOTDM";

			if (!hasObject(name_coop) || !to_bool(getObject(name_coop)))
				out << "|NOTCOOP";

			if (hasObject(name_friend) && to_bool(getObject(name_friend)))
				out << "|FRIEND";

			if (hasObject(name_dormant) && to_bool(getObject(name_dormant)))
				out << "|DORMANT";

			out << "\n";
		}

		if (hasObject(name_id))
			out << "\ttid " << to_int_l(getObject(name_id)) << '\n';

		out << "\targs {";

		if (hasObject(name_arg0))
			out << to_int_l(getObject(name_arg0));
		else
			out << "0";

		out << ", ";

		if (hasObject(name_arg1))
			out << to_int_l(getObject(name_arg1));
		else
			out << "0";

		out << ", ";

		if (hasObject(name_arg2))
			out << to_int_l(getObject(name_arg2));
		else
			out << "0";

		out << ", ";

		if (hasObject(name_arg3))
			out << to_int_l(getObject(name_arg3));
		else
			out << "0";

		out << ", ";

		if (hasObject(name_arg4))
			out << to_int_l(getObject(name_arg4));
		else
			out << "0";

		out << "}\n";

		if (hasObject(name_height))
			out << "\theight " << to_int_l(getObject(name_height)) << '\n';

		out << "}\n\n";

		return;
	}
}

void LevelObject::encodeHeretic(std::ostream & out)
{
	if (_data.getType() != any_t::OBJMAP_T) return;

	if (_type == type_t::type_linedef())
	{
		CHECKVALUE2_BINARY(name_v1,        uword, -1); // 00-01
		CHECKVALUE2_BINARY(name_v2,        uword, -1); // 02-03
		CHECKVALUE1_BINARY(name_flags,     uword    )  // 04-05
		{
			uword_t flags(0);

			CHECKFLAG(flags, name_blocking,      uword_t(0x0001U)); // Doom
			CHECKFLAG(flags, name_blockmonsters, uword_t(0x0002U)); // Doom
			CHECKFLAG(flags, name_twosided,      uword_t(0x0004U)); // Doom
			CHECKFLAG(flags, name_dontpegtop,    uword_t(0x0008U)); // Doom
			CHECKFLAG(flags, name_dontpegbottom, uword_t(0x0010U)); // Doom
			CHECKFLAG(flags, name_secret,        uword_t(0x0020U)); // Doom
			CHECKFLAG(flags, name_blocksound,    uword_t(0x0040U)); // Doom
			CHECKFLAG(flags, name_dontdraw,      uword_t(0x0080U)); // Doom
			CHECKFLAG(flags, name_mapped,        uword_t(0x0100U)); // Doom
			CHECKFLAG(flags, name_passuse,       uword_t(0x0200U)); // Boom

			flags.encodeBinary(out);
		}
		CHECKVALUE2_BINARY(name_id,        uword,  0); // 06-07
		CHECKVALUE2_BINARY(name_special,   uword,  0); // 08-09
		CHECKVALUE2_BINARY(name_sidefront, uword, -1); // 10-11
		CHECKVALUE2_BINARY(name_sideback,  uword, -1); // 12-13

		return;
	}

	if (_type == type_t::type_sector())
	{
		CHECKVALUE2_BINARY(name_heightfloor,    sword,     0); // 00-01
		CHECKVALUE2_BINARY(name_heightceiling,  sword,     0); // 02-03
		CHECKVALUE2_BINARY(name_texturefloor,   string8,  ""); // 04-11
		CHECKVALUE2_BINARY(name_textureceiling, string8,  ""); // 12-19
		CHECKVALUE2_BINARY(name_lightlevel,     uword,   160); // 20-21
		CHECKVALUE2_BINARY(name_special,        uword,     0); // 22-23
		CHECKVALUE2_BINARY(name_id,             uword,     0); // 24-25

		return;
	}

	if (_type == type_t::type_sidedef())
	{
		CHECKVALUE2_BINARY(name_offsetx,       sword,    0); // 00-01
		CHECKVALUE2_BINARY(name_offsety,       sword,    0); // 02-03
		CHECKVALUE2_BINARY(name_texturetop,    string8, ""); // 04-11
		CHECKVALUE2_BINARY(name_texturebottom, string8, ""); // 12-19
		CHECKVALUE2_BINARY(name_texturemiddle, string8, ""); // 20-27
		CHECKVALUE2_BINARY(name_sector,        uword,   -1); // 28-29

		return;
	}

	if (_type == type_t::type_thing())
	{
		CHECKVALUE2_BINARY(name_x,     sword, 0); // 00-01
		CHECKVALUE2_BINARY(name_y,     sword, 0); // 02-03
		CHECKVALUE2_BINARY(name_angle, uword, 0); // 04-05
		CHECKVALUE2_BINARY(name_type,  uword, 0); // 06-07
		CHECKVALUE1_BINARY(name_flags, uword   )  // 08-09
		{
			uword_t flags(0);

			CHECKFLAG   (flags, name_skill1, uword_t(0x0001U)); // Doom
			CHECKFLAG   (flags, name_skill2, uword_t(0x0001U)); // Doom
			CHECKFLAG   (flags, name_skill3, uword_t(0x0002U)); // Doom
			CHECKFLAG   (flags, name_skill4, uword_t(0x0004U)); // Doom
			CHECKFLAG   (flags, name_skill5, uword_t(0x0004U)); // Doom
			CHECKFLAG   (flags, name_ambush, uword_t(0x0008U)); // Doom
			CHECKFLAGNOT(flags, name_single, uword_t(0x0010U)); // Doom
			CHECKFLAGNOT(flags, name_dm,     uword_t(0x0020U)); // Boom
			CHECKFLAGNOT(flags, name_coop,   uword_t(0x0040U)); // Boom
			CHECKFLAG   (flags, name_friend, uword_t(0x0080U)); // Boom

			flags.encodeBinary(out);
		}

		return;
	}

	if (_type == type_t::type_vertex())
	{
		CHECKVALUE2_BINARY(name_x, sword, 0); // 00-01
		CHECKVALUE2_BINARY(name_y, sword, 0); // 02-03

		return;
	}
}

void LevelObject::encodeHexen(std::ostream & out)
{
	if (_data.getType() != any_t::OBJMAP_T) return;

	if (_type == type_t::type_linedef())
	{
		uword_t v1;        // 00-01
		uword_t v2;        // 02-03
		uword_t flags;     // 04-05
		ubyte_t special;   // 06
		ubyte_t arg0;      // 07
		ubyte_t arg1;      // 08
		ubyte_t arg2;      // 09
		ubyte_t arg3;      // 10
		ubyte_t arg4;      // 11
		uword_t sidefront; // 12-13
		uword_t sideback;  // 14-15

		ubyte_t flags2;    // **
		uword_t id;        // **

		CHECKVALUE2_ASSIGN(v1,        name_v1,        uword, -1); // 00-01
		CHECKVALUE2_ASSIGN(v2,        name_v2,        uword, -1); // 02-03
		CHECKVALUE1_ASSIGN(flags,     name_flags,     uword    )  // 04-05
		{
			CHECKFLAG(flags, name_blocking,        uword_t(0x0001U)); // Doom
			CHECKFLAG(flags, name_blockmonsters,   uword_t(0x0002U)); // Doom
			CHECKFLAG(flags, name_twosided,        uword_t(0x0004U)); // Doom
			CHECKFLAG(flags, name_dontpegtop,      uword_t(0x0008U)); // Doom
			CHECKFLAG(flags, name_dontpegbottom,   uword_t(0x0010U)); // Doom
			CHECKFLAG(flags, name_secret,          uword_t(0x0020U)); // Doom
			CHECKFLAG(flags, name_blocksound,      uword_t(0x0040U)); // Doom
			CHECKFLAG(flags, name_dontdraw,        uword_t(0x0080U)); // Doom
			CHECKFLAG(flags, name_mapped,          uword_t(0x0100U)); // Doom
			CHECKFLAG(flags, name_repeatspecial,   uword_t(0x0200U)); // Hexen
			CHECKFLAG(flags, name_monsteractivate, uword_t(0x2000U)); // Hexen
			CHECKFLAG(flags, name_blockplayers,    uword_t(0x4000U)); // Hexen
			CHECKFLAG(flags, name_blockeverything, uword_t(0x8000U)); // Hexen

			if (hasObject(name_playeruse) && to_bool(getObject(name_playeruse)))
				if (hasObject(name_playercross) && to_bool(getObject(name_playercross)))
					flags |= uword_t(0x1800U);
				else
					flags |= uword_t(0x0400U);
			else if (hasObject(name_monstercross) && to_bool(getObject(name_monstercross)))
				flags |= uword_t(0x0800U);
			else if (hasObject(name_impact) && to_bool(getObject(name_impact)))
				flags |= uword_t(0x0C00U);
			else if (hasObject(name_playerpush) && to_bool(getObject(name_playerpush)))
				flags |= uword_t(0x1000U);
			else if (hasObject(name_missilecross) && to_bool(getObject(name_missilecross)))
				flags |= uword_t(0x1400U);

			flags.encodeBinary(out);
		}
		CHECKVALUE2_ASSIGN(special,   name_special,   ubyte,  0); // 06
		CHECKVALUE2_ASSIGN(arg0,      name_arg0,      ubyte,  0); // 07
		CHECKVALUE2_ASSIGN(arg1,      name_arg1,      ubyte,  0); // 08
		CHECKVALUE2_ASSIGN(arg2,      name_arg2,      ubyte,  0); // 09
		CHECKVALUE2_ASSIGN(arg3,      name_arg3,      ubyte,  0); // 10
		CHECKVALUE2_ASSIGN(arg4,      name_arg4,      ubyte,  0); // 11
		CHECKVALUE2_ASSIGN(sidefront, name_sidefront, uword, -1); // 12-13
		CHECKVALUE2_ASSIGN(sideback,  name_sideback,  uword, -1); // 14-15
		CHECKVALUE1_ASSIGN(flags2,    name_flags2,    ubyte    )  // **
		{
			CHECKFLAG(flags2, name_zoneboundry,      ubyte_t(0x01U)); // ZDoom
			CHECKFLAG(flags2, name_jumpover,         ubyte_t(0x02U)); // ZDoom
			CHECKFLAG(flags2, name_blockfloaters,    ubyte_t(0x04U)); // ZDoom
			CHECKFLAG(flags2, name_clipmidtex,       ubyte_t(0x08U)); // ZDoom
			CHECKFLAG(flags2, name_wrapmidtex,       ubyte_t(0x10U)); // ZDoom
			CHECKFLAG(flags2, name_midtex3d,         ubyte_t(0x20U)); // ZDoom
			CHECKFLAG(flags2, name_checkswitchrange, ubyte_t(0x40U)); // ZDoom
			CHECKFLAG(flags2, name_firstsideonly,    ubyte_t(0x80U)); // ZDoom
		}
		CHECKVALUE2_ASSIGN(id,        name_id,        uword,  0); // **

		switch (special.makeInt())
		{
		case   0: // no special
			if ((id != uword_t()) || (flags2 != ubyte_t()))
			{
				special = ubyte_t(121U);
				arg0    = ubyte_t((id.makeInt()     ) & 0xFF); // Hexen
				arg1    = flags2;                              // ZDoom
				arg4    = ubyte_t((id.makeInt() >> 8) & 0xFF); // ZDoom
			}
			break;

		case   1: // Hexen (PolyObj_StartLine)
			if (arg3 == ubyte_t(0)) arg3 = ubyte_t((id.makeInt()     ) & 0xFF); // Hexen
			break;

		case   5: // Hexen (PolyObj_ExplicitLine)
			if (arg4 == ubyte_t(0)) arg4 = ubyte_t((id.makeInt()     ) & 0xFF); // Hexen
			break;

		case 121: // Hexen (Line_SetIdentification)
			if (arg0 == ubyte_t(0)) arg0 = ubyte_t((id.makeInt()     ) & 0xFF); // Hexen
			if (arg1 == ubyte_t(0)) arg1 = flags2;                              // ZDoom
			if (arg4 == ubyte_t(0)) arg4 = ubyte_t((id.makeInt() >> 8) & 0xFF); // ZDoom
			break;

		case 208: // ZDoom (TranslucentLine)
			if (arg0 == ubyte_t(0)) arg0 = ubyte_t((id.makeInt()     ) & 0xFF); // ZDoom
			if (arg3 == ubyte_t(0)) arg3 = flags2;                              // ZDoom
			break;

		case 215: // ZDoom (Teleport_Line)
			if (arg0 == ubyte_t(0)) arg0 = ubyte_t((id.makeInt()     ) & 0xFF); // ZDoom
			break;

		case 222: // ZDoom (Scroll_Texture_Model)
			if (arg0 == ubyte_t(0)) arg0 = ubyte_t((id.makeInt()     ) & 0xFF); // ZDoom
			break;
		}

		v1.encodeBinary(out);        // 00-01
		v2.encodeBinary(out);        // 02-03
		flags.encodeBinary(out);     // 04-05
		special.encodeBinary(out);   // 06
		arg0.encodeBinary(out);      // 07
		arg1.encodeBinary(out);      // 08
		arg2.encodeBinary(out);      // 09
		arg3.encodeBinary(out);      // 10
		arg4.encodeBinary(out);      // 11
		sidefront.encodeBinary(out); // 12-13
		sideback.encodeBinary(out);  // 14-15

		return;
	}

	if (_type == type_t::type_sector())
	{
		CHECKVALUE2_BINARY(name_heightfloor,    sword,     0); // 00-01
		CHECKVALUE2_BINARY(name_heightceiling,  sword,     0); // 02-03
		CHECKVALUE2_BINARY(name_texturefloor,   string8,  ""); // 04-11
		CHECKVALUE2_BINARY(name_textureceiling, string8,  ""); // 12-19
		CHECKVALUE2_BINARY(name_lightlevel,     uword,   160); // 20-21
		CHECKVALUE2_BINARY(name_special,        uword,     0); // 22-23
		CHECKVALUE2_BINARY(name_id,             uword,     0); // 24-25

		return;
	}

	if (_type == type_t::type_sidedef())
	{
		CHECKVALUE2_BINARY(name_offsetx,       sword,    0); // 00-01
		CHECKVALUE2_BINARY(name_offsety,       sword,    0); // 02-03
		CHECKVALUE2_BINARY(name_texturetop,    string8, ""); // 04-11
		CHECKVALUE2_BINARY(name_texturebottom, string8, ""); // 12-19
		CHECKVALUE2_BINARY(name_texturemiddle, string8, ""); // 20-27
		CHECKVALUE2_BINARY(name_sector,        uword,   -1); // 28-29

		return;
	}

	if (_type == type_t::type_thing())
	{
		CHECKVALUE2_BINARY(name_id,      uword,  0); // 00-01
		CHECKVALUE2_BINARY(name_x,       sword,  0); // 02-03
		CHECKVALUE2_BINARY(name_y,       sword,  0); // 04-05
		CHECKVALUE2_BINARY(name_height,  sword,  0); // 06-07
		CHECKVALUE2_BINARY(name_angle,   uword,  0); // 08-09
		CHECKVALUE2_BINARY(name_type,    uword,  0); // 10-11
		CHECKVALUE1_BINARY(name_flags,   uword    )  // 12-13
		{
			uword_t flags;

			CHECKFLAG(flags, name_skill1,      uword_t(0x0001U)); // Doom
			CHECKFLAG(flags, name_skill2,      uword_t(0x0001U)); // Doom
			CHECKFLAG(flags, name_skill3,      uword_t(0x0002U)); // Doom
			CHECKFLAG(flags, name_skill4,      uword_t(0x0004U)); // Doom
			CHECKFLAG(flags, name_skill5,      uword_t(0x0004U)); // Doom
			CHECKFLAG(flags, name_ambush,      uword_t(0x0008U)); // Doom
			CHECKFLAG(flags, name_dormant,     uword_t(0x0010U)); // Hexen
			CHECKFLAG(flags, name_single,      uword_t(0x0100U)); // Hexen
			CHECKFLAG(flags, name_coop,        uword_t(0x0200U)); // Hexen
			CHECKFLAG(flags, name_dm,          uword_t(0x0400U)); // Hexen
			CHECKFLAG(flags, name_translucent, uword_t(0x0800U)); // Hexen
			CHECKFLAG(flags, name_invisible,   uword_t(0x1000U)); // Hexen
			CHECKFLAG(flags, name_friend,      uword_t(0x2000U)); // Hexen
			CHECKFLAG(flags, name_standing,    uword_t(0x4000U)); // Hexen
		}

		CHECKVALUE2_BINARY(name_special, ubyte,  0); // 14
		CHECKVALUE2_BINARY(name_arg0,    ubyte,  0); // 15
		CHECKVALUE2_BINARY(name_arg1,    ubyte,  0); // 16
		CHECKVALUE2_BINARY(name_arg2,    ubyte,  0); // 17
		CHECKVALUE2_BINARY(name_arg3,    ubyte,  0); // 18
		CHECKVALUE2_BINARY(name_arg4,    ubyte,  0); // 19

		return;
	}

	if (_type == type_t::type_vertex())
	{
		CHECKVALUE2_BINARY(name_x, sword, 0); // 00-01
		CHECKVALUE2_BINARY(name_y, sword, 0); // 02-03

		return;
	}
}

void LevelObject::encodeStrife(std::ostream & out)
{
	if (_data.getType() != any_t::OBJMAP_T) return;

	if (_type == type_t::type_linedef())
	{
		CHECKVALUE2_BINARY(name_v1,        uword, -1); // 00-01
		CHECKVALUE2_BINARY(name_v2,        uword, -1); // 02-03
		CHECKVALUE1_BINARY(name_flags,     uword    )  // 04-05
		{
			uword_t flags(0);

			CHECKFLAG(flags, name_blocking,      uword_t(0x0001U)); // Doom
			CHECKFLAG(flags, name_blockmonsters, uword_t(0x0002U)); // Doom
			CHECKFLAG(flags, name_twosided,      uword_t(0x0004U)); // Doom
			CHECKFLAG(flags, name_dontpegtop,    uword_t(0x0008U)); // Doom
			CHECKFLAG(flags, name_dontpegbottom, uword_t(0x0010U)); // Doom
			CHECKFLAG(flags, name_secret,        uword_t(0x0020U)); // Doom
			CHECKFLAG(flags, name_blocksound,    uword_t(0x0040U)); // Doom
			CHECKFLAG(flags, name_dontdraw,      uword_t(0x0080U)); // Doom
			CHECKFLAG(flags, name_mapped,        uword_t(0x0100U)); // Doom
			CHECKFLAG(flags, name_railing,       uword_t(0x0200U)); // Strife
			CHECKFLAG(flags, name_translucent,   uword_t(0x0400U)); // Strife

			flags.encodeBinary(out);
		}
		CHECKVALUE2_BINARY(name_id,        uword,  0); // 06-07
		CHECKVALUE2_BINARY(name_special,   uword,  0); // 08-09
		CHECKVALUE2_BINARY(name_sidefront, uword, -1); // 10-11
		CHECKVALUE2_BINARY(name_sideback,  uword, -1); // 12-13

		return;
	}

	if (_type == type_t::type_sector())
	{
		CHECKVALUE2_BINARY(name_heightfloor,    sword,     0); // 00-01
		CHECKVALUE2_BINARY(name_heightceiling,  sword,     0); // 02-03
		CHECKVALUE2_BINARY(name_texturefloor,   string8,  ""); // 04-11
		CHECKVALUE2_BINARY(name_textureceiling, string8,  ""); // 12-19
		CHECKVALUE2_BINARY(name_lightlevel,     uword,   160); // 20-21
		CHECKVALUE2_BINARY(name_special,        uword,     0); // 22-23
		CHECKVALUE2_BINARY(name_id,             uword,     0); // 24-25

		return;
	}

	if (_type == type_t::type_sidedef())
	{
		CHECKVALUE2_BINARY(name_offsetx,       sword,    0); // 00-01
		CHECKVALUE2_BINARY(name_offsety,       sword,    0); // 02-03
		CHECKVALUE2_BINARY(name_texturetop,    string8, ""); // 04-11
		CHECKVALUE2_BINARY(name_texturebottom, string8, ""); // 12-19
		CHECKVALUE2_BINARY(name_texturemiddle, string8, ""); // 20-27
		CHECKVALUE2_BINARY(name_sector,        uword,   -1); // 28-29

		return;
	}

	if (_type == type_t::type_thing())
	{
		CHECKVALUE2_BINARY(name_x,     sword, 0); // 00-01
		CHECKVALUE2_BINARY(name_y,     sword, 0); // 02-03
		CHECKVALUE2_BINARY(name_angle, uword, 0); // 04-05
		CHECKVALUE2_BINARY(name_type,  uword, 0); // 06-07
		CHECKVALUE1_BINARY(name_flags, uword   )  // 08-09
		{
			uword_t flags(0);

			CHECKFLAG   (flags, name_skill1,      uword_t(0x0001U)); // Doom
			CHECKFLAG   (flags, name_skill2,      uword_t(0x0001U)); // Doom
			CHECKFLAG   (flags, name_skill3,      uword_t(0x0002U)); // Doom
			CHECKFLAG   (flags, name_skill4,      uword_t(0x0004U)); // Doom
			CHECKFLAG   (flags, name_skill5,      uword_t(0x0004U)); // Doom
			CHECKFLAG   (flags, name_standing,    uword_t(0x0008U)); // Strife
			CHECKFLAGNOT(flags, name_single,      uword_t(0x0010U)); // Doom
			CHECKFLAGNOT(flags, name_ambush,      uword_t(0x0020U)); // Strife
			CHECKFLAG   (flags, name_friend,      uword_t(0x0080U)); // Strife
			CHECKFLAGNOT(flags, name_translucent, uword_t(0x0100U)); // Strife
			CHECKFLAGNOT(flags, name_invisible,   uword_t(0x0200U)); // Strife
		}

		return;
	}

	if (_type == type_t::type_vertex())
	{
		CHECKVALUE2_BINARY(name_x, sword, 0); // 00-01
		CHECKVALUE2_BINARY(name_y, sword, 0); // 02-03

		return;
	}
}

void LevelObject::encodeUDMF(std::ostream & out, int depth)
{
	if (_data.getType() == any_t::OBJMAP_T)
	{
		if (!depth && (_type.getMode() == type_t::MODE_INLINE)) return;

		if (depth && (_type.getMode() != type_t::MODE_INLINE))
		{
			out << get_object_index(this);
		}
		else
		{
			if (_type.getMode() == type_t::MODE_COMPOUNDOBJECT) return;
			if (_type.getMode() == type_t::MODE_VALUE) return;

			for(int i = depth; i; --i) out.put('\t');
			out << _type.makeString() << " /* " << get_object_index(this) << " */\n";

			for(int i = depth; i; --i) out.put('\t');
			out << "{\n";

			FOREACH_T(objmap_t, it, _data.getObjMap())
			{
				obj_t itObj = _data.getObjMap().get(*it);

				if (itObj->_type.getMode() == type_t::MODE_INLINE)
				{
					itObj->encodeUDMF(out, depth+1);
				}
				else
				{
					for(int i = depth+1; i; --i) out.put('\t');
					out << *it << '='; itObj->encodeUDMF(out, depth+1); out << ";\n";
				}
			}

			for(int i = depth; i; --i) out.put('\t');
			out << "}\n\n";
		}

		return;
	}

	if (!depth) return;

	_data.encodeText(out);
}



