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

#include "SourceScanner.hpp"

#include "types.hpp"

#include "exceptions/SyntaxException.hpp"



template <typename TT, typename SS>
SourceScanner<TT, SS>::SourceScanner() : _in(NULL), _ungetStack()
{

}
template <typename TT, typename SS>
SourceScanner<TT, SS>::SourceScanner(SourceScanner<TT, SS> const & sc) : _in(NULL), _ungetStack(sc._ungetStack)
{

}
template <typename TT, typename SS>
SourceScanner<TT, SS>::SourceScanner(SS & in) : _in(&in), _ungetStack()
{

}

template <typename TT, typename SS>
TT SourceScanner<TT, SS>::get()
{
	TT token;

	if (_ungetStack.empty())
	{
		if (_in)
			(*_in) >> token;
		else
			token = TT::EOF_token;
	}
	else
	{
		token = _ungetStack.top();

		_ungetStack.pop();
	}

	return token;
}
template <typename TT, typename SS>
TT SourceScanner<TT, SS>::get(typename TT::TokenType typeMust)
{
	TT token = get();

	if (token.getType() != typeMust)
		throw SyntaxException("expected " + make_string(typeMust) + " got " + make_string(token.getType()));

	return token;
}
template <typename TT, typename SS>
TT SourceScanner<TT, SS>::get(typename TT::TokenType typeMust, typename TT::TokenType typeSkip)
{
	TT token = get();

	if (token.getType() == typeSkip)
		token = get();

	if (token.getType() != typeMust)
		throw SyntaxException("expected " + make_string(typeMust) + " got " + make_string(token.getType()));

	return token;
}

template <typename TT, typename SS>
SourceScanner<TT, SS> SourceScanner<TT, SS>::getblock(typename TT::TokenType typeOpen, typename TT::TokenType typeClose)
{
	SourceScanner<TT, SS> sc;
	std::stack<TT> tempStack;

	int depth = 0;

	while (true)
	{
		TT token(get());

		typename TT::TokenType type(token.getType());

		if (type == typeOpen) ++depth;
		else if (type == typeClose) --depth;

		tempStack.push(token);

		if (!depth) break;

		if (!*_in) break;
	}

	// Need to reverse the stack.
	while (tempStack.size())
	{
		sc._ungetStack.push(tempStack.top());
		tempStack.pop();
	}

	return sc;
}

template <typename TT, typename SS>
void SourceScanner<TT, SS>::unget(TT token)
{
	_ungetStack.push(token);
}



template class SourceScanner<SourceTokenDHLX, SourceStream>;
template class SourceScanner<SourceTokenDDL,  SourceStream>;



