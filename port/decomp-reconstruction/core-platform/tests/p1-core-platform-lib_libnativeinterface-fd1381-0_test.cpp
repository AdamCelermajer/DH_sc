// Tests for the libnativeinterface reconstruction (batch p1-core-platform-lib_libnativeinterface-fd1381-0).
//
// The SHA-1 core is checked against FIPS 180 digests. The derivation helpers are checked against formulas
// written out here (a digest's hexadecimal digit sum is the sum of its nibbles). CheckLicenseFile is checked
// against a license file written from the same derivation. The passphrase blobs are synthetic: the original
// 1024-byte texts are not in source, so the values from FUN_00010b48 are checked against the table pointers,
// not against the original text.

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

#include "core-platform/tb/nativeinterface_license.hpp"

namespace {

int failures = 0;

#define CHECK(cond)                                                     \
    do {                                                                \
        if (!(cond)) {                                                  \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++failures;                                                 \
        }                                                               \
    } while (0)

std::string to_hex(const uint8_t* bytes, size_t n) {
    static const char digits[] = "0123456789abcdef";
    std::string text;
    for (size_t i = 0; i < n; ++i) {
        text.push_back(digits[bytes[i] >> 4]);
        text.push_back(digits[bytes[i] & 15]);
    }
    return text;
}

// SHA-1 of text through the context functions, as big-endian bytes (the standard digest order).
void sha1_bytes(const std::string& text, uint8_t out[20]) {
    SHA1Context context;
    SHA1Reset(&context);
    SHA1Input(&context, reinterpret_cast<const uint8_t*>(text.data()), static_cast<uint32_t>(text.size()));
    SHA1Result(&context);
    for (int i = 0; i < 5; ++i) {
        out[4 * i + 0] = static_cast<uint8_t>(context.digest[i] >> 24);
        out[4 * i + 1] = static_cast<uint8_t>(context.digest[i] >> 16);
        out[4 * i + 2] = static_cast<uint8_t>(context.digest[i] >> 8);
        out[4 * i + 3] = static_cast<uint8_t>(context.digest[i]);
    }
}

uint32_t nibble_sum(const uint8_t* digest) {
    uint32_t sum = 0;
    for (int i = 0; i < 20; ++i) {
        sum += (digest[i] >> 4) + (digest[i] & 15u);
    }
    return sum;
}

// Synthetic passphrase texts: 1024 letters each, no terminator, read the same way the original reads them.
char g_test_blob_text[5][1024];
}  // namespace

extern const char* const g_license_passphrase_blob[5] = {g_test_blob_text[0], g_test_blob_text[1],
                                                         g_test_blob_text[2], g_test_blob_text[3],
                                                         g_test_blob_text[4]};

