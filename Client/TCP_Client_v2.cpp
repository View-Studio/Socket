#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
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
	const char* SendMessage[3] = { "Wow! Hi! I'm Client!", "My name is Code!", "I love you too!!" };
	char SendBuffer[BUF_SIZE], RecvBuffer[BUF_SIZE];
	int MessageLength = 0, RecvLen = 0, RecvTmp = 0;

	if (argc != 3)
	{
		ErrorHandling("main function parameter Error");
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
	{
		ErrorHandling("WSAStartup Error");
	}

	MySock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr(argv[1]);
	ServAddr.sin_port = htons(atoi(argv[2]));

	if (connect(MySock, (SOCKADDR*)&ServAddr, sizeof(ServAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect Error");
	}

	for (int i = 0; i < 3; ++i)
	{
		if (recv(MySock, RecvBuffer, 1, 0) == SOCKET_ERROR)
		{
			ErrorHandling("recv Error 1");
		}

		RecvLen = 1;

		while (RecvLen < (int)RecvBuffer[0])
		{
			if ((RecvTmp = recv(MySock, &RecvBuffer[RecvLen], BUF_SIZE - 1, 0)) == SOCKET_ERROR)
			{
				ErrorHandling("recv Error 2");
			}
			RecvLen += RecvTmp;
		}

		MessageLength = (int)strlen(SendMessage[i]);
		SendBuffer[0] = (char)MessageLength;
		strcpy_s(SendBuffer + 1, sizeof(SendBuffer) - 1, SendMessage[i]);

		if (send(MySock, SendBuffer, sizeof(SendBuffer), 0) == SOCKET_ERROR)
		{
			ErrorHandling("send Error");
		}

		cout << (RecvBuffer + 1) << endl;
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