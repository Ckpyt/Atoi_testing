/* Convert string representation of a number into an integer value.
   Copyright (C) 1991-2019 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef __STRTOL_H_
#define __STRTOL_H_
#include <stdlib.h>
#include <wchar.h>
#include "strtol_l.h"
//#include <locale/localeinfo.h>




# define UNSIGNED 0
# define INT LONG int

# define LONG long

# define STRING_TYPE char



#define INTERNAL(X) INTERNAL1(X)
#define INTERNAL1(X) __##X##_internal

#define SYM__(X) SYM__1 (X)
#define SYM__1(X) __ ## X
#define __strtol SYM__ (strtol)


long st__strtol_l (const STRING_TYPE *, STRING_TYPE **, int,
				  int, locale_t);


long
st_strtol(const STRING_TYPE* nptr, STRING_TYPE** endptr,
	int base, int group);



long
st_strtol(const STRING_TYPE* nptr, STRING_TYPE** endptr, int base = 10);


#endif