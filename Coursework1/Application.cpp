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
            cout << "Invalid command.\n";
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
    if (data.CountAllEntries() == 0)
    {
        cout << "No entries to store.\n";
        return;
    }

    string directoryName;

    cout << "Type complete path to directory: ";
    getline(cin, directoryName);

    if (directoryName.length() < 3 || !isalpha(directoryName[0]) ||
        directoryName[1] != ':' || directoryName[2] != '\\')
    {
        cout << "Invalid directory path. Use full path like C:\\Folder.\n";
        return;
    }

    fs::path directoryPath(directoryName);

    if (!fs::exists(directoryPath))
    {
        string answer;

        cout << "Directory does not exist. Create it? yes/no: ";
        getline(cin, answer);

        if (answer != "yes")
        {
            cout << "Store command cancelled.\n";
            return;
        }

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

    if (!fs::is_directory(directoryPath))
    {
        cout << "The path is not a directory.\n";
        return;
    }

    cout << "Files in directory:\n";

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

    cout << "Type text file name: ";
    getline(cin, fileName);

    if (fileName.empty())
    {
        cout << "Incorrect file name.\n";
        return;
    }

    fs::path filePath = directoryPath / fileName;

    ios::openmode mode = ios::out;

    if (fs::exists(filePath))
    {
        string answer;

        cout << "File already exists. Append? yes/no: ";
        getline(cin, answer);

        if (answer == "yes")
        {
            mode |= ios::app;
        }
        else if (answer == "no")
        {
            mode |= ios::trunc;
        }
        else
        {
            cout << "Incorrect answer. Store command cancelled.\n";
            return;
        }
    }

    fstream file(filePath, mode);

    if (!file)
    {
        cout << "File opening failed.\n";
        return;
    }

    data.StoreAll(file);

    file.close();

    cout << "Data stored successfully.\n";
}