#include "Data.h"

// Tagastab kirjete(Entry) arvu
int Data::CountAllEntries()
{
    return nEntries;
}

// Kuvab kõik andmestruktuuri andmed
void Data::PrintAll()
{
    // Liigub läbi DataStructure
    ranges::for_each(DataStructure, [](const auto& order)
        {
            // Liigub läbi sugukonna
            ranges::for_each(order.second, [](const auto& family)
                {          
                    // liigub läbi perekonna
                    ranges::for_each(family.second, [](const Entry& e)
                        {
                            cout << e << endl; // kuvab kirje
                        });
                });
        });
}

// Salvestab kõik andmestruktuuri andmed faili
void Data::StoreAll(fstream& fs)
{
    // Liigub läbi DataStructure
    ranges::for_each(DataStructure, [&](const auto& order)
        {
            // Liigub läbi sugukonna
            ranges::for_each(order.second, [&](const auto& family)
                {
                    // liigub läbi perekonna
                    ranges::for_each(family.second, [&](const Entry& e)
                        {
                            fs << e << endl; // Salvestab faili
                        });
                });
        });
}

// Sisestab kirje
bool Data::InsertEntry(Item* pI)
{
    // Kontroll, et sisend oleks olemas
    if (pI == nullptr)
    {
        return false;
    }

    // Teisendab Item objekti Entry objektiks
    Entry e = *pI;

    delete pI;

    auto result = DataStructure[e.GetOrder()][e.GetFamily()].insert(e);

    // kui lisamine oli edukas suurendatatakse kirjete koguarvu
    if (result.second)
    {
        ++nEntries;
    }

    return result.second;
}

// Sisetab kirjed
bool Data::InsertEntries(initializer_list<Item*> items)
{
    bool allInserted = true;

    // Käib läbi kõik listis olevad item'id 
    for (Item* item : items)   
    {
        // Kontroll, kas kirje lisamine oli edukas
        if (!InsertEntry(item))
        {
            allInserted = false;
        }
    }

    return allInserted;
}

// Kuvab kirjed vastavalt liigile
void Data::PrintEntries(string species) noexcept(false)
{
    string searchSpecies = ToLower(species);
    bool speciesFound = false;

    // Läbib kõik sugukonnad
    for (const auto& order : DataStructure)
    {
        // Läbib kõik perekonnad
        for (const auto& family : order.second)
        {
            // Läbib kõik Entry objektid antud perekonnas
            ranges::for_each(family.second, [&](const Entry& entry)
                {
                    // Kontroll, kas liigid kattuvad
                    if (ToLower(entry.GetSpecies()) == searchSpecies)
                    {
                        cout << entry << endl;
                        speciesFound = true;
                    }
                });

            // Kui liik leiti, lõpeta töö
            if (speciesFound)
            {
                return;
            }
        }
    }

    throw runtime_error("Species not found");
}

// Võtab kõik sama liigi kohta käivad kirjed kokku
Entry* Data::GetSummaryEntry(string species)
{
    bool speciesFound = false;
    Entry* result = nullptr;
    string searchSpecies = ToLower(species);

    // Läbib kõik sugukonnad
    for (const auto& [orderName, families] : DataStructure)
    {
        // Läbib kõik perekonnad
        for (const auto& [familyName, entries] : families)
        {
            // Läbib kõik Entry objektid 
            for (const Entry& e : entries)
            {
                // Kontroll, kas liik katuub
                if (ToLower(e.GetSpecies()) == searchSpecies)
                {
                    speciesFound = true;

                    // Kontroll, kas leiti esimene vaste
                    if (result == nullptr)
                    {
                        result = new Entry(e); // Luuakse uus Entry objekt
                    }
                    else
                    {
                        // Võtab olemasoleva kirjed ja salvestab ajutisse vektorisse 
                        auto resultObservations = result->GetObservations();

                        // Lisab praeguse objekti kirjed ja lisab need ajutisse vektorisse
                        ranges::copy(e.GetObservations(), back_inserter(resultObservations));

                        // Salvestab ajutise vektori info kokkuvõtvasse Entry objekti
                        result->SetObservations(resultObservations);
                    }
                }
            }

            // Kui liik on sellest perekonnast leitud lõpeta töö 
            if (speciesFound)
            {
                return result;
            }
        }
    }

    return result;
}