namespace {

void test_sha1_core() {
    uint8_t digest[20];
    sha1_bytes("abc", digest);
    CHECK(to_hex(digest, 20) == "a9993e364706816aba3e25717850c26c9cd0d89d");
    sha1_bytes("", digest);
    CHECK(to_hex(digest, 20) == "da39a3ee5e6b4b0d3255bfef95601890afd80709");
    sha1_bytes("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", digest);
    CHECK(to_hex(digest, 20) == "84983e441c3bd26ebaae4aa1f95129e5e54670f1");

    // One million 'a' in 1000 chunks of 1000 bytes: exercises many blocks through SHA1Input.
    SHA1Context context;
    SHA1Reset(&context);
    const std::string chunk(1000, 'a');
    for (int i = 0; i < 1000; ++i) {
        SHA1Input(&context, reinterpret_cast<const uint8_t*>(chunk.data()), 1000);
    }
    CHECK(SHA1Result(&context) == 1);
    uint8_t big[20];
    for (int i = 0; i < 5; ++i) {
        big[4 * i + 0] = static_cast<uint8_t>(context.digest[i] >> 24);
        big[4 * i + 1] = static_cast<uint8_t>(context.digest[i] >> 16);
        big[4 * i + 2] = static_cast<uint8_t>(context.digest[i] >> 8);
        big[4 * i + 3] = static_cast<uint8_t>(context.digest[i]);
    }
    CHECK(to_hex(big, 20) == "34aa973cd4c4daa4f61eeb2bdbad27316534016f");
}

void test_fun_00010924() {
    uint8_t out[20];
    FUN_00010924("abc", out, 3);
    CHECK(to_hex(out, 20) == "a9993e364706816aba3e25717850c26c9cd0d89d");
    FUN_00010924("", out, 0);
    CHECK(to_hex(out, 20) == "da39a3ee5e6b4b0d3255bfef95601890afd80709");
}

void test_hex_digit_sum() {
    CHECK(FUN_000109a0("") == 0);
    CHECK(FUN_000109a0("0") == 0);
    CHECK(FUN_000109a0("f") == 15);
    CHECK(FUN_000109a0("A9") == 19);
    CHECK(FUN_000109a0("09af") == 34);
    CHECK(FUN_000109a0("ABCDEF0123456789abcdef") == 195);
    uint8_t digest[20];
    sha1_bytes("abc", digest);
    const std::string hex = to_hex(digest, 20);
    CHECK(static_cast<uint32_t>(FUN_000109a0(hex.c_str())) == nibble_sum(digest));
}

void test_hex_encode() {
    uint8_t digest[20];
    digest[0] = 0x00;
    digest[1] = 0xff;
    digest[2] = 0x0a;
    for (int i = 3; i < 20; ++i) {
        digest[i] = static_cast<uint8_t>(0x5c + i);
    }
    char hex[41];
    FUN_00010af8(digest, hex);
    CHECK(hex[40] == '\0');
    CHECK(std::string(hex) == to_hex(digest, 20));
    CHECK(std::string(hex).substr(0, 6) == "00ff0a");
}

void test_derivation() {
    const char* cases[][2] = {
        {"alpha", "beta"},
        {"", "x"},
        {"a text longer than thirty-one characters, so the copy is cut", "second"},
    };
    for (const auto& pair : cases) {
        const std::string first = pair[0];
        const std::string second = pair[1];
        const char* out = nullptr;
        const size_t n = FUN_00010b48(pair[0], pair[1], &out);

        uint8_t d1[20];
        uint8_t d2[20];
        sha1_bytes(first.substr(0, 31), d1);
        sha1_bytes(second, d2);
        const uint32_t s2 = nibble_sum(d1);
        const uint32_t s3 = nibble_sum(d2);
        const uint32_t r = (s2 * s2) & 0x1ffu;
        CHECK(out == g_license_passphrase_blob[r % 5] + r);
        CHECK(n == static_cast<size_t>(((s3 * s3) & 0x1ffu) + 0x200u - r));
        CHECK(n >= 1 && n <= 1023);
    }
}

void test_check_license_file() {
    const char* path = "nativeinterface_license_test.bin";
    g_license_str2 = "alpha";
    g_license_str3 = "beta";

    // The license text is the derived passphrase followed by the two strings.
    const char* derived = nullptr;
    const size_t n = FUN_00010b48("alpha", "beta", &derived);
    const std::string text = std::string(derived, n) + "alpha" + "beta";
    uint8_t expected[20];
    sha1_bytes(text, expected);

    auto write_file = [&](const uint8_t* bytes, size_t count) {
        FILE* f = std::fopen(path, "wb");
        if (f != nullptr) {
            std::fwrite(bytes, 1, count, f);
            std::fclose(f);
        }
    };

    g_license_file_path = path;
    write_file(expected, 20);
    // The "r" text-mode read used by the original mangles CR, LF and Ctrl-Z bytes on Windows.
    bool text_mode_safe = true;
    for (uint8_t b : expected) {
        if (b == 0x0a || b == 0x0d || b == 0x1a) {
            text_mode_safe = false;
        }
    }
#if defined(_WIN32)
    if (text_mode_safe) {
        CHECK(CheckLicenseFile() == true);
    } else {
        std::printf("note: digest has a text-mode byte; positive read skipped on Windows\n");
    }
#else
    CHECK(CheckLicenseFile() == true);
#endif

    uint8_t wrong[20];
    std::memcpy(wrong, expected, 20);
    wrong[0] ^= 0x01;
    write_file(wrong, 20);
    CHECK(CheckLicenseFile() == false);

    write_file(expected, 19);
    CHECK(CheckLicenseFile() == false);

    std::remove(path);
    CHECK(CheckLicenseFile() == false);

    g_license_file_path = nullptr;
    g_license_str2 = nullptr;
    g_license_str3 = nullptr;
}

void test_check_license_file2() {
    CHECK(Java_com_samsung_zirconia_NativeInterface_checkLicenseFile2(nullptr, nullptr, nullptr, nullptr) == JNI_TRUE);
}

}  // namespace

int main() {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 1024; ++j) {
            g_test_blob_text[i][j] = static_cast<char>('A' + (i * 7 + j) % 26);
        }
    }
    test_sha1_core();
    test_fun_00010924();
    test_hex_digit_sum();
    test_hex_encode();
    test_derivation();
    test_check_license_file();
    test_check_license_file2();
    if (failures != 0) {
        std::printf("%d check(s) failed\n", failures);
        return 1;
    }
    std::printf("all checks passed\n");
    return 0;
}
