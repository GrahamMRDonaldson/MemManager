#include <iostream>
#include "MemManager.h"

int main()
{
	MemManager* MEM = new MemManager();
	DWORD someBaseAddress = reinterpret_cast<DWORD>(MEM->modEntrys[0].modBaseAddr);

	//assuming the integer location is static every time the process is started
	DWORD exampleOffset1 = 0x10;
	int someVal1 = MEM->Read<int>(someBaseAddress + exampleOffset1);


	//assuming the integer location changes everytime the process is started
	char* begin_of_module = (char*)(MEM->modEntrys[0].modBaseAddr);
	int size_of_module = (MEM->modEntrys[0].modBaseSize);
	
	//TODO do more tests to see if this really is correct. 
	//This really needs to get done
	DWORD exampleOffse2 = MEM->PatternScan(begin_of_module, size_of_module, "\xA1\x00\x00\x00\x00\x83\xC4\x1C\xF3\x0F\x10\x05", "x????xxxxxxx");

	int someVal2 = MEM->Read<int>(someBaseAddress + exampleOffse2);

}
