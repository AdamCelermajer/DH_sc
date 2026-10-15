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
// (+0x260 maxIndices, +0x25C segmentSizeMode, +0x250 segmentSize, the +0x248 mesh-buffer handle, the +0x244 raw
// primitive-stream pointer, the +0x240 vertex-streams handle, the +0x23C callback and the +0x238 / +0x234
// callbacks) and then the base. The two reference-counted drops the listing performs - +0x248 through the virtual
// drop (0x5a3d64 / 0x5a3d7c) and +0x240 inline through CVertexStreams::~CVertexStreams 0x5a0a1c plus free
// (0x5a3d80-0x5a3dbc) - ARE reproduced now: the four words +0x23C .. +0x248 were declared as members of this class
// (+0x240 and +0x248 as counting intrusive_ptr, so their destructors are exactly those two drops, in that order,
// ahead of the base). The one remaining deviation is structural, not behavioural: the original frees the
// CVertexStreams block with CustomFree (glitch::memory's free) where the port's intrusive_ptr_release calls
// `delete`, which is the same allocation path every other counted glitch-video object already uses here.
CBatchDriver::~CBatchDriver() = default;

// 0x58a920 _ZN6glitch5video12CBatchDriver10initBufferEv
// RESOLVED - the DEFECT block below is KEPT as the record of what was measured, but it is no longer true of the
// code under it: the body no longer calls releaseAppendBuffer, and the function name is now also flagged by
// -Wunused-function. Do NOT re-add the call because the paragraph below says the body has it - the paragraph says
// what happens IF it is re-added. The body is at the bottom of this comment: push_back(the handle), then
// m_batchBuffer = m_batchBuffers.front(), with no second drop.
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
//
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
// Reconstructed. The listing:
//   * 0x5a3a24 m_batchBuffer = the base's +0x120; 0x5a3a2c the test of CAppendMeshBuffer +0x50 and 0x5a3a34 the
//     branch that skips the whole unmap block while that word is zero;
//   * 0x5a3a38-0x5a3a40: the two IBuffer* of the append buffer (+0x58 vertex, +0x5c index) and their map-count byte
//     at IBuffer +0x13; 0x5a3a48 / 0x5a3a6c: while the low 5 bits are greater than 1 only the count is decremented
//     (0x5a3a4c-0x5a3a58, 0x5a3a70-0x5a3a7c);
//   * 0x5a3ad0-0x5a3b1c / 0x5a3b24-0x5a3b34: at one it reads IBuffer's flag byte (+0x12), and when bit 0x20
//     (kFlagUnmapCallback) is set calls vtable +0x18 (= unmapImpl) with the buffer, then clears the count;
//   * 0x5a3a80-0x5a3a88: both mapped pointers (+0x50, +0x54 in this class's own header) are zeroed;
//   * 0x5a3a90-0x5a3abc, decoded register by register: R3 = driver + 0x234, return when it is null (0x5a3a94);
//     R2 = append buffer + 0x3C (vertexUsedBytes), return when that is zero (0x5a3a98); R0 = R3 (the callback),
//     R1 = R5 (the append buffer), R2 = driver + 0x124 (the base's m_batchMaterial, 0x5a3aa4 `ADD R2,R4,#0x124`),
//     then the vtable load and `LDR PC,[R3,#8]`. So the signature is
//     `operator()(CAppendMeshBuffer& buffer, intrusive_ptr<CMaterial>& material)` and the port's call matches it
//     argument for argument.
//   * 0x5a3ac4-0x5a3acc: the tail call to CAppendMeshBuffer::clear (0x6b8878).
// The port writes the unmap with the port's own IBuffer::unmap(), which is the same operation the inlined listing is
// (0x637a68: `--mapCount; if (!mapCount && (flags & 0x20)) unmapImpl();`) and is a no-op for a buffer that was never
// mapped, so no map-count arithmetic is duplicated here. The `+0x50 != 0` gate is kept, because the port's
// CAppendMeshBuffer does keep that word (mappedVertices) and the listing gates on it.
// SETTLED, not open: the slot is +0x234 and the interface it holds IS IEndOfBatchCallback. The earlier note here
// read compileInternal 0x58facc as filling +0x234 with an SDefaultSegmentCompileCallback; it is the other way
// round, and four register-level facts from that listing settle it: the two stack objects are `v14[5]` at SP+0 and
// `v20[6]` at SP+0x2C; `v14[0] = off_9762B8` (0x58fb34) and off_9762B8 = _ZTVN6glitch5scene26SDefaultEndOfBatch-
// CallbackE + 8 (the symbol is at 0x9762b0); `v20[0] = off_9762A0` (0x58fb0c) and off_9762A0 = _ZTVN6glitch5scene30-
// SDefaultSegmentCompileCallbackE + 8 (0x976298); and the stores are `*(driver + 564) = v14` (0x58fb64) and
// `*(driver + 568) = v20` (0x58fb70). So driver +0x234 holds the SDefaultEndOfBatchCallback object - the same v14
// that `SDefaultEndOfBatchCallback::finalize(v14)` runs on (0x58fbfc `MOV R0,SP`, 0x58fc00 the call) - and +0x238
// holds the segment-compile one. flush therefore calls the END-OF-BATCH slot, which is what the argument shape
// (buffer, material) says as well. The earlier comment here - "a pointer to the base's m_batchBuffers vector at
// +0x124" - was wrong: +0x124 is m_batchMaterial.
void CBatchDriver::flush() {
    if (m_batchBuffer) {
        auto* buffer = reinterpret_cast<scene::CAppendMeshBuffer*>(m_batchBuffer.get());
        // The listing tests the CAppendMeshBuffer word at +0x50 (asm 0x5a3a2c), which that class's own header
        // names mappedVertices - NOT +0x44 indexUsedBytes, which is what this body read before. The two differ
        // whenever a buffer has mapped vertices but no indices written yet, and there the listing still unmaps
        // while the old gate skipped the block entirely.
        if (buffer->mappedVertices != nullptr) {
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
        // 0x5a3a90-0x5a3abc: the +0x234 END-OF-BATCH callback (vtable +0x08) with the finished buffer and the
        // base's m_batchMaterial, gated on vertexUsedBytes (+0x3c) being non-zero. R0/R1/R2 of the listing are
        // exactly the callback, the buffer and +0x124, and the slot is the IEndOfBatchCallback one (see the
        // register-level proof above), so both the offset and the interface here are settled.
        if (endOfBatchCallback != nullptr && buffer->vertexUsedBytes != 0) {
            (*endOfBatchCallback)(*buffer, m_batchMaterial);
        }
        buffer->clear();
    }
}

// 0x5a6fe0 _ZN6glitch5video12CBatchDriver15thisAppendBatchERKN5boost13intrusive_ptrIKNS0_14CVertexStreamsEEERKNS0_
// 16CPrimitiveStreamERKNS3_IKNS_5scene11CMeshBufferEEE
//
// NOT WRITTEN, and this is the recovered structure of the whole body so the next pass does not have to read it out
// of the export again (0x1aa0 bytes; Hex-Rays chunk_029.c lines 24009-25229). The Hex-Rays text prints byte offsets
// as DWORD indices of a float* `a1` (`*((_DWORD *)a1 + 143)` is byte +0x23C); its parameters are
// a1 = this, a2 = &streams, a3 = &primitives, a4 = &meshBuffer.
//
// PROLOGUE (0x5a6fe0-0x5a70d4) - recovered completely:
//   1. local state: a vector of records (`v207/v208/v209`), the clustering result
//      std::map<uint32, std::vector<uint32>> (`v202`..`v206`), the vertex-dedup std::map<uint16,uint16>
//      (`v191`..`v195`) and the per-primitive box vector `p` (`v211`).
//   2. `if (this->+0x23C == nullptr) goto CLUSTER;` - 0x5a6fec `LDR R12,[R0,#0x23C]`, 0x5a7034 `BEQ 0x5a83e0`.
//   3. rebind +0x240: add-ref argument 1 (`++*v4`, the count is the pointee's +0x00), store it and release the old
//      value (decrement, then CVertexStreams::~CVertexStreams 0x5a0a1c and free at zero).
//   4. `*((_DWORD *)a1 + 145) = a3;` - +0x244 = the CPrimitiveStream* argument, not owned.
//   5. rebind +0x248: add-ref the mesh buffer (count +0x04), store it, drop the old value through the virtual drop.
//   6. `(**(int(**)(...))(*(_DWORD *)a1 + 0x23C) vtable + 8)(this + 0x23C, 0, 0, (int *)a1 + 144)` and test the
//      result: non-zero enters the CLUSTER block. This is the +0x23C split-segment-choice callback called with
//      (0, 0, &this->+0x240). 0x5a70b4-0x5a70d4.
//
// CLUSTER BLOCK (0x5a70d4-0x5a83e0) - entered when the choice callback is absent or answered non-zero:
//   * gate: `((segmentSize.x || .y || .z) && !segmentSizeMode) || (maxIndices && segmentSizeMode)` (+0x250 floats,
//     +0x260, +0x25C).
//   * `v180 = *(uint16 *)(*a2 + 34)` = the vertex stride; the primitive stream's index buffer is mapped
//     (`IBuffer::map(*a3, 1) + a3[1]`) and its vertex buffer likewise (`*(*a2 + 20)` mapped + `*(*a2 + 24)`).
//   * for every primitive (`CPrimitiveStream::getPrimitiveCount`): the three vertices' floats at
//     `vertexBase + stride * index` are clamped into +-FLT_MAX, accumulated into the run's total box and stored as
//     a 28-byte `pair<uint32, aabbox3d<float>>` in `p`; then both mapped buffers are unbound with the inlined
//     `--mapCount; if (!mapCount && (flags & 0x20)) vtable[0x18]()` sequence.
//   * `maxIndices` mode: `v149 = a3[4] - a3[3]; v150 = v149 * v180 / *((_DWORD *)a1 + 151);
//     if (v150 > 1) kdTreeClustering(&v202, (int *)&p, v149 / v150, (float *)&v196);`
//     else `spatialGridClustering(&v202, (int *)&p, (const vector3d<float> *)(a1 + 148));`
//     The two BL sites inside this function are 0x5a88a8 (kd-tree) and 0x5a89b8 (spatial grid); the FUNCTIONS are
//     kdTreeClustering 0x5a6568 and spatialGridClustering 0x5a5860 (an earlier note listed the call sites as if
//     they were the callees).
//   * the `p` vector is released.
//
// RECORD BUILDING (0x5a77xx-0x5a83e0; the `v206 <= 1` test is the single-cluster shortcut):
//   * with 0 or 1 cluster keys one record is built straight from the arguments ({streams = *a2, the primitives word
//     for word: a3[1]..a3[4], a3[10]/a3[11]}) and pushed on the vector.
//   * otherwise `isVertexStreamHomolaced(a2, &v219)` picks a fast path, and for every cluster key a CVertexStreams
//     is built (a clone, or rebuilt stream by stream from the attribute map through
//     `CVertexStreams::updateHomogeneityInternal` and `detail::SVertexAttributeTypeInspection::ValueTypeSize`) plus
//     one IBuffer from the driver's vtable +0x78 (`(*(...)(*a1 + 120))(&out, a1, 0, 4, size, new[](size), 1)`), the
//     cluster's indices are merged into a CPrimitiveStream, and every primitive's three vertices are looked up in
//     the std::map<uint16,uint16> dedup table and copied once with
//     `copyVertex(index, a2, &streams, base, &dest, homogeneous)`.
//   * each finished record is pushed on the vector; the dedup map is erased and the temporary handles dropped.
//
// EMIT LOOP (0x5a83e0-0x5a8a70, `LABEL_117` in Hex-Rays):
//   * `CAppendMeshBuffer::beginAppend(this->m_batchBuffer)` (`*((_DWORD *)a1 + 72)`, the BASE's +0x120).
//   * per record, skipping while `(uint16)record[13] <= 3 || record[12] != 1`:
//     - `CAppendMeshBuffer::hasEnoughSpace(buffer, record[5] - record[4], 3 * getPrimitiveCount)`, and when it
//       answers false the original prints 28 identical `puts("@@@...")` lines and calls
//       `(*(vtable + 508))(this)` = vtable +0x1fc = CBatchDriver::flush (0x5a7cb8);
//     - the attribute map is the one reached through this +0xE8, or the static `IVideoDriver::DefaultAttribMap`
//       rebuilt by `MEMSET(..., 255, 0x1E)` plus `DefaultAttribMap[entry.attribute] = index >> 4` per stream entry;
//     - the record is handed to the batch baker's virtual at vtable +0x0C: `v80 = *((_DWORD *)a1 + 74)` = the base's
//       +0x128 m_batchBaker, and the call takes 18 arguments (`v80, record, record + 2, +0x12c, the +0x134 byte,
//       the attribute map, buffer + 20, buffer + 24, +0x124 material, ... , map counts, 0, primitives, -1`);
//     - the append buffer's counters (words 5, 8, 9, 10, 15, 17, 18, 19) are advanced from the record's index
//       counts, reproducing
//       `word15 = max(word15, word18 * ((uint16)(record[10] - record[8]) + word15 / word18))` and the matching
//       index-side maximum;
//     - `if (this->+0x238) { rebind +0x240 / +0x244 / +0x248 from the record and call the +0x238 SEGMENT-COMPILE
//       callback through vtable +0x08 with (0, 0, &this->+0x240); }`.
//   * the record vector and the cluster map are destroyed; the function returns 0.
//
// MISSING, named exactly - each of these is its own reconstruction, and none of them is this class:
//   1. glitch::video::spatialGridClustering 0x5a5860 - ledger `skipped`: the map key is a hash of an ostringstream
//      holding the three cell indices; the hashed string range and whether the stream is reset are unidentified.
//   2. glitch::video::kdTreeClustering 0x5a6568 - ledger `skipped`: needs CKdTree<pair<uint32, aabbox3d<float>>>
//      (shared/include/glitch-shared/kd_tree.hpp and glitch-core-io's core/kd_tree.hpp) in glitch-video.
//   3. glitch::video::isVertexStreamHomolaced 0x5a23f4 - `partial`, body not written.
//   4. glitch::video::copyVertex 0x5a2b18 - `partial`: the whole-vertex branch is not written.
//   5. the +0x23C and +0x238 call operators: neither interface declares one in the port
//      (scene_simple_accessors.hpp gives them only destructors), so the argument lists have to come out of the
//      callback classes' own listings (SDefaultSegmentCompileCallback 0x5891a8, SDefaultEndOfBatchCallback 0x59013c
//      / 0x58d3d4) before either call can be written.
//   6. IVideoDriver::DefaultAttribMap - a static table with no port definition.
//   7. the base vtable slots the body dispatches: +0x0C and +0x14 of m_batchBaker (IBatchBaker), +0x78 (the
//      CVertexStreams allocator) and +0x1FC (flush, already written).
//   8. aabbox3d<float> / vector3d<float> in glitch-video's own deps (the clustering signatures).
// (1)..(4) are the calls that produce and fill the records, so no honest sub-slice of the body exists: without them
// the emit loop would only append whatever it was handed. The row stays `partial` with this note.
//
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
