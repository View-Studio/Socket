#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET MySock;
	SOCKADDR_IN ServAddr;
	int StrLen = 0;
	int RecvTmp = 0;
	int RecvLen = 0;
	string Message = "";

	if (argc != 3)
	{
		ErrorHandling("main function parameter Error");
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup Error");
	}

	MySock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (MySock == INVALID_SOCKET)
	{
		ErrorHandling("socket Error");
	}

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr(argv[1]);
	ServAddr.sin_port = htons(atoi(argv[2]));

	if (connect(MySock, (SOCKADDR*)&ServAddr, sizeof(ServAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect Error");
	}
	else
	{
		cout << "Connected..." << endl;
	}

	while (true)
	{
		cout << "My> ";
		cin >> Message;

		if (Message == "q" || Message == "Q")
		{
			break;
		}

		StrLen = send(MySock, &Message[0], (int)Message.size(), 0);
		//StrLen = send(MySock, Message.c_str(), (int)Message.size(), 0);
		while (RecvLen < StrLen)
		{
			RecvTmp = recv(MySock, &Message[RecvLen], BUF_SIZE - 1, 0);
			if (RecvTmp == SOCKET_ERROR)
			{
				ErrorHandling("read Error");
			}
			RecvLen += RecvTmp;
		}
		
		Message[RecvLen] = '\0';
		cout << "Message from server : " << Message << endl;
		RecvLen = 0;
	}

	closesocket(MySock);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}