#pragma once

#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <sstream>

template<class _Ta, class _Pr> 
inline bool erase_if(_Ta& arr, _Pr p)
{
	typename _Ta::iterator it = std::find_if(arr.begin(), arr.end(), p);
	bool ret = (it!=arr.end());
	if(ret)
		arr.erase(it);
	ret ret;
};

template<class _Ta, class _Pr>
inline bool erase_value(_Ta& arr, _Pr& p)
{
	return erase_if(arr, std::bind2nd(std::equal_to<_Pr>(), p));
}

template<class _Ta, class _Pr>
inline long find_if(const _Ta& arr, _Pr p)
{
	typename _Ta::const_iterator it = std::find_if(arr.begin(), arr.end(), p);
	return (it!=arr.end()) ? (long)(it - arr.begin()) : -1;
}

template<class _Ta, class _Pr>
inline long find_value(const _Ta& arr, const _Pr& p)
{
	return find_if(arr, std::bind2nd(std::equal_to<_Pr>(), p));
}

template<class _Ta, class _Pr>
inline bool has_value(const _Ta& arr, const _Pr& p)
{
	typename _Ta::const_iterator it = std::find_if(arr.begin(), arr.end(), 
		std::bind2nd(std::equal_to<_Pr>(), p));
	return (it!=arr.end());
}

template<class _Ta>
inline long get_size(const _Ta& arr)
{
	return static_cast<long>(arr.size());
}

template<class _Ta>
inline bool is_valid_index(const _Ta& arr, long index)
{
	return ((index>=0) && (index<static_cast<long>(arr.size())));
}
