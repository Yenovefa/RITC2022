/*
* \file arbitrage.cpp
* \brief Function to verify if there is an arbitrage opportunity possible. Does it if it's worth it.
*/

#include <iostream>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "arbitrageOpportunity.h"
#include "dotToComma.h"

using namespace std;

void arbitrageOpportunity()
{
	string apiAdress = "http://localhost:9999/v1/";
	string apiKey = "123456";

	// ---------------------------------------------VALUES-------------------------------------
	//BEAR values
	cpr::Response r_bear = cpr::Get(cpr::Url{ (apiAdress + "securities/book") }, cpr::Parameters{ {"key", apiKey}, {"ticker", "BEAR" } });
	nlohmann::json parse_bear = nlohmann::json::parse(r_bear.text);

	long double bear_bid{ parse_bear["bids"][0]["price"].get<long double>() };
	long double bear_ask{ parse_bear["asks"][0]["price"].get<long double>() };

	long double bear_quantity_bid{ parse_bear["bids"][0]["quantity"].get<long double>() };
	long double bear_quantity_ask{ parse_bear["asks"][0]["quantity"].get<long double>() };

	//BULL values
	cpr::Response r_bull = cpr::Get(cpr::Url{ (apiAdress + "securities/book") }, cpr::Parameters{ {"key", apiKey}, {"ticker", "BULL" } });
	nlohmann::json parse_bull = nlohmann::json::parse(r_bull.text);

	long double bull_bid{ parse_bull["bids"][0]["price"].get<long double>() };
	long double bull_ask{ parse_bull["asks"][0]["price"].get<long double>() };

	long double bull_quantity_bid{ parse_bull["bids"][0]["quantity"].get<long double>() };
	long double bull_quantity_ask{ parse_bull["asks"][0]["quantity"].get<long double>() };

	//RITC values
	cpr::Response r_ritc = cpr::Get(cpr::Url{ (apiAdress + "securities/book") }, cpr::Parameters{ {"key", apiKey}, {"ticker", "RITC" } });
	nlohmann::json parse_ritc = nlohmann::json::parse(r_ritc.text);

	long double ritc_bid{ parse_ritc["bids"][0]["price"].get<long double>() };
	long double ritc_ask{ parse_ritc["asks"][0]["price"].get<long double>() };

	long double ritc_quantity_bid{ parse_ritc["bids"][0]["quantity"].get<long double>() };
	long double ritc_quantity_ask{ parse_ritc["asks"][0]["quantity"].get<long double>() };
	
	//USD values
	cpr::Response r_usd = cpr::Get(cpr::Url{ (apiAdress + "securities/book") }, cpr::Parameters{ {"key", apiKey}, {"ticker", "USD" } });
	nlohmann::json parse_usd = nlohmann::json::parse(r_usd.text);

	long double usd_bid{ parse_usd["bids"][0]["price"].get<long double>() };
	long double usd_ask{ parse_usd["asks"][0]["price"].get<long double>() };

	//------------------------------------VERIFICATION------------------------------------------------

	//Pre-calculations
	long double etf_ask = (bull_ask + bear_ask)/usd_ask;
	long double etf_bid = (bull_bid + bear_bid)/usd_bid;

	long double base_fee = 0.0;
	long double fee_bull_ask = base_fee * bull_quantity_ask;
	long double fee_bull_bid = base_fee * bull_quantity_bid;
	long double fee_bear_ask = base_fee * bear_quantity_ask;
	long double fee_bear_bid = base_fee * bear_quantity_bid;
	long double fee_ritc_ask = base_fee * ritc_quantity_ask;
	long double fee_ritc_bid = base_fee * ritc_quantity_bid;

	//Creation ETF
	//if (bear_quantity_ask == ritc_quantity_bid && bull_quantity_ask == ritc_quantity_bid)
	//{
		if ((etf_ask + fee_bull_ask + fee_bear_ask) < (ritc_bid - fee_ritc_bid))
		{
			//Buy bull and bear
			cpr::Response sell_bull = cpr::Post(cpr::Url{ (apiAdress + "orders") }, cpr::Parameters{ {"key", apiKey}, {"ticker", "BULL" }, { "type", "MARKET" }, { "quantity", dotToComma(bull_quantity_ask)}, { "action", "BUY" } });
			cout << sell_bull.text << endl;

			cpr::Response sell_bear = cpr::Post(cpr::Url{ (apiAdress + "orders") }, cpr::Parameters{ {"key", apiKey}, {"ticker", "BEAR" }, { "type", "MARKET" }, { "quantity", dotToComma(bear_quantity_ask)}, { "action", "BUY" } });

			cout << "Arbitrage bought" << endl;

			//Sell ETF
			cpr::Response sell_ritc = cpr::Post(cpr::Url{ (apiAdress + "orders") }, cpr::Parameters{ {"key", apiKey}, {"ticker", "RITC" }, { "type", "MARKET" }, { "quantity", dotToComma(ritc_quantity_bid)}, { "action", "SELL" } });
			cout << "Arbitrage sold" << endl;
		}
	//}

	else if(etf_bid > ritc_ask)
	{
		//Buy ETF

		//Sell bull and bear
		;
	}

}
