#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	const char* Addr1 = "1.2.3.4";
	const char* Addr2 = "1.2.3.256";

	if (WSAStartup(MAKEWORD(2, 2) , &wsaData) != 0)
	{
		cout << "WSAStartup Error" << endl;
	}

	unsigned long ConvAddr = inet_addr(Addr1);
	if (ConvAddr == INADDR_NONE)
	{
		cout << "Error occured" << endl;
	}
	else
	{
		cout << "Network ordered integer add : " << std::hex << ConvAddr << endl;
	}

	ConvAddr = inet_addr(Addr2);
	if (ConvAddr == INADDR_NONE)
	{
		cout << "Error occured" << endl;
	}
	else
	{
		cout << "Network ordered integer add : " << std::hex << ConvAddr << endl;
	}

	WSACleanup();

	return 0;
}