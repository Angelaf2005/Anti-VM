#include "MacDetect.h"

#include <iostream>

#ifdef _WIN32
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#else
#include <ifaddrs.h>
#include <netpacket/packet.h>
#include <net/if.h>
#include <cstring>
#endif


// Convierte MAC binaria a string formateada
std::string formatMac(unsigned char* mac, size_t len) {
    char buf[18];
    snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X",mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return std::string(buf);
}

// Compara si el prefijo MAC coincide con alguno de los OUIs
bool isVmMac(const std::string& mac) {
    for (const auto& prefix : vm_mac_pref) {
        if (mac.substr(0, prefix.size()) == prefix)
            return true;
    }
    return false;
}

bool detectVMByMac() {
#ifdef _WIN32
  IP_ADAPTER_INFO adapterInfo[16];
    DWORD buflen = sizeof(adapterInfo);
    if (GetAdaptersInfo(adapterInfo, &buflen) != ERROR_SUCCESS)
        return false;

    PIP_ADAPTER_INFO pAdapter = adapterInfo;
    while (pAdapter) {
        std::string desc = pAdapter->Description;
        std::string mac = formatMac(pAdapter->Address, pAdapter->AddressLength);

        // Filtrar solo la primera interfaz fisica activa
        if (pAdapter->Type == IF_TYPE_ETHERNET_CSMACD &&
            pAdapter->AddressLength >= 6 &&
            desc.find("VMware") == std::string::npos &&
            desc.find("Virtual") == std::string::npos &&
            desc.find("Loopback") == std::string::npos &&
            desc.find("Pseudo") == std::string::npos) {

            std::cout << "[MAC Detect] Interfaz principal activa: " << desc << ", MAC: " << mac << "\n";

            if (isVmMac(mac)) {
                std::cout << "MAC OUI: VM detectada.\n";
                return true;
            } else {
                std::cout << "MAC OUI: Sistema fisico detectado.\n";
                return false;
            }
        }

        pAdapter = pAdapter->Next;
    }

    std::cout << "[MAC Detect] No se encontro interfaz física activa.\n";
    return false;


#else
  struct ifaddrs* ifap;
    if (getifaddrs(&ifap) != 0)
        return false;

    for (struct ifaddrs* p = ifap; p; p = p->ifa_next) {
        if (!p->ifa_addr || p->ifa_addr->sa_family != AF_PACKET)
            continue;

        if (!(p->ifa_flags & IFF_UP))
            continue;

        std::string ifname = p->ifa_name;
        struct sockaddr_ll* s = (struct sockaddr_ll*)p->ifa_addr;
        std::string mac = formatMac(s->sll_addr);

        std::cout << "[MAC Detect] Activa: " << ifname << ", MAC: " << mac << "\n";

        if (isVmMac(mac)) {
            std::cout << "[MAC Detect] VM detectada por MAC activa: " << mac << "\n";
            freeifaddrs(ifap);
            return true;
        }
    }

    freeifaddrs(ifap);
    std::cout << "[MAC Detect] No se detectó MAC de VM en interfaces activas.\n";
    return false;


#endif

    std::cout << "[MAC Detect] No se detecto MAC de VM.\n";
    return false;

}

