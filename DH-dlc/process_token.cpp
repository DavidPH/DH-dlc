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
#include "lo_types.hpp"
#include "options.hpp"
#include "parsing.hpp"
#include "process_file.hpp"
#include "SourceToken.hpp"
#include "exceptions/CompilerException.hpp"



void process_token(SourceToken st) throw(CompilerException)
{
	if (st.empty())
		return;

	// commands start with #
	if (st.getName().size() > 0 && st.getName()[0] == '#')
	{
		// # default type : NAME : TYPE : [CONTEXT]
		if (st.getName() == command_name_defaulttype())
		{
			add_default_type(st.getBase(0), st.getBase(1), st.getBase(2));

			return;
		}

		// # define : TYPE { data }
		if (st.getName() == command_name_define())
		{
			add_compound_object(st.getBase(0), st.getData());

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
			add_lo_type_redirect(st.getBase(1), st.getBase(0));

			return;
		}

		// # typedef new : TYPE : value|object|compound object|inline
		if (st.getName() == command_name_typedefnew())
		{
			if (st.getBase(1) == "value")
				add_lo_type(st.getBase(0), LO_TYPE_VALUE);

			else if (st.getBase(1) == "object")
				add_lo_type(st.getBase(0), LO_TYPE_OBJECT);

			else if (st.getBase(1) == "compoundobject")
				add_lo_type(st.getBase(0), LO_TYPE_COMPOUNDOBJECT);

			else if (st.getBase(1) == "inline")
				add_lo_type(st.getBase(0), LO_TYPE_INLINE);

			else
				throw CompilerException("unknown typedefnew:" + st.getBase(1));

			return;
		}
	}

	global_object->addObject(name_t(st.getName()), st);
}



