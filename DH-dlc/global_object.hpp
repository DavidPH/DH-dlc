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
	03/02/2010 - Update for name_t.
	06/02/2010 - Minor cleanup.
*/

#ifndef GLOBAL_OBJECT_H
#define GLOBAL_OBJECT_H

#include "LevelObjectPointer.hpp"
#include "types.hpp"

#include <list>
#include <map>
#include <string>



typedef std::list<obj_t> global_object_list_t;
typedef std::map<std::string, global_object_list_t> global_object_map_t;

extern obj_t               global_object;
extern global_object_map_t global_object_map;

void add_object(name_t const &, obj_t);

void clean_objects();

obj_t get_object(name_t const &);
obj_t get_object(name_t const &, std::string const &);
obj_t get_object(int_s_t, std::string const &);

int_s_t get_object_index(obj_t);

bool has_object(name_t const &);



extern bool last_if_result;



class LevelObjectStack
{
	public:
		LevelObjectStack(obj_t=NULL);
		~LevelObjectStack();

		friend obj_t get_object(name_t const &);

		friend bool has_object(name_t const &);

		typedef std::list<obj_t> stack_type;

	private:
		obj_t _p;

		LevelObjectStack & operator = (LevelObjectStack const & other);

		static stack_type stack;
};



#endif /* GLOBAL_OBJECT_H */



