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

#include "LevelObjectType.hpp"

#include "../global_object.hpp"
#include "../options.hpp"
#include "../types.hpp"

#include "../exceptions/InvalidTypeException.hpp"
#include "../exceptions/NoDefaultTypeException.hpp"

#include "../types/string_t.hpp"

#include "../../common/foreach.hpp"



LevelObjectType const LevelObjectType::type_null;
// default_type_map[context][name] = type
LevelObjectType::default_type_map_t  LevelObjectType::default_type_map;
LevelObjectType::mode_vector_t       LevelObjectType::mode_vector(1, LevelObjectType::MODE_NONE);
LevelObjectType::redirect_type_map_t LevelObjectType::redirect_type_map;
LevelObjectType::type_map_t          LevelObjectType::type_map;

std::string LevelObjectType::makeString() const
{
	FOREACH_T(type_map_t, it, type_map)
		if (it->second == *this) return it->first;

	return "!!!NOT A TYPE!!!"; // SERIOUSLY!!!
}

void LevelObjectType::add_default_type(LevelObjectName const & name, LevelObjectType const context, LevelObjectType const type)
{
	default_type_map[context][name] = type;
}
LevelObjectType LevelObjectType::get_default_type(LevelObjectName const & name, LevelObjectType const context)
{
	if (!has_default_type(name, context))
		throw NoDefaultTypeException(make_string(name));

	return default_type_map[context][name];
}
bool LevelObjectType::has_default_type(LevelObjectName const & name, LevelObjectType const context)
{
	default_type_map_t::iterator mapIt = default_type_map.find(context);

	if (mapIt == default_type_map.end()) return false;

	default_type_map_context_t::iterator contextIt = mapIt->second.find(name);

	if (contextIt == mapIt->second.end()) return false;

	return true;
}

void LevelObjectType::add_redirect_type(std::string const & type_name, LevelObjectType const type)
{
	redirect_type_map[type_name] = type;
}
LevelObjectType LevelObjectType::get_redirect_type(std::string const & type_name)
{
	if (!has_redirect_type(type_name))
		throw InvalidTypeException(type_name);

	return redirect_type_map[type_name];
}
bool LevelObjectType::has_redirect_type(std::string const & type_name)
{
	redirect_type_map_t::iterator redirectIt = redirect_type_map.find(type_name);

	if (redirectIt == redirect_type_map.end()) return false;

	return true;
}

void LevelObjectType::add_type(std::string const & type_name, LevelObjectType::Mode const mode)
{
	type_map[type_name] = LevelObjectType(mode_vector.size());
	mode_vector.push_back(mode);
}
LevelObjectType LevelObjectType::get_type(std::string const & type_name)
{
	LevelObjectType type;

	if (has_redirect_type(type_name))
	{
		type = get_redirect_type(type_name);
	}
	else
	{
		if (!has_type(type_name))
			throw InvalidTypeException(type_name);

		type = type_map[type_name];
	}

	if (type.getMode() == MODE_NONE)
		throw InvalidTypeException(type_name);

	if (type.getMode() != MODE_DYNAMIC) return type;

	if ((type_name == type_name_returntype()) && has_object(name_t::name_return_type))
	{
		obj_t return_type = get_object(name_t::name_return_type);

		return get_type(to_string(return_type).makeString());
	}

	throw InvalidTypeException(type_name);
}
bool LevelObjectType::has_type(std::string const & type_name)
{
	if (has_redirect_type(type_name)) return true;

	type_map_t::iterator typeIt = type_map.find(type_name);

	if (typeIt == type_map.end()) return false;

	return true;
}

#define MAKE_type_X(TYPE) \
LevelObjectType LevelObjectType::type_##TYPE() \
{ \
	static LevelObjectType type = type_null; \
	\
	if ((type == type_null) && has_type(type_name_##TYPE())) \
		type = get_type(type_name_##TYPE()); \
	\
	return type; \
}
MAKE_type_X(bool)
MAKE_type_X(shortint)
MAKE_type_X(int)
MAKE_type_X(longint)
MAKE_type_X(shortfloat)
MAKE_type_X(float)
MAKE_type_X(longfloat)
MAKE_type_X(string)
MAKE_type_X(string8)
MAKE_type_X(sword)
MAKE_type_X(ubyte)
MAKE_type_X(uword)
MAKE_type_X(linedef)
MAKE_type_X(sector)
MAKE_type_X(sidedef)
MAKE_type_X(thing)
MAKE_type_X(vertex)
#undef MAKE_type_X



