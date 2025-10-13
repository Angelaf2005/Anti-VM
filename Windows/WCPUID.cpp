#include <iostream>
#include <array>
#include <cpuid.h>


bool isHypervisorPresent() {
    std::array<int, 4> cpuInfo;
    __cpuid(1, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
    return (cpuInfo[2] >> 31) & 1;
}


