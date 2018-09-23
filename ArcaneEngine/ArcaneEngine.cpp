#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>

#define DIV 1024000
#define WIDTH 3
using namespace std;

DWORD readCPUSpeed()
{
	DWORD BufSize = sizeof(DWORD);
	DWORD dwMHz;
	DWORD type = REG_DWORD;
	HKEY hKey;
	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
		0, KEY_READ, &hKey);
	if (lError == ERROR_SUCCESS)
	{
		// query the key:
		RegQueryValueEx(hKey, L"~MHz", NULL, &type, (LPBYTE)&dwMHz, &BufSize);
	}
	cout << "CPU Speed in MHz: " << dwMHz << endl;
	return 0;
}

void checkVirtualMemory() {
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex);

	GlobalMemoryStatusEx(&statex);

	if (statex.ullTotalPhys > 307200)
		_tprintf(TEXT("You have more then 300 MB of space \n"));
	else
		_tprintf(TEXT("You do not have enough space \n"));

	_tprintf(TEXT("There are %*I64d MB of RAM in total.\n"),
		WIDTH, statex.ullTotalPhys / DIV);
	_tprintf(TEXT("There are %*I64d MB of RAM free.\n"),
		WIDTH, statex.ullAvailPhys / DIV);
	_tprintf(TEXT("There are %*I64d total MB of virtual memory.\n"),
		WIDTH, statex.ullTotalVirtual / DIV);
	_tprintf(TEXT("There are %*I64d free  MB of virtual memory.\n"),
		WIDTH, statex.ullAvailVirtual / DIV);

	// Show the amount of extended memory available.

	_tprintf(TEXT("There are %*I64d MB of availiable RAM.\n"),
		WIDTH, statex.ullAvailExtendedVirtual / DIV);

}

bool CheckForOneInstance() {
	HANDLE handle = CreateMutex(NULL, TRUE, L"ArcaneGameTitle");
	if (ERROR_ALREADY_EXISTS == GetLastError() || handle == NULL) {
		HWND existingGame = ::FindWindow(L"ArcaneGameTitle", NULL);
		if (existingGame) {
			ShowWindow(existingGame, SW_NORMAL);
			SetForegroundWindow(existingGame);
			cout << "Game Already Exists" << endl;
			return false;
		}
	}
	cout << "First Instance Running" << endl;
	return true;
}

int main()
{
	CheckForOneInstance();
	checkVirtualMemory();
	readCPUSpeed();
	return 0;
}