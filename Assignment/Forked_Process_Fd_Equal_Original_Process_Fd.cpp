// Linux Code

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	int Socket = 0;
	pid_t pid = 0;

	Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == -1)
	{
		ErrorHandling("socket Error");
	}

	pid = fork();
	if (pid == 0)
	{
		cout << "Child Process Socket Fd : { " << Socket << " }" << endl;
	}
	else
	{
		cout << "Parent Process Socket Fd : { " << Socket << " }" << endl;
	}

	close(Socket);

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}
