// glitch::video::CBatchDriver: the constructor and the destructor, both reconstructed from the listings, plus the
// declarations of the append path (initBuffer / releaseBuffer / thisAppendBatch / flush), which need the
// glitch-scene-collada-gui CAppendMeshBuffer and are not written yet. See glitch-video/include/glitch-video/
// batch_driver.hpp for the full object layout and the evidence for every offset.
#include "glitch-video/batch_driver.hpp"

#include <cstring>

#include "glitch-video/null_driver.hpp"
#include "glitch-video/video_driver.hpp"

namespace glitch::video {

namespace {

// The option group IVideoDriver::setOption raises in the constructor: `setOption(0x100, 1)` is the first thing
// 0x5a2e20 does after storing the members (asm 0x5a2e8c-0x5a2e94). The name is not recovered from the binary; 0x100
// is the bit IVideoDriver::setOption tests to decide whether to run its per-option hook.
constexpr uint32_t kOptionGroupBatch = 0x100;

// The number of capability-word bits the constructor copies from the owning driver: the loop at 0x5a2ea0-0x5a2ec4
// runs while its counter is not 0x19, so it visits bits 0 .. 24.
constexpr uint32_t kCapabilityBitCount = 25;

}  // namespace

// 0x5a2e20 _ZN6glitch5video12CBatchDriverC1EjjPNS0_12IVideoDriverE (complete object)
// 0x5a2ed8 _ZN6glitch5video12CBatchDriverC2EjjPNS0_12IVideoDriverE (base object)
// Reconstructed. The listing, in order:
//   * CNullDriver::CNullDriver(this, driver)  (0x5b9764) - the parameter is the IVideoDriver* argument, and
//     CNullDriver's constructor is the overload that shares another driver's device, managers and default light;
//   * vptr = _ZTVN6glitch5video12CBatchDriverE + 8 (the +8 is the vtable header of that toolchain; the class's
//     vtable starts at 0x977108, the compiler adds 8 which lands on the destructor slot). The port's compiler does
//     this itself.
//   * +0x250 / +0x254 / +0x258 (segmentSize) and +0x25C (segmentSizeMode) = 0;
//   * +0x22C = the vertex capacity argument, +0x230 = the index capacity argument;
//   * +0x234, +0x238, +0x23C, +0x240 and +0x248 = null (the stores at 0x5a2e74-0x5a2e84);
//   * IVideoDriver::setOption(this, 0x100, true) (0x5a90b0), which ORs 0x100 into this object's own option word
//     (IVideoDriver +0x88);
//   * the capability sync: for bit in 0 .. 24, this->m_capabilities (IVideoDriver +0x9c) gets the bit of the owning
//     driver's m_capabilities (asm 0x5a2e98 LDR R3,[R4,#0x9C]; 0x5a2ea8 LDR R1,[R5,#0x9C]; ORRNE/BICEQ; 0x5a2ec0
//     STR R3,[R4,#0x9C]). The object is a CNullDriver, so +0x9C is the inherited capability word the texture-slot
//     check reads; the batch driver answers the same texture-type questions as the driver it wraps.
// Deviation (host): the owning driver may be null, in which case the capability loop and IVideoDriver::setOption are
// skipped. The original dereferences it unconditionally; the port's scene tests build a CSceneManager without a
// video driver, and the makeBatchDriver helper in CSceneManager_virtuals.cpp only calls this constructor when the
// driver is non-null anyway.
CBatchDriver::CBatchDriver(uint32_t vertexBufferCapacityBytes, uint32_t indexBufferCapacityBytes, IVideoDriver* driver)
    : CNullDriver(driver) {
    defaultVertexBufferCapacityBytes = vertexBufferCapacityBytes;
    defaultIndexBufferCapacityBytes = indexBufferCapacityBytes;
    if (driver == nullptr) return;
    setOption(kOptionGroupBatch, true);  // IVideoDriver::setOption (0x5a90b0) on this object
    const uint32_t mask = (kCapabilityBitCount >= 32) ? 0xffffffffu : ((1u << kCapabilityBitCount) - 1u);
    m_capabilities = (m_capabilities & ~mask) | (driver->m_capabilities & mask);
}

// 0x5a3d50 _ZN6glitch5video12CBatchDriverD1Ev (complete object)
// 0x5a3dd8 _ZN6glitch5video12CBatchDriverD0Ev (deleting destructor, skipped in the ledger)
// Reconstructed. The listing: the vptr is reset to the class's vtable, the word at +0x248 is dropped when it is not
// null (asm 0x5a3d64 LDR R0,[R0,#0x248]; 0x5a3d7c drop), then the intrusive_ptr at +0x240 is released the inline way
// (asm 0x5a3d80-0x5a3dbc: the count word of the pointed-to CVertexStreams is decremented and, at zero, the
// destructor 0x5a0a1c runs and the block is freed), and finally CNullDriver::~CNullDriver (0x5b9524) runs.
// The port writes this as the ordinary member teardown: C++ destroys members in reverse declaration order before
// the base class, which for this class walks the members from +0x260 down to +0x234 and then the base, and the append
// buffer at +0x23C (an intrusive_ptr here, a raw pointer in the original) is released with the rest. That covers the
// explicit +0x248 drop the listing performs - the member is declared as an intrusive_ptr<glitch::IReferenceCounted>
// for exactly that reason; if +0x248 and +0x24C turn out to be plain data, drop the intrusive_ptr and this destructor
// becomes empty.
CBatchDriver::~CBatchDriver() = default;

// 0x5a8a80 _ZN6glitch5video12CBatchDriver4drawERKN5boost13intrusive_ptrIKNS0_14CVertexStreamsEEERKNS0_
// 16CPrimitiveStreamEPPNS0_14CDriverBindingERKNS3_IKNS_5scene11CMeshBufferEEE (vtable 0x977108 slot +0x58)
// Reconstructed. The listing is two instructions - `LDR R3, [SP,#arg_0]` (the 4th parameter is on the stack and is
// only passed on) and `B 0x5a6fe0` (a tail call) - so the body is exactly the forwarding call below. Like the
// original it ignores the CDriverBinding** third parameter, and like the original it leaves thisAppendBatch to do
// everything; thisAppendBatch is not written yet, so this member only links the vtable today.
void CBatchDriver::draw(const boost::intrusive_ptr<const CVertexStreams>& streams,
                        const CPrimitiveStream& primitives, CDriverBinding** /*binding*/,
                        const boost::intrusive_ptr<const scene::CMeshBuffer>& meshBuffer) {
    thisAppendBatch(streams, primitives, meshBuffer);
}

}  // namespace glitch::video
