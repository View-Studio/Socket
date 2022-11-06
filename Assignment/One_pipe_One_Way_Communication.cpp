#include <iostream>
#include <unistd.h>

using namespace std;

const int M_BUF_SIZE = 128;

int main(int argc, char* argv[])
{
	pid_t pid;
	int PipeFd[2];

	pipe(PipeFd);

	pid = fork();
	if (pid == 0)
	{
		char SendBuffer[M_BUF_SIZE] = "Hello! Im Child Process";
		write(PipeFd[1], SendBuffer, sizeof(SendBuffer));
	}
	else
	{
		char RecvBuffer[M_BUF_SIZE];
		read(PipeFd[0], RecvBuffer, M_BUF_SIZE);
		cout << RecvBuffer << endl;
	}

	return 0;
}