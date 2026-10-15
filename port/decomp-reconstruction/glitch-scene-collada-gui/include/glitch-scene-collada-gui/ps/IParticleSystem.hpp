// glitch::ps particle system core: the two particle records (SParticle, GNPSParticle), IParticleContext<P> (the virtual base every model
// and the system share) and IParticleSystem<P>. Canonical header (residue pass res3); it replaces ps_types.hpp, the recon_17 / recon_18 /
// recon_19 / recon_32 stand-ins of the same classes and the layouts of layouts/ps_members.hpp.
//
// Object model of the original (libDungeonHunter2.so, ARM 32 bit). A particle system is the local class `Mixin` created by
// PSManager::createParticleSystem<P, GenerationModel, SizeModel, ColorModel, EmitterModel, MotionModel, ForcesModel, SpinModel,
// LifeModel, RenderDataModel>. Mixin derives from IParticleSystem<P> and from the nine models; every one of them derives *virtually*
// from IParticleContext<P>, and the models reach the shared state through the virtual-base offset (vptr - 12). The virtual base has
// one vtable that lists the model hooks in this order (byte offsets in the vtable of the virtual base, from the Mixin vtable at
// 0x97B... decoded by tools/decomp_vtable.py):
//   +0 ~D1  +4 ~D0  +8 getRenderVertexStreams  +12 getRenderBufferSizeNeeded  +16 deallocateBuffer  +20 setRenderBuffer
//   +24 getPRand  +28 hasColorAnimationTrack  +32 getWorldMatrixPtr
//   +36 initGenerationModel  +40 generateParticles
//   +44 initPSizeModel  +48 initPSize  +52 applyPSize
//   +56 initPColorModel  +60 initPColor  +64 applyPColor
//   +68 initPEmitterModel  +72 initPPosition
//   +76 initPMotionModel  +80 initPMotion  +84 applyPMotion
//   +88 addPForce  +92 removePForce(PForce*)  +96 removePForce(int)  +100 initPForcesModel  +104 initPForces  +108 applyPForces
//   +112 initPSpinModel  +116 initPSpin  +120 applyPSpin
//   +124 initPLifeModel  +128 initPLife  +132 applyPLife
//   +136 initPRenderDataModel  +140 initPRenderData (empty)  +144 applyPRenderData
// In the port the hooks are virtual member functions of IParticleContext<P>, the models override them, and C++ virtual inheritance
// reproduces the dispatch (the offsets above are the contract of the original and are kept as comments). getRenderData() is
// the seventh virtual of the render model's own vtable; the port lists it here as well so that a scene node can reach it.
//
// IParticleContext<P> layout (32-bit offsets):
//   +0x00 vptr; +0x04 axisLocked[2] (bytes); +0x08 axis[0] (vector3d); +0x14 axis[1]; +0x21 byte "transformAxes" (the +0x20 byte is
//   never written); +0x24 particles (a std::vector<P>: begin / end / capacity); +0x30 the parameter map (a STLport rb-tree
//   <hash, void*>: header +0x30, root +0x34, count +0x40); +0x48 current time; +0x4c previous time; +0x50 delta time; +0x54 bool
//   local-space flag; +0x58 animation database pointer (registered as the parameter "AnimationDatabase").
// IParticleSystem<P> layout: +0x00 vptr; +0x04 PSRandom (the seed); +0x08 the initial seed (restored by init); +0x0c world-matrix
//   pointer (getWorldMatrixPtr returns it; the scene node writes it).
// The particle systems are driven by the scene nodes (collada::CGlitchNewParticleSystemSceneNode for GNPSParticle, scene::
// CParticleSystemSceneNode of the collada factory for SParticle).
#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <vector>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/ps/PDomain.hpp"

namespace boost {
template <typename T>
class intrusive_ptr;
}

namespace glitch::collada {
struct SAnimation;
struct SAnimationOutput;
}  // namespace glitch::collada
namespace glitch::scene {
class CMeshBuffer;
}
namespace glitch::video {
class IBuffer;
class CVertexStreams;
}  // namespace glitch::video

namespace glitch::res {
// glitch::res::SFixedVec<T, N>: N values stored inline (setParameter<SFixedVec<float,3u>> copies the three words).
template <typename T, unsigned N>
struct SFixedVec {
    T value[N]{};
};
}  // namespace glitch::res

