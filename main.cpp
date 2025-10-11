#include "MacDetect.h"
#include <iostream>

int main() {
    bool isVM = detectVMByMac();
    std::cout << "MAC OUI: " << (isVM ? "VM detectada" : "No VM detectada") << "\n";
    return 0;
}
