// Linux Code

#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

void TimeOut(int sig);

int main(int argc, char* argv[])
{
	struct sigaction act;
	act.sa_handler = TimeOut;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGALRM, &act, 0);

	alarm(2);

	for (int i = 0; i < 3; ++i)
	{
		cout << "Wait..." << endl;
		sleep(100);
	}

	return 0;
}

void TimeOut(int sig)
{
	if (sig == SIGALRM)
	{
		cout << "Time Out" << endl;
	}
	alarm(2);
}
