#include "macDetect.h"

#include <iostream>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#else
#include <ifaddrs.h>
#include <netpacket/packet.h>
#include <net/if.h>
#include <cstring>
#endif

bool macWHost = false;
bool macWVBox = false;
bool macWVMware = false;
bool macLHost = false;
bool macLVBox = false;
bool macLVMware = false;

// Convierte MAC binaria a string formateada
std::string formatMac(const unsigned char *mac, size_t len)
{
    char buf[18];
    snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return std::string(buf);
}

std::string classMac(const std::string &mac)
{
    std::string prefix = mac.substr(0, 8);
    auto it = vm_mac_map.find(prefix);
    if (it != vm_mac_map.end())
        // Retorna el nombre del hipervisor si se encuentra en el mapa
        return it->second;
    return "Fisica";
}

bool isVirtualInterface(const std::string &ifname)
{
    return (
        ifname == "lo" ||                     // loopback
        ifname.find("docker") == 0 ||         // Docker
        ifname.find("veth") == 0 ||           // veth
        ifname.find("virbr") == 0 ||          // libvirt
        ifname.find("br-") == 0 ||            // manual
        ifname.find("tun") == 0 ||            // VPN tunnel
        ifname.find("tap") == 0 ||            // virtual tap
        ifname.find("vmnet") == 0 ||          // VMware virtual net
        ifname.find("enx") == 0 ||            // USB Ethernet
        ifname.find(":") != std::string::npos // alias (eth0:1)
    );
}

void detectVMByMac()
{
#ifdef _WIN32
    ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
    ULONG family = AF_UNSPEC;
    ULONG outBufLen = 0;
    PIP_ADAPTER_ADDRESSES addresses = nullptr;
    ULONG res = GetAdaptersAddresses(family, flags, NULL, addresses, &outBufLen);
    if (res == ERROR_BUFFER_OVERFLOW)
    {
        addresses = (PIP_ADAPTER_ADDRESSES)malloc(outBufLen);
        if (!addresses)
            return;
        res = GetAdaptersAddresses(family, flags, NULL, addresses, &outBufLen);
    }
    if (res != NO_ERROR)
    {
        if (addresses)
            free(addresses);
        return;
    }

    for (PIP_ADAPTER_ADDRESSES adapter = addresses; adapter; adapter = adapter->Next)
    {
        // solo adaptadores operativos
        if (adapter->OperStatus != IfOperStatusUp)
            continue;

        if (adapter->PhysicalAddressLength < 6)
            continue;

        std::string name;
        if (adapter->FriendlyName)
        {
            int len = WideCharToMultiByte(CP_UTF8, 0, adapter->FriendlyName, -1, NULL, 0, NULL, NULL);
            if (len > 0)
            {
                name.resize(len - 1);
                WideCharToMultiByte(CP_UTF8, 0, adapter->FriendlyName, -1, &name[0], len, NULL, NULL);
            }
        }

        unsigned char *phys = adapter->PhysicalAddress;
        std::string mac = formatMac(reinterpret_cast<const unsigned char *>(phys), adapter->PhysicalAddressLength);
        // std::cout << "[MAC Detect] Interfaz principal activa: " << (name.empty() ? "<sin nombre>" : name) << ", MAC: " << mac << endl;
        if (!name.empty())
        {
            if (name.find("VMware") != std::string::npos || name.find("Virtual") != std::string::npos || name.find("Pseudo") != std::string::npos || name.find("Loopback") != std::string::npos)
                continue;
        }

        std::string type = classMac(mac);
        if (type == "Fisica")
        {
            macWHost = true;
            if (addresses)
                free(addresses);
            return;
        }
        else if (type == "VirtualBox")
        {
            macWVBox = true;
            if (addresses)
                free(addresses);
            return;
        }
        else if (type == "VMware")
        {
            macWVMware = true;
            if (addresses)
                free(addresses);
            return;
        }
    }

    if (addresses)
        free(addresses);
    return;

#else
    struct ifaddrs *ifap;
    if (getifaddrs(&ifap) != 0)
        return;

    for (struct ifaddrs *p = ifap; p; p = p->ifa_next)
    {
        if (!p->ifa_addr || p->ifa_addr->sa_family != AF_PACKET)
            continue;

        if (!(p->ifa_flags & IFF_UP))
            continue;

        std::string ifname = p->ifa_name;
        struct sockaddr_ll *s = (struct sockaddr_ll *)p->ifa_addr;

        if (s->sll_halen < 6)
            continue;

        // Filtrar solo interfaz fisica principal
        if (isVirtualInterface(ifname))
            continue;

        std::string mac = formatMac(s->sll_addr, s->sll_halen);
        std::string type = classMac(mac);

        //std::cout << "[MAC Detect] Interfaz principal activa: " << ifname << ", MAC: " << mac << "\n";
        
        if (type == "Fisica")
        {
            macLHost = true;
            freeifaddrs(ifap);
            return;
        }
        else if (type == "VirtualBox")
        {
            macLVBox = true;
            freeifaddrs(ifap);
            return;
        }
        else if (type == "VMware")
        {
            macLVMware = true;
            freeifaddrs(ifap);
            return;
        }
    }

#endif
}
