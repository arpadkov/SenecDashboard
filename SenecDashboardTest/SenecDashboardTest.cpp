#include "pch.h"
#include "CppUnitTest.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "../Exceptions.h"
#include "../SenecClient.cpp"
#include "../PowerState.cpp"
#include "../HttpClient.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using json = nlohmann::json;

void SenecClientInvalidLogin()
{
	SenecClient* client = new SenecClient();
	client->setAuthToken("\\test\\unittest\\login_data_invalid.json");
}

void SenecClientIncorrectLoginDataFormat()
{
	SenecClient* client = new SenecClient();
	client->setAuthToken("\\test\\unittest\\login_data_incorrect_format.json");
}

void SenecClientIncorrectLoginFileNotFound()
{
	SenecClient* client = new SenecClient();
	client->setAuthToken("\\test\\unittest\\NON_EXISTING_FILENAME.json");
}

string getClientPath()
{
	string client_path;

	char* buf = nullptr;
	size_t sz = 0;
	if (_dupenv_s(&buf, &sz, "AppData") == 0 && buf != nullptr)
	{
		client_path = buf;
		free(buf);
	}
	return client_path + "\\SenecClient";
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
			SenecClient* client = new SenecClient();

			auto func = [] { SenecClientInvalidLogin(); };
			Assert::ExpectException<InvalidCredentialsException>(func);
		}

		TEST_METHOD(SenecClientAuthIncorrectLoginDataFormat)
		{
			SenecClient* client = new SenecClient();

			auto func = [] { SenecClientIncorrectLoginDataFormat(); };
			Assert::ExpectException<json::parse_error>(func);
		}

		TEST_METHOD(SenecClientAuthLoginFileNotFound)
		{
			SenecClient* client = new SenecClient();

			auto func = [] { SenecClientIncorrectLoginFileNotFound(); };
			Assert::ExpectException<LoginFileNotFoundException>(func);
		}
	};

	TEST_CLASS(PowerStateTest)
	{
	public:

		TEST_METHOD(PowerStateReadTest)
		{
			ifstream file(getClientPath() + "\\test\\unittest\\dashboard_response.json");
			json jfile = json::parse(file);

			PowerState state = PowerState(jfile);

			double autarkie = jfile["aktuell"]["autarkie"]["wert"];
			double netzbezug = jfile["aktuell"]["netzbezug"]["wert"];
			double netzeinspeisung = jfile["aktuell"]["netzeinspeisung"]["wert"];
			double speicherbeladung = jfile["aktuell"]["speicherbeladung"]["wert"];

			Assert::AreEqual(autarkie, 99.29);
			Assert::AreEqual(netzbezug, 5.859);
			Assert::AreEqual(netzeinspeisung, 1700.578);
			Assert::AreEqual(speicherbeladung, 1000.1);
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
