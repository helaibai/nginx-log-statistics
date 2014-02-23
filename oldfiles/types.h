#ifndef __TYPES_H__
#define __TYPES_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef unsigned long u64;
typedef unsigned long ulong;
typedef unsigned int  u32; 


union ip_type{
	int int_ip;
	struct in_addr net_ip;
};

#endif//__TYPES_H__
