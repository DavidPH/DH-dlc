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

#include "options.hpp"
#include "process_token.hpp"
#include "SourceToken.hpp"

#include <cstdlib>
#include <iostream>



template <class TokenClass>
void process_stream(SourceStream & ss, std::string const & filename)
{
	SourceScanner<TokenClass, SourceStream> sc(ss);
	TokenClass st;

	while (ss)
	{
		try
		{
			st = sc.get();
			process_token(st, sc);
		}
		catch (CompilerException & e)
		{
			PRINT_AND_COUNT_ERROR(filename << ':' << ss.getLineCount() << ':' << e << "\n  ->" << st << '\n');
		}
	}
}

template void process_stream<SourceTokenDDL>(SourceStream &, std::string const &);
template void process_stream<SourceTokenDHLX>(SourceStream &, std::string const &);



