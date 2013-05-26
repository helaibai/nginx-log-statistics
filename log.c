#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <error.h>
#include <fcntl.h>

#include "debug.h"
#include "store.h"
#include "strnchar.h"
#include "types.h"

#define _XOPEN_SOURCE 500

/* configure */

#define PTHREAD_NUMBER 	4

#define PTHREAD_BUF_SIZE 4096*2

#define BUF_SIZE 	512
#define LOG_PATH_SIZE 	128



struct log_part_t{
	off_t log_start;	
	off_t log_end;
	off_t log_cur_pos;

};
void *log_pthread(void *arg);

int buf_log(char *buf, int len);

void useg(char *cmd);

int one_log(char *log_start);

struct log_part_t log_arry[PTHREAD_NUMBER];

off_t up(char *log_buf, int buf_len);

off_t down(char *log_buf, int buf_len);

int log_fd;

int main(int argc, char **argv)
{
	char log_path[LOG_PATH_SIZE];
	u64 i;
	off_t log_size = 0;
	off_t log_start = 0;
	off_t log_end = 0;
	struct stat log_stat;
	off_t log_div_step;
	pthread_t pid_a[PTHREAD_NUMBER];
	off_t log_cur_pos = 0;
	off_t seek_ret;
	char buf[BUF_SIZE];
	if(argc != 2)
	{
		useg(argv[0]);
		return -1;
	}
	hash_init();

	/* DIV LOG  */
	strncpy(log_path, argv[1], LOG_PATH_SIZE);

	log_fd = open(log_path, O_RDONLY, 0666);
	if(log_fd < 0)
	{
		perror("open()");
		return -1;
	}
	
	if(fstat(log_fd, &log_stat) < 0)
	{
		perror("fstat()");
		return -1;
	}

	log_size = log_stat.st_size;
	if(log_size <= BUF_SIZE)
	{
		printf("file too small,do it by hand !\n");
		return 0;
	}

	//add align by '\n' for log src file

	//start align
	
	log_start = read(log_fd, buf, BUF_SIZE);
	log_start = up(buf, log_start); 
	log_cur_pos = log_start;
	
	DBG_INFO("log start %ld\n",log_start);
	//end align
	
	lseek(log_fd, (log_size - BUF_SIZE), SEEK_SET);
	log_end = read(log_fd, buf, BUF_SIZE);
	log_end = (log_size - BUF_SIZE) + down(buf, log_end);
	DBG_INFO("log end %ld\n",log_end);
	
	//DIV FOR EVERY thread 
	log_div_step = (log_end - log_start)/(PTHREAD_NUMBER);

	for(i=0; i<PTHREAD_NUMBER; ++i)
	{
		log_arry[i].log_start = log_cur_pos;
		log_cur_pos += log_div_step;
		seek_ret = lseek(log_fd, log_cur_pos, SEEK_SET);
		if(seek_ret == (off_t)(-1))
		{
		//do something	
		}	
		if(read(log_fd, buf, BUF_SIZE)< BUF_SIZE)
		{
			log_arry[i].log_end = log_end;
			break;
		}
		log_cur_pos += up(buf, BUF_SIZE);
		log_arry[i].log_end = log_cur_pos;
		log_cur_pos++;
		
	}
	
	/* debug infomation */
	for(i=0; i<PTHREAD_NUMBER; ++i)
	{
		DBG_INFO("log_part[%d] : %ld -- %ld \n",i,log_arry[i].log_start,log_arry[i].log_end);
	}
	/* LOG START */
	for(i=0; i<PTHREAD_NUMBER; ++i)
	{
		if(pthread_create(&pid_a[i], NULL, log_pthread, (void *)i) != 0)
		{
			perror("pthread_create");
		}
	}
	
	//printf("waiting for a while ...\n");
	
	/* LOG END */
	for(i=0; i<PTHREAD_NUMBER; ++i)
	{
		pthread_join(pid_a[i], NULL);	
	}
	hash_list_all();
	hash_unint();
	close(log_fd);	
	return 0;	
}
void useg(char *cmd)
{
	printf("Useg:%s <log path>\n",cmd);	
}
/*
pread() reads  up to count bytes from file descriptor fd at 
	offset offset (from the start of the file) into the buffer
        starting at buf.  The file offset is not changed.
*/
#define MIN_IP_LEN 8
#define MINI(a,b) ((a)>(b)?(b):(a))

int one_log(char *log_start)
{
	const char *p = log_start;
	
	p +=MIN_IP_LEN;
	
	while(*p++ != ' ');
	
	log_store(log_start,(p-log_start));

	//here may be a BUG
	p = find_char_c(p, '\n') + 1;
	
	return (p - log_start);//return 	
}
int buf_log(char *buf, int len)
{
	char *p = buf;	
	char *q = (buf+len);
	while(p <= q)
	{
		p += one_log(p);
	}

	return (p-buf);
}
void *log_pthread(void *arg)
{
	u64 no = (u64)arg;
	char pthread_buf[PTHREAD_BUF_SIZE];
	ssize_t read_len = 0;
	
	log_arry[no].log_cur_pos = log_arry[no].log_start;
	
	while(log_arry[no].log_cur_pos <= log_arry[no].log_end)
	{
		read_len = MINI(PTHREAD_BUF_SIZE, (log_arry[no].log_end - log_arry[no].log_cur_pos));
		read_len = pread(log_fd, pthread_buf, read_len, log_arry[no].log_cur_pos);
		read_len = down(pthread_buf, read_len);
		log_arry[no].log_cur_pos += buf_log(pthread_buf, read_len);
	}
	DBG_INFO("pthread[%d] over\n",no);
	pthread_exit(NULL);
	return (void *)NULL;
}
off_t down(char *log_buf, int buf_len)
{
	char *p = (log_buf+buf_len);

	while(*p-- != '\n');
	
	return (p-log_buf);
}
off_t up(char *log_buf, int buf_len)
{
	char *p = log_buf;
	while(*p++ != '\n' && buf_len--);
	if(buf_len == 0)
	{
		return (off_t)0;	
	}
	return (p - log_buf);
}
