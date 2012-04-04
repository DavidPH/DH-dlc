//-----------------------------------------------------------------------------
//
// Copyright(C) 2009-2012 David Hill
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
// Program entry point.
//
//-----------------------------------------------------------------------------

#include "global_object.hpp"
#include "math.hpp"
#include "option.hpp"
#include "options.hpp"
#include "process_file.hpp"
#include "process_stream.hpp"
#include "scripts.hpp"
#include "SourceException.hpp"
#include "SourceStream.hpp"
#include "types.hpp"

#include "exceptions/CompilerException.hpp"

#include "types/int_t.hpp"
#include "types/real_t.hpp"

#include "../common/foreach.hpp"
#include "../common/IO.hpp"

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

static inline void _init(int argc, char const *const *argv)
{
   option::help_program = argv[0];

   if (argc == 1)
   {
      option::print_help(stderr);
      throw 0;
   }

   //
   // Process options.
   //
   option::process_options(argc-1, argv+1, option::OPTF_KEEPA);
   if (option_directory.empty() && option::option_args::arg_count)
   {
      option_directory =
         option::option_args::arg_vector[--option::option_args::arg_count];
   }

   if (!option::option_args::arg_count)
   {
      option::print_help(stderr);
      throw 1;
   }

   //
   // Contextual defaults.
   //

   // TODO: option_output_udmf

   //
   // Initialize seed, if necessary.
   //
   #ifdef TARGET_OS_UNIX
   if (!option_seed_handled)
   {
      std::ifstream ifs("/dev/urandom");

      if (ifs)
      {
         for (size_t index = sizeof(option_seed); index; --index)
         {
            option_seed <<= CHAR_BIT;
            option_seed += ifs.get();
         }

         option_seed_handled = true;
      }
   }

   if (!option_seed_handled)
   {
      std::ifstream ifs("/dev/random");

      if (ifs)
      {
         for (size_t index = sizeof(option_seed); index; --index)
         {
            option_seed <<= CHAR_BIT;
            option_seed += ifs.get();
         }

         option_seed_handled = true;
      }
   }
   #endif

   if (!option_seed_handled)
   {
      option_seed = time(NULL);

      option_seed_handled = true;
   }

   srand(option_seed);

   if (option_debug_seed)
      std::cerr << "seed:" << option_seed << ";\n";
}

