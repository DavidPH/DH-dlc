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

*/

#include "LevelObject.hpp"

#include "LevelObjectMap.hpp"

#include "../compound_objects.hpp"
#include "../global_object.hpp"
#include "../options.hpp"
#include "../parsing.hpp"
#include "../scripts.hpp"

#include "../exceptions/UnknownCommandException.hpp"

#include "../types/string_t.hpp"

#include "../../common/foreach.hpp"



void LevelObject::doCommand(std::string const & command, SourceScannerDHLX & sc)
{
	// # break
	if (command == command_name_break())
		_isBreaked = true;

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
			addData(get_compound_object(st.getBase(0)), st.getBase(0));

		else
			addData(get_compound_object(_type.makeString()), _type.makeString());

		_isCompounded = true;
	}

	// # continue
	else if (command == command_name_continue())
		_isContinued = true;

	// # debug : message
	else if (command == command_name_debug())
		PRINT_AND_COUNT_DEBUG(parse_string(st.getBase(0)).makeString() << '\n');

	// # delete : KEY
	// removes KEY from this object
	// this DOES NOT prevent objects from being output
	else if (command == command_name_delete() && _data.getType() == any_t::OBJMAP_T)
	{
		if (hasObject(name_t(st.getBase(0))))
			_data.getObjMap().del(name_t(st.getBase(0)));

		clean_objects();
	}

	// # delete volatile
	// removes KEYs that have volatile names
	else if (command == command_name_deletevolatile() && _data.getType() == any_t::OBJMAP_T)
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
	else if (command == command_name_delete_() && _data.getType() == any_t::OBJMAP_T)
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
		while (parse_bool(st.getBase(0)));
	}

	// # else { data }
	else if (command == command_name_else())
		addDataIf(st.getData(), "", "", "", "", true);

	// # else if cmp : value1 : op : value2 : [type] { data }
	else if (command == command_name_elseifcmp())
		addDataIf(st.getData(), st.getBase(0), st.getBase(2), st.getBase(1), st.getBase(3), true);

	// # else if* : name... { data }
	else if (command.substr(0, 7) == command_name_elseif())
		addDataIf(st.getData(), st.getBase(), command.substr(7), true);

	// # error : message
	// prints message and counts towards error-limit
	else if (command == command_name_error())
		PRINT_AND_COUNT_ERROR(parse_string(st.getBase(0)).makeString() << '\n');

	// [type] # for : name : start : stop : step { data }
	else if (command == command_name_for())
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

		_data.getObjMap().del(forName);
	}

	// # if cmp : value1 : op : value2 : [type] { data }
	else if (command == command_name_ifcmp())
		addDataIf(st.getData(), st.getBase(0), st.getBase(2), st.getBase(1), st.getBase(3));

	// # if* : name... { data }
	else if (command.substr(0, 3) == command_name_if())
		addDataIf(st.getData(), st.getBase(), command.substr(3));

	// # info : message
	else if (command == command_name_info())
		PRINT_AND_COUNT_INFO(parse_string(st.getBase(0)).makeString() << '\n');

	// # return : VALUE
	// Used to return a value from a function.
	else if (command == command_name_return() && _data.getType() == any_t::OBJMAP_T)
	{
		obj_t returnType  = getObject(name_t::name_return_type);
		obj_t returnValue = create(type_t::get_type(convert<string_t, obj_t>(returnType).makeString()), st.getBase(0));
		addObject(name_t::name_return_value, returnValue);
		_isReturned = 1;
	}

	// # script : FILENAME { data }
	else if (command == command_name_script())
	{
		for (size_t index = 1; index < st.getBase().size(); ++index)
			add_script(st.getBase(0), parse_string(st.getBase(index)).makeString());

		add_script(st.getBase(0), st.getData());
	}

	// # script-acs { data }
	else if (command == command_name_script_acs())
	{
		std::string nameSCRIPTS(option_script_acs);
		if (option_use_file_extensions && nameSCRIPTS.find('.') == std::string::npos)
			nameSCRIPTS += ".acs";

		for (size_t index = 0; index < st.getBase().size(); ++index)
			add_script(nameSCRIPTS, parse_string(st.getBase(index)).makeString());

		add_script(nameSCRIPTS, st.getData());
	}

	// # script-extradata { data }
	else if (command == command_name_script_extradata())
	{
		std::string nameExtraData(option_script_extradata);
		if (option_use_file_extensions && nameExtraData.find('.') == std::string::npos)
			nameExtraData += ".txt";

		for (size_t index = 0; index < st.getBase().size(); ++index)
			add_script(nameExtraData, parse_string(st.getBase(index)).makeString());

		add_script(nameExtraData, st.getData());
	}

	// # script-fraggle { data }
	else if (command == command_name_script_fraggle())
	{
		std::string nameFRAGGLE(option_map_name);
		if (option_use_file_extensions && nameFRAGGLE.find('.') == std::string::npos)
			nameFRAGGLE += ".txt";

		for (size_t index = 0; index < st.getBase().size(); ++index)
			add_script(nameFRAGGLE, parse_string(st.getBase(index)).makeString());

		add_script(nameFRAGGLE, st.getData());
	}

	// # warn : message
	else if (command == command_name_warn())
		PRINT_AND_COUNT_WARNING(parse_string(st.getBase(0)).makeString() << '\n');

	// # while : condition {data}
	else if (command == command_name_while())
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
		throw UnknownCommandException(command);
}



