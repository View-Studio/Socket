#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;

const int BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET ServSock, ClntSock;
	SOCKADDR_IN ServAddr, ClntAddr;
	char Message[BUF_SIZE] = {0, };
	int StrLen = 0, ClntAddrSize = sizeof(ClntAddr);

	if (argc != 2)
	{
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
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
	ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ServAddr.sin_port = htons(atoi(argv[1]));

	if (bind(ServSock, (SOCKADDR*)&ServAddr, sizeof(ServAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind Error");
	}

	if (listen(ServSock, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen Error");
	}

	for (int i = 0; i < 5; ++i)
	{
		ClntSock = accept(ServSock, (SOCKADDR*)&ClntAddr, &ClntAddrSize);

		if (ClntSock == INVALID_SOCKET)
		{
			ErrorHandling("accept Error");
		}
		else
		{
			cout << "Connected client " << (i + 1) << endl;
		}

		while ((StrLen = recv(ClntSock, Message, BUF_SIZE, 0)) != 0)
		{
			// 반복문 안에 recv 함수가 있기 때문에 이 코드는 데이터를 다 받지 못 할 가능성이 존재하지 않음.
			send(ClntSock, Message, StrLen, 0);
		}

		closesocket(ClntSock);
	}

	closesocket(ServSock);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}