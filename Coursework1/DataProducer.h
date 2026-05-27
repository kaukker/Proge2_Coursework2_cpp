#pragma once

#ifdef DATAPRODUCER_EXPORTS
#define LIBSPEC extern "C" _declspec(dllexport)
#else
#define LIBSPEC extern "C" _declspec(dllimport)
#endif
class Item;

LIBSPEC void Initialize();
LIBSPEC Item* GetItem();
LIBSPEC Item* GetTestItem(int);
LIBSPEC void Start(char c,void *);


