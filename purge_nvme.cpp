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

    cout << "Target NVMe device: " << device << endl;
    cout << "Starting NVMe Secure Erase...\n";

    // Construct NVMe secure erase command
    string cmd = "sudo nvme format " + device + " -s 1";

    cout << "Running command: " << cmd << "\n";

    int ret = system(cmd.c_str());
    if (ret == 0)
        cout << "\nNVMe Secure Erase completed successfully. All data is gone.\n";
    else
        cout << "\nNVMe Secure Erase failed or drive does not support it.\n";

    return 0;
}
