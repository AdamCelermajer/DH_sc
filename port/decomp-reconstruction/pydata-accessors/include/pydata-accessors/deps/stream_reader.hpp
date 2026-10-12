#pragma once
// Minimal stand-ins for the engine's stream and allocator, used only by the
// reconstructed name-table readers. Multi-byte values are read little-endian
// (matching the original ARM targets and Windows hosts).
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

class IStreamBase {
public:
    virtual ~IStreamBase() = default;
    // Reads exactly n bytes into dst. Returns false at end of stream.
    virtual bool ReadBytes(void* dst, std::size_t n) = 0;
};

class StreamReader {
public:
    // Original StreamReader::readAs<T>(IStreamBase*): one little-endian value.
    template <class T>
    static T readAs(IStreamBase* stream) {
        unsigned char bytes[sizeof(T)] = {};
        if (stream) stream->ReadBytes(bytes, sizeof(T));
        T value{};
        std::memcpy(&value, bytes, sizeof(T));
        return value;
    }
    // Original StreamReader::readStringEx(stream, dst, len, 0): raw bytes, no terminator.
    static void readStringEx(IStreamBase* stream, char* dst, std::uint64_t len, int) {
        if (stream && len) stream->ReadBytes(dst, static_cast<std::size_t>(len));
    }
};

// Original MemoryHintState(size, hint): allocation (new[] in the original). Some pseudocode calls
// pass only the size; the hint then defaults to 0 here (the original's value is not in the listing).
inline void* MemoryHintState(std::size_t size, int = 0) {
    return std::malloc(size ? size : 1);
}
// Original CustomFree(p).
inline void CustomFree(void* p) { std::free(p); }
