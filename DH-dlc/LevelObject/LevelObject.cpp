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

#include "LevelObjectMap.hpp"
#include "LevelObjectName.hpp"

#include "../compound_objects.hpp"
#include "../global_object.hpp"
#include "../math.hpp"
#include "../options.hpp"
#include "../parsing.hpp"
#include "../scripts.hpp"
#include "../SourceStream.hpp"
#include "../types.hpp"

#include "../exceptions/InvalidTypeException.hpp"
#include "../exceptions/NoSuchElementException.hpp"
#include "../exceptions/UnknownCommandException.hpp"

#include "../types/binary.hpp"
#include "../types/int_t.hpp"
#include "../types/real_t.hpp"
#include "../types/string_t.hpp"

#include "../../common/foreach.hpp"

#include <ostream>



// The initialization list for the constructors are all basically the same.
#define LevelObject_INIT_LIST(TYPE, DATA) _index(-1), _refCount(0), _data(DATA), _type(TYPE), _addGlobal(true), _isBreaked(false), _isCompounded(false), _isContinued(false), _isReturned(false)

LevelObject::LevelObject() : LevelObject_INIT_LIST(, objmap_t()) {}
LevelObject::LevelObject(LevelObject const & other) : LevelObject_INIT_LIST(other._type, other._data) {}
LevelObject::LevelObject(type_t const type) : LevelObject_INIT_LIST(type, false)
{
	     if (_type == type_t::type_bool())       _data = bool_t(0);

	else if (_type == type_t::type_shortint())   _data = int_s_t(0);
	else if (_type == type_t::type_int())        _data = int_t(int_s_t(0));
	else if (_type == type_t::type_longint())    _data = int_l_t(0);

	else if (_type == type_t::type_shortfloat()) _data = real_s_t(0);
	else if (_type == type_t::type_float())      _data = real_t(0);
	else if (_type == type_t::type_longfloat())  _data = real_l_t(0);

	else if (_type == type_t::type_string())     _data = string_t("");
	else if (_type == type_t::type_string8())    _data = string8_t("");
	else if (_type == type_t::type_string16())   _data = string16_t("");
	else if (_type == type_t::type_string32())   _data = string32_t("");
	else if (_type == type_t::type_string80())   _data = string80_t("");
	else if (_type == type_t::type_string320())  _data = string320_t("");

	else if (_type == type_t::type_ubyte())      _data = ubyte_t(0);
	else if (_type == type_t::type_sword())      _data = sword_t(0);
	else if (_type == type_t::type_uword())      _data = uword_t(0);
	else if (_type == type_t::type_sdword())     _data = sdword_t(0);
	else if (_type == type_t::type_udword())     _data = udword_t(0);

	else                                         _data = objmap_t();
}
LevelObject::LevelObject(type_t const type, any_t const & data) : LevelObject_INIT_LIST(type, data) {}
LevelObject::LevelObject(type_t const type, SourceScannerDHLX & sc) : LevelObject_INIT_LIST(type, false)
{
	     if (_type == type_t::type_bool())       _data = parse_bool(sc);

	else if (_type == type_t::type_shortint())   _data = parse_int_s(sc);
	else if (_type == type_t::type_int())        _data = parse_int(sc);
	else if (_type == type_t::type_longint())    _data = parse_int_l(sc);

	else if (_type == type_t::type_shortfloat()) _data = parse_real_s(sc);
	else if (_type == type_t::type_float())      _data = parse_real(sc);
	else if (_type == type_t::type_longfloat())  _data = parse_real_l(sc);

	else if (_type == type_t::type_string())     _data = parse_string(sc);
	else if (_type == type_t::type_string8())    _data = parse_string8(sc);
	else if (_type == type_t::type_string16())   _data = parse_string16(sc);
	else if (_type == type_t::type_string32())   _data = parse_string32(sc);
	else if (_type == type_t::type_string80())   _data = parse_string80(sc);
	else if (_type == type_t::type_string320())  _data = parse_string320(sc);

	else if (_type == type_t::type_ubyte())      _data = parse_ubyte(sc);
	else if (_type == type_t::type_sword())      _data = parse_sword(sc);
	else if (_type == type_t::type_uword())      _data = parse_uword(sc);
	else if (_type == type_t::type_sdword())     _data = parse_sdword(sc);
	else if (_type == type_t::type_udword())     _data = parse_udword(sc);

	else                                         _data = objmap_t();
}
LevelObject::LevelObject(type_t const type, std::string const & value) : LevelObject_INIT_LIST(type, false)
{
	     if (_type == type_t::type_bool())       _data = parse_bool(value);

	else if (_type == type_t::type_shortint())   _data = parse_int_s(value);
	else if (_type == type_t::type_int())        _data = parse_int(value);
	else if (_type == type_t::type_longint())    _data = parse_int_l(value);

	else if (_type == type_t::type_shortfloat()) _data = parse_real_s(value);
	else if (_type == type_t::type_float())      _data = parse_real(value);
	else if (_type == type_t::type_longfloat())  _data = parse_real_l(value);

	else if (_type == type_t::type_string())     _data = parse_string(value);
	else if (_type == type_t::type_string8())    _data = parse_string8(value);
	else if (_type == type_t::type_string16())   _data = parse_string16(value);
	else if (_type == type_t::type_string32())   _data = parse_string32(value);
	else if (_type == type_t::type_string80())   _data = parse_string80(value);
	else if (_type == type_t::type_string320())  _data = parse_string320(value);

	else if (_type == type_t::type_ubyte())      _data = parse_ubyte(value);
	else if (_type == type_t::type_sword())      _data = parse_sword(value);
	else if (_type == type_t::type_uword())      _data = parse_uword(value);
	else if (_type == type_t::type_sdword())     _data = parse_sdword(value);
	else if (_type == type_t::type_udword())     _data = parse_udword(value);

	else                                         _data = objmap_t();
}
LevelObject::LevelObject(type_t const type, std::string const & data, std::vector<std::string> const & base) : LevelObject_INIT_LIST(type, objmap_t())
{
	++_refCount;

	for (size_t index = 0; index < base.size(); ++index)
		addBase(base[index]);

	addData(data);

	// (data.size() != 0), so that we don't compound if no keys have been set.
	// Used to be (this->dataObject.size() != 0), but this would cause derived
	// compound objects to be compounded even if they were no different.
	if ((_type.getMode() == type_t::MODE_COMPOUNDOBJECT) && (!_isCompounded && data.size() != 0))
	{
		addData(get_compound_object(_type.makeString()), _type.makeString());
		_isCompounded = true;
	}

	--_refCount;
}

