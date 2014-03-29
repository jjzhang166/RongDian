#include "StrUtil.h"

#pragma warning(push)
#pragma warning(disable:4996) // 消除_tcscat、_tcscpy、_tcsicmp等_tcs*函数的警告

#define OVECCOUNT 300 /* should be a multiple of 3 */

#include <Windows.h>
#pragma comment(lib, "kernel32.lib")

unsigned int StrUtil::a2w(const char * astr, wchar_t *wstr)
{
	int len = MultiByteToWideChar(CP_ACP, 0, astr, -1, NULL, 0);
	if (len==0)
		return 0;
	if(!wstr)
		return len;
	len = MultiByteToWideChar(CP_ACP, 0, astr, -1, wstr, len);

	return len;
}

unsigned int StrUtil::w2a(const wchar_t *wstr, char *astr)
{
	int len = WideCharToMultiByte(CP_OEMCP, NULL, wstr, -1, NULL, 0, NULL, FALSE);
	if (len==0)
		return 0;
	if(!astr)
		return len;
	len = WideCharToMultiByte (CP_OEMCP, NULL, wstr, -1, astr, len, NULL, FALSE);

	return len;
}

unsigned int StrUtil::a2u8(const char * astr, char *utf8)
{
	wchar_t *wstr = NULL;
	int len = MultiByteToWideChar(CP_ACP, 0, astr, -1, NULL, 0);
	if (len==0)
		return 0;
	wstr = new wchar_t[len];
	if(!wstr)
		return 0;
	len = MultiByteToWideChar(CP_ACP, 0, astr, -1, wstr, len);
	if (len==0)
	{
		delete[] wstr;
		return 0;
	}
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	if (len==0)
	{
		delete[] wstr;
		return 0;
	}
	if(!utf8)
	{
		delete[] wstr;
		return len;
	}
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8, len, NULL, NULL);
	delete[] wstr;

	return len;
}

unsigned int StrUtil::u82a(const char *utf8, char *astr)
{
	wchar_t *wstr = NULL;
	int len = MultiByteToWideChar (CP_UTF8, 0, utf8, -1, NULL, 0);
	wstr = new wchar_t[len];
	if(!wstr)
		return 0;
	len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	if (len==0)
	{
		delete[] wstr;
		return 0;
	}
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	if (len==0)
	{
		delete[] wstr;
		return 0;
	}
	if(!astr)
	{
		delete[] wstr;
		return len;
	}
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, astr, len, NULL, NULL);
	delete[] wstr;

	return len;
}

unsigned int StrUtil::u2u8(const wchar_t *wstr, char *utf8)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, FALSE);
	if (len==0)
		return 0;
	if(!utf8)
		return len;
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8, len, NULL, FALSE);
	return len;
}

char* StrUtil::u2u8(const wchar_t *wstr)
{
	int len = u2u8(wstr, NULL);
	char* utf8 = new char[len];
	if (!utf8)
		return NULL;
	int new_len = u2u8(wstr, utf8);
	if(len!=new_len)
	{
		delete[] utf8;
		utf8 = NULL;
	}
	return utf8;
}

unsigned int StrUtil::u82u(const char *utf8, wchar_t *wstr)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	if (len==0)
		return 0;
	if(!wstr)
		return len;
	len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	return len;
}

wchar_t* StrUtil::a2w(const char *astr)
{
	int len = a2w(astr, NULL);
	wchar_t *wstr = new wchar_t[len];
	if(!wstr)
		return NULL;
	int new_len = a2w(astr, wstr);
	if(len!=new_len)
	{
		delete[] wstr;
		wstr = NULL;
	}
	return wstr;
}

char* StrUtil::w2a(const wchar_t *wstr)
{
	int len = w2a(wstr, NULL);
	char *astr = new char[len];
	if(!astr)
		return NULL;
	int new_len = w2a(wstr, astr);
	if(len!=new_len)
	{
		delete[] astr;
		astr = NULL;
	}
	return astr;
}

char* StrUtil::a2u8(const char *astr)
{
	int len = a2u8(astr, NULL);
	char *utf8 = new char[len];
	if(!utf8)
		return NULL;
	int new_len = a2u8(astr, utf8);
	if(len!=new_len)
	{
		delete[] utf8;
		utf8 = NULL;
	}
	return utf8;
}

char* StrUtil::u82a(const char *utf8)
{
	int len = u82a(utf8, NULL);
	char *astr = new char[len];
	if(!astr)
		return NULL;
	int new_len = u82a(utf8, astr);
	if(len!=new_len)
	{
		delete[] astr;
		astr = NULL;
	}
	return astr;
}

wchar_t* StrUtil::u82u(const char *utf8)
{
	int len = u82u(utf8, NULL);
	wchar_t *wstr = new wchar_t[len];
	if(!wstr)
		return NULL;
	int new_len = u82u(utf8, wstr);
	if(len!=new_len)
	{
		delete[] wstr;
		wstr = NULL;
	}
	return wstr;
}

