#include <Windows.h>
#include <string>
#include <iostream>

using namespace std;

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "wldap32.lib")

#define CURL_STATICLIB
#define BUILDING_LIBCURL
#define HTTP_ONLY

#include <curl/curl.h>

#ifdef _DEBUG
#pragma comment(lib, "../../RongDian/curld.lib")
#else
#pragma comment(lib, "../../RongDian/curl.lib")
#endif

size_t callbackFunc(char *ptr, size_t size, size_t nmemb, void *userdata)
 {
	//char* buffer = (char*)userp;
    strcat((char*)userdata,ptr);
    printf("%d\n", nmemb);
    printf("=========================\n");
    return size * nmemb;     //���뷵�������С, ����ֻ�ص�һ��
 }


/**
* ��������
*/
 void singleThreadFunc()
 {
    CURL *curl = curl_easy_init();
    
    curl_easy_setopt(curl, CURLOPT_URL, "www.baidu.com"); //�������ص�URI
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20);        //���ó�ʱ
    //curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);        //���������ź�
    //curl_easy_setopt(curl, CURLOPT_HEADER, 1);          //�������ݰ���HTTPͷ��
    curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
    
    char buffer[57000] = {0x0};
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackFunc); //�����������ݵĻص�����
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);  

    curl_easy_perform(curl);
    //double length = 0;
    //CURLcode code = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD , &length); 
    printf("%d\n", strlen(buffer));
    printf("%s",buffer);
    curl_easy_cleanup(curl);
 }

/**
* ���߳�
*/
void multiThreadFunc()
{
	
	CURLM *multi_handle = curl_multi_init();

	//����easy handle
	CURL *easy_handle = curl_easy_init();
	curl_easy_setopt(easy_handle, CURLOPT_URL, "www.rongdian.net/rdol.php?type=3");
	curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 20);        //���ó�ʱ
	curl_easy_setopt(easy_handle, CURLOPT_HEADER, 1);          //�������ݰ���HTTPͷ��

	char buffer[5000] = {0};
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, callbackFunc);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, buffer);

	// ��ӵ�multi stack
	curl_multi_add_handle(multi_handle, easy_handle);

	int running_handle_count;
	while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multi_handle, &running_handle_count))
	{
		std::cout << running_handle_count << std::endl;
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
			std::cerr << "select error." << std::endl;
			break;
		}
		else
		{
			while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multi_handle, &running_handle_count))
			{
				std::cout << running_handle_count << std::endl;
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

