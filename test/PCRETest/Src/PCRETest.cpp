// PCRETest.cpp : Defines the entry point for the console application.
//
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>

#define PCRE_STATIC // ��̬�����ѡ��

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
	//char src[] = "   <note date=\"08/08/2008\">  \n   <to>   aG  eorge   \n</to>\n<from>John</from></note>"; // Ҫ������ƥ����ַ���
	char pattern[] = "([\\[\\{])\\s*|([^\\{\\[\\]\\},\\n]+,*)\\s*|([\\]\\}]\\s*,*)\\s*";

	//char src [] = "111 <title>Hello</title> 222<title>H3232d</title>";
	//char pattern [] = "<title>(\\w+)</title>";

	wchar_t *wsrc = L"{ \n \"people\":\n [\n{ \"firstName\": \"Brett\",\n \"lastName\":\"McLaughlin\", \n  \"email\": \"aaaa\" \n },\n { \"firstName\": \"Jason\", \"lastName\":\"Hunter\", \"email\": \"bbbb\"   \n  }, { \"firstName\": \"Elliotte\", \"lastName\":\"Harold\", \"email\": \"cccc\" } ]}";

	char* src = StrUtil::w2a(wsrc);

	printf("String : %s\n", src);
	printf("Pattern: \"%s\"\n", pattern);
	
	re = pcre_compile(pattern,	// pattern,�����������Ҫ��������ַ�����ʽ��������ʽ
			PCRE_MULTILINE,					// options,�������������ָ������ʱ��һЩѡ��
			&error,				// errptr,����������������������Ϣ
			&erroffset,			// erroffset,���������pattern�г���λ�õ�ƫ����
			NULL);				// tableptr,�������������ָ���ַ���һ�������NULL
	// ����ֵ��������õ�������ʽ��pcre�ڲ���ʾ�ṹ
	if (re == NULL) // �������ʧ�ܣ����ش�����Ϣ
	{
		printf("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return 1;
	}
	printf("\nOK, has matched ...\n\n"); // û�г����Ѿ�ƥ��
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
	pcre_free(re); // ����������ʽre�ͷ��ڴ�
	delete[] src;

	return 0;
}

