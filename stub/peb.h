#ifndef PEB_H
#define PEB_H

typedef struct _UNICODE_STRING  // 3 elements, 0x8 bytes (sizeof)
          {
/*0x000*/     UINT16       Length;
/*0x002*/     UINT16       MaximumLength;
/*0x004*/     UINT16*      Buffer;
          }UNICODE_STRING, *PUNICODE_STRING;

typedef struct _LDR_DATA_TABLE_ENTRY               // 18 elements, 0x50 bytes (sizeof)
          {
/*0x000*/     struct _LIST_ENTRY InLoadOrderLinks;           // 2 elements, 0x8 bytes (sizeof)
/*0x008*/     struct _LIST_ENTRY InMemoryOrderLinks;         // 2 elements, 0x8 bytes (sizeof)
/*0x010*/     struct _LIST_ENTRY InInitializationOrderLinks; // 2 elements, 0x8 bytes (sizeof)
/*0x018*/     VOID*        DllBase;
/*0x01C*/     VOID*        EntryPoint;
/*0x020*/     ULONG32      SizeOfImage;
/*0x024*/     struct _UNICODE_STRING FullDllName;            // 3 elements, 0x8 bytes (sizeof)
/*0x02C*/     struct _UNICODE_STRING BaseDllName;            // 3 elements, 0x8 bytes (sizeof)
          }LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef struct _PEB_LDR_DATA                            // 7 elements, 0x28 bytes (sizeof)
          {
/*0x000*/     ULONG32      Length;
/*0x004*/     UINT8        Initialized;
/*0x005*/     UINT8        _PADDING0_[0x3];
/*0x008*/     VOID*        SsHandle;
/*0x00C*/     struct _LIST_ENTRY InLoadOrderModuleList;           // 2 elements, 0x8 bytes (sizeof)
/*0x014*/     struct _LIST_ENTRY InMemoryOrderModuleList;         // 2 elements, 0x8 bytes (sizeof)
/*0x01C*/     struct _LIST_ENTRY InInitializationOrderModuleList; // 2 elements, 0x8 bytes (sizeof)
/*0x024*/     VOID*        EntryInProgress;
          }PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _PEB                                                                               // 65 elements, 0x210 bytes (sizeof)
          {
/*0x000*/     UINT8        InheritedAddressSpace;
/*0x001*/     UINT8        ReadImageFileExecOptions;
/*0x002*/     UINT8        BeingDebugged;
/*0x003*/     UINT8        SpareBool;
/*0x004*/     VOID*        Mutant;
/*0x008*/     VOID*        ImageBaseAddress;
/*0x00C*/     struct _PEB_LDR_DATA* Ldr;
          }PEB, *PPEB;

#endif
