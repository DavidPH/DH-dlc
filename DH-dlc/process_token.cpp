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
	2010/02/28 - Update for lo_type enum.
*/

#include "process_token.hpp"

#include "compound_objects.hpp"
#include "global_object.hpp"
#include "LevelObject.hpp"
#include "LevelObjectName.hpp"
#include "LevelObjectType.hpp"
#include "options.hpp"
#include "parsing.hpp"
#include "process_file.hpp"
#include "SourceToken.hpp"
#include "exceptions/CompilerException.hpp"
#include "exceptions/SyntaxException.hpp"



void process_token(SourceToken const & st, SourceScannerDDL & sc)
{
	if (st.empty())
		return;

	// commands start with #
	if (st.getName().size() > 0 && st.getName()[0] == '#')
	{
		// # default type : NAME : TYPE : [CONTEXT]
		if (st.getName() == command_name_defaulttype())
		{
			name_t type_name(st.getBase(0));
			type_t type = type_t::get_type(st.getBase(1));
			type_t context;

			if (!st.getBase(2).empty())
				context = type_t::get_type(st.getBase(2));

			type_t::add_default_type(type_name, context, type);

			return;
		}

		// # define : TYPE { data }
		if (st.getName() == command_name_define())
		{
			add_compound_object(st.getBase(0), st.getData());

			return;
		}

		// [TYPE] # function : NAME [: TYPE ...] { data }
		if (st.getName() == command_name_function())
		{
			add_function(st.getType(), st.getBase(0), st.getData());

			for (size_t index = 1; index < st.getBase().size(); ++index)
				add_function(st.getBase(index), st.getBase(0), st.getData());

			return;
		}

		// # include : FILENAME
		if (st.getName() == command_name_include())
		{
			process_file(st.getBase(0));

			return;
		}

		// # precision : NEW_PRECISION
		if (st.getName() == command_name_precision())
		{
			set_precision(parse_int_s(st.getBase(0)));

			return;
		}

		// # typedef : OLD_TYPE : NEW_TYPE
		if (st.getName() == command_name_typedef())
		{
			type_t::add_redirect_type(st.getBase(1), type_t::get_type(st.getBase(0)));

			return;
		}

		// # typedef new : TYPE : MODE
		if (st.getName() == command_name_typedefnew())
		{
			if (st.getBase(1) == "dynamic")
				type_t::add_type(st.getBase(0), type_t::MODE_DYNAMIC);

			else if (st.getBase(1) == "value")
				type_t::add_type(st.getBase(0), type_t::MODE_VALUE);

			else if (st.getBase(1) == "object")
				type_t::add_type(st.getBase(0), type_t::MODE_OBJECT);

			else if (st.getBase(1) == "compoundobject")
				type_t::add_type(st.getBase(0), type_t::MODE_COMPOUNDOBJECT);

			else if (st.getBase(1) == "inline")
				type_t::add_type(st.getBase(0), type_t::MODE_INLINE);

			else
				throw CompilerException("unknown typedefnew:" + st.getBase(1));

			return;
		}
	}

	global_object->addObject(name_t(st.getName()), st);
}

void process_token(SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	if ((st.getType() == SourceTokenDHLX::TT_EOF) || (st.getType() == SourceTokenDHLX::TT_NONE))
		return;

	if (st.getType() == SourceTokenDHLX::TT_OP_HASH)
	{
		SourceTokenDHLX commandToken(sc.get(SourceTokenDHLX::TT_IDENTIFIER));

		std::string commandString('#' + commandToken.getData());

		if (commandString == command_name_include())
		{
			SourceTokenDHLX arg0(sc.get(SourceTokenDHLX::TT_STRING, SourceTokenDHLX::TT_OP_COLON));

			sc.get(SourceTokenDHLX::TT_OP_SEMICOLON);

			process_file(arg0.getData());

			return;
		}
	}
}



