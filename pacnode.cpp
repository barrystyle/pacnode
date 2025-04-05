#include <message.h>

#include <functional>
#include <thread>

//pacnode
//barrystyle 05042025

void worker_thread(int thr_id, std::string peer_address, int peer_port)
{
    const int timeout_secs = 1;
    char* peer_ip = (char*)peer_address.c_str();

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return;
    }

    //nonblocking
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    //create socket
    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(peer_port);
    inet_pton(AF_INET, peer_ip, &addr.sin_addr);

    int result = connect(sock, (sockaddr*)&addr, sizeof(addr));
    if (result < 0 && errno != EINPROGRESS) {
        printf("offline\n");
        close(sock);
        return;
    }
    printf("%s:%d ", peer_ip, peer_port);

    //timeout
    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(sock, &writefds);

    struct timeval timeout;
    timeout.tv_sec = timeout_secs;
    timeout.tv_usec = 0;

    result = select(sock + 1, nullptr, &writefds, nullptr, &timeout);
    if (result <= 0) {
        printf("offline\n");
        close(sock);
        return;
    }

    //blocking
    fcntl(sock, F_SETFL, flags);

    //connect
    struct timeval recv_timeout;
    recv_timeout.tv_sec = timeout_secs;
    recv_timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout, sizeof(recv_timeout));

    std::vector<uint8_t> payload = make_version_payload(70221);
    std::vector<uint8_t> message = make_message(0x2c61e5c8, "version", payload);

    ssize_t sent = send(sock, message.data(), message.size(), 0);
    if (sent < 0) {
        printf("offline\n");
        close(sock);
        return;
    }

    uint8_t buffer[1024];
    ssize_t len = recv(sock, buffer, sizeof(buffer), 0);
    if (len > 0) {
        printf("online\n");
    } else {
        printf("offline\n");
    }

    close(sock);
}

int max_thr = 4;

int main()
{
    std::vector<std::thread> threads;
    
    int count = 0;
    std::vector<std::string> ips = { "109.11.183.98" };
    int peer_port = 7112;

    while (true) {

        int i=0;
        while (i < max_thr && count < ips.size()) {
            threads.push_back(std::thread(std::bind(&worker_thread, std::ref(i), std::ref(ips[count]), std::ref(peer_port))));
            ++i;     //thread count
            ++count; //mn index
        }

        i=0;
        while (i < max_thr) {
            if (threads[i].joinable()) {
                threads[i].join();
            }
            ++i;
        }

        usleep(1000);
    }

    return 1;
}
