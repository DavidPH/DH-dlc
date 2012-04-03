//-----------------------------------------------------------------------------
//
// Copyright(C) 2009-2012 David Hill
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------------
//
// Level object.
//
//-----------------------------------------------------------------------------

#ifndef HPP_LevelObject_
#define HPP_LevelObject_

#include "LevelObject/LevelObjectData.hpp"
#include "LevelObject/LevelObjectType.hpp"

#include "Counter.hpp"
#include "SourceScanner.hpp"
#include "types.hpp"

#include <ostream>
#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

class SourceTokenDDL;
class SourceTokenDHLX;

//
// LevelObject
//
class LevelObject : public Counter
{
   MAKE_NOCLONE_COUNTER_CLASS_BASE(LevelObject, Counter);

public:
	void addBase(name_t const & base);

	void addData(SourceScannerDHLX & sc);
	void addData(std::string const & data, std::string const & name = "...");
	bool addDataIf(SourceScannerDHLX & sc);
	bool addDataIf(std::string const & data, std::string const & value1, std::string const & value2, std::string const & op, std::string const & type = "", bool checkElse = false);
	bool addDataIf(std::string const & data, std::vector<std::string> const & values, std::string const & op, bool checkElse = false);

	void addObject(name_t const & name, obj_t);
	void addObject(name_t const & name, SourceTokenDDL const &);
	void addObject(SourceScannerDHLX &);

	void doCommand(std::string const & command, SourceScannerDHLX &);
	void doCommand(std::string const & command, SourceTokenDDL const &);

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
	void encodeUSDF     (std::ostream & out, int depth = 0);

	any_t const & getData() const {return _data;}

	std::ostream& printOn(std::ostream&, int depth = 0);

	LevelObject & operator = (LevelObject const & other);

	void setCompounded() {_isCompounded = true;};

	void skipData(SourceScannerDHLX & sc);

	static obj_t create();
	static obj_t create(type_t const type);
	static obj_t create(type_t const type, any_t const & data);
	static obj_t create(type_t const type, SourceScannerDHLX & sc);
	static obj_t create(type_t const type, std::string const & value);
	static obj_t create(type_t const type, std::string const & data, std::vector<std::string> const & base);

	friend void add_object(name_t const &, obj_t);
	friend int_s_t get_object_index(obj_t);
	friend bool rem_object(obj_t);

	friend std::ostream & operator << (std::ostream& out, const LevelObject& in);

private:
	LevelObject();
	LevelObject(LevelObject const &);
	LevelObject(type_t const);
	LevelObject(type_t const, any_t const &);
	LevelObject(type_t const, SourceScannerDHLX &);
	LevelObject(type_t const, std::string const &);
	LevelObject(type_t const, std::string const &, std::vector<std::string> const &);
	~LevelObject();

	void doCommandInfo(SourceScannerDHLX & sc);
	void doCommandInfo(SourceTokenDDL const & st);
	void doCommandInfo(string_t const & data);

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

#endif//HPP_LevelObject_

