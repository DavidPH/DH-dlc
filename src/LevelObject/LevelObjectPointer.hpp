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

	2010/02/06 - Original version.
	2010/02/16 - No more cmp function here.
	2010/06/30 - Inlined the functions.
*/

#ifndef LEVELOBJECTPOINTER_H
#define LEVELOBJECTPOINTER_H

#include "LevelObject.hpp"

#include "../types.hpp"



class LevelObjectPointer
{
	public:
		LevelObjectPointer(LevelObject * = NULL);
		LevelObjectPointer(LevelObjectPointer const &);
		~LevelObjectPointer();

		bool isLastPointer() const;

		LevelObject * operator -> () const;
		LevelObject & operator *  () const;

		bool operator == (LevelObjectPointer const &) const;

		LevelObjectPointer & operator = (LevelObjectPointer const &);

	private:
		LevelObject * _p;
};



inline LevelObjectPointer::LevelObjectPointer(LevelObject * p) : _p(p)
{
	if (_p) ++_p->_refCount;
}
inline LevelObjectPointer::LevelObjectPointer(LevelObjectPointer const & p) : _p(p._p)
{
	if (_p) ++_p->_refCount;
}
inline LevelObjectPointer::~LevelObjectPointer()
{
	if (_p && (--_p->_refCount == 0))
		delete _p;
}

inline bool LevelObjectPointer::isLastPointer() const
{
	if (_p) return _p->_refCount == 1;
	else    return true;
}

inline LevelObject * LevelObjectPointer::operator -> () const {return  _p;}
inline LevelObject & LevelObjectPointer::operator *  () const {return *_p;}

inline bool LevelObjectPointer::operator == (const LevelObjectPointer & other) const
{
	return this->_p == other._p;
}

inline LevelObjectPointer & LevelObjectPointer::operator = (const LevelObjectPointer & p)
{
	// DO NOT CHANGE THE ORDER OF THESE STATEMENTS!
	// (This order properly handles self-assignment)
	// (This order also properly handles recursion)

	LevelObject* const old = this->_p;

	this->_p = p._p;

	if (this->_p) ++_p->_refCount;

	if (old && --old->_refCount == 0)
		delete old;

	return *this;
}



#endif /* LEVELOBJECTPOINTER_H */



