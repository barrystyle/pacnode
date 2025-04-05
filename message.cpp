#include <message.h>

std::vector<uint8_t> make_version_payload(int64_t version)
{
    std::vector<uint8_t> payload;
    uint64_t services = 0;
    int64_t timestamp = time(nullptr);

    //node flags
    write_uint32_le(payload, version);
    write_int64_le(payload, services);
    write_int64_le(payload, timestamp);

    //addr and nonce
    payload.insert(payload.end(), 26, 0x00);
    payload.insert(payload.end(), 26, 0x00);
    write_uint64_le(payload, (uint32_t)rand());

    //payload
    write_varint(payload, 0);
    write_uint32_le(payload, 0);
    payload.push_back(0);

    return payload;
}

std::vector<uint8_t> make_message(const uint32_t pchmagic, const std::string& command, const std::vector<uint8_t>& payload)
{
    std::vector<uint8_t> message;
    write_uint32_le(message, pchmagic);

    //command
    char cmd[12] = { 0 };
    std::memcpy(cmd, command.c_str(), command.size());
    message.insert(message.end(), cmd, cmd + 12);

    //len
    write_uint32_le(message, payload.size());

    //checksum
    std::vector<uint8_t> checksum;
    checksum.resize(32);
    SHA256(payload.data(), payload.size(), checksum.data());
    SHA256(checksum.data(), 32, checksum.data());
    message.insert(message.end(), checksum.begin(), checksum.begin() + 4);

    //payload
    message.insert(message.end(), payload.begin(), payload.end());

    return message;
}

