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
	2010/02/03 - Update for name_t.
	2010/02/06 - refCount should be initialized to 0. Except for object
		constructors, which must initialize to 1 then decrement at end
		of function. This is because the object is put into use and has
		to consider itself a reference to itself.
	2010/06/02 - LevelObject::create functions no longer inline.
	2010/02/15 - Add isBreaked and isContinued to init list.
	2010/02/15 - Added operator < and operator += functions.
	2010/02/16 - Added addGlobal to the init list.
	2010/02/16 - Removed the math functions that operated on LevelObjects.
	2010/02/28 - Update for lo_type enum.
*/

#include "LevelObject.hpp"

#include <ostream>

#include "compound_objects.hpp"
#include "global_object.hpp"
#include "LevelObjectName.hpp"
#include "lo_types.hpp"
#include "math.hpp"
#include "options.hpp"
#include "parsing.hpp"
#include "scripts.hpp"
#include "SourceStream.hpp"
#include "types.hpp"
#include "../common/foreach.hpp"
#include "exceptions/InvalidTypeException.hpp"
#include "exceptions/NoSuchElementException.hpp"
#include "exceptions/UnknownCommandException.hpp"
#include "types/binary.hpp"
#include "types/int_t.hpp"
#include "types/real_t.hpp"
#include "types/string_t.hpp"



// The initialization list for the constructors are all basically the same.
#define LevelObject_INIT_LIST(TYPE, DATA) _refCount(0), data(DATA), type(TYPE), addGlobal(true), isBreaked(false), isCompounded(false), isContinued(false), isReturned(false)

LevelObject::LevelObject() : LevelObject_INIT_LIST(, objmap_t()) {}
LevelObject::LevelObject(LevelObject const & other) : LevelObject_INIT_LIST(other.type, other.data) {}
LevelObject::LevelObject(std::string const & type) : LevelObject_INIT_LIST(type, false)
{
	     if (type == type_name_bool())       data = bool_t(0);

	else if (type == type_name_shortint())   data = int_s_t(0);
	else if (type == type_name_int())        data = int_t(int_s_t(0));
	else if (type == type_name_longint())    data = int_l_t(0);

	else if (type == type_name_shortfloat()) data = real_s_t(0);
	else if (type == type_name_float())      data = real_t(0);
	else if (type == type_name_longfloat())  data = real_l_t(0);

	else if (type == type_name_string())     data = string_t("");
	else if (type == type_name_string8())    data = string8_t("");

	else if (type == type_name_sword())      data = sword_t(0);

	else if (type == type_name_ubyte())      data = ubyte_t(0);

	else if (type == type_name_uword())      data = uword_t(0);

	else                                     data = objmap_t();
}
LevelObject::LevelObject(std::string const & type, std::string const & value) : LevelObject_INIT_LIST(type, false)
{
	     if (type == type_name_bool())       data = parse_bool(value);

	else if (type == type_name_shortint())   data = parse_int_s(value);
	else if (type == type_name_int())        data = parse_int(value);
	else if (type == type_name_longint())    data = parse_int_l(value);

	else if (type == type_name_shortfloat()) data = parse_real_s(value);
	else if (type == type_name_float())      data = parse_real(value);
	else if (type == type_name_longfloat())  data = parse_real_l(value);

	else if (type == type_name_string())     data = parse_string(value);
	else if (type == type_name_string8())    data = parse_string8(value);

	else if (type == type_name_sword())      data = parse_sword(value);

	else if (type == type_name_ubyte())      data = parse_ubyte(value);

	else if (type == type_name_uword())      data = parse_uword(value);

	else                                     data = objmap_t();
}
LevelObject::LevelObject(std::string const & type, std::string const & data, std::vector<std::string> const & base) : LevelObject_INIT_LIST(type, objmap_t())
{
	++this->_refCount;

	for (size_t index = 0; index < base.size(); ++index)
		this->addBase(base[index]);

	this->addData(data);

	// (data.size() != 0), so that we don't compound if no keys have been set.
	// Used to be (this->dataObject.size() != 0), but this would cause derived
	// compound objects to be compounded even if they were no different.
	if ((get_lo_type(this->type) == LO_TYPE_COMPOUNDOBJECT) && (!this->isCompounded && data.size() != 0))
	{
		this->addData(get_compound_object(this->type), this->type);
		this->isCompounded = true;
	}

	--this->_refCount;
}

