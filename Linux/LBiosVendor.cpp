#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

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
    std::cout << "Detector de VM Linux" << std::endl;
    std::string vendor = getBiosVendor();
    bool isVM = isVirtualMachine();

    std::cout << "Fabricante del BIOS: " << vendor << std::endl;
    std::cout << "Maquina virtual detectada: " << (isVM ? "SI" : "NO") << std::endl;

    return 0;
}
*/
