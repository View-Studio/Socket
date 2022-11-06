#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

const int M_BUF_SIZE = 1024;

void ErrorHandling(const char* message);
void SIGCHLD_Handler(int sig);

int main(int argc, char* argv[])
{
	int ServSock, ClntSock;
	sockaddr_in ServAddr, ClntAddr;
	struct sigaction SigAct;
	pid_t pid;
	int ParentSendPipeFds[2], ChildSendPipeFds[2];
	char SendBuffer[M_BUF_SIZE], RecvBuffer[M_BUF_SIZE];
	int RecvLen = 0;
	socklen_t ClntAddrSize = sizeof(ClntAddr);

	if (argc != 2)
	{
		ErrorHandling("main function parameter Error");
	}

	if ((ServSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
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

	SigAct.sa_handler = SIGCHLD_Handler;
	sigemptyset(&SigAct.sa_mask);
	SigAct.sa_flags = 0;
	sigaction(SIGCHLD, &SigAct, 0);

	pipe(ParentSendPipeFds);
	pipe(ChildSendPipeFds);

	pid = fork();
	if (pid == 0) // 파일 저장 프로세스
	{
		close(ServSock);
		int OutputFileFd = open("/home/rudals/바탕화면/Socket/Assignment/Chapter11/EchoClientMessage.txt", O_RDWR | O_TRUNC);

		for (int i = 0; i < 10; ++i)
		{
			RecvLen = read(ParentSendPipeFds[0], RecvBuffer, M_BUF_SIZE); // read 반환값은 받은 바이트 수이고, 최대 값은 3번째 인수에 들어간 수만큼 이다.
			// 즉, 세번째 인수 값이 100인데 클라이언트가 1024바이트인 데이터를 보냈다면 100을 10번 반환하고, 24를 1번 반환한다.
			
			write(OutputFileFd, RecvBuffer, strlen(RecvBuffer)); // 위의 이유로 세번째 인수에 read 반환값을 넣는 것은 위험하기 때문에, read 반환값 대신에 받은 문자열 strlen 값 넣기.
		}
		close(OutputFileFd);
		return 0;
	} 
	else
	{
		while (true)
		{
			if ((ClntSock = accept(ServSock, (sockaddr*)&ClntAddr, &ClntAddrSize)) == -1)
			{
				continue;
			}
			else
			{
				cout << "New Client Connected..." << endl;
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

				while ((RecvLen = read(ClntSock, RecvBuffer, M_BUF_SIZE)) != -1)
				{
					if (RecvLen == 0)
					{
						break;
					}
					else
					{
						cout << RecvBuffer << endl;

						write(ClntSock, RecvBuffer, sizeof(RecvBuffer));
						write(ParentSendPipeFds[1], RecvBuffer, sizeof(RecvBuffer));
					}
				}
				close(ClntSock);
				return 0;
			}
			else
			{
				close(ClntSock);
				continue;
			}
		}
	}

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}

void SIGCHLD_Handler(int sig)
{
	int status = 0;
	if (sig == SIGCHLD)
	{
		waitpid(-1, &status, WNOHANG);
		if (WIFEXITED(status))
		{
			cout << WEXITSTATUS(status) << endl;
		}
	}
}