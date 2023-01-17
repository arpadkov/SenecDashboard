#include <fstream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include "SenecClient.h"
#include "PowerState.h"
#include "Exceptions.cpp"

using namespace std;
using json = nlohmann::json;

static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	// WriteCallback function, used by curl: CURLOPT_WRITEFUNCTION
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

std::string postRequest(const char* url, const char* data)
{
	// Sending a POST request with json data
	CURLcode res;
	CURL* curl;
	curl = curl_easy_init();

	std::string response;

	// curl setup
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");

	// Headers
	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	// Writing the data
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
	res = curl_easy_perform(curl);

	if (res != CURLE_OK) {
		throw MyException();
	}

	curl_easy_cleanup(curl);

	return response;
}

std::string getRequestWithAuth(const char* url, std::string authorization_token)
{
	// Sending a GET request with Authorization token in the header
	CURL* curl;
	CURLcode res;
	curl = curl_easy_init();

	std::string response;

	// curl setup
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");

	// Headers
	struct curl_slist* headers = NULL;
	std::string auth_string = "Authorization: " + authorization_token;   // Authorization header as string
	const char* auth_header = auth_string.c_str();                  // converted to const char* for curl
	headers = curl_slist_append(headers, auth_header);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	// Writing the data
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	res = curl_easy_perform(curl);

	curl_easy_cleanup(curl);

	return response;
}


SenecClient::SenecClient()
{
	login_url = "https://app-gateway-prod.senecops.com/v1/senec/login/";
	systems_url = "https://app-gateway-prod.senecops.com/v1/senec/anlagen/";

	// Setting path to login file
	char* buf = nullptr;
	size_t sz = 0;
	if (_dupenv_s(&buf, &sz, "AppData") == 0 && buf != nullptr)
	{
		client_path = buf;
		free(buf);
	}
	client_path += "\\SenecClient";
	//login_filename = "\\login_data.json";
}

void SenecClient::setAuthToken(string login_file)
{
	// Setting Authentication token
	string login_data = getLoginData(login_file);
	try
	{
		string authentication_response_string = postRequest(login_url.c_str(), login_data.c_str());
		json authentication_response = json::parse(authentication_response_string);
		token = authentication_response["token"];
	}
	catch (MyException& e)
	{
		string message = e.what();
	}


}

void SenecClient::setBatteryId()
{
	// Setting the battery id
	json systems_response = json::parse(getRequestWithAuth(systems_url.c_str(), token));
	battery_id = systems_response[0]["id"];          // Potentially multiple batteries available, selecting the first one
}

string SenecClient::getLoginData(string login_file)
{
	// TODO: include exception handling for login file not present/incorrect
	ifstream file(client_path + login_file);
	json jfile = json::parse(file);

	return jfile.dump();
}


PowerState SenecClient::getDashboardData()
{
	string dashboar_url = systems_url + battery_id + "/dashboard";
	json dashboard_response = json::parse(getRequestWithAuth(dashboar_url.c_str(), token));
	PowerState state = PowerState(dashboard_response);
	return state;
}

PowerState SenecClient::getTestDashboardData()
{
	// TODO: include exception handling for test response file not present/incorrect
	string test_path = client_path + "\\test\\test_dashboard_response.json";
	ifstream file(test_path);
	json jfile = json::parse(file);

	PowerState state = PowerState(jfile);

	return state;
}



