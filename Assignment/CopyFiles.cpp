#include <iostream>
#include <fstream>

int main()
{
	/*std::ifstream InputFile;
	InputFile.open("../PracticeMaterial/OriginFile.txt", std::ios::in);*/
	/*std::ofstream OutputFile;
	OutputFile.open("../PracticeMaterial/CopyFile.txt", std::ios::out | std::ios::trunc);*/
	std::ifstream InputFile("../PracticeMaterial/OriginFile.txt", std::ios::in);
	std::ofstream OutputFile("../PracticeMaterial/CopyFile.txt", std::ios::out | std::ios::trunc);
	char buffer[256] = { 0, };

	if (!InputFile.fail())
	{
		while (InputFile.getline(buffer, 256))
		{
			OutputFile << buffer << std::endl;
		}
	}

	InputFile.close();
	OutputFile.close();

	return 0;
}