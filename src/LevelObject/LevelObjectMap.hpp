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

#ifndef LEVELOBJECTMAP_H
#define LEVELOBJECTMAP_H

#include "LevelObjectName.hpp"

#include "../types.hpp"

#include <list>
#include <map>
#include <utility>



class LevelObjectMap
{
	public:
		typedef std::pair<name_t, obj_t> pair_t;
		typedef std::list<pair_t> list_t;
		typedef std::map<name_t, obj_t> map_t;
		typedef list_t::const_iterator const_iterator;
		typedef list_t::iterator iterator;

		explicit LevelObjectMap();
		         LevelObjectMap(LevelObjectMap const &);

		void  add(name_t const &, obj_t);
		void  add(obj_t);
		void  del(name_t const &);
		obj_t get(name_t const &);
		bool  has(name_t const &) const;

		const_iterator begin() const {return _objList.begin();}
		      iterator begin()       {return _objList.begin();}
		const_iterator end() const {return _objList.end();}
		      iterator end()       {return _objList.end();}

		LevelObjectMap & operator += (LevelObjectMap &);

	private:
		map_t  _objMap;
		list_t _objList;
};



#endif /* LEVELOBJECTMAP_H */



