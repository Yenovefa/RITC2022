/*
* \file tender.cpp
* \brief Function to verify if tender offers are worth buying/selling. Does the action if worth it  
*/

#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "tender.h"
#include <string>
#include "dotToComma.h"
#include "findBookSize.h"

using namespace std;

/*
* \brief 
*/
void tenderOffer()
{

	string apiAdress = "http://localhost:9999/v1/";
	string apiKey = "123456";

	cpr::Response r = cpr::Get(cpr::Url{ (apiAdress + "case") }, cpr::Parameters{ {"key", apiKey} });
	nlohmann::json parsing = nlohmann::json::parse(r.text); // put r.text in json format to parse

	// Tender values
	cpr::Response r_tender = cpr::Get(cpr::Url{ (apiAdress + "tenders") }, cpr::Parameters{ {"key", apiKey} });
	nlohmann::json parse_tender = nlohmann::json::parse(r_tender.text);
	
	long id_tender{ parse_tender[0]["tender_id"].get<long>() };
	long quantity_tender{ parse_tender[0]["quantity"].get<long>() };
	string action_tender{ parse_tender[0]["action"].get<string>() };
	long double price_tender{ parse_tender[0]["price"].get<long double>() };
	long expires_tender{ parse_tender[0]["expires"].get<long>() };
	
	string ticker = "RITC";

	// ---------------------------- IF LARGER THAN BOOK SIZE, PROGRAM WILL FAIL--------------------
	int book_length_int = 30;  
	string book_length = to_string(book_length_int);


	// ---------------------------------------------VALUES------------------------------------------------

	//BULL values
	cpr::Response r_BULL = cpr::Get(cpr::Url{ (apiAdress + "securities/book") }, cpr::Parameters{ {"key", apiKey}, {"ticker", "BULL" }, {"limit", book_length } });
	nlohmann::json parse_BULL = nlohmann::json::parse(r_BULL.text);

	long double BULL_bid{ parse_BULL["bids"][0]["price"].get<long double>() };
	long double BULL_ask{ parse_BULL["asks"][0]["price"].get<long double>() };
	
	//BEAR values
	cpr::Response r_BEAR = cpr::Get(cpr::Url{ (apiAdress + "securities/book") }, cpr::Parameters{ {"key", apiKey}, {"ticker", "BEAR" }, {"limit", book_length } });
	nlohmann::json parse_BEAR = nlohmann::json::parse(r_BEAR.text);

	long double BEAR_bid{ parse_BEAR["bids"][0]["price"].get<long double>() };
	long double BEAR_ask{ parse_BEAR["asks"][0]["price"].get<long double>() };

	//RITC values
	cpr::Response r_RITC = cpr::Get(cpr::Url{ (apiAdress + "securities/book") }, cpr::Parameters{ {"key", apiKey}, {"ticker", "RITC" }, {"limit", book_length } });
	nlohmann::json parse_RITC = nlohmann::json::parse(r_RITC.text);

	long double RITC_bid{ parse_RITC["bids"][0]["price"].get<long double>() };
	long double RITC_ask{ parse_RITC["asks"][0]["price"].get<long double>() };

	// ------------------------------------------------------------------------------------------------------------------------------------------------
	
	// Offer for RITC (ETF)
	if (action_tender == "BUY")
	{
		cout << "-----------Buying tender------" << endl;
		cout << RITC_bid << endl;
		cout << price_tender << endl;
		cout << quantity_tender << endl;


		// Verifiy quantity in book under tender price
		long double quantity_total = 0;
		string type_of_order = "bids";
		int book = min(findBookSize(ticker, type_of_order), book_length_int);
		for (int i = 0; i < book; i++)
		{
			long double price_i{ parse_RITC["bids"][i]["price"].get<long double>() };

			if (price_i > price_tender)
			{
				long double quantity_i{ parse_RITC["bids"][i]["quantity"].get<long double>() };
				quantity_total = quantity_total + quantity_i;
			}
		}
		cout << "Quantity total : " << quantity_total << endl;
		cout << "Quantity tender : " << quantity_tender << endl;

		// accept offer if tender is profitable
		
		if ((price_tender < RITC_bid) && (quantity_total > quantity_tender))
		{

			// accept offer
			cpr::Response accept_buy = cpr::Post(cpr::Url{ (apiAdress + "tenders/" + to_string(id_tender)) }, cpr::Parameters{ {"key", apiKey} });
			cout << "accept " + accept_buy.text + "\n";

			// sell offer

			// Changing dot by comma for long double prices
			string price = dotToComma(price_tender);

			int y = quantity_tender / 10;
			while (y <= quantity_tender)
			{
				cpr::Response sell_RITC = cpr::Post(cpr::Url{ (apiAdress + "orders") }, cpr::Parameters{ {"key", apiKey}, {"ticker", "RITC" }, { "type", "LIMIT" }, { "quantity", to_string(quantity_tender / 10)}, { "action", "SELL" }, {"price", price } });

				y = y + (quantity_tender / 10);
			}
			cout << "Everything sold" << endl;
		}
	}
	// ---------------------------------------------TENDER SELL --------------------------------------
	else if (action_tender == "SELL")
	{
		cout << "-----------Selling tender------" << endl;
		cout << RITC_ask << endl;
		cout << price_tender << endl;
		cout << quantity_tender << endl;


		// Verifiy quantity in book over tender price
		long double quantity_total = 0;
		string type_of_order = "asks";
		int book = min(findBookSize(ticker, type_of_order), book_length_int);
		for (int i = 0; i < book; i++)
		{
			long double price_i{ parse_RITC["asks"][i]["price"].get<long double>() };

			if (price_i < price_tender)
			{
				long double quantity_i{ parse_RITC["asks"][i]["quantity"].get<long double>() };
				quantity_total = quantity_total + quantity_i;
			}
		}

		cout << "Quantity total : " << quantity_total << endl;
		cout << "Quantity tender : " << quantity_tender << endl;

		// accept offer if tender is profitable
		if ((price_tender > RITC_ask) && (quantity_total > quantity_tender))
		{
			auto time = parsing["tick"];

			if (expires_tender != time)
			{
				// accept offer
				cpr::Response accept_sell = cpr::Post(cpr::Url{ (apiAdress + "tenders/" + to_string(id_tender)) }, cpr::Parameters{ {"key", apiKey} });
				cout << "accept " + accept_sell.text + "\n";

				// sell offer

				// Changing dot by comma for long double prices
				string price = dotToComma(price_tender);

				int y = quantity_tender / 10;
				while (y <= quantity_tender)
				{
					cpr::Response buy_RITC = cpr::Post(cpr::Url{ (apiAdress + "orders") }, cpr::Parameters{ {"key", apiKey}, {"ticker", "RITC" }, { "type", "LIMIT" }, { "quantity", to_string(quantity_tender / 10)}, { "action", "BUY" }, {"price", price } });
					y = y + (quantity_tender / 10);
				}
				cout << "Everything sold" << endl;
			}
		}
	}	
}
