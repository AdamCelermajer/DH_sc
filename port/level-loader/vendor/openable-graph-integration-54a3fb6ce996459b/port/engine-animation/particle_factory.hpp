#pragma once
#include <array>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>
namespace dh2::animation {
struct ParticleGeneration16 { float birth_rate; std::uint32_t max_particles,field_c,field_10; };
static_assert(sizeof(ParticleGeneration16)==16);
// Diagnostic source constructor-memory projection. Pointer/vtable/map address
// words are normalized when read. Untouched cells remain caller-seeded; this
// is not a native class ABI or a fabricated AnimationDatabase object.
using ParticleContextSeed92=std::array<std::uint8_t,92>;
class ParticleGenerationOwner;
struct ParticleParameterLease {
 std::shared_ptr<ParticleGenerationOwner> owner;
 void* storage=nullptr;
 explicit operator bool()const noexcept{return storage!=nullptr;}
};
// Only this recovered context/generation domain is supplied. No cloud model,
// material, renderer, simulation or attachment receiver is manufactured.
class ParticleGenerationOwner final:public std::enable_shared_from_this<ParticleGenerationOwner> {
 ParticleContextSeed92 context_;
 ParticleGeneration16 generation_;
 struct Parameter {void* storage=nullptr;std::shared_ptr<void> owner;};
 std::map<std::uint32_t,Parameter> parameters_;
 explicit ParticleGenerationOwner(const ParticleContextSeed92&);
public:
 ParticleGenerationOwner(const ParticleGenerationOwner&)=delete;
 ParticleGenerationOwner& operator=(const ParticleGenerationOwner&)=delete;
 static std::shared_ptr<ParticleGenerationOwner> create(const ParticleContextSeed92&);
 static std::uint32_t hash_name(const char*);
 // Unique insertion preserves the original pointer even if it is null.
 // A real provider can supply its owned model lease; otherwise its borrowed
 // storage must outlive this owner and every parameter lease.
 bool register_parameter(std::uint32_t hash,void* storage,std::shared_ptr<void> storage_owner={});
 // Missing lookup inserts a null entry; the lease still pins its owner.
 ParticleParameterLease parameter(const char* name);
 void* lookup_hash(std::uint32_t hash);
 // Original setParameter<float/int> stores the raw word only when nonnull.
 void set_word(std::uint32_t hash,std::uint32_t word);
 const ParticleGeneration16& generation()const{return generation_;}
 // Excludes opaque process addresses by normalizing vtable/root to0 and
 // header links to0. All remaining bytes are compared.
 ParticleContextSeed92 context_projection()const;
 std::size_t parameter_count()const{return parameters_.size();}
};
// Retains exact BRES bytes, scalar source offsets and copied authored records.
// Restricted actual FX77 descriptor3/mode0 domain; no pointer widening.
struct ParticleEmitterInput {
 std::shared_ptr<const std::vector<std::uint8_t>> resource;
 std::uint32_t record_offset=0;
 std::array<std::uint32_t,36> record{};
 std::array<std::uint32_t,9> descriptor{};
 std::vector<std::uint32_t> shape;
 std::string name;
};
bool decode_particle_emitter(std::shared_ptr<const std::vector<std::uint8_t>>,std::uint32_t index,ParticleEmitterInput&,std::string& error);
// Required earlier PEmitter context/model services. They must really register
// and mutate their source receivers; missing delivery is explicit failure.
struct ParticleGenerationInitServices {
 void* context;
 int (*emitter_type)(void*,ParticleGenerationOwner&,std::uint32_t);
 int (*shape_parameter)(void*,ParticleGenerationOwner&,const char*,std::uint32_t);
};
// Exact recovered source init prefix: EmitterType, authored shape keys, then
// MaxParticles then BirthRate. Remaining Life/etc/cloud/baker/render stages
// are REQUIRED later and this API does not claim a completed particle system.
// 0 prefix complete, -1 malformed input, -2 missing provider, other provider
// diagnostics returned unchanged. Delivered failure preserves source prefix.
int initialize_particle_generation(ParticleGenerationOwner&,const ParticleEmitterInput&,const ParticleGenerationInitServices&);
}
