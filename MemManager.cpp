#include "MemManager.h"

void MemManager::getProc(std::string target)
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
			}
		}
	}
}

void MemManager::getMod(std::string target)
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
			}
		}
	}

	CloseHandle(snapshot);
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

