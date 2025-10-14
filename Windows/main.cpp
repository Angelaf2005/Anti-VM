#include "macDetect.h"
#include <iostream>
#include "WBiosVendor.h"
#include "WCPUID.h"

void printDetectionTable(bool cpuidWinHost, bool cpuidWinVBox, bool cpuidWinVMware,
                         bool cpuidLinHost, bool cpuidLinVBox, bool cpuidLinVMware,
                         bool biosWinHost, bool biosWinVBox, bool biosWinVMware,
                         bool biosLinHost, bool biosLinVBox, bool biosLinVMware,
                         bool macWinHost, bool macWinVBox, bool macWinVMware,
                         bool macLinHost, bool macLinVBox, bool macLinVMware)
{
    std::cout << "## Tabla de Resultados\n";
    std::cout << "| Tecnica               | Win-Host | Win-VM (VBox) | Win-VM (VMware) | Lin-Host | Lin-VM (VBox) | Lin-VM (VMware) |\n";
    std::cout << "|-----------------------|----------|---------------|-----------------|----------|---------------|-----------------|\n";

    std::cout << "| CPUID Hypervisor Bit  | "
              << (cpuidWinHost ? "Si" : "No") << "       | "
              << (cpuidWinVBox ? "Si" : "No") << "            | "
              << (cpuidWinVMware ? "Si" : "No") << "              | "
              << (cpuidLinHost ? "Si" : "No") << "       | "
              << (cpuidLinVBox ? "Si" : "No") << "            | "
              << (cpuidLinVMware ? "Si" : "No") << "              |\n";

    std::cout << "| BIOS Vendor String    | "
              << (biosWinHost ? "Si" : "No") << "       | "
              << (biosWinVBox ? "Si" : "No") << "            | "
              << (biosWinVMware ? "Si" : "No") << "              | "
              << (biosLinHost ? "Si" : "No") << "       | "
              << (biosLinVBox ? "Si" : "No") << "            | "
              << (biosLinVMware ? "Si" : "No") << "              |\n";

    std::cout << "| MAC OUI               | "
              << (macWinHost ? "Si" : "No") << "       | "
              << (macWinVBox ? "Si" : "No") << "            | "
              << (macWinVMware ? "Si" : "No") << "              | "
              << (macLinHost ? "Si" : "No") << "       | "
              << (macLinVBox ? "Si" : "No") << "            | "
              << (macLinVMware ? "Si" : "No") << "              |\n";
}

int main() {

    detectVMByMac();
    isVirtualMachine();
    HypervisorPresente();
    printDetectionTable(
    cpuidWinHost, cpuidWinVBox, cpuidWinVMware,
    false, false, false,   // CPUID
    biosWinHost, biosWinVBox, biosWinVMware,   // BIOS
    false, false, false,
    macWHost, macWVBox, macWVMware,   // MAC
    macLHost, macLVBox, macLVMware
);
   
    return 0;
}
