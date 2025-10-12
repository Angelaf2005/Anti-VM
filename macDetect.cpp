#include "macDetect.h"

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

std::string classMac(const std::string& mac) {
    std::string prefix = mac.substr(0, 8);
    auto it = vm_mac_map.find(prefix);
    if (it != vm_mac_map.end())
        // Retorna el nombre del hipervisor si se encuentra en el mapa
        return it->second;
    return "Fisica";
}

bool isVirtualInterface(const std::string& ifname) {
    return (
        ifname == "lo" ||                          // loopback
        ifname.find("docker") == 0 ||              // Docker
        ifname.find("veth") == 0 ||                // veth
        ifname.find("virbr") == 0 ||               // libvirt
        ifname.find("br-") == 0 ||                 // manual
        ifname.find("tun") == 0 ||                 // VPN tunnel
        ifname.find("tap") == 0 ||                 // virtual tap
        ifname.find("vmnet") == 0 ||               // VMware virtual net
        ifname.find("enx") == 0 ||                 // USB Ethernet 
        ifname.find(":") != std::string::npos      // alias (eth0:1)
    );
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

            std::string type = classMac(mac);
            std::cout << "[MAC Detect] Interfaz principal activa: " << desc << ", MAC: " << mac << "\n";
            std::cout << "MAC OUI: " << type << "\n";

            if (type == "Fisica")
            {
                std::cout << "Sistema físico: Windows\n";
                return false;
            }
            else
            {
                return true;
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

    if (s->sll_halen < 6)
        continue;

    // Filtrar solo interfaz fisica principal
    if (isVirtualInterface(ifname))
    continue;


    std::string mac = formatMac(s->sll_addr, s->sll_halen);
    std::string type = classMac(mac);

    std::cout << "[MAC Detect] Interfaz principal activa: " << ifname << ", MAC: " << mac << "\n";
    std::cout << "MAC OUI: " << type << "\n";

    freeifaddrs(ifap); 

    if (type == "Fisica") {
        std::cout << "Sistema físico: Linux\n";
        return false;
    } else {
        return true;
    }
}


#endif

    

    std::cout << "[MAC Detect] No se detecto MAC de VM.\n";
    return false;

}

