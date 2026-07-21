#ifndef DITOS_H
#define DITOS_H

typedef unsigned int   uint32_t;
typedef unsigned char  uint8_t;
typedef unsigned long  size_t;

#define DITOS_MAGIC 0x4449544F  /* "DITO" */

typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint32_t uncomp_size;
    uint32_t comp_size;
    uint32_t entry_point;
} ditos_header_t;

size_t ditos_pack_kernel(const uint8_t *src, size_t src_len, uint8_t *out_buf, uint32_t entry_point);

#endif /* DITOS_H */
