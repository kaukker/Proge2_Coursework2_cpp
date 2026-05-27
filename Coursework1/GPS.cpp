#include "GPS.h"

string GPS::GetLatitude() const
{
	const auto& [deg, min, sec] = Latitude;
	return std::format("{:02d}°{:02d}'{:02d}\" N", deg, min, sec);
}

string GPS::GetLongitude() const
{
	const auto& [deg, min, sec] = Longitude;
	return std::format("{:02d}°{:02d}'{:02d}\" E", deg, min, sec);
}

bool GPS::operator<(const GPS& g) const
{
	if (Latitude != g.Latitude)
	{
		return Latitude < g.Latitude;
	}
	return Longitude < g.Longitude;
}

bool GPS::operator==(const GPS& g) const
{
	return Latitude == g.Latitude && Longitude == g.Longitude;
}