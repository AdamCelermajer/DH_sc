#pragma once
#include <array>
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

// Source-backed view of one serialized Collada SEmitter row. This is a port
// value object, not the original object ABI. Field names/offsets follow the
// PS parameters read by CParticleSystemSceneNode::initParticleSystem (0x656450);
// geometry_mode and shape_data follow CColladaFactory::createParticleSystem
// (0x634c64).
enum class EmitterError : std::uint32_t {
    ok, null_input, library, index, row, radius_pointer, shape_pointer,
    direction_pointer, spin_axis_pointer
};
// Emitter rows are valid visual content even with no geometry records. The
// particles_without_geometry classification is source-backed by the factory's
// generated billboard path; it does not mean the BDAE is corrupt or that a
// renderer may substitute an empty/fake FX object.
enum class VisualContentKind : std::uint32_t {
    no_particles_or_geometry, geometry, particles_without_geometry,
    geometry_and_particles, auxiliary_only
};
struct VisualContent {
    std::uint32_t geometry_count = 0;
    std::uint32_t emitter_count = 0;
    std::uint32_t gnps_emitter_count = 0;
    std::uint32_t material_count = 0;
    std::uint32_t effect_count = 0;
    std::uint32_t force_count = 0;
    std::uint32_t coronas_count = 0;
    VisualContentKind kind = VisualContentKind::no_particles_or_geometry;
};
struct Emitter {
    std::uint32_t row_index = 0;
    std::uint32_t row_offset = 0;
    std::uint32_t emitter_type = 0; // +0x08, PS parameter "EmitterType"
    std::uint32_t radius_data_offset = 0; // +0x0c serialized BRES target
    float radius_length = 0;              // target scalar, "RadiusLength"
    float shape_width = 0;                // radius target +4 for EmitterType 0
    float shape_height = 0;               // radius target +8 for type 0; +4 for 1/2
    bool shape_width_present = false;
    bool shape_height_present = false;
    std::int32_t max_particles = 0; // +0x18, "MaxParticles"
    float birth_rate = 0;           // +0x20, "BirthRate"; source zero is preserved
    float life = 0;                 // +0x28, "Life"
    float life_variation = 0;       // +0x2c
    float target_size = 0;          // +0x30
    float size_variation = 0;       // +0x34
    float size_growth_time = 0;     // +0x38
    float size_fade_time = 0;       // +0x3c
    float speed = 0;                // +0x40
    float speed_variation = 0;      // +0x44
    std::uint32_t direction_type = 0; // +0x48
    std::uint32_t direction_data_offset = 0; // +0x4c serialized BRES target
    std::array<float, 3> direction{};
    float direction_variation = 0;
    bool direction_variation_present = false;
    std::uint32_t geometry_mode = 0; // +0x50; source handles mode 2 as geometry
    std::uint32_t shape_data_offset = 0; // +0x54 serialized BRES target
    std::uint32_t shape_source_tag = 0;  // factory cases 1/2/3/4/7
    std::uint32_t shape_flags = 0;       // shape_data +4, bits 2 and 4
    std::uint32_t shape_subtype = 0;     // shape_data +8
    std::array<float, 3> shape_lock_axis_0{}; // shape_data +0x0c, lockAxis(0)
    std::array<float, 3> shape_lock_axis_1{}; // shape_data +0x18, lockAxis(1)
    float animation_offset = 0;            // +0x5c, "AnimOffset"
    float animation_offset_variation = 0;  // +0x60
    std::uint32_t animation_key_mapping_type = 0; // +0x64
    float animation_length = 0;             // +0x68
    float animation_length_variation = 0;   // +0x6c
    float animation_scale_multiplier = 0;   // +0x70
    float animation_scale_variation = 0;    // +0x74
    float spin_time = 0;                    // +0x78
    float spin_variation = 0;               // +0x7c
    float spin_phase = 0;                   // +0x80
    float spin_phase_variation = 0;         // +0x84
    std::uint32_t spin_axis_type = 0;       // +0x88
    std::uint32_t spin_axis_data_offset = 0; // +0x8c serialized BRES target
    std::array<float, 3> spin_axis{};
    float spin_axis_variation = 0;
};
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
dh2::resources::EmitterError dh2_bres_emitter(const dh2::resources::BresView*,
                                               std::int32_t index,
                                               dh2::resources::Emitter* out);
dh2::resources::EmitterError dh2_bres_visual_content(
    const dh2::resources::BresView*, dh2::resources::VisualContent* out);
}
