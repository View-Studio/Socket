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
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	int szClntAddr;
	int idx = 0, strLen = 0;
	char Message[] = "The output function is called more than the input function.";

	if (argc != 2)
	{
		cout << "Usage : " << argv[0] << "<port>" << endl;
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}

	hServSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hServSock == INVALID_SOCKET)
	{
		ErrorHandling("socket() error!");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind() error!");
	}

	if (listen(hServSock, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen() error!");
	}

	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);

	if (hClntSock == INVALID_SOCKET)
	{
		ErrorHandling("accept() error!");
	}

	while (send(hClntSock, &Message[idx++], 1, 0) != SOCKET_ERROR)
	{
		++strLen;
	}

	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}
