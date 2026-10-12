// vox::VoxUtils: file loaders. Partial. The listing's engine load calls pass the engine
// singleton and a sixth argument that the port's VoxEngine::LoadDataSource* signatures do
// not carry. The port's facade is static, so the engine argument is 0 and the sixth
// argument is dropped. The extension dispatch and the RAM read are reconstructed.
#include <cstdint>
#include <cstring>
#include <string>

#include "vox-audio/deps/vox_file_system.hpp"
#include "vox-audio/vox/memory.hpp"
#include "vox-audio/vox/voxengine.hpp"
#include "vox-audio/vox/voxutils.hpp"

namespace vox {

namespace {

// The "no data" handle the listing writes (id -1, no engine, no handlable).
DataHandle EmptySourceHandle() { return DataHandle(-1, nullptr, nullptr, 0, 0); }

// Source block the RAM load passes to LoadDataSource: the buffer, its size, then two flag
// bytes (cleared, then set) at +0x08 and +0x09 in the listing.
struct RamSourceBlock {
    void* data = nullptr;  // +0x00
    int size = 0;          // +0x04
    std::uint8_t flag_08 = 0;
    std::uint8_t flag_09 = 1;
};

// Decoder type code for the extension after the last dot: wav 1, ogg 2, mpc 3, vxn 4, or 0
// when there is no dot or the extension is unknown. The listing lowercases the extension
// before comparing; this port lowercases A to Z only, which gives the same four matches.
int DecoderFormatForPath(const char* path) {
    if (path == nullptr) {
        return 0;
    }
    const char* dot = std::strrchr(path, '.');
    if (dot == nullptr) {
        return 0;
    }
    std::string ext(dot + 1);
    for (char& c : ext) {
        if (c >= 'A' && c <= 'Z') {
            c = static_cast<char>(c - 'A' + 'a');
        }
    }
    if (ext == "wav") {
        return 1;
    }
    if (ext == "ogg") {
        return 2;
    }
    if (ext == "mpc") {
        return 3;
    }
    if (ext == "vxn") {
        return 4;
    }
    return 0;
}

}  // namespace

// 0x86f3b4 _ZN3vox8VoxUtils27LoadDataSourceFromFileToRAMEPKcii (partial)
// Opens the file in read mode, reads its size (seek to end, tell, seek to start), reads it
// into a VoxAlloc buffer, closes the file and hands the buffer to the engine as a memory
// source. Any failure before the hand-off returns the empty handle.
// The listing's read call passes (destination, 1) with no count; the port passes the
// remaining byte count as the count argument. Its read loop has no end check, so the port
// stops once the buffer is full.
DataHandle VoxUtils::LoadDataSourceFromFileToRAM(const char* path, int format, int unused) {
    (void)unused;
    VoxEngine::GetVoxEngine();

    FileSystemInterface* fs = FileSystemInterface::GetInstance();
    if (fs == nullptr) {
        return EmptySourceHandle();
    }
    FileInterface* file = fs->OpenFile(path, kVoxFileRead);
    if (file == nullptr) {
        return EmptySourceHandle();
    }
    file->Seek(0, 2);
    int size = file->Tell();
    if (size <= 0) {
        fs->CloseFile(file);
        return EmptySourceHandle();
    }
    file->Seek(0, 0);
    auto* buffer = static_cast<std::uint8_t*>(VoxAlloc(static_cast<std::size_t>(size)));
    if (buffer == nullptr) {
        fs->CloseFile(file);
        return EmptySourceHandle();
    }

    int done = 0;
    while (done < size) {
        int got = file->Read(buffer + done, 1, static_cast<std::size_t>(size - done));
        if (got <= 0) {
            break;
        }
        done += got;
    }
    fs->CloseFile(file);

    RamSourceBlock block;
    block.data = buffer;
    block.size = size;
    return VoxEngine::LoadDataSource(0, nullptr, 0, &block, format);
}

// 0x86f558 _ZN3vox8VoxUtils22LoadDataSourceFromFileEPKcii (partial)
// The listing calls the engine's path loader (kind 1) with the format; its sixth argument
// is 0 and is dropped.
DataHandle VoxUtils::LoadDataSourceFromFile(const char* path, int format, int unused) {
    (void)unused;
    VoxEngine::GetVoxEngine();
    return VoxEngine::LoadDataSource(0, nullptr, 1, const_cast<char*>(path), format);
}

// 0x86f5a0 _ZN3vox8VoxUtils27LoadDataSourceFromFileAsRAWEPKcii (partial)
// The listing loads through LoadDataSourceFromFile and passes the result to
// ConvertToRawSource. In the listing the second argument to that call is the engine
// singleton, not the loaded handle; the port converts the loaded handle, which is the
// evident intent. Unconfirmed.
DataHandle VoxUtils::LoadDataSourceFromFileAsRAW(const char* path, int format, int unused) {
    VoxEngine::GetVoxEngine();
    DataHandle loaded = LoadDataSourceFromFile(path, format, unused);
    return VoxEngine::ConvertToRawSource(loaded);
}

// 0x86f5fc _ZN3vox8VoxUtils24LoadDataSourceFromFileExEPKciNS_21VoxSourceLoadingFlagsEi (partial)
// Routes by load flags: bit 0x10000 loads asynchronously; bit 0 loads to RAM; the value 2
// loads as RAW; anything else loads from the file. The listing's async call also passes
// a sixth argument (0) and the user value; the port drops both.
DataHandle VoxUtils::LoadDataSourceFromFileEx(const char* path, int format, int load_flags, int user) {
    VoxEngine::GetVoxEngine();
    if ((load_flags & 0x10000) != 0) {
        return VoxEngine::LoadDataSourceAsync(0, nullptr, 1, const_cast<char*>(path), format,
                                              static_cast<VoxSourceLoadingFlags>(load_flags));
    }
    if ((load_flags & 1) != 0) {
        return LoadDataSourceFromFileToRAM(path, format, user);
    }
    if (load_flags == 2) {
        return LoadDataSourceFromFileAsRAW(path, format, user);
    }
    return LoadDataSourceFromFile(path, format, user);
}

// 0x86f6b0 _ZN3vox8VoxUtils39LoadDataSourceFromFileAutoDetectDecoderEPKci (partial)
// Picks the decoder from the extension, then loads from the file. An unknown or missing
// extension gives the empty handle.
DataHandle VoxUtils::LoadDataSourceFromFileAutoDetectDecoder(const char* path, int unused) {
    int format = DecoderFormatForPath(path);
    if (format == 0) {
        return EmptySourceHandle();
    }
    return LoadDataSourceFromFile(path, format, unused);
}

// 0x86f8e0 _ZN3vox8VoxUtils41LoadDataSourceFromFileAutoDetectDecoderExEPKciNS_21VoxSourceLoadingFlagsEPNS_18DataHandleUserDataE
// As the plain form, but through LoadDataSourceFromFileEx with the load flags and the user value.
DataHandle VoxUtils::LoadDataSourceFromFileAutoDetectDecoderEx(const char* path, int user, int load_flags) {
    int format = DecoderFormatForPath(path);
    if (format == 0) {
        return EmptySourceHandle();
    }
    return LoadDataSourceFromFileEx(path, format, load_flags, user);
}

}  // namespace vox
