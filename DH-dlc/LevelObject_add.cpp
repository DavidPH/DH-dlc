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

#include <cstdlib>
#include <iostream>
#include <sstream>

#include "compound_objects.hpp"
#include "global_object.hpp"
#include "LevelObjectName.hpp"
#include "math.hpp"
#include "options.hpp"
#include "parsing.hpp"
#include "scripts.hpp"
#include "SourceStream.hpp"
#include "SourceToken.hpp"
#include "types.hpp"
#include "../common/foreach.hpp"
#include "exceptions/CompilerException.hpp"
#include "exceptions/InvalidTypeException.hpp"
#include "exceptions/NoSuchElementException.hpp"
#include "exceptions/SyntaxException.hpp"
#include "exceptions/UnknownCommandException.hpp"
#include "types/int_t.hpp"
#include "types/real_t.hpp"
#include "types/string_t.hpp"



void LevelObject::addBase(std::string const & base)
{
	if (_data.getType() != any_t::OBJMAP_T)
		throw InvalidTypeException("non-objects have no keys");

	obj_t other = get_object(name_t(base));

	if (other->_data.getType() != any_t::OBJMAP_T)
		throw InvalidTypeException("non-objects have no keys");

	_data.getObjMap().insert(other->_data.getObjMap().begin(), other->_data.getObjMap().end());
}

