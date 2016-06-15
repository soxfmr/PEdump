#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include "dtype.h"
#include "PEdump.h"

bool IsNormalConfig(PDUMP_CONFIG);
void InitializeDumpConfig(PDUMP_CONFIG pDumper, bool value);

DWORD PEdump::LoadFile(wchar_t* szFileName, PDUMP_CONFIG pDumpConfig)
{
	if (szFileName == NULL || pDumpConfig == NULL)
		return -1;

	// Which information should be shown
	this->m_pDumpConfig = pDumpConfig;

	this->m_hFile = CreateFile(szFileName, GENERIC_READ, 
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (this->m_hFile == INVALID_HANDLE_VALUE)
	{
		return ERR_INVALID_FILE;
	}

	this->m_hMapFile = CreateFileMapping(this->m_hFile, NULL, PAGE_READONLY | SEC_IMAGE,
		0, 0, NULL);
	if (this->m_hMapFile == NULL)
	{
		return ERR_FAILED_CREATE_MAP_FILE;
	}

	this->m_lpImageBase = MapViewOfFile(this->m_hMapFile, FILE_MAP_READ, 0, 0, 0);
	if (this->m_lpImageBase == NULL)
	{
		return ERR_FAILED_MAP_VIEW_FILE;
	}

	if (! this->IsPeFile())
	{
		return ERR_INVALID_PE_FILE;
	}

	return 0;
}

PEdump::PEdump()
{
}

PEdump::~PEdump()
{
	this->Release();
}

void PEdump::ShowInfo()
{
	if (IsNormalConfig(this->m_pDumpConfig))
	{
		InitializeDumpConfig(this->m_pDumpConfig, true);
	}

	if (this->m_pDumpConfig->bShowDosHdr)
	{
		this->ShowDosHdrIinfo();
	}

	if (this->m_pDumpConfig->bShowFileHdr)
	{
		this->ShowFileHdrInfo();
	}

	if (this->m_pDumpConfig->bShowOptHdr)
	{
		this->ShowOptHdrInfo();
	}

	if (this->m_pDumpConfig->bShowDataDir)
	{
		this->ShowDataDirInfo();
	}

	if (this->m_pDumpConfig->bShowSectionInfo)
	{
		this->ShowSectionInfo();
	}

	if (this->m_pDumpConfig->bShowIAT)
	{
		this->ShowIAT();
	}

	if (this->m_pDumpConfig->bShowEAT)
	{
		this->ShowEAT();
	}

	/**if (this->m_pDumpConfig->bShowReloc)
	{
		this->ShowReloc();
	}*/
}

void PEdump::Release()
{
	if (this->m_hFile != NULL && this->m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(this->m_hFile);
		this->m_hFile = NULL;
	}

	if (this->m_hMapFile != NULL)
	{
		CloseHandle(this->m_hMapFile);
		this->m_hMapFile = NULL;
	}

	if (this->m_lpImageBase != NULL)
	{
		UnmapViewOfFile(this->m_lpImageBase);
		this->m_lpImageBase = NULL;
	}
}

bool PEdump::IsPeFile()
{
	bool bRet = false;

	if (this->m_lpImageBase == NULL)
	{
		return bRet;
	}

	this->m_pDosHdr = (PIMAGE_DOS_HEADER) this->m_lpImageBase;
	
	if (this->m_pDosHdr->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return bRet;
	}

	this->m_pNtHdr = (PIMAGE_NT_HEADERS) ((DWORD) this->m_lpImageBase + this->m_pDosHdr->e_lfanew);

	if (this->m_pNtHdr->Signature == IMAGE_NT_SIGNATURE)
	{
		bRet = true;
	}

	return bRet;
}

/**
* IMAGE_DOS_HEADER, show the RVA of the IMAGE_NT_HEADERS only.
*/
void PEdump::ShowDosHdrIinfo()
{
	this->ShowBanner(TEXT("DOS Header"), false);
	this->ShowInfo("0x%08X", TEXT("NT Headers RVA"), this->m_pDosHdr->e_lfanew);
}

void PEdump::ShowFileHdrInfo()
{
	PIMAGE_FILE_HEADER lpFileHdr = NULL;

	struct tm *stTime;
	char szBuffer[80] = { 0 };

	if (this->m_pNtHdr == NULL)
		return;

	lpFileHdr = &this->m_pNtHdr->FileHeader;
	// Format the linking time
	if (lpFileHdr->TimeDateStamp != 0)
	{
		stTime = localtime((time_t*) &lpFileHdr->TimeDateStamp);
		// Is the time valid
		if (stTime != NULL)
			strftime(szBuffer, 80, "%Y-%m-%d %H:%M:%S", stTime);
	}
	
	this->ShowBanner(TEXT("File Header"));

	this->ShowInfo("0x%04X", TEXT("Machine"), lpFileHdr->Machine);
	this->ShowCharacteristicDesc(lpFileHdr->Machine, CD_FILE_MACHINE, GET_ARRAY_SIZE(CD_FILE_MACHINE), true);

	this->ShowInfo("%d", TEXT("Number Of Section"), lpFileHdr->NumberOfSections);
	this->ShowInfo("0x%08X (%s)", TEXT("Linked At"), lpFileHdr->TimeDateStamp, 
		strlen(szBuffer) == 0 ? "0" : szBuffer);
	this->ShowInfo("%d", TEXT("Optional Header Size"), lpFileHdr->SizeOfOptionalHeader);

	this->ShowInfo("0x%08X", TEXT("Characteristics"), lpFileHdr->Characteristics);
	this->ShowCharacteristicDesc(lpFileHdr->Characteristics, CD_FILE_CHAR, GET_ARRAY_SIZE(CD_FILE_CHAR));
}

void PEdump::ShowOptHdrInfo()
{
	PIMAGE_OPTIONAL_HEADER lpOptHdr = NULL;

	if (this->m_pNtHdr == NULL)
		return;

	lpOptHdr = &this->m_pNtHdr->OptionalHeader;

	this->ShowBanner(TEXT("Optional Header"));

	this->ShowInfo("0x%03X", TEXT("Magic"), lpOptHdr->Magic);
	this->ShowCharacteristicDesc(lpOptHdr->Magic, CD_OPT_MAGIC, GET_ARRAY_SIZE(CD_OPT_MAGIC), true);

	this->ShowInfo("0x%08X", TEXT("ImageBase"), lpOptHdr->ImageBase);
	this->ShowInfo("0x%08X", TEXT("Base Of Code"), lpOptHdr->BaseOfCode);
	this->ShowInfo("0x%08X", TEXT("Base Of Data"), lpOptHdr->BaseOfData);
	this->ShowInfo("0x%08X", TEXT("OEP"), lpOptHdr->AddressOfEntryPoint);

	this->ShowInfo("0x%X", TEXT("Section Alignment"), lpOptHdr->SectionAlignment);
	this->ShowInfo("0x%X", TEXT("File Alignment"), lpOptHdr->FileAlignment);

	// Visual version
	this->ShowInfo("%d.%d", TEXT("Linker Version"), lpOptHdr->MajorLinkerVersion, lpOptHdr->MinorLinkerVersion);
	this->ShowInfo("%d.%d", TEXT("OS Version"), lpOptHdr->MajorOperatingSystemVersion, lpOptHdr->MinorOperatingSystemVersion);
	this->ShowInfo("%d.%d", TEXT("Image Version"), lpOptHdr->MajorImageVersion, lpOptHdr->MinorImageVersion);
	this->ShowInfo("%d.%d", TEXT("Subsystem Version"), lpOptHdr->MajorSubsystemVersion, lpOptHdr->MinorSubsystemVersion);

	this->ShowInfo("0x%X", TEXT("Image Size"), lpOptHdr->SizeOfImage);
	this->ShowInfo("0x%X", TEXT("Code Size"), lpOptHdr->SizeOfCode);
	this->ShowInfo("0x%X", TEXT("Headers Size"), lpOptHdr->SizeOfHeaders);

	this->ShowInfo("0x%08X", TEXT("CheckSum"), lpOptHdr->CheckSum);

	this->ShowInfo("%d", TEXT("Subsystem"), lpOptHdr->Subsystem);
	this->ShowCharacteristicDesc(lpOptHdr->Subsystem, CD_OPT_SUBSYSTEM, GET_ARRAY_SIZE(CD_OPT_SUBSYSTEM), true);

	this->ShowInfo("0x%08X", TEXT("Dll Characteristics"), lpOptHdr->DllCharacteristics);
	this->ShowCharacteristicDesc(lpOptHdr->DllCharacteristics, CD_OPT_DLL_CHAR, GET_ARRAY_SIZE(CD_OPT_DLL_CHAR), true);

	this->ShowInfo("%d", TEXT("Data Directory Size"), lpOptHdr->NumberOfRvaAndSizes);
	this->ShowInfo("0x%08X", TEXT("Loader Flags"), lpOptHdr->LoaderFlags);
}

void PEdump::ShowDataDirInfo()
{
	int i;
	PIMAGE_OPTIONAL_HEADER lpOptHdr = &this->m_pNtHdr->OptionalHeader;

	this->ShowBanner(TEXT("Data Directory"));
	// Table header
	printf("%-26s\t%-8s\t%-8s\n", "Name", "RVA", "Size");

	for (i = 0; i < IMAGE_NUMBEROF_DIRECTORY_ENTRIES; i++)
	{
		printf("%-26s\t", CD_DATA_DIR[i].szDesc);

		SHOW_AVALIDABLE_ADDRESS(lpOptHdr->DataDirectory[i].VirtualAddress);
		SHOW_AVALIDABLE_ADDRESS(lpOptHdr->DataDirectory[i].Size);

		printf("\n");
	}
}

void PEdump::ShowSectionInfo()
{
	int i = 0;
	WORD wNumSec = this->m_pNtHdr->FileHeader.NumberOfSections;

	PIMAGE_SECTION_HEADER pSecHdr = IMAGE_FIRST_SECTION(this->m_pNtHdr);

	this->ShowBanner(TEXT("Sections"));
	// Table header
	printf("%-10s\t%-8s\t%-8s\t%-8s\t%-8s\t%s\n", "Name", "RVA", "V.Size", "RAW", "R.Size", "Characteristics");

	for (; i < wNumSec; i++)
	{
		printf("%-10s\t0x%08X\t0x%08X\t0x%08X\t0x%08X\t0x%08X\n", pSecHdr->Name,
			pSecHdr->VirtualAddress,
			pSecHdr->Misc.VirtualSize,
			pSecHdr->PointerToRawData,
			pSecHdr->SizeOfRawData,
			pSecHdr->Characteristics);
		// To next section
		pSecHdr++;
	}
}

void PEdump::ShowIAT()
{
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc = NULL;
	PIMAGE_THUNK_DATA pChunkData = NULL;
	PIMAGE_IMPORT_BY_NAME pImportNameTable = NULL;
	// RVA
	DWORD dwIIDVirtualAddr = this->m_pNtHdr->OptionalHeader
		.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	// Size
	/*DWORD dwSize = this->m_pNtHdr->OptionalHeader
		.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size;*/
	
	if (dwIIDVirtualAddr == 0)
	{
		return;
	}

	this->ShowBanner(TEXT("Import Table"));

	pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR) ((DWORD) this->m_lpImageBase + dwIIDVirtualAddr);
	while (pImportDesc->FirstThunk)
	{
		this->m_Colorama.Output(FORE_DARK_GREEN, "%-20s\n", (char*)((DWORD) this->m_lpImageBase + pImportDesc->Name));
		pChunkData = (PIMAGE_THUNK_DATA) ((DWORD) this->m_lpImageBase + pImportDesc->FirstThunk);

		while (pChunkData->u1.AddressOfData)
		{
			pImportNameTable = (PIMAGE_IMPORT_BY_NAME) ((DWORD) this->m_lpImageBase + pChunkData->u1.AddressOfData);
			printf("%d\t%s\n", pImportNameTable->Hint, pImportNameTable->Name);

			pChunkData++;
		}

		printf("\n");

		pImportDesc++;
	}
}

