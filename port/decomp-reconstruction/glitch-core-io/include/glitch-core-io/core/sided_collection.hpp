// glitch::core::detail::SIDedCollection: the id-keyed collection behind the texture, shader,
// render-target-type and shader-parameter managers. One template serves every instantiation in the
// queue; each instance is ledgered against the member functions of this header.
//
// Layout of the original, 32-bit offsets (kept as comments, never used for pointer arithmetic):
//   +0x00..+0x10  name -> SIdValue map (the rb-tree header; its node count is at +0x10)
//   +0x18..+0x20  vector<SEntry> (begin, end, end of storage); an SEntry is the value plus its
//                 map iterator, 8 bytes for a pointer value and 20 bytes for SShaderParameterDef
//   +0x24         next free id (uint16)
//   +0x26         number of live entries (uint16)
// The map is std::map with the same key order (strcmp on the names), so the iteration order
// of idBegin/idEnd and removeAll matches. The rb-tree node layout is not copied; the id stored in a
// node is at the same place in the mapped value (SIdValue::id).
//
// Template arguments: T is the stored value (intrusive_ptr or plain struct), IdT is the id type
// (unsigned short in every instance), Flag is the third argument (false in every instance; its use
// is not recovered), Props is the per-entry property record stored in the map value, Traits gives
// isSet and useCount for a value.
// Address map: each queued body of this template, with its instance in parentheses.
// The status of each row is in ledger.csv.
//   ::size(                            0x5b9d8c(SShaderParameterDef), 0x5d7d78(CMaterialRenderer), 0x5e53d4(IShader), 0x5e83a4(ITexture), 0x6e07c0(IShaderCode)
//   ::idBegin(                         0x5b9d94(SShaderParameterDef), 0x5d7d34(CMaterialRenderer), 0x5e5390(IShader), 0x5e8398(ITexture), 0x6e0888(IShaderCode)
//   ::idEnd(                           0x5b9d9c(SShaderParameterDef), 0x5d7d3c(CMaterialRenderer), 0x5e5398(IShader), 0x5e83a0(ITexture), 0x6e0890(IShaderCode)
//   ::get(unsigned short) const        0x5b9da0(SShaderParameterDef), 0x5d7d40(CMaterialRenderer), 0x5e539c(IShader), 0x5e83ac(ITexture), 0x6e0788(IShaderCode)
//   SIDedCollection()                  0x5b9e1c(SShaderParameterDef), 0x5b9ec8(SShaderParameterDef), 0x5d7db0(CMaterialRenderer), 0x5d7de4(CMaterialRenderer), 0x5e5404(IShader), 0x5e5438(IShader), 0x5e840c(ITexture), 0x5e8440(ITexture), 0x6e07d0(IShaderCode), 0x6e082c(IShaderCode)
//   ::getNextId(                       0x5b9efc(SShaderParameterDef), 0x5d7e18(CMaterialRenderer), 0x5e546c(IShader), 0x5e8474(ITexture), 0x6e07c8(IShaderCode)
//   ::getProperties(                   0x5b9f04(SShaderParameterDef), 0x5b9f1c(SShaderParameterDef), 0x5d7d9c(CMaterialRenderer), 0x5d7e20(CMaterialRenderer), 0x5e53dc(IShader), 0x5e53f0(IShader), 0x5e83e4(ITexture), 0x5e83f8(ITexture), 0x6e0860(IShaderCode), 0x6e0874(IShaderCode)
//   ::getMaxID(                        0x5b9f34(SShaderParameterDef), 0x5d7d80(CMaterialRenderer), 0x5e5474(IShader), 0x5e847c(ITexture), 0x6e0894(IShaderCode)
//   SIdValue::SIdValue(                0x5b9f64(SShaderParameterDef), 0x5b9f74(SShaderParameterDef), 0x5d7e34(CMaterialRenderer), 0x5d7e54(CMaterialRenderer), 0x5e5490(IShader), 0x5e54a0(IShader), 0x5e85b4(ITexture), 0x5e85f0(ITexture), 0x6e08b0(IShaderCode), 0x6e08b8(IShaderCode)
//   SName::SName(                      0x5b9f84(SShaderParameterDef), 0x5b9f94(SShaderParameterDef), 0x5d7e74(CMaterialRenderer), 0x5d7e84(CMaterialRenderer), 0x5e54b0(IShader), 0x5e54c0(IShader), 0x5e8498(ITexture), 0x5e84a8(ITexture), 0x6e08c0(IShaderCode), 0x6e08d0(IShaderCode)
//   SName::setManaged(                 0x5b9fa4(SShaderParameterDef), 0x5d7e94(CMaterialRenderer), 0x5e54d0(IShader), 0x5e84b8(ITexture), 0x6e08e0(IShaderCode)
//   SName::get(                        0x5b9fb0(SShaderParameterDef), 0x5d7ea0(CMaterialRenderer), 0x5e54dc(IShader), 0x5e84c4(ITexture), 0x6e08ec(IShaderCode)
//   SEntry::SEntry()                   0x5b9fb8(SShaderParameterDef), 0x5b9fe4(SShaderParameterDef), 0x5d7ea8(CMaterialRenderer), 0x5d7eb8(CMaterialRenderer), 0x5e54e4(IShader), 0x5e54f4(IShader), 0x5e84cc(ITexture), 0x5e84dc(ITexture), 0x6e08f4(IShaderCode), 0x6e0904(IShaderCode)
//   SEntry::SEntry(                    0x5ba010(SShaderParameterDef), 0x5ba064(SShaderParameterDef), 0x5d7ec8(CMaterialRenderer), 0x5d7ef4(CMaterialRenderer), 0x5e5504(IShader), 0x5e5530(IShader), 0x5e84ec(ITexture), 0x5e8518(ITexture), 0x6e0914(IShaderCode), 0x6e0940(IShaderCode)
//   SName::operator<                   0x5ba7b0(SShaderParameterDef), 0x5d84f0(CMaterialRenderer), 0x5e555c(IShader), 0x5e8544(ITexture), 0x6e096c(IShaderCode)
//   SName::~SName(                     0x5ba7c8(SShaderParameterDef), 0x5ba7f4(SShaderParameterDef), 0x5d8578(CMaterialRenderer), 0x5d85a4(CMaterialRenderer), 0x5e5574(IShader), 0x5e55a0(IShader), 0x5e855c(ITexture), 0x5e8588(ITexture), 0x6e0984(IShaderCode), 0x6e09b0(IShaderCode)
//   ::getId(                           0x5bb378(SShaderParameterDef), 0x5d85d0(IShader), 0x5d98c4(CMaterialRenderer), 0x5e8ed8(ITexture), 0x6e1ab4(IShaderCode)
//   ::~SIDedCollection(                0x5bb520(SShaderParameterDef), 0x5d9ab0(CMaterialRenderer), 0x5e5eac(IShader), 0x5e97a0(ITexture), 0x6e1718(IShaderCode)
//   SEntry::reset(                     0x5bc5c4(SShaderParameterDef), 0x5d8098(CMaterialRenderer), 0x5e60e4(IShader), 0x5e9e50(ITexture), 0x6e169c(IShaderCode)
//   SEntry::set(                       0x5bc620(SShaderParameterDef), 0x5d80d8(CMaterialRenderer), 0x5e60a4(IShader), 0x5e9e10(ITexture), 0x6e165c(IShaderCode)
//   ::removeAll(bool)                  0x5c1104(SShaderParameterDef), 0x5da080(CMaterialRenderer), 0x5e66d8(IShader), 0x5e9f78(ITexture), 0x6e1f50(IShaderCode)
//   ::removeAll()                      0x5c11cc(SShaderParameterDef), 0x5da148(CMaterialRenderer), 0x5e67a0(IShader), 0x5ea040(ITexture), 0x6e2018(IShaderCode)
//   ::removeUnused()                   0x5c11d4(SShaderParameterDef), 0x5da150(CMaterialRenderer), 0x5e67a8(IShader), 0x5ea048(ITexture), 0x6e2020(IShaderCode)
//   ::remove(unsigned short)           0x5c1264(SShaderParameterDef), 0x5da158(CMaterialRenderer), 0x5e67bc(IShader), 0x5ea220(ITexture), 0x6e2054(IShaderCode)
//   ::remove(unsigned short, bool)     0x5d9f74(CMaterialRenderer), 0x5e65dc(IShader), 0x5e9e7c(ITexture), 0x6e1e54(IShaderCode)
//
#pragma once

