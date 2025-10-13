#include <iostream>
#include <cpuid.h>
bool hayHypervisor() {
 unsigned int eax, ebx, ecx, edx;
 __get_cpuid(1, &eax, &ebx, &ecx, &edx);
 return (ecx >> 31) & 1;
}
