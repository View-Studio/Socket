// Linux Code

#include <iostream>
#include <stdio.h>
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
	int ServSock, ClntSock = 0, ChildProcessIdx = 0;
	struct sockaddr_in ServAddr, ClntAddr;

	struct sigaction SigAct;
	SigAct.sa_handler = SIGCHLD_Handler;
	sigemptyset(&SigAct.sa_mask);
	SigAct.sa_flags = 0;
	sigaction(SIGCHLD, &SigAct, 0);

	socklen_t ClntAddrSize;
	int RecvBufLen = 0;
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

	if (bind(ServSock, (struct sockaddr*)&ServAddr, sizeof(ServAddr)) == -1)
	{
		ErrorHandling("bind Error");
	}

	if (listen(ServSock, 5) == -1)
	{
		ErrorHandling("listen Error");
	}
		
	while (true)
	{
		ClntAddrSize = sizeof(ClntAddr);
		ClntSock = accept(ServSock, (struct sockaddr*)&ClntAddr, &ClntAddrSize);
		if (ClntSock == -1)
		{
			continue;
		}
		else
		{
			cout << "New Client Conneceted..." << endl;
		}

		pid = fork();
		if (pid == -1)
		{
			close(ClntSock);
			continue;
		}
		if (pid == 0) // Child Process
		{
			++ChildProcessIdx;
			close(ServSock);
						
			while ((RecvBufLen = read(ClntSock, RecvBuffer, MAX_BUF_SIZE - 1) != -1)) // atoi(RecvBuffer[0]) 쓰지말기
			{
				if (write(ClntSock, RecvBuffer, RecvBufLen) == -1)
				{
					ErrorHandling("write Error");
				}
				cout << "Child Process IDX : " << ChildProcessIdx << ", RecvBuffer : " << RecvBuffer << endl;
				RecvBufLen = 0;
			}

			close(ClntSock);
			return 0;
		} 
		else // Parent Process
		{
			close(ClntSock);
			++ChildProcessIdx;
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