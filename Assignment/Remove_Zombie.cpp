// Linux Code

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

using namespace std;

void SIGCHLD_Handler(int sig);

int main(int argc, char* argv[])
{
	struct sigaction SigAct;
	SigAct.sa_handler = SIGCHLD_Handler;
	sigemptyset(&SigAct.sa_mask);
	SigAct.sa_flags = 0;
	sigaction(SIGCHLD, &SigAct, 0);

	pid_t Pid = fork();

	if (Pid == 0)
	{
		sleep(10);
		return 21;
	} 
	else
	{
		cout << "Child Process ID : { " << Pid << " }" << endl;

		Pid = fork();

		if (Pid == 0)
		{
			sleep(10);
			exit(11);
		} 
		else
		{
			cout << "Child Process ID : { " << Pid << " }" << endl;
			for (int i = 0; i < 5; ++i)
			{
				sleep(5);
			}
		}
	}

	return 0;
}

void SIGCHLD_Handler(int sig)
{
	if (sig == SIGCHLD)
	{
		int status = 0;

		wait(&status);

		if (WIFEXITED(status))
		{
			cout << "Child Process Send : { " << WEXITSTATUS(status) << " }" << endl;
		}
	}
}
