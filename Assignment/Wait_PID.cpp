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
		sleep(15);
		return 21;
	} 
	else
	{
		while (!(waitpid(pid, &status, WNOHANG)))
		{
			sleep(3);
			cout << "Sleep 3 sec" << endl;
		}

		if (WIFEXITED(status))
		{
			cout << "Child process send : { " << WEXITSTATUS(status) << " }" << endl;
		}
	}

	return 0;
}