#include "Nurn.hpp"
#define PACKETSIZE 2000

using namespace Nurn;
int main()
{
	NurnEngine nurn = NurnEngine();
	nurn.InitializeHost();

	unsigned char * data = new unsigned char[PACKETSIZE];

	Address source;
	//Receive a message from client
	while (nurn.Receive(source, data, PACKETSIZE) > 0)
	{
		//Send the message back to client
		nurn.Send(source, data, PACKETSIZE);
	}

	return 0;
}