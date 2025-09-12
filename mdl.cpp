#include <bits/stdc++.h> // Includes almost all standard C++ libraries
#include <cstdio>        // For popen() and pclose()
#include <memory>        // For smart pointers
#include <stdexcept>     // For runtime_error
#include <string>
#include <array>
#include <fstream> // For writing to files
using namespace std;

/*
    Function: execCommand
    ---------------------
    Runs a shell command and returns its output as a string.
    - cmd: command to run (like "lsblk" or "smartctl").
    - Uses popen() to execute the command and read its output.
*/
string execCommand(const char *cmd)
{
    array<char, 128> buffer; // Temporary storage for command output
    string result;           // Final output string
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

    if (!pipe)
    {
        throw runtime_error("popen() failed!"); // If popen fails, throw error
    }

    // Read command output line by line
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }

    return result; // Return the full command output as a string
}

int main()
{
    try
    {
        // -------------------------------
        // Create 2 files:
        // 1. Human-readable text report
        // 2. Machine-readable JSON report
        // -------------------------------
        ofstream txtfile("storage_report.txt");
        ofstream jsonfile("storage_report.json");

        // -------------------------------
        // Step 1: Detect storage devices
        // -------------------------------
        // "lsblk -d" lists storage devices (disks only, not partitions).
        // "-o" specifies columns: NAME, TYPE, SIZE, MODEL, SERIAL
        // "-J" outputs JSON directly.
        string devices = execCommand("lsblk -d -o NAME,TYPE,SIZE,MODEL,SERIAL -J");

        // Save plain text version of lsblk output
        txtfile << "=== Detected Storage Devices (lsblk output) ===\n";
        string devicesText = execCommand("lsblk -d -o NAME,TYPE,SIZE,MODEL,SERIAL");
        txtfile << devicesText << "\n\n";

        // -------------------------------
        // Step 2: Get detailed SMART info
        // -------------------------------
        // Extract only disk names (not partitions) using awk
        string deviceList = execCommand("lsblk -nd -o NAME,TYPE | awk '$2==\"disk\" {print $1}'");

        txtfile << "=== Device Metadata (SMART Info) ===\n";
        istringstream iss(deviceList);
        string dev;
        vector<string> jsonDevices; // Will store SMART info in JSON format

        // Loop through each detected disk
        while (iss >> dev)
        {
            txtfile << "---- /dev/" << dev << " ----\n";

            // Get SMART info in plain text
            string info = execCommand(("sudo smartctl -i /dev/" + dev).c_str());
            txtfile << info << "\n";

            // Get SMART info in JSON format (-j flag in smartctl)
            string jsoninfo = execCommand(("sudo smartctl -i -j /dev/" + dev).c_str());
            jsonDevices.push_back(jsoninfo);
        }

        // -------------------------------
        // Step 3: Write JSON report
        // -------------------------------
        // Final JSON will contain:
        //   1. lsblk device list
        //   2. SMART info for each disk
        jsonfile << "{\n";
        jsonfile << "  \"lsblk_devices\": " << devices << ",\n";
        jsonfile << "  \"smart_devices\": [\n";

        for (size_t i = 0; i < jsonDevices.size(); i++)
        {
            jsonfile << jsonDevices[i];
            if (i + 1 < jsonDevices.size())
                jsonfile << ","; // Add comma between JSON objects
            jsonfile << "\n";
        }

        jsonfile << "  ]\n";
        jsonfile << "}\n";

        // Close files
        txtfile.close();
        jsonfile.close();

        cout << "Reports saved as storage_report.txt and storage_report.json\n";
    }
    catch (const exception &e)
    {
        cerr << "Error: " << e.what() << "\n";
        return 1; // Exit with error code
    }

    return 0; // Program executed successfully
}
