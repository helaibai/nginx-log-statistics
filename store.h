#ifndef __STORE_H__
#define __STORE_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define HASH_MAGIC 2654435769 
#define HASH_SIZE  1024*1024+717

#define hash(key, hash_size) ((key)%(hash_size))


#ifdef DEBUG_ERR
#define DBG_ERR(fmt, args...) fprintf(stderr, "%s:%-5d: " fmt, __FUNCTION__, __LINE__, ## args)
#else
#define DBG_ERR(fmt, args...)   do {  } while(0)
#endif

#ifdef DEBUG_INFO
#define DBG_INFO(fmt, args...) fprintf(stdout, "%s:%-5d: " fmt, __FUNCTION__, __LINE__, ## args)
#else
#define DBG_INFO(fmt, args...)   do {  } while(0)
#endif

union ip_type{
	int int_ip;
	struct in_addr net_ip;
};

struct log{
	union ip_type ip;
	int number;
	struct log *next;
};

struct log_head{
	pthread_mutex_t head_lock;
	struct log *head;
};


void hash_init();

void hash_unint();

int  hash_put(union ip_type ip);

int  hash_get(union ip_type ip, struct log *log_get);

void hash_list_all();


int log_store(char *ip,int len);

#endif//__STORE_H__
