/*
    Copyright 2009, 2010, 2011 David Hill

    This file is part of DH-wad.

    DH-wad is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DH-wad is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DH-wad.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
	2010/02/03 - Update for new process_options.h.
	2010/06/24 - Added option_explicit.
*/



#ifndef OPTIONS_H
#define OPTIONS_H

#include "../common/process_options.h"



PROCESS_OPTION_EXTERN_bool(explicit);
PROCESS_OPTION_EXTERN_bool(iwad);
PROCESS_OPTION_EXTERN_bool(trans_bslash);
PROCESS_OPTION_EXTERN_bool(unwad);

PROCESS_OPTION_EXTERN_string(output);

PROCESS_OPTION_EXTERN_string_multi(directory);
PROCESS_OPTION_EXTERN_string_multi(map);
PROCESS_OPTION_EXTERN_string_multi(wad);



PROCESS_OPTION_LONG_EXTERN;
PROCESS_OPTION_SHORT_EXTERN;

PROCESS_OPTION_ARG_EXTERN;

PROCESS_OPTION_EXTERN;



#endif /* OPTIONS_H */



