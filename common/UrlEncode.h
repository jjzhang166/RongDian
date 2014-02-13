#pragma once

namespace UrlEncode
{
	wchar_t* escape(const wchar_t* wStr);
	
	wchar_t* unescape(const wchar_t* wStr);

	char* url_encode(const char* utf8);

	char* url_unencode(const char* sourceText);
}