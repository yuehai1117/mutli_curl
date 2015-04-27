#include <iostream>
#include <curl/curl.h>
#include <string>
#include <vector>
#include "mutli_curl.hpp"

Multi_Curl::Multi_Curl()
{
    curl_m = curl_multi_init() ;
}

Multi_Curl::~Multi_Curl()
{
    curl_multi_cleanup(curl_m) ;
}

bool Multi_Curl::push_handle(CURL *curl)
{
    curl_multi_add_handle(curl_m,curl) ;
    int running_handles = 0 ;
    while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(curl_m, &running_handles)) ;
    return true ;
}

bool Multi_Curl::pop_handle(CURL *curl)
{
    curl_multi_remove_handle(curl_m,curl) ;
    curl_easy_cleanup(curl);
    return true ;
}


int Multi_Curl::run()
{
    int msgs_left;
    CURLMsg *msg ;
    int running_handles = 0 ;

    while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(curl_m,&running_handles)) ;
    while((msg = curl_multi_info_read(curl_m, &msgs_left)))
    {
        if (CURLMSG_DONE == msg->msg)
        {
            pop_handle(msg->easy_handle) ;
        }
    }
    return running_handles ;
}
