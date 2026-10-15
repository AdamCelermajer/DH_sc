// glitch::video::CBatchDriver: the constructor, the destructor, the two append-buffer lifetime functions and flush,
// all reconstructed from the listings. thisAppendBatch is still only declared. See
// glitch-video/include/glitch-video/batch_driver.hpp for the full object layout and the evidence for every offset.
//
// LAYERING (the decision, so the next worker does not reopen it): the append buffer is NOT this class's state and is
// not declared as one. It lives in the base glitch::video::IVideoDriver (+0x110 m_batchBuffers, +0x120 m_batchBuffer,
// +0x124 m_batchMaterial, +0x128 m_batchBaker), which this category owns and already declares, and the base holds it
// as an opaque intrusive_ptr<glitch::IReferenceCounted> - drop() is virtual, so the concrete CAppendMeshBuffer is
// destroyed correctly without its class being named in any member. The result: no glitch-video HEADER pulls in a
// glitch-scene header, and this file needs one only at the construction site (operator new plus the constructor, and
// CAppendMeshBuffer::clear in flush). Of the three options the previous worker listed, that is option (a), "a narrow
// accessor" - satisfied by the opaque typed handle the base already has, with no second class declaration to keep in
// sync; option (b), moving the append-buffer part into glitch-scene, is rejected because it would move glitch-video's
// own driver state across the layering line in the wrong direction to serve a type that is never actually required.
// Option (c), a glitch-video header that re-declares CAppendMeshBuffer's members, is rejected as the worst of the
// three: the signatures would then exist twice with nothing checking that they match.
#include "glitch-video/batch_driver.hpp"

#include <cstring>
#include <vector>

// The one glitch-scene header this file needs, and it needs it for the CONSTRUCTION SITE only (operator new plus the
// constructor) and for CAppendMeshBuffer::clear in flush. Neither is stored in a member of this class: the append
// buffer lives in the base IVideoDriver's +0x120 / +0x110 as an opaque intrusive_ptr<IReferenceCounted>. This mirrors
// material_renderer_manager.cpp, which includes a glitch-scene-collada-gui header for the same reason, and it does not
// put a glitch-scene header on any glitch-video header's include path.
#include "glitch-scene-collada-gui/scene/CAppendMeshBuffer.hpp"
#include "glitch-video/buffer.hpp"
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

// The stride argument initBuffer's constructor call passes: the listing materialises 0xFFFFFFFF in R12 and pushes it
// with the usage argument (`MOV R12,#1; STR R12,[SP,#8+var_8]; MOV R12,#0xFFFFFFFF; STR R12,[SP,#8+var_4]`, asm
// 0x5a948-0x5a964), so the value is the all-attributes stream mask, not a byte stride.
constexpr uint32_t kAppendBufferStride = 0xffffffffu;

// IVideoDriver::m_batchBuffer (+0x120) and IVideoDriver::m_batchBuffers (+0x110) hold the append buffers as
// intrusive_ptr<glitch::IReferenceCounted>, so this file never names glitch::scene::CAppendMeshBuffer. drop() is
// virtual and reaches the concrete deleting destructor. The reference count of a CMeshBuffer-derived object starts at
// 0 (CMeshBuffer's constructors store 0), so the first use has to be added explicitly, which is what the original's
// hand-written `++*((_DWORD *)v2 + 1)` does at 0x58a978.
void releaseAppendBuffer(glitch::IReferenceCounted* buffer) {
    if (buffer != nullptr) {
        buffer->drop();
    }
}

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
// The port writes this as the ordinary member teardown: C++ destroys this class's declarations in reverse order
// (maxIndices, segmentSizeMode, segmentSize, and the +0x238 / +0x234 pointer members) and then the base. The two
// reference-counted drops the listing performs at +0x248 and +0x240 are NOT reproduced, and deliberately so: the
// append state is not declared on this class - everything from +0x110 to +0x15F belongs to the base IVideoDriver,
// whose own destructor releases it - and a previous version of the header declared those two words here, which put
// three live pointers inside the base object. Re-declaring them needs a listing that names their type.
CBatchDriver::~CBatchDriver() = default;

