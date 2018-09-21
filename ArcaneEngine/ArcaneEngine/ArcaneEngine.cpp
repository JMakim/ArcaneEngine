#include <windows.h>
#include "pch.h"
#include <stdio.h>
#include <iostream>
using namespace std;

typedef struct SYSTEM_INFO {
	union {
		DWORD  dwOemId;
		struct {
			WORD wProcessorArchitecture;
			WORD wReserved;
		};
	};
	DWORD     dwNumberOfProcessors;
	DWORD     dwProcessorType;
	DWORD     dwAllocationGranularity;
	WORD      wProcessorLevel;
	WORD      wProcessorRevision;
} SYSTEM_INFO;

	void GetProcessorSpeed()
	{
		float CGettheProcessorSpeedDlg::ProcSpeedCalc()
		{
			/*
			RdTSC:
			It's the Pentium instruction "ReaD Time Stamp Counter". It measures the
			number of clock cycles that have passed since the processor was reset, as a
			64-bit number. That's what the <CODE>_emit</CODE> lines do.*/
#define RdTSC __asm _emit 0x0f __asm _emit 0x31

			// variables for the clock-cycles:
			__int64 cyclesStart = 0, cyclesStop = 0;
			// variables for the High-Res Preformance Counter:
			unsigned __int64 nCtr = 0, nFreq = 0, nCtrStop = 0;


			// retrieve performance-counter frequency per second:
			if (!QueryPerformanceFrequency((LARGE_INTEGER *)&nFreq)) return 0;

			// retrieve the current value of the performance counter:
			QueryPerformanceCounter((LARGE_INTEGER *)&nCtrStop);

			// add the frequency to the counter-value:
			nCtrStop += nFreq;


			_asm
			{// retrieve the clock-cycles for the start value:
				RdTSC
				mov DWORD PTR cyclesStart, eax
				mov DWORD PTR[cyclesStart + 4], edx
			}

			do {
				// retrieve the value of the performance counter
				// until 1 sec has gone by:
				QueryPerformanceCounter((LARGE_INTEGER *)&nCtr);
			} while (nCtr < nCtrStop);

			_asm
			{// retrieve again the clock-cycles after 1 sec. has gone by:
				RdTSC
				mov DWORD PTR cyclesStop, eax
				mov DWORD PTR[cyclesStop + 4], edx
			}

			// stop-start is speed in Hz divided by 1,000,000 is speed in MHz
			return    ((float)cyclesStop - (float)cyclesStart) / 1000000;
		}
	}
}

int main()
{
	printCPUInfo();
}