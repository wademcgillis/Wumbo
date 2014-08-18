//
//  Wumbo-Socket.cpp
//  Wumbo
//
//  Created by Wade McGillis on 11/28/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//
#include "Wumbo-Header.h"
#if PLATFORM_WINDOWS
#include <WS2tcpip.h>
#endif
#include "Wumbo-Socket.h"
#if PLATFORM_APPLE
#define _GNU_SOURCE     /* To get defns of NI_MAXSERV and NI_MAXHOST */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#else
#include <stdio.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#endif
//#include <linux/if_link.h>


int sendall(int socket, char *buf, int len)
{
	int total = 0;        // how many bytes we've sent
	int bytesleft = len; // how many we have left to send
	int n = 0;
	
	while(total < len) {
		n = ::send(socket, buf+total, bytesleft, 0);
		if (n == -1) { break; }
		total += n;
		bytesleft -= n;
		//printf("SENDALL: sent %i bytes. total is now %i / %i\n",n,total,len);
	}
	/*for(int i=0;i<len;i++)
	{
		int c = buf[i];
		if (c == 0 || c == 10 || c == 13)
		{
			printf("%i : %i",c,c);
			printf("\n");
		}
		else
		{
			printf("%i : %c",c,(char)c);
			printf("\n");
		}
	}*/
	
	//*len = total; // return number actually sent here
	
	return n==-1?-1:total; // return -1 on failure, 0 on success
}

int recvall(int socket, char *buf, int len)
{
	int total = 0;        // how many bytes we've read
	int bytesleft = len; // how many we have left to read
	int n = 0;
	
	while(total < len) {
		n = ::recv(socket, buf+total, bytesleft, 0);
		if (n == -1) { break; }
		total += n;
		bytesleft -= n;
		//printf("RECVALL: recv'd %i bytes. total is now %i / %i\n",n,total,len);
	}
	/*for(int i=0;i<len;i++)
	{
		int c = buf[i];
		if (c == 0 || c == 10 || c == 13)
		{
			printf("%i : %i",c,c);
			printf("\n");
		}
		else
		{
			printf("%i : %c",c,(char)c);
			printf("\n");
		}
	}*/
	//*len = total; // return number actually sent here
	
	return n==-1?-1:total; // return -1 on failure, 0 on success
}


namespace Wumbo
{
	const char *Socket::uintToIP(unsigned int ip)
	{
		char *ipstr = new char[16];
		sprintf(ipstr,"%u.%u.%u.%u",(ip >> 0) & 0xFF,(ip >> 8) & 0xFF,(ip >> 16) & 0xFF,(ip >> 24) & 0xFF);
		return ipstr;
	}
	std::vector<unsigned int> Socket::getAdapterIPs()
	{
		std::vector<unsigned int> ips;
#if PLATFORM_APPLE
		struct ifaddrs *ifaddr;
		if (getifaddrs(&ifaddr) == -1)
			return ips;
		for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
			if (ifa->ifa_addr == NULL)
				continue;
			if (ifa->ifa_addr->sa_family != AF_INET)
				continue;
			unsigned int ip = ((sockaddr_in*)ifa->ifa_addr)->sin_addr.s_addr;
			if (ip == 0x0100007F)
				continue;
			ips.push_back(ip);
		}
		freeifaddrs(ifaddr);
#else
	unsigned long size = 0;
	if (GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, NULL, &size)  != ERROR_BUFFER_OVERFLOW)
		return ips;
	PIP_ADAPTER_ADDRESSES addrslist = (PIP_ADAPTER_ADDRESSES) malloc(size);
	if (GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, addrslist, &size)) // 0 is success
	{
		free(addrslist);
		return ips;
	}
	char IPCSTR[16];
	for (PIP_ADAPTER_ADDRESSES iter = addrslist; iter != NULL; iter = iter->Next)
		for (PIP_ADAPTER_UNICAST_ADDRESS addr = iter->FirstUnicastAddress; addr != NULL; addr = addr->Next)
		{
			memset(IPCSTR,0,16);
			getnameinfo(addr->Address.lpSockaddr, addr->Address.iSockaddrLength,IPCSTR,16,NULL,0,NI_NUMERICHOST);
			ips.push_back(inet_addr(IPCSTR));
		}
	free(addrslist);
