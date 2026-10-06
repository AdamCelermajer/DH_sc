#pragma once
#include <cstddef>
#include <cstdint>

// Reconstructed interfaces, not the studio's original class ABI. File offsets
// retain the ARM32 signed-long width; pointers use the target's native width.
namespace dh2::resources {
class ReadFile;
using Completion = void (*)(std::int32_t bytes, std::int32_t error,
                            ReadFile* file, void* user);
class ReadFile {
public:
    virtual std::int32_t read(void*, std::uint32_t) = 0;
    virtual bool read_async(void*, std::uint32_t, Completion, void*) = 0;
    virtual bool read_async_at(void*, std::uint32_t, std::int32_t, Completion, void*) = 0;
    virtual bool seek(std::int32_t, bool relative) = 0;
    virtual std::int32_t size() const = 0;
    virtual std::int32_t position() const = 0;
    virtual const char* name() const = 0;
    virtual const char* full_path() const = 0;
protected:
    ~ReadFile() = default;
};

// Borrowed buffer and names must outlive these objects. Ownership and the
// original glitch::core::string/boost::shared_ptr ABI are not reconstructed.
class MemoryReader final : public ReadFile {
public:
    std::uint8_t* data;
    std::int32_t length, cursor;
    const char* filename;
    MemoryReader(void* bytes, std::int32_t size, const char* name);
    std::int32_t read(void*, std::uint32_t) override;
    bool read_async(void*, std::uint32_t, Completion, void*) override;
    bool read_async_at(void*, std::uint32_t, std::int32_t, Completion, void*) override;
    bool seek(std::int32_t, bool relative) override;
    std::int32_t size() const override;
    std::int32_t position() const override;
    const char* name() const override;
    const char* full_path() const override;
    void* buffer(std::int32_t* position);
    bool valid() const;
    bool all_in_memory() const;
};

class LimitReader final : public ReadFile {
public:
    ReadFile* underlying;
    std::int32_t length, start, end, cursor;
    const char* filename;
    const char* fullpath;
    LimitReader(ReadFile*, std::int32_t size, const char* name, const char* full_path);
    std::int32_t read(void*, std::uint32_t) override;
    bool read_async(void*, std::uint32_t, Completion, void*) override;
    bool read_async_at(void*, std::uint32_t, std::int32_t, Completion, void*) override;
    bool seek(std::int32_t, bool relative) override;
    std::int32_t size() const override;
    std::int32_t position() const override;
    const char* name() const override;
    const char* full_path() const override;
};

enum class BresError : std::uint32_t {
    ok, null_input, short_header, magic, byte_order, already_relocated,
    header_size, file_size, external_base, fixup_table, fixup_field,
    fixup_target, root, capacity, library, string
};

// Immutable view of a complete, little-endian, unrelocated BRES image. No
// serialized pointer is cast to a native C++ struct or widened in place.
struct BresView {
    const std::uint8_t* bytes;
    std::size_t size;
    std::uint32_t fixup_count, fixup_offset, root_offset, tail_offset;
    std::uint32_t bulk_size, block_count, tail_size;
};
struct Fixup {
    const std::uint8_t* field;
    const std::uint8_t* target;
};
enum class Library : std::uint32_t {
    animation, animation_clip, camera, light, image, effect, material,
    geometry, controller, emitter, gnps_emitter, force, coronas, count
};
enum class RootPart : std::uint32_t { animation_clip_library, scene };
struct LibraryLayout {
    std::uint32_t count_offset, pointer_offset, stride;
    const char* name;
};
extern const LibraryLayout libraries[static_cast<unsigned>(Library::count)];
}

extern "C" {
// Setup helpers initialize the new port ABI; they are not translations of the
// original constructors. Storage must be suitably aligned and large enough.
void dh2_memory_init(dh2::resources::MemoryReader*, void*, std::int32_t, const char*);
void dh2_limit_init(dh2::resources::LimitReader*, dh2::resources::ReadFile*,
                    std::int32_t, const char*, const char*);
std::int32_t dh2_memory_read(dh2::resources::MemoryReader*, void*, std::uint32_t);
bool dh2_memory_seek(dh2::resources::MemoryReader*, std::int32_t, bool);
bool dh2_memory_async(dh2::resources::MemoryReader*, void*, std::uint32_t,
                      dh2::resources::Completion, void*);
bool dh2_memory_async_at(dh2::resources::MemoryReader*, void*, std::uint32_t,
                         std::int32_t, dh2::resources::Completion, void*);
void* dh2_memory_buffer(dh2::resources::MemoryReader*, std::int32_t*);
bool dh2_memory_valid(const dh2::resources::MemoryReader*);
bool dh2_memory_all_in_memory(const dh2::resources::MemoryReader*);
std::int32_t dh2_memory_size(const dh2::resources::MemoryReader*);
std::int32_t dh2_memory_position(const dh2::resources::MemoryReader*);
const char* dh2_memory_name(const dh2::resources::MemoryReader*);
const char* dh2_memory_full_path(const dh2::resources::MemoryReader*);
std::int32_t dh2_limit_read(dh2::resources::LimitReader*, void*, std::uint32_t);
bool dh2_limit_seek(dh2::resources::LimitReader*, std::int32_t, bool);
bool dh2_limit_async(dh2::resources::LimitReader*, void*, std::uint32_t,
                     dh2::resources::Completion, void*);
bool dh2_limit_async_at(dh2::resources::LimitReader*, void*, std::uint32_t,
                        std::int32_t, dh2::resources::Completion, void*);
std::int32_t dh2_limit_size(const dh2::resources::LimitReader*);
std::int32_t dh2_limit_position(const dh2::resources::LimitReader*);
const char* dh2_limit_name(const dh2::resources::LimitReader*);
const char* dh2_limit_full_path(const dh2::resources::LimitReader*);

dh2::resources::BresError dh2_bres_open(dh2::resources::BresView*, const void*, std::size_t);
// Each output pair is the native address of a serialized field and its target.
// The caller supplies capacity >= view.fixup_count; no allocations occur.
dh2::resources::BresError dh2_bres_fixups(const dh2::resources::BresView*,
                                        dh2::resources::Fixup*, std::size_t);
std::uint32_t dh2_bres_library_count(const dh2::resources::BresView*,
                                    dh2::resources::Library);
const std::uint8_t* dh2_bres_library_item(const dh2::resources::BresView*,
                                         dh2::resources::Library, std::int32_t);
const std::uint8_t* dh2_bres_root_part(const dh2::resources::BresView*,
                                      dh2::resources::RootPart);
const char* dh2_bres_version(const dh2::resources::BresView*);
}
