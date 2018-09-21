// ArcaneEngine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

#define DIV 1024000
#define WIDTH 3




int main()
{
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
	
	/*_tprintf(TEXT("There are %*I64d total KB of paging file.\n"),
		WIDTH, statex.ullTotalPageFile / DIV);
	_tprintf(TEXT("There are %*I64d free  KB of paging file.\n"),
		WIDTH, statex.ullAvailPageFile / DIV);*/
	/*_tprintf(TEXT("There is  %*ld percent of memory in use.\n"),
		WIDTH, statex.dwMemoryLoad);
	_tprintf(TEXT("There are %*I64d MB of physical memory in total.\n"),
		WIDTH, statex.ullTotalPhys / DIV);
	_tprintf(TEXT("There are %*I64d MB of physical memory free.\n"),
		WIDTH, statex.ullAvailPhys / DIV);*/
    return 0;
}






