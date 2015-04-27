#include <string>
#include <iostream>
#include <vector>
#include <curl/curl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mutli_curl.hpp"

using namespace std ;

string URL = "http://test812.sinaapp.com/index.php" ;
string PROXY = "" ;
unsigned int TIMEOUT=500000 ;

size_t curl_writer(void *buffer, size_t size, size_t count, void * stream)
{
    string str ;
    str.append((char *)buffer, size * count) ;
    cout<<str<<endl ;

    return size * count;
};

/**
 * 生成一个easy curl对象，进行一些简单的设置操作
 */
CURL * curl_easy_handler(const std::string & sUrl,
        const std::string & sProxy,
        unsigned int uiTimeout,
        int num=0)
{
    CURL * curl = curl_easy_init();
    char temp[20] ;
    memset(temp,0,20) ;
    sprintf(temp,"%d",num) ;

    string url = sUrl + "?aaa=" + temp;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

    if (uiTimeout > 0)
    {
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, uiTimeout);
    }
    if (!sProxy.empty())
    {
        curl_easy_setopt(curl, CURLOPT_PROXY, sProxy.c_str());
    }

    // write function //
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);

    return curl;
}

int main(int argc, char **argv)
{
    curl_global_init(CURL_GLOBAL_ALL);
    Multi_Curl multi_curl ;
    int handles = 1 ;

    if (argc < 2)
    {
        cout<<"input num"<<endl ;
        return 1 ;
    }

    int num = atoi(argv[1]);

    for(int i=0;i<num;++i)
    {
        CURL *curl = curl_easy_handler(URL,PROXY,TIMEOUT,i) ;
        multi_curl.push_handle(curl) ;
    }

    cout<<"add task OK"<<endl ;
    while(handles)
    {
        handles = multi_curl.run() ;
        sleep(1) ;
        /*if(handles <= 5)
        {
            cout<<"handles <=5 and task"<<endl ;
            CURL *curl = curl_easy_handler(URL,PROXY,TIMEOUT) ;
            multi_curl.push_handle(curl) ;
            CURL *curl2 = curl_easy_handler(URL,PROXY,TIMEOUT) ;
            multi_curl.push_handle(curl2) ;
        }*/
    }
    curl_global_cleanup() ;
    return 0 ;
}