namespace glitch::ps {

class PRenderData;
template <typename P>
class PForce;

// A particle of the old system (PSManager::createPCloudSystem). 100 bytes in the original.
struct SParticle {
    core::vector3d<float> position;       // +0x00
    core::vector3d<float> velocity;       // +0x0c
    video::SColor color{0xFFFFFFFFu};     // +0x18
    float texTransform[6]{1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f};  // +0x1c: cos * sx, -sin * sx, tx, sin * sy, cos * sy, ty
    float animOffset = 0.0f;              // +0x34 (the colour-animation start, plus the random offset)
    float animLength = 0.0f;              // +0x38
    float age = 0.0f;                     // +0x3c
    float life = 0.0f;                    // +0x40
    float size = 0.0f;                    // +0x44 (the size now)
    float baseSize = 0.0f;                // +0x48 (the size the growth / fade ramps toward)
    float spinPeriod = 0.0f;              // +0x4c
    float spinAngle = 0.0f;               // +0x50
    core::vector3d<float> spinAxis;       // +0x54
    float distanceSq = 0.0f;              // +0x60 (squared distance to the camera, the alpha-sort key)
};

// A particle of the new system (PSManager::createGNPSSystem). 156 bytes in the original.
struct GNPSParticle {
    core::vector3d<float> position;       // +0x00
    core::vector3d<float> velocity;       // +0x0c
    core::vector3d<float> direction;      // +0x18 (the unit launch direction)
    video::SColor color{0xFFFFFFFFu};     // +0x24
    float texTransform[6]{1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f};  // +0x28 .. +0x3c
    float uv[4]{0.0f, 0.0f, 1.0f, 1.0f};  // +0x40: u0, v0, u1, v1 (the sub-division cell)
    float animOffset = 0.0f;              // +0x50
    float animLength = 0.0f;              // +0x54
    float age = 0.0f;                     // +0x58
    float life = 0.0f;                    // +0x5c
    float size = 0.0f;                    // +0x60
    float baseSize = 0.0f;                // +0x64
    float reserved68 = 0.0f;              // +0x68 (never read or written by a model)
    float spinSpeed = 0.0f;               // +0x6c (the spin speed now: spinSpeedBase times the scale curve)
    float spinAngle = 0.0f;               // +0x70
    core::vector3d<float> spinAxis;       // +0x74
    float spinAngleOffset = 0.0f;         // +0x80
    float spinSpeedBase = 0.0f;           // +0x84
    core::vector3d<float> spinPivot;      // +0x88
    float motionRandom = 0.0f;            // +0x94
    float distanceSq = 0.0f;              // +0x98 (the alpha-sort key)
};
static_assert(sizeof(GNPSParticle) == 156, "GNPSParticle is 156 bytes");
static_assert(sizeof(SParticle) == 100, "SParticle is 100 bytes");

template <typename P>
class IParticleContext {
public:
    using ParticleItt = P*;

    // 0x63b560 (GNPSParticle) / 0x64d1d0 (SParticle): zeroes the state and registers "AnimationDatabase" at +0x58.
    IParticleContext();
    // 0x63a590 / 0x63a51c / 0x63a1f8 (GNPSParticle), 0x64d5bc / 0x64d630 / 0x64d298 (SParticle): the rb-tree and the vector are freed.
    virtual ~IParticleContext();

    // The hooks of the vtable of the virtual base (see the header comment).
    virtual boost::intrusive_ptr<video::CVertexStreams> getRenderVertexStreams() = 0;  // +8
    virtual std::uint32_t getRenderBufferSizeNeeded() const = 0;                       // +12
    virtual void deallocateBuffer() = 0;                                               // +16
    virtual void setRenderBuffer(void* data, std::uint32_t size) = 0;                  // +20
    virtual PSRandom* getPRand() = 0;                                                  // +24
    virtual int hasColorAnimationTrack() = 0;                                          // +28
    virtual const float* getWorldMatrixPtr() const = 0;                                // +32
    virtual void initGenerationModel() = 0;                                            // +36
    virtual P* generateParticles() = 0;                                                // +40
    virtual void initPSizeModel() = 0;                                                 // +44
    virtual void initPSize(P* first, P* last) = 0;                                     // +48
    virtual void applyPSize(P* first, P* last) = 0;                                    // +52
    virtual void initPColorModel() = 0;                                                // +56
    virtual void initPColor(P* first, P* last) = 0;                                    // +60
    virtual void applyPColor(P* first, P* last, video::SColor& color) = 0;             // +64
    virtual void initPEmitterModel() = 0;                                              // +68
    virtual void initPPosition(P* first, P* last) = 0;                                 // +72
    virtual void initPMotionModel() = 0;                                               // +76
    virtual void initPMotion(P* first, P* last) = 0;                                   // +80
    virtual void applyPMotion(P* first, P* last) = 0;                                  // +84
    virtual int addPForce(PForce<P>* force) = 0;                                       // +88
    virtual void removePForce(PForce<P>* force) = 0;                                   // +92
    virtual void removePForce(int index) = 0;                                          // +96
    virtual void initPForcesModel() = 0;                                               // +100
    virtual void initPForces(P* first, P* last) = 0;                                   // +104
    virtual void applyPForces(P* first, P* last) = 0;                                  // +108
    virtual void initPSpinModel() = 0;                                                 // +112
    virtual void initPSpin(P* first, P* last) = 0;                                     // +116
    virtual void applyPSpin(P* first, P* last) = 0;                                    // +120
    virtual void initPLifeModel() = 0;                                                 // +124
    virtual void initPLife(P* first, P* last) = 0;                                     // +128
    virtual void applyPLife(P* first, P* last) = 0;                                    // +132
    virtual void initPRenderDataModel() = 0;                                           // +136
    virtual void initPRenderData(P* first, P* last) = 0;                               // +140 (empty in the original)
    virtual void applyPRenderData(P* first, P* last) = 0;                              // +144
    virtual PRenderData* getRenderData() = 0;                                          // the render model's own vtable (0x640804 ...)

