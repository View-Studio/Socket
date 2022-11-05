#include <iostream>
#include <unistd.h>

using namespace std;

const int MAX_BUF_SIZE = 128;

int main(int argc, char* argv[])
{
	pid_t pid;
	int FirstPipeFds[2];
	int SecondPipeFds[2];

	pipe(FirstPipeFds); // [0]이 수신 fd, [1] 송신 fd
	pipe(SecondPipeFds);

	pid = fork();
	if (pid == 0)
	{
		char ChildProcessStr[MAX_BUF_SIZE] = "Hi Parent, Im Child Process";
		char ParentSendBuffer[MAX_BUF_SIZE];
		for (int i = 0; i < 2; ++i)
		{
			read(SecondPipeFds[0], ParentSendBuffer, sizeof(ParentSendBuffer));
			
			if (i == 0)
			{
				write(FirstPipeFds[1], ChildProcessStr, sizeof(ChildProcessStr));
			} 
			if (i == 1)
			{
				write(FirstPipeFds[1], ParentSendBuffer, sizeof(ParentSendBuffer));
			}

			cout << "[Child Process] : " << ParentSendBuffer << endl;
		}
		
	} 
	else
	{
		char ParentProcessStr[MAX_BUF_SIZE] = "Hi Child, Im Parent Process";
		char ChildSendBuffer[MAX_BUF_SIZE];

		for (int i = 0; i < 2; ++i)
		{
			if (i == 0)
			{
				write(SecondPipeFds[1], ParentProcessStr, sizeof(ParentProcessStr));
			}
			if (i == 1)
			{
				write(SecondPipeFds[1], ChildSendBuffer, sizeof(ChildSendBuffer));
			}
			
			read(FirstPipeFds[0], ChildSendBuffer, sizeof(ChildSendBuffer));

			cout << "[Parent Process] : " << ChildSendBuffer << endl;
		}		
	}

	return 0;
}