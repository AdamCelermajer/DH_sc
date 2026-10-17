/* New source reconstruction. Behavior traced to the supplied libnativeinterface.so.
 * See README.md for original undefined behavior and deliberate safety changes.
 */
#include "nativeinterface.h"
#include "passphrase_tables.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

_Static_assert(sizeof(SHA1Context) == 104, "original SHA1Context layout");
_Static_assert(offsetof(SHA1Context, message_block_index) == 0x5c, "original index offset");

static uint32_t rotate_left(uint32_t word, unsigned bits) {
    return (word << bits) | (word >> (32 - bits));
}

void SHA1Reset(SHA1Context *context) {
    context->digest[0] = UINT32_C(0x67452301);
    context->digest[1] = UINT32_C(0xefcdab89);
    context->digest[2] = UINT32_C(0x98badcfe);
    context->digest[3] = UINT32_C(0x10325476);
    context->digest[4] = UINT32_C(0xc3d2e1f0);
    context->length_low = context->length_high = 0;
    context->message_block_index = 0;
    context->computed = context->corrupted = 0;
}

void SHA1ProcessMessageBlock(SHA1Context *context) {
    uint32_t words[80];
    for (unsigned i = 0; i < 16; ++i) {
        const uint8_t *b = context->message_block + 4 * i;
        words[i] = ((uint32_t)b[0] << 24) | ((uint32_t)b[1] << 16) |
                   ((uint32_t)b[2] << 8) | b[3];
    }
    for (unsigned i = 16; i < 80; ++i)
        words[i] = rotate_left(words[i - 3] ^ words[i - 8] ^ words[i - 14] ^ words[i - 16], 1);
    uint32_t a = context->digest[0], b = context->digest[1], c = context->digest[2];
    uint32_t d = context->digest[3], e = context->digest[4];
    for (unsigned i = 0; i < 80; ++i) {
        uint32_t f, k;
        if (i < 20) { f = (b & c) | (~b & d); k = UINT32_C(0x5a827999); }
        else if (i < 40) { f = b ^ c ^ d; k = UINT32_C(0x6ed9eba1); }
        else if (i < 60) { f = (b & c) | (b & d) | (c & d); k = UINT32_C(0x8f1bbcdc); }
        else { f = b ^ c ^ d; k = UINT32_C(0xca62c1d6); }
        uint32_t next = rotate_left(a, 5) + f + e + words[i] + k;
        e = d; d = c; c = rotate_left(b, 30); b = a; a = next;
    }
    context->digest[0] += a; context->digest[1] += b; context->digest[2] += c;
    context->digest[3] += d; context->digest[4] += e;
    context->message_block_index = 0;
}

void SHA1Input(SHA1Context *context, const uint8_t *bytes, uint32_t length) {
    if (!length) return;
    if (context->computed || context->corrupted) { context->corrupted = 1; return; }
    while (length-- && !context->corrupted) {
        context->message_block[context->message_block_index++] = *bytes++;
        context->length_low += 8;
        if (!context->length_low && !++context->length_high) context->corrupted = 1;
        if (context->message_block_index == 64) SHA1ProcessMessageBlock(context);
    }
}

void SHA1PadMessage(SHA1Context *context) {
    context->message_block[context->message_block_index++] = 0x80;
    if (context->message_block_index > 56) {
        while (context->message_block_index < 64)
            context->message_block[context->message_block_index++] = 0;
        SHA1ProcessMessageBlock(context);
    }
    while (context->message_block_index < 56)
        context->message_block[context->message_block_index++] = 0;
    for (unsigned i = 0; i < 4; ++i) {
        context->message_block[56 + i] = (uint8_t)(context->length_high >> (24 - 8 * i));
        context->message_block[60 + i] = (uint8_t)(context->length_low >> (24 - 8 * i));
    }
    SHA1ProcessMessageBlock(context);
}

int SHA1Result(SHA1Context *context) {
    if (context->corrupted) return 0;
    if (!context->computed) { SHA1PadMessage(context); context->computed = 1; }
    return 1;
}

