// Layouts of glitch::collada::CColladaDatabase and the members of the COLLADA
// resource, factory, file-manager and streaming classes, reconstructed from the
// Hex-Rays pseudocode of libDungeonHunter2.so (32-bit ARM build).
//
// Only data members are declared here; no function bodies. Every member carries
// its byte offset in the original object. Pointer fields are host-width, so
// sizeof() on a 64-bit host differs from the original; the offsets are the
// contract. Gaps that were not read by any reconstructed code are unnamed pads.
//
// This header is standalone. It defines the glitch::collada names that
// collada_database.hpp, collada_factory.hpp and collada_misc.hpp also define, so
// do not include it together with those headers.
#ifndef GLITCH_SCENE_COLLADA_GUI_LAYOUTS_COLLADA_DB_HPP
#define GLITCH_SCENE_COLLADA_GUI_LAYOUTS_COLLADA_DB_HPP

#include <cstdint>

namespace glitch {

class IDevice;

namespace io {
class IReadFile;
}  // namespace io

namespace video {
class CMaterialVertexAttributeMap;
}  // namespace video

namespace collada {

class CAnimationBlock;
class CColladaFactory;
class CResFile;
class CResFactory;

// ---------------------------------------------------------------------------
// Table entry records. Each table is `count` followed by `base`, a pointer to a
// contiguous array of entries. The stride is noted per record. The name is the
// first word of each entry (the lookups compare `*entry` with the requested
// name), except SController, whose name is at +0x04.
// ---------------------------------------------------------------------------

// Stride 0x20. Name confirmed by getAnimation(char const*).
struct SAnimation {
    const char* name;               // +0x00
    std::uint8_t pad_0x04[0x1c];    // +0x04..+0x1f
};

// Stride 0x0c. Name confirmed by getAnimationClip(char const*).
struct SAnimationClip {
    const char* name;               // +0x00
    std::uint8_t pad_0x04[0x08];    // +0x04..+0x0b
};

// Stride 0x14. Name confirmed by getImage(char const*).
struct SImage {
    const char* name;               // +0x00
    std::uint8_t pad_0x04[0x10];    // +0x04..+0x13
};

// Stride 0x74. Name confirmed by getEffect(char const*).
struct SEffect {
    const char* name;               // +0x00
    std::uint8_t pad_0x04[0x70];    // +0x04..+0x73
};

// Stride 0x24. Name confirmed by getMaterial(char const*).
struct SMaterial {
    const char* name;               // +0x00
    std::uint8_t pad_0x04[0x20];    // +0x04..+0x23
};

// Stride 0x10. Name confirmed by getGeometry(char const*).
struct SGeometry {
    const char* name;               // +0x00
    std::uint32_t reserved04;       // +0x04 (not read by reconstructed code)
    std::uint32_t notInstantiated;  // +0x08 non-zero: geometry is skipped in releaseObjects and constructGeometry
    std::uint32_t reserved0c;       // +0x0c (not read by reconstructed code)
};

// Stride 0x0c. Name at +0x04, confirmed by getController(char const*).
struct SController {
    std::uint32_t reserved00;       // +0x00
    const char* name;               // +0x04
    std::uint32_t reserved08;       // +0x08
};

// Stride 0x18. Name confirmed by getLight(char const*).
struct SLight {
    const char* name;               // +0x00
    std::uint8_t pad_0x04[0x14];    // +0x04..+0x17
};

// Stride 0x1c. Name confirmed by getCamera(char const*).
struct SCamera {
    const char* name;               // +0x00
    std::uint8_t pad_0x04[0x18];    // +0x04..+0x1b
};

// Stride 0x10. Name confirmed by getVisualScene(char const*).
struct SVisualScene {
    const char* name;               // +0x00
    std::uint8_t pad_0x04[0x0c];    // +0x04..+0x0f
};

// Stride 0x10. Name confirmed by getForce(char const*).
struct SForce {
    const char* name;               // +0x00
    std::uint8_t pad_0x04[0x0c];    // +0x04..+0x0f (kind word not confirmed here)
};

// Stride 0x24. Name confirmed by getCoronas(char const*).
struct SCoronas {
    const char* name;               // +0x00
    std::uint8_t pad_0x04[0x20];    // +0x04..+0x23
};

// Stride 0xe8. Name confirmed by getGNPSEmitter(char const*).
struct SGNPSEmitter {
    const char* name;               // +0x00
    std::uint8_t pad_0x04[0xe4];    // +0x04..+0xe7
};

// Stride 0x90. Name confirmed by getEmitter(char const*).
struct SEmitter {
    const char* name;               // +0x00
    std::uint8_t pad_0x04[0x8c];    // +0x04..+0x8f
};

// Instance material. Only the link and the vertex attribute map read by
// releaseObjects and linkInstanceMaterial are mapped.
struct SInstanceMaterial {
    std::uint8_t pad_0x00[0x34];                                   // +0x00..+0x33
    video::CMaterialVertexAttributeMap* vertexAttributeMap;        // +0x34 (intrusive_ptr, one pointer)
    SInstanceMaterial* linkNext;                                   // +0x38 instance list link
};

// Segment entry, stride 0x18. begin and end are read by getAnimationSegment as
// the half-open range [begin, end).
struct SAnimationSegment {
    std::int32_t begin;             // +0x00
    std::int32_t end;               // +0x04
    std::uint8_t pad_0x08[0x10];    // +0x08..+0x17
};

// The clip library is an 8-byte object inline in the table holder at +0x34.
// getAnimationClipLibrary returns the address of that object.
struct SLibraryAnimationClips {
    std::int32_t count;             // +0x00 (holder +0x34)
    SAnimationClip* clips;          // +0x04 (holder +0x38), stride 0x0c
};

// ---------------------------------------------------------------------------
// Table holder. The database reaches it as record->data->tables (see
// CResFile::data below), at the original *(data + 0x20). Offsets are from the
// holder start. Counts are signed (the lookups test `count <= 0`).
// ---------------------------------------------------------------------------
struct SDatabaseTables {
    std::uint32_t version;                                // +0x00 read by getVersion()
    std::uint32_t heapPtr04;                              // +0x04 pointer to a heap block freed by releaseObjects (type not confirmed)
    SInstanceMaterial* instanceMaterials;                 // +0x08 head of the instance-material list
    std::uint8_t pad_0x0c[0x18];                          // +0x0c..+0x23
    std::int32_t animationCount;                          // +0x24
    SAnimation* animations;                               // +0x28 stride 0x20
    std::uint8_t pad_0x2c[0x04];                          // +0x2c
    struct {
        std::int32_t count;                               // +0x00 of the pointee
        SAnimationSegment* segments;                      // +0x04 of the pointee, stride 0x18
    }* segments;                                          // +0x30 pointer to a separate {count, base} record
    SLibraryAnimationClips animationClipLibrary;          // +0x34..+0x3b (count +0x34, clips +0x38)
    std::int32_t cameraCount;                             // +0x3c
    SCamera* cameras;                                     // +0x40 stride 0x1c
    std::int32_t lightCount;                              // +0x44
    SLight* lights;                                       // +0x48 stride 0x18
    std::int32_t imageCount;                              // +0x4c
    SImage* images;                                       // +0x50 stride 0x14
    std::int32_t effectCount;                             // +0x54
    SEffect* effects;                                     // +0x58 stride 0x74
    std::int32_t materialCount;                           // +0x5c
    SMaterial* materials;                                 // +0x60 stride 0x24
    std::uint8_t pad_0x64[0x04];                          // +0x64..+0x67
    std::int32_t geometryCount;                           // +0x68
    SGeometry* geometries;                                // +0x6c stride 0x10
    std::int32_t controllerCount;                         // +0x70
    SController* controllers;                             // +0x74 stride 0x0c
    std::int32_t emitterCount;                            // +0x78
    SEmitter* emitters;                                   // +0x7c stride 0x90
    std::int32_t gnpsEmitterCount;                        // +0x80
    SGNPSEmitter* gnpsEmitters;                           // +0x84 stride 0xe8
    std::int32_t forceCount;                              // +0x88
    SForce* forces;                                       // +0x8c stride 0x10
    std::int32_t coronasCount;                            // +0x90
    SCoronas* coronas;                                    // +0x94 stride 0x24
    std::int32_t visualSceneCount;                        // +0x98
    SVisualScene* visualScenes;                           // +0x9c stride 0x10
    std::uint8_t pad_0xa0[0x18];                          // +0xa0..+0xb7
    // The scene object at +0xb8 (returned by getScene) is not modelled: its
    // size and fields are not confirmed.
};

// ---------------------------------------------------------------------------
// The loaded COLLADA file record. CColladaDatabase::record points here. This is
// a CResFile (constructed with new(0x50)). The inline glitch::res::File starts at
// +0x24, and its first word is the header block `data`.
// ---------------------------------------------------------------------------
class CResFile {
public:
    std::uint32_t vptr_0x00 = 0;                 // +0x00 vtable pointer (virtual slots not modelled)
    std::uint32_t refCount = 1;                  // +0x04 intrusive count
    glitch::io::IReadFile* reader = nullptr;     // +0x08 file the record was read from (refcounted)
    std::uint8_t name_0x0c[0x14] = {};           // +0x0c..+0x1f name string (inline buffer at +0x0c)
    const char* fileName = nullptr;              // +0x20 name string data; passed to unload() and matched by the streaming cache
    struct {
        std::uint8_t pad_0x00[0x08];             // +0x00..+0x07
        std::uint32_t headerSize;                // +0x08 becomes res::File::SizeOfHeader
        std::uint8_t pad_0x0c[0x04];             // +0x0c..+0x0f
        std::uint32_t offsetEntries;             // +0x10 external offset table size is headerSize + 4 * this
        std::int32_t externalFlags;              // +0x14 sign bit selects the res::File::ExternalFilePtr slot
        std::uint8_t pad_0x18[0x08];             // +0x18..+0x1f
        SDatabaseTables* tables;                 // +0x20 table holder
    }* data = nullptr;                           // +0x24 first word of the inline res::File
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

// ---------------------------------------------------------------------------
// glitch::collada::CColladaDatabase: two words. record is refcounted; factory
// is the builder that the database calls.
// ---------------------------------------------------------------------------
class CColladaDatabase {
public:
    CResFile* record = nullptr;          // +0x00
    CColladaFactory* factory = nullptr;  // +0x04
};

// ---------------------------------------------------------------------------
// glitch::collada::CColladaFactory. Its only mapped state is the vtable pointer.
// The virtual slots are listed in collada_factory.hpp.
// ---------------------------------------------------------------------------
class CColladaFactory {
public:
    std::uint32_t vptr_0x00 = 0;         // +0x00 vtable pointer
};

// ---------------------------------------------------------------------------
// glitch::collada::CResFactory. Its members are not observed in the
// reconstructed bodies (getTexture and getTextureImpl do not touch `this`).
// The object is held through a pointer in CResFileManager.
// ---------------------------------------------------------------------------
class CResFactory {
public:
    // No data members observed. Polymorphic; the vtable is not modelled.
};

// ---------------------------------------------------------------------------
// glitch::collada::CResFileReader. Its vtable pointer is at +0x00 and the
// wrapped IReadFile is at +0x04. CResFile builds one on the stack.
// ---------------------------------------------------------------------------
class CResFileReader {
public:
    std::uint32_t vptr_0x00 = 0;                 // +0x00 vtable pointer
    glitch::io::IReadFile* file = nullptr;       // +0x04 wrapped file (tell/read/seek/getSize forward to it)
};

// ---------------------------------------------------------------------------
// glitch::collada::CResFileManager, a singleton (static Inst).
// ---------------------------------------------------------------------------
class CResFileManager {
public:
    static CResFileManager* Inst;
    static CResFactory DefaultResFactory;

