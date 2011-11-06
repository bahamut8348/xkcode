// IDTinfo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define  MAX_IDT_ENTRIES 256

#pragma pack(1)

struct IDTINFO
{
	u_short IDTLimit;
	u_short LowIDTbase;
	u_short HiIDTbase;
};


struct IDTENTRY
{
	u_short LowOffset;
	u_short selector;
	u_short unused_lo;
	u_char  segment_type:4;
	u_char  system_segment_flag:1;
	u_char  DPL:2;
	u_char  P:1;
	u_short HiOffset;
};

int _tmain(int argc, _TCHAR* argv[])
{
	
	IDTINFO idt_info;
	__asm sidt idt_info;
	
	IDTENTRY *idt_entries = (IDTENTRY *)MAKELONG( idt_info.LowIDTbase, idt_info.HiIDTbase );

	for (int count = 0; count < MAX_IDT_ENTRIES; count++)
	{
		char _t[255];
		IDTENTRY *i = &idt_entries[count];
		u_long addr = 0;
		addr = MAKELONG( i->LowOffset, i->HiOffset);
		sprintf_s(_t, "Interrupt %3ld: ISR 0x%08X", count, addr);
		printf("%s\r\n", _t);
	}
	

	return 0;
}

