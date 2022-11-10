#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>

using namespace std;

const int MAX_BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	int ServSock, ClntSock;
	sockaddr_in ServAddr, ClntAddr;
	char SendBuffer[MAX_BUF_SIZE], RecvBuffer[MAX_BUF_SIZE];
	fd_set ReadFds, TempFds;
	struct timeval Timeout;
	int Result = 0, LastFd = 0;
	socklen_t ClntAddrSize = 0;

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

		Result = select((LastFd + 1), &TempFds, 0, 0, &Timeout);

		if (Result == -1)
		{
			ErrorHandling("select Error");
		}
		else if (Result == 0)
		{
			cout << "Time Out..." << endl;
			continue;
		}
		else
		{
			for (int i = 0; i < LastFd + 1; ++i)
			{
				if (FD_ISSET(i, &TempFds))
				{
					if (i == ServSock)
					{
						ClntAddrSize = sizeof(ClntAddr);
						ClntSock = accept(ServSock, (sockaddr*)&ClntAddr, &ClntAddrSize);
						if (ClntSock == -1)
						{
							ErrorHandling("accept Error");
						}

						FD_SET(ClntSock, &ReadFds);
						LastFd = ClntSock;
						cout << "New Client Connected..." << endl;
					}
					else if (i > ServSock)
					{
						int ReadLen = read(i, RecvBuffer, MAX_BUF_SIZE);
						if (ReadLen == 0)
						{
							FD_CLR(i, &ReadFds);
							close(i);
						}
						else
						{
							strcpy(SendBuffer, RecvBuffer);
							write(i, SendBuffer, sizeof(SendBuffer));
						}
					}
				}
			}
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