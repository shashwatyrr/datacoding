#include <bits/stdc++.h> // Includes almost all common C++ headers
#include <cstdio>        // For _popen() and _pclose()
#include <memory>        // For smart pointers
#include <stdexcept>     // For runtime_error
#include <string>
#include <array>
#include <fstream> // For writing to files
using namespace std;

/*
    Function: execCommand
    ---------------------
    Runs a shell command (PowerShell in this case) and captures the output.
*/
string execCommand(const char *cmd)
{
    array<char, 512> buffer;
    string result;
    unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe)
    {
        throw runtime_error("_popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}

int main()
{
    try
    {
        cout << "🚀 Starting Storage Report Generation...\n";

        // -------------------------------
        // Create output files
        // -------------------------------
        cout << "📂 Creating output files...\n";
        ofstream txtfile("storage_report.txt");
        ofstream jsonfile("storage_report.json");

        // -------------------------------
        // Step 1: Detect storage devices
        // -------------------------------
        cout << "🔍 Detecting storage devices...\n";
        string deviceJson = execCommand(
            "powershell -Command \"Get-PhysicalDisk | Select FriendlyName,SerialNumber,MediaType,Size,FirmwareVersion | ConvertTo-Json -Depth 2\"");
        string deviceText = execCommand(
            "powershell -Command \"Get-PhysicalDisk | Format-Table -AutoSize\"");

        // Write to TXT
        txtfile << "=== Detected Storage Devices (Get-PhysicalDisk output) ===\n";
        txtfile << deviceText << "\n\n";
        cout << "✅ Storage device list collected.\n";

        // -------------------------------
        // Step 2: Get disk sizes in GB
        // -------------------------------
        cout << "💾 Calculating sizes in GB...\n";
        string sizeText = execCommand(
            "powershell -Command \"Get-PhysicalDisk | ForEach-Object { $_.FriendlyName + ' : ' + [math]::Round($_.Size/1GB,2) + ' GB' }\"");

        // Write to TXT
        txtfile << "=== Storage Sizes in GB ===\n";
        txtfile << sizeText << "\n\n";
        cout << "✅ Sizes in GB recorded.\n";

        // -------------------------------
        // Step 3: Write JSON Report
        // -------------------------------
        cout << "📝 Writing JSON report...\n";
        jsonfile << "{\n";
        jsonfile << "  \"physical_disks\": " << deviceJson << "\n";
        jsonfile << "}\n";
        cout << "✅ JSON report created.\n";

        // -------------------------------
        // Close files
        // -------------------------------
        txtfile.close();
        jsonfile.close();

        cout << "🎉 Reports saved as storage_report.txt and storage_report.json\n";
    }
    catch (const exception &e)
    {
        cerr << "❌ Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
