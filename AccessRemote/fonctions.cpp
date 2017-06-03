#include "stdafx.h"
#include "fonctions.h"

vector<string> parseCommandStr(string command) {
	string cmd = command;
	cmd += ' ';
	string strWait = "";
	vector<string> str;
	int nbSpace = 0;
	bool alreadySpace = false;

	

	for (unsigned int i = 0; i < cmd.size(); i++)
	{
		if (cmd[i] == ' ' && !alreadySpace)
		{
			nbSpace++;
			str.push_back(strWait);
			strWait = "";
			alreadySpace = true;
		}
		else if (cmd[i] == ' ' && alreadySpace)
		{
			alreadySpace = true;
		}
		else {
			strWait += cmd[i];
			alreadySpace = false;
		}
	}
	return str;
}

int vectorPlacement(vector<string> command, string msg, string msgAlias)
{
	int rtn = -1;
	for (unsigned int i = 0; i < command.size(); i++)
	{
		if (command[i] == msg || command[i] == msgAlias)
		{
			rtn = i;
			break;
		}
	}
	return rtn;
}

//  ||add -t box -n 3


void killProcessByName(const char* filename)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	UINT uExitCode;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	size_t *length = 0;
	mbstate_t * mbstate;
	wchar_t *wfilename = new wchar_t[strlen(filename) + 1]; //memory allocation
	mbstowcs_s(length, wfilename, (size_t)(strlen(filename) + 1), filename, (size_t)(strlen(filename) + 1));

	if (snapshot == INVALID_HANDLE_VALUE)
	{
		std::wcout << "can't get a process snapshot ";
		delete[]wfilename;
		return;
	}

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			//wcout << s.c_str() << endl;
			//cout << "entry: " << name << endl;
			if ((_tcsicmp(entry.szExeFile, wfilename)) == 0)
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);

				TerminateProcess(hProcess, 1);

				CloseHandle(hProcess);
			}
		}
	}
	delete[]wfilename;
	CloseHandle(snapshot);
}

vector<wstring> dir(wstring folder)
{
	vector<wstring> names;
	wstring search_path = folder + _T("/*.*");
	wstring result;
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				result = _T("<DIR> ");
				result += fd.cFileName;
				names.push_back(result);
				result = _T("");
			}
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}