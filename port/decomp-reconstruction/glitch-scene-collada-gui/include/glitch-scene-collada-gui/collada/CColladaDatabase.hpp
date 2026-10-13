// glitch::collada::CColladaDatabase and the loaded-file model it reads: the table-entry records (SAnimation, SImage, SEffect,
// SMaterial, SGeometry, SController, SLight, SCamera, SVisualScene, SForce, SCoronas, SGNPSEmitter, SEmitter, SNode), the table
// holder (SDatabaseTables), the loaded file (CResFile) and the resource classes around it (CResFileManager, CResFactory,
// CResFileReader, CAnimationStreamingManager). Canonical header of the engine unification wave (u1-scene). It replaces
// collada_database.hpp (the lookups over a vector model), layouts/collada_db.hpp (the 32-bit layout of the same records) and the
// CAnimationStreamingManager stand-in of collada_misc.hpp.
//
// Evidence (libDungeonHunter2.so, ARM, 32-bit layout; the offsets in the comments are the original ones):
//   CColladaDatabase is two words: +0x00 the loaded file (CResFile*, reference counted) and +0x04 the factory that builds the
//   scene objects. The tables are reached as record -> data (+0x24) -> tables (+0x20). Each table is a count followed by a base
//   pointer to contiguous entries of a fixed stride (given per record below). The host model keeps each table as a vector of
//   entry pointers (no stride arithmetic), except the animation-clip library, which is the contiguous 8-byte object the original
//   exposes (SLibraryAnimationClips).
//   The first word of every entry is its name, except SController, whose name is at +0x04.
// Port notes: pointers are host width, so sizeof differs from the original; the offsets are the contract. CResFile keeps its
// reference count as a plain word, because the database copies it only when it is not zero (0x6192f0, 0x619474), which is not the
// semantics of a counting handle.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "glitch-scene-collada-gui/collada/SAnimation.hpp"
#include "glitch-scene-collada-gui/collada/SLibraryAnimationClips.hpp"
#include "glitch-scene-collada-gui/deps/video_stand_ins.hpp"

namespace glitch::res {
class onDemandReader;  // layout not mapped in this category
}

namespace glitch {
class IReferenceCounted;
class IDevice;
namespace io {
class IReadFile;
class CMemoryReadFile;
}  // namespace io
namespace video {
class CMaterialVertexAttributeMap;
class CTextureManager;
class IBuffer;
}  // namespace video
}  // namespace glitch

namespace glitch::collada {

class CColladaFactory;
class CRootSceneNode;
class CSkinnedMesh;
class CMorphingMesh;
class CModularSkinnedMesh;
class CCameraSceneNode;
class CLightSceneNode;
class IMesh;
class CAnimationBlock;
class CColladaDatabase;
struct SAnimationBlockSearchKey;
class CResFactory;
class CResFileManager;
class CCoronasSceneNode;
class CParticleSystemSceneNode;
class CGlitchNewParticleSystemSceneNode;
struct SCollada;

// ---------------------------------------------------------------------------------------------------------------------
// Table entry records. `name` is the first word of each original entry, except SController where the name is at +0x04.
// ---------------------------------------------------------------------------------------------------------------------
struct SNode;
struct SEventKeyTable;

// SAnimation (stride 0x20), SChannel and the records they point to: canonical in collada/SAnimation.hpp.
// SAnimationClip (name, start, end; stride 0x0c) and SLibraryAnimationClips are in collada/SLibraryAnimationClips.hpp.
struct SImage { const char* name; /* stride 0x14 */ };
struct SEffect { const char* name; /* stride 0x74 */ };
struct SMaterial { const char* name; /* stride 0x24 */ };
struct SGeometry {
    const char* name;                 // +0x00, stride 0x10
    std::uint32_t reserved04;         // +0x04
    std::uint32_t notInstantiated;    // +0x08, non-zero makes constructGeometry return null
    std::uint32_t reserved0c;         // +0x0c
};
struct SController {
    std::uint32_t reserved;           // +0x00
    const char* name;                 // +0x04, stride 0x0c
    const void* morph = nullptr;      // +0x08 the morph record (a 32-bit pointer into the file data in the original; CMorphingMesh)
};
struct SLight {
    const char* name;             // +0x00, stride 0x18
    std::uint8_t unk_0c[12];      // +0x0c..+0x17, light block whose address getDefaultValue returns (case 16)
};
struct SCamera { const char* name; /* stride 0x1c */ };
struct SVisualScene {
    const char* name;             // +0x00, stride 0x10
    std::uint32_t reserved04 = 0; // +0x04
    std::uint32_t nodeCount = 0;  // +0x08, count of top-level SNode records
    SNode* nodes = nullptr;       // +0x0c, base of the SNode array (stride 0x50)
};
struct SForce {
    const char* name;            // +0x00, stride 0x10
    std::uint32_t reserved04;    // +0x04
    std::uint32_t kind;          // +0x08: 0 gravity, 1 wind, 2 deflector
    std::uint32_t reserved0c;    // +0x0c
};
struct SCoronas { const char* name; /* stride 0x24 */ };
struct SGNPSEmitter { const char* name; /* stride 0xe8 */ };
struct SEmitter { const char* name; /* stride 0x90 */ };

// Instance records name their target. The instance URL is "#<id>"; the original skips the leading '#' before looking the target up.
struct SInstanceForce {
    std::uint32_t reserved00;    // +0x00
    const char* url;             // +0x04, "#<name>"
};
struct SInstanceModularSkin { std::uint32_t reserved[1]; };  // opaque here

// COLLADA scene-node record (stride 0x50). Word offsets are the original 32-bit offsets.
struct SNode {
    const char* name = nullptr;                 // +0x00, compared with the lookup name
    std::uint32_t unk_04 = 0;                   // +0x04, not mapped
    std::uint32_t unk_08 = 0;                   // +0x08, not mapped
    std::uint32_t translation[3] = {};          // +0x0c..+0x14
    std::uint32_t rotation[4] = {};             // +0x18..+0x24
    std::uint32_t scale[3] = {};                // +0x28..+0x30
    std::uint32_t unk_34 = 0;                   // +0x34, bool word
    std::uint32_t childCount = 0;               // +0x38
    SNode* children = nullptr;                  // +0x3c, child array (stride 0x50)
    std::uint32_t instanceCount = 0;            // +0x40
    void* instances = nullptr;                  // +0x44, instance array (stride 8), entries not mapped
    std::uint32_t unk_48 = 0;                   // +0x48, passed to the factory node builders
    std::uint32_t billboard = 0;                // +0x4c, bool: selects the billboard builder
};

// Segment entry: 0x18 bytes, range [begin, end). Used by getAnimationSegment.
struct SAnimationSegment {
    std::int32_t begin;
    std::int32_t end;
    std::uint32_t reserved[4];

