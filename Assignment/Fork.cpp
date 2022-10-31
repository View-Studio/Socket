// Linux Code

#include <unistd.h>
#include <iostream>

using namespace std;

int One = 77;

int main(int argc, char* argv[])
{
	pid_t pid;
	int Two = 11;
	One += 3;
	Two += 9;

	pid = fork();

	if (pid == 0)
	{
		--One, --Two;
	}
	else
	{
		++One, ++Two;
	}

	if (pid == 0)
	{
		cout << "ChildProcess value is : [ " << One << ", " << Two << " ]" << endl;
	}
	else
	{
		cout << "ParentProcess value is : [ " << One << ", " << Two << " ]" << endl;
	}

	return 0;
}