static inline int _main()
{
   char const *const *argit;
   char const *const *argend =
      option::option_args::arg_vector + option::option_args::arg_count;

   clock_t clockValue;


   if (!option_directory.empty() && option_directory[option_directory.size()-1] != PATH_SEP)
      option_directory += PATH_SEP;

   if (!option_map_name_handled)
   {
      size_t lastSep = option_directory.find_last_of(PATH_SEP, option_directory.size()-2);

      option_map_name = option_directory.substr(lastSep+1, (option_directory.size() - lastSep) - 2);
   }


   clockValue = clock();

   // Load library scripts.
   if (option_lib_std)
      process_file("lib-std.ddl");

   if (option_lib_udmf_strict)
      process_file("lib-udmf-strict.ddl");
   else if (option_lib_udmf)
      process_file("lib-udmf.ddl");

   if (option_lib_usdf_strict)
      process_file("lib-usdf-strict.ddl");
   else if (option_lib_usdf)
      process_file("lib-usdf.ddl");

   // Load user scripts.
   for (argit = option::option_args::arg_vector; argit != argend; ++argit)
   {
      std::string arg = *argit;
      size_t lastSep = arg.find_last_of(PATH_SEP);

      #if 0
      // This ensures that files can always be included from the same directory.
      if (lastSep != std::string::npos)
         option_include.push_back(it->substr(0, lastSep+1));
      #endif

      if (arg == "-")
      {
         SourceStream ss(std::cin, "<stdin>", SourceStream::ST_DDL);
         process_stream<SourceTokenDDL>(ss, "stdin");
      }
      else
         process_file(arg);
   }

   // Log compile time.
   if (option_debug_time)
   {
      clockValue = clock() - clockValue;
      std::cerr << "ctime:" << (clockValue / (double)CLOCKS_PER_SEC) << ";\n";
   }


   // Dump objects.
   if (option_debug_dump)
   {
      std::cerr << "global=";
      global_object->printOn(std::cerr);

      FOREACH_T(global_object_map_t, mapIt, global_object_map)
      {
         FOREACH_T(global_object_list_t, listIt, mapIt->second)
         {
            (*listIt)->printOn(std::cerr);
         }
      }
   }


   if (!option_output_any)
      return 0;

   clockValue = clock();

   IO::mkdir(option_directory, true);

   #define OPENFILE(NAME, EXT) \
   std::string name##NAME(#NAME); \
   if (option_use_file_extensions) \
      name##NAME += EXT; \
   \
   std::ofstream file##NAME( \
      (option_directory + name##NAME).c_str(), \
      std::ios_base::out | std::ios_base::binary); \
   \
   if (!file##NAME) \
   { \
      std::cerr << "unable to open:" << name##NAME << "\n"; \
      exit(1); \
   }

   if (option_output_hexen)
   {
      OPENFILE(LINEDEFS, ".lmp");
      OPENFILE(SECTORS,  ".lmp");
      OPENFILE(SIDEDEFS, ".lmp");
      OPENFILE(THINGS,   ".lmp");
      OPENFILE(VERTEXES, ".lmp");

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_linedef()])
         (*it)->encodeHexen(fileLINEDEFS);

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_sector()])
         (*it)->encodeHexen(fileSECTORS);

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_sidedef()])
         (*it)->encodeHexen(fileSIDEDEFS);

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_thing()])
         (*it)->encodeHexen(fileTHINGS);

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_vertex()])
         (*it)->encodeHexen(fileVERTEXES);

      // ZDoom requires a BEHAVIOR lump to signify a Hexen map.
      // TODO: Make this an option.
      std::string nameBEHAVIOR("BEHAVIOR");
      if (option_use_file_extensions)
         nameBEHAVIOR += ".o";

      // Opened as append so as to not delete existing content, if any.
      std::ofstream fileBEHAVIOR((option_directory + nameBEHAVIOR).c_str(), std::ios_base::app);
   }

   if (option_output_strife)
   {
      OPENFILE(LINEDEFS, ".lmp");
      OPENFILE(SECTORS,  ".lmp");
      OPENFILE(SIDEDEFS, ".lmp");
      OPENFILE(THINGS,   ".lmp");
      OPENFILE(VERTEXES, ".lmp");

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_linedef()])
         (*it)->encodeStrife(fileLINEDEFS);

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_sector()])
         (*it)->encodeStrife(fileSECTORS);

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_sidedef()])
         (*it)->encodeStrife(fileSIDEDEFS);

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_thing()])
         (*it)->encodeStrife(fileTHINGS);

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_vertex()])
         (*it)->encodeStrife(fileVERTEXES);
   }

   if (option_output_heretic)
   {
      OPENFILE(LINEDEFS, ".lmp");
      OPENFILE(SECTORS,  ".lmp");
      OPENFILE(SIDEDEFS, ".lmp");
      OPENFILE(THINGS,   ".lmp");
      OPENFILE(VERTEXES, ".lmp");

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_linedef()])
         (*it)->encodeHeretic(fileLINEDEFS);

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_sector()])
         (*it)->encodeHeretic(fileSECTORS);

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_sidedef()])
         (*it)->encodeHeretic(fileSIDEDEFS);

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_thing()])
         (*it)->encodeHeretic(fileTHINGS);

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_vertex()])
         (*it)->encodeHeretic(fileVERTEXES);
   }

   if (option_output_doom)
   {
      OPENFILE(LINEDEFS, ".lmp");
      OPENFILE(SECTORS,  ".lmp");
      OPENFILE(SIDEDEFS, ".lmp");
      OPENFILE(THINGS,   ".lmp");
      OPENFILE(VERTEXES, ".lmp");

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_linedef()])
         (*it)->encodeDoom(fileLINEDEFS);

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_sector()])
         (*it)->encodeDoom(fileSECTORS);

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_sidedef()])
         (*it)->encodeDoom(fileSIDEDEFS);

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_thing()])
         (*it)->encodeDoom(fileTHINGS);

      FOREACH_T(global_object_list_t, it, global_object_map[type_t::type_vertex()])
         (*it)->encodeDoom(fileVERTEXES);
   }

   if (option_output_udmf)
   {
      OPENFILE(TEXTMAP, ".txt");

      fileTEXTMAP << "/* Compiled by DH-dlc. */\n\n";

      // TODO make a command for this
      if (global_object->hasObject(name_t("namespace")))
      {
         obj_t namespaceObj = global_object->getObject(name_t("namespace"));

         if (namespaceObj->getType() == type_t::type_string())
            fileTEXTMAP << "namespace = "; namespaceObj->encodeUDMF(fileTEXTMAP, 1); fileTEXTMAP << ";\n\n";
      }

      FOREACH_T(global_object_map_t, mapIt, global_object_map)
         FOREACH_T(global_object_list_t, listIt, mapIt->second)
            (*listIt)->encodeUDMF(fileTEXTMAP);
   }

   if (option_output_usdf)
   {
      OPENFILE(DIALOG, ".txt");

      fileDIALOG << "/* Compiled by DH-dlc. */\n\n";

      // TODO make a command for this
      if (global_object->hasObject(name_t("namespace")))
      {
         obj_t namespaceObj = global_object->getObject(name_t("namespace"));

         if (namespaceObj->getType() == type_t::type_string())
            fileDIALOG << "namespace = "; namespaceObj->encodeUSDF(fileDIALOG, 1); fileDIALOG << ";\n\n";
      }

      FOREACH_T(global_object_map_t, mapIt, global_object_map)
         FOREACH_T(global_object_list_t, listIt, mapIt->second)
            (*listIt)->encodeUSDF(fileDIALOG);
   }

   #undef OPENFILE

   if (option_output_extradata)
   {
      std::string nameExtraData(option_script_extradata);

      std::ofstream fileExtraData((option_directory + nameExtraData).c_str());

      if (!fileExtraData)
      {
         std::cerr << "unable to open:" << nameExtraData << '\n';
         return 1;
      }

      FOREACH_T(global_object_map_t, mapIt, global_object_map)
         FOREACH_T(global_object_list_t, listIt, mapIt->second)
            (*listIt)->encodeExtraData(fileExtraData);
   }

   FOREACH_T(scripts_data_type, it, scripts_data)
   {
      std::string nameSCRIPT = it->first;
      if (option_use_file_extensions && nameSCRIPT.find('.') == std::string::npos)
         nameSCRIPT += ".txt";

      std::ofstream fileSCRIPT((option_directory + it->first).c_str());

      if (!fileSCRIPT)
      {
         std::cerr << "unable to write script:\n  " << it->first << '\n';
         continue;
      }

      fileSCRIPT << it->second;
   }

   // Log output time.
   if (option_debug_time)
   {
      clockValue = clock() - clockValue;
      std::cerr << "otime:" << (clockValue / (double)CLOCKS_PER_SEC) << ";\n";
   }

   return 0;
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// main
//
int main(int argc, char** argv)
{
   try
   {
      _init(argc, argv);
      return _main();
   }
   catch (SourceException const &e)
   {
      std::cerr << e.what() << std::endl;
   }
   catch (option::exception const &e)
   {
      std::cerr << "(option::exception): " << e.what() << std::endl;
      option::print_help(stderr);
   }
   catch (std::exception const &e)
   {
      std::cerr << "(std::exception): " << e.what() << std::endl;
   }
   catch (char const *e)
   {
      std::cerr << e << std::endl;
   }
   catch (int e)
   {
      return e;
   }
   catch (...)
   {
      std::cerr << "Unknown exception." << std::endl;
   }

   return EXIT_FAILURE;
}

// EOF

