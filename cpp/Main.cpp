#include "Nurn.hpp"
#include "NetworkDefines.hpp"
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <fstream>
#include <string>

#if PLATFORM == PLATFORM_WINDOWS
#include "windows.h"
#include "psapi.h"
#else //PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/times.h"
//#include "sys/vtimes.h"
#endif

#define TIMEOUT_MS 10000

int getCurrentPhysicalMemoryUsage();

#if PLATFORM == PLATFORM_UNIX
int parseLine(char* line);
#endif

using namespace Nurn;
int main(int argc, char* argv[])
{
	// Check the number of parameters
	if (argc < 5) {
		// Tell the user how to run the program
		std::cerr << "Missing parameter, parameters should be packetsize, executionnumber, and runnumber" << std::endl;
		return 1;
	}

	int packetsize = atoi(argv[1]);
	std::string executionnumber = argv[2];
	std::string runnumber = argv[3];
	int port  = atoi(argv[4]);
	std::cout << "Packetsize: " << packetsize << std::endl;
	NurnEngine nurn = NurnEngine();
	nurn.InitializeHost(port);

	unsigned char * data = new unsigned char[packetsize];
	Address source;
	

	std::chrono::steady_clock::time_point timeSinceLastPacket = std::chrono::steady_clock::now();
	std::chrono::high_resolution_clock::time_point cpuTime = std::chrono::high_resolution_clock::now();
	unsigned long long tempCpuTime;
	std::vector<unsigned long long> cpuSendRecieveStart;
	std::vector<unsigned long long> cpuRecieveTimes;
	std::vector<unsigned long long> cpuSendTimes;
	std::vector<uint32_t> clientId;	
	std::vector<uint32_t> packetSequence;
	//Receive a message from client
	//If nothing is recieved in timeout time, exit the loop
	while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - timeSinceLastPacket).count() < TIMEOUT_MS)
	{
		tempCpuTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - cpuTime).count();
		if(nurn.Receive(source, data, packetsize) > 0)
		{
			cpuSendRecieveStart.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - cpuTime).count());
			cpuRecieveTimes.push_back(tempCpuTime);
			//Send the message back to client
			nurn.Send(source, data, packetsize);
			cpuSendTimes.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - cpuTime).count());
			clientId.push_back(data[8] | data[9] | data[10] | data[11]);
			packetSequence.push_back(data[12] | data[13] | data[14] | data[15]);
			timeSinceLastPacket = std::chrono::steady_clock::now();
		}
	}

	int ramUsage = getCurrentPhysicalMemoryUsage();
	std::ofstream testdatafile;
	std::string serverId = std::to_string(port%1500);
	//Write ramtestdata to memory
	testdatafile.open("./results/" + executionnumber + "-" + runnumber  + "-" + serverId + "ram" + ".txt", std::ofstream::app);
	testdatafile << ramUsage << std::endl;
	testdatafile.close();

	//Write cputimetestdata to memory
	testdatafile.open("./results/" + executionnumber + "-" + runnumber + "-" + serverId + "cputimestamps" + ".txt", std::ofstream::app);
	for (long long i = 0; i < cpuRecieveTimes.size(); ++i)
	{
		testdatafile << packetSequence.at(i) << "," << clientId.at(i) << "," << cpuSendRecieveStart.at(i) << "," << cpuRecieveTimes.at(i) << "," << cpuSendTimes.at(i) << std::endl;
	}
	testdatafile.close();

	std::cout << "Timeout: Shutting down server" << std::endl;

	return 0;
}

#if PLATFORM == PLATFORM_WINDOWS
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