    // 0x60bee4 _ZN6glitch7collada17SAnimationSegment7getDataERNS_3res14onDemandReaderE
    // Declared only (partial). The body needs glitch::res::onDemandReader and onDemand<SAnimationData>::get, neither of which is
    // mapped here.
    SAnimationSegment* getData(glitch::res::onDemandReader& reader, int a3, int a4);
};

// Instance material: the link that linkInstanceMaterial sets and the vertex-attribute map releaseObjects drops.
struct SInstanceMaterial {
    glitch::video::CMaterialVertexAttributeMap* vertexAttributeMap = nullptr;  // +0x34 (an intrusive pointer in the original)
    SInstanceMaterial* linkNext = nullptr;                                     // +0x38 instance list link
};

// Table holder: the record the original reaches as *(*this + 0x24) + 0x20. The comments give the original count and base
// offsets of each table. Counts are signed in the original (the lookups test `count <= 0`).
struct SDatabaseTables {
    std::uint32_t version = 0;                    // +0x00 read by getVersion()
    const char* sourceName = nullptr;             // +0x04 the name string CResFileManager::postLoadProcess stores (a heap string in the
                                                  //       original; null when the file has no reader name, stage D)
    SInstanceMaterial* instanceMaterials = nullptr;  // +0x08, head of the instance-material list
    std::uint32_t externalResourcesDone = 0;      // +0x10 set to 1 by postLoadProcess when the external resources were resolved
    std::uint32_t animationFlag = 0;              // +0x14 non-zero when the animations are streamed (CSceneNodeAnimator +0x34)
    std::int32_t animationStart = 0;              // +0x1c start time of the whole animation, milliseconds (CSceneNodeAnimator +0x38)
    std::int32_t animationEnd = 0;                // +0x20 end time, milliseconds (CSceneNodeAnimator +0x3c)
    std::vector<SAnimation*> animations;          // +0x24 / +0x28
    std::vector<SAnimationSegment> segments;      // segment record at +0x30 of the holder: {count, base}, stride 0x18
    const SEventKeyTable* eventKeyTable = nullptr;  // +0x2c the events track CSceneNodeAnimatorSet::setCurrentAnimation hands to setEventsTrack
    SLibraryAnimationClips animationClipLibrary;  // +0x34 count / +0x38 clips, stride 0x0c (the object getAnimationClipLibrary returns)
    std::vector<SImage*> images;                  // +0x4c / +0x50
    std::vector<SEffect*> effects;                // +0x54 / +0x58
    std::vector<SMaterial*> materials;            // +0x5c / +0x60
    std::vector<SGeometry*> geometries;           // +0x68 / +0x6c
    std::vector<SController*> controllers;        // +0x70 / +0x74
    std::vector<SLight*> lights;                  // +0x44 / +0x48
    std::vector<SCamera*> cameras;                // +0x3c / +0x40
    std::vector<SVisualScene*> visualScenes;      // +0x98 / +0x9c
    std::vector<SForce*> forces;                  // +0x88 / +0x8c
    std::vector<SCoronas*> coronas;               // +0x90 / +0x94
    std::vector<SGNPSEmitter*> gnpsEmitters;      // +0x80 / +0x84
    std::vector<SEmitter*> emitters;              // +0x78 / +0x7c
};

// Mask bits accepted by find(); the original tries them in this order.
enum : unsigned {
    kFindAnimation = 0x01,
    kFindAnimationClip = 0x02,
    kFindImage = 0x04,
    kFindEffect = 0x08,
    kFindMaterial = 0x10,
    kFindGeometry = 0x20,
    kFindController = 0x40,
    kFindVisualScene = 0x80,
};

// ---------------------------------------------------------------------------------------------------------------------
// The loaded file and the resource classes around it (the 32-bit layout of layouts/collada_db.hpp, with typed members).
// ---------------------------------------------------------------------------------------------------------------------

// Per-file data block reached through CResFile::data (+0x24; the first word of the inline glitch::res::File).
struct SColladaFileData {
    std::uint8_t pad_0x00[0x08] = {};  // +0x00..+0x07
    std::uint32_t headerSize = 0;      // +0x08 becomes res::File::SizeOfHeader
    std::uint8_t pad_0x0c[0x04] = {};  // +0x0c..+0x0f
    std::uint32_t offsetEntries = 0;   // +0x10 external offset table size is headerSize + 4 * this
    std::int32_t externalFlags = 0;    // +0x14 sign bit selects the res::File::ExternalFilePtr slot
    std::uint8_t pad_0x18[0x08] = {};  // +0x18..+0x1f
    SDatabaseTables* tables = nullptr; // +0x20 the table holder
};

// The loaded COLLADA file. CColladaDatabase::record points here; a CResFile is constructed with new(0x50). The inline
// glitch::res::File starts at +0x24 and its first word is the header block `data`.
class CResFile {
public:
    // Not part of the original (which has only the loading constructor): an empty record for tests and the stand-in loaders.
    CResFile() = default;
    // 0x658048 (C1) / 0x65815c (C2): reads the header of `reader` and builds the record named `name`; `flag` selects the
    // on-demand reading. Declared only: the file reader (glitch::res::File::read) is not reconstructed.
    CResFile(const char* name, glitch::io::IReadFile* reader, bool flag);
    // ~CResFile(): 0x658a2c (D1), 0x658b6c (D2), 0x658b50 (D0, skipped): the original drops the reader, frees the buffer at +0x40 and releases the
    // pointer array at +0x44. Not reproduced: the implicit destructor leaves them.
    // 0x657c8c: releases one buffer reference held by the record (the IBuffer handle is reset).
    // 0x657cf8 / 0x657d58: releases a removable buffer, by handle and by index. Declared only.
    void releaseBuffer(boost::intrusive_ptr<video::IBuffer>& buffer);
    void releaseRemovableBuffer(boost::intrusive_ptr<video::IBuffer>& buffer);
    void releaseRemovableBuffer(std::uint8_t index);
    // 0x658744: releases the objects the record created (instance materials, buffers). Declared only.
    void releaseObjects();

