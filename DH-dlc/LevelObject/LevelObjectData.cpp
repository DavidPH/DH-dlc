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
	03/02/2010 - Original version.
	16/02/2010 - Update for short floats being pointer.
	16/02/2010 - Major overhaul described in header.
*/

#include "LevelObjectData.hpp"

#include "LevelObjectMap.hpp"
#include "LevelObjectPointer.hpp"

#include "../types.hpp"
#include "../types/binary.hpp"
#include "../types/int_t.hpp"
#include "../types/real_t.hpp"
#include "../types/string_t.hpp"

#include <stdexcept>



LevelObjectData::LevelObjectData(LevelObjectData const & other)
{
	_val = other._val;

	switch (_val)
	{
		case NULL_T:
			break;

		case BOOL_T:
			_valBool = other._valBool;
			break;

		case INT_S_T:
			_valIntShort = other._valIntShort;
			break;

		case INT_T:
			_valInt = new int_t(*other._valInt);
			break;

		case INT_L_T:
			_valIntLong = new int_l_t(*other._valIntLong);
			break;

		case OBJ_T:
			_valObj = new obj_t(*other._valObj);
			break;

		case OBJMAP_T:
			_valObjMap = new objmap_t(*other._valObjMap);
			break;

		case REAL_S_T:
			_valRealShort = new real_s_t(*other._valRealShort);
			break;

		case REAL_T:
			_valReal = new real_t(*other._valReal);
			break;

		case REAL_L_T:
			_valRealLong = new real_l_t(*other._valRealLong);
			break;

		case STRING_T:
			_valString = new string_t(*other._valString);
			break;

		case STRING8_T:
			_valString8 = new string8_t(*other._valString8);
			break;

		case STRING16_T:
			_valString16 = new string16_t(*other._valString16);
			break;

		case STRING32_T:
			_valString32 = new string32_t(*other._valString32);
			break;

		case STRING80_T:
			_valString80 = new string80_t(*other._valString80);
			break;

		case STRING320_T:
			_valString320 = new string320_t(*other._valString320);
			break;

		case UBYTE_T:
			_valUByte = new ubyte_t(*other._valUByte);
			break;

		case SWORD_T:
			_valSWord = new sword_t(*other._valSWord);
			break;

		case UWORD_T:
			_valUWord = new uword_t(*other._valUWord);
			break;

		case SDWORD_T:
			_valSDWord = new sdword_t(*other._valSDWord);
			break;

		case UDWORD_T:
			_valUDWord = new udword_t(*other._valUDWord);
			break;
	}
}

LevelObjectData::LevelObjectData(bool_t const & v) : _val(BOOL_T), _valBool(v) {}

LevelObjectData::LevelObjectData(int_s_t const & v) : _val(INT_S_T), _valIntShort(            v ) {}
LevelObjectData::LevelObjectData(int_t   const & v) : _val(INT_T),   _valInt(     new int_t  (v)) {}
LevelObjectData::LevelObjectData(int_l_t const & v) : _val(INT_L_T), _valIntLong( new int_l_t(v)) {}

LevelObjectData::LevelObjectData(obj_t    const & v) : _val(OBJ_T),    _valObj(   new obj_t   (v)) {}
LevelObjectData::LevelObjectData(objmap_t const & v) : _val(OBJMAP_T), _valObjMap(new objmap_t(v)) {}

LevelObjectData::LevelObjectData(real_s_t const & v) : _val(REAL_S_T), _valRealShort(new real_s_t(v)) {}
LevelObjectData::LevelObjectData(real_t   const & v) : _val(REAL_T),   _valReal(     new real_t  (v)) {}
LevelObjectData::LevelObjectData(real_l_t const & v) : _val(REAL_L_T), _valRealLong( new real_l_t(v)) {}

LevelObjectData::LevelObjectData(string_t    const & v) : _val(STRING_T),    _valString(   new string_t   (v)) {}
LevelObjectData::LevelObjectData(string8_t   const & v) : _val(STRING8_T),   _valString8(  new string8_t  (v)) {}
LevelObjectData::LevelObjectData(string16_t  const & v) : _val(STRING16_T),  _valString16( new string16_t (v)) {}
LevelObjectData::LevelObjectData(string32_t  const & v) : _val(STRING32_T),  _valString32( new string32_t (v)) {}
LevelObjectData::LevelObjectData(string80_t  const & v) : _val(STRING80_T),  _valString80( new string80_t (v)) {}
LevelObjectData::LevelObjectData(string320_t const & v) : _val(STRING320_T), _valString320(new string320_t(v)) {}

