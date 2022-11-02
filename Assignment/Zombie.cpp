// Linux Code

#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[])
{
	pid_t pid = fork();

	if (pid == 0) // 자식 프로세스
	{
		cout << "Hi, I am a child process." << endl;
	}
	else // 부모 프로세스
	{
		cout << "Child Process ID : { " << pid << " }" << endl;
		sleep(30); // Sleep 30 sec
	}

	if (pid == 0)
	{
		cout << "End Child Process" << endl;
	}
	else
	{
		cout << "End Parent Process" << endl;
	}

	return 0;
}