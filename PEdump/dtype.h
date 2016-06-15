#pragma once

#define ERR_INVALID_FILE			0x1
#define ERR_FAILED_CREATE_MAP_FILE	0x2
#define ERR_FAILED_MAP_VIEW_FILE	0x4
#define ERR_INVALID_PE_FILE			0x8

#define GET_ARRAY_SIZE(arr) sizeof(arr) / sizeof(*arr)

#define SHOW_AVALIDABLE_ADDRESS(address) if (address == 0) \
											printf("%-8s\t", "(N/A)"); \
										else printf("0x%08X\t", address);

typedef struct stDumpInfo {
	bool bShowDosHdr;
	bool bShowFileHdr;
	bool bShowOptHdr;
	bool bShowDataDir;
	bool bShowSectionInfo;
	bool bShowIAT;
	bool bShowEAT;
	// bool bShowReloc;
} DUMP_CONFIG, *PDUMP_CONFIG;

typedef struct stCharacteristicDesc {
	long dwFlag;
	char szDesc[50];
} CHARACTERISTIC_DESC, *PCHARACTERISTIC_DESC;

const CHARACTERISTIC_DESC CD_FILE_MACHINE[] = {
	{ 0x014C,	"IMAGE_FILE_MACHINE_I386" },
	{ 0x0200,	"IMAGE_FILE_MACHINE_IA64" },
	{ 0x8664,	"IMAGE_FILE_MACHINE_AMD64" }
};

/**
	IMAGE_FILE_RELOCS_STRIPPED
	0x0001
	Relocation information was stripped from the file. The file must be loaded at its preferred base address. If the base address is not available, the loader reports an error.
	IMAGE_FILE_EXECUTABLE_IMAGE
	0x0002
	The file is executable (there are no unresolved external references).
	IMAGE_FILE_LINE_NUMS_STRIPPED
	0x0004
	COFF line numbers were stripped from the file.
	IMAGE_FILE_LOCAL_SYMS_STRIPPED
	0x0008
	COFF symbol table entries were stripped from file.
	IMAGE_FILE_AGGRESIVE_WS_TRIM
	0x0010
	Aggressively trim the working set. This value is obsolete.
	IMAGE_FILE_LARGE_ADDRESS_AWARE
	0x0020
	The application can handle addresses larger than 2 GB.
	IMAGE_FILE_BYTES_REVERSED_LO
	0x0080
	The bytes of the word are reversed. This flag is obsolete.
	IMAGE_FILE_32BIT_MACHINE
	0x0100
	The computer supports 32-bit words.
	IMAGE_FILE_DEBUG_STRIPPED
	0x0200
	Debugging information was removed and stored separately in another file.
	IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP
	0x0400
	If the image is on removable media, copy it to and run it from the swap file.
	IMAGE_FILE_NET_RUN_FROM_SWAP
	0x0800
	If the image is on the network, copy it to and run it from the swap file.
	IMAGE_FILE_SYSTEM
	0x1000
	The image is a system file.
	IMAGE_FILE_DLL
	0x2000
	The image is a DLL file. While it is an executable file, it cannot be run directly.
	IMAGE_FILE_UP_SYSTEM_ONLY
	0x4000
	The file should be run only on a uniprocessor computer.
	IMAGE_FILE_BYTES_REVERSED_HI
	0x8000
	The bytes of the word are revers
*/
const CHARACTERISTIC_DESC CD_FILE_CHAR[] = {
	{ 0x0001,	"IMAGE_FILE_RELOCS_STRIPPED" },
	{ 0x0002,	"IMAGE_FILE_EXECUTABLE_IMAGE" },
	{ 0x0004,	"IMAGE_FILE_LINE_NUMS_STRIPPED" },
	{ 0x0008,	"IMAGE_FILE_LOCAL_SYMS_STRIPPED" },
	{ 0x0010,	"IMAGE_FILE_AGGRESIVE_WS_TRIM" },
	{ 0x0020,	"IMAGE_FILE_LARGE_ADDRESS_AWARE" },
	{ 0x0080,	"IMAGE_FILE_BYTES_REVERSED_LO" },
	{ 0x0100,	"IMAGE_FILE_32BIT_MACHINE" },
	{ 0x0200,	"IMAGE_FILE_DEBUG_STRIPPED" },
	{ 0x0400,	"IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP" },
	{ 0x0800,	"IMAGE_FILE_NET_RUN_FROM_SWAP" },
	{ 0x1000,	"IMAGE_FILE_SYSTEM" },
	{ 0x2000,	"IMAGE_FILE_DLL" },
	{ 0x4000,	"IMAGE_FILE_UP_SYSTEM_ONLY" },
	{ 0x8000,	"IMAGE_FILE_BYTES_REVERSED_HI" }
};

/**
	IMAGE_NT_OPTIONAL_HDR_MAGIC
	The file is an executable image. This value is defined as IMAGE_NT_OPTIONAL_HDR32_MAGIC in a 32-bit application and as IMAGE_NT_OPTIONAL_HDR64_MAGIC in a 64-bit application.
	IMAGE_NT_OPTIONAL_HDR32_MAGIC
	0x10b
	The file is an executable image.
	IMAGE_NT_OPTIONAL_HDR64_MAGIC
	0x20b
	The file is an executable image.
	IMAGE_ROM_OPTIONAL_HDR_MAGIC
	0x107
	The file is a ROM image.
*/
const CHARACTERISTIC_DESC CD_OPT_MAGIC[] = {
	{ 0x010b,	"IMAGE_NT_OPTIONAL_HDR32_MAGIC" },
	{ 0x020b,	"IMAGE_NT_OPTIONAL_HDR64_MAGIC" },
	{ 0x0107,	"IMAGE_ROM_OPTIONAL_HDR_MAGIC" }
};