    // 0x63b44c (GNPSParticle) / 0x64d0bc (SParticle): the hash of a parameter name. The original builds a
    // std::string from the text, then folds every byte with the boost hash_combine step
    // `seed ^= c - 0x61C88647 + (seed << 6) + (seed >> 2)`, where c is the SIGN-EXTENDED char (the listing reads
    // it with `LDRSB` at 0x64d130 and the constant it adds is 0x9E3779B9, so 0xff folds as -1). The original's
    // symbol is NON-const (`...10hashStringEPKc`), so that is the overload the definition below implements; the
    // const overload the port's interface declared forwards to it. Explicit instantiations: the non-const
    // (original ABI) form in src/ps/IParticleContext.cpp, the const form implicitly where the port calls it.
    std::uint32_t hashString(const char* text) const {
        return const_cast<IParticleContext*>(this)->hashString(text);
    }
    std::uint32_t hashString(const char* text);

    // 0x630a34 (GNPSParticle) / 0x6309ac (SParticle): locks the axis `index` (the code uses 0 or 1) to half of the
    // direction, normalised first. Verified against the A32 listing of both: `*(this + index + 4) = 1`, then
    // `axis[index] = normalize(direction) * 0.5` (the original calls the out-of-line
    // glitch::core::vector3d<float>::normalize at 0x35e8e0 and multiplies the three components by 0.5 with a
    // separate multiply each), and the x half is returned.
    void lockAxis(unsigned int index, core::vector3d<float> direction);

    // The parameter setters. The slot of the hash is created empty when missing (map operator[]); when it holds storage the value
    // is written through it. Instantiated for the types of the original (bodies in src/ps/IParticleContext.cpp).
    // Verified against the SParticle instances 0x64fcbc, 0x6501d4, 0x650278, 0x650340, 0x650400 and the GNPS
    // instance 0x63efd8: hashString(name) -> map lookup -> when key is absent, insert (hash, null) -> if the
    // slot pointer is non-null, `*slot = value`. Nothing is written after a just-created (null) slot, which is
    // why registering a model member first (registerParameter below) is what makes a later setParameter write.
    template <typename T>
    void setParameter(const char* name, T value);

    // Registers the address of a model member under a parameter name (the model constructors do this).
    // The loop is inlined at every model constructor: 0x641f70 (GNPSSizeModel) registers this+0x04 as
    // "TargetSize", this+0x08 as "SizeVariation", this+0x10 as "SizeScale" and this+0x0c as "SizeScaleOverLife"
    // through four `hashString` + `_Rb_tree::insert_unique` pairs, each inserting (hash, address). There is no
    // out-of-line symbol for it in libDungeonHunter2.so (no queue row); the equivalent std::map form is
    // `parameters[hashString(name)] = storage`.
    void registerParameter(const char* name, void* storage);

    std::uint8_t axisLocked[2]{0, 0};            // +0x04
    core::vector3d<float> axis[2];               // +0x08, +0x14
    std::uint8_t transformAxes = 0;              // +0x21
    std::vector<P> particles;                    // +0x24
    std::map<std::uint32_t, void*> parameters;   // +0x30
    float currentTime = 0.0f;                    // +0x48
    float previousTime = 0.0f;                   // +0x4c
    float deltaTime = 0.0f;                      // +0x50
    bool localSpace = false;                     // +0x54
    const collada::SAnimationOutput* animationDatabase = nullptr;  // +0x58
};

template <typename P>
class IParticleSystem : public virtual IParticleContext<P> {
public:
    using Context = IParticleContext<P>;

    ~IParticleSystem() override = default;  // 0x63a4d0 / 0x63d74c (GNPSParticle), 0x64d570 / 0x64ef44 (SParticle)

