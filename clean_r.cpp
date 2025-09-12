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
    cout << "Starting disk wipe with RANDOM data using dd...\n";

    // Construct dd command using /dev/urandom
    string cmd = "sudo dd if=/dev/urandom of=" + device + " bs=1M status=progress";

    cout << "Running command: " << cmd << "\n";

    // Execute the command
    int ret = system(cmd.c_str());
    if (ret == 0)
        cout << "\nRandom disk wipe completed successfully. All data is gone.\n";
    else
        cout << "\nDisk wipe failed or was interrupted.\n";

    return 0;
}
