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
	2010/02/03 - Minor cleanup. Changed interface for name_t.
	2010/02/06 - LevelObjectPointer needs to check for NULL in assignment.
	2010/02/06 - LevelObject::create functions no longer inline.
	2010/02/06 - Changed isCompounded from a bool to a bitfield, and added
		isReturned.
	2010/02/06 - Moved LevelObjectPointer to its own file.
	2010/02/15 - Added isBreaked and isContinued bitfield elements.
	2010/02/15 - Added operator < and operator += and cmp functions.
	2010/02/16 - Added addGlobal bitfield element. It is meant to replace
		the costly checking in add_object() as well as the argument to
		LevelObject::addObject().
	2010/02/16 - Remove the addGlobal argument from addObject() since it
		has been replaced by the addGlobal bitfield element. Not that
		it was currently used anywhere. I actually don't remember what
		it was for...
	2010/02/16 - Removed the math functions that operated on LevelObjects.
	2010/05/06 - Added a getData method to make the converters in types.cpp
		much simpler. (Don't know how to have a template friend, if it
		is even possible.)
*/

#ifndef LEVELOBJECT_H
#define LEVELOBJECT_H

#include "LevelObjectData.hpp"
#include "LevelObjectType.hpp"
#include "SourceScanner.hpp"
#include "types.hpp"

#include <ostream>
#include <vector>

class SourceToken;
class SourceTokenDHLX;



class LevelObject
{
	public:
		~LevelObject();

		void addBase(std::string const &);

		void addData(std::string const & data, std::string const & name = "...");
		bool addDataIf(std::string const & data, std::string const & value1, std::string const & value2, std::string const & op, std::string const & type = "", bool checkElse = false);
		bool addDataIf(std::string const & data, std::vector<std::string> const & values, std::string const & op, bool checkElse = false);

		void addObject(name_t const & name, obj_t);
		void addObject(name_t const & name, SourceToken const &);
		void addObject(SourceScannerDHLX &);

		void doCommand(std::string const & command, SourceScannerDHLX &);

		obj_t getObject(name_t const & name);

		bool hasObject(name_t const & name);

		type_t getType() const;
		void setType(type_t const type, std::string const & value = "");

		void encodeDoom     (std::ostream & out);
		void encodeExtraData(std::ostream & out);
		void encodeHeretic  (std::ostream & out);
		void encodeHexen    (std::ostream & out);
		void encodeStrife   (std::ostream & out);
		void encodeUDMF     (std::ostream & out, int depth = 0);

		any_t const & getData() const {return _data;}

		std::ostream& printOn(std::ostream&, int depth = 0) const;

		LevelObject & operator = (LevelObject const & other);

		static obj_t create();
		static obj_t create(type_t const type);
		static obj_t create(type_t const type, any_t const & data);
		static obj_t create(type_t const type, std::string const & value);
		static obj_t create(type_t const type, std::string const & data, std::vector<std::string> const & base);

		friend void add_object(name_t const &, obj_t);
		friend int_s_t get_object_index(obj_t);
		friend bool rem_object(obj_t);

		friend std::ostream & operator << (std::ostream& out, const LevelObject& in);

		friend class LevelObjectPointer;

	private:
		LevelObject();
		LevelObject(LevelObject const &);
		LevelObject(type_t const);
		LevelObject(type_t const, any_t const &);
		LevelObject(type_t const, std::string const &);
		LevelObject(type_t const, std::string const &, std::vector<std::string> const &);

		size_t _index;
		size_t _refCount;

		any_t _data;

		type_t _type;

		unsigned _addGlobal    : 1;
		unsigned _isBreaked    : 1; // #break
		unsigned _isCompounded : 1; // #compound
		unsigned _isContinued  : 1; // #continue
		unsigned _isReturned   : 1; // #return
};



std::ostream & operator << (std::ostream &, LevelObject const &);



#endif /* LEVELOBJECT_H */



