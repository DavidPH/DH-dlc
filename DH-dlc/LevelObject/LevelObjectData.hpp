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
	2010/02/03 - Original version.
	2010/02/16 - Store short floats by pointer to be more consistent with
		the other non-sizeof-pointer types. Plus, it'll be a good
		excuse to possibly change to long double at some point.
	2010/02/16 - And NOW I realize that all the math operations between
		LevelObjects should be handled HERE. The entire handling of
		this class hasn't really changed since dividing it from
		LevelObject in the first place. One important change: get
		functions now check type. If it's wrong, throw an exception NOT
		derived from CompilerException.
*/

#ifndef LEVELOBJECTDATA_H
#define LEVELOBJECTDATA_H

#include "../types.hpp"

#include <ostream>



class LevelObjectData
{
	public:

	enum Type
	{
		NULL_T = 0,

		BOOL_T,

		INT_S_T,
		INT_T,
		INT_L_T,

		OBJ_T,
		OBJMAP_T,

		REAL_S_T,
		REAL_T,
		REAL_L_T,

		STRING_T,
		STRING8_T,
		STRING16_T,
		STRING32_T,
		STRING80_T,
		STRING320_T,

		UBYTE_T,
		SWORD_T,
		UWORD_T,
		SDWORD_T,
		UDWORD_T,
	};

	LevelObjectData(LevelObjectData const &);

	LevelObjectData(bool_t const &);

	LevelObjectData(int_s_t const &);
	LevelObjectData(int_t   const &);
	LevelObjectData(int_l_t const &);

	LevelObjectData(obj_t    const &);
	LevelObjectData(objmap_t const &);

	LevelObjectData(real_s_t const &);
	LevelObjectData(real_t   const &);
	LevelObjectData(real_l_t const &);

	LevelObjectData(string_t    const &);
	LevelObjectData(string8_t   const &);
	LevelObjectData(string16_t  const &);
	LevelObjectData(string32_t  const &);
	LevelObjectData(string80_t  const &);
	LevelObjectData(string320_t const &);

	LevelObjectData(ubyte_t  const &);
	LevelObjectData(sword_t  const &);
	LevelObjectData(uword_t  const &);
	LevelObjectData(sdword_t const &);
	LevelObjectData(udword_t const &);

	~LevelObjectData() {clear();}



	void encodeText(std::ostream & out);



	bool_t       & getBool();
	bool_t const & getBool() const;

	int_s_t       & getIntShort();
	int_s_t const & getIntShort() const;
	int_t         & getInt();
	int_t   const & getInt() const;
	int_l_t       & getIntLong();
	int_l_t const & getIntLong() const;

	obj_t          & getObj();
	obj_t    const & getObj() const;
	objmap_t       & getObjMap();
	objmap_t const & getObjMap() const;

	real_s_t       & getRealShort();
	real_s_t const & getRealShort() const;
	real_t         & getReal();
	real_t   const & getReal() const;
	real_l_t       & getRealLong();
	real_l_t const & getRealLong() const;

	string_t          & getString();
	string_t    const & getString() const;
	string8_t         & getString8();
	string8_t   const & getString8() const;
	string16_t        & getString16();
	string16_t  const & getString16() const;
	string32_t        & getString32();
	string32_t  const & getString32() const;
	string80_t        & getString80();
	string80_t  const & getString80() const;
	string320_t       & getString320();
	string320_t const & getString320() const;

	ubyte_t        & getUByte();
	ubyte_t  const & getUByte() const;
	sword_t        & getSWord();
	sword_t  const & getSWord() const;
	uword_t        & getUWord();
	uword_t  const & getUWord() const;
	sdword_t       & getSDWord();
	sdword_t const & getSDWord() const;
	udword_t       & getUDWord();
	udword_t const & getUDWord() const;



	Type getType() const {return _val;}



	bool_t toBool() const;

	int_s_t toIntShort() const;
	int_t   toInt()      const;
	int_l_t toIntLong()  const;

	real_s_t toRealShort() const;
	real_t   toReal()      const;
	real_l_t toRealLong()  const;

	string_t    toString()    const;
	string8_t   toString8()   const;
	string16_t  toString16()  const;
	string32_t  toString32()  const;
	string80_t  toString80()  const;
	string320_t toString320() const;

	ubyte_t  toUByte()  const;
	sword_t  toSWord()  const;
	uword_t  toUWord()  const;
	sdword_t toSDWord() const;
	udword_t toUDWord() const;



	bool operator == (LevelObjectData const & other) const {return cmp(*this, other) == 0;}
	bool operator != (LevelObjectData const & other) const {return cmp(*this, other) != 0;}
	bool operator >= (LevelObjectData const & other) const {return cmp(*this, other) >= 0;}
	bool operator >  (LevelObjectData const & other) const {return cmp(*this, other) >  0;}
	bool operator <= (LevelObjectData const & other) const {return cmp(*this, other) <= 0;}
	bool operator <  (LevelObjectData const & other) const {return cmp(*this, other) <  0;}

	LevelObjectData & operator += (LevelObjectData const &);

	LevelObjectData & operator = (LevelObjectData const &);



	friend int cmp(LevelObjectData const &, LevelObjectData const &);

	friend std::ostream & operator << (std::ostream &, LevelObjectData const &);



	private:

	void clear();

	Type _val;

	union
	{
		bool_t     _valBool;

		int_s_t    _valIntShort;
		int_t    * _valInt;
		int_l_t  * _valIntLong;

		obj_t    * _valObj;
		objmap_t * _valObjMap;

		real_s_t * _valRealShort;
		real_t   * _valReal;
		real_l_t * _valRealLong;

		string_t    * _valString;
		string8_t   * _valString8;
		string16_t  * _valString16;
		string32_t  * _valString32;
		string80_t  * _valString80;
		string320_t * _valString320;

		ubyte_t  * _valUByte;
		sword_t  * _valSWord;
		uword_t  * _valUWord;
		sdword_t * _valSDWord;
		udword_t * _valUDWord;
	};
};



std::ostream & operator << (std::ostream &, LevelObjectData const &);



#endif /* LEVELOBJECTDATA_H */



