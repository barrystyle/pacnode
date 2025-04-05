#ifndef MESSAGE_H
#define MESSAGE_H

#include <arpa/inet.h>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/select.h>
#include <unistd.h>
#include <vector>

#include <util.h>

#include <openssl/sha.h>

std::vector<uint8_t> make_version_payload(int64_t version);
std::vector<uint8_t> make_message(const uint32_t pchmagic, const std::string& command, const std::vector<uint8_t>& payload);

#endif // MESSAGE_H
