#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "store.h"
#include "debug.h"
#include "log.h"

struct log_head head_table[HASH_SIZE];

void debug(char *p, int len)
{
	while(len--)
	printf("%c",*p++);
	printf("\n");
}

/*
int log_store(log_info *log_info_p)
{
	union ip_type ip4;
	
	inet_aton(ip, &ip4.net_ip);
	//DBG_INFO("log_store():len = %d\n",len);	
	//debug(ip, len);
	hash_put(ip4);
	return 0;
}
*/
void hash_init()
{
	int i;
	for(i=0; i<HASH_SIZE; ++i)
	{
		pthread_mutex_init(&head_table[i].head_lock, NULL);
		head_table[i].head = NULL;
	}
}
static void _each_node(struct log_info *log_node,void (*func)(void *))
{
	struct log_info *p = log_node;
	while(p)
	{
		log_node = p->next;
		func(p);
		p = log_node;
	}
}
static void _each_list(void (*func)(void *))
{
	int i;
	for(i=0; i<HASH_SIZE; ++i)
	{
		if(head_table[i].head != NULL)
		{
			_each_node(head_table[i].head, func);
		}
	}
	
}
void hash_unint()
{
	int i;
	for(i=0; i<HASH_SIZE; ++i)
	{
		head_table[i].head = NULL;
	}
	_each_list(free);
}
//kernel code
int hash_put(struct log_info *log_info_p)
{
	struct log_info *p ,*q,*node;

	unsigned int pos = hash(log_info_p->ip.int_ip, HASH_SIZE);

	pthread_mutex_lock(&head_table[pos].head_lock);

	p = head_table[pos].head;
	if(p == NULL)
	{
		node = (struct log_info *)malloc(sizeof(struct log_info));
		if(node == NULL)
		{
			DBG_ERR("no memory\n");
			pthread_mutex_unlock(&head_table[pos].head_lock);
			return -1;
		}
		node->ip.int_ip = log_info_p->ip.int_ip;
		node->access_times = 1;
		node->ip_flow += log_info_p->ip_flow;
		node->next = NULL;
		head_table[pos].head = node;
		pthread_mutex_unlock(&head_table[pos].head_lock);
		return 0;
	}
	while(p)
	{

		if(p->ip.int_ip == log_info_p->ip.int_ip)
		{
			p->access_times++;
			p->ip_flow += log_info_p->ip_flow;
			pthread_mutex_unlock(&head_table[pos].head_lock);
			return 0;
		}
		q = p;
		p = p->next;
	}
	node = (struct log_info *)malloc(sizeof(struct log_info));
	if(node == NULL)
	{
		DBG_ERR("no memory\n");
		pthread_mutex_unlock(&head_table[pos].head_lock);
		return -1;
	}
	node->ip.int_ip = log_info_p->ip.int_ip;
	node->access_times++;
	node->ip_flow += log_info_p->ip_flow;
	node->next = NULL;
	q->next = node;

	pthread_mutex_unlock(&head_table[pos].head_lock);

	return 0;
}

int  hash_get(union ip_type ip, struct log_info *log_get)
{
	struct log_info *p;
	p=head_table[hash(ip.int_ip, HASH_SIZE)].head;
	if(p == NULL)
	{
		return -1;
	}
	while(p)
	{
		if(p->ip.int_ip == ip.int_ip)
		{
			log_get->ip.int_ip = p->ip.int_ip;
			log_get->access_times = p->access_times;
			break;
		}
		p = p->next;
	}

	return 0;
}
static void print_node(void *node_p)
{
	struct log_info *p = node_p;
	printf("%s\t%d\t%ld\n",inet_ntoa(p->ip.net_ip), p->access_times, p->ip_flow);
}
void hash_list_all()
{
	_each_list(print_node);	
}
/*
int main(int argc, char **argv)
{
	struct in_addr ipn;
	char ip[]={"192.168.1.3"};	
	inet_aton(ip, &ipn);
	printf("%d\n",sizeof(ipn));
	printf("%s\n",inet_ntoa(ipn));
	return 0;
}
*/
