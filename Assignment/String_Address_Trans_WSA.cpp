#define _WINSOCK_DEPRECATED_NO_WARNINGS
#undef UNICODE
#undef _UNICODE

#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	const char* StrAddr = "212.155.168.102:9403";

	char StrBuffer[50];
	int Size = 0;
	SOCKADDR_IN ServAddr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "WSAStartup Error" << endl;
	}

	Size = sizeof(ServAddr);
	WSAStringToAddress((LPTSTR)StrAddr, AF_INET, NULL, (SOCKADDR*)&ServAddr, &Size);

	Size = sizeof(StrBuffer);
	WSAAddressToString((SOCKADDR*)&ServAddr, sizeof(ServAddr), NULL, StrBuffer, (LPDWORD)&Size);

	cout << "Second Conv Result : " << StrBuffer << endl;

	WSACleanup();

	return 0;
}