#include <unistd.h>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>

#define ERR_EIXT(m) \
do { \
	fprintf(stderr, "[%s][%d]:%s\n", \
	__FILE__, __LINE__, strerror(m)); \
	exit(EXIT_FAILURE); \
}while(0)

#define CONSUMERS_COUNT 5
#define PRODUCERS_COUNT 1

pthread_mutex_t g_mutex;
pthread_cond_t  g_cond;

pthread_t g_thread[CONSUMERS_COUNT+PRODUCERS_COUNT];
int nready = 0;

void * cosume(void *arg) {
	int num = (int)arg;
	while ( 1 ) {
		pthread_mutex_lock(&g_mutex);
		while ( nready == 0 ) {
			printf("%d等待消费中...\n", num);
			pthread_cond_wait(&g_cond, &g_mutex);
		}
		printf("%d开始消费...\n", num);
		nready--;
		sleep(1);
		printf("%d消费完毕\n", num);
		pthread_mutex_unlock(&g_mutex);
	}
}

void * produce(void *arg) {
	int num = (int)arg;
	while ( 1 ) {
		pthread_mutex_lock(&g_mutex);
		printf("%d开始生产产品...\n", num);
		nready++;
		printf("%d生产完毕!\n", num);
		pthread_cond_signal(&g_cond);
		pthread_mutex_unlock(&g_mutex);
		sleep(3);
	}
}

int main( void )
{
	pthread_mutex_init(&g_mutex, NULL);
	pthread_cond_init(&g_cond, NULL);

	int i;
	for (i=0; i<CONSUMERS_COUNT; i++)
		pthread_create(&g_thread[i], NULL, cosume, (void*)i);
	for (i=0; i<PRODUCERS_COUNT; i++)
		pthread_create(&g_thread[CONSUMERS_COUNT+i], NULL, produce, (void*)i);
	
	for (i=0; i<CONSUMERS_COUNT+PRODUCERS_COUNT; i++)
		pthread_join(g_thread[i], (void**)0);
	pthread_cond_destroy(&g_cond);
	pthread_mutex_destroy(&g_mutex);
}

