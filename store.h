#ifndef __STORE_H__
#define __STORE_H__

#include "types.h"
#include "log.h"

#define HASH_MAGIC 2654435769 
#define HASH_SIZE  1024*1024+717

#define hash(key, hash_size) ((key)%(hash_size))



void hash_init();

void hash_unint();

int  hash_put(struct log_info *log_info_p);

int  hash_get(union ip_type ip, struct log_info *log_get);

void hash_list_all();


#endif//__STORE_H__
