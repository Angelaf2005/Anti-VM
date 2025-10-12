#pragma once

#include <vector>
#include <string>
#include <unordered_map>

// Lista de prefijos MAC conocidos de hipervisores
const std::unordered_map<std::string, std::string> vm_mac_map = {
    {"00:05:69", "VMware"},
    {"00:0C:29", "VMware"},
    {"00:50:56", "VMware"},
    {"00:1C:14", "VMware"},
    {"08:00:27", "VirtualBox"},
    {"0A:00:27", "VirtualBox"}
};

extern bool macWHost;
extern bool macWVBox;
extern bool macWVMware;
extern bool macLHost;
extern bool macLVBox;
extern bool macLVMware;

std::string formatMac(const unsigned char* mac, size_t len = 6);
std::string classMac(const std::string& mac);
bool isVirtualInterface(const std::string& ifname);
void detectVMByMac();
