#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <cstdlib>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char Message[40];
	int strLen = 0;
	int idx = 0, readLen = 0;

	if (argc != 3)
	{
		cout << "Usage : " << argv[0] << "<port>" << endl;
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		ErrorHandling("WSAStartup() error!");
	}

	hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hSocket == INVALID_SOCKET)
	{
		ErrorHandling("socket() error!");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect() error!");
	}

	while (readLen = recv(hSocket, &Message[idx++], 1, 0))
	{
		if (readLen == SOCKET_ERROR)
		{
			ErrorHandling("read() error!");
		}

		strLen += readLen;
	}

	cout << "Message from server : " << Message << endl;
	cout << "Function read call count : " << strLen << endl;

	closesocket(hSocket);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}