#include <cstdint>
#include <cstring>
#include <iterator>
#include <map>
#include <vector>

namespace glitch::core::detail {

namespace sidedcollection {

// Value traits for boost::intrusive_ptr values: a value is set when the pointer is not null,
// and its use count is the reference count at +0x04.
struct SValueTraits {
    template <class P>
    static bool isSet(const P& p) { return static_cast<bool>(p); }
    template <class P>
    static int useCount(const P& p) { return p->referenceCounter; }
};

// Property record used by the collections without per-entry properties.
struct SEmptyProperties {};

}  // namespace sidedcollection

template <class T, class IdT, bool Flag, class Props, class Traits>
class SIDedCollection {
public:
    // Name key of the map. The name is a C string; a managed name is owned by the key and
    // released with delete[] (SName::~SName, 0x5e855c). 8 bytes: name at +0x00, managed at +0x04.
    struct SName {
        explicit SName(const char* n) : name(n), managed(false) {}  // 0x5e8498
        // Copies of a managed name copy the characters, so every key owns its own buffer.
        SName(const SName& other) : name(other.name), managed(false) {
            if (other.managed && other.name) {
                char* copy = new char[std::strlen(other.name) + 1];
                std::strcpy(copy, other.name);
                name = copy;
                managed = true;
            }
        }
        SName& operator=(const SName&) = delete;
        ~SName() {  // 0x5e855c
            if (managed && name) delete[] name;
        }
        void setManaged() const { managed = true; }                // 0x5e84b8
        const char* get() const { return name; }                  // 0x5e84c4
        bool operator<(const SName& other) const {                 // 0x5e8544: strcmp(a, b) < 0
            return std::strcmp(name, other.name) < 0;
        }

