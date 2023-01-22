#include "HttpClient.h"
#include "Exceptions.h"
#include <curl/curl.h>

using string = std::string;

size_t HttpClient::writeCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	// WriteCallback function, used by curl: CURLOPT_WRITEFUNCTION
	((string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

string HttpClient::postRequest(const char* url, const char* data)
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

	if (res != CURLE_OK) {
		throw NetworkErrorException();
	}

	curl_easy_cleanup(curl);

	return response;
}

string HttpClient::getRequestWithAuth(const char* url, string authorization_token)
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

	if (res != CURLE_OK) {
		throw NetworkErrorException();
	}

	curl_easy_cleanup(curl);

	return response;
}