// 0x58a920 _ZN6glitch5video12CBatchDriver10initBufferEv
// DEFECT (found by the coverage batch, glitch-video/tests/batch_driver_buffers_test.cpp; the body is the only
// one of the family that no test could enter): the `releaseAppendBuffer(buffer);` below is a SECOND drop of the
// listing's single one. The listing drops the temporary once (0x58a9b0-0x58a9b8, count 2 -> 1) and then stores
// m_batchBuffers[0] into +0x120 with a raw store that adds no use (0x58a9bc-0x58a9c4). Here the C++ temporary
// `intrusive_ptr<IReferenceCounted>(buffer, true)` already drops itself at the end of the push_back statement,
// so by the time releaseAppendBuffer runs the count is 1 and that call deletes the CAppendMeshBuffer inside
// initBuffer; `m_batchBuffer = m_batchBuffers.front()` then add-refs freed memory and both handles dangle.
// Measured with a probe that overrides operator new/delete and calls initBuffer on a leaked driver: the block
// stored in m_batchBuffer was freed inside the call and the process exited 0xC0000374 (heap corruption). With
// `releaseAppendBuffer(buffer);` deleted the same probe reports the block alive, 0 frees, exit 0. Until that
// one line goes, any caller corrupts the heap, so the row is `partial` and the test does not call this.
// Reconstructed. The listing, in order (all of it in members of the base IVideoDriver; the object is R4 = `this`
// throughout, so +0x22C / +0x230 are this class's two capacities and +0x110 / +0x114 / +0x118 / +0x120 are the base's
// batch state):
//   * 0x58a924 `LDR R1,[R0,#0x120]` and 0x58a930 the null test: return at once when m_batchBuffer is set - the whole
//     body is inside that test;
//   * operator new(0x70) (`MOV R0,#0x70; BL _Znwj...`, 0x58a940-0x58a944), then
//     CAppendMeshBuffer(+0x22C, +0x230, this, /*usage*/ 1, /*stride*/ -1) at 0x58a968 (0x6b8cb4), with the injected
//     usage and stride arguments pushed at 0x58a958 / 0x58a964 and the two capacities loaded at 0x58a950 / 0x58a954;
//   * 0x58a970-0x58a978: `if (v2) ++*(v2+1)` - the object is born with count 0, so this adds its first use;
//   * 0x58a97c-0x58a9c8: push_back into m_batchBuffers. The listing does the growth inline (m_batchBuffers is a
//     vector of intrusive_ptr, and its +0x00/+0x04/+0x08 words are the base's +0x110/+0x114/+0x118): the fast path
//     stores at the end pointer, adds a use and bumps the end (0x58a98c-0x58a9a8); the slow path allocates
//     GlitchAlloc(4 * newCapacity) with the `1, 2*old, old` geometric growth of the original, add-refs every element
//     while copying it and calls _M_clear_after_move on the old block (0x58a9cc-0x58aa80). `std::vector`'s own
//     push_back reproduces that growth and that reference handling, and its allocator is SAllocator, so the port
//     writes the push_back;
//   * 0x58a9b0-0x58a9b8: drop the temporary - one use of the object belongs to the vector now;
//   * 0x58a9bc-0x58a9c4: m_batchBuffer = the first element of m_batchBuffers (`LDR R3,[R4,#0x110]; LDR R3,[R3]`), so
//     the port writes front(). Note this is the element the listing picks and not the one just pushed: with a
//     non-empty vector the original would store whatever lived at element 0. Nothing populates the vector before this
//     call (releaseBuffer clears it and this is its only append), so front() is that element.
void CBatchDriver::initBuffer() {
    if (m_batchBuffer) {
        return;
    }
    auto* buffer = reinterpret_cast<glitch::IReferenceCounted*>(
        new glitch::scene::CAppendMeshBuffer(defaultVertexBufferCapacityBytes, defaultIndexBufferCapacityBytes,
                                             static_cast<IVideoDriver*>(this), video::E_BUFFER_USAGE(1),
                                             kAppendBufferStride));
    m_batchBuffers.push_back(boost::intrusive_ptr<glitch::IReferenceCounted>(buffer, true));
    // The listing drops the temporary here (0x58a9b0-0x58a9b8, count 2 -> 1: the vector at +0x110 keeps the owning use).
    // The port must NOT repeat that drop: the C++ temporary intrusive_ptr<IReferenceCounted>(buffer, true) on the
    // push_back above already added and dropped its own use. Calling releaseAppendBuffer(buffer) as well takes the
    // count to 0 and deletes the buffer INSIDE initBuffer, after which m_batchBuffer below add-refs freed memory.
    // Measured: the block stored in m_batchBuffer was freed during the call and the process exited 0xC0000374
    // (STATUS_HEAP_CORRUPTION). Removing this one line makes the same probe report the block alive, 0 frees, exit 0.
    m_batchBuffer = m_batchBuffers.front();
}

