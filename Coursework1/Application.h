#pragma once
#include <fstream>
#include <filesystem>
#include <thread>
#include <atomic>
#include <conio.h>

#include "Main.h"
#include "Control.h"
#include "Consumer.h"

using namespace std;
namespace fs = std::filesystem;

class Application
{
private:
    Data data;

    void StartGenerator();
    void StoreData();

public:
    void Run();
};

