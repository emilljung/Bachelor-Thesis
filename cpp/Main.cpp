#include "Nurn.hpp"
#include "NetworkDefines.hpp"
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <fstream>

#ifdef  PLATFORM == PLATFORM_WINDOWS
#include "windows.h"
#include "psapi.h"
#else //PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/times.h"
#include "sys/vtimes.h"
#endif

#define TIMEOUT_MS 10000

int getCurrentPhysicalMemoryUsage();

using namespace Nurn;
int main(int argc, char* argv[])
{
	// Check the number of parameters
	if (argc < 2) {
		// Tell the user how to run the program
		std::cerr << "Missing parameter: packetsize" << std::endl;
		return 1;
	}

	int packetsize = atoi(argv[1]);
	std::cout << "Packetsize:" << packetsize << std::endl;
	NurnEngine nurn = NurnEngine();
	nurn.InitializeHost();

	unsigned char * data = new unsigned char[packetsize];
	Address source;
	

	std::chrono::steady_clock::time_point timeSinceLastPacket = std::chrono::steady_clock::now();
	std::chrono::high_resolution_clock::time_point cpuTime = cpuTime = std::chrono::high_resolution_clock::now();
	std::vector<long long> cpuTimes;
	//Receive a message from client
	//If nothing is recieved in timeout time, exit the loop
	while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - timeSinceLastPacket).count() < TIMEOUT_MS)
	{
		cpuTime = std::chrono::high_resolution_clock::now();
		if(nurn.Receive(source, data, packetsize) > 0)
		{
			//Send the message back to client
			nurn.Send(source, data, packetsize);
			cpuTimes.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - cpuTime).count());
			timeSinceLastPacket = std::chrono::steady_clock::now();
		}
	}

	std::ofstream myfile;
	myfile.open("example.txt");
	long long counter = 1;
	for (auto& time : cpuTimes)
	{
		 myfile << counter++ << "	"<< time << std::endl;
	}
	myfile.close();
	//write cputimes to file

	std::cout << "Timeout: Shutting down server" << std::endl;

	return 0;
}

#ifdef PLATFORM == PLATFORM_WINDOWS
int getCurrentPhysicalMemoryUsage()
{
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

	return (pmc.WorkingSetSize / 1024);
}
#else //PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

int getCurrentPhysicalMemoryUsage() { //Note: this value is in KB!
	FILE* file = fopen("/proc/self/status", "r");
	int result = -1;
	char line[128];

	while (fgets(line, 128, file) != NULL) {
		if (strncmp(line, "VmRSS:", 6) == 0) {
			result = parseLine(line);
			break;
		}
	}
	fclose(file);
	return result;
}

int parseLine(char* line) {
	// This assumes that a digit will be found and the line ends in " Kb".
	int i = strlen(line);
	const char* p = line;
	while (*p <'0' || *p > '9') p++;
	line[i - 3] = '\0';
	i = atoi(p);
	return i;
}
#endif