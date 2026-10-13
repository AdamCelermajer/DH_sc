// libnativeinterface.so reconstruction (batch p1-core-platform-lib_libnativeinterface-fd1381-0, platform pass).
//
// Sources: the ARM listing recovered/native/assembly/libnativeinterface.so (named functions and the raw
// bytes of the unnamed ones, decoded with Capstone) and the Ghidra export in
// recovered/native/decompiled/libnativeinterface.so (addresses there carry a 0x10000 base). Every address
// comment is the ELF virtual address. The Hex-Rays export and IDA database cover libDungeonHunter2.so only.
//
// Stack-protector checks (__stack_chk_guard, __stack_chk_fail) are compiler output and are not reproduced.
// The Ghidra function signatures for CheckLicenseFile, FUN_00010b48 and storeLicenseKey are void, but the
// listing returns a value in r0 for each; the bodies here return that value.
//
// Open inputs (rows are partial where they depend on them):
//   - Byte order of entry(): the first byte of .rodata (ELF 0x1260) selects it. The value is not in the
//     recovered listings. kRodataWordOrderFlag below is 0, which is the big-endian path; Track A's
//     reference writes big-endian digests. Confirm the byte from the binary.
//   - Passphrase texts: g_license_passphrase_blob is extern. The five texts are 1024-byte .rodata strings
//     at 0x1274 + 1028 * i (the first is 1024 bytes in strings.csv). The integrator defines the table.
//   - wcslen (storeLicenseKey) is applied to the UTF-8 string from GetStringUTFChars, as the binary does. It
//     counts 32-bit units on Android (bionic wchar_t) and 16-bit units on Windows.
//   - fopen modes "r" and "w" are as in the binary; on Windows these are text mode.

#include "core-platform/tb/nativeinterface_license.hpp"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <wchar.h>

const char* g_license_file_path = nullptr;  // 0x3888 (.bss, Ghidra label __data_start)
const char* g_license_str2 = nullptr;       // 0x388c (.bss)
const char* g_license_str3 = nullptr;       // 0x3890 (.bss)

namespace {

// .rodata byte at ELF 0x1260 (value not in the recovered listings; see the file header). Zero selects the
// big-endian store in entry().
const uint8_t kRodataWordOrderFlag = 0;

inline uint32_t rotl32(uint32_t x, int n) {
    return (x << n) | (x >> (32 - n));
}

}  // namespace

// ---------------------------------------------------------------------------------------------------------
// SHA-1 (ELF 0xedc, 0xf14, 0x1128, 0x118c, 0x123c). Structure follows the listing; the 80 rounds use the
// standard f and K values of the four groups, and the test checks the result against FIPS 180 digests.
// ---------------------------------------------------------------------------------------------------------

// 0xedc SHA1Reset
void SHA1Reset(SHA1Context* context) {
    context->length_low = 0;
    context->digest[0] = 0x67452301u;
    context->length_high = 0;
    context->message_block_index = 0;
    context->digest[1] = 0xefcdab89u;
    context->computed = 0;
    context->corrupted = 0;
    context->digest[2] = 0x98badcfeu;
    context->digest[3] = 0x10325476u;
    context->digest[4] = 0xc3d2e1f0u;
}

// 0xf14 SHA1ProcessMessageBlock
void SHA1ProcessMessageBlock(SHA1Context* context) {
    uint32_t w[80];
    for (int t = 0; t < 16; ++t) {  // +0x1c: big-endian words of the message block
        const uint8_t* p = context->message_block + 4 * t;
        w[t] = (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) | (uint32_t(p[2]) << 8) | uint32_t(p[3]);
    }
    for (int t = 16; t < 80; ++t) {
        w[t] = rotl32(w[t - 3] ^ w[t - 8] ^ w[t - 14] ^ w[t - 16], 1);
    }

    uint32_t a = context->digest[0];
    uint32_t b = context->digest[1];
    uint32_t c = context->digest[2];
    uint32_t d = context->digest[3];
    uint32_t e = context->digest[4];
    for (int t = 0; t < 80; ++t) {
        uint32_t f;
        uint32_t k;
        if (t < 20) {
            f = (b & c) | (~b & d);
            k = 0x5a827999u;
        } else if (t < 40) {
            f = b ^ c ^ d;
            k = 0x6ed9eba1u;
        } else if (t < 60) {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8f1bbcdcu;
        } else {
            f = b ^ c ^ d;
            k = 0xca62c1d6u;
        }
        const uint32_t temp = rotl32(a, 5) + f + e + k + w[t];
        e = d;
        d = c;
        c = rotl32(b, 30);
        b = a;
        a = temp;
    }
    context->digest[0] += a;
    context->digest[1] += b;
    context->digest[2] += c;
    context->digest[3] += d;
    context->digest[4] += e;
    context->message_block_index = 0;
}

