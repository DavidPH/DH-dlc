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

#include <stdexcept>



obj_t               global_object = LevelObject::create();
global_object_map_t global_object_map;

void add_object(name_t const & name, obj_t newObject)
{
	if (newObject == NULL || !newObject->_addGlobal) return;

	// Native types can't be output and can't have their address taken.
	if (newObject->getType().getNativeType() != type_t::NT_NONE) return;

	// inline mode objects cannot sensibly have their index taken, as their
	// position in output is undefined.
	if (newObject->getType().getMode() == type_t::MODE_INLINE) return;

	// Taking the address of a compound object has no valid use, as they are
	// not output. And storing references to them by index is deprecated.
	if (newObject->getType().getMode() == type_t::MODE_COMPOUNDOBJECT) return;

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

	global_object_list_t & objectList = global_object_map[type];

	FOREACH_T(global_object_list_t, it, objectList)
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

	throw CompilerException("object has no valid index");
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
	// And object with an index of -1 is not in an output list.
	if (oldObject->_index == size_t(-1)) return false;

	bool found = false;

	global_object_list_t & objectList = global_object_map[oldObject->getType()];

	global_object_list_t::reverse_iterator it(objectList.rbegin());

	for (; it != objectList.rend(); ++it)
	{
		if (oldObject == *it)
		{
			objectList.erase(--(it.base()));
			found = true;
			break;
		}

		--(*it)->_index;
	}

	// At this point, there is no meaningful way to proceed.
	if (!found)
		throw std::invalid_argument("rem_object(obj_t):oldObject not in objectList");

	oldObject->_index = -1;

	return true;
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



