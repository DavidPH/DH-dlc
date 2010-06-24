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

#include "LevelObjectPointer.hpp"

#include "LevelObject.hpp"
#include "math.hpp"
#include "types.hpp"



LevelObjectPointer::LevelObjectPointer(LevelObject * p) : _p(p)
{
	//std::cerr << "LevelObject::LevelObject(" << p << ");\n";

	//std::cerr << "  _p == " << _p << ";\n";
	//if (_p) std::cerr << "  _p->_refCount == " << _p->_refCount << ";\n";

	if (_p) ++_p->_refCount;

	//std::cerr << "LevelObject::LevelObject(" << p << ") return;\n";
}
LevelObjectPointer::LevelObjectPointer(LevelObjectPointer const & p) : _p(p._p)
{
	//std::cerr << "LevelObject::LevelObject(obj_t(" << p._p << "));\n";

	//std::cerr << "  _p == " << _p << ";\n";
	//if (_p) std::cerr << "  _p->_refCount == " << _p->_refCount << ";\n";

	if (_p) ++_p->_refCount;

	//std::cerr << "LevelObject::LevelObject(obj_t(" << p._p << ")) return;\n";
}
LevelObjectPointer::~LevelObjectPointer()
{
	//std::cerr << "LevelObject::~LevelObject();\n";

	//std::cerr << "  _p == " << _p << ";\n";
	//if (_p) std::cerr << "  _p->_refCount == " << _p->_refCount << ";\n";

	if (_p && (--_p->_refCount == 0))
	{
		//std::cerr << "  delete _p;\n";
		delete _p;
	}

	//std::cerr << "LevelObject::~LevelObject() return;\n";
}

bool LevelObjectPointer::isLastPointer() const
{
	if (_p) return _p->_refCount == 1;
	else    return true;
}

LevelObject       * LevelObjectPointer::operator -> ()       {return  _p;}
LevelObject const * LevelObjectPointer::operator -> () const {return  _p;}
LevelObject       & LevelObjectPointer::operator *  ()       {return *_p;}
LevelObject const & LevelObjectPointer::operator *  () const {return *_p;}

bool LevelObjectPointer::operator == (const LevelObjectPointer & other) const
{
	return this->_p == other._p;
}

LevelObjectPointer & LevelObjectPointer::operator = (const LevelObjectPointer & p)
{
	//std::cerr << "LevelObject::operator=(" << p._p << ");\n";

	// DO NOT CHANGE THE ORDER OF THESE STATEMENTS!
	// (This order properly handles self-assignment)
	// (This order also properly handles recursion)

	LevelObject* const old = this->_p;

	this->_p = p._p;

	//std::cerr << "  _p == " << _p << ";\n";
	//if (_p) std::cerr << "  _p->_refCount == " << _p->_refCount << ";\n";

	if (this->_p) ++_p->_refCount;

	//std::cerr << "  old == " << old << ";\n";
	//if (old) std::cerr << "  old->_refCount == " << old->_refCount << ";\n";

	if (old && --old->_refCount == 0)
	{
		//std::cerr << "  delete old;\n";
		delete old;
	}

	//std::cerr << "LevelObject::operator=(" << p._p << ") return;\n";

	return *this;
}



