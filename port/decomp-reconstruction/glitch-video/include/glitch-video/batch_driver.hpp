// glitch::video::CBatchDriver: the second driver a CSceneManager owns. CSceneManager (+0x1c) allocates it
// (operator new(0x264)) over the video driver's screen size and uses it to compile the static level geometry into
// CBatchMesh batches; Level::_LoadBatching (0x3f2ae4) then writes the two append-buffer capacities into it.
//
// Layout of the original object (libDungeonHunter2.so, ARM, 32-bit). Every offset below was decoded from the ARM
// load/store immediates; the Hex-Rays text for this class is not usable on its own, because for this class it prints a
// byte offset as a DWORD member index (so byte +0x240 comes out as "this + 144" and hex 0x120 as the decimal "this +
// 72"), and that mislabelling is what produced the earlier, wrong offset table here.
//
//   Base: glitch::video::CNullDriver, 0x22C bytes (its own base IVideoDriver is 0x160, then three CMatrix4<float> of
//   68 bytes). Evidence: CBatchDriver::CBatchDriver 0x5a2e20 calls CNullDriver::CNullDriver(this, driver) (0x5b9764)
//   as its first statement, and CNullDriver::CNullDriver fills exactly +0x160 .. +0x22B: the FALL-THROUGH of its
//   three-iteration matrix loop, `STRB R8,[R6,#0x40]; MEMSET(R6,0,0x40); ...; STRB R5,[R6,#0x40]; ADD R6,R6,#0x44;
//   CMP R6,R10; BNE` (0x5b97ec-0x5b980c, R6 = this + 0x160 after the `STR R3,[R6],#0x160` at 0x5b97e4), writes
//   R6+0x00 .. R6+0x43 for R6 = this + 0x160, +0x1A4, +0x1E8 and leaves the cursor at this + 0x22C. So the base is
//   +0x000 .. +0x22B and the derived object's own storage starts at +0x22C, with a 0x10-byte alignment hole between
//   the base's last byte and this class's second four-word run at +0x23C.
//   Total size 0x264 (original): the CSceneManager constructor 0x58d908 does `MOV R0,#0x264; BL operator new` before
//   calling 0x5a2e20 (asm 0x58d994-0x58d9bc). The four words are +0x23C (callback), +0x240, +0x244, +0x248, then
//   segmentSize +0x250 (three words), segmentSizeMode +0x25C (one byte) and maxIndices +0x260 (one word), so the
//   declared range ends at +0x264 and +0x24C is the one word left unclaimed.
//   HOST SIZE: the port's class is NOT 0x264 bytes and cannot be - CNullDriver is already 0x2F8 on this 64-bit host
//   (its IVideoDriver subobject alone is 0x228 against the original's 0x160), and a boost::intrusive_ptr is 8 bytes
//   here against the original's 4. The declarations below are therefore ordered for the ORIGINAL layout and named by
//   the ORIGINAL offsets, and the port does not attempt to reproduce them: as before this change the derived members
//   start at host offset 0x2F4 (after CNullDriver's 0x2F8, with the same kind of 4-byte hole), and declaring the four
//   words moves them +0x20. No correct 64-bit translation of the 32-bit offsets exists, and nothing in the tree reads
//   a CBatchDriver member by hard-coded byte offset (the one reader that had to consume such offsets,
//   CSceneManager::compileInternal, is unwritten and keeps typed member access), so only the ORDER and the TYPES are
//   reproduced here. The 0x264-byte claim belongs to the original and is kept only as the historical note above.
//
// The APPEND-BUFFER state is NOT declared on this class: it lives in the driver object, and video_driver.hpp already
// declares and types all of it (see the +0x10c .. +0x140 rows of that header): +0x10c m_maxBatchIndices, +0x110
// m_batchBuffers (std::vector<intrusive_ptr<IReferenceCounted>>, the CAppendMeshBuffers built for this driver),
// +0x120 m_batchBuffer (the current one), +0x124 m_batchMaterial, +0x128 m_batchBaker, +0x12c / +0x130
// m_batchMaterialHeap / m_batchSourceMaterial and +0x134 m_batchTechnique. Decoding initBuffer 0x58a920 settles that
// that buffer is not a member of CBatchDriver: initBuffer is a member function whose `this` is R0/R4 - the same
// register its two capacities at +0x22C / +0x230 come from, which the constructor's own stores prove is the complete
// object - and it reads and writes +0x110 / +0x114 / +0x118 / +0x120 only, i.e. inside the base. flush 0x5a3a20
// agrees: it reads a member of this class (+0x234, the callback) and +0x120 (the current batch buffer) from the same
// `this`. So the base stores the buffer as an opaque reference-counted handle and `drop()` is virtual, which is why
// the concrete CAppendMeshBuffer needs no declaration here and dies correctly.
//
// The +0x23C .. +0x248 run IS this class's own state, a SECOND and different batch state from the base's
// +0x110 / +0x120 / +0x124 / +0x128 (settled by the coverage batch; the earlier "these are the base's, read through
// a biased `this`" reading was wrong, and so was the one before it that declared them at the wrong offsets):
//   * thisAppendBatch 0x5a6fe0 reads +0x23C, +0x240, +0x244 and +0x248 from its own `this` - the register that also
//     never reaches the base's +0x110 run - while initBuffer and flush read the base's +0x120 from the same class;
//   * the constructor clears +0x23C / +0x240 / +0x248 in the same instruction run that stores +0x22C / +0x230
//     (0x5a2e7c / 0x5a2e80 / 0x5a2e84; it writes +0x244 not at all);
//   * the destructor releases +0x248 through the virtual drop (0x5a3d64 / 0x5a3d7c) and +0x240 inline with
//     CVertexStreams::~CVertexStreams (0x5a0a1c) plus free (0x5a3d80-0x5a3dbc);
//   * CSceneManager::compileInternal 0x58facc writes +0x23C from its fourth argument (0x58fb7c).
// There is no second base and no bias: the offsets are literal. The concrete types are declared below.
//
// MEMBERS OF THIS CLASS (only these groups belong to it):
//   +0x22C u32 defaultVertexBufferCapacityBytes   ctor 0x5a2e6c stores argument 1; initBuffer 0x58a950 reads it
//   +0x230 u32 defaultIndexBufferCapacityBytes    ctor 0x5a2e70 stores argument 2; initBuffer 0x58a954 reads it
//           Both arguments come from CSceneManager::CSceneManager, which passes the two words of the block at
//           IVideoDriver +0xd4 (= IDevice* m_device; IDevice::creationParameters is at +0x5c), so they are
//           device->creationParameters.bytes + 0x40 and + 0x44 (asm 0x58d994-0x58d9a0). Level::_LoadBatching writes
//           (unsigned)((double)size * 1.02) into them at 0x3f2c0c (vertex) and 0x3f2cd4 (index).
//   +0x234 scene::ISegmentCompileCallback*  the callback flush invokes through vtable +0x08 (0x5a3a90-0x5a3abc).
//           CSceneManager::compileInternal 0x58facc stores its own stack SDefaultSegmentCompileCallback object there
//           (`STR SP,[R0,#0x234]`, asm 0x58fb64 - the same SP it later passes to SDefaultEndOfBatchCallback::finalize
//           at 0x58fc00, so the object at SP is this slot's value). The constructor clears it (0x5a2e74).
//   +0x238 scene::IEndOfBatchCallback*  compileInternal stores the stack SDefaultEndOfBatchCallback pointer at
//           `[driver + 0x238]` (asm 0x58fb70, R2 = SP + 0x18, the object that finalize() is called on). The
//           constructor clears it (0x5a2e78). No listing in this class reads the word back, so only the store is
//           proven; the type is the best reading of that store.
//           CORRECTION (this batch): these two slots were NAMED THE OTHER WAY ROUND before (the +0x234 word was
//           called the end-of-batch callback). The listing says +0x234 = the SDefaultSegmentCompileCallback object
//           at SP and +0x238 = the SDefaultEndOfBatchCallback pointer built at SP+0x18. flush already calls +0x234
//           as the live one.
//   +0x23C scene::ISplitSegmentChoiceCallback*  thisAppendBatch null-tests it and returns at once when it is null
//           (0x5a6fec), then calls it through vtable +0x08 with (streams, &this->m_currentVertices) and, when it
//           returns non-null, jumps to the early-exit block 0x5a83e0 (0x5a70bc-0x5a70d4: R0 = the callback,
//           R3 = this + 0x240). compileInternal stores its fourth argument here (0x58fb7c). ctor clears it.
//   +0x240 intrusive_ptr<const CVertexStreams>  the streams thisAppendBatch is appending, and the argument it hands
//           the choice callback as `&this->m_currentVertices` (0x5a70c8). Rebound twice (0x5a7050-0x5a7068,
//           0x5a7cb8-0x5a7ce8) and released inline by the destructor (0x5a3d80-0x5a3dbc: decrement the count word at
//           the pointee's +0x00, CVertexStreams::~CVertexStreams 0x5a0a1c at zero, then free). ctor clears it.
//   +0x244 const CPrimitiveStream*  non-owning, and the constructor does NOT write it. thisAppendBatch stores its
//           second parameter here on the entry path (`STR R0,[R1,#0x244]` with R0 = the spilled argument 2, asm
//           0x5a707c) and stores a decimation-helper pointer here on the other path (asm 0x5a7ec4 region). It is a
//           raw pointer, not a counted one.
//   +0x248 intrusive_ptr<const scene::CMeshBuffer>  the batch buffers being compiled. thisAppendBatch add-refs the
//           pointee's +0x04 count, stores it here and drops the previous value (0x5a7098-0x5a70ac, 0x5a7cxx); the
//           destructor loads it and calls the virtual drop (0x5a3d64 / 0x5a3d7c). ctor clears it.
//   +0x24C u32  the only word of the class the object's 0x264 bytes leave unaccounted for (see the arithmetic
//           below): no listing read in this batch names it. Not declared, so the port does not invent a field.
//   +0x250 vector3d<float> segmentSize          ctor clears the three words at 0x5a2e60 / 0x5a2e64 / 0x5a2e54
//   +0x25C bool segmentSizeMode   ctor clears the byte at 0x5a2e88 (a full STR, the zeroing idiom); thisAppendBatch
//           loads it into r1 at 0x5a8874 and branches on it - non-zero selects the kdTreeClustering path, zero the
//           spatialGridClustering path (asm 0x5a8858-0x5a88a8).
//   +0x260 u32 maxIndices   the constructor does NOT write it; thisAppendBatch reads it as a byte at 0x5a83fc
//           (`LDRB R3,[R12,#0x260]`) while the mode byte is set, and divides by the word `v149 * v180 /
//           *((_DWORD *)a1 + 152)` at 0x5a8880. The CSceneManager compile entry points write it.
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
class CBatchSceneNode;  // the node the compile call comes from
// IEndOfBatchCallback, ISegmentCompileCallback and ISplitSegmentChoiceCallback are declared in
// glitch-scene-collada-gui headers (CAppendMeshBuffer.hpp and scene_simple_accessors.hpp), which this header must not
// include - the scene classes include glitch-video, not the other way round. They are only used through pointers
// below, so they are re-declared here (a repeated class declaration is legal). The append buffer itself needs no
// declaration at all: the base IVideoDriver holds it as an intrusive_ptr<glitch::IReferenceCounted>.
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
    // 0x5a3d50 (D1); 0x5a3dd8 is the compiler's deleting thunk (D0). Reconstructed: the vtable is reset, the word at
    // +0x248 is dropped, the count of the object at +0x240 is released inline (and CVertexStreams::~CVertexStreams
    // runs at zero), then the CNullDriver base is destroyed. Both words ARE declared here now (see the members
    // below), so the compiler's own reverse-order member teardown performs exactly those two drops, in that order
    // (+0x248 before +0x240), ahead of the base's destructor.
    ~CBatchDriver() override;

    // 0x58a920 _ZN6glitch5video12CBatchDriver10initBufferEv
    // Reconstructed in glitch-video/src/batch_driver.cpp: when the base's +0x120 (m_batchBuffer) is null, allocates
    // operator new(0x70), runs CAppendMeshBuffer(+0x22C, +0x230, this, 1, static_cast<uint32_t>(-1)) (0x6b8cb4),
    // pushes it on the base's +0x110 vector and stores it at +0x120. Needs no glitch-scene type: the handle is
    // opaque.
    void initBuffer();
    // 0x589f40 _ZN6glitch5video12CBatchDriver13releaseBufferEv
    // Reconstructed in glitch-video/src/batch_driver.cpp: erases the base's +0x110 vector when +0x120 is set, clears
    // +0x120, drops the base's +0x128 and calls IVideoDriver::clearImplementationDependentData (0x5adfe0, itself a
    // skipped row, so this adds an undefined symbol the integrator already tracks).
    void releaseBuffer();
    // 0x5a6fe0 (0x1aa0 bytes, 1221 decompiled lines): the append path - rebinds THIS class's own batch state at
    // +0x23C / +0x240 / +0x244 / +0x248 (the base's +0x110 / +0x120 / +0x124 / +0x128 is a different state and is
    // only read indirectly), clusters the primitives (spatialGridClustering 0x5a89b8 or kdTreeClustering 0x5a88a8
    // according to +0x25C) and copies or deduplicates the vertices into the base's current append buffer. NOT
    // WRITTEN: it needs one pass of its own, and glitch::video::isVertexStreamHomolaced (0x5a23f4) and copyVertex
    // (0x5a2b18) are still partial. The CAppendMeshBuffer dependency that was recorded as its blocker does not exist
    // (the buffer is an opaque handle).
    void thisAppendBatch(const boost::intrusive_ptr<const CVertexStreams>& streams, const CPrimitiveStream& primitives,
                         const boost::intrusive_ptr<const scene::CMeshBuffer>& meshBuffer);
    // 0x5a3a20 _ZN6glitch5video12CBatchDriver5flushEv (vtable 0x977108 slot +0x1fc, the IVideoDriver::flush slot)
    // Reconstructed in glitch-video/src/batch_driver.cpp from the port's ISegmentCompileCallback declaration: the
    // listing's unmap half, the +0x234 callback call (0x5a3a90-0x5a3abc, vtable +0x08, gated on the append
    // buffer's +0x3c vertexUsedBytes) and the trailing clear() are all written. The callback slot is +0x234, the
    // ISegmentCompileCallback one (see the member table); the earlier comment here (and in that source) called the
    // +0x234 slot the end-of-batch callback and named the base's batch-buffer vector as the second argument, which
    // the listing does not support. Declared with `override` because it is the base's slot.
    void flush() override;

    // ---- vtable slots this class overrides -------------------------------------------------------------------
    // 0x5a8a80, slot +0x58 of the vtable at 0x977108: a two-instruction thunk (`LDR R3,[SP,#arg_0]; B 0x5a6fe0`) into
    // thisAppendBatch, i.e. IVideoDriver::draw with an extra mesh-buffer argument. Declared, not written. `flush` above
    // is the other overridden slot.
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

    // ---- members (original 32-bit offsets; the base IVideoDriver is 0x160 bytes, CNullDriver ends at +0x22C) ------
    uint32_t defaultVertexBufferCapacityBytes = 0;  // +0x22C
    uint32_t defaultIndexBufferCapacityBytes = 0;   // +0x230

    // +0x234. The ORIGINAL slot contents, in order: the ISegmentCompileCallback at +0x234, the IEndOfBatchCallback
    // at +0x238 (the PORT had these two names the other way round; see the correction in the member table above).
    // compileInternal 0x58facc stores its stack SDefaultSegmentCompileCallback object at +0x234 (`STR SP,[R0,#0x234]`,
    // asm 0x58fb64) and its stack SDefaultEndOfBatchCallback object at the next word (asm 0x58fb70, R2 = SP + 0x18);
    // flush 0x5a3a20 calls the +0x234 word through vtable +0x08. The constructor clears both.
    scene::ISegmentCompileCallback* segmentCompileCallback = nullptr;  // +0x234
    scene::IEndOfBatchCallback* endOfBatchCallback = nullptr;          // +0x238 (vptr off_9762A0 at the store site)

    // +0x23C .. +0x248: this class's OWN batch state, a different state from the base's +0x110 / +0x120 / +0x124 /
    // +0x128 (the evidence is in the file comment and in the member table). Recovered types, in original order:
    //
    //   +0x23C  the split-segment choice callback. thisAppendBatch returns at once while it is null (0x5a6fec) and
    //           otherwise calls it through vtable +0x08 as `(*cb)(streams, &m_currentVertices)` - R0 = the callback,
    //           R1 = the streams argument, R2 = 0, R3 = this + 0x240 (0x5a70b0-0x5a70d4). compileInternal stores its
    //           fourth argument here (0x58fb7c). The constructor clears it (0x5a2e7c).
    //   +0x240  the vertex streams being appended, handed to that callback by address. The constructor clears it
    //           (0x5a2e80) and the destructor releases it inline through CVertexStreams::~CVertexStreams (0x5a3d80-
    //           0x5a3dbc, i.e. the count word at the pointee's +0x00 and a free), so it is a counted handle and the
    //           pointee type is proven. thisAppendBatch rebinds it (0x5a7050-0x5a7068, 0x5a7cb8-0x5a7ce8).
    //   +0x244  a raw, NON-owning pointer; the constructor does not write it at all (the run 0x5a2e7c-0x5a2e84 clears
    //           +0x23C, +0x240 and +0x248 and skips this word). thisAppendBatch stores its own primitives parameter
    //           there on the entry path (`STR R0,[R1,#0x244]`, R0 = the spilled argument 2, asm 0x5a707c) and a
    //           computed pointer on the other path (asm 0x5a7ec4); a stream VIEW is also taken by address
    //           (`ADD R0, this, #0x240` at 0x5a773c). Declared with no initialiser, exactly as the listing leaves it.
    //   +0x248  the batch buffers being compiled. thisAppendBatch add-refs the pointee's +0x04 count, stores it here
    //           and drops the previous value (0x5a7098-0x5a70ac, 0x5a7cxx); the destructor loads it and calls the
    //           virtual drop (0x5a3d64 / 0x5a3d7c), so it is a counted handle.
    //   +0x24C  no listing read in this batch names it, so no field is invented for it. (In the original the four
    //           words above end at +0x24B, making +0x24C the one word of the 0x264-byte object left unclaimed.)
    scene::ISplitSegmentChoiceCallback* m_choiceCallback = nullptr;             // +0x23C
    boost::intrusive_ptr<const CVertexStreams> m_currentVertices;               // +0x240
    const CPrimitiveStream* m_primitives;                                       // +0x244 (deliberately uninitialised)
    boost::intrusive_ptr<const scene::CMeshBuffer> m_meshBuffer;                // +0x248

    // +0x250, +0x25C and +0x260 (evidence in the file comment).
    core::vector3d<float> segmentSize;  // +0x250, cleared by the constructor
    bool segmentSizeMode = false;       // +0x25C, byte; non-zero selects the kd-tree clustering path
    uint32_t maxIndices = 0;            // +0x260
};

}  // namespace glitch::video
