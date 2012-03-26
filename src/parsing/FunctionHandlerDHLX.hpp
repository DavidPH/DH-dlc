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
	Handles user-defined DHLX functions.
*/

#ifndef HPP_FunctionHandlerDHLX__parsing_
#define HPP_FunctionHandlerDHLX__parsing_

#include "FunctionHandler.hpp"

#include "../types.hpp"

#include "../LevelObject/LevelObjectType.hpp"



template<typename T>
class FunctionHandlerDHLX : public FunctionHandler<T>
{
	public:
		explicit FunctionHandlerDHLX();
		explicit FunctionHandlerDHLX(SourceScannerDHLX & sc);
		explicit FunctionHandlerDHLX(SourceScannerDHLX & sc, std::vector<type_t> const & argt);
		virtual ~FunctionHandlerDHLX();

		virtual T operator () (SourceScannerDHLX & sc) const;
		virtual T operator () (std::vector<std::string> const & args) const;

	private:
		std::vector<type_t> _argt;
		SourceScannerDHLX _data;
};



#endif /* HPP_FunctionHandlerDHLX__parsing_ */



