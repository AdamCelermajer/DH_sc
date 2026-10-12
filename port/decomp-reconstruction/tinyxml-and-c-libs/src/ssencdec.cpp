// SSEncDec blob encoding, from the Hex-Rays pseudocode in recovered/native/hexrays-all/chunk_055.c and the
// LC copies in the chunk that holds 0x89c910. The control flow of the encoder is kept as the pseudocode has it,
// including the goto to the trailing character. Key helpers match the Java Encoder for every input.
//
// Wrappers: XP_API_STRLEN and XP_API_MEMSET are std::strlen and std::memset (net-session-online/src/xp_api.cpp).
// The LC_API_* wrappers have no definition in this repo. The LC data functions and the LC_API_STRLEN_0 uses are
// therefore marked partial in the ledger, and here they call the same code.
#include "tinyxml-and-c-libs/ssencdec.hpp"

#include <cstring>

namespace {

char keyToChar(int index) {
    if (index <= 25) return static_cast<char>(index + 97);
    if (index <= 51) return static_cast<char>(index + 39);
    if (index <= 61) return static_cast<char>(index - 4);
    return index == 62 ? '_' : '-';
}

char charToKey(int c) {
    if (c == 45) return 63;
    if (c == 95) return 62;
    if (c > 57) {
        if (c > 90) return static_cast<char>(c - 97);
        return static_cast<char>(c - 39);
    }
    return static_cast<char>(c + 4);
}

}  // namespace

char SSEncDec_GetCharFromKeyByIndex(int index) {
    return keyToChar(index);
}

char SSEncDec_GetKeyFromChar(char c) {
    return charToKey(c);
}

char SSEncDec_GetCharFromKeyByIndexLC(int index) {
    return keyToChar(index);
}

char SSEncDec_GetKeyFromCharLC(char c) {
    return charToKey(c);
}

unsigned char* SSEncDec_ByteArray2Blob(const unsigned char* bytes, int length) {
    const int v4 = 8 * length / 6;
    const int allocSize = v4 ? v4 + 3 : 2;
    const int v5 = v4 ? v4 + 2 : 1;
    unsigned char* out = new unsigned char[allocSize]();

    int v8 = 1;
    int v9 = 0;
    if (length > 0) {
        int v10 = 0;  // output index
        int v11 = 1;  // trails v10 by one
        int v12 = 8;  // bits left in the current input byte
        int v13 = 0;  // input index
        for (;;) {
            const int v15 = bytes[v13] >> (8 - v12);
            if (v12 <= 5) {
                if (length <= ++v13) {
                    out[v10] = static_cast<unsigned char>(SSEncDec_GetCharFromKeyByIndex(v15 & 0x3F));
                    return out;
                }
                out[v10] = static_cast<unsigned char>(SSEncDec_GetCharFromKeyByIndex((v15 | (bytes[v13] << v12)) & 0x3F));
                v12 += 2;
            } else {
                v12 -= 6;
                if (v12 == 0) {
                    ++v13;
                    v12 = 8;
                }
                out[v10] = static_cast<unsigned char>(SSEncDec_GetCharFromKeyByIndex(v15 & 0x3F));
                v9 = v11;
                if (v13 >= length) {
                    v8 = (v12 == 8);
                    goto trailing;
                }
            }
            ++v10;
            ++v11;
        }
    }
    v8 = 1;
    v9 = 0;

trailing:
    if (v5 != 1 && v8) out[v9] = static_cast<unsigned char>(SSEncDec_GetCharFromKeyByIndex(0));
    return out;
}

int SSEncDec_Blob2ByteArray(unsigned char** out, const char* blob) {
    const std::size_t blobLength = std::strlen(blob);
    const int v4 = static_cast<int>((6 * blobLength) >> 3);
    unsigned char* v7 = new unsigned char[v4 + 1]();
    int v8 = 0;
    int i = 8;
    for (int v9 = 0; v9 < static_cast<int>(blobLength); ++v9) {
        const char key = SSEncDec_GetKeyFromChar(blob[v9]);
        v7[v8] = static_cast<unsigned char>(v7[v8] | (key << (8 - i)));
        if (i <= 6) {
            if (v8 < v4 - 1) {
                const int v12 = v7[++v8] | (key >> i);
                i += 2;
                v7[v8] = static_cast<unsigned char>(v12);
            }
        } else {
            i -= 6;
        }
    }
    *out = v7;
    return v4;
}

unsigned char* SSEncDec_String2Blob(const char* s) {
    return SSEncDec_ByteArray2Blob(reinterpret_cast<const unsigned char*>(s), static_cast<int>(std::strlen(s)));
}

char* SSEncDec_Blob2String(const char* blob) {
    // The original allocates a buffer here and then replaces the pointer with the one from Blob2ByteArray, which
    // leaks the first. That leak is not reproduced.
    unsigned char* out = nullptr;
    SSEncDec_Blob2ByteArray(&out, blob);
    return reinterpret_cast<char*>(out);
}

unsigned char* SSEncDec_ByteArray2BlobLC(const unsigned char* bytes, int length) {
    return SSEncDec_ByteArray2Blob(bytes, length);
}

int SSEncDec_Blob2ByteArrayLC(unsigned char** out, const char* blob) {
    return SSEncDec_Blob2ByteArray(out, blob);
}

char* SSEncDec_String2BlobLC(const char* s) {
    return reinterpret_cast<char*>(SSEncDec_String2Blob(s));
}

char* SSEncDec_Blob2StringLC(const char* blob) {
    return SSEncDec_Blob2String(blob);
}