    std::uint32_t vptr_0x00 = 0;                 // +0x00 vtable pointer word (the virtual slots are not modelled)
    std::uint32_t refCount = 1;                  // +0x04 intrusive count
    glitch::io::IReadFile* reader = nullptr;     // +0x08 file the record was read from (refcounted)
    std::uint8_t name_0x0c[0x14] = {};           // +0x0c..+0x1f name string (inline buffer at +0x0c)
    const char* fileName = nullptr;              // +0x20 name string data; passed to unload() and matched by the streaming cache
    SColladaFileData* data = nullptr;            // +0x24 first word of the inline res::File
    std::uint8_t flag_0x28 = 0;                  // +0x28 bool, zeroed by the constructor (role not confirmed)
    std::uint8_t pad_0x29[0x03] = {};            // +0x29..+0x2b
    std::uint32_t word_0x2c = 0;                 // +0x2c zeroed by the constructor (role not confirmed)
    std::uint8_t pad_0x30[0x08] = {};            // +0x30..+0x37
    std::int32_t arrayCount_0x38 = 0;            // +0x38 bound of the pointer array at +0x44 in the destructor
    void* buffer_0x40 = nullptr;                 // +0x40 buffer released with operator delete[] (type not confirmed)
    void** array_0x44 = nullptr;                 // +0x44 pointer array, elements released in the destructor
    std::uint8_t flag_0x48 = 0;                  // +0x48 bool (role not confirmed)
    std::uint8_t pad_0x49[0x07] = {};            // +0x49..+0x4f
};

// glitch::collada::CResFactory. Its members are not observed in the reconstructed bodies (getTexture and getTextureImpl do not
// touch `this`). The object is held through a pointer in CResFileManager. Polymorphic in the original.
class CResFactory {
public:
    // 0x65788c _ZN6glitch7collada11CResFactoryD2Ev (empty, same as D1)
    // 0x657890 _ZN6glitch7collada11CResFactoryD1Ev (empty)
    ~CResFactory() = default;
    // 0x657a08 (D0, skipped).
    // 0x6594a0: the texture named `name`, loaded from `file` through the texture manager (the two strings are directories); the
    // original returns an intrusive_ptr through a hidden out slot (the first argument). Declared only.
    glitch::IReferenceCounted** getTextureImpl(glitch::IReferenceCounted** out, const std::string& name, glitch::io::IReadFile* file,
                                               glitch::video::CTextureManager* manager, const char* directory,
                                               const char* fallback);
    // 0x659704: as getTextureImpl for the image record `image` of `resFile`. Declared only.
    glitch::IReferenceCounted** getTexture(glitch::IReferenceCounted** out, CResFile* resFile, const std::string& name,
                                           glitch::io::IReadFile* file, glitch::video::CTextureManager* manager, SImage* image);
};

// glitch::collada::CResFileReader. Its vtable pointer is at +0x00 and the wrapped IReadFile is at +0x04. CResFile builds one on
// the stack. Polymorphic in the original.
class CResFileReader {
public:
    // 0x657888 _ZN6glitch7collada14CResFileReaderD1Ev (empty)
    ~CResFileReader() = default;

    // 0x6579e0 (D0, skipped).
    // 0x657818, 0x657834, 0x657850, 0x65786c: forward to the wrapped file's position, read, seek and size slots. Declared only (the
    // IReadFile interface is not included here).
    std::int32_t tell();
    std::int32_t read(void* buffer, std::uint32_t size);
    std::int32_t seek(std::int32_t position, bool relative);
    std::int32_t getSize();

    std::uint32_t vptr_0x00 = 0;                 // +0x00 vtable pointer
    glitch::io::IReadFile* file = nullptr;       // +0x04 wrapped file (tell/read/seek/getSize forward to it)
};

// glitch::collada::CResFileManager, a singleton (static Inst).
class CResFileManager {
public:
    static CResFileManager* Inst;
    static CResFactory DefaultResFactory;

    std::uint32_t vptr_0x00 = 0;                 // +0x00 vtable pointer (constructed with off_983030)
    std::uint32_t refCount = 1;                  // +0x04 set to 1 by the constructor
    // std::map<std::string, CResFile*> of loaded records. The rb-tree header node is at +0x08 (color +0x08, parent +0x0c, left
    // +0x10, right +0x14), and the node count is at +0x18.
    std::uint8_t files_0x08[0x14] = {};          // +0x08..+0x1b
    std::uint8_t pad_0x1c[0x04] = {};            // +0x1c..+0x1f
    glitch::IDevice* device = nullptr;           // +0x20 file system is read from device +0x34
    CResFactory* resFactory = nullptr;           // +0x24 set to &DefaultResFactory
    bool autoUnloadEnabled = true;               // +0x28 streaming/unload path of ~CColladaDatabase; get() clears it temporarily
    bool flag_0x29 = false;                      // +0x29 read by CResFactory::getTexture (role not confirmed)
    bool zipLoadingEnabled = true;               // +0x2a enables the zip path in getReadFile
    bool disableZipAfterFirstUse = true;         // +0x2b when set, getReadFile clears zipLoadingEnabled after one use

    // 0x657894 (C1) / 0x657914 (C2): zeroes the members, stores the device and the default factory and publishes the singleton.
    // Declared only. ~CResFileManager(): 0x6582c4 (D1), 0x6583c4 (D2), 0x6583a8 (D0, skipped): unload everything and unpublish. Not reproduced.
    explicit CResFileManager(glitch::IDevice* device);
    CResFileManager() = default;
    // 0x65799c: the version check of a file by name. Declared only.
    std::int32_t checkVersion(const char* fileName);
    // 0x657bd0: the read file the loaders use for `file` (a memory copy for zipped archives). Declared only.
    glitch::io::CMemoryReadFile* getReadFile(glitch::io::IReadFile* file);
    // 0x658564: resolves the external resources of `file` once (the SDatabaseTables flag). Declared only; the body in
    // recon/slice_32.cpp is the free function CResFileManager_updateExternalResources.
    void updateExternalResources(CResFile* file, glitch::io::IReadFile* reader);
    // 0x6584fc: unload(iterator into the loaded-file map); the STLport iterator is not declared here.
    // 0x659b60, 0x659cfc, 0x659c3c: unload by name, by SCollada record, and everything.
    std::int32_t unload(const char* fileName, bool flag);
    void* unload(const SCollada* collada, bool flag);  // returns the map node after the erased one
    std::int32_t unloadAll();
    // 0x65a748, 0x65a9a8, 0x65ac74: the loaded record for a read file / a file name / a name relative to a record; loads it when
    // missing. 0x65a98c, 0x65ac5c: load with an optional completion callback.
    CResFile* get(glitch::io::IReadFile* file, bool flagA, bool flagB);
    CResFile* get(const char* fileName, bool flag);
    CResFile* get(CResFile* relativeTo, const char* fileName, bool flag);
    CResFile* load(glitch::io::IReadFile* file, bool flag, void (*callback)(const char*, const SCollada*), bool flagB);
    CResFile* load(const char* fileName, bool flag, void (*callback)(const char*, const SCollada*));

