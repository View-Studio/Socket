#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

const int M_BUF_SIZE = 128;

int main(int argc, char* argv[])
{
	pid_t pid;
	int PipeFds[2];
	char SendBuffer[M_BUF_SIZE];
	char RecvBuffer[M_BUF_SIZE];

	pipe(PipeFds);

	pid = fork();
	if (pid == 0)
	{
		strcpy(SendBuffer, "Im Child Process");

		write(PipeFds[1], SendBuffer, sizeof(SendBuffer));
		sleep(2);
		read(PipeFds[0], RecvBuffer, M_BUF_SIZE);
		cout << "[Child] " << RecvBuffer << endl;
	}
	else
	{
		strcpy(SendBuffer, "Im Parent Process");

		read(PipeFds[0], RecvBuffer, M_BUF_SIZE);
		write(PipeFds[1], SendBuffer, sizeof(SendBuffer));
		cout << "[Parent] " << RecvBuffer << endl;
		sleep(3);
	}

	return 0;
}