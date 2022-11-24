#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int MAX_BUF_SIZE = 30;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA WsaData;
	SOCKET RecvSock;
	SOCKADDR_IN MyAddr, SourceAddr;
	struct ip_mreq JoinAddr;
	char ReadBuffer[MAX_BUF_SIZE];
	int result = 0, SourceAddrSize = 0;

	if (argc != 3)
	{
		ErrorHandling("<Group IP> <PORT>");
	}

	if (WSAStartup(MAKEWORD(2, 2), &WsaData) == SOCKET_ERROR)
	{
		ErrorHandling("WSAStartup Error");
	}

	RecvSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (RecvSock == INVALID_SOCKET)
	{
		ErrorHandling("socket Error");
	}

	memset(&MyAddr, 0, sizeof(MyAddr));
	memset(&SourceAddr, 0, sizeof(SourceAddr));
	MyAddr.sin_family = AF_INET;
	MyAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//MyAddr.sin_port = htons(9999);
	MyAddr.sin_port = htons(atoi(argv[2]));

	if (bind(RecvSock, (SOCKADDR*)&MyAddr, sizeof(MyAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind Error");
	}

	/*JoinAddr.imr_multiaddr.s_addr = inet_addr("224.1.1.2");*/
	JoinAddr.imr_multiaddr.s_addr = inet_addr(argv[1]);
	JoinAddr.imr_interface.s_addr = htonl(INADDR_ANY);

	setsockopt(RecvSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&JoinAddr, sizeof(JoinAddr));

	SourceAddrSize = sizeof(SourceAddr);

	while (true)
	{
		result = recvfrom(RecvSock, ReadBuffer, MAX_BUF_SIZE - 1, 0, (SOCKADDR*)&SourceAddr, &SourceAddrSize);

		if (result < 0)
		{
			break;
		}

		ReadBuffer[result] = '\0';
		cout << ReadBuffer << endl;
	}

	closesocket(RecvSock);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}