LevelObject::~LevelObject() {}



obj_t LevelObject::create()
{
	return new LevelObject();
}
obj_t LevelObject::create(std::string const & type)
{
	return new LevelObject(type);
}
obj_t LevelObject::create(std::string const & type, std::string const & value)
{
	return new LevelObject(type, value);
}
obj_t LevelObject::create(std::string const & type, std::string const & data, std::vector<std::string> const & base)
{
	return new LevelObject(type, data, base);
}



obj_t LevelObject::getObject(name_t const & name)
{
	//std::cerr << "LevelObject::getObject('" << name << "')\n";

	if (this->data.getType() != any_t::OBJMAP_T)
		throw InvalidTypeException("non-objects have no keys");

	if (name.size() != 1)
	{
		if (name.getString() == misc_name_global())
			return global_object->getObject(name.getRest());

		if (name.getString() == misc_name_this())
			return this->getObject(name.getRest());

		return this->getObject(name.getFirst())->getObject(name.getRest());
	}

	objmap_t::iterator elemIterator = this->data.getObjMap().find(name);

	if (elemIterator == this->data.getObjMap().end())
	{
		//FOREACH_T(objmap_t, it, this->data.getObjMap())
		//	std::cerr << "  " << it->first << '\n';

		throw NoSuchElementException(name.getString());
	}

	return elemIterator->second;
}

bool LevelObject::hasObject(name_t const & name)
{
	if (this->data.getType() != any_t::OBJMAP_T)
		return false;

	if (name.size() != 1)
	{
		if (name.getString() == misc_name_global())
			return global_object->hasObject(name.getRest());

		if (name.getString() == misc_name_this())
			return this->hasObject(name.getRest());

		if (!this->hasObject(name.getFirst()))
			return false;

		return this->getObject(name.getFirst())->hasObject(name.getRest());
	}

	objmap_t::iterator elemIterator = this->data.getObjMap().find(name);

	if (elemIterator == this->data.getObjMap().end())
		return false;

	return true;
}



std::string const & LevelObject::getType() const
{
	return type;
}



void LevelObject::setType(std::string const & newType, std::string const & value)
{
	if (this->type.empty())
		throw InvalidTypeException("cannot change type");

	this->type = newType;

	     if (type == type_name_bool())       data = parse_bool(value);

	else if (type == type_name_shortint())   data = parse_int_s(value);
	else if (type == type_name_int())        data = parse_int(value);
	else if (type == type_name_longint())    data = parse_int_l(value);

	else if (type == type_name_shortfloat()) data = parse_real_s(value);
	else if (type == type_name_float())      data = parse_real(value);
	else if (type == type_name_longfloat())  data = parse_real_l(value);

	else if (type == type_name_string())     data = parse_string(value);

	else if (data.getType() != any_t::OBJMAP_T) data = to_obj(data)->data.getObjMap();
}



std::ostream & LevelObject::printOn (std::ostream & out, int indent) const
{
	out << '[' << this->getType() << ']';

	if (this->data.getType() == any_t::OBJMAP_T)
	{
		out << '\n';

		for (int i = indent; i; --i) out << "  ";

		out << "{\n";

		FOREACH_T_CONST(objmap_t, it, this->data.getObjMap())
		{
			for (int i = indent + 1; i; --i) out << "  ";

			out << it->first << '=';
			it->second->printOn(out, indent+1);
		}

		for (int i = indent; i; --i) out << "  ";

		out << '}';
	}
	else
		out << ' ' << make_string(this->data) << ';';

	out << '\n';

	return out;
}



LevelObject & LevelObject::operator = (LevelObject const & other)
{
	this->data = other.data;
	this->type = other.type;

	return *this;
}



std::ostream & operator << (std::ostream & out, LevelObject const & in)
{
	out << '[' << in.getType() << ']';

	if (in.data.getType() != any_t::OBJMAP_T)
		out << ' ' << make_string(in.data);

	return out;
}



