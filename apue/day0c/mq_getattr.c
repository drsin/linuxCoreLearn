#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define ERR_EXIT(m) \
	do { \
		fprintf(stderr, "[%s][%d]:%s %s\n", __FILE__, __LINE__, m, strerror(errno)); \
		exit(EXIT_FAILURE); \
	} while ( 0 )

int main( void )
{
	mqd_t mqd;
	mqd=mq_open("/abc", O_RDWR);
	if ( mqd == (mqd_t)-1)
		ERR_EXIT("mq_open");
	
	struct mq_attr attr;
	if ( mq_getattr(mqd, &attr) == -1)
		ERR_EXIT("mq_getattr");
	
	printf("mq_maxmsg=%d, mq_msgsize=%d, mq_curmsgs=%d\n",
		attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
	
	mq_close(mqd);
	return 0;
}

