// Classes of the glitch::collada interfaces whose only reconstructed body is an
// empty base-object destructor (D1; the complete-object D2 is the same empty
// body when both are present). The original classes are polymorphic; these
// stand-ins carry no vtable, so the destructor is a plain defaulted member.
// The deleting destructors (D0) are compiler-generated and are not listed here.
#pragma once

namespace glitch::collada {

class IEventsManager {
public:
    // 0x60e028 _ZN6glitch7collada14IEventsManagerD1Ev (empty)
    ~IEventsManager() = default;
};

// One key slot of the event table: the event names that fire at that key.
struct SEventEntry {
    int nameCount;             // +0x00 (8-byte record in the original)
    const char* const* names;  // +0x04 array of nameCount event names
};

// Event key table, held at CEventsManager +0x14.
// keyKind: 1 = uint8 keys, 3 = uint16 keys (30 per second, converted with 33.333 ms),
// 4 = int32 keys in milliseconds.
struct SEventKeyTable {
    int keyKind;           // +0x00
    int reserved04;        // +0x04 not read by the CEventsManager code in this batch
    int keyCount;          // +0x08 bound of findEntry
    const void* keys;      // +0x0c key array, element width given by keyKind
    int entryCount;        // +0x10 number of SEventEntry records
    SEventEntry* entries;  // +0x14
};

// Arguments passed to the event callback (the original builds a 3-word stack array; only
// the first two words are written).
struct SEventCallArgs {
    int timeOffset;            // time since the key of the fired event
    const char* eventName;     // event name from the entry
};

class CEventsManager {
public:
    // Event callback, called as callback(args, userData) (the original loads it from +0x08).
    using EventCallback = void (*)(const SEventCallArgs* args, void* userData);

    // 0x60edd0 _ZN6glitch7collada14CEventsManagerD1Ev (empty)
    ~CEventsManager() = default;

    // 0x60e02c _ZN6glitch7collada14CEventsManager9findEntryEi
    int findEntry(int time);
    // 0x60e934, 0x60ea08, 0x60eae4 dispatchEventsEx<Key, Scale>, specialised below
    template <typename Key, int Scale>
    void dispatchEventsEx(int firstEntry, int lastEntry, int time);
    // 0x60ebb4 _ZN6glitch7collada14CEventsManager14dispatchEventsEiii
    void dispatchEvents(int firstEntry, int lastEntry, int time);
    // 0x60ebe0 _ZN6glitch7collada14CEventsManager8onUpdateEiiii
    void onUpdate(int previousTime, int currentTime, int loopStart, int loopEnd);
    // 0x60ecb4 _ZN6glitch7collada14CEventsManager8onUpdateEii
    void onUpdate(int previousTime, int currentTime);
    // 0x60fcf8, 0x60fd9c, 0x60fe48 getEventTimeFromEventNameEx<Key, Scale>, specialised below
    template <typename Key, int Scale>
    int getEventTimeFromEventNameEx(const char* eventName);
    // 0x60feec _ZN6glitch7collada14CEventsManager25getEventTimeFromEventNameEPKc
    int getEventTimeFromEventName(const char* eventName);

    // +0x04 reference count (IReferenceCounted counter, grabbed and dropped by onUpdate)
    int referenceCounter;
    // +0x08 event callback (nonzero means events are enabled)
    EventCallback callback;
    // +0x0c user data passed to the callback
    void* callbackUserData;
    // +0x10 entry index of the last update, used to skip a repeated entry
    int lastEntry;
    // +0x14 event key table
    SEventKeyTable* keyTable;
};

template <>
void CEventsManager::dispatchEventsEx<int, 1000>(int firstEntry, int lastEntry, int time);
template <>
void CEventsManager::dispatchEventsEx<unsigned short, 30>(int firstEntry, int lastEntry, int time);
template <>
void CEventsManager::dispatchEventsEx<unsigned char, 30>(int firstEntry, int lastEntry, int time);
template <>
int CEventsManager::getEventTimeFromEventNameEx<int, 1000>(const char* eventName);
template <>
int CEventsManager::getEventTimeFromEventNameEx<unsigned short, 30>(const char* eventName);
template <>
int CEventsManager::getEventTimeFromEventNameEx<unsigned char, 30>(const char* eventName);

class IAnimationTrackEx {
public:
    // 0x60def4 _ZN6glitch7collada17IAnimationTrackExD1Ev (empty)
    ~IAnimationTrackEx() = default;
};

class IColladaSceneNodeSynchronizedAnimatorInterface {
public:
    // 0x660dac _ZN6glitch7collada46IColladaSceneNodeSynchronizedAnimatorInterfaceD1Ev (empty)
    ~IColladaSceneNodeSynchronizedAnimatorInterface() = default;
};

class CResFileReader {
public:
    // 0x657888 _ZN6glitch7collada14CResFileReaderD1Ev (empty)
    ~CResFileReader() = default;
};

class CResFactory {
public:
    // 0x65788c _ZN6glitch7collada11CResFactoryD2Ev (empty, same as D1)
    // 0x657890 _ZN6glitch7collada11CResFactoryD1Ev (empty)
    ~CResFactory() = default;
};

struct SAnimationAccessor;

class CAnimationTrackEx {
public:
    // 0x6e29b0 _ZN6glitch7collada17CAnimationTrackExD1Ev (empty)
    ~CAnimationTrackEx() = default;

    // 0x6e2c40 _ZNK6glitch7collada17CAnimationTrackEx8getValueERKNS0_18SAnimationAccessorEiPvRib
    // Keyed or non-keyed value read. Declared in tb_scene-collada-gui-glitch_collada_CAnimationTrackEx-0.cpp.
    int getValue(const SAnimationAccessor& accessor, int index, void* out, int& keyIndex, bool flag) const;
};

namespace animation_track {

class CApplicatorInfo {
public:
    // 0x667c34 _ZN6glitch7collada15animation_track15CApplicatorInfoD1Ev (empty)
    ~CApplicatorInfo() = default;
};

class CMaterialApplicatorInfo {
public:
    // 0x667ccc _ZN6glitch7collada15animation_track23CMaterialApplicatorInfoD1Ev (empty)
    ~CMaterialApplicatorInfo() = default;
};

}  // namespace animation_track

}  // namespace glitch::collada
