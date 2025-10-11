#pragma once

#include <vector>
#include <string>

// Lista de prefijos MAC conocidos de hipervisores
const std::vector<std::string> vm_mac_pref = {
    "00:05:69", "00:0C:29", "00:50:56", // VMware
    "08:00:27",                         // VirtualBox
    "00:15:5D"                          // Hyper-V
};


std::string formatMac(const unsigned char* mac, size_t len = 6);
bool isVmMac(const std::string& mac);
bool detectVMByMac();