void LevelObject::addData(SourceScannerDHLX & sc)
{
	LevelObjectStack los(this);

	SourceTokenDHLX st;

	while (true)
	{
		st = sc.get();
		sc.unget(st); // TODO: SourceScanner::peek

		if ((st.getType() == SourceTokenDHLX::TT_OP_BRACE_C) || (st.getType() == SourceTokenDHLX::TT_EOF))
		{
			break;
		}

		addObject(sc);
	}
}
void LevelObject::addData(std::string const & data, std::string const & name)
{
	LevelObjectStack los(this);

	std::istringstream iss(data);
	SourceStream       ss(iss);
	SourceToken        st;

	while (ss)
	{
		if (_isBreaked || _isContinued || _isReturned)
			return;

		try
		{
			st.clear();
			ss >> st;
			addObject(name_t(st.getName()), st);
		}
		catch (CompilerException& e)
		{
			PRINT_ERROR(name << ':' << ss.getLineCount() << ':' << e << "\n  ->" << st << '\n');

			throw;
		}
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
		cmpResult = cmp( get_object(name_t(value1))->_data, get_object(name_t(value2))->_data );

	else if (type == type_name_bool())
		cmpResult = cmp( parse_bool(value1), parse_bool(value2) );

	else if (type == type_name_shortint())
		cmpResult = cmp( parse_int_s(value1), parse_int_s(value2) );
	else if (type == type_name_int())
		cmpResult = cmp( parse_int(value1), parse_int(value2) );
	else if (type == type_name_longint())
		cmpResult = cmp( parse_int_l(value1), parse_int_l(value2) );

	else if (type == type_name_shortfloat())
		cmpResult = cmp( parse_real_s(value1), parse_real_s(value2) );
	else if (type == type_name_float())
		cmpResult = cmp( parse_real(value1), parse_real(value2) );
	else if (type == type_name_longfloat())
		cmpResult = cmp( parse_real_l(value1), parse_real_l(value2) );

	else if (type == type_name_string())
		cmpResult = cmp( parse_string(value1), parse_string(value2) );

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

			if (parse_bool(*it) != op_not)
				CHECK_RESULT

			case OP_EXISTS:

			if (
				(op_local ? hasObject(name_t(*it)) : has_object(name_t(*it))) != op_not)
				CHECK_RESULT

			case OP_FALSE:

			if (
				(
					!(op_local ? hasObject(name_t(*it)) : has_object(name_t(*it))) ||
					!to_bool(op_local ? getObject(name_t(*it)) : get_object(name_t(*it)))
				) != op_not
			)
				CHECK_RESULT

			case OP_TRUE:

			if (
				(
					(op_local ? hasObject(name_t(*it)) : has_object(name_t(*it))) &&
					to_bool(op_local ? getObject(name_t(*it)) : get_object(name_t(*it)))
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
	if (_data.getType() != any_t::OBJMAP_T)
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
		_data.getObjMap()[name] = newObject;
}
void LevelObject::addObject(name_t const & name, SourceToken const & st)
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
		std::string commandName = name.getString();

		// # break
		if (commandName == command_name_break())
			_isBreaked = true;

		// # change type : TYPE : [VALUE]
		// VALUE is used when changing to a value type
		else if (commandName == command_name_changetype())
			setType(type_t::get_type(st.getBase(0)), st.getBase(1));

		// # compound : [TYPE]
		else if (commandName == command_name_compound())
		{
			if (!st.getBase(0).empty())
				addData(get_compound_object(st.getBase(0)), st.getBase(0));

			else
				addData(get_compound_object(_type.makeString()), _type.makeString());

			_isCompounded = true;
		}

		// # continue
		else if (commandName == command_name_continue())
			_isContinued = true;

		// # debug : message
		else if (commandName == command_name_debug())
			PRINT_AND_COUNT_DEBUG(parse_string(st.getBase(0)).makeString() << '\n');

		// # delete : KEY
		// removes KEY from this object
		// this DOES NOT prevent objects from being output
		else if (commandName == command_name_delete() && _data.getType() == any_t::OBJMAP_T)
		{
			if (hasObject(name_t(st.getBase(0))))
				_data.getObjMap().erase(name_t(st.getBase(0)));

			clean_objects();
		}

		// # delete volatile
		// removes KEYs that have volatile names
		else if (commandName == command_name_deletevolatile() && _data.getType() == any_t::OBJMAP_T)
		{
			FOREACH_T(objmap_t, it, _data.getObjMap())
			{
				if (it->first.isVolatile())
					_data.getObjMap().erase(it->first);
			}

			clean_objects();
		}

		// # delete _
		// removes KEYs that start with _ but not with __
		else if (commandName == command_name_delete_() && _data.getType() == any_t::OBJMAP_T)
		{
			FOREACH_T(objmap_t, it, _data.getObjMap())
			{
				std::string itName = it->first.getString();

				if (itName.size() > 1 && itName[0] == '_' && itName[1] != '_')
					_data.getObjMap().erase(it->first);
			}

			clean_objects();
		}

		// # do : condition {data}
		else if (commandName == command_name_do())
		{
			do
			{
				addData(st.getData());

				if (_isContinued)
				{
					_isContinued = false;
					continue;
				}

				if (_isBreaked)
				{
					_isBreaked = false;
					break;
				}
			}
			while (parse_bool(st.getBase(0)));
		}

		// # else { data }
		else if (commandName == command_name_else())
			addDataIf(st.getData(), "", "", "", "", true);

		// # else if cmp : value1 : op : value2 : [type] { data }
		else if (commandName == command_name_elseifcmp())
			addDataIf(st.getData(), st.getBase(0), st.getBase(2), st.getBase(1), st.getBase(3), true);

		// # else if* : name... { data }
		else if (commandName.substr(0, 7) == command_name_elseif())
			addDataIf(st.getData(), st.getBase(), commandName.substr(7), true);

		// # error : message
		// prints message and counts towards error-limit
		else if (commandName == command_name_error())
			PRINT_AND_COUNT_ERROR(parse_string(st.getBase(0)).makeString() << '\n');

		// [type] # for : name : start : stop : step { data }
		else if (commandName == command_name_for())
		{
			type_t forType;
			if (st.getType().empty())
				forType = type_t::get_type(type_name_int());
			else
				forType = type_t::get_type(st.getType());

			name_t      forName   (st.getBase(0));
			std::string forStart = st.getBase(1);
			std::string forStop  = st.getBase(2);
			std::string forStep  = st.getBase(3, "1");

			obj_t forIterObj = LevelObject::create(forType, forStart);
			obj_t forStopObj = LevelObject::create(forType, forStop);
			obj_t forStepObj = LevelObject::create(forType, forStep);

			addObject(forName, forIterObj);

			while (forIterObj->_data < forStopObj->_data)
			{
				addData(st.getData());

				forIterObj->_data += forStepObj->_data;

				if (_isContinued)
				{
					_isContinued = false;
					continue;
				}

				if (_isBreaked)
				{
					_isBreaked = false;
					break;
				}
			}

			_data.getObjMap().erase(forName);
		}

		// # if cmp : value1 : op : value2 : [type] { data }
		else if (commandName == command_name_ifcmp())
			addDataIf(st.getData(), st.getBase(0), st.getBase(2), st.getBase(1), st.getBase(3));

		// # if* : name... { data }
		else if (commandName.substr(0, 3) == command_name_if())
			addDataIf(st.getData(), st.getBase(), commandName.substr(3));

		// # info : message
		else if (commandName == command_name_info())
			PRINT_AND_COUNT_INFO(parse_string(st.getBase(0)).makeString() << '\n');

		// # return : VALUE
		// Used to return a value from a function.
		else if (commandName == command_name_return() && _data.getType() == any_t::OBJMAP_T)
		{
			obj_t returnType  = getObject(name_t::name_return_type);
			obj_t returnValue = create(type_t::get_type(to_string(returnType).makeString()), st.getBase(0));
			addObject(name_t::name_return_value, returnValue);
			_isReturned = 1;
		}

		// # script : FILENAME { data }
		else if (commandName == command_name_script())
		{
			for (size_t index = 1; index < st.getBase().size(); ++index)
				add_script(st.getBase(0), parse_string(st.getBase(index)).makeString());

			add_script(st.getBase(0), st.getData());
		}

		// # script-acs { data }
		else if (commandName == command_name_script_acs())
		{
			std::string nameSCRIPTS(option_script_acs);
			if (option_use_file_extensions && nameSCRIPTS.find('.') == std::string::npos)
				nameSCRIPTS += ".acs";

			for (size_t index = 0; index < st.getBase().size(); ++index)
				add_script(nameSCRIPTS, parse_string(st.getBase(index)).makeString());

			add_script(nameSCRIPTS, st.getData());
		}

		// # script-extradata { data }
		else if (commandName == command_name_script_extradata())
		{
			std::string nameExtraData(option_script_extradata);
			if (option_use_file_extensions && nameExtraData.find('.') == std::string::npos)
				nameExtraData += ".txt";

			for (size_t index = 0; index < st.getBase().size(); ++index)
				add_script(nameExtraData, parse_string(st.getBase(index)).makeString());

			add_script(nameExtraData, st.getData());
		}

		// # script-fraggle { data }
		else if (commandName == command_name_script_fraggle())
		{
			std::string nameFRAGGLE(option_map_name);
			if (option_use_file_extensions && nameFRAGGLE.find('.') == std::string::npos)
				nameFRAGGLE += ".txt";

			for (size_t index = 0; index < st.getBase().size(); ++index)
				add_script(nameFRAGGLE, parse_string(st.getBase(index)).makeString());

			add_script(nameFRAGGLE, st.getData());
		}

		// # warn : message
		else if (commandName == command_name_warn())
			PRINT_AND_COUNT_WARNING(parse_string(st.getBase(0)).makeString() << '\n');

		// # while : condition {data}
		else if (commandName == command_name_while())
		{
			while (parse_bool(st.getBase(0)))
			{
				addData(st.getData());

				if (_isContinued)
				{
					_isContinued = false;
					continue;
				}

				if (_isBreaked)
				{
					_isBreaked = false;
					break;
				}
			}
		}

		else
			throw UnknownCommandException(commandName);

		return;
	}

	if (name.size() != 1)
	{
		if (name.getString() == misc_name_global())
			return global_object->addObject(name.getRest(), st);

		return getObject(name.getFirst())->addObject(name.getRest(), st);
	}

	if (_data.getType() != any_t::OBJMAP_T)
		throw InvalidTypeException("cannot assign key to non-object");

	type_t newType;

	if (st.getType().empty())
	{
		if (!st.getBase(0).empty())
			newType = get_object(name_t(st.getBase(0)))->_type;
		else if (hasObject(name))
			newType = getObject(name)->_type;
		else if (has_object(name_t(st.getValue())))
			newType = get_object(name_t(st.getValue()))->_type;
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
	SourceTokenDHLX typeToken = sc.get(SourceTokenDHLX::TT_IDENTIFIER);

	type_t newType(type_t::type_null);

	if (type_t::has_type(typeToken.getData()))
	{
		newType = type_t::get_type(typeToken.getData());
	}
	else
	{
		sc.unget(typeToken);
	}

	name_t newName(parse_name(sc));

	if (newType == type_t::type_null)
	{
		if (hasObject(newName))
			newType = getObject(newName)->_type;
		else
			newType = type_t::get_default_type(newName, _type);
	}

	obj_t newObject;

	SourceTokenDHLX opToken(sc.get());

	switch (opToken.getType())
	{
	case SourceTokenDHLX::TT_OP_BRACE_O:
		newObject = create(newType);
		newObject->addData(sc);
		sc.get(SourceTokenDHLX::TT_OP_BRACE_C);
		break;

	case SourceTokenDHLX::TT_OP_EQUALS:
		newObject = create(newType, sc);
		sc.get(SourceTokenDHLX::TT_OP_SEMICOLON);
		break;

	default:
		throw SyntaxException("Unexpected token:" + make_string(opToken.getType()));
	}

	addObject(newName, newObject);
}



