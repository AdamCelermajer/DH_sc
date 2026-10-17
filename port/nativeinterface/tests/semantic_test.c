#include "nativeinterface.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void test_digest(const char *message, const char *expected) {
    uint8_t digest[20];
    char hex[41];
    dh2_sha1((const uint8_t *)message, strlen(message), digest);
    for (unsigned i = 0; i < 20; ++i) snprintf(hex + 2 * i, 3, "%02x", digest[i]);
    assert(strcmp(hex, expected) == 0);
}

int main(void) {
    test_digest("", "da39a3ee5e6b4b0d3255bfef95601890afd80709");
    test_digest("abc", "a9993e364706816aba3e25717850c26c9cd0d89d");
    test_digest("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
                "84983e441c3bd26ebaae4aa1f95129e5e54670f1");
    SHA1Context context;
    SHA1Reset(&context);
    for (int i = 0; i < 1000000; ++i) SHA1Input(&context, (const uint8_t *)"a", 1);
    assert(SHA1Result(&context) == 1);
    assert(context.digest[0] == 0x34aa973c && context.digest[4] == 0x6534016f);
    assert(SHA1Result(&context) == 1);
    SHA1Input(&context, (const uint8_t *)"x", 1);
    assert(SHA1Result(&context) == 0);
    SHA1Reset(&context);
    context.length_low = UINT32_MAX - 7;
    context.length_high = UINT32_MAX;
    SHA1Input(&context, (const uint8_t *)"x", 1);
    assert(context.corrupted == 1);
    assert(SHA1Result(&context) == 0);

    const char *fragment, *truncated_fragment;
    size_t fragment_length = dh2_passphrase("synthetic-test-identity", "synthetic-test-product", &fragment);
    assert(fragment_length > 0 && fragment_length <= 1023);
    size_t a = dh2_passphrase("1234567890123456789012345678901EXTRA", "product", &fragment);
    size_t b = dh2_passphrase("1234567890123456789012345678901", "product", &truncated_fragment);
    assert(a == b && fragment == truncated_fragment);

    char path[] = "dh2-nativeinterface-test-XXXXXX";
    int fd = mkstemp(path);
    assert(fd >= 0);
    close(fd);
    assert(dh2_check_license_file(path, "synthetic-test-identity", "synthetic-test-product") == 0);
    assert(dh2_check_license_file("/this/path/does/not/exist", "a", "b") == 0);
    assert(CheckLicenseFile() == 0);
    const char *first = "synthetic-test-identity", *second = "synthetic-test-product";
    fragment_length = dh2_passphrase(first, second, &fragment);
    size_t joined_length = fragment_length + strlen(first) + strlen(second);
    char *joined = (char *)malloc(joined_length + 1);
    assert(joined);
    memcpy(joined, fragment, fragment_length);
    strcpy(joined + fragment_length, first);
    strcat(joined, second);
    uint8_t digest[20], stored[40], metadata_digest[20];
    dh2_sha1((const uint8_t *)joined, joined_length, digest);
    free(joined);
    assert(dh2_store_license_key(path, digest, 19, "metadata") == 0);
    assert(dh2_store_license_key(path, digest, 20, "metadata") == 1);
    FILE *file = fopen(path, "rb");
    assert(file && fread(stored, 1, 40, file) == 40);
    assert(fgetc(file) == EOF);
    fclose(file);
    dh2_sha1((const uint8_t *)"metadata", 2, metadata_digest);
    assert(memcmp(stored, digest, 20) == 0);
    assert(memcmp(stored + 20, metadata_digest, 20) == 0);
    assert(dh2_check_license_file(path, first, second) == 1);
    assert(dh2_check_license_with_globals(path, first, second) == 1);
    assert(CheckLicenseFile() == 0);
    assert(dh2_check_license_file(path, "wrong-identity", second) == 0);
    file = fopen(path, "rb+");
    assert(file);
    assert(fseek(file, 20, SEEK_SET) == 0);
    assert(fwrite("arbitrary-trailing-metadata", 1, 26, file) == 26);
    fclose(file);
    assert(dh2_check_license_file(path, first, second) == 1);
    file = fopen(path, "rb+");
    assert(file && fputc(stored[0] ^ 1, file) != EOF);
    fclose(file);
    assert(dh2_check_license_file(path, first, second) == 0);
    unlink(path);
    assert(dh2_legacy_metadata_length("") == 0);
    assert(dh2_legacy_metadata_length("abc") == 1);
    assert(dh2_legacy_metadata_length("abcd") == 1);
    assert(dh2_legacy_metadata_length("abcde") == 2);
    puts("PASS: SHA1 vectors/state/overflow; passphrase truncation; license match/mismatch/short-file; stored metadata compatibility");
    return 0;
}
