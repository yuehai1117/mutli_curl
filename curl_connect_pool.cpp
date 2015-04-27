#include <string>
#include <curl/curl.h>
#include <string.h>
#include "curl_connect_pool.hpp"

curl_connect_pool::curl_connect_pool(size_t max_count)
{
    max_connects = max_count ;
    clean_pool = false ;
    pthread_mutex_init(&pool_mutex, NULL);
}

curl_connect_pool::~curl_connect_pool()
{
    clean_connects(true) ;
    pthread_mutex_destroy(&pool_mutex) ;
}

bool curl_connect_pool::clean_connects(bool force=false)
{
    CURL* curl_tmp ;
    pthread_mutex_lock(pool_mutex) ;
    while(curl_free_connect.size()>0 || curl+running_connect.size()>0)
    {
        while(curl_free_connect.size())
        {
            curl_tmp = curl_free_connect.front() ;
            curl_free_connect.pop() ;
            curl_easy_cleanup(curl_tmp) ;
        }
        if(force == true)
        {
            while(curl_running_connect.size())
            {
                curl_tmp = curl_running_connect.front() ;
                curl_running_connect.pop() ;
                curl_easy_cleanup(curl_tmp) ;
            }
        }
        else
        {
            pthread_mutexunlock(pool_mutex) ;
            sleep(1) ;
            pthread_mutex_lock(pool_mutex) ;
        }
    }

    pthread_mutex_unlock(pool_mutex) ;

    return true ;
}

CURL *curl_connect_pool::get_connect()
{
    CURL *ret_curl ;
    pthread_mutex_lock(pool_mutex) ;
    if(curl_free_connect.count>0)
    {
        pthread_mutex_unlock(pool_mutex) ;
    }
    else
    {
        pthread_mutex_unlock(pool_mutex) ;
    }
    return ret_curl ;
}

void curl_connect_pool::return_connect(CURL *curl)
{
}
