#include <WinSock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET ServSock;
	SOCKADDR_IN Addr;
	const char* PORT = "9503";

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0);
	{
		cout << "WSAStartup Error" << endl;
	}

	ServSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset(&Addr, 0, sizeof(Addr));
	Addr.sin_family = AF_INET;
	Addr.sin_addr.s_addr = htonl(INADDR_ANY);
	Addr.sin_port = htons(atoi(PORT));

	bind(ServSock, (SOCKADDR*)&Addr, sizeof(Addr));

	closesocket(ServSock);

	WSACleanup();

	return 0;
}