    std::uint32_t vptr_0x00 = 0;                 // +0x00 vtable pointer (constructed with off_983030)
    std::uint32_t refCount = 1;                  // +0x04 set to 1 by the constructor
    // std::map<std::string, CResFile*> of loaded records. The rb-tree header
    // node is at +0x08 (color +0x08, parent +0x0c, left +0x10, right +0x14), and
    // the node count is at +0x18.
    std::uint8_t files_0x08[0x14] = {};          // +0x08..+0x1b
    std::uint8_t pad_0x1c[0x04] = {};            // +0x1c..+0x1f
    glitch::IDevice* device = nullptr;           // +0x20 file system is read from device +0x34
    CResFactory* resFactory = nullptr;           // +0x24 set to &DefaultResFactory
    bool autoUnloadEnabled = true;               // +0x28 streaming/unload path of ~CColladaDatabase; get() clears it temporarily
    bool flag_0x29 = false;                      // +0x29 read by CResFactory::getTexture (role not confirmed)
    bool zipLoadingEnabled = true;               // +0x2a enables the zip path in getReadFile
    bool disableZipAfterFirstUse = true;         // +0x2b when set, getReadFile clears zipLoadingEnabled after one use

    // 0x658c90 _ZN6glitch7collada15CResFileManager15postLoadProcessEPNS0_8CResFileEPNS_2io9IReadFileE
    // Partial: see src/tb_scene-collada-gui-glitch_collada_CResFileManager-0.cpp.
    int postLoadProcess(CResFile* file, glitch::io::IReadFile* reader);
};

// ---------------------------------------------------------------------------
// glitch::collada::CAnimationStreamingManager, a singleton (static Instance). Its
// constructor zeroes the eight words that follow.
// ---------------------------------------------------------------------------
class CAnimationStreamingManager {
public:
    // Cache entry, 12 bytes. Stored in the cache vector at +0x0c..+0x14.
    struct SegmentCacheEntry {
        std::uint32_t size;              // +0x00 bytes charged to cacheUsage
        std::int32_t* refCount;          // +0x04 refcount word of the cached block data (evicted when it is 1)
        const char* fileName;            // +0x08 owning record's fileName, matched by release()
    };

    static CAnimationStreamingManager* Instance;

    CAnimationBlock** registryBegin = nullptr;        // +0x00 sorted registry of blocks
    CAnimationBlock** registryEnd = nullptr;          // +0x04
    CAnimationBlock** registryCapacityEnd = nullptr;  // +0x08
    SegmentCacheEntry* cacheBegin = nullptr;          // +0x0c
    SegmentCacheEntry* cacheEnd = nullptr;            // +0x10
    SegmentCacheEntry* cacheCapacityEnd = nullptr;    // +0x14
    std::int32_t cacheLimit = 0;                      // +0x18 maximum cache bytes; caching is off when not positive
    std::int32_t cacheUsage = 0;                      // +0x1c bytes currently cached
};

}  // namespace collada
}  // namespace glitch

// Static data members declared above are defined in the original translation
// units, not here.

#endif  // GLITCH_SCENE_COLLADA_GUI_LAYOUTS_COLLADA_DB_HPP
