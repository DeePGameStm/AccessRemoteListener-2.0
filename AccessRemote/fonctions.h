#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <tlhelp32.h>
#include <winbase.h>
#include <sstream>
#include <tchar.h>
#include <wchar.h>


using namespace std;

vector<string> parseCommandStr(string command);

int vectorPlacement(vector<string> command, string msg, string msgAlias = "");

void killProcessByName(const char* filename);

vector<wstring> dir(wstring folder);