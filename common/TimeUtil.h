#pragma once

#include <Windows.h>
#include <time.h>

namespace TimeUtil  
{
	BOOL TimeToWStr(LPWSTR wstr,time_t const timer);
	time_t GetCurTime();
};