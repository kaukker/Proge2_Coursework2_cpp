#include "Entry.h"

string Entry::GetOrder() const
{
    return Order;
}

void Entry::SetOrder(string s)
{
    Order = s;
}

string Entry::GetFamily() const
{
    return Family;
}

void Entry::SetFamily(string s)
{
    Family = s;
}

string Entry::GetSpecies() const
{
    return Species;
}

void Entry::SetSpecies(string s)
{
    Species = s;
}

char Entry::GetCategory() const
{
    return Category;
}

void Entry::SetCategory(char c)
{
    Category = c;
}

vector<tuple<variant<string, GPS>, chrono::year_month_day, chrono::hh_mm_ss<chrono::seconds>, 
    optional<int>>> Entry::GetObservations() const
{
    return Observations;
}

void Entry::SetObservations(vector<tuple<variant<string, GPS>, chrono::year_month_day,
    chrono::hh_mm_ss<chrono::seconds>, optional<int>>> o)
{
    Observations = o;
}

bool Entry::operator<(const Entry& right) const
{
    // Kontroll, kas sugukond, perekond, liik või ohustus klass on sama 
    if (Order != right.Order) 
    {
        return Order < right.Order;
    }
    if (Family != right.Family)
    {
        return Family < right.Family;
    }
    if (Species != right.Species)
    {
        return Species < right.Species;
    }
    if (Category != right.Category)
    {
        return Category < right.Category;
    }

    // Leiab lühema vaatluste arvu
    const size_t n = min(Observations.size(), right.Observations.size());

    // Läbib kõik võrreldavad vaatlused
    for (size_t i = 0; i < n; ++i)
    {
        const auto& leftObs = Observations[i];
        const auto& rightObs = right.Observations[i];

        // Võtab välja sukohad
        const auto& leftLocation = get<0>(leftObs);
        const auto& rightLocation = get<0>(rightObs);

        // Kontroll, kas asukohad on sama tüüpi
        if (leftLocation.index() == rightLocation.index())
        {
            // kontroll kas asukoht on sõnaline
            if (holds_alternative<string>(leftLocation))
            {
                const string& l = get<string>(leftLocation);
                const string& r = get<string>(rightLocation);

                // Asukoha võrdlus
                if (l != r)
                {
                    return l < r;
                }
            }
            else // Asukohad on koordinaadid
            {
                const GPS& l = get<GPS>(leftLocation);
                const GPS& r = get<GPS>(rightLocation);

                // Asukoha võrdlus
                if (!(l == r))
                {
                    return l < r;
                }
            }
        }

        // Võtab välja kuupäeva
        const auto& leftDate = get<1>(leftObs);
        const auto& rightDate = get<1>(rightObs);

        // Kuupäeva võrdlus
        if (leftDate != rightDate)
        {
            return (leftDate <=> rightDate) < 0;
        }

        // Võtab välja kellaaja
        const auto leftTime = get<2>(leftObs).to_duration();
        const auto rightTime = get<2>(rightObs).to_duration();

        // Kellaaja võrdlus
        if (leftTime != rightTime)
        {
            return leftTime < rightTime;
        }

        // võtab välja lindude arvu 
        const auto& leftCount = get<3>(leftObs);
        const auto& rightCount = get<3>(rightObs);

        // Koguse võrdlus
        if (leftCount != rightCount)
        {
            return leftCount < rightCount;
        }
    }

    // Vaatluste arvu võrdlus
    return Observations.size() < right.Observations.size();
}

Entry& Entry::operator=(const Entry& Right)
{
    // Kontroll, et tegemist on teise objektiga
    if (this != &Right)
    {
        Order = Right.Order;
        Family = Right.Family;
        Species = Right.Species;
        Category = Right.Category;

        // Tühjendab olemasolevd vaatlused
        Observations.clear();
        // Muudab suurust vastavalt kopeeritavale vaatlusele
        Observations.resize(Right.Observations.size());
        // Vaatlus kopeeritakse ümber
        ranges::copy(Right.Observations, Observations.begin());
    }

    return *this;
}

ostream& operator<<(ostream& os, const Entry& e)
{
    os << "\n"
       << e.Species
       << " (Order: " << e.Order
       << ", Family: " << e.Family << ")\n\n";

    int index = 1;

    // Läbib kõik objekti vaatlused
    for (const auto& obs : e.Observations)
    {
        // Võtab tuple'st vaatlusandmed 
        const auto& location = get<0>(obs);
        const auto& date = get<1>(obs);
        const auto& time = get<2>(obs);
        const auto& count = get<3>(obs);

        os << index++ << ")Location: ";

        // Kontroll, kas asukoht on sõnaline
        if (holds_alternative<string>(location))
        {
            os << get<string>(location) << "\n";
        }
        // Asukoht on kirjas koordinaatidena
        else
        {
            const GPS& gps = get<GPS>(location);
            os << gps.GetLatitude() << " " //Kuvab laiuskraadid
               << gps.GetLongitude() << "\n"; // Kuvab pikkusktraadid
        }

        os << "Time: "
           << format("{:%H:%M}", time.to_duration()) << " "
           << format("{:%d-%m-%Y}", date) << "\n";

        // Kontroll, kas lindude arv on teada
        if (count.has_value())
        {
            os << "Number of birds: " << count.value() << "\n\n";
        }
        else
        {
            os << "Number of birds: Unknown"<< "\n\n";
        }
    }

    return os;

}

fstream& operator<<(fstream& fs, const Entry& e)
{
    operator<<(static_cast<ostream&>(fs), e);
    return fs;
}