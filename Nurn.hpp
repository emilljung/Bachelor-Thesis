#pragma once

#include "NetworkDefines.hpp"

#if PLATFORM == PLATFORM_WINDOWS
#include <winsock2.h>
#pragma comment( lib, "ws2_32.lib" )

#else PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#endif

#include <iostream>
#include <assert.h>
#include "Address.hpp"

#ifdef DEBUGGING_NETWORK
#include "DebugNetwork.hpp"
#endif

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
		NURN_API NurnEngine();
		NURN_API virtual ~NurnEngine();

		NURN_API bool InitializeHost(uint16_t port = 35500);
		// Takes an ipv4 address with each of the 255 values seperated by commas, for example ( 127, 0, 0, 1 )
		NURN_API bool InitializeClient(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4, uint16_t destPort = 35500, uint16_t origPort = 35500);

		NURN_API bool AcceptCommunication();

		NURN_API bool Send(const void * data, int size);
		NURN_API bool Send(const Address & destination, const void * data, int size);
		NURN_API bool Send();

		// Returns 1 or 0 if a byte has been recieved or not. Then returns the data through the void *
		NURN_API int Receive(void * data, int size);
		NURN_API bool Receive();

		NURN_API void Shutdown();
		
#ifdef DEBUGGING_NETWORK
		NURN_API float getPing();
#endif

	private:
		Address address;

		unsigned char * sendPacketMemoryPtr;
		unsigned char * receivePacketMemoryPtr;

#ifdef USING_UDP
		UDPCommunication netCommunication;
#elif USING_TCP
		TCPCommunication netCommunication;
#endif

#ifdef DEBUGGING_NETWORK
		DebugNetwork debugNetwork;
#endif
	};
}