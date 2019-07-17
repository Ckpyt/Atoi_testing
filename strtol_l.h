/* Convert string representing a number to integer value, using given locale.
   Copyright (C) 1997-2019 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1997.

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

#ifndef __STRTOL__
#define __STRTOL__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef _LIBC
# define USE_NUMBER_GROUPING
# define HAVE_LIMITS_H
#endif

#include <cctype>
#include <ctype.h>
#include <errno.h>
#include <locale>
#ifndef __set_errno
# define __set_errno(Val) errno = (Val)
#endif

#ifdef HAVE_LIMITS_H
# include <limits.h>
#endif

#include <stddef.h>
#include <string.h>
#include <locale.h>
#include <ctime>
//#include <stdint.h>
//#include <bits/wordsize.h>

#define size_t unsigned int


#ifdef USE_NUMBER_GROUPING
# include "../locale/localeinfo.h"
#endif

/* Nonzero if we are defining `strtoul' or `strtoull', operating on
   unsigned integers.  */
#ifndef UNSIGNED
# define UNSIGNED 0
# define INT LONG int
#else
# define INT unsigned LONG int
#endif


/* If QUAD is defined, we are defining `strtoll' or `strtoull',
   operating on `long long int's.  */
#ifdef QUAD
# define LONG long long
# define STRTOL_LONG_MIN LONG_LONG_MIN
# define STRTOL_LONG_MAX LONG_LONG_MAX
# define STRTOL_ULONG_MAX ULONG_LONG_MAX
#else
# define LONG long

# ifndef ULONG_MAX
#  define ULONG_MAX ((unsigned long int) ~(unsigned long int) 0)
# endif
# ifndef LONG_MAX
#  define LONG_MAX ((long int) (ULONG_MAX >> 1))
# endif
# define STRTOL_LONG_MIN LONG_MIN
# define STRTOL_LONG_MAX LONG_MAX
# define STRTOL_ULONG_MAX ULONG_MAX
#endif


/* We use this code for the extended locale handling where the
   function gets as an additional argument the locale which has to be
   used.  To access the values we have to redefine the _NL_CURRENT and
   _NL_CURRENT_WORD macros.  */
#undef _NL_CURRENT
#define _NL_CURRENT(category, item) \
  (current->values[_NL_ITEM_INDEX (item)].string)
#undef _NL_CURRENT_WORD
#define _NL_CURRENT_WORD(category, item) \
  ((uint32_t) current->values[_NL_ITEM_INDEX (item)].word)

#if defined _LIBC || defined HAVE_WCHAR_H
# include <wchar.h>
#endif

#ifdef USE_WIDE_CHAR
# include <wctype.h>
# define L_(Ch) L##Ch
# define UCHAR_TYPE wint_t
# define STRING_TYPE wchar_t
# define ISSPACE(Ch) __iswspace_l ((Ch), loc)
# define ISALPHA(Ch) __iswalpha_l ((Ch), _nl_C_locobj_ptr)
# define TOUPPER(Ch) __towupper_l ((Ch), _nl_C_locobj_ptr)
#else
# if defined _LIBC \
   || defined STDC_HEADERS || (!defined isascii && !defined HAVE_ISASCII)
#  define IN_CTYPE_DOMAIN(c) 1
# else
#  define IN_CTYPE_DOMAIN(c) isascii(c)
# endif
# define L_(Ch) Ch
# define UCHAR_TYPE unsigned char
# define STRING_TYPE char
# define ISSPACE(Ch) (Ch==' ') 
# define ISALPHA(Ch) isalpha ((Ch))
# define TOUPPER(Ch) toupper ((Ch))
#endif

#define INTERNAL(X) INTERNAL1(X)
#define INTERNAL1(X) __##X##_internal
#define WEAKNAME(X) WEAKNAME1(X)

#ifdef USE_NUMBER_GROUPING
/* This file defines a function to check for correct grouping.  */
# include "grouping.h"
#endif

#define locale_t std::locale
#define _NL_CURRENT_LOCALE std::locale::classic()

#define LC_GLOBAL_LOCALE ((locale_t)-1)
#define LC_ALL_MASK      LC_ALL
#define LC_COLLATE_MASK  LC_COLLATE
#define LC_CTYPE_MASK    LC_CTYPE
#define LC_MONETARY_MASK LC_MONETARY
#define LC_NUMERIC_MASK  LC_NUMERIC
#define LC_TIME_MASK     LC_TIME

/* Define tables of maximum values and remainders in order to detect
   overflow.  Do this at compile-time in order to avoid the runtime
   overhead of the division.  */

#if defined(QUAD) && __WORDSIZE == 32
extern const unsigned long long __strtol_ull_max_tab[] attribute_hidden;
extern const unsigned char __strtol_ull_rem_tab[] attribute_hidden;
#endif

#define DEF(TYPE, NAME)							   \
  const TYPE NAME[]  =					   \
  {									   \
    F(2), F(3), F(4), F(5), F(6), F(7), F(8), F(9), F(10), 		   \
    F(11), F(12), F(13), F(14), F(15), F(16), F(17), F(18), F(19), F(20),  \
    F(21), F(22), F(23), F(24), F(25), F(26), F(27), F(28), F(29), F(30),  \
    F(31), F(32), F(33), F(34), F(35), F(36)				   \
  }

#if !UNSIGNED && !defined (USE_WIDE_CHAR) && !defined (QUAD)
#define F(X)	ULONG_MAX / X
  DEF (unsigned long, __strtol_ul_max_tab);
#undef F
#define F(X)	ULONG_MAX % X
  DEF (unsigned char, __strtol_ul_rem_tab);
#undef F
#endif
#if !UNSIGNED && !defined (USE_WIDE_CHAR) && defined (QUAD) \
    && __WORDSIZE == 32
# define F(X)	ULONG_LONG_MAX / X
  DEF (unsigned long long, __strtol_ull_max_tab);
# undef F
# define F(X)	ULONG_LONG_MAX % X
  DEF (unsigned char, __strtol_ull_rem_tab);
# undef F
#endif
#undef DEF

/* Define some more readable aliases for these arrays which correspond
   to how they'll be used in the function below.  */
#define jmax_tab	__strtol_ul_max_tab
#define cutoff_tab	__strtol_ul_max_tab
#define cutlim_tab	__strtol_ul_rem_tab



/* Convert NPTR to an `unsigned long int' or `long int' in base BASE.
   If BASE is 0 the base is determined by the presence of a leading
   zero, indicating octal or a leading "0x" or "0X", indicating hexadecimal.
   If BASE is < 2 or > 36, it is reset to 10.
   If ENDPTR is not NULL, a pointer to the character after the last
   one converted is stored in *ENDPTR.  */

  long st__strtol_l(const STRING_TYPE* nptr, STRING_TYPE** endptr,
	  int base, locale_t loc);

//  long st__strtol_l_opt(const STRING_TYPE* nptr, STRING_TYPE** endptr,
//	  int base, locale_t loc);

  long st__strtol_l(const STRING_TYPE* nptr, STRING_TYPE** endptr,
		  int base, int group, locale_t loc);
#endif
