#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "WSAStartup Error" << endl;
	}

	SOCKADDR_IN Addr1, Addr2;
	char* StrPtr = nullptr;
	char StrArr[20] = {0, };

	Addr1.sin_addr.s_addr = htonl(0x1020304);
	Addr2.sin_addr.s_addr = htonl(0x1010101);

	StrPtr = inet_ntoa(Addr1.sin_addr);
	strcpy_s(StrArr, sizeof(StrArr), StrPtr);
	cout << "Dotted Decimal Notation1 : " << StrPtr << endl;

	inet_ntoa(Addr2.sin_addr);
	cout << "Dotted Decimal Notation2 : " << StrPtr << endl;
	cout << "Dotted Decimal Notation3 : " << StrArr << endl;

	return 0;
}