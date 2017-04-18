#include "Nurn.hpp"
#include <iostream>
#include <cstdlib>
#include <chrono>

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
	NurnEngine nurn = NurnEngine();
	nurn.InitializeHost();

	unsigned char * data = new unsigned char[packetsize];

	std::chrono::steady_clock::time_point timeSinceLastPacket = std::chrono::steady_clock::now();

	Address source;
	//Receive a message from client
	while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - timeSinceLastPacket).count() < 10000)
	{
		if(nurn.Receive(source, data, packetsize) > 0)
		{
			//Send the message back to client
			nurn.Send(source, data, packetsize);
			timeSinceLastPacket = std::chrono::steady_clock::now();
		}
	}

	std::cout << packetsize << std::endl;

	return 0;
}