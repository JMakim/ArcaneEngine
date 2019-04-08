#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include "Arcane.hpp"
#include "InitializeCheck.hpp"

int main(int argc, char** argv) {
	InitializeCheck::Initialize();
	Arcane::Start();
}