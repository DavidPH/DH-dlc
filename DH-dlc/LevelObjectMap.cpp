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

#include "LevelObjectMap.hpp"

#include "LevelObjectPointer.hpp"

#include "../common/foreach.hpp"
#include "exceptions/NoSuchElementException.hpp"

#include <iostream>



LevelObjectMap::LevelObjectMap() : _objMap(), _objList()
{

}
LevelObjectMap::LevelObjectMap(LevelObjectMap const & other) : _objMap(other._objMap), _objList(other._objList)
{

}

void LevelObjectMap::add(name_t const & name, obj_t obj)
{
	if (!has(name))
		_objList.push_back(pair_t(name, obj));
	else
	{
		FOREACH_T(list_t, it, _objList)
		{
			if (it->first == name)
			{
				it->second = obj;
				break;
			}
		}
	}

	_objMap[name] = obj;
}
void LevelObjectMap::add(obj_t obj)
{
	_objList.push_back(pair_t(name_t(""), obj));
}
void LevelObjectMap::del(name_t const & name)
{
	_objMap.erase(name);

	FOREACH_T(list_t, it, _objList)
	{
		if (it->first == name)
		{
			_objList.erase(it);
			break;
		}
	}
}
obj_t LevelObjectMap::get(name_t const & name)
{
	if (!has(name))
		throw NoSuchElementException(name.getString());

	return _objMap[name];
}
bool LevelObjectMap::has(name_t const & name) const
{
	if (_objMap.find(name) == _objMap.end())
		return false;

	return true;
}

LevelObjectMap & LevelObjectMap::operator += (LevelObjectMap & other)
{
	FOREACH_T(LevelObjectMap, it, other)
		add(it->first, other.get(it->first));

	return *this;
}



