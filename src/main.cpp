#include "network_capture.h"
#include "packet_injection.h"
#include "interface_monitor.h"
#include "log_manager.h"
#include <iostream>
#include <thread>

void startNetworkCapture() {
    NetworkCapture capture;
    capture.startCapture();
}

void startPacketInjection() {
    PacketInjection injector;
    injector.injectPacket();
}

void startInterfaceMonitoring() {
    InterfaceMonitor monitor;
    monitor.monitorInterfaces();
}

int main() {
    std::thread captureThread(startNetworkCapture);
    std::thread injectionThread(startPacketInjection);
    std::thread monitorThread(startInterfaceMonitoring);

    captureThread.join();
    injectionThread.join();
    monitorThread.join();

    return 0;
}