    // 0x657994 _ZN6glitch7collada15CResFileManager12checkVersionEPNS_2io9IReadFileE: returns 0 (the original body is a constant).
    std::uint32_t checkVersion(glitch::io::IReadFile* file);
    // 0x658c90 _ZN6glitch7collada15CResFileManager15postLoadProcessEPNS0_8CResFileEPNS_2io9IReadFileE
    // Partial: see src/tb_scene-collada-gui-glitch_collada_CResFileManager-0.cpp.
    int postLoadProcess(CResFile* file, glitch::io::IReadFile* reader);
};

// glitch::collada::CAnimationStreamingManager, a singleton (static Instance). The object is 0x20 bytes of data (no vptr): its
// constructor (0x60b3cc / 0x60b410) zeroes the eight words.
class CAnimationStreamingManager {
public:
    // Cache entry, 12 bytes. Stored in the cache vector at +0x0c..+0x14.
    struct SegmentCacheEntry {
        std::uint32_t size;              // +0x00 bytes charged to cacheUsage
        std::int32_t* refCount;          // +0x04 refcount word of the cached block data (evicted when it is 1)
        const char* fileName;            // +0x08 owning record's fileName, matched by release()
    };

    // 0x60b3cc _ZN6glitch7collada26CAnimationStreamingManagerC2Ev
    // 0x60b410 _ZN6glitch7collada26CAnimationStreamingManagerC1Ev
    // Zeroes the members (the original publishes the object through the static Instance pointer).
    CAnimationStreamingManager();

    static CAnimationStreamingManager* Instance;

    // 0x60c59c _ZN6glitch7collada26CAnimationStreamingManager17getAnimationBlockERNS0_24SAnimationBlockSearchKeyERN5boost13intrusive_ptrINS0_15CAnimationBlockEEE
    // Finds (or loads) the block that holds the keys of the clip at the key's time and stores it in `block`, replacing the held one.
    // PARTIAL: the block registry (the sorted vector at +0x00, CAnimationBlock::getBlock 0x60b350, the block constructors
    // 0x60c05c .. 0x60c940 and the cache) is not reconstructed, so no block is found and `block` is left unchanged.
    void getAnimationBlock(SAnimationBlockSearchKey& key, CAnimationBlock*& block);
    // 0x60c220: the one-argument form; returns the block (null when none). Declared only (see the registry note above).
    CAnimationBlock* getAnimationBlock(SAnimationBlockSearchKey& key);
    // 0x60b918: drops the cached and registered blocks of `database`'s file. 0x60b990: evicts cached blocks while the usage is above
    // the limit. 0x60bc7c: adds a block's data to the cache. 0x60c414 / 0x60bb64: insert into / remove from the sorted registry.
    // Declared only: the registry and the cache are not reconstructed.
    void release(CColladaDatabase* database);
    void checkMemoryUsage();
    void cache(CAnimationBlock* block);
    void registerAnimationBlock(CAnimationBlock* block);
    void unregisterAnimationBlock(CAnimationBlock* block);

    CAnimationBlock** registryBegin = nullptr;        // +0x00 sorted registry of blocks
    CAnimationBlock** registryEnd = nullptr;          // +0x04
    CAnimationBlock** registryCapacityEnd = nullptr;  // +0x08
    SegmentCacheEntry* cacheBegin = nullptr;          // +0x0c
    SegmentCacheEntry* cacheEnd = nullptr;            // +0x10
    SegmentCacheEntry* cacheCapacityEnd = nullptr;    // +0x14
    std::int32_t cacheLimit = 0;                      // +0x18 maximum cache bytes; caching is off when not positive
    std::int32_t cacheUsage = 0;                      // +0x1c bytes currently cached
};

// ---------------------------------------------------------------------------------------------------------------------
// glitch::collada::CColladaDatabase. The original's two pointer words are `record` (+0x00) and `factory` (+0x04); the table
// holder is reached as record->data->tables (original *(*this + 0x24) + 0x20).
//
// Member functions below are the ones declared so far; each comment gives the original address. Overloads taking an index use the
// unchecked base + index * stride reads of the original.
// ---------------------------------------------------------------------------------------------------------------------
struct SInstanceController;
struct SInstanceGeometry;
struct SInstanceCamera;
struct SInstanceCoronas;
struct SInstanceEmitter;
struct SInstanceGNPSEmitter;

class CColladaDatabase {
public:
    CResFile* record = nullptr;           // +0x00
    CColladaFactory* factory = nullptr;   // +0x04

    const SDatabaseTables& tables() const { return *record->data->tables; }
    const char* fileName() const { return record ? record->fileName : nullptr; }

    // 0x60e2a8 getVersion() const
    std::uint32_t getVersion() const;
    // 0x60e334 getAnimationClipLibrary() const
    // The address of the 8-byte clip library inside the table holder (holder +0x34): null when no file is loaded.
    const SLibraryAnimationClips* getAnimationClipLibrary() const;
    // 0x60e348 getScene() const
    void* getScene() const;
    // 0x60fa24 constructController(IVideoDriver*, SController*, CRootSceneNode*) const
    // The original returns through a hidden out slot (intrusive_ptr); the out slot is the first argument here.
    glitch::IReferenceCounted** constructController(glitch::IReferenceCounted** out, video::IVideoDriver* driver,
                                                    SController* controller, CRootSceneNode* root) const;
    // 0x60fb54 constructAnimator() const
    void* constructAnimator() const;

    // 0x60e4bc getExternalLightSceneNode(intrusive_ptr<CMaterial const> const&, unsigned short, unsigned int, char const*) const
    void getExternalLightSceneNode(const boost::intrusive_ptr<video::CMaterial const>& material, std::uint16_t a,
                                   std::uint32_t b, const char* name) const;
    // 0x60e504 getExternalLightSceneNode(intrusive_ptr<CMaterialRenderer const> const&, unsigned short, unsigned int, char const*) const
    void getExternalLightSceneNode(const boost::intrusive_ptr<video::CMaterialRenderer const>& renderer,
                                   std::uint16_t a, std::uint32_t b, const char* name) const;

