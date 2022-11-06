#include <iostream>
#include <unistd.h>
#include <string.h>

using namespace std;

const int M_BUF_SIZE = 128;

int main(int argc, char* argv[])
{
	pid_t pid;
	int FirstPipeFds[2], SecondPipeFds[2];
	char SendBuffer[M_BUF_SIZE], RecvBuffer[M_BUF_SIZE];

	pipe(FirstPipeFds);
	pipe(SecondPipeFds);

	pid = fork();
	if (pid == 0)
	{
		strcpy(SendBuffer, "Im Child Process");

		write(SecondPipeFds[1], SendBuffer, sizeof(SendBuffer));
		read(FirstPipeFds[0], RecvBuffer, M_BUF_SIZE);

		cout << "[Child] " << RecvBuffer << endl;
	} 
	else
	{
		strcpy(SendBuffer, "Im Parent Process");

		write(FirstPipeFds[1], SendBuffer, sizeof(SendBuffer));
		read(SecondPipeFds[0], RecvBuffer, M_BUF_SIZE);

		cout << "[Parent] " << RecvBuffer << endl;
	}

	return 0;
}