    // 0x64bcf0 (SParticle) / the GNPSParticle twin: the random generator at +0x04.
    PSRandom* getPRand() override { return &random; }
    // 0x637bcc / 0x64bbc8: the pointer stored at +0x0c.
    const float* getWorldMatrixPtr() const override { return worldMatrixPtr; }

    // 0x637be4 (GNPSParticle) / 0x64bbe0 (SParticle): restores the seed, clears the times and runs the nine model initialisers.
    // Declaration only, deliberately. The body is a fixed sequence of nine virtual-base slot calls (the model
    // initialisers, in the order the vtable contract at the top of this header lists them) plus the write of the
    // stored seed; the generated Mixin is the only object that owns a concrete instance of every model, and the
    // model bodies are not written yet (see the model declarations below), so an implemented body here would
    // have to guess at the dispatch order and would not be callable anyway without a Mixin. Ledger: partial.
    virtual void init();
    // 0x63e82c (GNPSParticle) / 0x6532cc (SParticle): one time step; `color` is the system colour for models without a colour track.
    // Declaration only for the same reason as init(): the pipeline is a sequence of virtual-base slot calls plus
    // the particle sort and the erase of the dead particles, none of which has a model body yet. Ledger: partial.
    virtual void update(float time, video::SColor& color);

    // 0x630018 .. 0x630180: binds a force. The original allocates the 16-byte PForceProxy and adds it through addPForce.
    template <typename Force>
    int bindForce(typename Force::parameters_type& parameters);

    PSRandom random;                       // +0x04
    std::int32_t initialSeed = 123456789;  // +0x08
    const float* worldMatrixPtr = nullptr; // +0x0c
};

// -------------------------------------------------------------------------------------------------------------------------------
// The out-of-line bodies of IParticleContext<P> and IParticleSystem<P> (src/ps/IParticleContext.cpp, the file line 2
// names). hashString is the only one of the context's declared members that the original emits as its own function:
// the constructor, the destructor, lockAxis, setParameter<T> and registerParameter are inlined at every call site
// there, so libDungeonHunter2.so carries a symbol only for hashString (0x63b44c / 0x64d0bc) and for the setParameter
// instantiations the linker chose to emit out of line. The definitions live here because they belong to a class
// template; src/ps/IParticleContext.cpp instantiates the ones whose body has been checked.
// -------------------------------------------------------------------------------------------------------------------------------

// 0x63b560 _ZN6glitch2ps16IParticleContextINS0_12GNPSParticleEEC2Ev
// 0x64d1d0 _ZN6glitch2ps16IParticleContextINS0_9SParticleEEC2Ev
// The listing zeroes every state word (and the byte at +0x21), then inserts the pair
// (hashString("AnimationDatabase"), this + 0x58) into the tree at +0x30. The port's members carry those same zero
// initialisers, so the body is the registration; "this + 0x58" is the animationDatabase member.
// Deviation: the original seats STLport's _Rb_tree by hand and allocates its nodes with
// glitch::core::SAllocator<..., GlitchAlloc>; the port's std::map uses the default allocator, so a system that
// depended on the GlitchAlloc hint would differ (nothing observable in this class does).
template <typename P>
IParticleContext<P>::IParticleContext() {
    registerParameter("AnimationDatabase", &animationDatabase);
}

