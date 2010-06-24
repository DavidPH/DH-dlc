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
	2010/05/04 - Original version.
*/

#ifndef STRING_T_H
#define STRING_T_H

#include <string>



class string_t
{
	public:
		explicit string_t();
		         string_t(string_t const &);
		explicit string_t(char const *);
		explicit string_t(std::string const &);
		~string_t();

		void clear();

		bool empty() const;

		std::string const & makeString() const;

		char       & operator [] (size_t index);
		char const & operator [] (size_t index) const;

		string_t & operator += (string_t const &);

		size_t size() const;

	private:
		std::string _data;
};



int cmp(string_t const &, string_t const &);

bool operator == (string_t const &, string_t const &);
bool operator != (string_t const &, string_t const &);
bool operator >= (string_t const &, string_t const &);
bool operator >  (string_t const &, string_t const &);
bool operator <= (string_t const &, string_t const &);
bool operator <  (string_t const &, string_t const &);

string_t operator + (string_t const &, string_t const &);



inline bool operator == (string_t const & l, string_t const & r) {return cmp(l, r) == 0;}
inline bool operator != (string_t const & l, string_t const & r) {return cmp(l, r) != 0;}
inline bool operator >= (string_t const & l, string_t const & r) {return cmp(l, r) >= 0;}
inline bool operator >  (string_t const & l, string_t const & r) {return cmp(l, r) >  0;}
inline bool operator <= (string_t const & l, string_t const & r) {return cmp(l, r) <= 0;}
inline bool operator <  (string_t const & l, string_t const & r) {return cmp(l, r) <  0;}



#endif /* STRING_T_H */



