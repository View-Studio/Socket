// Linux Code

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

const int MAX_BUF_SIZE = 1024;

void ErrorHandling(const char* message, pid_t pid = 0);
void SIGCHLD_Handler(int sig);

int main(int argc, char* argv[])
{
	pid_t pid = 0;
	int ServSock, ClntSock = 0;
	sockaddr_in ServAddr, ClntAddr;

	struct sigaction SigAct;
	SigAct.sa_handler = SIGCHLD_Handler;
	sigemptyset(&SigAct.sa_mask);
	SigAct.sa_flags = 0;
	sigaction(SIGCHLD, &SigAct, 0);

	socklen_t ClntAddrSize;
	int RecvTempLen = 0, RecvBufLen = 1;
	char SendBuffer[MAX_BUF_SIZE], RecvBuffer[MAX_BUF_SIZE];
	
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

	ClntAddrSize = sizeof(ClntAddr);
	ClntSock = accept(ServSock, (sockaddr*)&ClntAddr, &ClntAddrSize);

	pid = fork();

	for (int i = 0; i < 4; ++i)
	{
		if (pid == 0)
		{
			if (ClntSock == -1)
			{
				ErrorHandling("accept Error");
			}

			if (read(ClntSock, RecvBuffer, 1) == -1)
			{
				ErrorHandling("read Error 1");
			}
			
			while (RecvBufLen < atoi((const char*)RecvBuffer[0]))
			{
				RecvTempLen = read(ClntSock, &RecvBuffer[RecvBufLen], (MAX_BUF_SIZE - 1));
				RecvBufLen += RecvTempLen;
			}

			strcpy(SendBuffer, RecvBuffer);
			if (write(ClntSock, SendBuffer, sizeof(SendBuffer)) == -1)
			{
				ErrorHandling("write Error");
			}

			cout << "Child Process ID : " << pid << ", RecvBuffer : " << (RecvBuffer + 1) << endl;
			shutdown(ClntSock, SHUT_WR);
			while (read(ClntSock, RecvBuffer, 1) != 0);
			exit(1);
		}
		else
		{
			ClntSock = accept(ServSock, (sockaddr*)&ClntAddr, &ClntAddrSize);

			pid = fork();
		}
	}
	
	close(ServSock);

	return 0;
}

void ErrorHandling(const char* message, pid_t pid)
{
	if (pid != 0)
	{
		cout << "Error Process ID : " << pid << ", Error Message : " << message << endl;
		exit(1);
	}
	else
	{
		cout << "Error Message : " << message << endl;
		exit(1);
	}
}

void SIGCHLD_Handler(int sig)
{
	if (sig == SIGCHLD)
	{
		int status = 0;
		waitpid(-1, &status, WNOHANG);
	}
}