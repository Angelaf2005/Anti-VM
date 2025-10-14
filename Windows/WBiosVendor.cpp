#include <iostream>
#include <windows.h>
#include <string>
#include <algorithm>
#include "WBiosVendor.h"

bool biosWinVMware = false;
bool biosWinVBox = false;
bool biosWinHost = false;

std::string getBiosVendor() {
    HKEY hKey;
    char vendor[256] = { 0 };
    DWORD size = sizeof(vendor);

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "HARDWARE\\DESCRIPTION\\System\\BIOS",
        0, KEY_READ, &hKey) == ERROR_SUCCESS) {

        if (RegQueryValueExA(hKey, "SystemManufacturer", NULL, NULL,
            (BYTE*)vendor, &size) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return std::string(vendor);
        }
        RegCloseKey(hKey);
    }
    return "";
}

void isVirtualMachine() {
    std::string vendor = getBiosVendor();
    if (vendor.empty()) return;

    std::string lowerVendor = vendor;
    std::transform(lowerVendor.begin(), lowerVendor.end(), lowerVendor.begin(), ::tolower);

    if (lowerVendor.find("vmware") != std::string::npos) {
        biosWinVMware = true;
        return;
    } else if (lowerVendor.find("virtualbox") != std::string::npos) {
        biosWinVBox = true;
        return;
    } else{
        biosWinHost = true;
        return;
        }
    return;
}

    

/*
int main() {
    std::cout << "Detector de VM por BIOS Vendor" << std::endl;
    std::string vendor = getBiosVendor();
    bool isVM = isVirtualMachine();

    std::cout << "Fabricante del BIOS: " << vendor << std::endl;
    std::cout << "Maquina virtual detectada: " << (isVM ? "SI" : "NO") << std::endl;

    return 0;
}
*/