#endif
		return ips;
	}
	
	
	
	
	bool UDPSender::open(const char *ip, unsigned short port)
	{
		memset(_ipaddress,0,strlen(_ipaddress));
		sprintf(_ipaddress,"%s",ip);
		sprintf(_port,"%i",port);
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_DGRAM;
		
		printf("  IP: \"%s\"\n",_ipaddress);
		printf("PORT: \"%s\"\n",_port);
		
		if ((rv = getaddrinfo(_ipaddress, _port, &hints, &servinfo)) != 0) {
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
			return false;
		}
		
		// loop through all the results and make a socket
		for(p = servinfo; p != NULL; p = p->ai_next)
		{
			if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			{
				perror("talker: socket");
				continue;
			}
			
			break;
		}
		
		const char yes = 1;
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
		{
			perror("setsockopt (SO_REUSEADDR)");
		}
		// this call is what allows broadcast packets to be sent:
		if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &yes,
					   sizeof yes) == -1) {
			perror("setsockopt (SO_BROADCAST)");
			//exit(1);
		}
		if (p == NULL)
		{
			printf("talker: failed to bind socket\n");
			return false;
		}
#if PLATFORM_WINDOWS
		u_long nonblockyes = 1;
		ioctlsocket(sockfd,FIONBIO,&nonblockyes);
#else
		int flags = fcntl(sockfd, F_GETFL, 0);
		fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
#endif
		return true;
	}
	bool UDPSender::send(Wumbo::Buffer *buffer)
	{
		//printf("udpsender::send\n");
		int total = 0;        // how many bytes we've sent
		int bytesleft = buffer->getlength(); // how many we have left to send
		int n = 0;
		while(total < buffer->getlength()) {
			//printf("bytesleft = %i, total = %i\n",bytesleft,total);
			n = ::sendto(sockfd,buffer->getbytes(), bytesleft, 0, p->ai_addr, p->ai_addrlen);
			if (n == -1) { break; }
			total += n;
			bytesleft -= n;
		}
		return n==-1?-1:total; // return -1 on failure, 0 on success
	}
	void UDPSender::close()
	{
		freeaddrinfo(servinfo);
#if PLATFORM_APPLE
		::close(sockfd);
#else
		closesocket(sockfd);
#endif
	}
	
	
	
	
	bool UDPReceiver::open(unsigned short port)
	{
		//printf("BEGIN UDPReceiver::open(%i)\n",port);
		sprintf(_port,"%i",port);
		
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_flags = AI_PASSIVE; // use my IP
		
		if ((rv = getaddrinfo(NULL, _port, &hints, &servinfo)) != 0)
		{
			//fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
			return false;
		}
		
		// loop through all the results and bind to the first we can
		for(p = servinfo; p != NULL; p = p->ai_next)
		{
			if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			{
				//perror("listener: socket");
				continue;
			}
			
			if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
			{
#if PLATFORM_APPLE
				::close(sockfd);
#else
				closesocket(sockfd);
#endif
				//perror("listener: bind");
				continue;
			}
			
			break;
		}
		const char one = 1;
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof one) == -1)
		{
			perror("setsockopt (SO_REUSEADDR)");
		}
		if (p == NULL)
		{
			//fprintf(stderr, "listener: failed to bind socket\n");
			return false;
		}
#if PLATFORM_WINDOWS
		u_long nonblockyes = 1;
		ioctlsocket(sockfd,FIONBIO,&nonblockyes);
#else
		int flags = fcntl(sockfd, F_GETFL, 0);
		fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
