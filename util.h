#ifndef UTIL_H
#define UTIL_H

#include <cstdint>
#include <vector>

void write_uint32_le(std::vector<uint8_t>& buf, uint32_t val);
void write_int64_le(std::vector<uint8_t>& buf, int64_t val);
void write_uint64_le(std::vector<uint8_t>& buf, uint64_t val);
void write_varint(std::vector<uint8_t>& buf, uint64_t val);

#endif // UTIL_H
