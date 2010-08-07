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

*/

#ifndef LEVELOBJECTTYPE_H
#define LEVELOBJECTTYPE_H

#include "LevelObjectName.hpp"

#include <map>
#include <string>
#include <vector>



class LevelObjectType
{
public:
	enum Mode
	{
		MODE_NONE,
		MODE_DYNAMIC,
		MODE_VALUE,
		MODE_OBJECT,
		MODE_COMPOUNDOBJECT,
		MODE_INLINE,
	};

	typedef unsigned short index_t;
	typedef std::map<LevelObjectName, LevelObjectType> default_type_map_context_t;
	typedef std::map<LevelObjectType, default_type_map_context_t> default_type_map_t;
	typedef std::vector<Mode> mode_vector_t;
	typedef std::map<std::string, LevelObjectType> redirect_type_map_t;
	typedef std::map<std::string, LevelObjectType> type_map_t;

	LevelObjectType();

	Mode getMode() const;
	std::string makeString() const;

	friend int cmp(LevelObjectType const, LevelObjectType const);

	static void            add_default_type(LevelObjectName const & name, LevelObjectType const context, LevelObjectType const type);
	static LevelObjectType get_default_type(LevelObjectName const & name, LevelObjectType const context = type_null);
	static bool            has_default_type(LevelObjectName const & name, LevelObjectType const context = type_null);

	static void            add_redirect_type(std::string const & type_name, LevelObjectType const type);

	static void            add_type(std::string const & type_name, Mode const mode);
	static LevelObjectType get_type(std::string const & type_name);
	static bool            has_type(std::string const & type_name);

	// Internally used types. Will return type_null if the type hasn't been
	// defined yet. (Will also cache the result once it is defined.)
	static LevelObjectType type_bool();
	static LevelObjectType type_shortint();
	static LevelObjectType type_int();
	static LevelObjectType type_longint();
	static LevelObjectType type_shortfloat();
	static LevelObjectType type_float();
	static LevelObjectType type_longfloat();
	static LevelObjectType type_string();
	static LevelObjectType type_string8();
	static LevelObjectType type_string16();
	static LevelObjectType type_string32();
	static LevelObjectType type_string80();
	static LevelObjectType type_string320();
	static LevelObjectType type_ubyte();
	static LevelObjectType type_sword();
	static LevelObjectType type_uword();
	static LevelObjectType type_sdword();
	static LevelObjectType type_udword();
	static LevelObjectType type_choice();
	static LevelObjectType type_conversation();
	static LevelObjectType type_cost();
	static LevelObjectType type_ifitem();
	static LevelObjectType type_linedef();
	static LevelObjectType type_page();
	static LevelObjectType type_sector();
	static LevelObjectType type_sidedef();
	static LevelObjectType type_thing();
	static LevelObjectType type_vertex();

	template<typename T>
	static LevelObjectType type_auto();

	static LevelObjectType const type_null;

private:
	explicit LevelObjectType(index_t const index);

	index_t _index;

	// These functions are only for internal use.
	static LevelObjectType get_redirect_type(std::string const & type_name);
	static bool            has_redirect_type(std::string const & type_name);

	static default_type_map_t default_type_map;
	static mode_vector_t mode_vector;
	static redirect_type_map_t redirect_type_map;
	static type_map_t type_map;
};



int cmp(LevelObjectType const l, LevelObjectType const r);

bool operator == (LevelObjectType const l, LevelObjectType const r);
bool operator != (LevelObjectType const l, LevelObjectType const r);
bool operator >= (LevelObjectType const l, LevelObjectType const r);
bool operator >  (LevelObjectType const l, LevelObjectType const r);
bool operator <= (LevelObjectType const l, LevelObjectType const r);
bool operator <  (LevelObjectType const l, LevelObjectType const r);



inline LevelObjectType::LevelObjectType() : _index(0)
{

}
inline LevelObjectType::LevelObjectType(LevelObjectType::index_t const index) : _index(index)
{

}

inline LevelObjectType::Mode LevelObjectType::getMode() const
{
	return mode_vector[_index];
}



inline int cmp(LevelObjectType const l, LevelObjectType const r)
{
	if (l._index > r._index) return +1;
	if (l._index < r._index) return -1;
	return 0;
}

inline bool operator == (LevelObjectType const l, LevelObjectType const r) {return cmp(l, r) == 0;}
inline bool operator != (LevelObjectType const l, LevelObjectType const r) {return cmp(l, r) != 0;}
inline bool operator >= (LevelObjectType const l, LevelObjectType const r) {return cmp(l, r) >= 0;}
inline bool operator >  (LevelObjectType const l, LevelObjectType const r) {return cmp(l, r) >  0;}
inline bool operator <= (LevelObjectType const l, LevelObjectType const r) {return cmp(l, r) <= 0;}
inline bool operator <  (LevelObjectType const l, LevelObjectType const r) {return cmp(l, r) <  0;}



#endif /* LEVELOBJECTTYPE_H */



