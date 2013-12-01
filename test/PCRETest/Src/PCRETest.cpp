// PCRETest.cpp : Defines the entry point for the console application.
//
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>

#define PCRE_STATIC // 静态库编译选项

#include <pcre\pcre.h>
#ifdef _DEBUG
#pragma comment(lib, "..\\..\\RongDian\\bin\\pcred.lib")
#else
#pragma comment(lib, "..\\..\\RongDian\\bin\\pcre.lib")
#endif

#include <StrUtil.h>

#define OVECCOUNT 30 /* should be a multiple of 3 */  
#define EBUFLEN 128  
#define BUFLEN 1024 

int _tmain()
{
	pcre *re;
	const char *error;
	int erroffset;
	int ovector[OVECCOUNT];
	int rc;
	//char src[] = "   <note date=\"08/08/2008\">  \n   <to>   aG  eorge   \n</to>\n<from>John</from></note>"; // 要被用来匹配的字符串
	char pattern[] = "([\\[\\{])\\s*|([^\\{\\[\\]\\},\\n]+,*)\\s*|([\\]\\}]\\s*,*)\\s*";

	//char src [] = "111 <title>Hello</title> 222<title>H3232d</title>";
	//char pattern [] = "<title>(\\w+)</title>";

	wchar_t *wsrc = L"{ \n \"people\":\n [\n{ \"firstName\": \"Brett\",\n \"lastName\":\"McLaughlin\", \n  \"email\": \"aaaa\" \n },\n { \"firstName\": \"Jason\", \"lastName\":\"Hunter\", \"email\": \"bbbb\"   \n  }, { \"firstName\": \"Elliotte\", \"lastName\":\"Harold\", \"email\": \"cccc\" } ]}";

	char* src = StrUtil::w2a(wsrc);

	printf("String : %s\n", src);
	printf("Pattern: \"%s\"\n", pattern);
	
	re = pcre_compile(pattern,	// pattern,输入参数，将要被编译的字符串形式的正则表达式
			PCRE_MULTILINE,					// options,输入参数，用来指定编译时的一些选项
			&error,				// errptr,输出参数，用来输出错误信息
			&erroffset,			// erroffset,输出参数，pattern中出错位置的偏移量
			NULL);				// tableptr,输入参数，用来指定字符表，一般情况用NULL
	// 返回值：被编译好的正则表达式的pcre内部表示结构
	if (re == NULL) // 如果编译失败，返回错误信息
	{
		printf("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return 1;
	}
	printf("\nOK, has matched ...\n\n"); // 没有出错，已经匹配
	unsigned int cur = 0;
	while(cur<strlen(src) && (rc = pcre_exec(re, NULL, src, strlen(src), cur, PCRE_NOTEMPTY, ovector, OVECCOUNT)) >= 0)
	{
		for(int j=1; j<rc; j++)
		{
			char *substring_start = src + ovector[2*j];
			int substring_length = ovector[2*j+1] - ovector[2*j];
			if (substring_length>0)
			{
				printf("%2d: %.*s=======\n", j, substring_length, substring_start);
			}
		}
		cur = ovector[1];
	}
	pcre_free(re); // 编译正则表达式re释放内存
	delete[] src;

	return 0;
}

