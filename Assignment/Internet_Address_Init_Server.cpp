#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "WSAStartup Error" << endl;
	}

	SOCKADDR_IN Addr;
	const char* PORT = "5039";

	memset(&Addr, 0, sizeof(Addr));
	Addr.sin_family = AF_INET;
	Addr.sin_addr.s_addr = htonl(INADDR_ANY);
	Addr.sin_port = htons(atoi(PORT));

	WSACleanup();

	return 0;
}