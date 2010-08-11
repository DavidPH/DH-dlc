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
	2010/02/06 - clean_objects() broke now that the reference counting is
		fixed and in use.
	2010/02/06 - Minor cleanup.
	2010/02/16 - Use addGlobal bitfield in add_object() to prevent
		duplicate entires instead of iterating over the entire list of
		objects.
	2010/02/28 - Update for lo_type enum.
*/

#include "global_object.hpp"

#include "types.hpp"

#include "exceptions/InvalidTypeException.hpp"
#include "exceptions/ParsingException.hpp"

#include "LevelObject/LevelObject.hpp"
#include "LevelObject/LevelObjectName.hpp"
#include "LevelObject/LevelObjectType.hpp"

#include "../common/foreach.hpp"



obj_t               global_object = LevelObject::create();
global_object_map_t global_object_map;

void add_object(name_t const & name, obj_t newObject)
{
	if (newObject == NULL || !newObject->_addGlobal) return;

	// Native types can't be output and can't have their address taken.
	// While inline/compound object modes also don't need to be added for
	// output, they do need to have their index taken.
	if (newObject->getType().getNativeType() != type_t::NT_NONE) return;

	// Must not have duplicate entries in list...
	newObject->_addGlobal = false;

	global_object_list_t & typeList = global_object_map[newObject->getType()];

	newObject->_index = typeList.size();

	typeList.push_back(newObject);
}

void clean_objects()
{

}

obj_t get_object(name_t const & name)
{
	FOREACH_REVERSE_T(LevelObjectStack::stack_type, rit, LevelObjectStack::stack)
	{
		if ((*rit)->hasObject(name))
			return (*rit)->getObject(name);
	}

	return global_object->getObject(name);
}
obj_t get_object(name_t const & name, type_t const type)
{
	obj_t lop = get_object(name);

	if (lop->getType() != type)
		throw InvalidTypeException("expected:" + type.makeString());

	return lop;
}
obj_t get_object(int_s_t objectIndex, type_t const type)
{
	if (objectIndex == -1)
		return NULL;

	int_s_t typeCount = 0;

	FOREACH_T(global_object_list_t, it, global_object_map[type])
	{
		if (typeCount == objectIndex)
			return *it;

		++typeCount;
	}

	throw ParsingException("index out of bounds:" + make_string(objectIndex));
}

int_s_t get_object_index(obj_t oldObject)
{
	if (oldObject == NULL)
		return -1;

	if (oldObject->_index != (size_t)-1) return oldObject->_index;

	throw CompilerException("object has no index set");

	int_s_t typeCount = 0;

	FOREACH_T(global_object_list_t, it, global_object_map[oldObject->getType()])
	{
		if (oldObject == *it)
			return typeCount;

		++typeCount;
	}

	return -1;
}

bool has_object(name_t const & name)
{
	FOREACH_REVERSE_T(LevelObjectStack::stack_type, rit, LevelObjectStack::stack)
	{
		if ((*rit)->hasObject(name))
			return true;
	}

	return global_object->hasObject(name);
}

bool rem_object(obj_t oldObject)
{
	bool removed = false;

	size_t index = 0;

	global_object_list_t & objectList = global_object_map[oldObject->getType()];

	FOREACH_T(global_object_list_t, it, objectList)
	{
		if (oldObject == *it)
		{
			objectList.erase(it--);

			--index;

			removed = true;
		}

		if (removed)
			(*it)->_index = index;

		++index;
	}

	oldObject->_index = -1;

	return removed;
}



bool last_if_result = true;



LevelObjectStack::stack_type LevelObjectStack::stack(1, global_object);



LevelObjectStack::LevelObjectStack(obj_t p) : _p(p)
{
	LevelObjectStack::stack.push_back(_p);
}

LevelObjectStack::~LevelObjectStack()
{
	LevelObjectStack::stack.pop_back();
}



