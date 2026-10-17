#ifndef DH2_PERSISTENCE_BINARY_H
#define DH2_PERSISTENCE_BINARY_H
#include <stddef.h>
#include <stdint.h>
#include <string.h>
/* Authored portable encounter state. No original savegame ABI or pointers. */
#define DH2_ACTOR_SAVE_BYTES 932U
#define DH2_QUEST_SAVE_BYTES 40U
#define DH2_RANDOM_SAVE_BYTES 20U
static inline uint32_t dh2_save_read32(const unsigned char *p) {
    return (uint32_t)p[0]|(uint32_t)p[1]<<8|(uint32_t)p[2]<<16|(uint32_t)p[3]<<24;
}
static inline int32_t dh2_save_read_i32(const unsigned char *p) {
    uint32_t bits=dh2_save_read32(p);int32_t value;memcpy(&value,&bits,4);return value;
}
static inline void dh2_save_write32(unsigned char *p,uint32_t value) {
    p[0]=(unsigned char)value;p[1]=(unsigned char)(value>>8);
    p[2]=(unsigned char)(value>>16);p[3]=(unsigned char)(value>>24);
}
static inline uint32_t dh2_save_crc32(const void *bytes,size_t size) {
    const unsigned char *p=(const unsigned char *)bytes;uint32_t crc=0xffffffffU;
    for(size_t i=0;i<size;++i) {
        crc^=p[i];for(unsigned bit=0;bit<8;++bit)crc=(crc>>1)^((0U-(crc&1U))&0xedb88320U);
    }
    return ~crc;
}
#endif
