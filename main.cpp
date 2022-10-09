/*
* \file main.cpp
* \brief Main file
* 
* Values to change before running
* findBookSize.h -> buffer
* findBookSize -> size_to_try
* main -> apiKey
* tender.h -> book_length_int
*/

#include <iostream>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "timeRemaining.h"
#include "arbitrageOpportunity.h"
#include "tender.h"
#include <exception>
#include "findBookSize.h"
#include <vector>

using namespace std;
using namespace nlohmann;

int main()
{
	string go;
	cout << "Type anything to start ";
	cin >> go;
	cout << "Let's make some bread" << endl;
	string apiAdress = "http://localhost:9999/v1/";
	string apiKey = "123456";
	
	// Project loop
	while (1)
	{
		cpr::Response r = cpr::Get(cpr::Url{ (apiAdress + "case") }, cpr::Parameters{ {"key", apiKey} });

		if (r.status_code == 200)
		{
			while (timeRemaining(r) > 2 && timeRemaining(r) < 300)
			{
				cpr::Response r_tender = cpr::Get(cpr::Url{ (apiAdress + "tenders") }, cpr::Parameters{ {"key", apiKey} });

				if (r_tender.text != "[]")
				{
					try
					{
						tenderOffer();

					}
					catch (...)
					{
						cout << "Exception error, probably low book size" << endl;
						break;
					}
				}
				//arbitrageOpportunity();

			}
		}
		else
		{
			cout << r.status_code << endl;
			cout << r.text << endl;

		}
	}
}