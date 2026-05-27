#include "Application.h"

// Menüü
void Application::Run()
{
    try
    {
        Initialize();
    }
    catch (exception& e)
    {
        cout << "Initialize failed: " << e.what() << endl;
        return;
    }

    string command;

    cout << "\nType command: start, print, store, exit\n> ";

    // Tsükkel kasutaja käskude töötlemiseks
    while (true)
    {
        // Loetakse kasutaja sisestatud käsk.
        getline(cin, command);

        // Käsuga start käivitatakse item'ite generaator.
        if (command == "start")
        {
            StartGenerator();
        }
        // Käsuga print kuvatakse kõik andmed
        else if (command == "print")
        {
            if (data.CountAllEntries() == 0)
            {
                cout << "No entries.\n";
            }
            else
            {
                data.PrintAll();
            }

            cout << "\nType command\n> ";
        }
        // käsuga store salvestatakse andmed faili
        else if (command == "store")
        {
            StoreData();

            cout << "\nType command\n> ";
        }
        // Käsuga exit väljutakse programmist
        else if (command == "exit")
        {
            cout << "Exiting application.\n";
            break;
        }
        // Tundmatu käsu korral kuvab teate
        else
        {
            cout << "\nInvalid command.\n> ";
        }
    }
}

// Item'te genereerimine / thread'de tööle panemine
void Application::StartGenerator()
{
    // Loob jagatud objekti
    Control_A control;

    // Loob stop_source objekti thread'ide peatamiseks
    stop_source stopSource;
    // Salvestab stop tokeni control objekti sisse
    control.stop = stopSource.get_token();

    atomic<bool> running = true;

    // Producer thread
    jthread producer([&]()
        {
            try
            {
                // Käivitab DLL-is item'ite generaatori
                Start('A', &control);
            }
            catch (exception& e)
            {
                cout << "Producer error: " << e.what() << endl;
                running = false;
                stopSource.request_stop(); // Saadab stop käsu kõikidele thread'idele
                control.cva.notify_all(); // Ärkab üles kõik wait() seisundis thread'id
            }
        });

    // Consumer thread
    jthread consumer([&]()
        {
            try
            {
                // Võtab järjekorrast item'id ja lisab andmestruktuuri
                Consumer(control, data);
            }
            catch (exception& e)
            {
                cout << "Consumer error: " << e.what() << endl;
                running = false;
                stopSource.request_stop(); // Saadab stop käsu kõikidele thread'idele
                control.cva.notify_all(); // Ärkab üles kõik wait() seisundis thread'id
            }
        });

    cout << "Generator is running\n";

    // Main(keyboard) thread ootab Esc klahvi vajutamist
    while (running)
    {
        // Kontrollib kas klaviatuurilt vajutati klahvi
        if (_kbhit())
        {
            // Loeb vajutatud klahvi ASCII väärtuse
            int key = _getch();

            // Kontrollib kas vajutati Esc klahvi
            if (key == 27)
            {
                cout << "\nEsc pressed. Stopping generator...\n";

                running = false;
                stopSource.request_stop(); // Saadab stop käsu kõikidele thread'idele
                control.cva.notify_all(); // Ärkab üles kõik wait() seisundis thread'id
            }
        }

        this_thread::sleep_for(chrono::milliseconds(50));  // Väike paus
    }

    cout << "> ";
}

// Salvestab andmed Faili
void Application::StoreData()
{
    // Kontroll kas andmed on olemas
    if (data.CountAllEntries() == 0)
    {
        cout << "No entries.\n";
        return;
    }

    string directoryName;

    cout << "Type complete path to directory: ";

    // Loeb kasutaja sisendi
    getline(cin, directoryName);    

    // Teisendab string'i kausta asukohaks
    fs::path directoryPath(directoryName);

    // Kontroll, et sisend oleks korrektne (c:\Folder\Test)
    if (!directoryPath.is_absolute())
    {
        cout << "Invalid directory path.\n";
        return;
    }

    // Kontroll, kas on vaja luua uus kaust
    if (!fs::exists(directoryPath))
    {
        string answer;

        cout << "Directory does not exist. Create it? yes/no: ";
        getline(cin, answer);

        // Kontroll, kas kausta soovitakse luua
        if (answer != "yes")
        {
            cout << "Store command cancelled.\n";
            return;
        }

        // Proovib luua kausta
        try
        {
            fs::create_directories(directoryPath);
        }
        catch (...)
        {
            cout << "Could not create directory.\n";
            return;
        }
    }

    // Kontroll, et tegemist on kaustaga
    if (!fs::is_directory(directoryPath))
    {
        cout << "The path is not a directory.\n";
        return;
    }

    cout << "Files in directory:\n";

    // Prindib välja kõik failid antud kaustas
    try
    {
        for (const auto& file : fs::directory_iterator(directoryPath))
        {
            cout << file.path().filename().string() << endl;
        }
    }
    catch (...)
    {
        cout << "Could not list directory files.\n";
        return;
    }

    string fileName;

    // Küsitakse kasutajalt faili nime
    cout << "Type text file name: ";
    getline(cin, fileName);

    // Kontroll, et kasutaja on midagi sisestanud
    if (fileName.empty())
    {
        cout << "Incorrect file name.\n";
        return;
    }

    // Moodustab täieliku faili asukoha
    fs::path filePath = directoryPath / fileName;

    // Määrab faili avamise režiimi kirjutamiseks
    ios::openmode mode = ios::out;

    // Kontrollib kas fail on juba olemas
    if (fs::exists(filePath))
    {
        string answer;

        cout << "File already exists. Append? yes/no: ";
        getline(cin, answer);

        if (answer == "yes")
        {
            // avab faili lisamisrežiimis
            // Liidetakse mode'le juurde 
            mode |= ios::app;
        }
        else if (answer == "no")
        {
            // Tühjendab vana faili sisu enne kirjutamist
            // Liidetakse mode'le juurde 
            mode |= ios::trunc;
        }
        else
        {
            cout << "Incorrect answer. Store command cancelled.\n";
            return;
        }
    }

    // Avab faili määratud režiimis
    // või loob uue fili, kui faili ei eksisteeri
    fstream file(filePath, mode);

    // Kontroll kas faili avamine õnnestus
    if (!file)
    {
        cout << "File opening failed.\n";
        return;
    }

    // Salvestab kõik andmed faili
    data.StoreAll(file);

    // Sulgeb faili
    file.close();

    cout << "Data stored successfully.\n";
}