    // 0x60e570 constructEffect(IVideoDriver*, SEffect*, CRootSceneNode*) const
    video::CMaterialRenderer* constructEffect(video::IVideoDriver* driver, SEffect* effect,
                                              CRootSceneNode* root) const;
    // 0x60e5e4 constructEffect(IVideoDriver*, int, CRootSceneNode*) const
    video::CMaterialRenderer* constructEffect(video::IVideoDriver* driver, int index, CRootSceneNode* root) const;
    // 0x61b10c constructEffect(IVideoDriver*, char const*, CRootSceneNode*) const
    video::CMaterialRenderer* constructEffect(video::IVideoDriver* driver, const char* name,
                                              CRootSceneNode* root) const;

    // 0x60e634 constructGeometry(IVideoDriver*, SGeometry*) const
    IMesh* constructGeometry(video::IVideoDriver* driver, SGeometry* geometry) const;
    // 0x60e6b8 constructGeometry(IVideoDriver*, int) const
    IMesh* constructGeometry(video::IVideoDriver* driver, int index) const;
    // 0x61aaa8 constructGeometry(IVideoDriver*, char const*) const
    IMesh* constructGeometry(video::IVideoDriver* driver, const char* name) const;

    // 0x60e6f0 constructModularSkin(SInstanceModularSkin*, CRootSceneNode*) const
    CModularSkinnedMesh* constructModularSkin(SInstanceModularSkin* instance, CRootSceneNode* root) const;

    // 0x60e88c constructForce(SForce*, CRootSceneNode*) const
    void* constructForce(SForce* force, CRootSceneNode* root) const;
    // 0x60e910 constructForce(int, CRootSceneNode*) const
    void* constructForce(int index, CRootSceneNode* root) const;
    // 0x61a970 constructForce(char const*, CRootSceneNode*) const
    void constructForce(const char* name, CRootSceneNode* root) const;
    // 0x61a994 constructForce(SInstanceForce*, CRootSceneNode*) const
    void constructForce(SInstanceForce* instance, CRootSceneNode* root) const;

    // 0x60f9a4 constructMorph(IVideoDriver*, SController*, CRootSceneNode*) const
    CMorphingMesh* constructMorph(video::IVideoDriver* driver, SController* controller, CRootSceneNode* root) const;

    // 0x60e62c constructCamera(SInstanceCamera*, CRootSceneNode*) const: returns null (an empty body in the original).
    CCameraSceneNode* constructCamera(SInstanceCamera* instance, CRootSceneNode* root) const;
    // 0x619398 constructCamera(SCamera*, CRootSceneNode*) const
    CCameraSceneNode* constructCamera(SCamera* camera, CRootSceneNode* root) const;
    // 0x6193e0 constructCamera(int, CRootSceneNode*) const
    CCameraSceneNode* constructCamera(int index, CRootSceneNode* root) const;
    // 0x61b2d0 constructCamera(char const*, CRootSceneNode*) const
    void constructCamera(const char* name, CRootSceneNode* root) const;

    // 0x619404 constructLight(SLight*, CRootSceneNode*) const
    void* constructLight(SLight* light, CRootSceneNode* root) const;
    // 0x619450 constructLight(int, CRootSceneNode*) const
    void* constructLight(int index, CRootSceneNode* root) const;
    // 0x61b24c constructLight(char const*, CRootSceneNode*) const
    void constructLight(const char* name, CRootSceneNode* root) const;

    // 0x61ace8 constructController(IVideoDriver*, SInstanceController*, CRootSceneNode*, bool) const
    // Writes the controller into the out slot and returns it. Partial: see the .cpp.
    glitch::IReferenceCounted** constructController(glitch::IReferenceCounted** out, video::IVideoDriver* driver,
                                                    SInstanceController* instance, CRootSceneNode* root,
                                                    bool flag) const;
    // 0x61aeb8 constructGeometry(IVideoDriver*, SInstanceGeometry*, CRootSceneNode*) const
    // Writes the geometry into the out slot and returns it. Partial: see the .cpp.
    glitch::IReferenceCounted** constructGeometry(glitch::IReferenceCounted** out, video::IVideoDriver* driver,
                                                  SInstanceGeometry* instance, CRootSceneNode* root) const;
    // 0x61b2f4 constructNode(IVideoDriver*, SNode*, CRootSceneNode*) const
    // Returns the scene node built for `node` (an owned reference, as the factory returns it). Partial.
    void* constructNode(video::IVideoDriver* driver, SNode* node, CRootSceneNode* root) const;
    // 0x61b9e8 constructScene(IVideoDriver*) const
    // Returns the root scene node of the file. Partial.
    CRootSceneNode* constructScene(video::IVideoDriver* driver) const;

    // 0x61c0c8 getAnimation(char const*, SChannel::Type, unsigned char) const
    SAnimation* getAnimation(const char* name, SChannel::Type type, std::uint8_t flag) const;
    // 0x61c1e0 getBlendableAnimation(SChannel const*) const
    SAnimation* getBlendableAnimation(const SChannel* channel) const;
    // 0x61c214 getNode(char const*, SNode&) const
    // Returns `node` when its name matches, else the first match among its descendants.
    SNode* getNode(const char* name, SNode& node) const;
    // 0x61c290 getNode(char const*) const
    SNode* getNode(const char* name) const;
    // 0x61c2f8 getDefaultValue(char const*, SChannel::Type, void**, void*) const
    // The original returns a char that is 1 on success; here it is bool. Partial: see the .cpp.
    bool getDefaultValue(const char* name, SChannel::Type type, void** out, void* flag) const;
    // 0x61c6bc getDefaultValue(SChannel const*, void**) const
    bool getDefaultValue(const SChannel* channel, void** out) const;

