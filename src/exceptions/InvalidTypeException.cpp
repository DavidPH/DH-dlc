/*
    Copyright 2009 David Hill

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

#include <string>

#include "InvalidTypeException.hpp"



InvalidTypeException::InvalidTypeException() throw()
{
	whatString = "";
}
InvalidTypeException::InvalidTypeException(const char* whatData) throw()
{
	whatString = whatData;
}
InvalidTypeException::InvalidTypeException(const std::string& whatData) throw()
{
	whatString = whatData;
}

const char* InvalidTypeException::whatClass() const throw()
{
	return "InvalidTypeException";
}



