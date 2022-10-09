#include <iostream>
#include "findBookSize.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <exception>
#include <algorithm>
#include <string>

using namespace std;


int findBookSize(const string& security, const string& type_of_order)
{
	string apiAdress = "http://localhost:9999/v1/";
	string apiKey = "123456";

	size_t size_to_try = 30;
	
	cpr::Response resp = cpr::Get(cpr::Url{ (apiAdress + "securities/book") }, cpr::Parameters{ {"key", apiKey}, {"ticker", security }, {"limit", to_string(size_to_try) } });

	nlohmann::json j = nlohmann::json::parse(resp.text);
	return j[type_of_order].size();

}