// Tagastab viida nimekrija, kus on kõik kirjed liigi põhjal
list<Entry>* Data::GetEntries(string species)
{
    list<Entry>* result = new list<Entry>();

    string searchSpecies = ToLower(species);
    bool speciesFound = false;

    // Läbib kõik sugukonnad
    for (const auto& order : DataStructure)
    {
        // läbib kõik perekonnad
        for (const auto& family : order.second)
        {
            // Saab iteraatorti perekonna kirjete algusele
            auto it = family.second.begin();

            // Kestab kuni perekonnas on kirjeid
            while (it != family.second.end())
            {
                // Leiab kirje, mille liik on sama mida otsitakse
                it = ranges::find_if(it, family.second.end(), [&](const Entry& entry)
                    {
                        return ToLower(entry.GetSpecies()) == searchSpecies;
                    });
                
                // Kui vaste leiti
                if (it != family.second.end())
                {
                    result->push_back(*it); // Lisatakse tulemuste listi
                    speciesFound = false;
                    ++it;
                }
            }

            // Kui abtud perekonnast leiti liik võib tulemuse tagastada
            if (speciesFound)
            {
                return result;
            }
        }
    }

    // Kui andmestruktuutis liiki pole tagastatakse nullpointer
    if (result->empty())
    {
        delete result;
        return nullptr;
    }

    return result;
}

// Loeb kokku kirjed kindla liigi kohta
int Data::CountEntries(string species)
{
    bool speciesFound = false;
    int count = 0;
    string searchSpecies = ToLower(species);

    // Läbib kõik sugukonnad
    for (const auto& order : DataStructure)
    {
        // Läbib kõik perekonnad
        for (const auto& family : order.second)
        {
            // Läbib kõik Entry objektid antud perekonnas 
            ranges::for_each(family.second, [&](const Entry& entry)
                {
                    // Kontroll kas liik katub otsituga
                    if (ToLower(entry.GetSpecies()) == searchSpecies)
                    {
                        ++count; 
                        speciesFound = true;
                    }
                });

            // Kui antud perekonnast leiti
            if (speciesFound)
            {
                return count;
            }
        }
    }

    return count;
}

// Kustutab kirjed liigi alusel
bool Data::RemoveEntries(string species)
{
    bool removed = false;
    string searchSpecies = ToLower(species);

    // Läbib kõik sugukonnad
    for (auto& order : DataStructure)
    {
        // Läbib kõik perekonnad
        for (auto& family : order.second)
        {
            // Eemaldab kõik kirjed, mis vastavad otsitavale liigile
            size_t removedCount = erase_if(family.second, [&](const Entry& entry)
                {
                    return ToLower(entry.GetSpecies()) == searchSpecies;
                });

            // Kontroll, kas kirjeid eemaldati
            if (removedCount > 0)
            {
                nEntries -= static_cast<int>(removedCount);
                removed = true;
            }

            // Kui kirjeid eemaldatki liigu tsükklist välja
            if (removed)
            {
                break;
            }
        }

        // Eemaldab kõik tühjaks jäänud perekonnad
        erase_if(order.second, [](const auto& family)
            {
                return family.second.empty();
            });
        
        // Kui kirjeid eemaldatki liigu tsükklist välja
        if (removed)
        {
            break;
        }
    }

    // Eemaldab kõik tühjaks jäänud sugukonnad
    erase_if(DataStructure, [](const auto& order)
        {
            return order.second.empty();
        });

    return removed;
}

// Kuvab kirjed perekonna põhjal
void Data::PrintFamilyEntries(string family) noexcept(false)
{
    string searchFamily = ToLower(family);

    // Läbib kõik sugukonnad
    for (const auto& order : DataStructure)
    {
        // Läbib kõik perekonnad
        for (const auto& family : order.second)
        {
            // Kontroll, kas perekond katub otsitavaga
            if (ToLower(family.first) == searchFamily)
            {
                // Prindib välja kõik perekonna kirjed
                ranges::for_each(family.second,
                    [](const Entry& entry)
                    {
                        cout << entry << endl;
                    });

                return;
            }
        }
    }

    throw runtime_error("Family not found");
}

