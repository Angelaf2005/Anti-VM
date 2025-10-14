#ifndef VM_DETECTION_H
#define VM_DETECTION_H

#include <string>
extern bool biosWinVMware;
extern bool biosWinVBox;
extern bool biosWinHost;

std::string getBiosVendor();
void isVirtualMachine();

#endif // VM_DETECTION_H