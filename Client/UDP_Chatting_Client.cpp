#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int MAX_BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET MySock;
	SOCKADDR_IN ServAddr, FromAddr;
	char SendBuffer[MAX_BUF_SIZE], RecvBuffer[MAX_BUF_SIZE];
	int ServAddrSize = 0, FromAddrSize = 0;

	if (argc != 3)
	{
		ErrorHandling("main function parameter Error");
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
	{
		ErrorHandling("WSAStartup Error");
	}

	MySock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (MySock == INVALID_SOCKET)
	{
		ErrorHandling("socket Error");
	}

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr(argv[1]);
	ServAddr.sin_port = htons(atoi(argv[2]));
	/*ServAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServAddr.sin_port = htons(9999);*/
	
	ServAddrSize = sizeof(ServAddr);
	FromAddrSize = sizeof(FromAddr);
	while (true)
	{
		cout << "> ";
		cin >> SendBuffer;

		sendto(MySock, SendBuffer, sizeof(SendBuffer), 0, (SOCKADDR*)&ServAddr, ServAddrSize);

		recvfrom(MySock, RecvBuffer, MAX_BUF_SIZE - 1, 0, (SOCKADDR*)&FromAddr, &FromAddrSize);

		cout << RecvBuffer << endl;
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