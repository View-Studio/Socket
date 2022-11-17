#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int MAX_BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA WSAData;
	SOCKET ListenSock, SendSock;
	SOCKADDR_IN ListenAddr, SendAddr;
	fd_set Read, Except, ReadCpy, ExceptCpy;
	TIMEVAL Timeout;
	char RecvBuffer[MAX_BUF_SIZE];
	int SendAddrSize, Result, StrLen;

	FD_ZERO(&Read);
	FD_ZERO(&Except);

	if (argc != 2)
	{
		ErrorHandling("main function parameter Error");
	}

	if (WSAStartup(MAKEWORD(2, 2), &WSAData) == SOCKET_ERROR)
	{
		ErrorHandling("WSAStartup Error");
	}

	ListenSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSock == INVALID_SOCKET)
	{
		ErrorHandling("socket Error");
	}

	memset(&ListenAddr, 0, sizeof(ListenAddr));
	ListenAddr.sin_family = AF_INET;
	ListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ListenAddr.sin_port = htons(atoi(argv[1]));

	if (bind(ListenSock, (SOCKADDR*)&ListenAddr, sizeof(ListenAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind Error");
	}

	if (listen(ListenSock, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen Error");
	}

	SendAddrSize = sizeof(SendAddr);
	SendSock = accept(ListenSock, (SOCKADDR*)&SendAddr, &SendAddrSize);
	if (SendSock == INVALID_SOCKET)
	{
		ErrorHandling("accept Error");
	}

	FD_SET(SendSock, &Read);
	FD_SET(SendSock, &Except);

	while (true)
	{
		ReadCpy = Read;
		ExceptCpy = Except;
		Timeout.tv_sec = 5;
		Timeout.tv_usec = 0;

		Result = select(0, &ReadCpy, 0, &ExceptCpy, &Timeout);

		if (Result > 0)
		{
			if (FD_ISSET(SendSock, &ExceptCpy) > 0)
			{
				StrLen = recv(SendSock, RecvBuffer, MAX_BUF_SIZE, MSG_OOB);
				RecvBuffer[StrLen] = '\0';
				cout << "[Urgent MSG] : " << RecvBuffer << endl;
			}
			if (FD_ISSET(SendSock, &ReadCpy) > 0)
			{
				StrLen = recv(SendSock, RecvBuffer, MAX_BUF_SIZE, 0);
				if (StrLen == 0)
				{
					FD_CLR(SendSock, &Read);
					FD_CLR(SendSock, &Except);
					closesocket(SendSock);
					break;
				}
				else
				{
					RecvBuffer[StrLen] = '\0';
					cout << RecvBuffer << endl;
				}
			}
		}
	}

	closesocket(ListenSock);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}