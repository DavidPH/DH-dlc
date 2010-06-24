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

*/

#include "process_stream.hpp"

#include <cstdlib>
#include <iostream>

#include "options.hpp"
#include "process_token.hpp"
#include "SourceToken.hpp"



void process_stream(SourceStream& ss, std::string const & filename)
{
	SourceToken st;

	while (ss)
	{
		try
		{
			st.clear();
			ss >> st;
			process_token(st);
		}
		catch (CompilerException& e)
		{
			PRINT_AND_COUNT_ERROR(filename << ':' << ss.getLineCount() << ':' << e << "\n  ->" << st << '\n');
		}
	}
}



