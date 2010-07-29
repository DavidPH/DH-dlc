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
	Extracts and manages tokens from a SourceStream.
*/

#ifndef SOURCESCANNER_H
#define SOURCESCANNER_H

#include "SourceStream.hpp"
#include "SourceToken.hpp"

#include <stack>



template <typename TT, typename SS>
class SourceScanner
{
	public:
		explicit SourceScanner(SS & in);

		TT get();
		TT get(typename TT::TokenType typeMust);
		TT get(typename TT::TokenType typeMust, typename TT::TokenType typeSkip);
		void unget(TT token);

	private:
		SS & _in;

		std::stack<TT> _ungetStack;
};

typedef SourceScanner<SourceToken,     SourceStream> SourceScannerDDL;
typedef SourceScanner<SourceTokenDHLX, SourceStream> SourceScannerDHLX;



#endif /* SOURCESCANNER_H */



