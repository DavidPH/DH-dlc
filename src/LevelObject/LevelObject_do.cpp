/* Copyright (C) 2010, 2011 David Hill
**
** This file is part of DH-dlc.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* LevelObject/LevelObject_do.cpp
**
** Defines the LevelObject::do* methods.
*/

#include "LevelObject.hpp"

#include "LevelObjectMap.hpp"

#include "../compound_objects.hpp"
#include "../global_object.hpp"
#include "../options.hpp"
#include "../scripts.hpp"

#include "../exceptions/UnknownCommandException.hpp"

#include "../parsing/parsing.hpp"

#include "../types/string_t.hpp"

#include "../../common/foreach.hpp"



void LevelObject::doCommand(std::string const & command, SourceScannerDHLX & sc)
{
	// # break;
	if (command == command_name_break())
	{
		_isBreaked = true;

		sc.get(SourceTokenDHLX::TT_OP_SEMICOLON);
	}
	// # change type : TYPE : [VALUE]
	// VALUE is used when changing to a value type
	else if (command == command_name_changetype())
	{
		setType(type_t::get_type(sc.get(SourceTokenDHLX::TT_IDENTIFIER).getData()));

		sc.get(SourceTokenDHLX::TT_OP_SEMICOLON);
	}

	// # compound [IDENTIFIER];
	else if (command == command_name_compound())
	{
		SourceTokenDHLX type(sc.get());

		if (type.getType() == SourceTokenDHLX::TT_IDENTIFIER)
		{
			do_compound_object(type.getData(), this);
		}
		else
		{
			do_compound_object(_type.makeString(), this);
			sc.unget(type);
		}

		sc.get(SourceTokenDHLX::TT_OP_SEMICOLON);
	}
	// # continue;
	else if (command == command_name_continue())
	{
		_isContinued = true;

		sc.get(SourceTokenDHLX::TT_OP_SEMICOLON);
	}
	// # delete KEY...
	// removes KEYs from this object
	else if (command == command_name_delete() && _data.get_dataType() == any_t::OBJMAP_T)
	{
		_data.getObjMap().del(parse_name(sc));

		sc.get(SourceTokenDHLX::TT_OP_SEMICOLON);
	}
	else if (command == command_name_else())
	{
		if (!last_if_result)
			addData(sc);
		else
			skipData(sc);
	}
	// # if expression block
	else if (command == command_name_if())
		addDataIf(sc);

	// # info string-expr;
	else if (command == command_name_info())
		doCommandInfo(sc);

	// # return expression;
	else if (command == command_name_return())
	{
		obj_t returnType  = getObject(name_t::name_return_type);
		obj_t returnValue = create(returnType->_data.getType(), sc);

		sc.get(SourceTokenDHLX::TT_OP_SEMICOLON);

		addObject(name_t::name_return_value, returnValue);

		_isReturned = 1;
	}
	// # script-acs string-expr... ;
	else if (command == command_name_script_acs())
	{
		std::string nameSCRIPTS(option_script_acs);
		if (option_use_file_extensions && nameSCRIPTS.find('.') == std::string::npos)
			nameSCRIPTS += ".acs";

		SourceTokenDHLX token(sc.get());

		while (token.getType() != SourceTokenDHLX::TT_OP_SEMICOLON)
		{
			sc.unget(token);

			add_script(nameSCRIPTS, parse<string_t>(sc).makeString());

			token = sc.get();
		}
	}
	// # while (expression) block
	else if (command == command_name_while())
	{
		SourceScannerDHLX cond(sc.getblock(SourceTokenDHLX::TT_OP_PARENTHESIS_O, SourceTokenDHLX::TT_OP_PARENTHESIS_C));
		SourceScannerDHLX data(sc.getblock(SourceTokenDHLX::TT_OP_BRACE_O, SourceTokenDHLX::TT_OP_BRACE_C));

		while (true)
		{
			SourceScannerDHLX condcopy(cond);
			if (!parse<bool_t>(condcopy)) break;

			SourceScannerDHLX datacopy(data);
			addData(datacopy);

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
		throw UnknownCommandException(command);
}

void LevelObject::doCommand(std::string const & command, SourceTokenDDL const & st)
{
	// # break
	if (command == command_name_break())
		_isBreaked = true;

	// # change type : TYPE : [VALUE]
	// VALUE is used when changing to a value type
	else if (command == command_name_changetype())
		setType(type_t::get_type(st.getBase(0)), st.getBase(1));

	// # compound : [TYPE]
	else if (command == command_name_compound())
	{
		if (!st.getBase(0).empty())
			do_compound_object(st.getBase(0), this);
		else
			do_compound_object(_type.makeString(), this);

		_isCompounded = true;
	}

	// # continue
	else if (command == command_name_continue())
		_isContinued = true;

	// # debug : message
	else if (command == command_name_debug())
		PRINT_AND_COUNT_DEBUG(parse<string_t>(st.getBase(0)).makeString() << '\n');

	// # delete : KEY
	// removes KEY from this object
	// this DOES NOT prevent objects from being output
	else if (command == command_name_delete() && _data.get_dataType() == any_t::OBJMAP_T)
	{
		if (hasObject(parse_name(st.getBase(0))))
			_data.getObjMap().del(parse_name(st.getBase(0)));

		clean_objects();
	}

	// # delete volatile
	// removes KEYs that have volatile names
	else if (command == command_name_deletevolatile() && _data.get_dataType() == any_t::OBJMAP_T)
	{
		FOREACH_T(objmap_t, it, _data.getObjMap())
		{
			if (it->first.isVolatile())
				_data.getObjMap().del((it--)->first);
		}

		clean_objects();
	}

	// # delete _
	// removes KEYs that start with _ but not with __
	else if (command == command_name_delete_() && _data.get_dataType() == any_t::OBJMAP_T)
	{
		FOREACH_T(objmap_t, it, _data.getObjMap())
		{
			std::string itName = it->first.getString();

			if (itName.size() > 1 && itName[0] == '_' && itName[1] != '_')
				_data.getObjMap().del(it->first);
		}

		clean_objects();
	}

	// # do : condition {data}
	else if (command == command_name_do())
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
		while (parse<bool_t>(st.getBase(0)));
	}

	// # else { data }
	else if (command == command_name_else())
		addDataIf(st.getData(), "", "", "", "", true);

	// # else if cmp : value1 : op : value2 : [type] { data }
	else if (command == command_name_elseifcmp())
		addDataIf(st.getData(), st.getBase(0), st.getBase(2), st.getBase(1), st.getBase(3), true);

	// # else if* : name... { data }
	else if (command.substr(0, 6) == command_name_elseif())
		addDataIf(st.getData(), st.getBase(), command.substr(6), true);

	// # error : message
	// prints message and counts towards error-limit
	else if (command == command_name_error())
		PRINT_AND_COUNT_ERROR(parse<string_t>(st.getBase(0)).makeString() << '\n');

	// [type] # for : name : start : stop : step { data }
	else if (command == command_name_for())
	{
		type_t forType;
		if (st.getType().empty())
			forType = type_t::get_type(type_name_int());
		else
			forType = type_t::get_type(st.getType());

		name_t      forName(parse_name(st.getBase(0)));
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

		_data.getObjMap().del(forName);
	}

	// # if cmp : value1 : op : value2 : [type] { data }
	else if (command == command_name_ifcmp())
		addDataIf(st.getData(), st.getBase(0), st.getBase(2), st.getBase(1), st.getBase(3));

	// # if* : name... { data }
	else if (command.substr(0, 2) == command_name_if())
		addDataIf(st.getData(), st.getBase(), command.substr(2));

	// # info : message
	else if (command == command_name_info())
		PRINT_AND_COUNT_INFO(parse<string_t>(st.getBase(0)).makeString() << '\n');

	// # return : VALUE
	// Used to return a value from a function.
	else if (command == command_name_return() && _data.get_dataType() == any_t::OBJMAP_T)
	{
		obj_t returnType  = getObject(name_t::name_return_type);
		obj_t returnValue = create(returnType->_data.getType(), st.getBase(0));
		addObject(name_t::name_return_value, returnValue);
		_isReturned = 1;
	}

	// # script : FILENAME { data }
	else if (command == command_name_script())
	{
		for (size_t index = 1; index < st.getBase().size(); ++index)
			add_script(st.getBase(0), parse<string_t>(st.getBase(index)).makeString());

		add_script(st.getBase(0), st.getData());
	}

	// # script-acs { data }
	else if (command == command_name_script_acs())
	{
		std::string nameSCRIPTS(option_script_acs);
		if (option_use_file_extensions && nameSCRIPTS.find('.') == std::string::npos)
			nameSCRIPTS += ".acs";

		for (size_t index = 0; index < st.getBase().size(); ++index)
			add_script(nameSCRIPTS, parse<string_t>(st.getBase(index)).makeString());

		add_script(nameSCRIPTS, st.getData());
	}

	// # script-extradata { data }
	else if (command == command_name_script_extradata())
	{
		std::string nameExtraData(option_script_extradata);
		if (option_use_file_extensions && nameExtraData.find('.') == std::string::npos)
			nameExtraData += ".txt";

		for (size_t index = 0; index < st.getBase().size(); ++index)
			add_script(nameExtraData, parse<string_t>(st.getBase(index)).makeString());

		add_script(nameExtraData, st.getData());
	}

	// # script-fraggle { data }
	else if (command == command_name_script_fraggle())
	{
		std::string nameFRAGGLE(option_map_name);
		if (option_use_file_extensions && nameFRAGGLE.find('.') == std::string::npos)
			nameFRAGGLE += ".txt";

		for (size_t index = 0; index < st.getBase().size(); ++index)
			add_script(nameFRAGGLE, parse<string_t>(st.getBase(index)).makeString());

		add_script(nameFRAGGLE, st.getData());
	}

	// # warn : message
	else if (command == command_name_warn())
		PRINT_AND_COUNT_WARNING(parse<string_t>(st.getBase(0)).makeString() << '\n');

	// # while : condition {data}
	else if (command == command_name_while())
	{
		while (parse<bool_t>(st.getBase(0)))
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
		throw UnknownCommandException(command);
}

void LevelObject::doCommandInfo(SourceScannerDHLX & sc)
{
	doCommandInfo(parse<string_t>(sc));

	sc.get(SourceTokenDHLX::TT_OP_SEMICOLON);
}
void LevelObject::doCommandInfo(SourceTokenDDL const & st)
{
	doCommandInfo(parse<string_t>(st.getBase(0)));
}
void LevelObject::doCommandInfo(string_t const & data)
{
	PRINT_AND_COUNT_INFO(data.makeString() << '\n');
}

