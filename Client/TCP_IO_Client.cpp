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

	char Message[128];
	int strLen;

	if (argc != 3)
	{
		cout << "Usage : " << argv[0] << "<port>" << endl;
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}

	hSocket = socket(PSF_INET, SOCK_STREAM, IPPROTO_TCP);
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

	for (int i = 0; i < 3000; ++i)
	{
		// Busy Waiting
		cout << "Wait time " << i << endl;
	}

	strLen = recv(hSocket, Message, sizeof(Message) - 1, 0);
	if (strLen == -1)
	{
		ErrorHandling("read() error!");
	}

	cout << "Message from server : " << Message << endl;

	closesocket(hSocket);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}