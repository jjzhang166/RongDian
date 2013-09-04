#include "TimeUtil.h"

BOOL TimeUtil::TimeToWStr(LPWSTR wstr,time_t const timer)
{
	struct tm *l=localtime(&timer);
	wsprintf(wstr,L"%04d-%02d-%02d %02d:%02d:%02d",l->tm_year+1900,l->tm_mon+1,l->tm_mday,l->tm_hour,l->tm_min,l->tm_sec);
	return TRUE;
}

time_t TimeUtil::GetCurTime()
{
	time_t lt;
	lt =time(NULL);
	return lt;
}