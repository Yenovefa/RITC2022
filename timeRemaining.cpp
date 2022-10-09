/*
* \file timeRemaining.cpp
* \brief Function to return the time remaining for the simulation
*/

#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using namespace std;

int timeRemaining(cpr::Response& r)
{
	nlohmann::json parsing = nlohmann::json::parse(r.text); // put r.text in json format to parse
	auto timeRemaining = parsing["ticks_per_period"] - parsing["tick"];

	return timeRemaining;
}
