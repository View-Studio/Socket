#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

const int MAX_BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	int ServSock, ClntSock, LastFd, SelectResult;
	sockaddr_in ServAddr, ClntAddr;
	socklen_t ClntAddrSize;
	fd_set ReadFds, TempFds;
	struct timeval Timeout;
	char SendBuffer[MAX_BUF_SIZE], ReadBuffer[MAX_BUF_SIZE];
	int TempLen = 0, RecvLen = 0;
	unsigned char TotalLen = ' ';
	unsigned char SendBufferLength = 0;

	if (argc != 2)
	{
		ErrorHandling("main function parameter Error");
	}

	ServSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServSock == -1)
	{
		ErrorHandling("socket Error");
	}

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ServAddr.sin_port = htons(atoi(argv[1]));

	if (bind(ServSock, (sockaddr*)&ServAddr, sizeof(ServAddr)) == -1)
	{
		ErrorHandling("bind Error");
	}

	if (listen(ServSock, 5) == -1)
	{
		ErrorHandling("listen Error");
	}

	FD_ZERO(&ReadFds);
	FD_SET(ServSock, &ReadFds);
	LastFd = ServSock;

	while (true)
	{
		TempFds = ReadFds;
		Timeout.tv_sec = 5;
		Timeout.tv_usec = 0;
		
		SelectResult = select(LastFd + 1, &TempFds, 0, 0, &Timeout);

		if (SelectResult == -1)
		{
			ErrorHandling("select Error");
			continue;
		} 
		else if (SelectResult == 0)
		{
			cout << "Time Out..." << endl;
			continue;
		}
		else if (SelectResult > 0)
		{
			for (int i = 0; i < LastFd+1; ++i)
			{
				if (FD_ISSET(i, &TempFds) > 0)
				{
					if (i == ServSock)
					{
						ClntAddrSize = sizeof(ClntAddr);
						ClntSock = accept(ServSock, (sockaddr*)&ClntAddr, &ClntAddrSize);
						FD_SET(ClntSock, &ReadFds);
						LastFd = ClntSock;

						cout << "New Client Connected..." << endl;
						continue;
					}
					else if (i > ServSock)
					{
						TempLen = read(i, ReadBuffer, MAX_BUF_SIZE - 1);
						if (TempLen == 0)
						{
							for (int j = ServSock + 1; j < LastFd + 1; ++j)
							{
								char ExitBuffer[MAX_BUF_SIZE] = "한 명이 나갔습니다.";
								if (j == i)
									continue;

								write(j, ExitBuffer, strlen(ExitBuffer) + 1);
							}
							FD_CLR(i, &ReadFds);
							close(i);
							continue;
						}
						
						for (int j = ServSock + 1; j < LastFd + 1; ++j)
						{
							if (j == i)
								continue;

							write(j, ReadBuffer, strlen(ReadBuffer)+1);
						}
					}
				}
			}
			continue;
		}
	}

	close(ServSock);
	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}