bool StrUtil::is_special_utf8(unsigned char c)
{
	unsigned special_byte = 0X02; // binary 00000010
	if ((unsigned char)(c >> 6) == special_byte)
		return true;
	else
		return false;
}

int StrUtil::is_utf8(const char *str)
{
	unsigned char utf8WithBOMTxt[] = {0xEF, 0xBB, 0xBF}; // UTF_8 with BOM file header
	if(memcmp(utf8WithBOMTxt, str, 3)==0)
		return UTF8_BOM;

	unsigned one_byte = 0X00; // binary 00000000
	unsigned two_byte = 0X06; // binary 00000110
	unsigned three_byte = 0X0E; // binary 00001110
	unsigned four_byte = 0X1E; // binary 00011110
	unsigned five_byte = 0X3E; // binary 00111110
	unsigned six_byte = 0X7E; // binary 01111110
	int utf8_yes = 0;
	int utf8_no = 0;

	unsigned char k = 0;
	unsigned char m = 0;
	unsigned char n = 0;
	unsigned char p = 0;
	unsigned char q = 0;

	unsigned char c = 0;
	for (unsigned int i=0; i<strlen(str);)
	{
		c = (unsigned char)str[i];
		if ((unsigned)(c>>7) == one_byte)
		{
			i++;
			continue;
		}
		else if ((unsigned)(c>>5) == two_byte)
		{
			k = (unsigned char)str[i+1];
			if (is_special_utf8(k))
			{
				utf8_yes++;
				i += 2;
				continue;
			}
		}
		else if ((unsigned)(c>>4) == three_byte)
		{
			m = (unsigned char)str[i+1];
			n = (unsigned char)str[i+2];
			if (is_special_utf8(m) && is_special_utf8(n))
			{
				utf8_yes++;
				i += 3;
				continue;
			}
		}
		else if ((unsigned)(c>>3) == four_byte)
		{
			k = (unsigned char)str[i+1];
			m = (unsigned char)str[i+2];
			n = (unsigned char)str[i+3];
			if (is_special_utf8(k) && is_special_utf8(m)
				&& is_special_utf8(n))
			{
				utf8_yes++;
				i += 4;
				continue;
			}
		}
		else if ((unsigned)(c>>2) == five_byte)
		{
			unsigned char k = (unsigned char)str[i+1];
			unsigned char m = (unsigned char)str[i+2];
			unsigned char n = (unsigned char)str[i+3];
			unsigned char p = (unsigned char)str[i+4];
			if (is_special_utf8(k) && is_special_utf8(m)
				&& is_special_utf8(n) && is_special_utf8(p))
			{
				utf8_yes++;
				i += 5;
				continue;
			}
		}
		else if ((unsigned)(c>>1) == six_byte)
		{
			k = (unsigned char)str[i+1];
			m = (unsigned char)str[i+2];
			n = (unsigned char)str[i+3];
			p = (unsigned char)str[i+4];
			q = (unsigned char)str[i+5];
			if (is_special_utf8(k) && is_special_utf8(m)
				&& is_special_utf8(n) && is_special_utf8(p)
				&& is_special_utf8(q))
			{
				utf8_yes++;
				i += 6;
				continue;
			}
		}
		utf8_no++;
		i++;
	}
	//printf("%d %d\n", utf8_yes, utf8_no);
	//cout << utf8_yes << "," << utf8_no;
	if((utf8_yes + utf8_no)==0) // 当文件全部为英文时，文件中的字符编码与ascii编码一致，都为一个字节，故并不能判断是utf8或是unicode big endian或是unicode little endian
		return UTF8_ACSII;
	int ret = (100*utf8_yes)/(utf8_yes + utf8_no);
	if (ret > 90)
		return UTF8_NOBOM;
	else
		return UTF8_INVALID;
}

bool StrUtil::is_gb2312(const char *str)
{
	unsigned one_byte = 0X00; // binary 00000000
	int gb2312_yes = 0;
	int gb2312_no = 0;
	unsigned char k = 0;
	unsigned char c = 0;
	for (unsigned int i=0; i<strlen(str);)
	{
		c = (unsigned char)str[i];
		if ((unsigned)(c>>7) == one_byte) // 遇到ascii码即返回
		{
			i++;
			continue;
		}
		else if (c >= 0XA1 && c <= 0XF7) // 汉字区间
		{
			k = (unsigned char)str[i+1];
			if (k >= 0XA1 && k <= 0XFE)
			{
				gb2312_yes++;
				i += 2;
				continue;
			}
		}
		//cout<<gb2312_no<<","<<gb2312_no<<endl;
		gb2312_no++;
		i += 2;
	}

	//printf("%d %d\n", gb2312_yes, gb2312_no);
	int ret = 0;
	if((gb2312_no+gb2312_yes)!=0) // 当文件中没有汉字时，全都是ascii码时，两者都为0
		ret = (100*gb2312_yes) / (gb2312_yes+gb2312_no);
	else
		ret=0;
	//cout << "ret:" << ret << endl;
	if (ret > 90)
		return true;
	else
		return false;
}

