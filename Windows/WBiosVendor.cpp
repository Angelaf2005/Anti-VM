#include <iostream>
#include <windows.h>
#include <string>
#include <algorithm>

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

bool isVirtualMachine() {
    std::string vendor = getBiosVendor();
    if (vendor.empty()) return false;

    std::string lowerVendor = vendor;
    std::transform(lowerVendor.begin(), lowerVendor.end(), lowerVendor.begin(), ::tolower);

    return (lowerVendor.find("vmware") != std::string::npos ||
        lowerVendor.find("virtualbox") != std::string::npos ||
        lowerVendor.find("innotek") != std::string::npos ||
        lowerVendor.find("qemu") != std::string::npos);
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
