#include "packet_injection.h"
#include <libnet.h>
#include <iostream>

void PacketInjection::injectPacket() {
    char errbuf[LIBNET_ERRBUF_SIZE];
    libnet_t *context = libnet_init(LIBNET_LINK, nullptr, errbuf);

    if (!context) {
        std::cerr << "Libnet initialization failed: " << errbuf << std::endl;
        return;
    }

    std::cout << "Packet injection simulated." << std::endl;
    libnet_destroy(context);
}
