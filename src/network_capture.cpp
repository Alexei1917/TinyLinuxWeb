#include "network_capture.h"
#include <pcap.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>

#define MAX_EVENTS 10 // epoll 事件的最大数量

// 设置文件描述符为非阻塞模式
int setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

// 使用 epoll 实现网络数据包捕获
void NetworkCapture::startCapture() {
    char errbuf[PCAP_ERRBUF_SIZE]; // 错误缓冲区
    pcap_t *handle;                // pcap 句柄

    // 打开第一个网络接口进行抓包
    handle = pcap_open_live("eth0", BUFSIZ, 1, 1000, errbuf);
    if (!handle) {
        std::cerr << "Error opening device: " << errbuf << std::endl;
        return;
    }

    // 获取 pcap 的文件描述符
    int pcap_fd = pcap_get_selectable_fd(handle);
    if (pcap_fd == -1) {
        std::cerr << "Failed to get pcap file descriptor." << std::endl;
        pcap_close(handle);
        return;
    }

    // 设置文件描述符为非阻塞模式
    if (setNonBlocking(pcap_fd) == -1) {
        std::cerr << "Failed to set non-blocking mode." << std::endl;
        pcap_close(handle);
        return;
    }

    // 创建 epoll 实例
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "Failed to create epoll instance." << std::endl;
        pcap_close(handle);
        return;
    }

    // 添加 pcap 文件描述符到 epoll 中
    epoll_event event;
    event.events = EPOLLIN; // 监听可读事件
    event.data.fd = pcap_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, pcap_fd, &event) == -1) {
        std::cerr << "Failed to add pcap fd to epoll." << std::endl;
        close(epoll_fd);
        pcap_close(handle);
        return;
    }

    // 事件循环
    epoll_event events[MAX_EVENTS];
    while (true) {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1); // 阻塞等待事件
        if (n == -1) {
            std::cerr << "Epoll wait error: " << strerror(errno) << std::endl;
            break;
        }

        for (int i = 0; i < n; ++i) {
            if (events[i].data.fd == pcap_fd) {
                // 数据包捕获事件
                const u_char *packet;
                struct pcap_pkthdr header;

                // 抓取数据包
                packet = pcap_next(handle, &header);
                if (packet) {
                    std::cout << "Captured packet of length " << header.len << std::endl;
                }
            }
        }
    }

    // 清理资源
    close(epoll_fd);
    pcap_close(handle);
}
