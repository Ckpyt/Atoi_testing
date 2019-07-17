#include "strtol_l.h"

extern const unsigned long __strtol_ul_max_tab[];
extern const unsigned char __strtol_ul_rem_tab[];

long st__strtol_l(const STRING_TYPE* nptr, STRING_TYPE** endptr,
	int base, int group, locale_t loc)
{

	int negative;
	unsigned LONG int cutoff;
	unsigned int cutlim;
	unsigned LONG int i;
	const STRING_TYPE* s;
	UCHAR_TYPE c;
	const STRING_TYPE* save, * end;
	int overflow;

	if (base < 0 || base == 1 || base > 36)
	{
		__set_errno(EINVAL);
		return 0;
	}

	save = s = nptr;

	// Skip white space.  
	while (*s == ' ')
		++s;
	if ((*s == '\0'))
		goto noconv;

	/* Check for a sign.  */
	negative = 0;
	if (*s == L_('-'))
	{
		negative = 1;
		++s;
	}
	else if (*s == L_('+'))
		++s;

	/* Recognize number prefix and if BASE is zero, figure it out ourselves.  */
	if (*s == L_('0'))
	{
		if ((base == 0 || base == 16) && TOUPPER(s[1]) == L_('X'))
		{
			s += 2;
			base = 16;
		}
		else if (base == 0)
			base = 8;
	}
	else if (base == 0)
		base = 10;

	/* Save the pointer so we can check later if anything happened.  */
	save = s;

	end = NULL;

	/* Avoid runtime division; lookup cutoff and limit.  */
	cutoff = cutoff_tab[base - 2];
	cutlim = cutlim_tab[base - 2];

	overflow = 0;
	i = 0;
	c = *s;
	if (sizeof(long int) != sizeof(LONG int))
	{
		unsigned long int j = 0;
		unsigned long int jmax = jmax_tab[base - 2];

		for (; c != L_('\0'); c = *++s)
		{
			if (s == end)
				break;
			if (c >= L_('0') && c <= L_('9'))
				c -= L_('0');
			else if (ISALPHA(c))
				c = TOUPPER(c) - L_('A') + 10;
			else
				break;
			if ((int)c >= base)
				break;
			/* Note that we never can have an overflow.  */
			else if (j >= jmax)
			{
				/* We have an overflow.  Now use the long representation.  */
				i = (unsigned LONG int) j;
				goto use_long;
			}
			else
				j = j * (unsigned long int) base + c;
		}

		i = (unsigned LONG int) j;
	}
	else
		for (; c != L_('\0'); c = *++s)
		{
			if (s == end)
				break;
			if (c >= L_('0') && c <= L_('9'))
				c -= L_('0');
			else if (ISALPHA(c))
				c = TOUPPER(c) - L_('A') + 10;
			else
				break;
			if ((int)c >= base)
				break;
			/* Check for overflow.  */
			if (i > cutoff || (i == cutoff && c > cutlim))
				overflow = 1;
			else
			{
			use_long:
				i *= (unsigned LONG int) base;
				i += c;
			}
		}

	/* Check if anything actually happened.  */
	if (s == save)
		goto noconv;

	/* Store in ENDPTR the address of one character
	   past the last character we converted.  */
	if (endptr != NULL)
		* endptr = (STRING_TYPE*)s;

#if !UNSIGNED
	/* Check for a value that is within the range of
	   `unsigned LONG int', but outside the range of `LONG int'.  */
	if (overflow == 0
		&& i > (negative
			? -((unsigned LONG int) (STRTOL_LONG_MIN + 1)) + 1
			: (unsigned LONG int) STRTOL_LONG_MAX))
		overflow = 1;
#endif

	if (overflow)
	{
		__set_errno(ERANGE);
#if UNSIGNED
		return STRTOL_ULONG_MAX;
#else
		return negative ? STRTOL_LONG_MIN : STRTOL_LONG_MAX;
#endif
	}

	/* Return the result of the appropriate sign.  */
	return negative ? -i : i;

noconv:
	/* We must handle a special case here: the base is 0 or 16 and the
	   first two characters are '0' and 'x', but the rest are no
	   hexadecimal digits.  This is no error case.  We return 0 and
	   ENDPTR points to the `x`.  */
	if (endptr != NULL)
	{
		if (save - nptr >= 2 && TOUPPER(save[-1]) == L_('X')
			&& save[-2] == L_('0'))
			* endptr = (STRING_TYPE*)& save[-1];
		else
			/*  There was no number to convert.  */
			*endptr = (STRING_TYPE*)nptr;
	}

	return 0L;
}
#if defined _LIBC && !defined USE_WIDE_CHAR
libc_hidden_def(INTERNAL(__strtol_l))
#endif



long st__strtol_l(const STRING_TYPE* nptr, STRING_TYPE** endptr,
	int base, locale_t loc)
{
	return st__strtol_l(nptr, endptr, base, 0, loc);
}
