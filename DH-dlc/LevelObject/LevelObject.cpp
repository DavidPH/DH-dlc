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
#include "../scripts.hpp"
#include "../SourceStream.hpp"
#include "../types.hpp"

#include "../exceptions/InvalidTypeException.hpp"
#include "../exceptions/NoSuchElementException.hpp"
#include "../exceptions/UnknownCommandException.hpp"

#include "../parsing/parsing.hpp"

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
	switch (_type.getNativeType())
	{
	case LevelObjectType::NT_NONE:        _data = objmap_t();    break;

	case LevelObjectType::NT_BOOL_T:      _data = bool_t();      break;

	case LevelObjectType::NT_INT_S_T:     _data = int_s_t();     break;
	case LevelObjectType::NT_INT_T:       _data = int_t();       break;
	case LevelObjectType::NT_INT_L_T:     _data = int_l_t();     break;

	case LevelObjectType::NT_REAL_S_T:    _data = real_s_t();    break;
	case LevelObjectType::NT_REAL_T:      _data = real_t();      break;
	case LevelObjectType::NT_REAL_L_T:    _data = real_l_t();    break;

	case LevelObjectType::NT_STRING_T:    _data = string_t();    break;
	case LevelObjectType::NT_STRING8_T:   _data = string8_t();   break;
	case LevelObjectType::NT_STRING16_T:  _data = string16_t();  break;
	case LevelObjectType::NT_STRING32_T:  _data = string32_t();  break;
	case LevelObjectType::NT_STRING80_T:  _data = string80_t();  break;
	case LevelObjectType::NT_STRING320_T: _data = string320_t(); break;

	case LevelObjectType::NT_UBYTE_T:     _data = ubyte_t();     break;
	case LevelObjectType::NT_SWORD_T:     _data = sword_t();     break;
	case LevelObjectType::NT_UWORD_T:     _data = uword_t();     break;
	case LevelObjectType::NT_SDWORD_T:    _data = sdword_t();    break;
	case LevelObjectType::NT_UDWORD_T:    _data = udword_t();    break;
	}
}
LevelObject::LevelObject(type_t const type, any_t const & data) : LevelObject_INIT_LIST(type, data) {}
LevelObject::LevelObject(type_t const type, SourceScannerDHLX & sc) : LevelObject_INIT_LIST(type, false)
{
	switch (_type.getNativeType())
	{
	case LevelObjectType::NT_NONE:        _data = objmap_t();             break;

	case LevelObjectType::NT_BOOL_T:      _data = parse<bool_t>(sc);      break;

	case LevelObjectType::NT_INT_S_T:     _data = parse<int_s_t>(sc);     break;
	case LevelObjectType::NT_INT_T:       _data = parse<int_t>(sc);       break;
	case LevelObjectType::NT_INT_L_T:     _data = parse<int_l_t>(sc);     break;

	case LevelObjectType::NT_REAL_S_T:    _data = parse<real_s_t>(sc);    break;
	case LevelObjectType::NT_REAL_T:      _data = parse<real_t>(sc);      break;
	case LevelObjectType::NT_REAL_L_T:    _data = parse<real_l_t>(sc);    break;

	case LevelObjectType::NT_STRING_T:    _data = parse<string_t>(sc);    break;
	case LevelObjectType::NT_STRING8_T:   _data = parse<string8_t>(sc);   break;
	case LevelObjectType::NT_STRING16_T:  _data = parse<string16_t>(sc);  break;
	case LevelObjectType::NT_STRING32_T:  _data = parse<string32_t>(sc);  break;
	case LevelObjectType::NT_STRING80_T:  _data = parse<string80_t>(sc);  break;
	case LevelObjectType::NT_STRING320_T: _data = parse<string320_t>(sc); break;

	case LevelObjectType::NT_UBYTE_T:     _data = parse<ubyte_t>(sc);     break;
	case LevelObjectType::NT_SWORD_T:     _data = parse<sword_t>(sc);     break;
	case LevelObjectType::NT_UWORD_T:     _data = parse<uword_t>(sc);     break;
	case LevelObjectType::NT_SDWORD_T:    _data = parse<sdword_t>(sc);    break;
	case LevelObjectType::NT_UDWORD_T:    _data = parse<udword_t>(sc);    break;
	}
}
LevelObject::LevelObject(type_t const type, std::string const & value) : LevelObject_INIT_LIST(type, false)
{
	switch (_type.getNativeType())
	{
	case LevelObjectType::NT_NONE:        _data = objmap_t();                break;

	case LevelObjectType::NT_BOOL_T:      _data = parse<bool_t>(value);      break;

	case LevelObjectType::NT_INT_S_T:     _data = parse<int_s_t>(value);     break;
	case LevelObjectType::NT_INT_T:       _data = parse<int_t>(value);       break;
	case LevelObjectType::NT_INT_L_T:     _data = parse<int_l_t>(value);     break;

	case LevelObjectType::NT_REAL_S_T:    _data = parse<real_s_t>(value);    break;
	case LevelObjectType::NT_REAL_T:      _data = parse<real_t>(value);      break;
	case LevelObjectType::NT_REAL_L_T:    _data = parse<real_l_t>(value);    break;

	case LevelObjectType::NT_STRING_T:    _data = parse<string_t>(value);    break;
	case LevelObjectType::NT_STRING8_T:   _data = parse<string8_t>(value);   break;
	case LevelObjectType::NT_STRING16_T:  _data = parse<string16_t>(value);  break;
	case LevelObjectType::NT_STRING32_T:  _data = parse<string32_t>(value);  break;
	case LevelObjectType::NT_STRING80_T:  _data = parse<string80_t>(value);  break;
	case LevelObjectType::NT_STRING320_T: _data = parse<string320_t>(value); break;

	case LevelObjectType::NT_UBYTE_T:     _data = parse<ubyte_t>(value);     break;
	case LevelObjectType::NT_SWORD_T:     _data = parse<sword_t>(value);     break;
	case LevelObjectType::NT_UWORD_T:     _data = parse<uword_t>(value);     break;
	case LevelObjectType::NT_SDWORD_T:    _data = parse<sdword_t>(value);    break;
	case LevelObjectType::NT_UDWORD_T:    _data = parse<udword_t>(value);    break;
	}
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

	switch (_type.getNativeType())
	{
	case LevelObjectType::NT_NONE: if (_data.getType() != any_t::OBJMAP_T) _data = convert<obj_t, any_t>(_data)->_data.getObjMap(); break;

	case LevelObjectType::NT_BOOL_T:      _data = parse<bool_t>(value);      break;

	case LevelObjectType::NT_INT_S_T:     _data = parse<int_s_t>(value);     break;
	case LevelObjectType::NT_INT_T:       _data = parse<int_t>(value);       break;
	case LevelObjectType::NT_INT_L_T:     _data = parse<int_l_t>(value);     break;

	case LevelObjectType::NT_REAL_S_T:    _data = parse<real_s_t>(value);    break;
	case LevelObjectType::NT_REAL_T:      _data = parse<real_t>(value);      break;
	case LevelObjectType::NT_REAL_L_T:    _data = parse<real_l_t>(value);    break;

	case LevelObjectType::NT_STRING_T:    _data = parse<string_t>(value);    break;
	case LevelObjectType::NT_STRING8_T:   _data = parse<string8_t>(value);   break;
	case LevelObjectType::NT_STRING16_T:  _data = parse<string16_t>(value);  break;
	case LevelObjectType::NT_STRING32_T:  _data = parse<string32_t>(value);  break;
	case LevelObjectType::NT_STRING80_T:  _data = parse<string80_t>(value);  break;
	case LevelObjectType::NT_STRING320_T: _data = parse<string320_t>(value); break;

	case LevelObjectType::NT_UBYTE_T:     _data = parse<ubyte_t>(value);     break;
	case LevelObjectType::NT_SWORD_T:     _data = parse<sword_t>(value);     break;
	case LevelObjectType::NT_UWORD_T:     _data = parse<uword_t>(value);     break;
	case LevelObjectType::NT_SDWORD_T:    _data = parse<sdword_t>(value);    break;
	case LevelObjectType::NT_UDWORD_T:    _data = parse<udword_t>(value);    break;
	}

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



void LevelObject::skipData(SourceScannerDHLX & sc)
{
	SourceTokenDHLX st(sc.get());

	// No opening brace means the block is a single statement.
	if (st.getType() != SourceTokenDHLX::TT_OP_BRACE_O)
	{
		while (true)
		{
			if (st.getType() == SourceTokenDHLX::TT_OP_SEMICOLON) return;

			if (st.getType() == SourceTokenDHLX::TT_EOF)
				throw CompilerException("Unexpected EOF");

			st = sc.get();
		}
	}

	sc.unget(st);

	sc.getblock(SourceTokenDHLX::TT_OP_BRACE_O, SourceTokenDHLX::TT_OP_BRACE_C);
}



std::ostream & operator << (std::ostream & out, LevelObject const & in)
{
	out << '[' << in.getType() << ']';

	if (in._data.getType() != any_t::OBJMAP_T)
		out << ' ' << make_string(in._data);

	return out;
}