LevelObjectData::LevelObjectData(ubyte_t  const & v) : _val(UBYTE_T),  _valUByte( new ubyte_t (v)) {}
LevelObjectData::LevelObjectData(sword_t  const & v) : _val(SWORD_T),  _valSWord( new sword_t (v)) {}
LevelObjectData::LevelObjectData(uword_t  const & v) : _val(UWORD_T),  _valUWord( new uword_t (v)) {}
LevelObjectData::LevelObjectData(sdword_t const & v) : _val(SDWORD_T), _valSDWord(new sdword_t(v)) {}
LevelObjectData::LevelObjectData(udword_t const & v) : _val(UDWORD_T), _valUDWord(new udword_t(v)) {}



void LevelObjectData::clear()
{
	switch (_val)
	{
		case NULL_T:
			break;

		case BOOL_T:
			break;

		case INT_S_T:
			break;

		case INT_T:
			delete _valInt;
			break;

		case INT_L_T:
			delete _valIntLong;
			break;

		case OBJ_T:
			delete _valObj;
			break;

		case OBJMAP_T:
			delete _valObjMap;
			break;

		case REAL_S_T:
			delete _valRealShort;
			break;

		case REAL_T:
			delete _valReal;
			break;

		case REAL_L_T:
			delete _valRealLong;
			break;

		case STRING_T:
			delete _valString;
			break;

		case STRING8_T:
			delete _valString8;
			break;

		case STRING16_T:
			delete _valString16;
			break;

		case STRING32_T:
			delete _valString32;
			break;

		case STRING80_T:
			delete _valString80;
			break;

		case STRING320_T:
			delete _valString320;
			break;

		case UBYTE_T:
			delete _valUByte;
			break;

		case SWORD_T:
			delete _valSWord;
			break;

		case UWORD_T:
			delete _valUWord;
			break;

		case SDWORD_T:
			delete _valSDWord;
			break;

		case UDWORD_T:
			delete _valUDWord;
			break;
	}

	_val = NULL_T;
}



void LevelObjectData::encodeText(std::ostream & out)
{
	switch (_val)
	{
	case NULL_T:
		break;

	case BOOL_T:
		if (_valBool)
			out << "true";
		else
			out << "false";
		break;

	case INT_S_T:
		out << _valIntShort;
		break;

	case INT_T:
		_valInt->encodeText(out);
		break;

	case INT_L_T:
		_valIntLong->encodeText(out);
		break;

	case OBJ_T:
		break;

	case OBJMAP_T:
		break;

	case REAL_S_T:
		out.precision(256);
		out << *_valRealShort;
		break;

	case REAL_T:
		_valReal->encodeText(out);
		break;

	case REAL_L_T:
		_valRealLong->encodeText(out);
		break;

	case STRING_T:
		_valString->encodeText(out);
		break;

	case STRING8_T:
		_valString8->encodeText(out);
		break;

	case STRING16_T:
		_valString16->encodeText(out);
		break;

	case STRING32_T:
		_valString32->encodeText(out);
		break;

	case STRING80_T:
		_valString80->encodeText(out);
		break;

	case STRING320_T:
		_valString320->encodeText(out);
		break;

	case UBYTE_T:
		_valUByte->encodeText(out);
		break;

	case SWORD_T:
		_valSWord->encodeText(out);
		break;

	case UWORD_T:
		_valUWord->encodeText(out);
		break;

	case SDWORD_T:
		_valSDWord->encodeText(out);
		break;

	case UDWORD_T:
		_valUDWord->encodeText(out);
		break;
	}
}