    // ---- Loading constructors. CColladaDatabase is a plain {record, factory} pair in this port (a handle that is copied with
    // copyDatabase and dropped with dropDatabase), so the constructors that load a file are static creators here. Each original
    // constructor is a C1 / C2 pair. Declared only: they go through CResFileManager::get / load, which are not reconstructed.
    static CColladaDatabase create(const char* fileName, CColladaFactory* factory);                           // 0x60f25c (C1), 0x60f2b0 (C2)
    static CColladaDatabase create(glitch::io::IReadFile* file, bool flag, CColladaFactory* factory);          // 0x60f304 (C1), 0x60f364 (C2)
    static CColladaDatabase create(glitch::io::IReadFile* file, CColladaFactory* factory);                     // 0x60f3c4 (C1), 0x60f42c (C2)
    // 0x619344: the database of file `name` next to `source`'s file.
    static CColladaDatabase create(const CColladaDatabase* source, const char* name, CColladaFactory* factory);

    // ---- The indexed getters (getLight(int) and the family) are the free helpers lightAt ... below; the two-name getMaterial is
    // materialOfFile below. Members of those names would hide the free getX(tables, name) lookups the constructX members call.
    // 0x61c1fc / 0x61c6ec: the SAnimation* spellings of getBlendableAnimation(const SChannel*) / getDefaultValue(const SChannel*, ...).
    SAnimation* getBlendableAnimation(const SAnimation* animation) const;
    bool getDefaultValue(const SAnimation* animation, void** out) const;
    // 0x61c91c: the animation named `name` of type `type` whose binding URI matches `uri`.
    SAnimation* getAnimation(const char* name, SChannel::Type type, const char* uri) const;

    // ---- Constructors of scene objects by record, index and name. The factory is `factory` (+0x04). Declared only unless noted.
    // Coronas: 0x60e75c (record), 0x61a750 (name), 0x61a77c (instance record). The original builds the node through the factory.
    CCoronasSceneNode* constructCoronas(SCoronas* coronas, video::IVideoDriver* driver, CRootSceneNode* root) const;
    CCoronasSceneNode* constructCoronas(const char* name, video::IVideoDriver* driver, CRootSceneNode* root) const;
    CCoronasSceneNode* constructCoronas(SInstanceCoronas* instance, video::IVideoDriver* driver, CRootSceneNode* root) const;
    // Emitters: 0x60e79c (record), 0x60e7e0 (index), 0x61a854 (name), 0x61a888 (instance record).
    CParticleSystemSceneNode* constructEmitter(SEmitter* emitter, video::IVideoDriver* driver, res::vector<res::String>* materials,
                                               CRootSceneNode* root) const;
    CParticleSystemSceneNode* constructEmitter(int index, video::IVideoDriver* driver, res::vector<res::String>* materials,
                                               CRootSceneNode* root) const;
    CParticleSystemSceneNode* constructEmitter(const char* name, video::IVideoDriver* driver, res::vector<res::String>* materials,
                                               CRootSceneNode* root) const;
    CParticleSystemSceneNode* constructEmitter(SInstanceEmitter* instance, video::IVideoDriver* driver, CRootSceneNode* root) const;
    // Glitch-new-particle-system emitters: 0x60e814 (record), 0x60e858 (index), 0x61a5d4 (name), 0x61a608 (instance record).
    CGlitchNewParticleSystemSceneNode* constructGNPSEmitter(SGNPSEmitter* emitter, video::IVideoDriver* driver,
                                                            res::vector<res::String>* materials, CRootSceneNode* root) const;
    CGlitchNewParticleSystemSceneNode* constructGNPSEmitter(int index, video::IVideoDriver* driver,
                                                            res::vector<res::String>* materials, CRootSceneNode* root) const;
    CGlitchNewParticleSystemSceneNode* constructGNPSEmitter(const char* name, video::IVideoDriver* driver,
                                                            res::vector<res::String>* materials, CRootSceneNode* root) const;
    CGlitchNewParticleSystemSceneNode* constructGNPSEmitter(SInstanceGNPSEmitter* instance, video::IVideoDriver* driver,
                                                            CRootSceneNode* root) const;
    // Skins, controllers by index / name, images, materials: the original returns an intrusive_ptr through a hidden out slot (the first
    // argument here, as for the other controllers). Declared only.
    glitch::IReferenceCounted** constructSkin(glitch::IReferenceCounted** out, video::IVideoDriver* driver, SController* controller,
                                              CRootSceneNode* root) const;                                    // 0x60f924
    glitch::IReferenceCounted** constructController(glitch::IReferenceCounted** out, video::IVideoDriver* driver, int index,
                                                    CRootSceneNode* root) const;                              // 0x60fa70
    glitch::IReferenceCounted** constructController(glitch::IReferenceCounted** out, video::IVideoDriver* driver, const char* name,
                                                    CRootSceneNode* root) const;                              // 0x61aa00
    glitch::IReferenceCounted** constructImage(glitch::IReferenceCounted** out, SImage* image, CRootSceneNode* root) const;      // 0x60fc70
    glitch::IReferenceCounted** constructImage(glitch::IReferenceCounted** out, int index, CRootSceneNode* root) const;          // 0x60fcc0
    glitch::IReferenceCounted** constructImage(glitch::IReferenceCounted** out, const char* name, CRootSceneNode* root) const;   // 0x61b1b4
    glitch::IReferenceCounted** constructMaterial(glitch::IReferenceCounted** out, video::IVideoDriver* driver, SMaterial* material,
                                                  CRootSceneNode* root) const;                                // 0x61cca8
    glitch::IReferenceCounted** constructMaterial(glitch::IReferenceCounted** out, video::IVideoDriver* driver, const char* name,
                                                  CRootSceneNode* root) const;                                // 0x61cefc
    glitch::IReferenceCounted** constructMaterial(glitch::IReferenceCounted** out, video::IVideoDriver* driver, int index,
                                                  CRootSceneNode* root) const;                                // 0x61cf44
    // 0x61aae0: the geometry `geometryName` of the file `fileName`, which is opened next to this database's file through
    // CResFileManager::get (it logs "File not found" and "Geometry not found" through the printer). Out-slot return as above.
    glitch::IReferenceCounted** constructGeometry(glitch::IReferenceCounted** out, video::IVideoDriver* driver, const char* fileName,
                                                  const char* geometryName) const;
    // Visual scenes: 0x61b8bc (record), 0x61b988 (index), 0x61b9b8 (name). The scene's nodes are constructed below `root`.
    CRootSceneNode* constructVisualScene(video::IVideoDriver* driver, SVisualScene* scene, CRootSceneNode* root) const;
    CRootSceneNode* constructVisualScene(video::IVideoDriver* driver, int index, CRootSceneNode* root) const;
    CRootSceneNode* constructVisualScene(video::IVideoDriver* driver, const char* name, CRootSceneNode* root) const;
    // Scenes: 0x61babc (the scene of the file with the flag) is a member. The four loading overloads below are static in the
    // original (the decompiler lists a leading `this` that is really the driver): each one asks CResFileManager::load for the file,
    // switches the manager's flag at +0x28 off, builds a temporary database over the loaded file with `factory` (the static
    // CColladaDatabase::DefaultFactory when null), runs the member constructScene(driver) and drops the database.
    CRootSceneNode* constructScene(video::IVideoDriver* driver, bool flag) const;                                          // 0x61babc
    static CRootSceneNode* constructScene(video::IVideoDriver* driver, const char* fileName, CColladaFactory* factory);   // 0x61bb20
    static CRootSceneNode* constructScene(video::IVideoDriver* driver, const char* fileName, bool flag,
                                          CColladaFactory* factory);                                                       // 0x61bbd4
    static CRootSceneNode* constructScene(video::IVideoDriver* driver, glitch::io::IReadFile* file, bool flagA, bool flagB,
                                          CColladaFactory* factory);                                                       // 0x61bc8c
    static CRootSceneNode* constructScene(video::IVideoDriver* driver, glitch::io::IReadFile* file, bool flag,
                                          CColladaFactory* factory);                                                       // 0x61bd54
    // Nodes: 0x61be14 (a record, without the root) and 0x61c6f4 (by name) are members; the three loading overloads are static and
    // follow the scheme above for the named node.
    CRootSceneNode* constructNode(video::IVideoDriver* driver, SNode* node) const;                                         // 0x61be14
    CRootSceneNode* constructNode(video::IVideoDriver* driver, const char* nodeName) const;                                // 0x61c6f4
    static CRootSceneNode* constructNode(video::IVideoDriver* driver, glitch::io::IReadFile* file, const char* nodeName, bool flag,
                                         CColladaFactory* factory);                                                        // 0x61c71c
    static CRootSceneNode* constructNode(video::IVideoDriver* driver, glitch::io::IReadFile* file, const char* nodeName,
                                         CColladaFactory* factory);                                                        // 0x61c7cc
    static CRootSceneNode* constructNode(video::IVideoDriver* driver, const char* fileName, const char* nodeName,
                                         CColladaFactory* factory);                                                        // 0x61c878
    // 0x619510, 0x6195c8, 0x61967c: the animator of a file, static, same scheme (the member constructAnimator() is 0x60fb54).
    static void* constructAnimator(glitch::io::IReadFile* file, bool flag, CColladaFactory* factory);
    static void* constructAnimator(glitch::io::IReadFile* file, CColladaFactory* factory);
    static void* constructAnimator(const char* fileName, CColladaFactory* factory);
};

// ---------------------------------------------------------------------------------------------------------------------
// The animation block the streaming manager hands to the animators (glitch::collada::CAnimationBlock, 0x20 bytes; constructors
// 0x60c05c, 0x60c610, 0x60c7a8, 0x60c940, destructor 0x60bbd8, drop 0x60bb1c; all read through the layout evidence of the
// constructors and the destructor):
//   +0x00 the reference count (drop frees the block when it reaches one), +0x04 / +0x08 a copy of the CColladaDatabase handle (its
//   record gets a use), +0x0c the clip range the block was built for (when 0 it becomes the database clip at +24), +0x10 the animation
//   segment (count +0x00, index +0x04, users +0x08), +0x14 the data holder (SAnimationDataHolder), +0x18 / +0x1c the next and previous
//   blocks (prepareNextBlock links them; the destructor clears the peer's link).
// The data holder keeps the loaded key data: its +0x00 is the reference count (the buffer is freed when it reaches one) and +0x0c the
// buffer, the output array the animators build their SAnimationAccessor over.
// ---------------------------------------------------------------------------------------------------------------------
struct SAnimationDataHolder {
    std::uint32_t refCount = 0;               // +0x00 (decremented; the buffer is freed at 1)
    std::uint32_t unk_04 = 0;                 // +0x04, not mapped
    std::uint32_t unk_08 = 0;                 // +0x08, not mapped
    const SAnimationOutput* buffer = nullptr; // +0x0c the output array (operator delete[] target)
};

class CAnimationBlock {
public:
    std::uint32_t refCount = 0;               // +0x00
    CColladaDatabase database;                // +0x04 / +0x08
    const SAnimationClip* range = nullptr;    // +0x0c
    SAnimationSegment* segment = nullptr;     // +0x10
    SAnimationDataHolder* dataHolder = nullptr;  // +0x14
    CAnimationBlock* next = nullptr;          // +0x18 the next block, or this block when there is none
    CAnimationBlock* previous = nullptr;      // +0x1c

