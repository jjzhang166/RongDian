#include "TimeUtil.h"

BOOL TimeUtil::TimeToWStr(const time_t timer, LPWSTR wstr)
{
	struct tm *ltTime = localtime(&timer);
	wsprintf(wstr, L"%04d-%02d-%02d %02d:%02d:%02d", 
		ltTime->tm_year+1900, ltTime->tm_mon+1, ltTime->tm_mday, ltTime->tm_hour, ltTime->tm_min, ltTime->tm_sec);
	return TRUE;
}

time_t TimeUtil::GetCurTime() 
{
	time_t lt;
	lt = time(NULL);
	return lt;
}