#define LevelObjectData_get(TYPE_NAME, ENUM_NAME, FUNC_NAME, RETURN)		\
TYPE_NAME & LevelObjectData::FUNC_NAME()					\
{										\
	if (_val != ENUM_NAME)							\
		throw std::logic_error("non_" #TYPE_NAME "->" #FUNC_NAME "()");	\
										\
	return RETURN;								\
}										\
TYPE_NAME const & LevelObjectData::FUNC_NAME() const				\
{										\
	if (_val != ENUM_NAME)							\
		throw std::logic_error("non_" #TYPE_NAME "->" #FUNC_NAME "()");	\
										\
	return RETURN;								\
}

LevelObjectData_get(bool_t, BOOL_T, getBool, _valBool)

LevelObjectData_get(int_s_t, INT_S_T, getIntShort,  _valIntShort)
LevelObjectData_get(int_t,   INT_T,   getInt,      *_valInt)
LevelObjectData_get(int_l_t, INT_L_T, getIntLong,  *_valIntLong)

LevelObjectData_get(obj_t,    OBJ_T,    getObj,    *_valObj)
LevelObjectData_get(objmap_t, OBJMAP_T, getObjMap, *_valObjMap)

LevelObjectData_get(real_s_t, REAL_S_T, getRealShort, *_valRealShort)
LevelObjectData_get(real_t,   REAL_T,   getReal,      *_valReal)
LevelObjectData_get(real_l_t, REAL_L_T, getRealLong,  *_valRealLong)

LevelObjectData_get(string_t,    STRING_T,    getString,    *_valString)
LevelObjectData_get(string8_t,   STRING8_T,   getString8,   *_valString8)
LevelObjectData_get(string16_t,  STRING16_T,  getString16,  *_valString16)
LevelObjectData_get(string32_t,  STRING32_T,  getString32,  *_valString32)
LevelObjectData_get(string80_t,  STRING80_T,  getString80,  *_valString80)
LevelObjectData_get(string320_t, STRING320_T, getString320, *_valString320)

LevelObjectData_get(ubyte_t,  UBYTE_T,  getUByte,  *_valUByte)
LevelObjectData_get(sword_t,  SWORD_T,  getSWord,  *_valSWord)
LevelObjectData_get(uword_t,  UWORD_T,  getUWord,  *_valUWord)
LevelObjectData_get(sdword_t, SDWORD_T, getSDWord, *_valSDWord)
LevelObjectData_get(udword_t, UDWORD_T, getUDWord, *_valUDWord)

#undef LevelObjectData_get



#define LevelObjectData_to(TYPE, FUNC_NAME) \
TYPE##_t LevelObjectData::FUNC_NAME() const \
{ \
	switch (_val) \
	{ \
	case NULL_T: throw std::invalid_argument("LevelObjectData(NULL_T)->" #FUNC_NAME "()"); \
		\
	case BOOL_T: return to_##TYPE( _valBool); \
		\
	case INT_S_T: return to_##TYPE( _valIntShort); \
	case INT_T:   return to_##TYPE(*_valInt); \
	case INT_L_T: return to_##TYPE(*_valIntLong); \
		\
	case OBJ_T:    return to_##TYPE(*_valObj); \
	case OBJMAP_T: throw std::invalid_argument("LevelObjectData(OBJMAP_T)->" #FUNC_NAME "()"); \
		\
	case REAL_S_T: return to_##TYPE(*_valRealShort); \
	case REAL_T:   return to_##TYPE(*_valReal); \
	case REAL_L_T: return to_##TYPE(*_valRealLong); \
		\
	case STRING_T:    return to_##TYPE(*_valString); \
	case STRING8_T:   return to_##TYPE(*_valString8); \
	case STRING16_T:  return to_##TYPE(*_valString16); \
	case STRING32_T:  return to_##TYPE(*_valString32); \
	case STRING80_T:  return to_##TYPE(*_valString80); \
	case STRING320_T: return to_##TYPE(*_valString320); \
		\
	case UBYTE_T:  return to_##TYPE(*_valUByte); \
	case SWORD_T:  return to_##TYPE(*_valSWord); \
	case UWORD_T:  return to_##TYPE(*_valUWord); \
	case SDWORD_T: return to_##TYPE(*_valSDWord); \
	case UDWORD_T: return to_##TYPE(*_valUDWord); \
	} \
	\
	return TYPE##_t(); \
}

LevelObjectData_to(bool, toBool)

LevelObjectData_to(int_s, toIntShort)
LevelObjectData_to(int,   toInt)
LevelObjectData_to(int_l, toIntLong)

LevelObjectData_to(real_s, toRealShort)
LevelObjectData_to(real,   toReal)
LevelObjectData_to(real_l, toRealLong)

LevelObjectData_to(string,    toString)
LevelObjectData_to(string8,   toString8)
LevelObjectData_to(string16,  toString16)
LevelObjectData_to(string32,  toString32)
LevelObjectData_to(string80,  toString80)
LevelObjectData_to(string320, toString320)

LevelObjectData_to(ubyte,  toUByte)
LevelObjectData_to(sword,  toSWord)
LevelObjectData_to(uword,  toUWord)
LevelObjectData_to(sdword, toSDWord)
LevelObjectData_to(udword, toUDWord)

#undef LevelObjectData_to



LevelObjectData & LevelObjectData::operator += (LevelObjectData const & other)
{
	switch (_val)
	{
		case NULL_T: throw std::invalid_argument("LevelObjectData(NULL_T)+=...");

		case BOOL_T: _valBool += other.toBool(); break;

		case INT_S_T:  _valIntShort += other.toIntShort(); break;
		case INT_T:   *_valInt      += other.toInt();      break;
		case INT_L_T: *_valIntLong  += other.toIntLong();  break;

		case OBJ_T:    throw std::invalid_argument("LevelObjectData(OBJ_T)+=...");
		case OBJMAP_T: throw std::invalid_argument("LevelObjectData(OBJMAP_T)+=...");

		case REAL_S_T: *_valRealShort += other.toRealShort(); break;
		case REAL_T:   *_valReal      += other.toReal();      break;
		case REAL_L_T: *_valRealLong  += other.toRealLong();  break;

		case STRING_T:    *_valString    += other.toString();    break;
		case STRING8_T:   *_valString8   += other.toString8();   break;
		case STRING16_T:  *_valString16  += other.toString16();  break;
		case STRING32_T:  *_valString32  += other.toString32();  break;
		case STRING80_T:  *_valString80  += other.toString80();  break;
		case STRING320_T: *_valString320 += other.toString320(); break;

		case UBYTE_T:  *_valUByte  += other.toUByte();  break;
		case SWORD_T:  *_valSWord  += other.toSWord();  break;
		case UWORD_T:  *_valUWord  += other.toUWord();  break;
		case SDWORD_T: *_valSDWord += other.toSDWord(); break;
		case UDWORD_T: *_valUDWord += other.toUDWord(); break;
	}

	return *this;
}

LevelObjectData & LevelObjectData::operator = (LevelObjectData const & other)
{
	clear();

	_val = other._val;

	switch (_val)
	{
		case NULL_T:
			break;

		case BOOL_T:
			_valBool = other._valBool;
			break;

		case INT_S_T:
			_valIntShort = other._valIntShort;
			break;

		case INT_T:
			_valInt = new int_t(*other._valInt);
			break;

		case INT_L_T:
			_valIntLong = new int_l_t(*other._valIntLong);
			break;

		case OBJ_T:
			_valObj = new obj_t(*other._valObj);
			break;

		case OBJMAP_T:
			_valObjMap = new objmap_t(*other._valObjMap);
			break;

		case REAL_S_T:
			_valRealShort = new real_s_t(*other._valRealShort);
			break;

		case REAL_T:
			_valReal = new real_t(*other._valReal);
			break;

		case REAL_L_T:
			_valRealLong = new real_l_t(*other._valRealLong);
			break;

		case STRING_T:
			_valString = new string_t(*other._valString);
			break;

		case STRING8_T:
			_valString8 = new string8_t(*other._valString8);
			break;

		case STRING16_T:
			_valString16 = new string16_t(*other._valString16);
			break;

		case STRING32_T:
			_valString32 = new string32_t(*other._valString32);
			break;

		case STRING80_T:
			_valString80 = new string80_t(*other._valString80);
			break;

		case STRING320_T:
			_valString320 = new string320_t(*other._valString320);
			break;

		case UBYTE_T:
			_valUByte = new ubyte_t(*other._valUByte);
			break;

		case SWORD_T:
			_valSWord = new sword_t(*other._valSWord);
			break;

		case UWORD_T:
			_valUWord = new uword_t(*other._valUWord);
			break;

		case SDWORD_T:
			_valSDWord = new sdword_t(*other._valSDWord);
			break;

		case UDWORD_T:
			_valUDWord = new udword_t(*other._valUDWord);
			break;
	}

	return *this;
}



int cmp(LevelObjectData const & l, LevelObjectData const & r)
{
	switch (l._val)
	{
		// NULL LevelObjectData instances should never be used.
		case LevelObjectData::NULL_T: throw std::invalid_argument("cmp(LevelObjectData(NULL_T),...)");

		case LevelObjectData::BOOL_T: return cmp(l._valBool, r.toBool());

		case LevelObjectData::INT_S_T: return cmp( l._valIntShort, r.toIntShort());
		case LevelObjectData::INT_T:   return cmp(*l._valInt,      r.toInt());
		case LevelObjectData::INT_L_T: return cmp(*l._valIntLong,  r.toIntLong());

		case LevelObjectData::OBJ_T:    throw std::invalid_argument("cmp(LevelObjectData(OBJ_T),...)");
		// Even if there was a meaningful way to handle this, it is still obviously an error.
		case LevelObjectData::OBJMAP_T: throw std::invalid_argument("cmp(LevelObjectData(OBJMAP_T),...)");

		case LevelObjectData::REAL_S_T: return cmp(*l._valRealShort, r.toRealShort());
		case LevelObjectData::REAL_T:   return cmp(*l._valReal,      r.toReal());
		case LevelObjectData::REAL_L_T: return cmp(*l._valRealLong,  r.toRealLong());

		case LevelObjectData::STRING_T:    return cmp(*l._valString,    r.toString());
		case LevelObjectData::STRING8_T:   return cmp(*l._valString8,   r.toString8());
		case LevelObjectData::STRING16_T:  return cmp(*l._valString16,  r.toString16());
		case LevelObjectData::STRING32_T:  return cmp(*l._valString32,  r.toString32());
		case LevelObjectData::STRING80_T:  return cmp(*l._valString80,  r.toString80());
		case LevelObjectData::STRING320_T: return cmp(*l._valString320, r.toString320());

		case LevelObjectData::UBYTE_T:  return cmp(*l._valUByte,  r.toUByte());
		case LevelObjectData::SWORD_T:  return cmp(*l._valSWord,  r.toSWord());
		case LevelObjectData::UWORD_T:  return cmp(*l._valUWord,  r.toUWord());
		case LevelObjectData::SDWORD_T: return cmp(*l._valSDWord, r.toSDWord());
		case LevelObjectData::UDWORD_T: return cmp(*l._valUDWord, r.toUDWord());
	}

	return 0;
}



std::ostream & operator << (std::ostream & out, LevelObjectData const & in)
{
	switch (in._val)
	{
		// NULL LevelObjectData instances should never be used.
		case LevelObjectData::NULL_T: throw std::invalid_argument("operator << (...,LevelObjectData(NULL_T))");

		case LevelObjectData::BOOL_T: return out << make_string(in.getBool());

		case LevelObjectData::INT_S_T: return out << in.getIntShort();
		case LevelObjectData::INT_T:   return out << in.getInt();
		case LevelObjectData::INT_L_T: return out << in.getIntLong();

		case LevelObjectData::OBJ_T:    throw std::invalid_argument("operator << (...,LevelObjectData(OBJ_T))");
		case LevelObjectData::OBJMAP_T: throw std::invalid_argument("operator << (...,LevelObjectData(OBJMAP_T))");

		case LevelObjectData::REAL_S_T: return out << in.getRealShort();
		case LevelObjectData::REAL_T:   return out << in.getReal();
		case LevelObjectData::REAL_L_T: return out << in.getRealLong();

		case LevelObjectData::STRING_T:    return out << in.getString();
		case LevelObjectData::STRING8_T:   return out << in.getString8();
		case LevelObjectData::STRING16_T:  return out << in.getString16();
		case LevelObjectData::STRING32_T:  return out << in.getString32();
		case LevelObjectData::STRING80_T:  return out << in.getString80();
		case LevelObjectData::STRING320_T: return out << in.getString320();

		case LevelObjectData::UBYTE_T:  return out << in.getUByte();
		case LevelObjectData::SWORD_T:  return out << in.getSWord();
		case LevelObjectData::UWORD_T:  return out << in.getUWord();
		case LevelObjectData::SDWORD_T: return out << in.getSDWord();
		case LevelObjectData::UDWORD_T: return out << in.getUDWord();
	}

	return out;
}