    // ~CAnimationBlock(): 0x60ba2c (D1), 0x60bbd8 (D0, skipped): unregisters the block, drops the data holder and the database handle. Not reproduced.
    // 0x60bad0: frees the block's key data. 0x60c31c: loads the next block of the chain. 0x60c3f0: takes a use of the block.
    // Declared only.
    void free();
    void prepareNextBlock();
    void grab();
};

// The key the streaming manager looks a block up by (SAnimationBlockSearchKey): the database handle, the clip and the time.
struct SAnimationBlockSearchKey {
    SAnimationBlockSearchKey() = default;
    // 0x65d6d8: copies the database handle (a use of the record) and stores the clip and the time.
    SAnimationBlockSearchKey(const CColladaDatabase& source, std::int32_t clipIndex, std::int32_t timeMs);
    CColladaDatabase database;                // +0x00 / +0x04 (a copy of the animator's handle; the destructor drops it)
    std::int32_t clip = 0;                    // +0x08 the clip of the timeline controller (vptr+0x38)
    std::int32_t time = 0;                    // +0x0c
};

inline SAnimationBlockSearchKey::SAnimationBlockSearchKey(const CColladaDatabase& source, std::int32_t clipIndex, std::int32_t timeMs)
    : clip(clipIndex), time(timeMs) {
    database.record = source.record;
    database.factory = source.factory;
    if (database.record != nullptr && database.record->refCount != 0) {
        ++database.record->refCount;
    }
}

// The copy of a database handle that the original inlines at each member copy (e.g. 0x65dbd8, 0x644eb8, 0x64b9c8): the record gets
// a use only when its count is not zero. CColladaDatabase keeps plain pointers (see the file comment), so the copy takes the use.
inline void copyDatabase(CColladaDatabase& to, const CColladaDatabase& from) {
    to.record = from.record;
    to.factory = from.factory;
    if (to.record != nullptr && to.record->refCount != 0) {
        ++to.record->refCount;
    }
}

// The first part of ~CColladaDatabase (0x619474): the record loses the use the handle held. PARTIAL: when the count is then 1 and the
// manager's auto-unload is on, the original also calls CAnimationStreamingManager::release and CResFileManager::unload (stand-ins in
// recon/slice_07.cpp and recon/slice_11.cpp), which are not reconstructed.
// 0x619784 ~CColladaDatabase is this helper: the original's destructor decrements the record count and unloads the file when
// the last handle goes (the auto-unload is not modelled).
inline void dropDatabase(CColladaDatabase& database) {
    if (database.record != nullptr && database.record->refCount != 0) {
        --database.record->refCount;
    }
    database.record = nullptr;
}

// Lookups by name over the tables (the CColladaDatabase::getX(char const*) members of the original, over the table holder).
// 0x61be98 _ZNK6glitch7collada16CColladaDatabase16getAnimationClipEPKc
SAnimationClip* getAnimationClip(const SDatabaseTables& tables, const char* name);
// 0x61bef8 _ZNK6glitch7collada16CColladaDatabase12getAnimationEPKc
SAnimation* getAnimation(const SDatabaseTables& tables, const char* name);
// 0x61a574 _ZNK6glitch7collada16CColladaDatabase14getGNPSEmitterEPKc
SGNPSEmitter* getGNPSEmitter(const SDatabaseTables& tables, const char* name);
// 0x61a6f0 _ZNK6glitch7collada16CColladaDatabase10getCoronasEPKc
SCoronas* getCoronas(const SDatabaseTables& tables, const char* name);
// 0x61a7f4 _ZNK6glitch7collada16CColladaDatabase10getEmitterEPKc
SEmitter* getEmitter(const SDatabaseTables& tables, const char* name);
// 0x61a690 _ZNK6glitch7collada16CColladaDatabase14getVisualSceneEPKc
SVisualScene* getVisualScene(const SDatabaseTables& tables, const char* name);
// 0x61a910 _ZNK6glitch7collada16CColladaDatabase8getForceEPKc
SForce* getForce(const SDatabaseTables& tables, const char* name);
// 0x61a9a0 _ZNK6glitch7collada16CColladaDatabase13getControllerEPKc
SController* getController(const SDatabaseTables& tables, const char* name);
// 0x61aa48 _ZNK6glitch7collada16CColladaDatabase11getGeometryEPKc
SGeometry* getGeometry(const SDatabaseTables& tables, const char* name);
// 0x61ac28 _ZNK6glitch7collada16CColladaDatabase11getMaterialEPKc
SMaterial* getMaterial(const SDatabaseTables& tables, const char* name);
// 0x61b0ac _ZNK6glitch7collada16CColladaDatabase9getEffectEPKc
SEffect* getEffect(const SDatabaseTables& tables, const char* name);
// 0x61b154 _ZNK6glitch7collada16CColladaDatabase8getImageEPKc
SImage* getImage(const SDatabaseTables& tables, const char* name);
// 0x61b1ec _ZNK6glitch7collada16CColladaDatabase8getLightEPKc
SLight* getLight(const SDatabaseTables& tables, const char* name);
// 0x61b270 _ZNK6glitch7collada16CColladaDatabase9getCameraEPKc
SCamera* getCamera(const SDatabaseTables& tables, const char* name);

// 0x61bf58 _ZNK6glitch7collada16CColladaDatabase4findEPKcRj
// Tries the kinds selected in `mask` in order; on a hit, `mask` is set to the kind that matched and the entry is returned (as
// void*, the kind is in mask).
const void* find(const SDatabaseTables& tables, const char* name, unsigned& mask);

// 0x60e2bc _ZNK6glitch7collada16CColladaDatabase19getAnimationSegmentEi
// Returns the segment whose [begin, end) contains `frame`; otherwise the last segment. Null when the table is empty.
const SAnimationSegment* getAnimationSegment(const SDatabaseTables& tables, int frame);
// 0x60e28c _ZNK6glitch7collada16CColladaDatabase20linkInstanceMaterialEPNS0_17SInstanceMaterialE
void linkInstanceMaterial(SDatabaseTables& tables, SInstanceMaterial* instance);

// Index getters of the original (0x60e390..0x60e450 family): base + index * stride. They are unchecked, as in the original.
SEffect* effectAt(const SDatabaseTables& tables, int index);        // 0x60e3e4 getEffect(int)
SGeometry* geometryAt(const SDatabaseTables& tables, int index);    // 0x60e41c getGeometry(int)
SForce* forceAt(const SDatabaseTables& tables, int index);          // 0x60e450 getForce(int)
SLight* lightAt(const SDatabaseTables& tables, int index);          // 0x60e3ac getLight(int)
SCamera* cameraAt(const SDatabaseTables& tables, int index);        // 0x60e390 getCamera(int)
// 0x61ac88 getMaterial(char const*, char const*) const: the material `name` of the file `fileName` next to the database's file (the
// original builds a temporary database over that file with the default factory, looks the name up and drops the database).
// Declared only: it goes through CResFileManager::get, which is not reconstructed.
SMaterial* materialOfFile(const CColladaDatabase& database, const char* fileName, const char* name);
// Index getters of the remaining CColladaDatabase::getX(int) members (see collada_database_create.cpp for the members that are
// already written). Kept as free helpers: a member named getX would hide the free getX(tables, name) lookups that the constructX
// members call unqualified.
SAnimation* animationAt(const SDatabaseTables& tables, int index);        // 0x60e35c
SAnimationClip* animationClipAt(const SDatabaseTables& tables, int index); // 0x60e374
SImage* imageAt(const SDatabaseTables& tables, int index);                 // 0x60e3c8
SMaterial* materialAt(const SDatabaseTables& tables, int index);           // 0x60e400
SController* controllerAt(const SDatabaseTables& tables, int index);       // 0x60e434
SEmitter* emitterAt(const SDatabaseTables& tables, int index);             // 0x60e468
SGNPSEmitter* gnpsEmitterAt(const SDatabaseTables& tables, int index);     // 0x60e484
SCoronas* coronasAt(const SDatabaseTables& tables, int index);             // 0x60e4a0
SVisualScene* visualSceneAt(const SDatabaseTables& tables, int index);     // 0x60e54c

}  // namespace glitch::collada
