#include <iostream>
#include "dotToComma.h"
#include <string>

using namespace std;

std::string dotToComma(long double x)
{
	std::string price = to_string(x);
	size_t len = price.length();
	for (size_t i = 0; i < len; i++)
	{
		if (price[i] == '.')
		{
			price[i] = ',';
			i = len; // or `break;`
		}
	}

	return price;
}