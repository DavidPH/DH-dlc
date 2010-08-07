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
#include "options.hpp"
#include "parsing.hpp"
#include "process_file.hpp"
#include "SourceToken.hpp"

#include "exceptions/CompilerException.hpp"
#include "exceptions/SyntaxException.hpp"

#include "parsing/FunctionHandlerDDL.hpp"



void process_token(SourceTokenDDL const & st, SourceScannerDDL & sc)
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
			std::vector<std::string> type_vec(st.getBase());
			type_vec.push_back(st.getType());

			for (size_t index = 1; index < type_vec.size(); ++index)
			{
				if (type_vec[index] == "")
					break;

				type_t type(type_t::get_type(type_vec[index]));

				if (type == type_t::type_bool()) FunctionHandler<bool_t>::add_function(type_vec[0], new FunctionHandlerDDL<bool_t>(st.getData()));

				     if (type == type_t::type_bool())       FunctionHandler<bool_t>::add_function(type_vec[0], new FunctionHandlerDDL<bool_t>(st.getData()));
				else if (type == type_t::type_shortint())   FunctionHandler<int_s_t>::add_function(type_vec[0], new FunctionHandlerDDL<int_s_t>(st.getData()));
				else if (type == type_t::type_int())        FunctionHandler<int_t>::add_function(type_vec[0], new FunctionHandlerDDL<int_t>(st.getData()));
				else if (type == type_t::type_longint())    FunctionHandler<int_l_t>::add_function(type_vec[0], new FunctionHandlerDDL<int_l_t>(st.getData()));

				else if (type == type_t::type_shortfloat()) FunctionHandler<real_s_t>::add_function(type_vec[0], new FunctionHandlerDDL<real_s_t>(st.getData()));
				else if (type == type_t::type_float())      FunctionHandler<real_t>::add_function(type_vec[0], new FunctionHandlerDDL<real_t>(st.getData()));
				else if (type == type_t::type_longfloat())  FunctionHandler<real_l_t>::add_function(type_vec[0], new FunctionHandlerDDL<real_l_t>(st.getData()));

				else if (type == type_t::type_string())     FunctionHandler<string_t>::add_function(type_vec[0], new FunctionHandlerDDL<string_t>(st.getData()));
				else if (type == type_t::type_string8())    FunctionHandler<string8_t>::add_function(type_vec[0], new FunctionHandlerDDL<string8_t>(st.getData()));
				else if (type == type_t::type_string16())   FunctionHandler<string16_t>::add_function(type_vec[0], new FunctionHandlerDDL<string16_t>(st.getData()));
				else if (type == type_t::type_string32())   FunctionHandler<string32_t>::add_function(type_vec[0], new FunctionHandlerDDL<string32_t>(st.getData()));
				else if (type == type_t::type_string80())   FunctionHandler<string80_t>::add_function(type_vec[0], new FunctionHandlerDDL<string80_t>(st.getData()));
				else if (type == type_t::type_string320())  FunctionHandler<string320_t>::add_function(type_vec[0], new FunctionHandlerDDL<string320_t>(st.getData()));

				else if (type == type_t::type_ubyte())      FunctionHandler<ubyte_t>::add_function(type_vec[0], new FunctionHandlerDDL<ubyte_t>(st.getData()));
				else if (type == type_t::type_sword())      FunctionHandler<sword_t>::add_function(type_vec[0], new FunctionHandlerDDL<sword_t>(st.getData()));
				else if (type == type_t::type_uword())      FunctionHandler<uword_t>::add_function(type_vec[0], new FunctionHandlerDDL<uword_t>(st.getData()));
				else if (type == type_t::type_sdword())     FunctionHandler<sdword_t>::add_function(type_vec[0], new FunctionHandlerDDL<sdword_t>(st.getData()));
				else if (type == type_t::type_udword())     FunctionHandler<udword_t>::add_function(type_vec[0], new FunctionHandlerDDL<udword_t>(st.getData()));
			}

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
		}
		else
		{
			global_object->doCommand(commandString, sc);
		}
	}
	else if (st.getType() == SourceTokenDHLX::TT_IDENTIFIER)
	{
		sc.unget(st);
		global_object->addObject(sc);
	}
	else
	{
		throw SyntaxException("invalid top-level token:" + make_string(st));
	}
}



