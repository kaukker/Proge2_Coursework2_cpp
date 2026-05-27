#pragma once
#include <stop_token>
#include <mutex>
#include <condition_variable>
#include <queue>

#include "Item.h"

using namespace std;

class Control_A
{
public:
	stop_token stop;
	mutex mx;
	condition_variable_any cva;
	queue<Item*> Items;
};