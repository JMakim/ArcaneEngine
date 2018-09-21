#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#define DIV 1024000
#define WIDTH 3
using namespace std;

DWORD readCPUSpeed() {
	DWORD BufSize = sizeof(DWORD);
	DWORD dwMHz = 0;
	DWORD type = REG_DWORD;
	HKEY hKey;
	

	if (RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"), &hKey) != ERROR_SUCCESS)
		cout << "Can't open registry key\n";
	else
		cout << "Successfully opened registry key\n";

	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"),
		0,
		KEY_READ,
		&hKey);
	if (lError == ERROR_SUCCESS) {
		RegQueryValueEx(hKey, TEXT("MHz"),
			NULL,
			&type,
			(LPBYTE)&dwMHz,
			&BufSize);
	}
	
	cout << "Processor Speed: " << dwMHz << endl;
	return dwMHz;
}

void checkVirtualMemory() {
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex);

	GlobalMemoryStatusEx(&statex);

	if (statex.ullTotalPhys > 307200)
		_tprintf(TEXT("You have more then 300 MB of space \n"));
	else
		_tprintf(TEXT("You do not have enough space \n"));


	_tprintf(TEXT("There are %*I64d total MB of virtual memory.\n"),
		WIDTH, statex.ullTotalVirtual / DIV);
	_tprintf(TEXT("There are %*I64d free  MB of virtual memory.\n"),
		WIDTH, statex.ullAvailVirtual / DIV);

	// Show the amount of extended memory available.

	_tprintf(TEXT("There are %*I64d MB of availiable RAM.\n"),
		WIDTH, statex.ullAvailExtendedVirtual / DIV);

}



int main()
{
	checkVirtualMemory();
	readCPUSpeed();
    return 0;
}