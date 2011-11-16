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
	2010/02/04 - Added #deletevolatile command.
	2010/02/06 - Should check for multiple name parts after check for
		command, so that the right error is generated.
	2010/02/06 - Added #return command.
	2010/02/14 - Do NOT change last_if_result for else. Doing so breaks
		if; else if; else if; else horribly.
	2010/02/15 - Add isBreaked and isContinued checks to addData(str_t) and
		to #while. Also add #break and #continue.
	2010/02/15 - Added #for and #do command.
	2010/02/16 - Remove addGlobal argument from addObject(name_t, obj_t,
		bool).
	2010/02/28 - Update for lo_type enum.
	2010/06/24 - Update for explicit name_t constructors.
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
#include "../SourceToken.hpp"
#include "../types.hpp"

#include "../exceptions/CompilerException.hpp"
#include "../exceptions/InvalidTypeException.hpp"
#include "../exceptions/NoSuchElementException.hpp"
#include "../exceptions/SyntaxException.hpp"
#include "../exceptions/UnknownCommandException.hpp"

#include "../parsing/parsing.hpp"

#include "../types/int_t.hpp"
#include "../types/real_t.hpp"
#include "../types/string_t.hpp"

#include "../../common/foreach.hpp"

#include <cstdlib>
#include <iostream>
#include <sstream>



void LevelObject::addBase(name_t const & base)
{
	if (_data.get_dataType() != any_t::OBJMAP_T)
		throw InvalidTypeException("non-objects have no keys");

	obj_t other = get_object(base);

	if (other->_data.get_dataType() != any_t::OBJMAP_T)
		throw InvalidTypeException("non-objects have no keys");

	_data.getObjMap() += other->_data.getObjMap();
}

void LevelObject::addData(SourceScannerDHLX & sc)
{
	LevelObjectStack los(this);

	SourceTokenDHLX st(sc.get());

	// No opening brace means the block is a single statement.
	if (st.getType() != SourceTokenDHLX::TT_OP_BRACE_O)
	{
		sc.unget(st);

		addObject(sc);

		return;
	}

	while (true)
	{
		st = sc.get();

		if (st.getType() == SourceTokenDHLX::TT_OP_BRACE_C)
			break;

		sc.unget(st);

		addObject(sc);
	}
}
void LevelObject::addData(std::string const & data, std::string const & name)
{
	LevelObjectStack los(this);

	std::istringstream iss(data);
	SourceStream       ss(iss);
	SourceTokenDDL     st;

	while (ss)
	{
		if (_isBreaked || _isContinued || _isReturned)
			return;

		try
		{
			st.clear();
			ss >> st;

			if (!st.getName().empty() && st.getName()[0] == '#')
				doCommand(st.getName().substr(1), st);
			else
				addObject(parse_name(st.getName()), st);
		}
		catch (CompilerException& e)
		{
			PRINT_ERROR(name << ':' << ss.getLineCount() << ':' << e << "\n  ->" << st << '\n');

			throw;
		}
	}
}

