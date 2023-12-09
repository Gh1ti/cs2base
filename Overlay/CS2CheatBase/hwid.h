#pragma once
#include <iostream>
#include <windows.h>
#include <sstream>
#include <intrin.h>
#include <thread>


std::string GetHardwareID() {
    std::ostringstream hardwareID;

    // Get volume information
    DWORD serialNumber;
    if (GetVolumeInformationA("C:\\", NULL, 0, &serialNumber, NULL, NULL, NULL, 0)) {
        hardwareID << serialNumber;
    }

    // Get processor information
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    hardwareID << systemInfo.dwProcessorType << systemInfo.dwNumberOfProcessors;

    // Get BIOS information
    int biosID[8] = { 0 };
    __cpuid(biosID, 0);
    for (int i = 0; i < 8; i++) {
        hardwareID << biosID[i];
    }

    return hardwareID.str();
}