// 0x589f40 _ZN6glitch5video12CBatchDriver13releaseBufferEv
// Reconstructed. The listing:
//   * 0x589f44 `LDR R3,[R0,#0x120]` and the null test at 0x589f50: the whole first block runs only when the base's
//     m_batchBuffer is set;
//   * 0x589f58-0x589f70: erase [m_batchBuffers.begin(), m_batchBuffers.end()) - the vector's _M_erase, which is
//     exactly what clear() does for an intrusive_ptr vector (every element loses a use);
//   * 0x589f74-0x589f78: m_batchBuffer = null, which drops the use that member held (the `drop` is inlined here in
//     the original);
//   * 0x589f7c-0x589f90: take the base's +0x128 word out of the member, store null and drop it when it was set. The
//     port's member at that offset is m_batchBaker, an intrusive_ptr, so the assignment drops it;
//   * 0x589f94-0x589f98: tail call to IVideoDriver::clearImplementationDependentData (0x5adfe0). NOTE: that row is
//     `skipped` in this category's ledger and has no body, so this call is a real, intended undefined reference -
//     precisely what the listing does, and the integrator's link report already lists the symbol.
void CBatchDriver::releaseBuffer() {
    if (m_batchBuffer) {
        m_batchBuffers.clear();
        m_batchBuffer = nullptr;
    }
    m_batchBaker = nullptr;
    clearImplementationDependentData();
}

// 0x5a3a20 _ZN6glitch5video12CBatchDriver5flushEv (vtable 0x977108 slot +0x1fc)
// PARTIAL. Written here: everything except the +0x234 callback call. The listing:
//   * 0x5a3a24 m_batchBuffer = the base's +0x120; 0x5a3a2c the test of CAppendMeshBuffer +0x50 and 0x5a3a34 the
//     branch that skips the whole unmap block while that word is zero;
//   * 0x5a3a38-0x5a3a40: the two IBuffer* of the append buffer (+0x58 vertex, +0x5c index) and their map-count byte
//     at IBuffer +0x13; 0x5a3a48 / 0x5a3a6c: while the low 5 bits are greater than 1 only the count is decremented
//     (0x5a3a4c-0x5a3a58, 0x5a3a70-0x5a3a7c);
//   * 0x5a3ad0-0x5a3b1c / 0x5a3b24-0x5a3b34: at one it reads IBuffer's flag byte (+0x12), and when bit 0x20
//     (kFlagUnmapCallback) is set calls vtable +0x18 (= unmapImpl) with the buffer, then clears the count;
//   * 0x5a3a80-0x5a3a88: both mapped pointers (+0x50, +0x54 in this class's own header) are zeroed;
//   * 0x5a3a90-0x5a3abc: the +0x234 callback, called through vtable +0x08 with (the finished append buffer, the
//     base's batch-buffer vector) - NOT WRITTEN, see below;
//   * 0x5a3ac4-0x5a3acc: the tail call to CAppendMeshBuffer::clear (0x6b8878).
// The port writes the unmap with the port's own IBuffer::unmap(), which is the same operation the inlined listing is
// (0x637a68: `--mapCount; if (!mapCount && (flags & 0x20)) unmapImpl();`) and is a no-op for a buffer that was never
// mapped, so no map-count arithmetic is duplicated here. The `+0x50 != 0` gate is kept, because the port's
// CAppendMeshBuffer does keep that word (indexUsedBytes) and the listing gates on it.
// MISSING, named exactly: the call at 0x5a3aa8-0x5a3abc.
//   * the port's glitch::scene::ISegmentCompileCallback (scene_simple_accessors.hpp) declares no operator() at all,
//     so there is nothing to call; the virtual slot the listing calls assumes the interface has one.
//   * the second argument is not the batch material: 0x5a3ab0 computes `ADD R2, R4, #0x124`, i.e. a pointer to the
//     base's m_batchBuffers vector, and R1 holds the finished append buffer, so this is a builder hook over both and
//     its parameter list has to be recovered from the real callback implementation before it can be declared.
//   Guessing either would be exactly the kind of invention rule 5 forbids, so the row stays partial.
void CBatchDriver::flush() {
    if (m_batchBuffer) {
        auto* buffer = reinterpret_cast<scene::CAppendMeshBuffer*>(m_batchBuffer.get());
        if (buffer->indexUsedBytes != 0) {
            auto* vertexBuffer = const_cast<IBuffer*>(buffer->vertexBuffer.get());
            auto* indexBuffer = const_cast<IBuffer*>(buffer->indexBuffer.get());
            if (vertexBuffer != nullptr) {
                vertexBuffer->unmap();
            }
            if (indexBuffer != nullptr) {
                indexBuffer->unmap();
            }
            buffer->mappedIndices = nullptr;
            buffer->mappedVertices = nullptr;
        }
        // The +0x234 callback call (0x5a3aa8-0x5a3abc) is not written; see the comment above.
        buffer->clear();
    }
}

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
