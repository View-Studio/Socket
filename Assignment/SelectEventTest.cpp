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
	fd_set ReadFds;
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

	ClntAddrSize = sizeof(ClntAddr);
	ClntSock = accept(ServSock, (sockaddr*)&ClntAddr, &ClntAddrSize);
	if (ClntSock == -1)
	{
		ErrorHandling("accept Error");
	}
	
	sleep(10);

	FD_SET(ClntSock, &ReadFds);
	select((ClntSock + 1), &ReadFds, 0, 0, NULL);

	if (FD_ISSET(ClntSock, &ReadFds))
	{
		read(ClntSock, RecvBuffer, MAX_BUF_SIZE);
		strcpy(SendBuffer, RecvBuffer);
		write(ClntSock, SendBuffer, sizeof(SendBuffer));
	}

	close(ClntSock);
	close(ServSock);
	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}