// 0x1128 SHA1Input
void SHA1Input(SHA1Context* context, const uint8_t* bytes, uint32_t length) {
    if (length == 0) {
        return;
    }
    if (context->computed != 0 || context->corrupted != 0) {
        context->corrupted = 1;
        return;
    }
    do {
        context->message_block[context->message_block_index++] = *bytes;
        context->length_low += 8;
        if (context->length_low == 0) {
            context->length_high += 1;
            if (context->length_high == 0) {
                context->corrupted = 1;
            }
        }
        if (context->message_block_index == 64) {
            SHA1ProcessMessageBlock(context);
        }
        if (--length == 0 || context->corrupted != 0) {
            break;
        }
        ++bytes;
    } while (true);
}

// 0x118c SHA1PadMessage
void SHA1PadMessage(SHA1Context* context) {
    if (context->message_block_index < 56) {
        context->message_block[context->message_block_index++] = 0x80;
        while (context->message_block_index < 56) {
            context->message_block[context->message_block_index++] = 0;
        }
    } else {
        context->message_block[context->message_block_index++] = 0x80;
        while (context->message_block_index < 64) {
            context->message_block[context->message_block_index++] = 0;
        }
        SHA1ProcessMessageBlock(context);
        while (context->message_block_index < 56) {
            context->message_block[context->message_block_index++] = 0;
        }
    }

    // +0x54..+0x5b: message length in bits, big-endian, high word first.
    const uint32_t high = context->length_high;
    context->message_block[56] = static_cast<uint8_t>(high >> 24);
    context->message_block[57] = static_cast<uint8_t>(high >> 16);
    context->message_block[58] = static_cast<uint8_t>(high >> 8);
    context->message_block[59] = static_cast<uint8_t>(high);
    const uint32_t low = context->length_low;
    context->message_block[60] = static_cast<uint8_t>(low >> 24);
    context->message_block[61] = static_cast<uint8_t>(low >> 16);
    context->message_block[62] = static_cast<uint8_t>(low >> 8);
    context->message_block[63] = static_cast<uint8_t>(low);
    SHA1ProcessMessageBlock(context);
}

// 0x123c SHA1Result
int SHA1Result(SHA1Context* context) {
    int result = 0;
    if (context->corrupted == 0) {
        result = 1;
        if (context->computed == 0) {
            SHA1PadMessage(context);
            context->computed = 1;
            result = 1;
        }
    }
    return result;
}

// ---------------------------------------------------------------------------------------------------------
// Derivation helpers (ELF 0x8e4, 0x924, 0x9a0, 0xaf8, 0xb48).
// ---------------------------------------------------------------------------------------------------------

// 0x8e4 entry (the ELF entry symbol as Ghidra labels it). Stores a 32-bit word as four bytes. Zero in the flag
// byte stores the most significant byte first; any other value stores the least significant byte first.
static void entry(uint8_t* out, uint32_t value) {
    if (kRodataWordOrderFlag != 0) {
        out[0] = static_cast<uint8_t>(value);
        out[1] = static_cast<uint8_t>(value >> 8);
        out[2] = static_cast<uint8_t>(value >> 16);
        out[3] = static_cast<uint8_t>(value >> 24);
    } else {
        out[0] = static_cast<uint8_t>(value >> 24);
        out[1] = static_cast<uint8_t>(value >> 16);
        out[2] = static_cast<uint8_t>(value >> 8);
        out[3] = static_cast<uint8_t>(value);
    }
}

// 0x924 FUN_00010924: SHA-1 of length bytes at data. Each of the five context words goes to out at +0, +4,
// +8, +0xc and +0x10 through entry().
void FUN_00010924(const void* data, uint8_t* out, uint32_t length) {
    SHA1Context context;
    SHA1Reset(&context);
    SHA1Input(&context, static_cast<const uint8_t*>(data), length);
    SHA1Result(&context);
    entry(out + 0x00, context.digest[0]);
    entry(out + 0x04, context.digest[1]);
    entry(out + 0x08, context.digest[2]);
    entry(out + 0x0c, context.digest[3]);
    entry(out + 0x10, context.digest[4]);
}

