#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

#include "threadpool.h"
#include "condition.h"

void *thread_routine(void *arg)
{
	threadpool_t *pool = (threadpool_t*)arg;
	struct timespec ts;
	int timeout = 0;
	int ret;
	while ( 1 ) {
		condition_lock(&pool->ready);
		
		pool->idle++;
		while ( pool->first == NULL && !pool->quit ){
			printf("%X is waiting..\n", pthread_self());
			//condition_wait(&pool->ready);
			clock_gettime(CLOCK_REALTIME, &ts);
			ts.tv_sec += 2;
			ret = condition_timedwait(&pool->ready, &ts);
			if ( ret == ETIMEDOUT ) {
				printf("%X is wait timeout!\n", pthread_self());
				timeout = 1;
				break;
			}
		}
		pool->idle--;
	
		if ( pool->first != NULL ) {
			task_t *t = pool->first;
			pool->first = t->next;
			condition_unlock(&pool->ready);
			t->run(t->arg);
			free(t);
			condition_lock(&pool->ready);
		}

		if ( pool->quit && pool->first == NULL ) {
			pool->counter --;
			if ( pool->counter == 0 )
				condition_signal(&pool->ready);
			condition_unlock(&pool->ready);	
			break;	
		}
	
		if ( timeout && pool->first == NULL ) {
			condition_unlock(&pool->ready);	
			break;
		}

		condition_unlock(&pool->ready);
	}
	printf("%X is exiting!\n", pthread_self());
	return NULL;
}

void threadpool_init(threadpool_t *pool, int threads)
{
	condition_init(&pool->ready);
	pool->first   = NULL;
	pool->last    = NULL;
	pool->counter = 0;
	pool->idle    = 0;
	pool->quit    = 0;
	pool->max_threads = threads;
}

void threadpool_add_task(threadpool_t *pool, void *(*run)(void *arg), void *arg)
{
	task_t *newtask = (task_t*)malloc(sizeof(task_t));
	newtask->run  = run;
	newtask->arg  = arg;
	newtask->next = NULL;
	
	condition_lock(&pool->ready);
	
	if ( pool->first == NULL )
		pool->first = newtask;
	else
		pool->last->next = newtask;
	pool->last = newtask;
	
	if ( pool->idle > 0 )
		condition_signal(&pool->ready);
	else if ( pool->counter < pool->max_threads ) {
		pthread_t tid;
		pthread_create(&tid, NULL, thread_routine, pool);
		pool->counter++;
	}
	
	condition_unlock(&pool->ready);
}

void threadpool_destroy(threadpool_t *pool)
{
	if ( pool->quit ) 
		return ;
	condition_lock(&pool->ready);
	pool->quit = 1;
	if ( pool->counter > 0 ) {
		if ( pool->idle > 0 )
			condition_broadcast(&pool->ready);
		while ( pool->counter > 0 )
			condition_wait(&pool->ready);
	}
	condition_unlock(&pool->ready);
	condition_destroy(&pool->ready);
}