    // 0x63a1f8 / 0x63a51c (GNPSParticle), 0x64d298 / 0x64d5bc (SParticle):
    //   v1 = *(this + 0x40);                       // the map's node count
    //   if (v1) { _M_erase(root at +0x34); *(this+0x3c) = header; *(this+0x40) = 0; *(this+0x38) = header;
    //             *(this+0x34) = 0; }
    //   v3 = *(this + 0x24);                       // the particle vector's begin word
    //   if (v3) GlitchFree(v3);
    // The port's std::map at +0x30 and std::vector at +0x24 release exactly that storage in their own destructors,
    // and neither the map's values (void*) nor the particles are owned through them, so the body is empty.
    // Deviation: GlitchFree (glitch-core-io's allocator) versus std::free; the same substitution PS_NEW/PS_FREE record.
template <typename P>
IParticleContext<P>::~IParticleContext() {}

// 0x63b44c _ZN6glitch2ps16IParticleContextINS0_12GNPSParticleEE10hashStringEPKc
// 0x64d0bc _ZN6glitch2ps16IParticleContextINS0_9SParticleEE10hashStringEPKc
// Bodies as the listings show them (both instantiations are the same code):
//   h = 0; for every byte c of text: h ^= (int32_t)(int8_t)c - 1640531527u + (h << 6) + (h >> 2);
// The original builds a temporary std::string (LC_API_STRLEN_0 + _M_range_initialize + _M_deallocate_block) and
// iterates its bytes; the fold is the same over text[0..strlen) with the terminating NUL excluded, so the string
// round-trip is not reproduced (a host-necessitated deviation: it is the only observable difference, and the
// original's own 0xff case sign-extends because the read is `LDRSB`).
template <typename P>
std::uint32_t IParticleContext<P>::hashString(const char* text) {
    std::uint32_t seed = 0;
    for (const char* c = text; *c != '\0'; ++c) {
        const auto byte = static_cast<std::uint32_t>(static_cast<std::int32_t>(*c));  // sign-extended, as LDRSB
        seed ^= byte - 0x61C88647u + (seed << 6) + (seed >> 2);
    }
    return seed;
}

// 0x630a34 (GNPSParticle) / 0x6309ac (SParticle). The listing, both instantiations byte-for-byte equal:
//   *(uint8_t*)(this + index + 4) = 1;
//   core::vector3d<float> normalised = direction;      // copy to a stack slot
//   glitch::core::vector3d<float>::normalize(&normalised);
//   core::vector3d<float>& slot = *(core::vector3d<float>*)(this + 12 * index);   // axis[index], so +0x08 + 12*i
//   slot.X = normalised.X * 0.5f;  slot.Y = normalised.Y * 0.5f;  slot.Z = normalised.Z * 0.5f;
//   return slot.X;
// Deviations: (1) the port writes axis[2] as a member array, so the address arithmetic the listing does is the
// member select; (2) the bounds check on `index` is the port's guard — the original indexes without one, so a
// caller with index > 1 corrupts memory there. No caller in the port passes anything but 0 or 1.
template <typename P>
void IParticleContext<P>::lockAxis(unsigned int index, core::vector3d<float> direction) {
    if (index >= 2) {  // guard added; the original writes past the array
        return;
    }
    axisLocked[index] = 1;
    direction.normalize();
    axis[index].X = direction.X * 0.5f;
    axis[index].Y = direction.Y * 0.5f;
    axis[index].Z = direction.Z * 0.5f;
}

// The map lookup shared by every setParameter instantiation, exactly as the listings show it (for example
// 0x650400 for float and 0x650340 for vector3d):
//   key = hashString(name);
//   node = lower_bound(root, key);                      // the _Rb_tree walk the listing spells out
//   if (node is the header sentinel || key < node->key) node = insert_unique(node, key, nullptr);
//   if (node->value) *node->value = value;              // nothing is written into a fresh null slot
// std::map::operator[] is that lookup-and-insert (its lower_bound/insert_unique are the same STLport pair), so
// the three statements below are the body. `parameters` is std::map where the original embeds STLport's
// _Rb_tree<unsigned, pair<const unsigned, void*>> at +0x30; the observable behaviour (insert null, write only
// through a registered slot, return nothing) is the same.
template <typename P>
template <typename T>
void IParticleContext<P>::setParameter(const char* name, T value) {
    void*& slot = parameters[hashString(name)];
    if (slot != nullptr) {
        *static_cast<T*>(slot) = value;
    }
}

// As above: the original's four insert_unique calls in GNPSSizeModel::GNPSSizeModel (0x641f70) insert the pair
// (hashString(name), storage) directly; std::map::operator= on the slot is the same insert-or-overwrite.
template <typename P>
void IParticleContext<P>::registerParameter(const char* name, void* storage) {
    parameters[hashString(name)] = storage;
}

// The nine models of PSManager::createParticleSystem<P, ...>, each deriving virtually from IParticleContext<P>.
// Signatures only: the virtual hooks the original's vtable holds for each model, taken from the symbol names in
// queue.csv (which carry the exact parameter types) and from the queue's own demangled return types. The bodies
// are NOT written — most are large (the GNPSColorModel constructor is 1128 bytes, applyPColor 3.4 KB) and the
// ones that are small still need the model's parameter map entries and the particle record fields, so guessing
// would violate rule 5. What these declarations buy is the thing the slices were missing: the class exists, the
// virtual signatures are exact, and a body can now attach to `Model<P>::hook` instead of a stand-in free function.
//
// Member offsets are the original 32-bit ones, kept as comments (ps_evidence.hpp holds the constructor evidence).
// Where a model owns an `initP...Model` hook the original's body is one instruction (a return), and the queue
// records the sizes: 4 bytes for GNPSSizeModel/GNPSColorModel/GNPSMotionModel/PForcesModel/GNPSSpinModel/
// GNPSLifeModel, i.e. empty bodies. Those are the only bodies the declarations deliberately leave out too: an
// empty override would be correct but the row cannot be marked reconstructed until each is checked against its
// own listing, so they are declared here and stay partial.

// 0x641f70 ctor, 0x63802c initPSizeModel (4 bytes), 0x638040 initPSize, 0x63c388 applyPSize, 0x63a438/0x63d7f4 dtors.
template <typename P>
class GNPSSizeModel : public virtual IParticleContext<P> {
public:
    ~GNPSSizeModel() override = default;
    void initPSizeModel() override;
    void initPSize(P* first, P* last) override;
    void applyPSize(P* first, P* last) override;

