#include <util.h>

void write_uint32_le(std::vector<uint8_t>& buf, uint32_t val)
{
    buf.push_back(val & 0xff);
    buf.push_back((val >> 8) & 0xff);
    buf.push_back((val >> 16) & 0xff);
    buf.push_back((val >> 24) & 0xff);
}

void write_int64_le(std::vector<uint8_t>& buf, int64_t val)
{
    for (int i = 0; i < 8; ++i)
        buf.push_back((val >> (8 * i)) & 0xff);
}

void write_uint64_le(std::vector<uint8_t>& buf, uint64_t val)
{
    for (int i = 0; i < 8; ++i)
        buf.push_back((val >> (8 * i)) & 0xff);
}

void write_varint(std::vector<uint8_t>& buf, uint64_t val)
{
    if (val < 0xfd) {
        buf.push_back(val);
    } else if (val <= 0xffff) {
        buf.push_back(0xfd);
        buf.push_back(val & 0xff);
        buf.push_back((val >> 8) & 0xff);
    }
}
