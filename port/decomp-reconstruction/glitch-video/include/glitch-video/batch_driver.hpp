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
//   as its first statement, and CNullDriver::CNullDriver fills exactly +0x160 .. +0x22B (its two loop bounds are
//   `this + 352` and `this + 556`).
//   Total size 0x264: the CSceneManager constructor 0x58d908 does `MOV R0,#0x264; BL operator new` before calling
//   0x5a2e20 (asm 0x58d994-0x58d9bc), and the last member declared below ends at exactly +0x264.
//
// The append/batch state is NOT declared on this class: it lives in the driver object, and video_driver.hpp already
// declares and types all of it (see the +0x10c .. +0x140 rows of that header): +0x10c m_maxBatchIndices, +0x110
// m_batchBuffers (std::vector<intrusive_ptr<IReferenceCounted>>, the CAppendMeshBuffers built for this driver),
// +0x120 m_batchBuffer (the current one), +0x124 m_batchMaterial, +0x128 m_batchBaker, +0x12c / +0x130
// m_batchMaterialHeap / m_batchSourceMaterial and +0x134 m_batchTechnique. Decoding initBuffer 0x58a920 settles that
// it is not a member of CBatchDriver: initBuffer is a member function whose `this` is R0/R4 - the same register its
// two capacities at +0x22C / +0x230 come from, which the constructor's own stores prove is the complete object - and
// it reads and writes +0x110 / +0x114 / +0x118 / +0x120 only, i.e. inside the base. flush 0x5a3a20 agrees: it reads
// +0x234 (a member of this class) and +0x120 (the current batch buffer) from the same `this`. So the base stores the
// buffer as an opaque reference-counted handle and `drop()` is virtual, which is why the concrete CAppendMeshBuffer
// needs no declaration here and dies correctly. Open, and recorded rather than guessed: thisAppendBatch 0x5a6fe0
// reaches the same batch state through offsets exactly 0x11C higher (+0x23C current buffer, +0x240 material, +0x244
// baker, +0x248 a fourth word, +0x12c / +0x134 relative reads), which no declared base of this object explains; see
// that function's ledger row. The port keeps the names initBuffer and flush agree on.
//
// MEMBERS OF THIS CLASS (only these three groups belong to it):
//   +0x22C u32 defaultVertexBufferCapacityBytes   ctor 0x5a2e6c stores argument 1; initBuffer 0x58a950 reads it
//   +0x230 u32 defaultIndexBufferCapacityBytes    ctor 0x5a2e70 stores argument 2; initBuffer 0x58a954 reads it
//           Both arguments come from CSceneManager::CSceneManager, which passes the two words of the block at
//           IVideoDriver +0xd4 (= IDevice* m_device; IDevice::creationParameters is at +0x5c), so they are
//           device->creationParameters.bytes + 0x40 and + 0x44 (asm 0x58d994-0x58d9a0). Level::_LoadBatching writes
//           (unsigned)((double)size * 1.02) into them at 0x3f2c0c (vertex) and 0x3f2cd4 (index).
//   +0x234 scene::ISegmentCompileCallback*  ctor clears it (0x5a2e74); CSceneManager::compileInternal stores the
//           SDefaultSegmentCompileCallback that lives on its stack there (`STR SP, [R0,#0x234]`, asm 0x58fb64, with
//           R0 = manager +0x1c). flush is what calls it: 0x5a3a90 loads the +0x234 word into R3, 0x5a3ab4 loads its
//           vtable and 0x5a3abc calls slot +0x08.
//   +0x238 scene::IEndOfBatchCallback*  ctor clears it (0x5a2e78); compileInternal stores the
//           SDefaultEndOfBatchCallback on its stack there (asm 0x58fb70). No listing in this class reads the word
//           back, so only the store is proven; keep the type as the best reading of that store.
//   +0x23C / +0x240 / +0x248  the constructor clears these three (0x5a2e7c / 0x5a2e80 / 0x5a2e84) and the destructor
//           drops +0x248 (asm 0x5a3d64 load, 0x5a3d7c drop) and +0x240 (0x5a3d80-0x5a3dbc: decrement the count word of
//           the pointee, at zero run CVertexStreams::~CVertexStreams 0x5a0a1c and free). What each word HOLDS is not
//           pinned by any listing this batch opened: they are not the append buffer or the current vertex streams (see
//           above - those are the base's +0x120 and the material at +0x124), and +0x244 is a gap the constructor does
//           not touch. Rows 0x23C / 0x240 / 0x244 / 0x248 therefore stay UNDECLARED; declaring them was the bug that
//           made this class place three live pointers inside the base object.
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
    // runs at zero), then the CNullDriver base is destroyed. Those two words are NOT declared here (see the member
    // note), so the port's member teardown covers only the two capacities, the callbacks and the segment fields.
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
    // 0x5a6fe0 (0x1aa0 bytes, 1221 decompiled lines): the append path - rebinds the base's +0x120 / +0x124 / +0x128
    // batch state, clusters the primitives (spatialGridClustering 0x5a89b8 or kdTreeClustering 0x5a88a8 according to
    // +0x25C) and copies or deduplicates the vertices into the append buffer. NOT WRITTEN: it needs one pass of its
    // own, and glitch::video::isVertexStreamHomolaced (0x5a23f4) and copyVertex (0x5a2b18) are still partial. The
    // CAppendMeshBuffer dependency that was recorded as its blocker does not exist (the buffer is an opaque handle).
    void thisAppendBatch(const boost::intrusive_ptr<const CVertexStreams>& streams, const CPrimitiveStream& primitives,
                         const boost::intrusive_ptr<const scene::CMeshBuffer>& meshBuffer);
    // 0x5a3a20 _ZN6glitch5video12CBatchDriver5flushEv (vtable 0x977108 slot +0x1fc, the IVideoDriver::flush slot)
    // PARTIAL in glitch-video/src/batch_driver.cpp: the listing's unmap half and the trailing clear() are written.
    // Still missing is the +0x234 callback call (0x5a3aa8-0x5a3abc), because the port's scene callbacks declare no
    // operator() and the listing's builder-style second argument (the base's batch-buffer vector, not the material) is
    // not recovered; see the note in the source. Declared with `override` because it is the base's slot.
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

    scene::ISegmentCompileCallback* segmentCompileCallback = nullptr;  // +0x234, called by flush (vtable +0x08)
    // +0x238. Everything proven about this word and about +0x234 is: the constructor clears both, compileInternal
    // stores the stack SDefaultSegmentCompileCallback pointer at +0x234 (STR SP,[R0,#0x234], asm 0x58fb64) and the
    // stack SDefaultEndOfBatchCallback pointer at the next word (asm 0x58fb70), and flush calls the +0x234 word. The
    // exact pairing of interface to offset is therefore the best reading of the two stores, not a proven fact.
    scene::IEndOfBatchCallback* endOfBatchCallback = nullptr;  // +0x238

    // +0x23C, +0x240, +0x244 and +0x248 are declared NOWHERE on purpose. The constructor clears +0x23C, +0x240 and
    // +0x248 and the destructor drops +0x248 and +0x240, so they are real storage, but no listing read assigns them a
    // value and the only readers seen for that run (thisAppendBatch 0x5a7050-0x5a709c) are the base's batch state, not
    // these words: 0x5a7050 / 0x5a7054 rebind +0x124, 0x5a707c writes +0x128 and 0x5a7098-0x5a70ac rebound +0x12c,
    // which are m_batchMaterial, m_batchBaker and m_batchMaterialHeap of IVideoDriver. Declaring them here (as
    // `appendBuffer` / `currentAppendVertices` / `sortedBatches` / `unknown248` were) put three live pointers inside
    // the base object - CBatchDriver is a real, derivational object and everything from +0x110 to +0x15F belongs to
    // IVideoDriver. Re-declare them only with a listing that names their type.

    // +0x250, +0x25C and +0x260 (evidence in the file comment).
    core::vector3d<float> segmentSize;  // +0x250, cleared by the constructor
    bool segmentSizeMode = false;       // +0x25C, byte; non-zero selects the kd-tree clustering path
    uint32_t maxIndices = 0;            // +0x260
};

}  // namespace glitch::video