LevelObject::~LevelObject() {}



obj_t LevelObject::create()
{
	return new LevelObject();
}
obj_t LevelObject::create(type_t const type)
{
	return new LevelObject(type);
}
obj_t LevelObject::create(type_t const type, any_t const & data)
{
	return new LevelObject(type, data);
}
obj_t LevelObject::create(type_t const type, SourceScannerDHLX & sc)
{
	return new LevelObject(type, sc);
}
obj_t LevelObject::create(type_t const type, std::string const & value)
{
	return new LevelObject(type, value);
}
obj_t LevelObject::create(type_t const type, std::string const & data, std::vector<std::string> const & base)
{
	return new LevelObject(type, data, base);
}



obj_t LevelObject::getObject(name_t const & name)
{
	if (_data.getType() != any_t::OBJMAP_T)
		throw InvalidTypeException("non-objects have no keys");

	if (name.size() != 1)
	{
		if (name.getString() == misc_name_global())
			return global_object->getObject(name.getRest());

		if (name.getString() == misc_name_this())
			return getObject(name.getRest());

		return getObject(name.getFirst())->getObject(name.getRest());
	}

	if (!_data.getObjMap().has(name))
		throw NoSuchElementException(name.getString());

	return _data.getObjMap().get(name);
}

bool LevelObject::hasObject(name_t const & name)
{
	if (_data.getType() != any_t::OBJMAP_T)
		return false;

	if (name.size() != 1)
	{
		if (name.getString() == misc_name_global())
			return global_object->hasObject(name.getRest());

		if (name.getString() == misc_name_this())
			return hasObject(name.getRest());

		if (!hasObject(name.getFirst()))
			return false;

		return getObject(name.getFirst())->hasObject(name.getRest());
	}

	if (!_data.getObjMap().has(name))
		return false;

	return true;
}



type_t LevelObject::getType() const
{
	return _type;
}



void LevelObject::setType(type_t const newType, std::string const & value)
{
	if (_type == type_t::type_null)
		throw InvalidTypeException("cannot change type");

	_addGlobal = rem_object(this) || _addGlobal;

	_type = newType;

	     if (_type == type_t::type_bool())       _data = parse_bool(value);

	else if (_type == type_t::type_shortint())   _data = parse_int_s(value);
	else if (_type == type_t::type_int())        _data = parse_int(value);
	else if (_type == type_t::type_longint())    _data = parse_int_l(value);

	else if (_type == type_t::type_shortfloat()) _data = parse_real_s(value);
	else if (_type == type_t::type_float())      _data = parse_real(value);
	else if (_type == type_t::type_longfloat())  _data = parse_real_l(value);

	else if (_type == type_t::type_string())     _data = parse_string(value);
	else if (_type == type_t::type_string8())    _data = parse_string8(value);
	else if (_type == type_t::type_string16())   _data = parse_string16(value);
	else if (_type == type_t::type_string32())   _data = parse_string32(value);
	else if (_type == type_t::type_string80())   _data = parse_string80(value);
	else if (_type == type_t::type_string320())  _data = parse_string320(value);

	else if (_type == type_t::type_ubyte())      _data = parse_ubyte(value);
	else if (_type == type_t::type_sword())      _data = parse_sword(value);
	else if (_type == type_t::type_uword())      _data = parse_uword(value);
	else if (_type == type_t::type_sdword())     _data = parse_sdword(value);
	else if (_type == type_t::type_udword())     _data = parse_udword(value);

	else if (_data.getType() != any_t::OBJMAP_T) _data = to_obj(_data)->_data.getObjMap();

	add_object(name_t(""), this);
}



std::ostream & LevelObject::printOn (std::ostream & out, int indent)
{
	out << '[' << getType() << ']';

	if (_data.getType() == any_t::OBJMAP_T)
	{
		out << '\n';

		for (int i = indent; i; --i) out << "  ";

		out << "{\n";

		FOREACH_T_CONST(objmap_t, it, _data.getObjMap())
		{
			for (int i = indent + 1; i; --i) out << "  ";

			out << it->first << '=';
			it->second->printOn(out, indent+1);
		}

		for (int i = indent; i; --i) out << "  ";

		out << '}';
	}
	else
		out << ' ' << make_string(_data) << ';';

	out << '\n';

	return out;
}



LevelObject & LevelObject::operator = (LevelObject const & other)
{
	_data = other._data;
	_type = other._type;

	return *this;
}



std::ostream & operator << (std::ostream & out, LevelObject const & in)
{
	out << '[' << in.getType() << ']';

	if (in._data.getType() != any_t::OBJMAP_T)
		out << ' ' << make_string(in._data);

	return out;
}


