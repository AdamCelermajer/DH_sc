// glitch::video::CBatchDriver: the second driver a CSceneManager owns. CSceneManager (+0x1c) allocates it
// (operator new(0x264)) over the video driver's screen size and uses it to compile the static level geometry into
// CBatchMesh batches; Level::_LoadBatching (0x3f2ae4) then writes the two append-buffer capacities into it.
//
// Layout of the original object (libDungeonHunter2.so, ARM, 32-bit). Every offset below was read from a listing; the
// member offsets were decoded from the ARM load/store immediates themselves (the Hex-Rays text is not reliable here:
// for this class it prints +0x248 as "this + 72" and, in the other direction, a 5-instruction `LDRSH r0,[r2,#0x22]`
// as `LDRSH R12,[R7,#0x22]` in a function whose r7 is something else). The decode covered the constructors, the
// destructor, initBuffer, releaseBuffer, flush, thisAppendBatch and draw.
//
//   Base: glitch::video::CNullDriver, 0x22C bytes (its own base IVideoDriver is 0x160, then three CMatrix4<float> of
//   68 bytes). Evidence: CBatchDriver::CBatchDriver 0x5a2e20 calls CNullDriver::CNullDriver(this, driver) (0x5b9764)
//   as its first statement, and CNullDriver::CNullDriver fills exactly +0x160 .. +0x22B (its two loop bounds are
//   `this + 352` and `this + 556`).
//   Total size 0x264: the CSceneManager constructor 0x58d908 does `MOV R0,#0x264; BL operator new` before calling
//   0x5a2e20 (asm 0x58d994-0x58d9bc).
//
// Derived members (all offsets decoded from the immediates):
//   +0x22C u32 defaultVertexBufferCapacityBytes   ctor 0x5a2e6c stores argument 1; initBuffer 0x58a950 reads it
//   +0x230 u32 defaultIndexBufferCapacityBytes    ctor 0x5a2e70 stores argument 2; initBuffer 0x58a954 reads it
//           Both arguments come from CSceneManager::CSceneManager, which passes the two words of the block at
//           IVideoDriver +0xd4 (= IDevice* m_device; IDevice::creationParameters is at +0x5c), so they are
//           device->creationParameters.bytes + 0x40 and + 0x44 (asm 0x58d994-0x58d9a0). Level::_LoadBatching writes
//           (unsigned)((double)size * 1.02) into them at 0x3f2c0c (vertex) and 0x3f2cd4 (index).
//   +0x234 ISegmentCompileCallback*  ctor clears it (0x5a2e74); CSceneManager::compileInternal stores the
//           SDefaultSegmentCompileCallback that lives on its stack there (`STR SP, [R0,#0x234]`, asm 0x58fb64, with
//           R0 = manager +0x1c); flush reads it into r3 at 0x5a3a90 and calls its vtable +0x08 through the other word.
//           NOTE: the call at 0x5a3a9c is gated on the +0x238 word, not on this one, and it is the +0x238 word that
//           is passed; see the member comment for +0x234 below for what is proven and what is not.
//   +0x238 IEndOfBatchCallback*      ctor clears it (0x5a2e78); compileInternal stores the SDefaultEndOfBatchCallback
//           on its stack there (asm 0x58fb70), and that same word reaches the +0x24c dereference below.
//   +0x23C intrusive_ptr<CAppendMeshBuffer> (raw in the original)  ctor clears it (0x5a2e7c); initBuffer allocates
//           operator new(0x70), runs CAppendMeshBuffer(vertexCapacity, indexCapacity, this, 1, -1) (0x6b8cb4) and
//           keeps the pointer (0x58aa54-0x58aaa0); thisAppendBatch loads it into r12 at 0x5a6fec and does nothing at
//           all while it is null; releaseBuffer and flush clear it after use.
//   +0x240 intrusive_ptr<CVertexStreams>  ctor clears it (0x5a2e80); thisAppendBatch assigns the new streams at
//           0x5a7050-0x5a7054 and drops the old one at 0x5a7068; the destructor drops it at 0x5a3d80.
//   +0x244 uint16_t* sortedBatches   NOT WRITTEN BY THE CONSTRUCTOR (it is the one gap in the cleared run
//           0x234..0x248) and no listing read assigns it. PROVEN: CBatchSceneNode::registerSolidBatches 0x57e800
//           loads this word (`LDR r3,[r7,#0x130]`-style: `LDR R3,[R7,#0x244]` in that function) and walks it as a
//           uint16 list starting at element 1, taking element i as `batch index + 1`. NOT PROVEN: who fills it and
//           whether the elements are uint16 (a uint32 array fits the same reads). See the member comment.
//   +0x248 IReferenceCounted* (one word)  ctor clears it (0x5a2e84); the destructor drops it (0x5a3d64 loads it into
//           r0, 0x5a3d7c calls drop). This is the most likely home of the end-of-batch callback, but the evidence is
//           only "a reference-counted pointer that is dropped"; see the member comment.
//   +0x24C NOT RECONSTRUCTED. No listing read touches it and the constructor does not write it, yet the object is
//           0x264 bytes and the members below are pinned by their own reads, so it is a real member of unknown role.
//   +0x250 vector3d<float> segmentSize          ctor clears the three words at 0x5a2e60 / 0x5a2e64 / 0x5a2e54
//   +0x25C bool segmentSizeMode   ctor clears the byte at 0x5a2e88 (a full STR, the zeroing idiom); thisAppendBatch
//           loads it into r1 at 0x5a8874 and branches on it - non-zero selects the kdTreeClustering path, zero the
//           spatialGridClustering path (asm 0x5a8858-0x5a88a8).
//   +0x260 u32 maxIndices   thisAppendBatch divides by it when the mode byte is set:
//           `v149 * v180 / *((_DWORD *)a1 + 151)` at 0x5a8880. The CSceneManager compile entry points write it.
#pragma once

