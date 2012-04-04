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

#ifndef HPP_SourceTokenizerEDMF_
#define HPP_SourceTokenizerEDMF_

#include "SourceTokenC.hpp"

#include <map>
#include <stack>
#include <string>
#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

class SourceStream;

//
// SourceTokenizerEDMF
//
class SourceTokenizerEDMF
{
public:
   typedef std::vector<SourceTokenC> DefVec;
   typedef std::map<std::string, DefVec> DefMap;
   typedef int eval_t;


   explicit SourceTokenizerEDMF(SourceStream *in);
   ~SourceTokenizerEDMF();

   SourceTokenC const &get();
   SourceTokenC const &get(SourceTokenC::TokenType type);
   SourceTokenC const &get(SourceTokenC::TokenType type, std::string const &data);

   SourceTokenC const &peek();
   SourceTokenC const &peek(SourceTokenC::TokenType type);

   bool peekType(SourceTokenC::TokenType type);
   bool peekType(SourceTokenC::TokenType type, std::string const &data);

   void unget(SourceTokenC const & token);


   static void add_define_base(std::string const &name);
   static void add_define_base(std::string const &name, DefVec const &tokens);
   static void add_define_base(std::string const &name, std::string const &source);

private:
   SourceTokenizerEDMF(SourceTokenizerEDMF const &tokenizer)/* = delete*/;

   void addDefine(std::string const &name);
   void addDefine
   (std::string const &name, SourcePosition const &position,
    std::vector<SourceTokenC> const &tokens);

   void addSkip(bool skip);

   void doAssert(SourceTokenC::TokenType type);
   void doAssert(std::string const &data);

   void doCommand();
   void doCommand_define();
   void doCommand_else();
   void doCommand_elif();
   void doCommand_endif();
   void doCommand_error();
   void doCommand_if();
   void doCommand_ifdef();
   void doCommand_ifndef();
   void doCommand_include();
   void doCommand_undef();

   bool getIf();
   eval_t getIfMultiple();
   eval_t getIfSingle();

   bool hasDefine();
   bool hasDefine(std::string const &name);

   bool isSkip();

   SourceTokenizerEDMF &operator = (SourceTokenizerEDMF const &tokenizer)/* = delete*/;

   void prep();
   void prepDefine();

   void remDefine();
   void remSkip();

   DefMap defines;

   std::stack<SourceStream *> in;
   std::vector<bool> skipStack;
   std::stack<SourceTokenC> ungetStack;
   std::stack<bool> unskipStack;

   SourceTokenC token;

   bool canCommand : 1;
   bool canExpand  : 1;
   bool canSkip    : 1;
   bool canString  : 1;


   static DefMap defines_base;
};

#endif//HPP_SourceTokenizerEDMF_
