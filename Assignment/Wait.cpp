// Linux Code

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char* argv[])
{
	int status = 0;
	pid_t pid = fork();

	if (pid == 0)
	{
		return 3;
	}
	else
	{
		cout << "1. Child Process ID : { " << pid << " }" << endl;
		pid = fork();
		if (pid == 0)
		{
			exit(7);
		}
		else
		{
			cout << "2. Child Process ID : { " << pid << " }" << endl;

			wait(&status);
			if (WIFEXITED(status))
			{
				cout << "Childe send one : { " << WEXITSTATUS(status) << " }" << endl;
			}

			wait(&status);
			if (WIFEXITED(status))
			{
				cout << "Childe send one : { " << WEXITSTATUS(status) << " }" << endl;
			}

			sleep(30);
		}
	}

	return 0;
}