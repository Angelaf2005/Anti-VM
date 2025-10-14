#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "LBiosVendor.h"

std::string getBiosVendor() {
    std::ifstream file("/sys/class/dmi/id/bios_vendor");
    if (file.is_open()) {
        std::string vendor;
        std::getline(file, vendor);
        file.close();
        return vendor;
    }
    return ""; 
}

void isVirtualMachine() {
    std::string vendor = getBiosVendor();
    if (vendor.empty()) return;

    std::string lowerVendor = vendor;
    std::transform(lowerVendor.begin(), lowerVendor.end(), lowerVendor.begin(), ::tolower);

    if (lowerVendor.find("vmware") != std::string::npos) {
        biosLinVMware = true;
        return;
    } else if (lowerVendor.find("virtualbox") != std::string::npos) {
        biosLinVBox = true;
        return;
    } else{
        biosLinHost = true;
        return;
        }
    return;
}
