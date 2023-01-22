#include "SenecClient.h"
#include "HttpClient.h"
#include "PowerState.h"
#include "Exceptions.h"
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <fstream>

using string = std::string;
using json = nlohmann::json;

SenecClient::SenecClient()
{
	Initialized = false;

	login_url = "https://app-gateway-prod.senecops.com/v1/senec/login/";
	systems_url = "https://app-gateway-prod.senecops.com/v1/senec/anlagen/";

	// Setting path to client folder
	char* buf = nullptr;
	size_t sz = 0;
	if (_dupenv_s(&buf, &sz, "AppData") == 0 && buf != nullptr)
	{
		client_path = buf;
		free(buf);
	}
	client_path += "\\SenecClient";
}

void SenecClient::setAuthToken(string login_file)
{
	// Setting Authentication token
	string login_data = getLoginData(login_file);
	string authentication_response_string;

	authentication_response_string = HttpClient::postRequest(login_url.c_str(), login_data.c_str());

	if (authentication_response_string != "")
	{
		// Got valid response for authorization
		json authentication_response = json::parse(authentication_response_string);
		token = authentication_response["token"];
		Initialized = true;
	}
	else
	{
		throw InvalidCredentialsException();
	}
}

void SenecClient::setBatteryId()
{
	// Setting the battery id, neccessary to get Dashboard response
	json systems_response = json::parse(HttpClient::getRequestWithAuth(systems_url.c_str(), token));
	battery_id = systems_response[0]["id"];          // Potentially multiple batteries available, selecting the first one
}

string SenecClient::getLoginData(string login_file)
{
	std::ifstream file(client_path + login_file);
	if (!file)
	{
		throw LoginFileNotFoundException();		
	}
	
	json jfile;
	try
	{
		jfile = json::parse(file);
	}
	catch (json::parse_error parseError)
	{
		throw;
	}

	return jfile.dump();
}


PowerState SenecClient::getDashboardData()
{
	string dashboar_url = systems_url + battery_id + "/dashboard";
	json dashboard_response = json::parse(HttpClient::getRequestWithAuth(dashboar_url.c_str(), token));
	PowerState state = PowerState(dashboard_response);
	return state;
}

PowerState SenecClient::getTestDashboardData()
{
	// TODO: include exception handling for test response file not present/incorrect
	string test_path = client_path + "\\test\\test_dashboard_response.json";
	std::ifstream file(test_path);

	json jfile = json::parse(file);	

	PowerState state = PowerState(jfile);

	return state;
}




