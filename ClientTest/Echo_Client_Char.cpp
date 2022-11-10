#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;
using std::cin;

const int MAX_BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET MySock;
	SOCKADDR_IN ServAddr;
	char SendBuffer[MAX_BUF_SIZE], RecvBuffer[MAX_BUF_SIZE];
	int recvTemp = 0, StrLen = 0;

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
		cin >> SendBuffer;

		send(MySock, SendBuffer, sizeof(SendBuffer), 0);

		StrLen = 0;
		recvTemp = 0;

		while (StrLen < sizeof(SendBuffer))
		{
			recvTemp = recv(MySock, &RecvBuffer[StrLen], MAX_BUF_SIZE - 1, 0);
			StrLen += recvTemp;
		}
		cout << "Message from server : " << RecvBuffer << endl;
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