/**
	IMAGE_SUBSYSTEM_UNKNOWN
	0
	Unknown subsystem.
	IMAGE_SUBSYSTEM_NATIVE
	1
	No subsystem required (device drivers and native system processes).
	IMAGE_SUBSYSTEM_WINDOWS_GUI
	2
	Windows graphical user interface (GUI) subsystem.
	IMAGE_SUBSYSTEM_WINDOWS_CUI
	3
	Windows character-mode user interface (CUI) subsystem.
	IMAGE_SUBSYSTEM_OS2_CUI
	5
	OS/2 CUI subsystem.
	IMAGE_SUBSYSTEM_POSIX_CUI
	7
	POSIX CUI subsystem.
	IMAGE_SUBSYSTEM_WINDOWS_CE_GUI
	9
	Windows CE system.
	IMAGE_SUBSYSTEM_EFI_APPLICATION
	10
	Extensible Firmware Interface (EFI) application.
	IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER
	11
	EFI driver with boot services.
	IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER
	12
	EFI driver with run-time services.
	IMAGE_SUBSYSTEM_EFI_ROM
	13
	EFI ROM image.
	IMAGE_SUBSYSTEM_XBOX
	14
	Xbox system.
	IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION
	16
	Boot application.
*/
const CHARACTERISTIC_DESC CD_OPT_SUBSYSTEM[] = {
	{ 0x00,	"IMAGE_SUBSYSTEM_UNKNOWN" },
	{ 0x01,	"IMAGE_SUBSYSTEM_NATIVE" },
	{ 0x02,	"IMAGE_SUBSYSTEM_WINDOWS_GUI" },
	{ 0x03,	"IMAGE_SUBSYSTEM_WINDOWS_CUI" },
	{ 0x05,	"IMAGE_SUBSYSTEM_OS2_CUI" },
	{ 0x07,	"IMAGE_SUBSYSTEM_POSIX_CUI" },
	{ 0x09,	"IMAGE_SUBSYSTEM_WINDOWS_CE_GUI" },
	{ 0x0A,	"IMAGE_SUBSYSTEM_EFI_APPLICATION" },
	{ 0x0B,	"IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER" },
	{ 0x0C,	"IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER" },
	{ 0x0D,	"IMAGE_SUBSYSTEM_EFI_ROM" },
	{ 0x0E,	"IMAGE_SUBSYSTEM_XBOX" },
	{ 0x10,	"IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION" },
};

/**
	IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE
	0x0040
	The DLL can be relocated at load time.
	IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY
	0x0080
	Code integrity checks are forced. If you set this flag and a section contains only uninitialized data, set the PointerToRawData member of IMAGE_SECTION_HEADER for that section to zero; otherwise, the image will fail to load because the digital signature cannot be verified.
	IMAGE_DLLCHARACTERISTICS_NX_COMPAT
	0x0100
	The image is compatible with data execution prevention (DEP).
	IMAGE_DLLCHARACTERISTICS_NO_ISOLATION
	0x0200
	The image is isolation aware, but should not be isolated.
	IMAGE_DLLCHARACTERISTICS_NO_SEH
	0x0400
	The image does not use structured exception handling (SEH). No handlers can be called in this image.
	IMAGE_DLLCHARACTERISTICS_NO_BIND
	0x0800
	Do not bind the image.
	IMAGE_DLLCHARACTERISTICS_WDM_DRIVER
	0x2000
	A WDM driver.
	IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE
	0x8000
*/
const CHARACTERISTIC_DESC CD_OPT_DLL_CHAR[] = {
	{ 0x0040,	"IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE" },
	{ 0x0080,	"IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY" },
	{ 0x0100,	"IMAGE_DLLCHARACTERISTICS_NX_COMPAT" },
	{ 0x0200,	"IMAGE_DLLCHARACTERISTICS_NO_ISOLATION" },
	{ 0x0400,	"IMAGE_DLLCHARACTERISTICS_NO_SEH" },
	{ 0x0800,	"IMAGE_DLLCHARACTERISTICS_NO_BIND" },
	{ 0x2000,	"IMAGE_DLLCHARACTERISTICS_WDM_DRIVER" },
	{ 0x8000,	"IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE" }
};


/**
	96/112	Export table address and size
	104/120	Import table address and size
	112/128	Resource table address and size
	120/136	Exception table address and size
	128/144	Certificate table address and size
	136/152	Base relocation table address and size
	144/160	Debugging information starting address and size
	152/168	Architecture-specific data address and size
	160/176	Global pointer register relative virtual address
	168/184	Thread local storage (TLS) table address and size
	176/192	Load configuration table address and size
	184/200	Bound import table address and size
	192/208	Import address table address and size
	200/216	Delay import descriptor address and size
	208/224	The CLR header address and size
	216/232	Reserved
*/
const CHARACTERISTIC_DESC CD_DATA_DIR[] = {
	{ 0x0,	"Export Table" },
	{ 0x1,	"Import Table" },
	{ 0x2,	"Resource Table" },
	{ 0x3,	"Exception Table" },
	{ 0x4,	"Security Table" },
	{ 0x5,	"Base relocation Table" },
	{ 0x6,	"Debug" },
	{ 0x7,	"Copyright" },
	{ 0x8,	"Global Pointer" },
	{ 0x9,	"Thread local storage" },
	{ 0xA,	"Load Configuration" },
	{ 0xB,	"Bound Import Table" },
	{ 0xC,	"Import Address Table" },
	{ 0xD,	"Delay Import Descriptor" },
	{ 0xE,	"COM Header" },
	{ 0xF,	"Reserved" }
};