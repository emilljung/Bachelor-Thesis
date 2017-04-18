#include "Nurn.hpp"

using namespace Nurn;
int main()
{
	NurnEngine nurn = NurnEngine();
	nurn.InitializeHost();

	unsigned char * data = new unsigned char[2000];


	//Receive a message from client
	while (nurn.Receive(data, 2000) > 0)
	{
		//Send the message back to client
		nurn.Send(data, 2000); // write(sock, client_message, strlen(client_message));
	}

	return 0;
}