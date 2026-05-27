#pragma once
#include <string>
#include <vector>
#include <tuple>
#include <variant>
#include <optional>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "GPS.h"

using namespace std;

class Entry
{
private:
	string Order{ "" };                         // As in class Item
	string Family{ "" };                        // As in class Item
	string Species{ "" };                       // As in class Item
	char Category = 0;                           // As in class Item
	vector<tuple<variant<string, GPS>, chrono::year_month_day, chrono::hh_mm_ss<chrono::seconds>,
		optional<int>>> Observations{};  // As in class Item
public:
	//
	// Constructors
	//
	Entry() {}
	Entry(string s1, string s2, string s3, char c, vector<tuple<variant<string, GPS>, chrono::year_month_day,
		chrono::hh_mm_ss<chrono::seconds>, optional<int>>> v) :
		Order(s1), Family(s2), Species(s3), Category(c)
	{
		Observations.resize(v.size());
		ranges::copy(v, Observations.begin());
	}
	Entry(const Entry& Original) : Order(Original.Order), Family(Original.Family),
		Species(Original.Species), Category(Original.Category)
	{
		Observations.resize(Original.Observations.size());
		ranges::copy(Original.Observations, Observations.begin());
	}
	//
	// Accessor functions
	//
	string GetOrder() const;
	void SetOrder(string s);
	string GetFamily() const;
	void SetFamily(string s);
	string GetSpecies() const;
	void SetSpecies(string s);
	char GetCategory() const;
	void SetCategory(char c);
	vector<tuple<variant<string, GPS>, chrono::year_month_day, chrono::hh_mm_ss<chrono::seconds>, optional<int>>> GetObservations() const;
	void SetObservations(vector<tuple<variant<string, GPS>, chrono::year_month_day, chrono::hh_mm_ss<chrono::seconds>, optional<int>>> o);
	//
	// Operator functions
	//
	bool operator<(const Entry&) const;
	Entry& operator=(const Entry& Right);
	friend ostream& operator<<(ostream&, const Entry&);
	friend fstream& operator<<(fstream&, const Entry&);
};