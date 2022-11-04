#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int BUF_SIZE = 50;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET MySock;
	SOCKADDR_IN ServAddr;
	char SendBuffer[BUF_SIZE] = "Hi, I'm UDP Client", RecvBuffer[BUF_SIZE];
	int ServAddrSize = 0, RecvBufLen = 0;

	if (argc != 3)
	{
		ErrorHandling("main function paramater Error");
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
	{
		ErrorHandling("WSAStartup Error");
	}

	MySock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr(argv[1]);
	ServAddr.sin_port = htons(atoi(argv[2]));

	ServAddrSize = sizeof(ServAddr);
	while (true)
	{
		cout << "> ";
		cin >> SendBuffer;

		sendto(MySock, SendBuffer, (int)sizeof(SendBuffer), 0, (SOCKADDR*)&ServAddr, sizeof(ServAddr));
		recvfrom(MySock, RecvBuffer, BUF_SIZE, 0, (SOCKADDR*)&ServAddr, &ServAddrSize);

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