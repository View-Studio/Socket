// Linux Code

#include <iostream>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

using namespace std;

const int MAX_BUF_SIZE = 128;

int main(int argc, char* argv[])
{
	fd_set Reads, Temps;
	struct timeval Timeout;
	char ReadBuffer[MAX_BUF_SIZE];
	int Result = 0, StrLen = 0;

	FD_ZERO(&Reads);
	FD_SET(0, &Reads);


	while (true)
	{
		Temps = Reads;
		Timeout.tv_sec = 5;
		Timeout.tv_usec = 0;

		Result = select(1, &Temps, 0, 0, &Timeout);

		if (Result == -1)
		{
			cout << "Select Error" << endl;
			break;
		}
		else if (Result == 0)
		{
			cout << "Time out" << endl;
			continue;
		}
		else
		{
			if (FD_ISSET(0, &Temps))
			{
				StrLen = read(0, ReadBuffer, MAX_BUF_SIZE - 1);
				ReadBuffer[StrLen] = '\0';
				cout << "Message from console : " << ReadBuffer << endl;
			}
		}
	}

	return 0;
}