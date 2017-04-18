#pragma once

#include "NetworkDefines.hpp"

#include <iostream>
#include <assert.h>
#include "Address.hpp"

#ifdef USING_UDP
#include "UDPCommunication.hpp"
#elif USING_TCP
#include "TCPCommunication.hpp"
#endif

namespace Nurn
{
	class NurnEngine
	{
	public:
		NurnEngine();
		virtual ~NurnEngine();

		bool InitializeHost(uint16_t port = 35500);
		// Takes an ipv4 address with each of the 255 values seperated by commas, for example ( 127, 0, 0, 1 )
		bool InitializeClient(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4, uint16_t destPort = 35500, uint16_t origPort = 35500);

		bool AcceptCommunication();

		bool Send(const void * data, int size);
		bool Send(const Address & destination, const void * data, int size);

		// Returns 1 or 0 if a byte has been recieved or not. Then returns the data through the void *
		int Receive(void * data, int size);
		int Receive(Address & source, void * data, int size);

		void Shutdown();
		
	private:
		Address address;
		
#ifdef USING_UDP
		UDPCommunication netCommunication;
#elif USING_TCP
		TCPCommunication netCommunication;
#endif
	};
}