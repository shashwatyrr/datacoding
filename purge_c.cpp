#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
using namespace std;

int main()
{
    string deviceFile = "device.txt"; // file containing device name
    string device;

    // Read device name from file
    ifstream infile(deviceFile);
    if (!infile)
    {
        cerr << "Error: Cannot open " << deviceFile << endl;
        return 1;
    }
    getline(infile, device);
    infile.close();

    cout << "Target device: " << device << endl;
    cout << "Starting crypto erase...\n";

    // hdparm command for crypto erase (instant secure erase for encrypted drives)
    string cmd = "sudo hdparm --user-master u --security-erase NULL " + device;

    cout << "Running command: " << cmd << "\n";

    // Execute the command
    int ret = system(cmd.c_str());
    if (ret == 0)
        cout << "\nCrypto erase completed successfully. All data is gone.\n";
    else
        cout << "\nCrypto erase failed or drive does not support it.\n";

    return 0;
}
