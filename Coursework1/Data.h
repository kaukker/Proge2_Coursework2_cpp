#pragma once

#include <map>              
#include <set>              
#include <list>             
#include <string>   
#include <cctype>
#include <initializer_list>


#include "Entry.h"          
#include "Item.h"

using namespace std;

class Data
{
private:
	map<string, map<string, set<Entry>>> DataStructure{ };  // It is a C++ map in which the Entry members Order are the keys. 
	// The values are inner C++ maps in which the keys are Entry members
	// Family  and the values are sets. The sets contain entries belonging
	// to the specified order and family.

	int nEntries = 0;                                       // total number of entries in DataStructure
	string ToLower(std::string str) const;

public:
	Data() {}                                               // default constructor

	~Data() {}                                              // default destructor

	int CountAllEntries();                                  // returns the total number of entries in DataStructure

	void PrintAll();                                        // prints all the entries that are currently stored in 
	// the DataStructure

	void StoreAll(fstream&);								// stores all entries currently stored in DataStructure into a file

	bool InsertEntry(Item* pI);                             // creates the entry and inserts into the data structure and
	// releases the memory of input value

	bool InsertEntries(initializer_list<Item*> items);     // creates the entries and inserts into the data structure and
	// releases the memory of input values

	void PrintEntries(string species) noexcept(false);      // prints all the entries presenting the given bird species 


	Entry* GetSummaryEntry(string species);                 // returns a new entry in which all the observations concerning 
	// the given bird species are combined into one vector

	list<Entry>* GetEntries(string species);                // returns the pointer to list containing all the entries
	// presenting the given bird species

	int CountEntries(string species);                       // returns the number of entites presenting the given bird species

	bool RemoveEntries(string species);                     // removes all the entries presenting the given bird species 

	void PrintFamilyEntries(string family) noexcept(false); // prints all the entries presenting the birds from the  
	// specified family

	int CountFamilyEntries(string family);                  // returns the number of entites presenting the birds from the  
	// specified family

	bool RemoveFamilyEntries(string family);                // removes all the entities presenting the birds from the  
	// specified family

	void PrintOrderEntries(string order) noexcept(false);  // prints all the entries presenting the birds from the  
	// specified order

	int CountOrderEntries(string order);                   // returns the number of entites presenting the birds from the  
	// specified order

	bool RemoveOrderEntries(string order);                 // removes all the entities  presenting the birds from the  
	// specified order
};