#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA WSAData;
	SOCKET MySock;
	SOCKADDR_IN DestAddr;

	if (argc != 3)
	{
		ErrorHandling("main function parameter Error");
	}

	if (WSAStartup(MAKEWORD(2, 2), &WSAData) == SOCKET_ERROR)
	{
		ErrorHandling("WSACleanup Error");
	}

	MySock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (MySock == INVALID_SOCKET)
	{
		ErrorHandling("socket Error");
	}

	memset(&DestAddr, 0, sizeof(DestAddr));
	DestAddr.sin_family = AF_INET;
	DestAddr.sin_addr.s_addr = inet_addr(argv[1]);
	DestAddr.sin_port = htons(atoi(argv[2]));
	/*DestAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	DestAddr.sin_port = htons(9999);*/

	if (connect(MySock, (SOCKADDR*)&DestAddr, sizeof(DestAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect Error");
	}

	send(MySock, "123", (int)strlen("123"), 0);
	send(MySock, "4", (int)strlen("4"), MSG_OOB);
	send(MySock, "567", (int)strlen("567"), 0);
	send(MySock, "890", (int)strlen("890"), MSG_OOB);

	closesocket(MySock);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}