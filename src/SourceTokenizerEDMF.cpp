//-----------------------------------------------------------------------------
//
// Copyright(C) 2011-2012 David Hill
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------------
//
// EDMF preprocessing.
//
//-----------------------------------------------------------------------------

#include "SourceTokenizerEDMF.hpp"

#include "option.hpp"
#include "SourceException.hpp"
#include "SourceStream.hpp"

#include <cstdlib>
#include <cstring>


//----------------------------------------------------------------------------|
// Static Prototypes                                                          |
//

static int add_define_base
(char const *opt, int optf, int argc, char const *const *argv);


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

static option::option_call option_define_handler
('D', "define", NULL, "Adds a define.", NULL, add_define_base);


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

SourceTokenizerEDMF::DefMap SourceTokenizerEDMF::defines_base;


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// add_define_base
//
static int add_define_base
(char const *opt, int optf, int argc, char const *const *argv)
{
   if (!argc) option::exception::error(opt, optf, "requires argument");

   char const *eq = std::strchr(argv[0], '=');

   if (eq)
   {
      std::string name(argv[0], eq-argv[0]);
      SourceTokenizerEDMF::add_define_base(name, eq);
   }
   else
   {
      SourceTokenizerEDMF::add_define_base(argv[0]);
   }

   return 1;
}

