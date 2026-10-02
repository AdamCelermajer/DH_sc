/* New source reconstruction from the supplied 32-bit ELF; not studio source. */
#ifndef DH2_NATIVEINTERFACE_H
#define DH2_NATIVEINTERFACE_H
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Fixed-width layout matches the original 104-byte SHA1Context on ARM32. */
typedef struct {
    uint32_t digest[5];
    uint32_t length_low;
    uint32_t length_high;
    uint8_t message_block[64];
    int32_t message_block_index;
    int32_t computed;
    int32_t corrupted;
} SHA1Context;

void SHA1Reset(SHA1Context *context);
void SHA1Input(SHA1Context *context, const uint8_t *bytes, uint32_t length);
int SHA1Result(SHA1Context *context);
void SHA1ProcessMessageBlock(SHA1Context *context);
void SHA1PadMessage(SHA1Context *context);
int CheckLicenseFile(void);

/* Helpers added for this reconstruction; hidden in the Android shared object. */
void dh2_sha1(const uint8_t *bytes, size_t length, uint8_t result[20]);
size_t dh2_passphrase(const char *first, const char *second, const char **fragment);
int dh2_check_license_file(const char *path, const char *first, const char *second);
int dh2_store_license_key(const char *path, const uint8_t *key, size_t key_length,
                          const char *metadata);
size_t dh2_legacy_metadata_length(const char *bytes);
int dh2_check_license_with_globals(const char *path, const char *first, const char *second);

#ifdef __cplusplus
}
#endif
#endif
