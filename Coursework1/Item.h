#pragma once
#include "Main.h" 

class Item
{
private:
	string Order{ "" };    // selts, näiteks värvulised
	string Family{ "" };   // sugukond, näiteks vintlased
	string Species{ "" };  // liik, näiteks metsvint
	char Category = 0;     // esinemise kategooria A, B, C, D, E (vt. originaali web-st)
	vector<tuple<variant<string, GPS>, year_month_day, hh_mm_ss<seconds>, optional<int>>> Observations{};
	// This is a vector of tuples: 
	// get<0> specifies the location as object of type variant<string, GPS> where string presents 
	// the community and object from class GPS the location coordinates
	// get<1> specifies the date of observation as object of type year_month_day
	// get<2> specifies the time of obseravation as object of type hh_mm_ss<seconds>
	// get<3> specifies the number of observed birds. nullopt means that the counting failed
	// The size ov vector Observations may be any except 0.
public:
	Item() {}
	operator Entry()  // operator function to cast an object of Item to object of Entry (see below)
	{
		return Entry(Order, Family, Species, Category, Observations);
	}
};