#include <Windows.h>
#include <iostream>
#include "curl/curl.h"
using namespace std;

#pragma comment(lib, "ws2_32.lib")
#ifdef _DEBUG
#pragma comment(lib, "../../RongDian/bin/libcurld.lib")
#else
#pragma comment(lib, "../../RongDian/bin/libcurl.lib")
#endif

size_t callbackFunc(char *ptr, size_t size, size_t nmemb, void *userdata)
 {
	//char* buffer = (char*)userp;
    strcat((char*)userdata,ptr);
    printf("%d\n", nmemb);
    printf("=========================\n");
    return size * nmemb;     //必须返回这个大小, 否则只回调一次
 }


/**
* 阻塞调用
*/
 void singleThreadFunc()
 {
    CURL *curl = curl_easy_init();
    
    curl_easy_setopt(curl, CURLOPT_URL, "www.baidu.com"); //设置下载的URI
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20);        //设置超时
    //curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);        //屏蔽其它信号
    //curl_easy_setopt(curl, CURLOPT_HEADER, 1);          //下载数据包括HTTP头部
    curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
    
    char buffer[57000] = {0x0};
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackFunc); //设置下载数据的回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);  

    curl_easy_perform(curl);
    //double length = 0;
    //CURLcode code = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD , &length); 
    printf("%d\n", strlen(buffer));
    printf("%s",buffer);
    curl_easy_cleanup(curl);
 }

/**
* 多线程
*/
void multiThreadFunc()
{
	
	CURLM *multi_handle = curl_multi_init();

	//设置easy handle
	CURL *easy_handle = curl_easy_init();
	curl_easy_setopt(easy_handle, CURLOPT_URL, "www.rongdian.net/rdol.php?type=3");
	curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 20);        //设置超时
	curl_easy_setopt(easy_handle, CURLOPT_HEADER, 1);          //下载数据包括HTTP头部

	char buffer[5000] = {0};
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, callbackFunc);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, buffer);

	// 添加到multi stack
	curl_multi_add_handle(multi_handle, easy_handle);

	int running_handle_count;
	while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multi_handle, &running_handle_count))
	{
		cout << running_handle_count << endl;
	}
	while (running_handle_count)
	{
		timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		int max_fd;
		fd_set fd_read;
		fd_set fd_write;
		fd_set fd_except;

		FD_ZERO(&fd_read);
		FD_ZERO(&fd_write);
		FD_ZERO(&fd_except);

		curl_multi_fdset(multi_handle, &fd_read, &fd_write, &fd_except, &max_fd);
		int return_code = select(max_fd + 1, &fd_read, &fd_write, &fd_except, &tv);
		if (SOCKET_ERROR == return_code)
		{
			cerr << "select error." << endl;
			break;
		}
		else
		{
			while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multi_handle, &running_handle_count))
			{
				cout << running_handle_count << endl;
			}
		}
	}

	curl_easy_cleanup(easy_handle);
	curl_multi_cleanup(multi_handle);
}

void main()
{
	curl_global_init(CURL_GLOBAL_WIN32);
	singleThreadFunc();
	//multiThreadFunc();
	curl_global_cleanup();
}