void PEdump::ShowEAT()
{
	DWORD i;
	PIMAGE_EXPORT_DIRECTORY pExportDir = NULL;
	DWORD* pFuncName = NULL, *pFuncAddr = NULL;
	WORD *pFuncOrdinal = NULL;

	DWORD dwExportDir = this->m_pNtHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]
		.VirtualAddress;
	if (dwExportDir == 0)
	{
		return;
	}

	pExportDir = (PIMAGE_EXPORT_DIRECTORY) ((DWORD) this->m_lpImageBase + dwExportDir);
	if (pExportDir->NumberOfFunctions == 0)
	{
		return;
	}

	pFuncName		= (DWORD*) ((DWORD) this->m_lpImageBase + pExportDir->AddressOfNames);
	pFuncOrdinal	= (WORD*) ((DWORD) this->m_lpImageBase + pExportDir->AddressOfNameOrdinals);
	pFuncAddr		= (DWORD*) ((DWORD) this->m_lpImageBase + pExportDir->AddressOfFunctions);

	this->ShowBanner(TEXT("Export Table"));
	this->m_Colorama.Output(FORE_DARK_GREEN, "%s\n", (char*) ((DWORD) this->m_lpImageBase + pExportDir->Name));

	for (i = 0; i < pExportDir->NumberOfFunctions; i++)
	{
		printf("%d\t%-30s\t0x%08X\n", pFuncOrdinal[i],
			(char*) ((DWORD) this->m_lpImageBase + *pFuncName), pFuncAddr[pFuncOrdinal[i]]);

		pFuncName++;
	}
}

/**
void PEdump::ShowReloc()
{
}
*/

void PEdump::ShowInfo(const char * format, wchar_t * title, ...)
{
	va_list args;
	va_start(args, title);

	m_Colorama.Output(FORE_DARK_GREEN, "%-20ls%5s", title, ": ");
	vprintf(format, args);
	printf("\n");

	va_end(args);
}

void PEdump::ShowBanner(wchar_t * text, bool striped)
{
	m_Colorama.Output(FORE_LIGHT_BLUE, "%s[%ls]\n", striped ? "\n" : "", text);
}

void PEdump::ShowCharacteristicDesc(DWORD dwFlags, const CHARACTERISTIC_DESC* desc, int size, bool unique)
{
	bool condition = false;
	for (int i = 0; i < size; i++)
	{
		condition = unique ? desc[i].dwFlag == dwFlags : (desc[i].dwFlag & dwFlags) != 0;

		if (condition)
		{
			printf("\t\t\t%s\t0x%08X\n", desc[i].szDesc, desc[i].dwFlag);
			// Jump out if it is a unique flag
			if (unique) break;
		}
	}
}
