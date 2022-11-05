#include <iostream>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

using namespace std;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	int ServSock, ClntSock;
	sockaddr_in ServAddr, ClntAddr;
	pid_t pid;
	sigaction SigAct;

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

	while (true)
	{
		ClntSock = accept(ServSock, (sockaddr*)&ClntAddr, sizeof(ClntAddr));
		if (ClntSock == -1)
		{
			continue;
		}

		pid = fork();
		if (pid == -1)
		{
			close(ClntSock);
			continue;
		} 
		else if (pid == 0)
		{
			close(ServSock);

			// TODO
		}
		else
		{
			close(ClntSock);
			continue;
		}
	}


	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}