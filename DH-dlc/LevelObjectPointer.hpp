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
	A simple reference counting LevelObject pointer.

	06/02/2010 - Original version.
	16/02/2010 - No more cmp function here.
*/

#ifndef LEVELOBJECTPOINTER_H
#define LEVELOBJECTPOINTER_H

#include "types.hpp"

class LevelObject;



class LevelObjectPointer
{
	public:
		LevelObjectPointer(LevelObject * = NULL);
		LevelObjectPointer(LevelObjectPointer const &);
		~LevelObjectPointer();

		bool isLastPointer() const;

		LevelObject       * operator -> ();
		LevelObject const * operator -> () const;
		LevelObject       & operator *  ();
		LevelObject const & operator *  () const;

		bool operator == (LevelObjectPointer const &) const;

		LevelObjectPointer & operator = (LevelObjectPointer const &);

	private:
		LevelObject * _p;
};



#endif /* LEVELOBJECTPOINTER_H */



