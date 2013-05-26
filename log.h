#ifndef __LOG_H__
#define __LOG_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "types.h"

struct log_part_t{
	off_t log_start;	
	off_t log_end;
	off_t log_cur_pos;
};

struct log_info{
	union ip_type ip;
	u32 access_times;
	u64 ip_flow;
	struct log_info *next;
};

struct log_head{
	pthread_mutex_t head_lock;
	struct log_info *head;
};



#endif//__LOG_H__
