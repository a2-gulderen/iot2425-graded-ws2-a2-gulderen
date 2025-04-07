#ifndef SIM_SOCKET_H
#define SIM_SOCKET_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

namespace sim {

class socket {
public:
    inline socket(int domain, int type, int protocol) {
        sockfd = ::socket(domain, type, protocol);
        if (sockfd < 0)
            throw std::runtime_error("socket creation failed");
    }
    
    inline ~socket() {
        if (sockfd >= 0)
            ::close(sockfd);
    }
    
    inline void bind(const struct sockaddr_in &addr) {
        if (::bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
            throw std::runtime_error("bind failed");
    }
    
    inline ssize_t sendto(const void* data, size_t size, int flags, const struct sockaddr_in &destAddr) {
        return ::sendto(sockfd, data, size, flags, (struct sockaddr*)&destAddr, sizeof(destAddr));
    }
    
    inline ssize_t recvfrom(void* buffer, size_t size, int flags, struct sockaddr_in &srcAddr) {
        socklen_t addrlen = sizeof(srcAddr);
        return ::recvfrom(sockfd, buffer, size, flags, (struct sockaddr*)&srcAddr, &addrlen);
    }
    
private:
    int sockfd;
};

} // namespace sim

#endif // SIM_SOCKET_H
