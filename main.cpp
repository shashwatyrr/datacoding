#include <iostream>
#include <cstdlib>
using namespace std;

// Function to run another cpp executable
void runProgram(const string &program, const string &description)
{
    cout << "\n[*] Running: " << description << endl;
    int ret = system(program.c_str());
    if (ret == 0)
        cout << "[✓] " << description << " completed successfully.\n";
    else
        cout << "[✗] " << description << " failed or was interrupted.\n";
}

int main()
{
    int choice;

    while (true)
    {
        cout << "\n=== Disk Wipe / Secure Erase Controller ===\n";
        cout << "1. Zero Overwrite\n";
        cout << "2. Random Overwrite\n";
        cout << "3. Crypto Erase\n";
        cout << "4. ATA Secure Erase\n";
        cout << "5. NVMe Secure Erase\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            runProgram("./clean0.cpp", "Zero Overwrite"); // compile zero overwrite as zero_wipe
            break;
        case 2:
            runProgram("./clean_r.cpp", "Random Overwrite"); // compile random overwrite as random_wipe
            break;
        case 3:
            runProgram("./purge_c.cpp", "Crypto Erase"); // compile crypto erase as crypto_erase
            break;
        case 4:
            runProgram("./purge_ata.cpp", "ATA Secure Erase"); // compile ATA erase as ata_erase
            break;
        case 5:
            runProgram("./purge_nvme.cpp", "NVMe Secure Erase"); // compile NVMe erase as nvme_erase
            break;
        case 6:
            cout << "Exiting...\n";
            return 0;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}
