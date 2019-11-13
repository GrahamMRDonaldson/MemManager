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
	void getProc(std::string target);
	//gets a given module and adds it to the module lists
	void getMod(std::string target);

	//READ
	template <class T> inline T Read(DWORD dwAddress) {
		T value;
		ReadProcessMemory(hProcess, LPVOID(dwAddress), &value, sizeof(value), 0);
		return value;
	}
	//Write
	template <class T> void Write(DWORD dwAddress, const T& value) {
		WriteProcessMemory(hProcess, LPVOID(dwAddress), &value, sizeof(value), 0);
	}

	//TODO
	/*
	Write Pattern scanning so I am able to locate specific points in the process (done dynamically so its differnt each time, but the surrounding values are the same)
	Change getProc and getMod to booleans to return false if the process/module was not found
	*/

};

extern MemManager* MEM;