    // Bound by name in the ctor: +0x04 "TargetSize" (1.0f), +0x08 "SizeVariation", +0x10 "SizeScale",
    // +0x0c "SizeScaleOverLife" — the four registerParameter pairs, in that order.
    float targetSize = 1.0f;     // +0x04
    float sizeVariation = 0.0f;  // +0x08
    float sizeScaleOverLife = 0.0f;  // +0x0c
    float sizeScale = 0.0f;      // +0x10
};

// 0x6420a8 ctor, 0x6380d8 initPColorModel (4 bytes), 0x6380ec initPColor, 0x638524 hasColorAnimationTrack,
// 0x63c5a8 applyPColor, 0x63a3ec/0x63d848 dtors.
template <typename P>
class GNPSColorModel : public virtual IParticleContext<P> {
public:
    ~GNPSColorModel() override = default;
    void initPColorModel() override;
    void initPColor(P* first, P* last) override;
    int hasColorAnimationTrack() override;
    void applyPColor(P* first, P* last, video::SColor& color) override;

    // ps_evidence.hpp GNPSColorModel_ev: +0x04 "TextureTransformTrack", +0x08 "DiffuseColorTrack",
    // +0x0c "AnimKeyMappingType" (1), +0x10 "AnimOffset", +0x14 "AnimOffsetVariation", +0x18 "AnimLength",
    // +0x1c "AnimLengthVariation", +0x20 "AnimScaleMultiplier" (1.0f), +0x24 its variation, +0x28 "TintStrength",
    // +0x2c "TintColor", +0x30 "Visibility", +0x34 "RandomColor", +0x38 "TextureSubDiviseH" (1),
    // +0x3c "TextureSubDiviseV" (1), +0x40 the subdivision random pick, +0x48 "FlipU", +0x4c "FlipV".
    void* textureTransformTrack = nullptr;  // +0x04, an SAnimationAccessor*
    void* diffuseColorTrack = nullptr;      // +0x08, an SAnimationAccessor*: hasColorAnimationTrack tests this one
};

// 0x6415ac ctor, 0x64e750 (SParticle) / 0x63c4c8 (GNPSParticle) initPEmitterModel, 0x638544 (GNPS) /
// 0x64c08c (SParticle) initPPosition, 0x63a350+0x64d3f0 dtors.
template <typename P>
class PEmitterModel : public virtual IParticleContext<P> {
public:
    ~PEmitterModel() override = default;
    void initPEmitterModel() override;
    void initPPosition(P* first, P* last) override;

    // ps_evidence.hpp PEmitterModel_GNPS_ev (ctor 0x6415ac allocates a 0x5C-byte domain and runs
    // PDBox(-1,-1,-1 .. +1,+1,+1)): +0x04 the domain pointer (a PDomain*), +0x08 "EmitterType",
    // +0x0c "RadiusLength" (2.0f), +0x10 "Width" (2.0f), +0x14 "Height" (2.0f).
    PDomain* domain = nullptr;  // +0x04
    int emitterType = 0;        // +0x08
    float radiusLength = 2.0f;  // +0x0c
    float width = 2.0f;         // +0x10
    float height = 2.0f;        // +0x14
};

// 0x6540ec (SParticle) ctor, 0x64c17c (SParticle) / 0x638634 (GNPS) initPMotionModel (4 bytes),
// 0x653bb8 (SParticle) / 0x6410b0 (GNPS) initPMotion, 0x64c190 (SParticle) applyPMotion; the GNPS twin
// (GNPSMotionModel, ctor 0x641724, applyPMotion 0x63beec) has the same shape and is declared separately below.
template <typename P>
class PMotionModel : public virtual IParticleContext<P> {
public:
    ~PMotionModel() override = default;
    void initPMotionModel() override;
    void initPMotion(P* first, P* last) override;
    void applyPMotion(P* first, P* last) override;
};

// The GNPS twin of PMotionModel: ctor 0x641724, initPMotionModel 0x638634 (4 bytes), initPMotion 0x6410b0,
// applyPMotion 0x63beec, dtors 0x63a304/0x63d89c.
template <typename P>
class GNPSMotionModel : public virtual IParticleContext<P> {
public:
    ~GNPSMotionModel() override = default;
    void initPMotionModel() override;
    void initPMotion(P* first, P* last) override;
    void applyPMotion(P* first, P* last) override;

