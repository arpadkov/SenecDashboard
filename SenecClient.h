#pragma once

//#include <string>
#include "PowerState.h"

class SenecClient
{
private:
	std::string login_url;
	std::string systems_url;
	std::string client_path;
	std::string token;
	std::string battery_id;

	std::string getLoginData(std::string login_file);

public:
	SenecClient();

	bool Initialized;

	void setAuthToken(std::string login_file);
	void setBatteryId();

	PowerState getDashboardData();
	PowerState getTestDashboardData();
};



