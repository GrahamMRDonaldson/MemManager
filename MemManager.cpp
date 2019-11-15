#include "MemManager.h"

bool MemManager::getProc(std::string target)
{
	//yes the converison is a bit sloppy
	std::wstring w_target(target.begin(), target.end());
	const wchar_t* wchar_target = w_target.c_str();

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			//std::wcout << entry.szExeFile << std::endl; //this would print all of the process names until you found the one you were looking for
			if (!wcscmp(entry.szExeFile, wchar_target))
			{

				CloseHandle(snapshot);
				this->th32ProcessID = entry.th32ProcessID;
				this->hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->th32ProcessID);
				return true;
			}
		}
	}
	CloseHandle(snapshot);
	return false;
}

bool MemManager::getMod(std::string target)
{
	//yes the converison is a bit sloppy
	std::wstring w_target(target.begin(), target.end());
	const wchar_t* wchar_target = w_target.c_str();

	MODULEENTRY32 entry;
	entry.dwSize = sizeof(MODULEENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, this->th32ProcessID);

	if (Module32First(snapshot, &entry) == TRUE)
	{
		while (Module32Next(snapshot, &entry) == TRUE)
		{
			//std::wcout << entry.szModule << std::endl; //this would print all of the module names until you found the one you were looking for
			if (!wcscmp(entry.szModule, wchar_target))
			{
				this->modEntrys.push_back(entry);
				CloseHandle(snapshot);
				return true;
			}
		}
	}

	CloseHandle(snapshot);
	return false;
}

DWORD MemManager::PatternScan(char* base, int size, std::string pattern, std::string mask)
{
	int maskLength = mask.length();
	bool found = true;

	for (unsigned int i = 0; i < size - maskLength; i++) {						//cycle through the entire specified range
		found = true;															//set true at beginning to be proven wrong
		for (unsigned int j = 0; j < maskLength; j++) {							//cylce through the potential memory
			if (mask.c_str()[j] != '?' && mask.c_str()[j] != *(base + i + j)) { //if the byte isn't a wildcard, or not the correct character
				found = false;	//the pattern doesn't match
				break;			//move on in the list
			}
		}
		if (found) {//if the area matched the pattern
			return (DWORD)(base + i); //returning the beginning of the pattern
		}
	}
	return NULL; //otherwise return a nullpointer
}

MemManager::MemManager()
{
	//note this is just an example, don't read process memory of a program unless the program allows you to/is okay with it
	this->getProc("example.exe");
	this->getMod("example.dll");
	//Alternatively, you could declare what program and modules you want to read based off of the program
}

MemManager::~MemManager()
{
	CloseHandle(hProcess);
}

