#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

void TimeOut(int sig);
void KeyControl(int sig);

int main(int argc, char* argv[])
{	
	signal(SIGALRM, TimeOut);
	signal(SIGINT, KeyControl);

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

void KeyControl(int sig)
{
	if (sig == SIGINT)
	{
		cout << endl << "Ctrl+C Pressed!" << endl;
	}
}