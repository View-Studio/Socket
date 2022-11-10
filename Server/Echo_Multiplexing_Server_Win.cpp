#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int MAX_BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET ServSock, ClntSock;
	SOCKADDR_IN ServAddr, ClntAddr;
	fd_set ReadFds, TempFds;
	TIMEVAL Timeout;
	char RecvBuffer[MAX_BUF_SIZE];
	int SelectRes = 0, ClntAddrSize = 0, RecvLen = 0;

	if (argc != 2)
	{
		ErrorHandling("main function parameter Error");
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
	{
		ErrorHandling("WSAStartup Error");
	}

	ServSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
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

	if (listen(ServSock, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen Error");
	}

	FD_ZERO(&ReadFds);
	FD_SET(ServSock, &ReadFds);

	while (true)
	{
		TempFds = ReadFds;
		Timeout.tv_sec = 5;
		Timeout.tv_usec = 0;

		SelectRes = select(0, &TempFds, 0, 0, &Timeout);

		if (SelectRes == SOCKET_ERROR)
		{
			ErrorHandling("select Error");
		}
		else if (SelectRes == 0)
		{
			cout << "Time out..." << endl;
			continue;
		}
		else
		{
			for (unsigned int i = 0; i < ReadFds.fd_count; ++i)
			{
				if (FD_ISSET(ReadFds.fd_array[i], &TempFds))
				{
					if (ReadFds.fd_array[i] == ServSock)
					{
						ClntAddrSize = sizeof(ClntAddr);
						ClntSock = accept(ServSock, (SOCKADDR*)&ClntAddr, &ClntAddrSize);
						if (ClntSock == INVALID_SOCKET)
						{
							ErrorHandling("accept Error");
						}
						FD_SET(ClntSock, &ReadFds);
						cout << "New Client Connected..." << endl;
					}
					else
					{
						RecvLen = recv(ReadFds.fd_array[i], RecvBuffer, MAX_BUF_SIZE, 0);
						if (RecvLen == 0)
						{
							FD_CLR(ReadFds.fd_array[i], &ReadFds);
							closesocket(TempFds.fd_array[i]);
						} 
						else
						{
							send(ReadFds.fd_array[i], RecvBuffer, sizeof(RecvBuffer), 0);
						}
					}
				}
			}
		}
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