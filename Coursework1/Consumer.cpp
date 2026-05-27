#include "Consumer.h"

// Eemaldab item'id Control_A järjekorrast ja salvestab
// need andmestruktuuri
void Consumer(Control_A& control, Data& data)
{
    // Töötab seni kuni peatmaimis käsku pole saadetud
    while (!control.stop.stop_requested())
    {
        // Loob luku ja lukustab mutexi
        unique_lock<mutex> lock(control.mx);

        // Ootab kuni järjekirda lisatakse elemente
        // või programm peatatatakse
        control.cva.wait(lock, control.stop, [&]()
            {
                return !control.Items.empty();
            });

        // Töötleb kõik järjekorras olevad elemendid
        while (!control.Items.empty())
        {
            // Võtab ja eemaldab esimese elemendi järjekorrast
            Item* item = control.Items.front();
            control.Items.pop();

            // Vabastab mutexi
            lock.unlock();

            // Lisab elemdid andmestruktuuri
            data.InsertEntry(item); 

            // Lukustab mutexi
            lock.lock();
        }

        // Saadab teate, et järjekord on tühi
        control.cva.notify_one();
    }
}