// 0x9a0 FUN_000109a0: scans text from the last character to the first, after tolower. Characters up to '/'
// add 0. Digits add 0-9. Any other character adds c - 0x57, so a-f add 10-15 and the rest add larger values.
int FUN_000109a0(const char* text) {
    int sum = 0;
    for (int i = static_cast<int>(strlen(text)) - 1; i >= 0; --i) {
        const uint32_t c = static_cast<uint32_t>(std::tolower(static_cast<unsigned char>(text[i])));
        uint32_t v = 0;
        if (c > 0x2f) {
            v = c - 0x30;
            if (v > 9) {
                v = c - 0x57;
            }
        }
        sum = static_cast<int>(static_cast<uint32_t>(sum) + v);
    }
    return sum;
}

// 0xaf8 FUN_00010af8: each digest byte as two lowercase hex digits, then a NUL at out[40].
void FUN_00010af8(const uint8_t* digest, char* out) {
    char pair[3] = {0, 0, 0};
    for (int i = 0; i < 20; ++i) {
        std::snprintf(pair, sizeof pair, "%02x", static_cast<unsigned>(digest[i]));
        out[2 * i] = pair[0];
        out[2 * i + 1] = pair[1];
    }
    out[40] = '\0';
}

// 0xb48 FUN_00010b48. Both inputs are hashed as the listing does:
//   a = copy of first, up to 31 characters (32-byte buffer); s2 = hex digit sum of hex(SHA-1(a)).
//   b = second, hashed;               s3 = hex digit sum of hex(b).
//   r = (s2 * s2) & 0x1ff; *out = passphrase[r % 5] + r; returns ((s3 * s3) & 0x1ff) + 0x200 - r.
size_t FUN_00010b48(const char* first, const char* second, const char** out) {
    char first_copy[32] = {};
    strncpy(first_copy, first, 0x1f);
    uint8_t digest1[20] = {};
    FUN_00010924(first_copy, digest1, static_cast<uint32_t>(strlen(first_copy)));
    char hex1[41] = {};
    FUN_00010af8(digest1, hex1);
    const int s2 = FUN_000109a0(hex1);

    uint8_t digest2[20] = {};
    FUN_00010924(second, digest2, static_cast<uint32_t>(strlen(second)));
    char hex2[41] = {};
    FUN_00010af8(digest2, hex2);
    const int s3 = FUN_000109a0(hex2);

    const uint32_t r = (static_cast<uint32_t>(s2) * static_cast<uint32_t>(s2)) & 0x1ffu;
    *out = g_license_passphrase_blob[r % 5] + r;
    return static_cast<size_t>(((static_cast<uint32_t>(s3) * static_cast<uint32_t>(s3)) & 0x1ffu) + 0x200u - r);
}

// ---------------------------------------------------------------------------------------------------------
// License file and JNI natives (ELF 0x920, 0x9d0, 0xc60, 0xd20, 0xe2c).
// ---------------------------------------------------------------------------------------------------------

// 0xd20 CheckLicenseFile. Reads 20 bytes from the license file and compares them with the digest of
// (derived passphrase, g_license_str2, g_license_str3).
bool CheckLicenseFile() {
    uint8_t stored[20] = {};
    bool matched = false;
    FILE* file = fopen(g_license_file_path, "r");
    if (file == nullptr) {
        return false;
    }
    const size_t got = fread(stored, 1, 20, file);
    fclose(file);
    if (got != 20) {
        return false;
    }

    const char* derived = nullptr;
    const size_t n = FUN_00010b48(g_license_str2, g_license_str3, &derived);
    const size_t len2 = strlen(g_license_str2);
    const size_t len3 = strlen(g_license_str3);
    const uint32_t total = static_cast<uint32_t>(len2 + n + len3);
    const size_t alloc_size = static_cast<size_t>(total) + 1;
    char* dest = static_cast<char*>(malloc(alloc_size));
    strncpy(dest, derived, n);
    dest[n] = '\0';
    strcat(dest, g_license_str2);
    strcat(dest, g_license_str3);

    uint8_t digest[20] = {};
    FUN_00010924(dest, digest, total);
    matched = memcmp(stored, digest, 20) == 0;
    memset(dest, 0, alloc_size);
    free(dest);
    return matched;
}