        const char* name;             // +0x00
        mutable bool managed;         // +0x04: the key owns the characters when set
    };

    // Mapped value of the name map: the id, plus the per-entry properties. The original stores a
    // string (16-byte block) before the id at +0x1c; it is not used by the functions here.
    struct SIdValue {
        explicit SIdValue(IdT value) : id(value) {}  // 0x5e85b4 (C1 and C2)
        IdT id;      // +0x1c (within the map node value; node value starts at +0x18)
        Props properties{};
    };

    using Map = std::map<SName, SIdValue>;
    using MapIterator = typename Map::iterator;

    // Vector element: the value and its map iterator. 8 bytes for a pointer value.
    struct SEntry {
        SEntry() = default;                                          // 0x5e84cc
        SEntry(const T& v, MapIterator it) : value(v), iterator(it) {}  // 0x5e84ec
        // 0x5e9e10: sets the value (the previous one is released) and the iterator.
        void set(const T& v, MapIterator it) {
            value = v;
            iterator = it;
        }
        // 0x5e9e50: clears the value (released) and the iterator.
        void reset() {
            value = T{};
            iterator = MapIterator{};
        }

        T value{};              // +0x00
        MapIterator iterator{};  // +0x04
    };

    SIDedCollection() = default;  // 0x5e840c (C1, C2)
    ~SIDedCollection() {          // 0x5e97a0 (D2): releases the entries, then empties the map
        entries.clear();
        ids.clear();
        count = 0;
    }
    SIDedCollection(const SIDedCollection&) = delete;
    SIDedCollection& operator=(const SIDedCollection&) = delete;

    // Map iteration over the names (idBegin is the leftmost node, idEnd the header).
    typename Map::const_iterator idBegin() const { return ids.begin(); }  // 0x5e8398
    typename Map::const_iterator idEnd() const { return ids.end(); }      // 0x5e83a0
    std::uint16_t size() const { return count; }                          // 0x5e83a4