void dh2_sha1(const uint8_t *bytes, size_t length, uint8_t result[20]) {
    SHA1Context context;
    SHA1Reset(&context);
    while (length) {
        uint32_t chunk = length > UINT32_MAX ? UINT32_MAX : (uint32_t)length;
        SHA1Input(&context, bytes, chunk);
        bytes += chunk; length -= chunk;
    }
    SHA1Result(&context);
    for (unsigned i = 0; i < 20; ++i)
        result[i] = (uint8_t)(context.digest[i / 4] >> (24 - 8 * (i % 4)));
}

/* Original helper formats SHA1 as lowercase hex and adds its nibble values.
 * Sum the same values directly, avoiding a temporary hex buffer. */
static unsigned digest_nibble_sum(const uint8_t digest[20]) {
    unsigned sum = 0;
    for (unsigned i = 0; i < 20; ++i) sum += (digest[i] >> 4) + (digest[i] & 15);
    return sum;
}

size_t dh2_passphrase(const char *first, const char *second, const char **fragment) {
    char truncated[32] = {0};
    uint8_t a[20], b[20];
    strncpy(truncated, first, 31);
    dh2_sha1((const uint8_t *)truncated, strlen(truncated), a);
    dh2_sha1((const uint8_t *)second, strlen(second), b);
    unsigned x = digest_nibble_sum(a), y = digest_nibble_sum(b);
    unsigned start = (x * x) & 511u, stop = ((y * y) & 511u) + 512u;
    *fragment = dh2_passphrase_tables[start % 5] + start;
    return stop - start;
}

int dh2_check_license_file(const char *path, const char *first, const char *second) {
    if (!path || !first || !second) return 0;
    uint8_t file_digest[20], expected[20];
    FILE *file = fopen(path, "rb");
    if (!file) return 0;
    size_t count = fread(file_digest, 1, 20, file);
    fclose(file);
    if (count != 20) return 0;
    const char *fragment;
    size_t fragment_length = dh2_passphrase(first, second, &fragment);
    size_t first_length = strlen(first), second_length = strlen(second);
    if (first_length > SIZE_MAX - second_length - 1 ||
        fragment_length > SIZE_MAX - first_length - second_length - 1) return 0;
    size_t total = fragment_length + first_length + second_length;
    char *joined = (char *)malloc(total + 1);
    if (!joined) return 0;
    /* strncpy preserves the original zero-padding semantics if a table contains
     * a zero byte; all five recovered 1024-byte tables contain plain text. */
    strncpy(joined, fragment, fragment_length);
    joined[fragment_length] = 0;
    strcat(joined, first);
    strcat(joined, second);
    dh2_sha1((const uint8_t *)joined, total, expected);
    int result = memcmp(file_digest, expected, 20) == 0;
    memset(joined, 0, total + 1);
    free(joined);
    return result;
}

/* The original JNI function invokes wcslen on UTF-8 bytes. Define the compatible
 * result for a byte buffer padded to a terminating 32-bit zero unit, without
 * reading beyond its bounds: ceil(strlen / 4). See README.md. */
size_t dh2_legacy_metadata_length(const char *bytes) {
    size_t length = strlen(bytes);
    return length / 4 + (length % 4 != 0);
}

int dh2_store_license_key(const char *path, const uint8_t *key, size_t key_length,
                          const char *metadata) {
    if (!path || !key || key_length < 20 || !metadata) return 0;
    uint8_t metadata_digest[20];
    dh2_sha1((const uint8_t *)metadata, dh2_legacy_metadata_length(metadata), metadata_digest);
    FILE *file = fopen(path, "wb");
    if (!file) return 0;
    fwrite(key, 1, 20, file);
    fwrite(metadata_digest, 1, 20, file);
    fclose(file);
    /* Matches original: success means fopen succeeded, independent of write results. */
    return 1;
}

/* The original exported CheckLicenseFile uses three globals populated only while
 * its JNI wrapper is running. Keep its signature, using per-thread storage in
 * this port to prevent concurrent calls from exchanging identities or paths. */
static _Thread_local const char *license_path, *first_identity, *second_identity;

int CheckLicenseFile(void) {
    return dh2_check_license_file(license_path, first_identity, second_identity);
}

int dh2_check_license_with_globals(const char *path, const char *first, const char *second) {
    license_path = path; first_identity = first; second_identity = second;
    int result = CheckLicenseFile();
    license_path = first_identity = second_identity = NULL;
    return result;
}
