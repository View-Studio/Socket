#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET ServSock;
	SOCKADDR_IN ServAddr, ClntAddr;
	int ClntAddrSize = 0;

	if (argc != 2)
	{
		ErrorHandling("main function parameter Error");
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
	{
		ErrorHandling("WSAStartup Error");
	}

	ServSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
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

	// TODO 

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}