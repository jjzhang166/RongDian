#pragma once

#include <Windows.h>
#include <time.h>

namespace TimeUtil  
{
	BOOL TimeToWStr(const time_t timer, LPWSTR wstr); 
	time_t GetCurTime();
};