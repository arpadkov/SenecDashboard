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

public:
	SenecClient();
	PowerState getDashboardData();
	PowerState getTestDashboardData();
	//std::string getClientPath();
	
};

