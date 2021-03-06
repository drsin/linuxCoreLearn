#include <unistd.h>
#include <fcntl.h>

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
	char *envp[] = {"hehe=haha", NULL};
	if ( execle("/bin/ls", "ls", "-l", NULL, envp) == -1)
		ERR_EXIT("execlp");

	return 0;
}

