//
//  Wumbo-Socket.h
//  Wumbo
//
//  Created by Wade McGillis on 11/28/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#ifndef __Wumbo_Socket_h__
#define __Wumbo_Socket_h__


#include <iostream>
#include "Wumbo-Header.h"
#if PLATFORM_APPLE
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
//#include <CFNetwork/CFNetwork.h>
//#include <SystemConfigu
#else
//#include <winsock.h>
#include <windows.h>
#include <WS2tcpip.h>
#endif
#include <vector>
#include "Wumbo-Buffer.h"
/*
 fcntl(sockfd, F_SETFL, O_NONBLOCK);
 */
namespace Wumbo
{
	class Socket
	{
	public:
		static const char *uintToIP(unsigned int ip);
		static std::vector<unsigned int> getAdapterIPs();
	};
	
	class TCPSocket
	{
	private:
		//
		struct addrinfo connectedSocketAddressInfo;
		//
		bool accepted;
		struct sockaddr acceptedSocketAddr;
		socklen_t acceptedSocketSize;
	public:
		int mysocket;
		TCPSocket(int sock, struct sockaddr info, int infosize);
		TCPSocket();
		void connect(const char *address, unsigned int port);
		bool connected();
		void close();
		bool send(Wumbo::Buffer *buffer);
		Wumbo::Buffer *receive();
		const char* getipaddress();
	};
	
	class TCPListener
	{
	private:
	public:
		int listenSocket;
		void listen(unsigned int port);
		Wumbo::TCPSocket *accept();
		void close();
	};
	
	class UDPSender
	{
	private:
		char _ipaddress[32];
		char _port[32];
	private:
		struct addrinfo hints, *servinfo, *p;
		int rv;
		struct sockaddr_storage their_addr;
		socklen_t addr_len;
	public:
		int sockfd;
		bool open(const char *ip, unsigned short port);
		bool send(Wumbo::Buffer *buffer);
		void close();
	};
	
	class UDPReceiver
	{
	private:
		char *_ipaddress;
		char _port[32];
	private:
		struct addrinfo hints, *servinfo, *p;
		int rv;
		struct sockaddr_storage their_addr;
		socklen_t addr_len;
	public:
		int sockfd;
		bool open(unsigned short port);
		Wumbo::Buffer *receive(char *their_ip);
		void close();
	};
};

#endif /* defined(__Wumbo__Wumbo_Socket__) */
