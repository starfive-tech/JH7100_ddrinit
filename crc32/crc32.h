#ifndef _CRC32_H
#define _CRC32_H

void crc32(const void *data, uint32_t n_bytes, uint32_t* crc);
uint32_t crc32_buf(const void *buf, uint32_t n_bytes);

#endif

