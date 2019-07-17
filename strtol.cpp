#include "strtol.h"

long
st_strtol(const STRING_TYPE* nptr, STRING_TYPE** endptr,
	int base, int group)
{
	return st__strtol_l(nptr, endptr, base, group, _NL_CURRENT_LOCALE);
}

long
st_strtol(const STRING_TYPE* nptr, STRING_TYPE** endptr, int base)
{
	return st__strtol_l(nptr, endptr, base, 0, _NL_CURRENT_LOCALE);
}