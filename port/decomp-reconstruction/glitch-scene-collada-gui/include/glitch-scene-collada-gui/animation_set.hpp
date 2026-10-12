// glitch::collada::CAnimationSet accessors. Layout is inferred from the
// offsets the accessors use; ranges are modelled as begin/end pointers to
// 32-bit entries, so no raw offset arithmetic is needed.
#pragma once

#include <cstdint>

namespace glitch::collada {

struct CColladaDatabase;
struct CSceneNodeAnimationTemplate;

// One entry of the database table (8 bytes in the original).
struct SAnimationDatabaseRef {
    CColladaDatabase* database;  // +0x00
    std::uint32_t reserved;      // +0x04
};

// One entry of the binding table (12 bytes in the original).
struct SAnimationBinding {
    std::uint32_t words[3];
};

struct CAnimationSet {
    std::uint32_t* channels = nullptr;              // +0x0c
    std::uint32_t* tracks = nullptr;                // +0x18
    SAnimationDatabaseRef* databases = nullptr;     // +0x24
    SAnimationBinding* bindings = nullptr;          // +0x30
    std::uint32_t animatedTrackCount = 0;           // +0x3c
    std::uint32_t* starts_begin = nullptr;          // +0x40
    std::uint32_t* starts_end = nullptr;            // +0x44
    std::uint32_t* ends_begin = nullptr;            // +0x4c
    std::uint32_t* ends_end = nullptr;              // +0x50
    std::uint32_t* lengths_begin = nullptr;         // +0x58
    std::uint32_t* lengths_end = nullptr;           // +0x5c
    CSceneNodeAnimationTemplate* animTemplate = nullptr;  // +0x64
};

// 0x65f004 _ZN6glitch7collada13CAnimationSet12remAnimationEPKNS0_10SAnimationE
int remAnimation(CAnimationSet* self, const void* animation);
// 0x65f00c _ZN6glitch7collada13CAnimationSet10getChannelEj
std::uint32_t getChannel(CAnimationSet* self, unsigned index);
// 0x65f018 _ZNK6glitch7collada13CAnimationSet10getChannelEj
std::uint32_t getChannel(const CAnimationSet* self, unsigned index);
// 0x65f038 _ZN6glitch7collada13CAnimationSet19getAnimationTrackExEi
std::uint32_t getAnimationTrackEx(const CAnimationSet* self, int index);
// 0x65f058 _ZNK6glitch7collada13CAnimationSet21getAnimatedTrackCountEv
std::uint32_t getAnimatedTrackCount(const CAnimationSet* self);
// 0x65f060 _ZNK6glitch7collada13CAnimationSet18getAnimationLengthEj
std::uint32_t getAnimationLength(const CAnimationSet* self, unsigned index);
// 0x65f07c _ZNK6glitch7collada13CAnimationSet17getAnimationStartEj
std::uint32_t getAnimationStart(const CAnimationSet* self, unsigned index);
// 0x65f098 _ZNK6glitch7collada13CAnimationSet15getAnimationEndEj
std::uint32_t getAnimationEnd(const CAnimationSet* self, unsigned index);
// 0x65f0b4 _ZN6glitch7collada13CAnimationSet11getDatabaseEi
SAnimationDatabaseRef* getDatabase(CAnimationSet* self, int index);
// 0x65f0c0 _ZNK6glitch7collada13CAnimationSet10getBindingEi
const SAnimationBinding* getBinding(const CAnimationSet* self, int index);
// 0x65f0d0 _ZN6glitch7collada13CAnimationSet19remAnimationLibraryERNS0_16CColladaDatabaseE
void remAnimationLibrary(CAnimationSet* self, CColladaDatabase& database);
// 0x65f0d4 _ZN6glitch7collada13CAnimationSet19remAnimationLibraryEi
void remAnimationLibrary(CAnimationSet* self, int index);

}  // namespace glitch::collada
