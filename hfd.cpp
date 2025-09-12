#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>
using namespace std;

// Function to run a command and print output live
void runCommand(const string &cmd, const string &message)
{
    cout << "[*] " << message << endl;
    int ret = system(cmd.c_str());
    if (ret == 0)
        cout << "[✓] Command executed successfully.\n\n";
    else
        cout << "[✗] Command failed or returned non-zero status.\n\n";
    this_thread::sleep_for(chrono::milliseconds(500));
}

// Function to continuously monitor drive HPA/DCO status
void monitorDrive(const string &device)
{
    cout << "[*] Starting live drive monitor for " << device << " (Press Ctrl+C to stop)\n";
    while (true)
    {
        system(("clear && echo '=== DRIVE STATUS ===\nDevice: " + device + "'").c_str());

        // Display HPA status
        cout << "\n[HPA Status]\n";
        system(("sudo hdparm -N " + device).c_str());

        // Display DCO status
        cout << "\n[DCO Status]\n";
        system(("sudo hdparm --dco-identify " + device).c_str());

        cout << "\n--- Updating every 2 seconds ---\n";
        this_thread::sleep_for(chrono::seconds(2));
    }
}

int main()
{
    string device;
    cout << "Enter target device (e.g., /dev/sda): ";
    cin >> device;

    cout << "\n=== STARTING PROCESS ===\n\n";

    // 1. Unhide hidden files
    string unhideCmd = "for file in .*; do "
                       "if [ -f \"$file\" ]; then "
                       "mv \"$file\" \"${file#.}\"; "
                       "fi; "
                       "done";
    runCommand(unhideCmd, "Unhiding all hidden files in current directory...");

    // 2. Disable HPA
    string hpaCmd = "sudo hdparm --yes-i-know-what-i-am-doing --dco-restore " + device;
    runCommand(hpaCmd, "Disabling HPA...");

    // 3. Disable DCO
    string dcoCmd = "sudo hdparm --yes-i-know-what-i-am-doing --dco-restore " + device;
    runCommand(dcoCmd, "Disabling DCO...");

    // 4. Start live drive monitoring
    monitorDrive(device);

    return 0;
}
