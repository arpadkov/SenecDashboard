#pragma once

#include <string>
#include "PowerState.h"

class SenecClient
{
private:
	std::string login_filename;
	std::string login_url;
	std::string systems_url;
	std::string client_path;
	std::string token;
	std::string battery_id;

	std::string getLoginData();
	void setAuthToken();
	void setBatteryId();

	//static std::string postRequest(const char* url, const char* data);
	//static std::string getRequestWithAuth(const char* url, std::string authorization_token);
	//size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);

public:
	SenecClient();
	PowerState getDashboardData();
	PowerState getTestDashboardData();
	//std::string getClientPath();
	
};

