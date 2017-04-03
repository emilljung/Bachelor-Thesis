#include "Nurn.hpp"

namespace Nurn
{
	NurnEngine::NurnEngine()
	{
		this->sendPacketMemoryPtr = new unsigned char[packetSize];
		this->receivePacketMemoryPtr = new unsigned char[packetSize];
		return;
	}

	NurnEngine::~NurnEngine()
	{
		delete[] this->sendPacketMemoryPtr;
		delete[] this->receivePacketMemoryPtr;

		NurnEngine::Shutdown();
		return;
	}

	bool NurnEngine::InitializeHost(uint16_t port)
	{
#ifdef DEBUGGING_NETWORK
		this->debugNetwork.initializeDebugNetwork(0, true);
#endif

		return netCommunication.InitializeCommunicationHost(port);
	}

	bool NurnEngine::InitializeClient(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4, uint16_t destPort, uint16_t origPort)
	{
#ifdef DEBUGGING_NETWORK
		this->debugNetwork.initializeDebugNetwork(1, false);
#endif

		address = Address(ip1, ip2, ip3, ip4, destPort);

		return netCommunication.InitializeCommunicationClient(origPort, address);
	}

	bool NurnEngine::AcceptCommunication()
	{
		return netCommunication.AcceptCommunication(address);
	}

	bool NurnEngine::Send(const void * data, int size)
	{
		return netCommunication.SendPackage(address, data, size);
	}

	bool NurnEngine::Send(const Address & destination, const void * data, int size)
	{
		return netCommunication.SendPackage(destination, data, size);
	}

	bool NurnEngine::Send()
	{
		this->Send(this->sendPacketMemoryPtr, packetSize);

		return true;
	}

	int NurnEngine::Receive(void * data, int size)
	{
		return netCommunication.ReceivePackage(address, data, size);
	}

	bool NurnEngine::Receive()
	{
		int bytes_read = netCommunication.Peek(address, this->receivePacketMemoryPtr, 2);
		if (bytes_read > 0)
		{
			uint16_t expectedSize = this->receivePacketMemoryPtr[0] | this->receivePacketMemoryPtr[1] << 8;
			this->Receive(this->receivePacketMemoryPtr, expectedSize);
		}
		return true;
	}

	void NurnEngine::Shutdown()
	{
		netCommunication.Shutdown();
	}
	
#ifdef DEBUGGING_NETWORK
	float NurnEngine::getPing()
	{
		return this->debugNetwork.getPing();
	}
#endif
}
