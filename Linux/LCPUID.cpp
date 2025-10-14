#include <iostream>
#include <array>
#include <cpuid.h>
#include <cstring>
#include <string>
#include "LCPUID.h"

bool cpuidLinVMware = false;
bool cpuidLinVBox = false;
bool cpuidLinHost = false;



void HypervisorPresente() {
    std::array<int, 4> cpuInfo;
    __cpuid(1, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
    bool hypervisorBit = (cpuInfo[2] >> 31) & 1;

    if (!hypervisorBit) {
        cpuidLinHost = true;
        return;
    }

    char hypervisorId[13] = {};
    __cpuid(0x40000000, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
    std::memcpy(hypervisorId, &cpuInfo[1], 4); // EBX
    std::memcpy(hypervisorId + 4, &cpuInfo[2], 4); // ECX
    std::memcpy(hypervisorId + 8, &cpuInfo[3], 4); // EDX
    hypervisorId[12] = '\0';

    std::string id(hypervisorId);
    if (id == "VMwareVMware") {
        cpuidLinVMware = true;
    } else if (id == "VBoxVBoxVBox") {
        cpuidLinVBox = true;
    } else {
        cpuidLinHost = true; // 
    }

    return;


}
