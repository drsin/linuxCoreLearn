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
	FILE *fout = NULL;
	
	fout = fdopen(STDOUT_FILENO, "w");
	if ( fout == NULL )
		ERR_EXIT("fdopen");

	fputs("miss you so much, TC!\n", fout);
	
	return 0;
}

