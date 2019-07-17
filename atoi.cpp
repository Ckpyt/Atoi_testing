#include "atoi.h"

int st_atoi(const char* nptr)
{
	return (int)st_strtol(nptr, (char**)NULL, 10);
}