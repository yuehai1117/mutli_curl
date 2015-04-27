#include <iostream>
#include <curl/curl.h>
#include <string>
#include <vector>

class Multi_Curl
{
    public:
    Multi_Curl() ;
    ~Multi_Curl() ;

    bool push_handle(CURL *curl) ;
    bool pop_handle(CURL *curl) ;
    int run() ;

    private:
    CURLM *curl_m ;
} ;
