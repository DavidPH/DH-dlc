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
	Handles user-defined DDL functions.
*/

#ifndef HPP_FunctionHandlerDDL__PARSING_
#define HPP_FunctionHandlerDDL__PARSING_

#include "FunctionHandler.hpp"

#include "../types.hpp"

#include "../LevelObject/LevelObjectType.hpp"



template<typename T>
class FunctionHandlerDDL : public FunctionHandler<T>
{
	public:
		explicit FunctionHandlerDDL();
		explicit FunctionHandlerDDL(std::string const & data);
		explicit FunctionHandlerDDL(std::string const & data, std::vector<type_t> const & argt);
		virtual ~FunctionHandlerDDL();

		virtual T operator () (SourceScannerDHLX & sc) const;
		virtual T operator () (std::vector<std::string> const & args) const;

	private:
		std::vector<type_t> _argt;
		std::string _data;
};



#endif /* HPP_FunctionHandlerDDL__PARSING_ */



