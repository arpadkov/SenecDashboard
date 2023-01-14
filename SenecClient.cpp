#include <fstream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include "SenecClient.h"
#include "PowerState.h"
//#include "HttpRequest.cpp"

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

	string response;

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

	curl_easy_cleanup(curl);

	return response;
}

string getRequestWithAuth(const char* url, string authorization_token)
{
	// Sending a GET request with Authorization token in the header
	CURL* curl;
	CURLcode res;
	curl = curl_easy_init();

	string response;

	// curl setup
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");

	// Headers
	struct curl_slist* headers = NULL;
	string auth_string = "Authorization: " + authorization_token;   // Authorization header as string
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

	// Getting path to login file
	char* buf = nullptr;
	size_t sz = 0;
	if (_dupenv_s(&buf, &sz, "AppData") == 0 && buf != nullptr)
	{
		client_path = buf;
		free(buf);
	}
	client_path += "\\SenecClient";
	login_filename = "\\login_data.json";

	// Setting Authorization token and battery ID
	setAuthToken();
	setBatteryId();
}

void SenecClient::setAuthToken()
{
	// Setting Authentication token
	string login_data = getLoginData();
	json authentication_response = json::parse(postRequest(login_url.c_str(), login_data.c_str()));
	token = authentication_response["token"];
}

void SenecClient::setBatteryId()
{
	// Setting the battery id
	json systems_response = json::parse(getRequestWithAuth(systems_url.c_str(), token));
	battery_id = systems_response[0]["id"];          // Potentially multiple batteries available, selecting the first one
}

string SenecClient::getLoginData()
{
	// TODO: include exception handling for login file not present/incorrect
	ifstream file(client_path + login_filename);
	json jfile = json::parse(file);

	return jfile.dump();
}


PowerState SenecClient::getDashboardData()
{
	string dashboar_url = systems_url + battery_id + "/dashboard";
	json dashboard_response = json::parse(getRequestWithAuth(dashboar_url.c_str(), token));
	PowerState state = PowerState(dashboard_response);
	return state;
	//return dashboard_response.dump(4);
}


