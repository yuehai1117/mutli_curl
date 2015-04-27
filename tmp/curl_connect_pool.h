#ifndef _CURL_CONNECT_POOL_H
#define _CURL_CONNECT_POOL_H

#include <curl/curl.h>
#include <curl/easy.h>

#ifdef __cplusplus
extern "C" {
#endif

	struct curl_connect_pool
	{
		size_t buffer_connect_count ;
		size_t max_connect_count ;
		size_t free_connect_count ;
		size_t used_connect_count ;
		CURL ** curl_free_connect ;
	} ;

	typedef struct curl_connect_pool curl_connect_pool_t ;

	int curl_connect_pool_init(size_t buffer_connect_count, size_t max_connect_count) ;
	void curl_connect_pool_cleanup() ;

	CURL * get_connection() ;

	void return_connection(CURL *curl) ;

#ifdef __cplusplus
}
#endif

#endif
