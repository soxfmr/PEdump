#pragma once
#include <Windows.h>
#include "dtype.h"
#include "Colorama.h"

class PEdump
{
public:
	PEdump();
	~PEdump();
	DWORD LoadFile(wchar_t* szFileName, PDUMP_CONFIG pDumpConfig);
	VOID ShowInfo();
protected:
	void Release();
private:
	Colorama m_Colorama;
	PDUMP_CONFIG m_pDumpConfig;

	HANDLE m_hFile;
	HANDLE m_hMapFile;
	PVOID m_lpImageBase;

	PIMAGE_DOS_HEADER m_pDosHdr;
	PIMAGE_NT_HEADERS m_pNtHdr;
	PIMAGE_SECTION_HEADER m_SecHdr;

	bool IsPeFile();

	void ShowDosHdrIinfo();
	void ShowFileHdrInfo();
	void ShowOptHdrInfo();
	void ShowDataDirInfo();
	void ShowSectionInfo();
	void ShowIAT();
	void ShowEAT();
	// void ShowReloc();

	void ShowInfo(const char* format, wchar_t* title, ...);
	void ShowBanner(wchar_t* text, bool striped = true);

	void ShowCharacteristicDesc(DWORD dwFlags, const CHARACTERISTIC_DESC* desc, int size, bool unique = false);
};

