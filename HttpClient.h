#pragma once
#include <string>

class HttpClient
{
private:
	static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);

public:
	static std::string postRequest(const char* url, const char* data);
	static std::string getRequestWithAuth(const char* url, std::string authorization_token);
};

