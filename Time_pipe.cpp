#include <cstdlib>
#include <cstdio>
#include <array>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <chrono>
#include <windows.h>
#include <winsock.h>

//#include <io.h>
//#include <windows.h>

using namespace std;

void pipecommand(string strcmd)
{
	std::array<char, 80> buffer;
	FILE* pipe = _popen(strcmd.c_str(), "r");
	if (!pipe)
	{
		std::cerr << "Cannot open pipe to read" << endl;
		return;
	}

	int c = 0;
	while (fgets(buffer.data(), 80, pipe) != NULL)
	{
		c++;	//counts the number of lines that return back from the pipe
		std::cout << c << " " << buffer.data();
	}
	_pclose(pipe);

}

//int gettimeofday(struct timeval* start, void* tzp)
//{
//	if ()
//}


int main(void)
{
	/*struct timeval start, end;

	if(gettimeofday())*/

	DWORD pid = GetCurrentProcessId();
	std::cout << "Current Process PID: " << pid << std::endl;



	string strcmd = "";
	while (1)
	{
		std::cout << "Enter a command:";
		std::getline(cin, strcmd);
		pipecommand(strcmd);
	}
	return 0;

}