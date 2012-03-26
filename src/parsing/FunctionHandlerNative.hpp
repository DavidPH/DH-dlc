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

#ifndef HPP_FunctionHandlerNative__PARSING_
#define HPP_FunctionHandlerNative__PARSING_

#include "FunctionHandler.hpp"

#include "../SourceScanner.hpp"

#include <string>
#include <vector>



template<typename T>
class FunctionHandlerNative : public FunctionHandler<T>
{
	public:
		typedef T(*funcDDL_t)(std::vector<std::string> const &);
		typedef T(*funcDHLX_t)(SourceScannerDHLX &);

		explicit FunctionHandlerNative();
		explicit FunctionHandlerNative(funcDDL_t);
		explicit FunctionHandlerNative(funcDHLX_t);
		explicit FunctionHandlerNative(funcDDL_t, funcDHLX_t);

		virtual T operator () (SourceScannerDHLX & sc) const;
		virtual T operator () (std::vector<std::string> const & args) const;

	private:
		funcDDL_t  _funcDDL;
		funcDHLX_t _funcDHLX;
};



#endif /* HPP_FunctionHandlerNative__PARSING_ */