#endif
		freeaddrinfo(servinfo);
		//printf("END UDPReceiver::open(%i)\n",port);
		return true;
	}
	Wumbo::Buffer *UDPReceiver::receive(char *their_ip)
	{
		Wumbo::Buffer *bob = new Wumbo::Buffer();
		char *data = new char[65536];
		int numbytes = 0;
		addr_len = sizeof their_addr;
		
		if ((numbytes = recvfrom(sockfd, data, 65535, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1)
		{
			//printf("nothing\n");
			//perror("recvfrom");
			//exit(1);
		}
		
		if (numbytes > 0)
		{
			bob->setbytes(data, numbytes);
			printf("------------------------ZOOF GABBER!------------------------\n");
			if (their_ip != NULL)
				printf("listener: got packet from %s\n", inet_ntop(their_addr.ss_family, &((struct sockaddr_in *)&their_addr)->sin_addr, their_ip, INET6_ADDRSTRLEN));
		}
		else
		{
			delete[] data;
			data = NULL;
			delete bob;
			bob = NULL;
		}
		delete[] data;
		return bob;
	}
	void UDPReceiver::close()
	{
#if PLATFORM_APPLE
		::close(sockfd);
#endif
	}
	
	
	
	
	
	
	
	
	
	
	TCPSocket::TCPSocket(int sock, struct sockaddr info, int infosize)
	{
		accepted = true;
		mysocket = sock;
		
		acceptedSocketAddr = info;
		acceptedSocketSize = infosize;
		

		//memcpy(&connectedSocketAddressInfo,info,infosize);
	}
	TCPSocket::TCPSocket()
	{
		accepted = false;
	}
	
	void TCPSocket::connect(const char *address, unsigned int port)
	{
		/*struct addrinfo hints, *res;
		int sockfd;
		
		// first, load up address structs with getaddrinfo():
		
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_INET;  // use IPv4 or IPv6, whichever
		hints.ai_socktype = SOCK_STREAM;
		errno = 0;
		// we could put "80" instead on "http" on the next line:
		getaddrinfo("www.google.com", "80", &hints, &res);
		if (errno >= 0)
		{
			perror(NULL);
			errno = 0;
		}
		// make a socket:
		
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (errno >= 0)
		{
			perror(NULL);
			errno = 0;
		}
		fcntl(sockfd, F_SETFL, O_NONBLOCK);
		// connect it to the address and port we passed in to getaddrinfo():
		if (errno >= 0)
		{
			perror(NULL);
			errno = 0;
		}
		if (::connect(sockfd, res->ai_addr, res->ai_addrlen) == -1)
		{
			printf("TCPSocket::connect tried on socket %i, but we got error #%i\n",sockfd,errno);
			perror(NULL);
			
		}
		else
			printf("SUCCESS");*/
		printf("hello???\n");
		if (accepted)
		{
			printf("ACCEPTED\n");
			return;
		}
		if (port > 65535)
		{
			printf("MY PORT'S TOO BIG\n");
			return;
		}
		struct addrinfo *serverinfo;
		errno = 0;
		printf("STARTING TO DO THE CONNECT THING\n");
		// first, load up address structs with getaddrinfo():
		memset(&connectedSocketAddressInfo, 0, sizeof(connectedSocketAddressInfo));
		connectedSocketAddressInfo.ai_family = AF_INET;
		connectedSocketAddressInfo.ai_socktype = SOCK_STREAM;
		connectedSocketAddressInfo.ai_protocol = IPPROTO_TCP;
		char port_cstr[6] = ""; //12345 + \0
		sprintf(port_cstr,"%i",port);
		printf("address: %s ... port: %s\n",address,port_cstr);
		perror(NULL);
		errno = 0;
		int r;
		if ((r = getaddrinfo(address,port_cstr,&connectedSocketAddressInfo, &serverinfo)) != 0)//address, port_cstr, &connectedSocketAddressInfo, &res);
		{
			strerror(r);//errno);
			perror(NULL);
			errno = NULL;
			printf("-----\n");
		}
		else
			printf("\tsuccess\n");
		perror(NULL);
		
		
		const char yes = 1;
		struct addrinfo *walker;
		int walky = 0;
		for(walker = serverinfo; walker != NULL; walker = walker->ai_next) {
			walky++;
			mysocket = socket(walker->ai_family, walker->ai_socktype, walker->ai_protocol);
			if (mysocket == -1) {
				perror("server: socket");
				continue;
			}
			
			int ret = ::connect(mysocket, walker->ai_addr, walker->ai_addrlen);
			if (ret == -1)
			{
				printf("TCPSocket::connect tried on socket %i, but we got error #%i\n",mysocket,errno);
				perror(NULL);
				errno = 0;
				continue;
			}
			else
			{
				//printf("IPPROTO_TCP IS.... %i ... %i\n",walker->ai_protocol,IPPROTO_TCP);
				printf("TCPSocket::connect SUCCESS on socket %i!!! ... returned %i\n",mysocket,ret);
			}
			if (setsockopt(mysocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
			{
				perror("setsockopt");
				//exit(1);
			}
			
			/*if (bind(mysocket, walker->ai_addr, walker->ai_addrlen) == -1)
			{
				::close(mysocket);
				perror("server: bind");
				continue;
			}*/
			
			break;
		}
		freeaddrinfo(serverinfo);
		printf("walk successful on %ith step\n",walky);
		
		setsockopt(mysocket, IPPROTO_TCP, /*TCP_NODELAY*/1, (char*)&yes, sizeof(int));
		//setsockopt(, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		// make a socket
		/*errno = 0;
		mysocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		perror(NULL);
		errno = 0;*/
#if PLATFORM_WINDOWS
		u_long nonblockyes = 1;
		ioctlsocket(mysocket,FIONBIO,&nonblockyes);
#else
		int flags = fcntl(mysocket, F_GETFL, 0);
		fcntl(mysocket, F_SETFL, flags | O_NONBLOCK);
#endif
		perror(NULL);
		errno = 0;
		
		char buf[7] = "";
		printf("RECV RIGHT AFTER CONNECTION RETURNS: %i\n",(int)recv(mysocket,(char*)&buf,5,MSG_PEEK));
		
		errno = 0;
		//fcntl(mysocket, F_SETFL, O_NONBLOCK);
		//perror(NULL);
		//errno = 0;
	}
	bool TCPSocket::connected()
	{
		errno = 0;
		char bob[4] = "";
		int b = recv(mysocket,(char*)&bob,2,MSG_PEEK);
		//printf("RECV WHENEVER: %i\n",b);
		if (b != 0)
		{
			//printf("connected = TRUE\n");
			return true;
		}
		else
		{
			//printf("connected = FALSE ... %i\n",b);
			//strerror(b);
			//perror(NULL);
			//errno = 0;
			return false;
		}
	}
	void TCPSocket::close()
	{
#if PLATFORM_WINDOWS
		closesocket(mysocket);
#else
		::close(mysocket);
#endif
	}
	
	bool TCPSocket::send(Wumbo::Buffer *buffer)
	{
		int len = buffer->getlength();
		int l = 0;
		if ((l = sendall(mysocket,(char*)&len,4)) != 4)
		{
			printf("error occurred while sending length. Only sent %i bytes\n",l);
			return false;
		}
		if ((l = sendall(mysocket,buffer->getbytes(),buffer->getlength())) != buffer->getlength())
		{
			printf("error occurred while sending data. Only sent %i bytes\n",l);
			return false;
		}
		
		return true;
	}
	Wumbo::Buffer *TCPSocket::receive()
	{
		/*unsigned int b = 0;
		unsigned int actualSize = 0;
		int totalBytesRead = 0;
		int lastBytesRead = ::recv(mysocket,&b + totalBytesRead,4-totalBytesRead,0);
		totalBytesRead += lastBytesRead;
		int i = 0;
		while (totalBytesRead < 4)
		{
			if (lastBytesRead == 0)
			{
				printf("oops no bytes: %i\n",i);
				return NULL;
			}
			printf("b is now %i on iteration %i\n",b,i);
			i++;
			totalBytesRead += lastBytesRead;
			lastBytesRead = ::recv(mysocket,&b + totalBytesRead,4-totalBytesRead,0);
		}
		//recv(mysocket,&b,4,0);
		actualSize = b;
		printf("b is now %i with %i bytes read\n",b,totalBytesRead);
		char *buff = new char[actualSize + 1];
		memset(buff,0,actualSize+1);
		totalBytesRead = 0;
		lastBytesRead = ::recv(mysocket,buff,actualSize,0);
		totalBytesRead += lastBytesRead;
		while (totalBytesRead < actualSize)
		{
			if (lastBytesRead == 0)
				return NULL;
			totalBytesRead += lastBytesRead;
			lastBytesRead = ::recv(mysocket,buff + totalBytesRead,(actualSize)-totalBytesRead,0);
		}
		printf("totalBytesRead = %i\n",totalBytesRead);
		Wumbo::Buffer *newbuffer = new Wumbo::Buffer();
		newbuffer->setBytes(buff,actualSize);
		return newbuffer;*/
		
		int len = 0;
		int l = 0;
		if ((l = recvall(mysocket,(char*)&len,4)) != 4)
		{
			//printf("error occurred while receiving length. Only read %i bytes\n",l);
			return NULL;
		}
		
		char *buff = new char[len+1];
		if ((l = recvall(mysocket,buff,len)) != len)
		{
			//printf("error occurred while receiving data. Only read %i bytes\n",l);
			delete[] buff;
			return NULL;
		}
		Wumbo::Buffer *wBuff = new Wumbo::Buffer();
		wBuff->setbytes(buff, len);
		delete[] buff;
		return wBuff;
//		return 1;
	}
	
	const char* TCPSocket::getipaddress()
	{
		int i;
		struct hostent *he;
		struct in_addr **addr_list;
		
		char name[128];
		gethostname(name,127);
		
		if ((he = gethostbyname(name)) == NULL)
		{  // get the host info
#if PLATFORM_APPLE
			herror("gethostbyname");
#endif
			return NULL;
		}
		
		// print information about this host:
		printf("Official name is: %s\n", he->h_name);
		printf("    IP addresses: ");
		while (*he->h_aliases)
			printf("alias: %s\n", *he->h_aliases++);
		addr_list = (struct in_addr **)he->h_addr_list;
		for(i = 0; addr_list[i] != NULL; i++) {
			printf("%s ", inet_ntoa(*addr_list[i]));
		}
		printf("\n");
		
		return inet_ntoa(*addr_list[0]);
	}
	
	void TCPListener::listen(unsigned int port)
	{
		if (port > 65535)
			return;
		//strerror(errno);
		//perror(NULL);
		errno = NULL;
		struct addrinfo hints, *serverinfo;
		//fill in all the listening socket info with our own
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_INET;//ipv4
		hints.ai_socktype = SOCK_STREAM;//tcp
		hints.ai_flags = AI_PASSIVE;//fill in ip
		char port_cstr[6] = ""; //12345 + \0
		//memset(port_cstr,0,6);
		sprintf(port_cstr,"%i",port);
		printf("getaddrinfo\n");
		if (getaddrinfo(NULL, port_cstr, &hints, &serverinfo) != 0)
		{
			strerror(errno);
			perror(NULL);
			errno = NULL;
			printf("-----\n");
		}
		else
			printf("\tsuccess\n");
		// make socket
		printf("listen:\n");

		int yes = 1;
		struct addrinfo *walker;
		int walky = 0;
		for(walker = serverinfo; walker != NULL; walker = walker->ai_next) {
			walky++;
			listenSocket = socket(walker->ai_family, walker->ai_socktype, walker->ai_protocol);
			if (listenSocket == -1)
			{
				perror("server: socket");
				continue;
			}
			
			if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)) == -1)
			{
				perror("setsockopt");
				{printf("FRIGGLE NIPS\n");exit(1);}
			}
			 
			if (bind(listenSocket, walker->ai_addr, walker->ai_addrlen) == -1)
			{
#if PLATFORM_WINDOWS
				closesocket(listenSocket);
#else
				::close(listenSocket);
#endif
				perror("server: bind");
				continue;
			}
			
			break;
		}
		freeaddrinfo(serverinfo);
		printf("walk successful on %ith step\n",walky);
		
		/*listenSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (listenSocket == -1)
		{
			printf("making listen socket failed:\n");
		}
		printf("listen socket = %i\n",listenSocket);
		strerror(errno);
		perror(NULL);
		errno = NULL;
		printf("-----\n");*/
		// non blocking
		printf("fcntl\n");
#if PLATFORM_WINDOWS
		u_long nonblockyes = 1;
		ioctlsocket(listenSocket,FIONBIO,&nonblockyes);
#else
		int flags = fcntl(listenSocket, F_GETFL, 0);
		fcntl(listenSocket, F_SETFL, O_NONBLOCK | flags);
#endif

		strerror(errno);
		perror(NULL);
		errno = NULL;
		printf("-----\n");
		// ONIX USED BIND
		/*printf("bind:\n");
		bind(listenSocket, res->ai_addr, res->ai_addrlen);
		strerror(errno);
		perror(NULL);
		errno = NULL;
		printf("-----\n");*/
		// now listen
		
		if (::listen(listenSocket, 10) == -1)//10 is the waiting queue
		{
			printf("listening failed:\n");
			strerror(errno);
			perror(NULL);
		}
		printf("listenSocket = %i\n",listenSocket);
	}
	Wumbo::TCPSocket *TCPListener::accept()
	{
		struct sockaddr acceptedSocketAddr;
		socklen_t acceptedSocketSize;
		memset(&acceptedSocketAddr, 0, sizeof(acceptedSocketAddr));
		int newsocket = ::accept(listenSocket, &acceptedSocketAddr, &acceptedSocketSize);
		if (newsocket == -1)
		{
#if PLATFORM_WINDOWS
			if (WSAGetLastError() == WSAEWOULDBLOCK)
#else
			if (errno == EWOULDBLOCK)
#endif
			{
				errno = 0;
				//printf("wouldblock\n");
				//perror(NULL);
				//strerror(errno);
			}
			else
			{
				printf("oh shit error #%i\n",errno);
				perror(NULL);
				strerror(errno);
			}
			return NULL;
		}
		else
		{
			printf("yay new socket\n");
			/*int size = 7;
			::send(newsocket,(char*)&size,4,0);
			::send(newsocket,"TURTLES",7,0);
			Wumbo::Buffer *buff = new Wumbo::Buffer();
			buff->setbytes("TUR2LES",7);
			
			size = buff->getlength();
			printf("test size is %i\n",size);
			::send(newsocket,(char*)&size,4,0);
			::send(newsocket,buff->getbytes(),buff->getlength(),0);*/
			return new TCPSocket(newsocket,acceptedSocketAddr,acceptedSocketSize);//, &acceptedSocketInfo, acceptedSocketSize);
		}
	}
	void TCPListener::close()
	{
#if PLATFORM_WINDOWS
		closesocket(listenSocket);
#else
		::close(listenSocket);
#endif
	}
};