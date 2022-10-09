#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "sma.h"
#include <vector>


long double sma(std::vector<long double>& data, const int size,const std::string& ticker)
{
	long double average = 0;
	//std::vector <long double> data;

	std::string apiAdress = "http://localhost:9999/v1/";
	std::string apiKey = "123456";

	cpr::Response r = cpr::Get(cpr::Url{ (apiAdress + "securities") }, cpr::Parameters{ {"key", apiKey}, {"ticker", ticker } });

	//cpr::Response r = cpr::Get(cpr::Url{ (apiAdress + "case") }, cpr::Parameters{ {"key", apiKey} });
	nlohmann::json parsing = nlohmann::json::parse(r.text); // put r.text in json format to parse

	std::string value = parsing[0]["last"];

	data.push_back(std::stold(value));

	return average;
}