#include <cstdint>

#include "glitch-video/deps/intrusive_ptr.hpp"
#include "glitch-video/null_driver.hpp"
#include "glitch-video/primitive_stream.hpp"
#include "glitch-video/video_types.hpp"

namespace glitch {
class IReferenceCounted;
}  // namespace glitch

namespace glitch::video {

class IVideoDriver;

}  // namespace glitch::video

namespace glitch::scene {
class CAppendMeshBuffer;  // the type of +0x23C
class CBatchSceneNode;    // the node the compile call comes from
// IEndOfBatchCallback, ISegmentCompileCallback and ISplitSegmentChoiceCallback are declared in
// glitch-scene-collada-gui headers (CAppendMeshBuffer.hpp and scene_types.hpp), which this header must not include -
// the scene classes include glitch-video, not the other way round. They are only used through pointers below, so
// they are re-declared here (a repeated class declaration is legal).
class IEndOfBatchCallback;
class ISegmentCompileCallback;
class ISplitSegmentChoiceCallback;
}  // namespace glitch::scene

namespace glitch::video {

// The batch driver of CSceneManager. It derives from CNullDriver: most of its work is the append path (thisAppendBatch
// + flush) that turns draw calls into the growable CAppendMeshBuffer, and the compile call the CBatchSceneNode issues
// for every batch of a CBatchMesh.
class CBatchDriver : public CNullDriver {
public:
    // 0x5a2e20 (_ZN6glitch5video12CBatchDriverC1EjjPNS0_12IVideoDriverE) and 0x5a2ed8 (C2, same body).
    // Reconstructed in glitch-video/src/batch_driver.cpp: the base constructor runs with the owning driver, the
    // +0x22C / +0x230 capacities and the cleared members above are stored, IVideoDriver::setOption(0x100, true)
    // (0x5a90b0) is called on this object, and the 25 low capability bits (IVideoDriver +0x9c) of the owning driver
    // are copied into this object's own capability word.
    CBatchDriver(uint32_t vertexBufferCapacityBytes, uint32_t indexBufferCapacityBytes, IVideoDriver* driver);
    // 0x5a3d50 (D1); 0x5a3dd8 is the compiler's deleting thunk (D0). Reconstructed: the vtable is reset, +0x248 is
    // dropped, +0x240 is released, then the CNullDriver base is destroyed.
    ~CBatchDriver() override;

    // 0x58a920: when +0x23C is null, allocates the CAppendMeshBuffer over the two capacities and stores it there.
    // The body needs glitch::scene::CAppendMeshBuffer, which batch_driver.cpp may include but this header must not,
    // so it is not written yet. Ledger row stays partial.
    void initBuffer();
    // 0x589f40: clears the batch list, drops +0x240 and calls IVideoDriver::clearImplementationDependentData
    // (0x5adfe0). Not written yet (same scene type).
    void releaseBuffer();
    // 0x5a6fe0 (0x1aa0 bytes, 1221 decompiled lines): the append path - rebinds +0x23C / +0x240 / +0x244 to the new
    // streams and primitive stream, clusters the primitives (spatialGridClustering 0x5a89b8 or kdTreeClustering
    // 0x5a88a8 according to +0x25C) and copies or deduplicates the vertices into the append buffer. Not written:
    // it needs CAppendMeshBuffer, isVertexStreamHomolaced and copyVertex. Ledger row stays partial.
    void thisAppendBatch(const boost::intrusive_ptr<const CVertexStreams>& streams, const CPrimitiveStream& primitives,
                         const boost::intrusive_ptr<const scene::CMeshBuffer>& meshBuffer);
    // 0x5a3a20: closes an open append on +0x23C, hands the finished buffer to the +0x238 callback (vtable +0x08)
    // together with the sorted batch list, then clears the append state. Not written yet.
    void flush();

