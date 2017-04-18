#include "UDPCommunication.hpp"

namespace Nurn
{
	UDPCommunication::UDPCommunication()
	{

	}

	UDPCommunication::~UDPCommunication()
	{

	}		

	bool UDPCommunication::SendPackage(const Address & destination, const void * data, int size)
	{
		assert(data);
		assert(size > 0);
		int networkSocket = listenSocket.GetSocket();

		if (networkSocket == 0)
		{
			return false;
		}

		int sent_bytes = sendto(networkSocket, (const char*)data, size, 0, (sockaddr*)&destination.GetAddress(), sizeof(sockaddr_in));

		return sent_bytes == size;
	}

	int UDPCommunication::ReceivePackage(Address & sender, void * data, int size)
	{
		assert(data);
		assert(size > 0);

		int networkSocket = listenSocket.GetSocket();

		if (networkSocket == 0)
		{
			return 0;
		}

#if PLATFORM == PLATFORM_WINDOWS
		typedef int socklen_t;
#endif
		sockaddr_in from;
		socklen_t fromLength = sizeof(from);

		int received_bytes = recvfrom(networkSocket, (char*)data, size, 0, (sockaddr*) &from, &fromLength);

		sender.SetAddress(from);
		return received_bytes;
	}

	int UDPCommunication::Peek(Address & sender, void * data, int size)
	{
		assert(data);
		assert(size > 0);

		int networkSocket = listenSocket.GetSocket();

		if (networkSocket == 0)
		{
			return 0;
		}

		int received_bytes = recv(networkSocket, (char*)data, size, MSG_PEEK);

		return received_bytes;
	}
}