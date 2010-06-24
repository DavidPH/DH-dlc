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
	03/02/2010 - Original version.
*/

#include "LevelObjectName.hpp"

#include "parsing.hpp"
#include "../common/foreach.hpp"



LevelObjectName::LevelObjectName() {}
LevelObjectName::LevelObjectName(LevelObjectName const & other, size_t count)
{
	for (size_t index = 0; index < other._name.size() && index < count; ++index)
		this->_name.push_back(other._name[index]);
}
LevelObjectName::LevelObjectName(char const * name)
{
	this->_name.push_back(name);
}
LevelObjectName::LevelObjectName(std::string const & raw_name)
{
	//std::cerr << "LevelObject::LevelObject('" << raw_name << "');\n";

	this->_name.push_back("");

	int bracketCount = 0;
	for (size_t index = 0; index < raw_name.size(); ++index)
	{
		char indexChar = raw_name[index];

		     if (indexChar == '[' || indexChar == '<') ++bracketCount;
		else if (indexChar == ']' || indexChar == '>') --bracketCount;

		if (indexChar == '.' && bracketCount == 0)
		{
			this->_name.push_back("");
		}
		else
		{
			this->_name.rbegin()->push_back(indexChar);
		}
	}

	FOREACH_T(std::vector<std::string>, it, this->_name)
	{
		//std::cerr << *it << '\n';
		*it = parse_name(*it);
		//std::cerr << *it << '\n';
	}
}

bool LevelObjectName::empty() const
{
	return this->_name.size() == 1 && this->_name[0].empty();
}

/*
	Privacy is determined by the name beginning with two underscores. Added
	along with the below function, but not sure what I'll do with this one
	yet.
*/
bool LevelObjectName::isPrivate() const
{
	if (this->_name.size() != 1) return false;

	return (this->_name[0].size() > 1 && this->_name[0][0] == '_' && this->_name[0][1] == '_');
}

/*
	Returns true if this name is volatile. Volatility is determined by the
	name, not a property of the LevelObject. Currently, it means starting
	with a single underscore.

	The biggest use of volatility is automatic deletion of keys.
*/
bool LevelObjectName::isVolatile() const
{
	if (this->_name.size() != 1) return false;

	return (this->_name[0].size() > 1 && this->_name[0][0] == '_' && this->_name[0][1] != '_');
}

LevelObjectName LevelObjectName::getFirst() const
{
	return LevelObjectName(*this, 1);
}
LevelObjectName LevelObjectName::getRest() const
{
	LevelObjectName newName;

	for (size_t index = 1; index < this->_name.size(); ++index)
		newName._name.push_back(this->_name[index]);

	return newName;
}

std::string LevelObjectName::getString(size_t index) const
{
	return this->_name[index];
}

size_t LevelObjectName::size() const
{
	return this->_name.size();
}

bool LevelObjectName::operator < (LevelObjectName const & other) const
{
	// TODO This could cause problems if a multi-part name was used for a
	// key. On the other hand, that should never happen, so maybe throw an
	// exception?
	return this->_name[0] < other._name[0];
}



std::ostream & operator << (std::ostream & out, LevelObjectName const & in)
{
	out << in._name[0];

	for (size_t index = 1; index < in._name.size(); ++index)
		out << '.' << in._name[index];

	return out;
}