//
// make_expression
//
static SourceTokenizerEDMF::eval_t make_expression
(std::vector<SourceTokenizerEDMF::eval_t> const &values,
 std::vector<SourceTokenC> const &operators, size_t begin, size_t end)
{
   #define EXPRL make_expression(values, operators, begin, iter)
   #define EXPRR make_expression(values, operators, iter+1, end)

   #define CARGS operators[iter].pos

   // Terminating case. Only one expression, so just return it.
   if (begin == end) return values[begin];

   size_t iter;

   // ?:

   // ||
   for (iter = end; iter-- > begin;)
   {
      switch (operators[iter].type)
      {
      case SourceTokenC::TT_OP_PIPE2: return EXPRL || EXPRR;
      default: break;
      }
   }

   // ^^
   for (iter = end; iter-- > begin;)
   {
      switch (operators[iter].type)
      {
      case SourceTokenC::TT_OP_CARET2: return !!EXPRL ^ !!EXPRR;
      default: break;
      }
   }

   // &&
   for (iter = end; iter-- > begin;)
   {
      switch (operators[iter].type)
      {
      case SourceTokenC::TT_OP_AND2: return EXPRL && EXPRR;
      default: break;
      }
   }

   // |
   for (iter = end; iter-- > begin;)
   {
      switch (operators[iter].type)
      {
      case SourceTokenC::TT_OP_PIPE: return EXPRL | EXPRR;
      default: break;
      }
   }

   // ^
   for (iter = end; iter-- > begin;)
   {
      switch (operators[iter].type)
      {
      case SourceTokenC::TT_OP_CARET: return EXPRL ^ EXPRR;
      default: break;
      }
   }

   // &
   for (iter = end; iter-- > begin;)
   {
      switch (operators[iter].type)
      {
      case SourceTokenC::TT_OP_AND: return EXPRL & EXPRR;
      default: break;
      }
   }

   // == !=

   // >= > <= <

   // >> <<

   // - +
   for (iter = end; iter-- > begin;)
   {
      switch (operators[iter].type)
      {
      case SourceTokenC::TT_OP_MINUS: return EXPRL - EXPRR;
      case SourceTokenC::TT_OP_PLUS: return EXPRL + EXPRR;
      default: break;
      }
   }

   // * % /
   for (iter = end; iter-- > begin;)
   {
      switch (operators[iter].type)
      {
      case SourceTokenC::TT_OP_ASTERISK: return EXPRL * EXPRR;
      case SourceTokenC::TT_OP_PERCENT: return EXPRL % EXPRR;
      case SourceTokenC::TT_OP_SLASH: return EXPRL / EXPRR;
      default: break;
      }
   }

   ERROR(operators[begin].pos, "unexpected operator");

   #undef CARGS

   #undef EXPRR
   #undef EXPRL
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// SourceTokenizerEDMF::SourceTokenizerEDMF
//
SourceTokenizerEDMF::SourceTokenizerEDMF(SourceStream *_in)
 : defines(defines_base),
   canCommand(true), canExpand(true), canSkip(true), canString(true)
{
   in.push(_in);
}

//
// SourceTokenizerEDMF::~SourceTokenizerEDMF
//
SourceTokenizerEDMF::~SourceTokenizerEDMF()
{
	// All but the first SourceStream was alloc'd by this.
	while (in.size() > 1)
	{
		delete in.top();
		in.pop();
	}
}

//
// SourceTokenizerEDMF::add_define_base
//
void SourceTokenizerEDMF::add_define_base(std::string const &name)
{
   static DefVec const tokens;
   add_define_base(name, tokens);
}

//
// SourceTokenizerEDMF::add_define_base
//
void SourceTokenizerEDMF::add_define_base
(std::string const &name, DefVec const &tokens)
{
   defines_base[name] = tokens;
}

//
// SourceTokenizerEDMF::add_define_base
//
void SourceTokenizerEDMF::add_define_base
(std::string const &name, std::string const &source)
{
   DefVec tokens;
   SourceStream in(source, SourceStream::ST_C);

   try
   {
      tokens.resize(tokens.size()+1);
      tokens.back().readToken(&in);
   }
   catch (SourceStream::EndOfStream &)
   {
      // Keep reading until end-of-stream.
      // (I sure hope there aren't any incomplete tokens.)
   }

   add_define_base(name, tokens);
}

//
// SourceTokenizerEDMF::addDefine
//
void SourceTokenizerEDMF::addDefine(std::string const &name)
{
   static DefVec const tokens;
   addDefine(name, SourcePosition::builtin(), tokens);
}

//
// SourceTokenizerEDMF::addDefine
//
void SourceTokenizerEDMF::addDefine
(std::string const &name, SourcePosition const &pos,
 std::vector<SourceTokenC> const &tokens)
{
   if (hasDefine(name))
      ERROR_P("attempt to redefine define");

   defines[name] = tokens;
}

//
// SourceTokenizerEDMF::addSkip
//
void SourceTokenizerEDMF::addSkip(bool skip)
{
   skipStack.push_back(skip);
   unskipStack.push(!skip);
}

//
// SourceTokenizerEDMF::doAssert
//
void SourceTokenizerEDMF::doAssert(SourceTokenC::TokenType type)
{
   if (token.type != type)
      ERROR(token.pos, "expected %s got %s", make_string(type).c_str(),
            make_string(token.type).c_str());
}

//
// SourceTokenizerEDMF::doAssert
//
void SourceTokenizerEDMF::doAssert(std::string const &data)
{
   if (token.data != data)
      ERROR(token.pos, "expected %s got %s", data.c_str(), token.data.c_str());
}

//
// SourceTokenizerEDMF::doCommand
//
void SourceTokenizerEDMF::doCommand()
{
   canCommand = false;
   canExpand  = false;
   canSkip    = false;

   prep(); doAssert(SourceTokenC::TT_IDENTIFIER);

   std::string const &command = token.data;

        if (command == "define")  doCommand_define();
   else if (command == "else")    doCommand_else();
   else if (command == "elif")    doCommand_elif();
   else if (command == "endif")   doCommand_endif();
   else if (command == "error")   doCommand_error();
   else if (command == "if")      doCommand_if();
   else if (command == "ifdef")   doCommand_ifdef();
   else if (command == "ifndef")  doCommand_ifndef();
   else if (command == "include") doCommand_include();
   else if (command == "undef")   doCommand_undef();

   else ERROR(token.pos, "unknown command: %s", command.c_str());

   canCommand = true;
   canExpand  = true;
   canSkip    = true;
}

//
// SourceTokenizerEDMF::doCommand_define
//
void SourceTokenizerEDMF::doCommand_define()
{
   prep(); doAssert(SourceTokenC::TT_IDENTIFIER);

   std::string name = token.data;
   SourcePosition position = token.pos;

   std::vector<SourceTokenC> tokens;

   for (prep(); token.type != SourceTokenC::TT_OP_HASH; prep())
      tokens.push_back(token);

   if (isSkip()) return;

   addDefine(name, position, tokens);
}

//
// SourceTokenizerEDMF::doCommand_else
//
void SourceTokenizerEDMF::doCommand_else()
{
   if (skipStack.empty())
      ERROR(token.pos, "unmatched #else");

   skipStack.back() = unskipStack.top();
   unskipStack.top() = true; // If it wasn't, it is now.
}

//
// SourceTokenizerEDMF::doCommand_elif
//
void SourceTokenizerEDMF::doCommand_elif()
{
   if (skipStack.empty())
      ERROR(token.pos, "unmatched #elif");

   bool ifResult = getIf();

   skipStack.back()  = unskipStack.top() || !ifResult;
   unskipStack.top() = unskipStack.top() ||  ifResult;
}

//
// SourceTokenizerEDMF::doCommand_endif
//
void SourceTokenizerEDMF::doCommand_endif()
{
   if (skipStack.empty())
      ERROR(token.pos, "unmatched #endif");

   remSkip();
}

//
// SourceTokenizerEDMF::doCommand_error
//
void SourceTokenizerEDMF::doCommand_error()
{
   prep(); doAssert(SourceTokenC::TT_STRING);

   if (isSkip()) return;

   ERROR(token.pos, "#error %s", token.data.c_str());
}

//
// SourceTokenizerEDMF::doCommand_if
//
void SourceTokenizerEDMF::doCommand_if()
{
   addSkip(!getIf());
}

//
// SourceTokenizerEDMF::doCommand_ifdef
//
void SourceTokenizerEDMF::doCommand_ifdef()
{
   prep(); doAssert(SourceTokenC::TT_IDENTIFIER);

   addSkip(!hasDefine(token.data));
}

//
// SourceTokenizerEDMF::doCommand_ifndef
//
void SourceTokenizerEDMF::doCommand_ifndef()
{
   prep(); doAssert(SourceTokenC::TT_IDENTIFIER);

   addSkip(hasDefine(token.data));
}

//
// SourceTokenizerEDMF::doCommand_include
//
void SourceTokenizerEDMF::doCommand_include()
{
   canString = false;
   prep(); doAssert(SourceTokenC::TT_STRING);
   canString = true;

   if (isSkip()) return;

   try
   {
      in.push(new SourceStream(token.data, SourceStream::ST_C));
   }
   catch (std::exception & e)
   {
      ERROR(token.pos, "file not found: %s", token.data.c_str());
   }
}

//
// SourceTokenizerEDMF::doCommand_undef
//
void SourceTokenizerEDMF::doCommand_undef()
{
   prep(); doAssert(SourceTokenC::TT_IDENTIFIER);

   if (isSkip()) return;

   remDefine();
}

//
// SourceTokenizerEDMF::get
//
SourceTokenC const &SourceTokenizerEDMF::get()
{
   prep();

   return token;
}

//
// SourceTokenizerEDMF::get
//
SourceTokenC const &SourceTokenizerEDMF::get(SourceTokenC::TokenType type)
{
   prep(); doAssert(type);

   return token;
}

//
// SourceTokenizerEDMF::get
//
SourceTokenC const &SourceTokenizerEDMF::get
(SourceTokenC::TokenType type, std::string const &data)
{
   prep(); doAssert(type); doAssert(data);

   return token;
}

//
// SourceTokenizerEDMF::getIf
//
bool SourceTokenizerEDMF::getIf()
{
   canExpand = true;

   eval_t value = getIfMultiple();

   canExpand = false;

   return value;
}

//
// SourceTokenizerEDMF::getIfMultiple
//
SourceTokenizerEDMF::eval_t SourceTokenizerEDMF::getIfMultiple()
{
   std::vector<eval_t> values;
   std::vector<SourceTokenC> operators;

   values.push_back(getIfSingle());

   for (prep(); token.type != SourceTokenC::TT_OP_HASH; prep())
   {
      switch (token.type)
      {
      case SourceTokenC::TT_OP_AND:
      case SourceTokenC::TT_OP_AND2:
      case SourceTokenC::TT_OP_ASTERISK:
      case SourceTokenC::TT_OP_CARET:
      case SourceTokenC::TT_OP_CARET2:
      case SourceTokenC::TT_OP_CMP_EQ:
      case SourceTokenC::TT_OP_CMP_GE:
      case SourceTokenC::TT_OP_CMP_GT:
      case SourceTokenC::TT_OP_CMP_GT2:
      case SourceTokenC::TT_OP_CMP_LE:
      case SourceTokenC::TT_OP_CMP_LT:
      case SourceTokenC::TT_OP_CMP_LT2:
      case SourceTokenC::TT_OP_CMP_NE:
      case SourceTokenC::TT_OP_MINUS:
      case SourceTokenC::TT_OP_PERCENT:
      case SourceTokenC::TT_OP_PIPE:
      case SourceTokenC::TT_OP_PIPE2:
      case SourceTokenC::TT_OP_PLUS:
      case SourceTokenC::TT_OP_SLASH:
      case_expr:
         operators.push_back(token);
         values.push_back(getIfSingle());
         break;

      case SourceTokenC::TT_OP_COLON:
      case SourceTokenC::TT_OP_PARENTHESIS_C:
         goto done;

      case SourceTokenC::TT_OP_QUERY:
         operators.push_back(token);
         values.push_back(getIfMultiple());
         prep(); doAssert(SourceTokenC::TT_OP_COLON);
         goto case_expr;

      default:
         ERROR(token.pos, "unexpected token type");
      }
   }

done:
   return make_expression(values, operators, 0, operators.size());
}

//
// SourceTokenizerEDMF::getIfSingle
//
SourceTokenizerEDMF::eval_t SourceTokenizerEDMF::getIfSingle()
{
   prep();

   switch (token.type)
   {
   case SourceTokenC::TT_IDENTIFIER:
      if (token.data == "defined")
      {
         canExpand = false;

         prep();

         bool hasParentheses = token.type == SourceTokenC::TT_OP_PARENTHESIS_O;

         if (hasParentheses) prep();

         doAssert(SourceTokenC::TT_IDENTIFIER);

         eval_t value = hasDefine(token.data);

         if (hasParentheses)
            {prep(); doAssert(SourceTokenC::TT_OP_PARENTHESIS_C);}

         canExpand = true;

         return value;
      }
      else
      {
         // If this function sees any other identifier, that means it's not a
         // define. C says that undefined identifiers evaluate to 0.
         return 0;
      }

   case SourceTokenC::TT_INTEGER:
      return strtoll(token.data.c_str(), NULL, 10);

   case SourceTokenC::TT_OP_EXCLAMATION:
      return !getIfSingle();

   case SourceTokenC::TT_OP_PARENTHESIS_O:
   {
      eval_t value = getIfMultiple();

      doAssert(SourceTokenC::TT_OP_PARENTHESIS_C);

      return value;
   }

   default:
      ERROR(token.pos, "unexpected token type");
   }
}

//
// SourceTokenizerEDMF::hasDefine
//
bool SourceTokenizerEDMF::hasDefine()
{
   if (token.type != SourceTokenC::TT_IDENTIFIER)
      return false;

   return hasDefine(token.data);
}

//
// SourceTokenizerEDMF::hasDefine
//
bool SourceTokenizerEDMF::hasDefine(std::string const &name)
{
   return defines.find(name) != defines.end();
}

//
// SourceTokenizerEDMF::isSkip
//
bool SourceTokenizerEDMF::isSkip()
{
   std::vector<bool>::iterator it;

   for (it = skipStack.begin(); it != skipStack.end(); ++it)
      if (*it) return true;

   return false;
}

//
// SourceTokenizerEDMF::peek
//
SourceTokenC const &SourceTokenizerEDMF::peek()
{
   prep();

   ungetStack.push(token);

   return token;
}

//
// SourceTokenizerEDMF::peek
//
SourceTokenC const &SourceTokenizerEDMF::peek(SourceTokenC::TokenType type)
{
   prep(); doAssert(type);

   ungetStack.push(token);

   return token;
}

//
// SourceTokenizerEDMF::peekType
//
bool SourceTokenizerEDMF::peekType(SourceTokenC::TokenType type)
{
   prep();

   ungetStack.push(token);

   return token.type == type;
}

//
// SourceTokenizerEDMF::peekType
//
bool SourceTokenizerEDMF::peekType
(SourceTokenC::TokenType type, std::string const &data)
{
   prep();

   ungetStack.push(token);

   return token.type == type && token.data == data;
}

//
// SourceTokenizerEDMF::prep
//
void SourceTokenizerEDMF::prep()
{
   while (true)
   {
      if (!ungetStack.empty())
      {
         token = ungetStack.top();
         ungetStack.pop();
      }
      else try
      {
         token.readToken(in.top());
      }
      catch (SourceStream::EndOfStream &e)
      {
         if (in.size() == 1) throw;

         delete in.top();
         in.pop();

         continue;
      }

      // Preprocessor directive.
      if (canCommand && token.type == SourceTokenC::TT_OP_HASH)
      {
         doCommand();
         continue;
      }

      if (canExpand && hasDefine())
      {
         prepDefine();
         continue;
      }

      if (canSkip && isSkip())
         continue;

      break;
   }

   // String literal concatenation.
   if (canString && token.type == SourceTokenC::TT_STRING)
   {
      SourceTokenC tokenTemp = token;

      prep();

      if (token.type == SourceTokenC::TT_STRING)
         tokenTemp.data += token.data;
      else
         unget(token);

      token = tokenTemp;
   }
}

//
// SourceTokenizerEDMF::prepDefine
//
void SourceTokenizerEDMF::prepDefine()
{
   std::vector<SourceTokenC> const &tokens(defines[token.data]);

   for (size_t i(tokens.size()); i--;)
      ungetStack.push(tokens[i]);
}

//
// SourceTokenizerEDMF::remDefine
//
void SourceTokenizerEDMF::remDefine()
{
   defines.erase(token.data);
}

//
// SourceTokenizerEDMF::remSkip
//
void SourceTokenizerEDMF::remSkip()
{
   skipStack.pop_back();
   unskipStack.pop();
}

//
// SourceTokenizerEDMF::unget
//
void SourceTokenizerEDMF::unget(SourceTokenC const &_token)
{
   ungetStack.push(_token);
}

// EOF