bool StrUtil::is_big5(const char *str)
{
	unsigned one_byte = 0X00; // binary 00000000
	int big5_yes = 0;
	int big5_no = 0;
	unsigned char k = 0;
	unsigned char c = 0;
	for (unsigned int i=0; i<strlen(str);)
	{
		c = (unsigned char)str[i];
		if ((unsigned)(c>>7) == one_byte) // 遇到ascii码时
		{
			i++;
			continue;
		}
		else if (c >= 0XA1 && c <= 0XF9) // big5的字节区间
		{
			k = (unsigned char)str[i+1];
			if ( k >= 0X40 && k <= 0X7E || k >= 0XA1 && k <= 0XFE)
			{
				big5_yes++;
				i += 2;
				continue;
			}
		}
		big5_no++;
		i += 2;
	}
	//printf("%d %d\n", big5_yes, big5_no);
	if((big5_yes+big5_no)==0) // 如果全部是ascii码时。无法判断编码
		return false;
	int ret = (100*big5_yes)/(big5_yes+big5_no);
	if (ret > 90)
		return true;
	else
		return false;
}

bool StrUtil::is_gbk(const char *str)
{
	unsigned one_byte = 0X00; // binary 00000000
	int gbk_yes = 0;
	int gbk_no = 0;
	unsigned char k = 0;
	unsigned char c = 0;
	for (unsigned int i=0; i<strlen(str);)
	{
		c = (unsigned char)str[i];
		if ((unsigned)(c>>7) == one_byte)
		{
			i++;
			continue;
		}
		else if (c >= 0X81 && c <= 0XFE)
		{
			k = (unsigned char)str[i+1];
			if (k >= 0X40 && k <= 0XFE)
			{
				gbk_yes++;
				i += 2;
				continue;
			}
		}
		gbk_no++;
		i += 2;
	}
	//printf("%d %d\n", gbk_yes, gbk_no);
	if ((gbk_yes+gbk_no)==0)
		return false;
	int ret = (100*gbk_yes)/(gbk_yes+gbk_no);
	if (ret > 90)
		return true;
	else
		return false;
}

int StrUtil::wcstrim(wchar_t *str, int len)
{
	int nStrLen = (int)wcslen(str);
	if (nStrLen == 0)
		return 0;

	bool bSige = true;
	for (int i = nStrLen - 1; (bSige) && (i >= 0); i--) // 去掉后面的空格
	{
		if (str[i] == L' ')
			str[i] = L'\0';
		else
			bSige = false;
	}
	nStrLen = (int)wcslen(str);
	if (nStrLen == 0)
		return 0;

	int nSpaceCount = 0;
	bSige = true;
	int i = 0;
	do {   // 查找前面的空格的个数
		if (str[i] == L' ')
		{
			nSpaceCount ++;
			i++;
		}
		else
		{
			bSige = false;
		}
	} while (bSige);

	if (nSpaceCount == 0)
		return nStrLen;

	int nMoveCount = nStrLen - nSpaceCount;
	for (int i = 0; i < nMoveCount; i++) // 向前移动
		str[i] = str[i+nSpaceCount];

	for(int j=nMoveCount; j<len; j++) // 后面的清零
		str[j] = '\0';

	return (int)wcslen(str);
}

int StrUtil::strtrim(char *str, int len)
{
	int nStrLen = (int)strlen(str);
	if (nStrLen == 0)
		return 0;

	bool bSige = true;
	for (int i = nStrLen - 1; (bSige) && (i >= 0); i--) // 去掉后面的空格
	{
		if (str[i] == ' ')
			str[i] = '\0';
		else
			bSige = false;
	}
	nStrLen = (int)strlen(str);
	if (nStrLen == 0)
		return 0;

	int nSpaceCount = 0;
	bSige = true;
	int i = 0;
	do {   // 查找前面的空格的个数
		if (str[i] == ' ')
		{
			nSpaceCount ++;
			i++;
		}
		else
		{
			bSige = false;
		}
	} while (bSige);

	if (nSpaceCount == 0)
		return nStrLen;

	int nMoveCount = nStrLen - nSpaceCount;
	for (int i = 0; i < nMoveCount; i++) // 向前移动
		str[i] = str[i+nSpaceCount];

	for(int j=nMoveCount; j<len; j++) // 后面的清零
		str[j] = '\0';

	return (int)strlen(str);
}

bool StrUtil::is_wsubstr(const wchar_t* wStr,const wchar_t* wSubStr)
{
	bool f = false;
	char* str = w2a(wStr);
	char* str2 = w2a(wSubStr);
	if (strstr(str,str2)!=NULL)
	{
		f = true;
	}
	delete[] str;
	delete[] str2;
	return f;
}

bool StrUtil::replace_all(char *str, const char old_value, const char new_value)
{
	for(int i=0;i<strlen(str);i++)
	{
		if (str[i]==old_value)
		{
			str[i] = new_value;
		}
	}   
	return true;
}

int StrUtil::count_char(const wchar_t *str, const char ch)
{
	int count = 0;
	for(int i=0;i<wcslen(str);i++)
	{
		if(str[i]==ch)
		{
			count++;
		}
	}
	return count;
}

#pragma warning(pop)
