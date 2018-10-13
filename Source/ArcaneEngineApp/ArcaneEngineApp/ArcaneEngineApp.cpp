#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include "Initialization.h"
#include "Resources.h"

int main()
{
	while (true) {
		checkGraphicsResources();
		loadAudioResources();
		checkAudioResources();
		loadGraphicsResources();
		detectHardware();
		checkInput();
		setMem();
		alocCache();
		setAudioBuffer();
		setGraphicsBuffer();
		initWindow();
		AudioListener();
	}
	return 0;
}