// 0x920 Java_com_samsung_zirconia_NativeInterface_checkLicenseFile2. The listing returns 1 and reads no
// arguments.
extern "C" JNIEXPORT jboolean JNICALL Java_com_samsung_zirconia_NativeInterface_checkLicenseFile2(
    JNIEnv* env, jclass cls, jstring str, jstring str2) {
    (void)env;
    (void)cls;
    (void)str;
    (void)str2;
    return JNI_TRUE;
}

// 0x9d0 Java_com_samsung_zirconia_NativeInterface_storeLicenseKey. Calls, in order: GetStringUTFChars(path),
// GetArrayLength(key) with the result unused, GetByteArrayElements(key), GetStringUTFChars(text), wcslen, then
// FUN_00010924 over the text. Writes the 20 key bytes and the 20-byte digest to the file named by path.
// Returns JNI_TRUE only when the file opened.
extern "C" JNIEXPORT jboolean JNICALL Java_com_samsung_zirconia_NativeInterface_storeLicenseKey(
    JNIEnv* env, jclass cls, jstring path, jbyteArray key, jstring text) {
    (void)cls;
    const char* filename = env->GetStringUTFChars(path, nullptr);
    (void)env->GetArrayLength(key);
    jbyte* elements = env->GetByteArrayElements(key, nullptr);
    const char* wide = env->GetStringUTFChars(text, nullptr);

    uint8_t digest[20] = {};
    const size_t wide_length = wcslen(reinterpret_cast<const wchar_t*>(wide));
    FUN_00010924(wide, digest, static_cast<uint32_t>(wide_length));

    jboolean status = JNI_FALSE;
    FILE* file = fopen(filename, "w");
    if (file != nullptr) {
        fwrite(elements, 1, 20, file);
        fwrite(digest, 1, 20, file);
        fclose(file);
        status = JNI_TRUE;
    }
    if (filename != nullptr) {
        env->ReleaseStringUTFChars(path, filename);
    }
    if (elements != nullptr) {
        env->ReleaseByteArrayElements(key, elements, JNI_ABORT);
    }
    if (wide != nullptr) {
        env->ReleaseStringUTFChars(text, wide);
    }
    return status;
}

// 0xc60 Java_com_samsung_zirconia_NativeInterface_doPassphraseTest. Derives the passphrase from both strings and
// returns it as a new Java string.
extern "C" JNIEXPORT jstring JNICALL Java_com_samsung_zirconia_NativeInterface_doPassphraseTest(
    JNIEnv* env, jclass cls, jstring str, jstring str2) {
    (void)cls;
    const char* first = env->GetStringUTFChars(str, nullptr);
    const char* second = env->GetStringUTFChars(str2, nullptr);

    const char* derived = nullptr;
    const size_t n = FUN_00010b48(first, second, &derived);
    const size_t size = n + 1;
    char* dest = static_cast<char*>(malloc(size));
    memset(dest, 0, size);
    strncpy(dest, derived, n);

    jstring result = env->NewStringUTF(dest);
    if (dest != nullptr) {
        memset(dest, 0, size);
        free(dest);
    }
    if (first != nullptr) {
        env->ReleaseStringUTFChars(str, first);
    }
    if (second != nullptr) {
        env->ReleaseStringUTFChars(str2, second);
    }
    return result;
}

// 0xe2c Java_com_samsung_zirconia_NativeInterface_checkLicenseFile. Stores the three strings in the file-level
// globals, runs CheckLicenseFile, releases the strings and returns the result.
extern "C" JNIEXPORT jboolean JNICALL Java_com_samsung_zirconia_NativeInterface_checkLicenseFile(
    JNIEnv* env, jclass cls, jstring path, jstring str2, jstring str3) {
    (void)cls;
    g_license_file_path = env->GetStringUTFChars(path, nullptr);
    g_license_str2 = env->GetStringUTFChars(str2, nullptr);
    g_license_str3 = env->GetStringUTFChars(str3, nullptr);

    const jboolean result = CheckLicenseFile() ? JNI_TRUE : JNI_FALSE;

    if (g_license_file_path != nullptr) {
        env->ReleaseStringUTFChars(path, g_license_file_path);
        g_license_file_path = nullptr;
    }
    if (g_license_str2 != nullptr) {
        env->ReleaseStringUTFChars(str2, g_license_str2);
        g_license_str2 = nullptr;
    }
    if (g_license_str3 != nullptr) {
        env->ReleaseStringUTFChars(str3, g_license_str3);
        g_license_str3 = nullptr;
    }
    return result;
}