bool LevelObject::addDataIf(SourceScannerDHLX & sc)
{
	if (parse<bool_t>(sc))
	{
		addData(sc);
		last_if_result = true;
		return true;
	}
	else
	{
		skipData(sc);
		last_if_result = false;
		return false;
	}
}
// Adds data if (value1 opString value2) == true
// If checkElse is true, the last if statement must have been false, or data
// will not be added.
// If type is non-empty, evaluate values as that type before comparing,
// otherwise take as names.
bool LevelObject::addDataIf(std::string const & data, std::string const & value1, std::string const & value2, std::string const & opString, std::string const & type, bool checkElse)
{
	if (checkElse && last_if_result)
		return false;

	// # else
	// See above for checking of 'else'.
	if (value1.empty() && value2.empty() && opString.empty())
	{
		last_if_result = true;
		addData(data);
		last_if_result = true;
		return true;
	}

	int cmpResult;

	if (type.empty())
		cmpResult = cmp( get_object(parse_name(value1))->_data, get_object(parse_name(value2))->_data );

	else if (type == type_name_bool())
		cmpResult = cmp( parse<bool_t>(value1), parse<bool_t>(value2) );

	else if (type == type_name_shortint())
		cmpResult = cmp( parse<int_s_t>(value1), parse<int_s_t>(value2) );
	else if (type == type_name_int())
		cmpResult = cmp( parse<int_t>(value1), parse<int_t>(value2) );
	else if (type == type_name_longint())
		cmpResult = cmp( parse<int_l_t>(value1), parse<int_l_t>(value2) );

	else if (type == type_name_shortfloat())
		cmpResult = cmp( parse<real_s_t>(value1), parse<real_s_t>(value2) );
	else if (type == type_name_float())
		cmpResult = cmp( parse<real_t>(value1), parse<real_t>(value2) );
	else if (type == type_name_longfloat())
		cmpResult = cmp( parse<real_l_t>(value1), parse<real_l_t>(value2) );

	else if (type == type_name_string())
		cmpResult = cmp( parse<string_t>(value1), parse<string_t>(value2) );

	else
		throw UnknownCommandException("#cmp type:" + type);

	#define CHECK_CMP(CMP_STRING, OP)	\
	if (opString == CMP_STRING)		\
	{					\
		if (cmpResult OP 0)		\
		{				\
			last_if_result = true;	\
			this->addData(data);	\
			last_if_result = true;	\
			return true;		\
		}				\
		else				\
		{				\
			last_if_result = false;	\
			return false;		\
		}				\
	}

	     CHECK_CMP(cmp_name_eq(), ==)
	else CHECK_CMP(cmp_name_ne(), !=)
	else CHECK_CMP(cmp_name_lt(), <)
	else CHECK_CMP(cmp_name_le(), <=)
	else CHECK_CMP(cmp_name_gt(), >)
	else CHECK_CMP(cmp_name_ge(), >=)
	else
		throw UnknownCommandException("#if cmp op:" + opString);

	#undef CHECK_CMP
}
bool LevelObject::addDataIf(std::string const & data, std::vector<std::string> const & value, std::string const & opString, bool checkElse)
{
	if (checkElse && last_if_result)
		return false;

	// # else
	// See above for checking of 'else'.
	if (value.empty() && opString.empty())
	{
		last_if_result = true;
		addData(data);
		last_if_result = true;
		return true;
	}

	bool op_not   = false;
	bool op_local = false;

	std::string op_true(opString);

	if (op_true.substr(0, 3) == iftype_name_not())
	{
		op_not  = true;
		op_true = op_true.substr(3);
	}

	if (op_true.size() >= 5 && op_true.substr(op_true.size()-5) == iftype_name_local())
	{
		op_local = true;
		op_true  = op_true.substr(0, op_true.size()-5);
	}
	else if (op_true.size() >= 6 && op_true.substr(op_true.size()-6) == iftype_name_global())
	{
		op_local = false;
		op_true  = op_true.substr(0, op_true.size()-6);
	}

	enum
	{
		OP_NONE,
		OP_BOOL,
		OP_EXISTS,
		OP_FALSE,
		OP_TRUE
	} op_type;

	     if (op_true.empty())                 op_type = OP_NONE;
	else if (op_true == iftype_name_bool())   op_type = OP_BOOL;
	else if (op_true == iftype_name_exists()) op_type = OP_EXISTS;
	else if (op_true == iftype_name_false())  op_type = OP_FALSE;
	else if (op_true == iftype_name_true())   op_type = OP_TRUE;
	else
		throw UnknownCommandException("#if op:" + op_true);

	FOREACH_T_CONST(std::vector<std::string>, it, value)
	{
		#define CHECK_RESULT		\
		{				\
			continue;		\
		}				\
		else				\
		{				\
			last_if_result = false;	\
			return false;		\
		}

		switch (op_type)
		{
			// != op_not causes op_not to reverse the boolean meaning when true.

			case OP_NONE:
			case OP_BOOL:

			if (parse<bool_t>(*it) != op_not)
				CHECK_RESULT

			case OP_EXISTS:

			if (
				(op_local ? hasObject(parse_name(*it)) : has_object(parse_name(*it))) != op_not)
				CHECK_RESULT

			case OP_FALSE:

			if (
				(
					!(op_local ? hasObject(parse_name(*it)) : has_object(parse_name(*it))) ||
					!convert<bool_t, obj_t>(op_local ? getObject(parse_name(*it)) : get_object(parse_name(*it)))
				) != op_not
			)
				CHECK_RESULT

			case OP_TRUE:

			if (
				(
					(op_local ? hasObject(parse_name(*it)) : has_object(parse_name(*it))) &&
					convert<bool_t, obj_t>(op_local ? getObject(parse_name(*it)) : get_object(parse_name(*it)))
				) != op_not
			)
				CHECK_RESULT
		}

		#undef CHECK_RESULT
	}

	last_if_result = true;
	addData(data);
	last_if_result = true;
	return true;
}

