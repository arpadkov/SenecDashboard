#include "pch.h"
#include "CppUnitTest.h"
//#include <fstream>
//#include <nlohmann/json.hpp>
#include "../Exceptions.h"
#include "../SenecClient.cpp"
#include "../PowerState.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
//using json = nlohmann::json;

void SenecClientLoginFails()
{
	SenecClient* client = new SenecClient();
	client->setAuthToken("\\test\\unittest\\login_data_invalid.json");
}

namespace SenecDashboardTest
{
	TEST_CLASS(SenecDashboardTest)
	{
	public:
		
		TEST_METHOD(SenecClientAuthTestPositiv)
		{
			SenecClient* client = new SenecClient();
			client->setAuthToken("\\test\\unittest\\login_data_valid.json");
			Assert::AreEqual(client->Initialized, true);
		}

		TEST_METHOD(SenecClientAuthTestFileNotFound)
		{
			SenecClient* client = new SenecClient();

			auto func = [] { SenecClientLoginFails(); };
			Assert::ExpectException<std::exception>(func);
		}


		//string client_path;

		//char* buf = nullptr;
		//size_t sz = 0;
		//if (_dupenv_s(&buf, &sz, "AppData") == 0 && buf != nullptr)
		//{
		//	client_path = buf;
		//	free(buf);
		//}
		//client_path += "\\SenecClient";

		//string test_path = client_path + "\\test\\test_dashboard_response.json";
		//ifstream file(test_path);
		//json jfile = json::parse(file);

		//PowerState state = PowerState(jfile);

		//PowerState state = client->getDashboardData();


		//std::string name = "Bill";
		//std::string name2 = "Bill";
		//Assert::AreEqual(name, name2);
	};
}