    // ps_evidence.hpp GNPSMotionModel_ev (ctor 0x641724): +0x04 "Direction", +0x10 "DirectionVariation",
    // +0x1c "MotionRandom", +0x20 "MotionRandomVariation", +0x24 "SpeedScaleOverLife",
    // +0x28 "MotionRandomScaleOverLife", +0x2c "SpeedScale", +0x30 "MotionRandomScale", +0x34 "Speed",
    // +0x38 "SpeedVariation".
    core::vector3d<float> direction;          // +0x04
    core::vector3d<float> directionVariation; // +0x10
    float motionRandom = 0.0f;                // +0x1c
    float motionRandomVariation = 0.0f;       // +0x20
    float speedScaleOverLife = 0.0f;          // +0x24
    float motionRandomScaleOverLife = 0.0f;   // +0x28
    float speedScale = 0.0f;                  // +0x2c
    float motionRandomScale = 0.0f;           // +0x30
    float speed = 0.0f;                       // +0x34
    float speedVariation = 0.0f;              // +0x38
};

// 0x63a5ac ctor?/0x64d64c dtors, addPForce 0x63ec44 (GNPS) / 0x64f190 (SParticle), removePForce(PForce*)
// 0x63da68 / 0x64f278, removePForce(int) 0x63dc3c / 0x64f318, initPForcesModel 0x63b188 / 0x64d9d4,
// initPForces 0x638648 / 0x64c234 (4 bytes), applyPForces 0x63b0fc / 0x64d948.
template <typename P>
class PForcesModel : public virtual IParticleContext<P> {
public:
    ~PForcesModel() override = default;
    int addPForce(PForce<P>* force) override;
    void removePForce(PForce<P>* force) override;
    void removePForce(int index) override;
    void initPForcesModel() override;
    void initPForces(P* first, P* last) override;
    void applyPForces(P* first, P* last) override;
};

// 0x654218 (SParticle) ctor, 0x64c248 (SParticle) / 0x63865c (GNPS) initPSpinModel (4 bytes),
// 0x64e3c8 (SParticle) initPSpin, 0x64c25c (SParticle) applyPSpin.
template <typename P>
class PSpinModel : public virtual IParticleContext<P> {
public:
    ~PSpinModel() override = default;
    void initPSpinModel() override;
    void initPSpin(P* first, P* last) override;
    void applyPSpin(P* first, P* last) override;
};

// The GNPS twin: ctor 0x6419dc, initPSpinModel 0x63865c (4 bytes), initPSpin 0x63cce8, applyPSpin 0x63d390,
// dtors 0x63a2b8/0x63d8f0.
template <typename P>
class GNPSSpinModel : public virtual IParticleContext<P> {
public:
    ~GNPSSpinModel() override = default;
    void initPSpinModel() override;
    void initPSpin(P* first, P* last) override;
    void applyPSpin(P* first, P* last) override;

    // ps_evidence.hpp GNPSSpinModel_ev (ctor 0x6419dc): +0x04 "SpinTime", +0x08 "SpinVariation",
    // +0x0c "SpinPhase", +0x10 "SpinPhaseVariation", +0x14 "SpinAxis", +0x20 "SpinAxisVariation",
    // +0x2c "StartAngleType", +0x30 "SpinKeepAlignedToMotion", +0x78 "SpinPivotPoint",
    // +0x84 "SpinScaleOverLife", +0x88 "SpinScale", +0x8c "SpinAxisType". The 68 bytes at +0x34 are
    // unbound in the ctor evidence and are left out.
    float spinTime = 0.0f;                  // +0x04
    float spinVariation = 0.0f;             // +0x08
    float spinPhase = 0.0f;                 // +0x0c
    float spinPhaseVariation = 0.0f;        // +0x10
    core::vector3d<float> spinAxis;         // +0x14
    core::vector3d<float> spinAxisVariation;  // +0x20
    int startAngleType = 0;                 // +0x2c
    int spinKeepAlignedToMotion = 0;         // +0x30
    core::vector3d<float> spinPivotPoint;   // +0x78 (bytes +0x34..+0x77 unbound)
    float spinScaleOverLife = 0.0f;         // +0x84
    float spinScale = 0.0f;                 // +0x88
    int spinAxisType = 0;                   // +0x8c
};

// 0x65452c (SParticle) ctor, 0x64c2e8 (SParticle) / 0x638670 (GNPS) initPLifeModel (4 bytes),
// 0x64c2fc (SParticle) initPLife, 0x64c3f4 (SParticle) applyPLife.
template <typename P>
class PLifeModel : public virtual IParticleContext<P> {
public:
    ~PLifeModel() override = default;
    void initPLifeModel() override;
    void initPLife(P* first, P* last) override;
    void applyPLife(P* first, P* last) override;
};

// The GNPS twin: ctor 0x641d7c, initPLifeModel 0x638670 (4 bytes), initPLife 0x638684, applyPLife 0x63904c,
// dtors 0x63a26c/0x63d944. ps_evidence.hpp GNPSLifeModel_ev: +0x04 "Life", +0x08 "LifeVariation".
template <typename P>
class GNPSLifeModel : public virtual IParticleContext<P> {
public:
    ~GNPSLifeModel() override = default;
    void initPLifeModel() override;
    void initPLife(P* first, P* last) override;
    void applyPLife(P* first, P* last) override;