void LevelObject::addObject(name_t const & name, obj_t newObject)
{
	if (_data.get_dataType() != any_t::OBJMAP_T)
		throw InvalidTypeException("cannot assign key to non-object");

	if (newObject == NULL)
		return;

	if (name.size() != 1)
	{
		if (name.getString() == misc_name_global())
			return global_object->addObject(name.getRest(), newObject);

		return getObject(name.getFirst())->addObject(name.getRest(), newObject);
	}

	add_object(name, newObject);

	if (!name.empty())
		_data.getObjMap().add(name, newObject);
	else if (newObject->getType().getMode() == type_t::MODE_INLINE)
		_data.getObjMap().add(newObject);
}
void LevelObject::addObject(name_t const & name, SourceTokenDDL const & st)
{
	if (st.empty())
		return;

	if (name.empty() && st.getType().empty() && st.getBase().empty() && st.getValue().empty())
	{
		addData(st.getData());
		return;
	}

	// commands start with #
	if (!name.empty() && name.getString()[0] == '#')
	{
		doCommand(name.getString().substr(1), st);

		return;
	}

	if (name.size() != 1)
	{
		if (name.getString() == misc_name_global())
			return global_object->addObject(name.getRest(), st);

		return getObject(name.getFirst())->addObject(name.getRest(), st);
	}

	if (_data.get_dataType() != any_t::OBJMAP_T)
		throw InvalidTypeException("cannot assign key to non-object");

	type_t newType;

	if (st.getType().empty())
	{
		if (!st.getBase(0).empty())
			newType = get_object(parse_name(st.getBase(0)))->_type;
		else if (hasObject(name))
			newType = getObject(name)->_type;
		else if (has_object(parse_name(st.getValue())))
			newType = get_object(parse_name(st.getValue()))->_type;
		else
			newType = type_t::get_default_type(name, _type);
	}
	else
	{
		newType = type_t::get_type(st.getType());
	}

	if (option_force_default_types && type_t::has_default_type(name, _type) && (newType != type_t::get_default_type(name, _type)))
		throw InvalidTypeException("force-default-types:" + newType.makeString() + " for " + name.getString() + " in " + _type.makeString());

	obj_t newObject;

	switch (newType.getMode())
	{
	case type_t::MODE_VALUE:
		newObject = LevelObject::create(newType, st.getValue());

		break;

	case type_t::MODE_OBJECT:
	case type_t::MODE_COMPOUNDOBJECT:
	case type_t::MODE_INLINE:
		if (st.getValue().empty())
			newObject = LevelObject::create(newType, st.getData(), st.getBase());

		else
			newObject = parse_obj(st.getValue(), newType);

		break;

	default:
		throw InvalidTypeException(newType.makeString() + " is not a type");
	}

	addObject(name, newObject);
}
void LevelObject::addObject(SourceScannerDHLX & sc)
{
	// Check for semicolon/hash.
	{
		SourceTokenDHLX st(sc.get());

		SourceTokenDHLX::TokenType tt(st.getType());

		if (tt == SourceTokenDHLX::TT_OP_SEMICOLON)
			return;

		if (tt == SourceTokenDHLX::TT_OP_HASH)
		{
			doCommand(sc.get(SourceTokenDHLX::TT_IDENTIFIER).getData(), sc);

			return;
		}

		sc.unget(st);
	}

	type_t newType(type_t::type_null);

	// Check for type.
	{
		SourceTokenDHLX typeToken(sc.get(SourceTokenDHLX::TT_IDENTIFIER));

		std::string typeData(typeToken.getData());

		if (type_t::has_type(typeData))
		{
			newType = type_t::get_type(typeData);
		}
		else
		{
			sc.unget(typeToken);
		}
	}

	name_t newName(parse_name(sc));

	obj_t newObject(NULL);

	if (newType == type_t::type_null)
	{
		if (hasObject(newName))
		{
			newObject = getObject(newName);
			newType   = newObject->getType();
		}
		else
		{
			newType = type_t::get_default_type(newName, _type);
		}
	}

	SourceTokenDHLX opToken(sc.get());

	std::vector<name_t> baseName;

	while (opToken.getType() == SourceTokenDHLX::TT_OP_COLON)
	{
		baseName.push_back(parse_name(sc));

		opToken = sc.get();
	}

	switch (opToken.getType())
	{
	case SourceTokenDHLX::TT_OP_BRACE_O:
		if (newObject == NULL) newObject = create(newType);

		for (size_t index = 0; index < baseName.size(); ++index)
			newObject->addBase(baseName[index]);

		sc.unget(opToken);
		newObject->addData(sc);

		if ((newObject->_type.getMode() == type_t::MODE_COMPOUNDOBJECT) && !newObject->_isCompounded)
		{
			do_compound_object(newObject->_type.makeString(), newObject);
			newObject->_isCompounded = true;
		}

		break;

	case SourceTokenDHLX::TT_OP_EQUALS:
		if (newType.getMode() == type_t::MODE_VALUE)
			newObject = create(newType, sc);
		else
			newObject = get_object(parse<int_s_t>(sc), newType);

		sc.get(SourceTokenDHLX::TT_OP_SEMICOLON);

		break;

	case SourceTokenDHLX::TT_OP_SEMICOLON:
		if (newObject == NULL) newObject = create(newType);

		for (size_t index = 0; index < baseName.size(); ++index)
			newObject->addBase(baseName[index]);

		break;

	default:
		throw SyntaxException("Unexpected token:" + make_string(opToken.getType()));
	}

	addObject(newName, newObject);
}



