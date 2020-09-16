#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <string>
#include <cstdarg>//for dynamic number of arguments
#include <iostream>

class MemManager
{
public:
	//Default Functions
	MemManager();
	~MemManager();

	//VAR
	DWORD th32ProcessID;						//process ID
	HANDLE hProcess;							//process handle
	std::vector<MODULEENTRY32> modEntrys;		//vector of modEntrys in procHandle

	//FUNC
	//gets the process handle/ID
	bool getProc(std::string target);
	//gets a given module and adds it to the module lists
	bool getMod(std::string target);
	

	//PATTERN SCANNING

	//TODO create a function in MemManager Class to automate the search through a given module, if given the name of the module, and the mask/pattern
	//DWORD PatternScanModule(std::string module, std::string pattern, std::string mask);

	//scans through a base with a given size, searching for a pattern with wildcards defined by mask
	//DWORD PatternScan(char* base, int size, std::string pattern, std::string mask);

	//READ
	template <class T> inline T Read(DWORD dwAddress) {
		T value;
		ReadProcessMemory(hProcess, LPVOID(dwAddress), &value, sizeof(value), 0);
		return value;
	}

	//Write
	template <class T> inline void Write(DWORD dwAddress, const T& value) {
		WriteProcessMemory(hProcess, LPVOID(dwAddress), &value, sizeof(value), 0);
	}

};

extern MemManager* MEM;