#include "dtype.h"

#define LENGTH_OF_DUMP_CONFIG sizeof(DUMP_CONFIG) / sizeof(bool)

void InitializeDumpConfig(PDUMP_CONFIG pDumper, bool value)
{
	bool* pArg = (bool*)pDumper;
	for (int i = 0; i < LENGTH_OF_DUMP_CONFIG; i++)
	{
		*pArg++ = value;
	}
}

bool IsNormalConfig(PDUMP_CONFIG pDumper)
{
	bool* pArg = (bool*)pDumper;
	for (int i = 0; i < LENGTH_OF_DUMP_CONFIG; i++)
	{
		if (*pArg == true)
			return false;

		pArg++;
	}

	return true;
}