    // ---- vtable slots this class overrides -------------------------------------------------------------------
    // 0x5a8a80, slot +0x58 of the vtable at 0x977108: a two-instruction thunk (`LDR R3,[SP,#arg_0]; B 0x5a6fe0`) into
    // thisAppendBatch, i.e. IVideoDriver::draw with an extra mesh-buffer argument. Declared, not written.
    void draw(const boost::intrusive_ptr<const CVertexStreams>& streams, const CPrimitiveStream& primitives,
              CDriverBinding** binding, const boost::intrusive_ptr<const scene::CMeshBuffer>& meshBuffer) override;

    // The compile entry point registerSolidBatches 0x57e800 and registerTransparentBatches 0x57ea70 call:
    //   (node, *mesh->getMaterial(batch - 1), batch, 4, nullptr, 0x7fffffff)                       [asm 0x57e8b8-0x57e8e4]
    //   (node, material, batch | ((segment->field34 + 1) << 16), 8, segmentCentre, segment+0x22 word)
    //                                                                                              [asm 0x57eb64-0x57eb84]
    // The listings reach it through a vtable slot the notes call +0x24. This header deliberately declares it
    // NON-virtual: slot +0x24 of the real vtable (0x977108) is an inherited IVideoDriver slot - IDA reads the word at
    // 0x97712c as IVideoDriver::draw3DTriangle - and inserting a slot would renumber every later slot of every
    // driver class in the tree. The parameter list below is read off those two call sites; it is not proven by a
    // mangled symbol, and the last argument is left as a void pointer because the compile-info record the scene code
    // passes has no header of its own in the port. The body is not written.
    void compile(scene::CBatchSceneNode* node, const boost::intrusive_ptr<CMaterial>& material, uint32_t batchWord,
                 uint32_t flags, const core::vector3d<float>* segmentCentre, uint32_t maxValue, const void* compileInfo);

    // ---- members (original 32-bit offsets) -------------------------------------------------------------------
    uint32_t defaultVertexBufferCapacityBytes = 0;  // +0x22C
    uint32_t defaultIndexBufferCapacityBytes = 0;   // +0x230

    scene::ISegmentCompileCallback* segmentCompileCallback = nullptr;  // +0x234
    // +0x238. Everything proven about this word and about +0x234 is: the constructor clears both, compileInternal
    // stores the stack SDefaultSegmentCompileCallback pointer at +0x234 and the stack SDefaultEndOfBatchCallback
    // pointer at +0x238, and flush uses the +0x238 word (with the +0x23C buffer) to publish the finished batch. The
    // exact pairing of interface to offset is therefore the best reading of the two stores, not a proven fact.
    scene::IEndOfBatchCallback* endOfBatchCallback = nullptr;  // +0x238

    // +0x23C. The original holds a raw CAppendMeshBuffer* (initBuffer allocates 0x70 bytes and calls
    // CAppendMeshBuffer(capacity, capacity, this, 1, -1)); CAppendMeshBuffer belongs to glitch-scene-collada-gui,
    // which this header must not include, so the member is typed as the reference-counted interface the port can
    // hold instead. Retype it in a later batch_driver.cpp that includes that header - the offset and the lifetime
    // are what matter.
    boost::intrusive_ptr<glitch::IReferenceCounted> appendBuffer;  // +0x23C

    // +0x240: the vertex streams of the batch being appended (ctor clears it, thisAppendBatch rebinds it, the
    // destructor drops it). CVertexStreams is non-const in this position: thisAppendBatch assigns through the
    // intrusive_ptr<const CVertexStreams> its signature takes, which the port's intrusive_ptr permits, but the
    // member itself is the object whose count the destructor decrements.
    boost::intrusive_ptr<CVertexStreams> currentAppendVertices;  // +0x240

    // +0x244: the batch order CBatchSceneNode::registerSolidBatches reads, as a uint16 list walked from element 1
    // (element i is `batch index + 1`). NOT PROVEN: who fills it (the constructor does not) and the element width.
    // Declared as the raw uint16 pointer the reader treats it as; retype it if a filling listing turns up.
    uint16_t* sortedBatches = nullptr;  // +0x244

    // +0x248: one reference-counted pointer the destructor drops (asm 0x5a3d64 LDR, 0x5a3d7c drop). The constructor
    // clears it. It is probably the end-of-batch callback of flush, but that is not proven; declared as the
    // IReferenceCounted pointer the drop requires.
    boost::intrusive_ptr<glitch::IReferenceCounted> unknown248;  // +0x248

    // +0x24C: nothing is known about this word (see the file comment). Kept so the declaration accounts for the
    // 0x264-byte object and so the offsets in the comments above stay meaningful.
    uint32_t unknown24c = 0;  // +0x24C

    // +0x250, +0x25C and +0x260 (evidence in the file comment).
    core::vector3d<float> segmentSize;  // +0x250, cleared by the constructor
    bool segmentSizeMode = false;       // +0x25C, byte; non-zero selects the kd-tree clustering path
    uint32_t maxIndices = 0;            // +0x260
};

}  // namespace glitch::video
