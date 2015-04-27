#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#include "curl_connect_pool.h"

curl_connect_pool_t curl_pool ;
static pthread_mutex_t pool_mut ;

int curl_connect_pool_init(size_t buffer_connect_count, size_t max_connect_count)
{
	curl_global_init(CURL_GLOBAL_ALL);
	pthread_mutex_init(&pool_mut, NULL);

	memset(&curl_pool,0,sizeof(curl_pool)) ;
	curl_pool.buffer_connect_count = buffer_connect_count ;
	curl_pool.max_connect_count = max_connect_count ;
	curl_pool.curl_free_connect = (CURL **)malloc(sizeof(CURL *) * buffer_connect_count) ;
	return 1 ;
}


void curl_connect_pool_cleanup()
{
	int i = 0 ;
	while(curl_pool.free_connect_count>0 || curl_pool.used_connect_count>0){
		pthread_mutex_lock(&pool_mut);
		for(i=0; i<curl_pool.free_connect_count; ++i){
			curl_easy_cleanup(curl_pool.curl_free_connect[i]) ;
		}
		curl_pool.free_connect_count = 0 ;
		pthread_mutex_unlock(&pool_mut);
		sleep(1) ;
	}
	
	pthread_mutex_destroy(&pool_mut) ;
	free(curl_pool.curl_free_connect) ;
	memset(&curl_pool,0,sizeof(curl_pool)) ;
	curl_global_cleanup() ;
}

CURL * get_connection()
{
	pthread_mutex_lock(&pool_mut);
	CURL * curl = NULL ;
	if(curl_pool.free_connect_count > 0)
		curl = curl_pool.curl_free_connect[--(curl_pool.free_connect_count)] ;
	else if (curl_pool.free_connect_count <=0 && curl_pool.used_connect_count <= curl_pool.max_connect_count)
		curl = curl_easy_init() ;
	else
		curl = NULL ;

	if(curl != NULL){
		(curl_pool.used_connect_count)++ ;
	}
	pthread_mutex_unlock(&pool_mut);
	return curl ;
}

void return_connection(CURL *curl)
{
	curl_easy_reset(curl) ;
	pthread_mutex_lock(&pool_mut);
	if(curl_pool.free_connect_count >= curl_pool.buffer_connect_count)
		curl_easy_cleanup(curl) ;
	else
		curl_pool.curl_free_connect[(curl_pool.free_connect_count)++] = curl ;
	curl_pool.used_connect_count-- ;
	pthread_mutex_unlock(&pool_mut);
}