    float life = 0.0f;           // +0x04
    float lifeVariation = 0.0f;  // +0x08
};

// 0x6545d0 ctor, 0x64fc50 initGenerationModel, 0x65317c generateParticles, 0x64d524/0x64ef98 dtors.
// (The GNPS generation model is the same shape; only the SParticle constructor is in the queue.)
template <typename P>
class PGenerationModel : public virtual IParticleContext<P> {
public:
    ~PGenerationModel() override = default;
    void initGenerationModel() override;
    P* generateParticles() override;
};

// 0x652384 (SParticle, NullColor bakers) / 0x6524dc (SParticle, Billboard bakers) / 0x63fa14 (GNPS, Billboard) /
// 0x63fb6c (GNPS, NullColor) initPRenderDataModel, getRenderVertexStreams 0x64c448 / 0x64c4b0 / 0x638728 /
// 0x638790, getRenderBufferSizeNeeded 0x64c484 / 0x64c4ec / 0x638764 / 0x6387cc, deallocateBuffer 0x64dba0 /
// 0x64dc3c / 0x63b6c4 / 0x63b760, setRenderBuffer 0x64dbe8 / 0x64dc84 / 0x63b70c / 0x63b7a8,
// initPRenderData (empty, 4 bytes) 0x64c49c / 0x64c504 / 0x63877c / 0x6387e4, applyPRenderData 0x651eec /
// 0x652138 / 0x639884 / 0x639ad0, getRenderData(int) 0x6558b0 / 0x65605c / 0x640804 / 0x640fb0,
// dtors 0x6539a8/0x653ab0/0x63fcc4/0x63fdcc (and the SParticle GNPS twins).
// The five baker types are the ones the queue's symbol names carry; they are forward-declared here because the
// render-data body only stores them, and their own definitions live with the video-side bakers.
template <typename Baker>
class PSGenericNormalBaker;
template <typename Baker>
class PSGenericPositionBaker;
template <typename Baker>
class PSGenericTexCoordsBaker;
template <typename Baker>
class PSNullColorBaker;
template <typename Baker>
class PSBillboardColorBaker;
template <typename Baker>
class PSBillboardNormalBaker;
template <typename Baker>
class PSBillboardPositionBaker;
template <typename Baker>
class PSBillboardTexCoordsBaker;
class PSNullShaderParametersBaker;

template <typename P, typename ShaderParametersBaker, typename ColorBaker, typename NormalBaker, typename PositionBaker,
          typename TexCoordsBaker>
class PRenderDataBillboardModel : public virtual IParticleContext<P> {
public:
    ~PRenderDataBillboardModel() override = default;
    boost::intrusive_ptr<video::CVertexStreams> getRenderVertexStreams() override;
    std::uint32_t getRenderBufferSizeNeeded() const override;
    void deallocateBuffer() override;
    void setRenderBuffer(void* data, std::uint32_t size) override;
    void initPRenderDataModel() override;
    void initPRenderData(P* first, P* last) override;  // empty in the original
    void applyPRenderData(P* first, P* last) override;
    // 0x640804 family: the seventh virtual of the render model's own vtable (the header comment at the top),
    // not of the virtual base, so it is not an override. The argument is the queue's `int` index.
    PRenderData* getRenderData(int index);

    // The word the getRenderVertexStreams/deallocateBuffer/setRenderBuffer bodies read at +0x10 (an
    // intrusive_ptr target whose count is at target +0x00), and the size-needed word at +0x98. The 5.1 KB
    // buffer bodies that own the other words are not written yet, so these two are the only offsets named.
    void* renderStream = nullptr;                        // +0x10
    std::uint32_t renderBufferSizeNeeded = 0;            // +0x98
};

}  // namespace glitch::ps
