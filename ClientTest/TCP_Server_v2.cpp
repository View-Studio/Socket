#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET ServSock, ClntSock;
	SOCKADDR_IN ServAddr, ClntAddr;
	char SendBuffer[BUF_SIZE], RecvBuffer[BUF_SIZE];
	const char* SendMessage[3] = { "Wow! Hi! I'm Server!", "What's your name?", "I love you!!" };
	int MessageLength = 0;
	int szClntAddr = 0, RecvLen = 0, RecvTmp = 0;

	if (argc != 2)
	{
		ErrorHandling("main function parameter Error");
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
	{
		ErrorHandling("WSAStartup Error");
	}

	ServSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServSock == INVALID_SOCKET)
	{
		ErrorHandling("socket Error");
	}

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_port = htons(atoi(argv[1]));
	ServAddr.sin_port = htons(9999);

	if (bind(ServSock, (SOCKADDR*)&ServAddr, sizeof(ServAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind Error");
	}

	if (listen(ServSock, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen Error");
	}

	szClntAddr = sizeof(ClntAddr);
	ClntSock = accept(ServSock, (SOCKADDR*)&ClntAddr, &szClntAddr);
	if (ClntSock == INVALID_SOCKET)
	{
		ErrorHandling("accept Error");
	}

	for (int i = 0; i < 3; ++i)
	{
		MessageLength = (int)strlen(SendMessage[i]);
		SendBuffer[0] = (char)MessageLength;
		strcpy_s(SendBuffer + 1, sizeof(SendBuffer)-1, SendMessage[i]);

		if (send(ClntSock, SendBuffer, sizeof(SendBuffer), 0) == SOCKET_ERROR)
		{
			ErrorHandling("send Error");
		}

		if (recv(ClntSock, RecvBuffer, 1, 0) == SOCKET_ERROR)
		{
			ErrorHandling("recv Error 1");
		}

		RecvLen = 1;

		while (RecvLen < (int)RecvBuffer[0])
		{
			if ((RecvTmp = recv(ClntSock, &RecvBuffer[RecvLen], BUF_SIZE - 1, 0)) == SOCKET_ERROR)
			{
				ErrorHandling("recv Error 2");
			}
			RecvLen += RecvTmp;
		}

		cout << (RecvBuffer+1) << endl;
	}

	closesocket(ClntSock);
	closesocket(ServSock);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}