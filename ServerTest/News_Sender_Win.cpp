#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int TTL = 64, MAX_BUF_SIZE = 30;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA WsaData;
	SOCKET SendSock;
	SOCKADDR_IN MultiAddr;
	int TimeLive = TTL;
	ifstream InputStream;
	char SendBuffer[MAX_BUF_SIZE];

	if (argc != 3)
	{
		ErrorHandling("<Group IP> <PORT>");
	}

	if (WSAStartup(MAKEWORD(2, 2), &WsaData) == SOCKET_ERROR)
	{
		ErrorHandling("WSAStartup Error");
	}

	SendSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SendSock == INVALID_SOCKET)
	{
		ErrorHandling("socket Error");
	}

	memset(&MultiAddr, 0, sizeof(MultiAddr));
	MultiAddr.sin_family = AF_INET;
	/*MultiAddr.sin_addr.s_addr = inet_addr("224.1.1.2");
	MultiAddr.sin_port = htons(9999);*/
	MultiAddr.sin_addr.s_addr = inet_addr(argv[1]);
	MultiAddr.sin_port = htons(atoi(argv[2]));

	setsockopt(SendSock, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&TimeLive, sizeof(TimeLive));

	InputStream.open("C:/work/SocketNetwork/PracticeMaterial/News.txt", ios::in);

	while (!(InputStream.eof()))
	{
		InputStream.read(SendBuffer, MAX_BUF_SIZE - 1);
		SendBuffer[MAX_BUF_SIZE-1] = '\0';

		sendto(SendSock, SendBuffer, strlen(SendBuffer), 0, (SOCKADDR*)&MultiAddr, sizeof(MultiAddr));

		Sleep(2000);
	}

	InputStream.close();

	closesocket(SendSock);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}