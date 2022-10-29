#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int MAX_CLIENT = 100, MAX_BUF_SIZE = 1024;

void ErrorHandling(const char* message);
bool operator==(const SOCKADDR_IN& L_SockAddr, const SOCKADDR_IN& R_SockAddr);
int SockAddrInLength(int SizeofClntAddrList);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET ServSock;
	SOCKADDR_IN ServAddr, TempClntAddr, CompareAddr, ClntAddrList[MAX_CLIENT] = {NULL, };
	char SendBuffer[MAX_BUF_SIZE], RecvBuffer[MAX_BUF_SIZE];
	int TempClntAddrSize = 0;
	int ClntAddrCount = 0;

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

	memset(&CompareAddr, 0, sizeof(CompareAddr));
	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ServAddr.sin_port = htons(atoi(argv[1]));
	//ServAddr.sin_port = htons(9999);

	if (bind(ServSock, (SOCKADDR*)&ServAddr, sizeof(ServAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind Error");
	}

	TempClntAddrSize = sizeof(TempClntAddr);
	while (true)
	{
		recvfrom(ServSock, RecvBuffer, MAX_BUF_SIZE - 1, 0, (SOCKADDR*)&TempClntAddr, &TempClntAddrSize);

		for (int i = 0; i < SockAddrInLength(sizeof(ClntAddrList)); ++i)
		{
			if (!(ClntAddrList[i] == TempClntAddr) && (ClntAddrList[i] == CompareAddr))
			{
				ClntAddrList[i] = TempClntAddr;
				++ClntAddrCount;
				break;
			}
		}

		strcpy_s(SendBuffer, sizeof(SendBuffer), RecvBuffer);
		// 멀티캐스트
		for (int i = 0; i < ClntAddrCount; ++i)
		{
			if (!(ClntAddrList[i] == TempClntAddr))
			{
				sendto(ServSock, SendBuffer, sizeof(SendBuffer), 0, (SOCKADDR*)&ClntAddrList[i], TempClntAddrSize);
			}
		}

		cout << RecvBuffer << endl;
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

bool operator==(const SOCKADDR_IN& L_SockAddr, const SOCKADDR_IN& R_SockAddr)
{
	return (L_SockAddr.sin_addr.s_addr == R_SockAddr.sin_addr.s_addr) && (L_SockAddr.sin_port == R_SockAddr.sin_port);
}

int SockAddrInLength(int SizeofClntAddrList)
{
	return (SizeofClntAddrList / sizeof(SOCKADDR_IN));
}