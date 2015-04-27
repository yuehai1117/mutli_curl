#include <string>
#include <queue>
#include <curl/curl.h>

use namespace std ;

class base_connect_pool
{
    public:
        virtual CURL * get_connect() = 0 ;
        virtual void return_connect(CURL *) = 0 ;
} ;

class curl_connect_pool : public base_connect_curl
{
public:
    curl_connect_pool(size_t max_count) ;
    ~curl_connect_pool() ;
    CURL *get_connect() ;
    void return_connect(CURL *curl) ;
    bool clean_connects(bool force=false) ;
private:
    size_t max_connects ;
    bool clean_pool ;
    pthread_mutex_t pool_mutex ;

    queue<CURL *> curl_free_connect ;
    queue<CURL *> curl_running_connect ;
} ;