    // The entry at id, or the invalid entry when id is out of range (0x5e83ac).
    const SEntry* get(std::uint16_t id) const {
        if (id < entries.size()) return &entries[id];
        return &invalidEntry;
    }
    SEntry* get(std::uint16_t id) {
        if (id < entries.size()) return &entries[id];
        return &invalidEntry;
    }

    // Properties of the entry (0x5e83e4, 0x5e83f8): the record stored with its name.
    const Props* getProperties(std::uint16_t id) const { return &get(id)->iterator->second.properties; }
    Props* getProperties(std::uint16_t id) { return &get(id)->iterator->second.properties; }

    std::uint16_t getNextId() const { return nextId; }  // 0x5e8474
    // The highest id in use: one less than the vector length, truncated to 16 bits (0x5e847c).
    std::uint32_t getMaxID() const { return static_cast<std::uint32_t>(entries.size() - 1) & 0xffffu; }

    // Id of the entry with this name, or 0xffff (0x5e8ed8). The lookup key is a temporary name
    // that is not managed.
    std::uint16_t getId(const char* name) const {
        const auto it = ids.find(SName(name));
        if (it == ids.end()) return 0xffff;
        return it->second.id;
    }

    // Removes the entry at id when it is set and either only the collection uses the value or
    // force is true (0x5e9e7c). The entry is reset, the next free id moves down to id when that
    // is lower, the count drops, and the trailing unset entries are cut from the vector.
    // Partial: the trailing trim is read from the ARM loop; the released temporary is dropped
    // through assignment here.
    bool remove(std::uint16_t id, bool force) {
        if (id >= entries.size()) return false;
        SEntry& entry = entries[id];
        if (!Traits::isSet(entry.value)) return false;
        if (Traits::useCount(entry.value) != 1 && !force) return false;
        ids.erase(entry.iterator);
        entry.reset();
        if (id < nextId) nextId = id;
        --count;
        while (!entries.empty() && !Traits::isSet(entries.back().value)) entries.pop_back();
        return true;
    }

    // Removes every entry for which remove(id, force) succeeds, visiting the names in key order
    // (0x5e9f78). The successor is taken before each removal, as the original does. Returns the
    // number removed (short in the original).
    std::int16_t removeAll(bool force) {
        std::int16_t removed = 0;
        auto it = ids.begin();
        while (it != ids.end()) {
            const auto next = std::next(it);
            if (remove(it->second.id, force)) ++removed;
            it = next;
        }
        return removed;
    }

    void removeAll() { removeAll(false); }                    // 0x5ea040
    void removeUnused() { removeAll(false); }                 // 0x5ea048
    void remove(std::uint16_t id) { remove(id, false); }      // 0x5ea220

    // Test helper (not a recovered function): appends a value under name with the next free id,
    // the way the managers fill the collection. Returns the id, or 0xffff when the name exists.
    std::uint16_t add(const char* name, const T& value) {
        if (ids.find(SName(name)) != ids.end()) return 0xffff;
        const std::uint16_t id = nextId;
        char* owned = new char[std::strlen(name) + 1];
        std::strcpy(owned, name);
        SName key(owned);
        key.setManaged();
        const auto it = ids.emplace(key, SIdValue(id)).first;
        if (id >= entries.size()) entries.resize(static_cast<std::size_t>(id) + 1);
        entries[id].set(value, it);
        ++count;
        nextId = static_cast<std::uint16_t>(id + 1);
        while (nextId < entries.size() && Traits::isSet(entries[nextId].value)) ++nextId;
        return id;
    }

    // Entry table, name map, next free id and count, at the original offsets (see above).
    std::vector<SEntry> entries;  // +0x18..+0x20
    Map ids;                      // +0x00..+0x10
    std::uint16_t nextId = 0;     // +0x24
    std::uint16_t count = 0;      // +0x26

private:
    SEntry invalidEntry{};  // the original's shared "Invalid" entry
};

}  // namespace glitch::core::detail