// Loendab kokku kirjed perekonna põhjal
int Data::CountFamilyEntries(string family)
{
    string searchFamily = ToLower(family);

    // Liigub läbi sugukonna
    for (const auto& order : DataStructure)
    {
        // Liigub läbi perekonna
        for (const auto& family : order.second)
        {
            // Kontroll, kas perekonna nimi kattub otsitavaga
            if (ToLower(family.first) == searchFamily)
            {
                int count = 0;

                // Loendab kokku kõik kirjed antud perekonna kohta
                ranges::for_each(family.second, [&](const Entry& e)
                    {
                        ++count;
                    });

                return count;
            }
        }
    }

    return 0;
}

// Eemaldab kirjed perekonna põhjal
bool Data::RemoveFamilyEntries(string family)
{
    string searchFamily = ToLower(family);
    bool removed = false;

    // Läbib kõik sugukonnad
    for (auto& order : DataStructure)
    {
        // Leiab perekonna vastavalt otsitavale perekonnale
        auto it = ranges::find_if(order.second, [&](const auto& family)
            {
                return ToLower(family.first) == searchFamily;
            });

        // Kontroll, et perekond leiti
        if (it != order.second.end())
        {
            // Vähendab kirjete arvu vastavalt, mitu kirjet läheb kustutamisele
            nEntries -= static_cast<int>(it->second.size());

            order.second.erase(it); // Eemaldab perekonna, koos selle kirjetega

            removed = true;
            break;
        }
    }

    // Eemaldab sugukonnad, millel pole perekondi
    erase_if(DataStructure, [](const auto& order)
        {
            return order.second.empty();
        });

    return removed;
}

// Kuvab kirjed vastavalt sugukonnale
void Data::PrintOrderEntries(string order) noexcept(false)
{
    string searchOrder = ToLower(order);

    // Otsib  andmestruktuurist sobivat sugukonda
    auto it = ranges::find_if(DataStructure, [&](const auto& order)
        {
            return ToLower(order.first) == searchOrder;
        });

    // Kontroll, kas sugukond leiti
    if (it == DataStructure.end())
    {
        throw runtime_error("Order not found");
    }

    // Läbib kõik perekonnad
    ranges::for_each(it->second, [](const auto& family)
        {
            // Läbib ja prindib kõik kirjed 
            ranges::for_each(family.second, [](const Entry& entry)
                {
                    cout << entry << endl;
                });
        });
}

// Loendab kirjeid vastavalt sugukonnale
int Data::CountOrderEntries(string order)
{
    string searchOrder = ToLower(order);

    // Otsib  andmestruktuurist sobivat sugukonda
    auto it = ranges::find_if(DataStructure, [&](const auto& item)
        {
            return ToLower(item.first) == searchOrder;
        });

    // Kontroll, kas sugukond leiti
    if (it == DataStructure.end())
    {
        return 0;
    }

    int count = 0;

    // Läbib kõik perekonnad ja loeb kokku kirjete koguse
    ranges::for_each(it->second, [&](const auto& family)
        {
            count += static_cast<int>(family.second.size());
        });

    return count;
}

// Eemaldab kirjed vsatavalt sugukonnale
bool Data::RemoveOrderEntries(string order)
{
    string searchOrder = ToLower(order);
    bool removed = false;

    // Otsib  andmestruktuurist sobivat sugukonda
    auto it = ranges::find_if(DataStructure, [&](const auto& order)
        {
            return ToLower(order.first) == searchOrder;
        });

    // Kontroll, kas sugukond leiti
    if (it != DataStructure.end())
    {
        // Läbib kõik perekonnad ja lahutab maha eemaldatavate kirjete arvu
        ranges::for_each(it->second, [&](const auto& family)
            {
                nEntries -= static_cast<int>(family.second.size());
            });

        // Eeemaldab sugukonna koos perekondade ja kirjetega
        DataStructure.erase(it);
        removed = true;
    }

    return removed;
}

// Muudab kõik sõnad väikse tähega
string Data::ToLower(string str) const
{
    string result;

    for (unsigned char c : str)
    {
        // Kontrollib tähti ÕÄÖÜ
        switch (c)
        {
            // Õ -> õ
            case 0xD5: 
                result += char(0xF5); 
                break;

            // Ä -> ä
            case 0xC4: 
                result += char(0xE4); 
                break; 
            
            // Ö -> ö
            case 0xD6: 
                result += char(0xF6); 
                break;
            
            // Ü -> ü
            case 0xDC: 
                result += char(0xFC); 
                break; 

            // Kõikide muude jaoks kasutan tolower() funktsiooni
            default:
                result += static_cast<char>(tolower(c));
                break;
        }
    }

    return result;
}