#include <nlohmann/json.hpp>
#include "pch.h"
#include "CppUnitTest.h"
#include <fstream>
#include "../Exceptions.h"
#include "../SenecClient.cpp"
#include "../PowerState.cpp"
#include "../HttpClient.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using string = std::string;
using json = nlohmann::json;

string getClientPath()
{
	string path;

	char* buf = nullptr;
	size_t sz = 0;
	if (_dupenv_s(&buf, &sz, "AppData") == 0 && buf != nullptr)
	{
		path = buf;
		free(buf);
	}
	return path + "\\SenecClient";
}

void SenecClientInvalidLogin()
{
	// Reads login file with invalid credentials -> Exception
	SenecClient* client = new SenecClient();
	client->setAuthToken("\\test\\unittest\\login_data_invalid.json");
}

void SenecClientIncorrectLoginDataFormat()
{
	// Reads login file with not json format -> Exception
	SenecClient* client = new SenecClient();
	client->setAuthToken("\\test\\unittest\\login_data_incorrect_format.json");
}

void SenecClientIncorrectLoginFileNotFound()
{
	// Readsnon existing login file -> Exception
	SenecClient* client = new SenecClient();
	client->setAuthToken("\\test\\unittest\\NON_EXISTING_FILENAME.json");
}

void PowerStateIncorrectRead()
{
	// Deleted entry from json -> Can not be parsed into PowerState -> Exception
	std::ifstream file(getClientPath() + "\\test\\unittest\\dashboard_response_incorrect.json");
	json jfile = json::parse(file);

	PowerState state = PowerState(jfile);
}

namespace SenecDashboardTest
{
	TEST_CLASS(SenecClientTest)
	{
	public:
		
		TEST_METHOD(SenecClientAuthPositiv)
		{
			SenecClient* client = new SenecClient();
			client->setAuthToken("\\test\\unittest\\login_data_valid.json");
			Assert::AreEqual(client->Initialized, true);
		}

		TEST_METHOD(SenecClientAuthInvalidLogin)
		{
			auto func = [] { SenecClientInvalidLogin(); };
			Assert::ExpectException<InvalidCredentialsException>(func);
		}

		TEST_METHOD(SenecClientAuthIncorrectLoginDataFormat)
		{
			auto func = [] { SenecClientIncorrectLoginDataFormat(); };
			Assert::ExpectException<json::parse_error>(func);
		}

		TEST_METHOD(SenecClientAuthLoginFileNotFound)
		{
			auto func = [] { SenecClientIncorrectLoginFileNotFound(); };
			Assert::ExpectException<LoginFileNotFoundException>(func);
		}
	};

	TEST_CLASS(PowerStateTest)
	{
	public:

		TEST_METHOD(PowerStateReadTest)
		{
			std::ifstream file(getClientPath() + "\\test\\unittest\\dashboard_response.json");
			json jfile = json::parse(file);

			PowerState state = PowerState(jfile);

			double autarkie = jfile["aktuell"]["autarkie"]["wert"];
			double netzbezug = jfile["aktuell"]["netzbezug"]["wert"];
			double netzeinspeisung = jfile["aktuell"]["netzeinspeisung"]["wert"];
			double speicherbeladung = jfile["aktuell"]["speicherbeladung"]["wert"];

			// Assering some test values to validate
			Assert::AreEqual(autarkie, 99.29);
			Assert::AreEqual(netzbezug, 5.859);
			Assert::AreEqual(netzeinspeisung, 1700.578);
			Assert::AreEqual(speicherbeladung, 1000.1);
		}

		TEST_METHOD(PowerStateIncorrectReadTest)
		{
			auto func = [] { PowerStateIncorrectRead(); };
			Assert::ExpectException<IncorrectDashboardResponseException>(func);
		}
	};

	TEST_CLASS(HttpClientTest)
	{
	public:

		TEST_METHOD(GetRequestTest)
		{
			string httpTestServerGet = "https://httpbin.org/get";
			json response = json::parse(HttpClient::getRequestWithAuth(httpTestServerGet.c_str(), "TOKEN"));
			string urlFromResponse = response["url"];
			Assert::AreEqual(urlFromResponse, httpTestServerGet);
		}
	};
}
