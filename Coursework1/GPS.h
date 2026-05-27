#pragma once
#include <string>
#include <tuple> 
#include <format>

using namespace std;

class GPS
{
private: tuple<int, int, int> Latitude;
		 tuple<int, int, int> Longitude;
public:
	GPS() {}
	GPS(tuple<int, int, int> lat, tuple<int, int, int> lon) :
		Latitude(lat), Longitude(lon) {
	}
	string GetLatitude() const;
	string GetLongitude() const;
	bool operator<(const GPS& g) const;
	bool operator==(const GPS& g) const;
};