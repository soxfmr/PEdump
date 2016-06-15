#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include "dtype.h"
#include "PEdump.h"
#include "getopt.h"

void InitializeDumpConfig(PDUMP_CONFIG pDumper, bool value);

void Usage()
{
	printf("Usage: PEdump [options] <file>\n\n");
	printf("%-20s\tDOS Header\n", "--header-dos");
	printf("%-20s\tNT File Header\n", "--header-file");
	printf("%-20s\tNT Optional Header\n", "--header-opt");
	printf("%-20s\tData Directory\n", "--dir");
	printf("%-20s\tSection Information\n", "--section");
	printf("%-20s\tImport Address Table\n", "--iat");
	printf("%-20s\tEmport Address Table If Exists\n", "--eat");
}

int _tmain(int argc, wchar_t* argv[])
{
	int opt, opt_index = 0, opt_counter = 0;
	DWORD dwLoadStatus = 0;

	PEdump dumper;
	DUMP_CONFIG dumpConfig;

	wchar_t *szFileName = NULL;
	static struct option long_options[] = {
		{ TEXT("header-dos"), no_argument, 0, 'd' },
		{ TEXT("header-file"), no_argument, 0, 'f' },
		{ TEXT("header-opt"), no_argument, 0, 'o' },
		{ TEXT("dir"), no_argument, 0, 'r' },
		{ TEXT("section"), no_argument, 0, 's' },
		{ TEXT("iat"), no_argument, 0, 'i'},
		{ TEXT("eat"), no_argument, 0, 'e'},
		{ ARG_NULL , ARG_NULL , ARG_NULL , ARG_NULL }
	};

	// No argument present
	if (argc <= 1)
	{
		Usage();
		return 0;
	}

	// Reset the config
	InitializeDumpConfig(&dumpConfig, false);

	// Parse the arguments
	while ((opt = getopt_long(argc, argv, TEXT("dforsie"), long_options, &opt_index)) != -1)
	{
		switch (opt)
		{
			case 'd':
				dumpConfig.bShowDosHdr = true;
				break;
			case 'f':
				dumpConfig.bShowFileHdr = true;
				break;
			case 'o':
				dumpConfig.bShowOptHdr = true;
				break;
			case 'r':
				dumpConfig.bShowDataDir = true;
				break;
			case 's':
				dumpConfig.bShowSectionInfo = true;
				break;
			case 'i':
				dumpConfig.bShowIAT = true;
				break;
			case 'e':
				dumpConfig.bShowEAT = true;
			default:
				break;
		}

		opt_counter++;
	}

	// Retrieve the filename
	if (opt_counter < argc)
	{
		szFileName = argv[++opt_counter];
	}

	// Load the PE file
	dwLoadStatus = dumper.LoadFile(szFileName, &dumpConfig);
	if (dwLoadStatus != NULL)
	{
		switch (dwLoadStatus)
		{
			case ERR_INVALID_FILE:
				printf("Cannot open file %ls\n", szFileName);
				break;
			case ERR_FAILED_CREATE_MAP_FILE:
				printf("Cannot create the file mapping.\n");
				break;
			case ERR_FAILED_MAP_VIEW_FILE:
				printf("Cannot create the view of the file mapping.\n");
				break;
			case ERR_INVALID_PE_FILE:
				printf("Invalid PE file.\n");
				break;
			default:
				break;
		}

		return -1;
	}

	// Dump the information
	dumper.ShowInfo();

	return 0;
}