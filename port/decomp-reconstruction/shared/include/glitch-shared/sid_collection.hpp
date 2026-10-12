// glitch::core::detail::SIDedCollection, layout recovered from the binary (ARM, 32-bit offsets).
// Shared header for the SIDedCollection instances (texture, shader, shader-code, material-renderer
// and shader-parameter managers). Offsets are kept as comments and are never used for pointer
// arithmetic. Evidence: the ITexture instance (insert 0x5ea53c, rename 0x5e9348, remove 0x5e9e7c,
// removeAll(bool) 0x5e9f78, getId 0x5e8ed8, ctor 0x5e840c, dtor 0x5e97a0) and the element-size and
// property checks in the other instances (listed below).
//
// Collection (this):
//   +0x00  name map header (rb-tree header node): color byte (cleared), parent (0), left = leftmost
//          node at +0x08 (idBegin, 0x5e8398), right = rightmost at +0x0c. The header address is the
//          end iterator (idEnd, 0x5e83a0 returns this). The ctor (0x5e840c) points left and right at
//          the header itself.
//   +0x10  name map node count (0 in the ctor; not read by any recovered function)
//   +0x18  entry vector begin, +0x1c end, +0x20 end of storage (get 0x5e83ac, size, dtor)
//   +0x24  next free id (uint16)          (getNextId 0x5e8474)
//   +0x26  live count (uint16)            (size 0x5e83a4)
//
// Entry (vector element), the value followed by its name-map iterator (node pointer):
//   pointer values (ITexture, IShader, IShaderCode, CMaterialRenderer): stride 8 (get: LSL #3 at
//     0x5e83ac, 0x5d7d40, 0x5e539c, 0x6e0788), value +0x00, iterator +0x04. The count check uses
//     ASR #3 on the byte distance.
//   SShaderParameterDef: stride 20 (get 0x5b9da0: MLA with 0x14). Value is a shared-string data
//     pointer at +0x00 (count at data+0x00, released by SSharedStringHeapEntry::SData::release),
//     u16 at +0x04, bytes at +0x06 and +0x07, ints at +0x08 and +0x0c, iterator at +0x10 (SEntry
//     ctor 0x5ba010, SEntry() 0x5b9fb8, reset 0x5bc5c4, set 0x5bc620). Not covered by the template
//     below (see the open questions).
//
// Name map node (rb-tree node, 0x38 bytes):
//   +0x00..+0x0f  rb base (color, parent, left, right)
//   +0x10         key SName: name char* at +0x10, managed byte at +0x14 (SName ctor 0x5e8498,
//                 setManaged 0x5e84b8, get 0x5e84c4, operator< 0x5e8544 = strcmp(a, b) < 0,
//                 dtor 0x5e855c = delete[] the name when managed)
//   +0x18         mapped SIdValue (0x20 bytes, ctor 0x5e85b4):
//                   SIdValue+0x00..+0x0f  inline buffer of an STLport-style std::string
//                   SIdValue+0x10         string start pointer (written as SIdValue+0x00 for the empty
//                                         string; _M_allocate_block(16) does nothing)
//                   SIdValue+0x14         string finish pointer
//                   SIdValue+0x18         int 39 (0x27) in every ctor and in insert/rename (meaning not
//                                         recovered)
//                   SIdValue+0x1c         id (uint16)  = node +0x34 (getId reads it at 0x5e8f18)
//   getProperties(id) returns node +0x18, that is the SIdValue address, so the caller's property
//   record starts at SIdValue+0x00 and overlaps the string header (see the open questions).
//
// Functions (ITexture instance addresses; the other instances use the same shape):
//   size 0x5e83a4, idBegin 0x5e8398, idEnd 0x5e83a0, get(id) 0x5e83ac (invalid entry when out of
//   range), getProperties(id) 0x5e83e4 (no range check), getNextId 0x5e8474, getMaxID 0x5e847c
//   (vector length - 1, truncated to 16 bits; 0xffff when the vector is empty), getId(name) 0x5e8ed8
//   (find; the key is an unmanaged temporary name; 0xffff when not found).
//   insert(name, value, managed) 0x5ea53c: see insert() below.
//   rename(id, name, managed) 0x5e9348: see rename() below.
//   remove(id, force) 0x5e9e7c: see remove() below.
//   removeAll(force) 0x5e9f78, removeAll() 0x5e67a0 (IShader), removeUnused() 0x5e67a8, remove(id)
//   0x5e67bc: wrappers with force = false.
//
// Template arguments: T is the stored value (a pointer to a reference-counted object), Props is the
// record handed out by getProperties, Traits gives isSet, useCount, addRef and release for a value.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <map>
#include <vector>

namespace glitch::core::detail::sid_layout {

// Value traits for a pointer to an IReferenceCounted-like object: the count is the
// referenceCounter field (+0x04 in the original), and release is drop() (deletes at zero).
struct IntrusiveTraits {
    template <class P>
    static bool isSet(const P& p) { return static_cast<bool>(p); }
    template <class P>
    static int useCount(const P& p) { return p->referenceCounter; }
    template <class P>
    static void addRef(const P& p) {
        if (p) ++p->referenceCounter;
    }
    template <class P>
    static void release(const P& p) {
        if (p) p->drop();
    }
};

template <class T, class Props, class Traits = IntrusiveTraits>
class SIDedCollection {
public:
    // The record handed out by getProperties sits at SIdValue+0x00; the id is at +0x1c. The string
    // header of the original occupies SIdValue+0x00..+0x1b, so Props must fit there.
    static_assert(sizeof(Props) < 0x1c, "the properties record must fit before the id at +0x1c");

    // Name key. The rb node copy is bitwise (managed name owned by the node, released by the dtor).
    struct SName {
        explicit SName(const char* n) : name(n), managed(false) {}  // 0x5e8498
        ~SName() {                                                 // 0x5e855c
            if (managed && name) delete[] name;
        }
        void setManaged() const { managed = true; }                // 0x5e84b8
        const char* get() const { return name; }                   // 0x5e84c4
        bool operator<(const SName& other) const {                 // 0x5e8544
            return std::strcmp(name, other.name) < 0;
        }

        const char* name;             // +0x00 (node +0x10)
        mutable bool managed;         // +0x04 (node +0x14)
    };

    // Mapped value. Props occupies the string header area; id is at +0x1c (0x5e85b4 writes it).
    struct SIdValue {
        SIdValue() = default;
        explicit SIdValue(std::uint16_t value) : id(value) {}
        Props properties{};                                  // +0x00
        std::uint8_t header[0x1c - sizeof(Props)]{};         // +sizeof(Props)..+0x1c, string header
        std::uint16_t id = 0;                                // +0x1c
    };

    using Map = std::map<SName, SIdValue>;

    // Vector element: the value (+0x00) and its name-map node (+0x04). Pointer values only.
    struct SEntry {
        // 0x5e9e10: adds a reference to the new value, stores it, releases the old, stores the node.
        void set(const T& v, typename Map::iterator it) {
            Traits::addRef(v);
            T old = value;
            value = v;
            Traits::release(old);
            iterator = it;
        }
        // 0x5e9e50: clears the value (released) and the node.
        void reset() {
            T old = value;
            value = T{};
            Traits::release(old);
            iterator = typename Map::iterator{};
        }

        T value{};                           // +0x00
        typename Map::iterator iterator{};   // +0x04
    };

    SIDedCollection() = default;  // 0x5e840c: count 0, next id 0, empty map and vector

    // 0x5e97a0: the vector is destroyed first (each value is released), then the map is cleared.
    ~SIDedCollection() {
        for (auto& entry : entries) Traits::release(entry.value);
        entries.clear();
        ids.clear();
        count = 0;
    }

    SIDedCollection(const SIDedCollection&) = delete;
    SIDedCollection& operator=(const SIDedCollection&) = delete;

    // Names in key order (strcmp); idBegin is the leftmost node, idEnd the header.
    typename Map::const_iterator idBegin() const { return ids.begin(); }  // 0x5e8398
    typename Map::const_iterator idEnd() const { return ids.end(); }      // 0x5e83a0
    std::uint16_t size() const { return count; }                          // 0x5e83a4

    // The entry at id, or the shared invalid entry when id is out of range (0x5e83ac).
    SEntry* get(std::uint16_t id) {
        if (id < entries.size()) return &entries[id];
        return &invalidEntry;
    }
    const SEntry* get(std::uint16_t id) const {
        if (id < entries.size()) return &entries[id];
        return &invalidEntry;
    }

    // Record of the entry (0x5e83e4): no range check in the original.
    Props* getProperties(std::uint16_t id) { return &entries[id].iterator->second.properties; }

    std::uint16_t getNextId() const { return nextId; }  // 0x5e8474

    // Vector length - 1, truncated to 16 bits (0x5e847c). 0xffff for an empty vector.
    std::uint16_t getMaxID() const { return static_cast<std::uint16_t>(entries.size() - 1); }

    // Id of the name, or 0xffff (0x5e8ed8).
    std::uint16_t getId(const char* name) const {
        const auto it = ids.find(SName(name));
        if (it == ids.end()) return 0xffff;
        return it->second.id;
    }

    // 0x5ea53c. Counts the entry first (+0x26), takes id = next free id (+0x24), inserts the name
    // (the original uses insert_unique and does not test its bool: an existing name is re-linked
    // to the new value; see the open questions), sets the managed flag when asked, stores the
    // value at entries[id] (replacing a slot below the vector length, or appending), then moves
    // next free id up past the set entries. Returns id.
    std::uint16_t insert(const char* name, const T& value, bool managed) {
        ++count;
        const std::uint16_t id = nextId;
        const auto result = ids.emplace(SName(name), SIdValue(id));
        const auto it = result.first;
        if (managed) it->first.setManaged();
        if (id < entries.size()) {
            entries[id].set(value, it);
        } else {
            SEntry entry;
            entry.set(value, it);
            entries.push_back(entry);
        }
        std::size_t next = id;
        do {
            ++next;
            nextId = static_cast<std::uint16_t>(next);
        } while (next < entries.size() && Traits::isSet(entries[next].value));
        return id;
    }

    // 0x5e9348. Fails (false) when the id is out of range or unset, or when the new name already
    // exists. Otherwise the new name is linked to the same id, the old node is erased, the managed
    // flag is set on the new key when asked, and the entry points at the new node. Count and next
    // free id are unchanged.
    bool rename(std::uint16_t id, const char* name, bool managed) {
        if (id >= entries.size()) return false;
        SEntry& entry = entries[id];
        if (!Traits::isSet(entry.value)) return false;
        const auto result = ids.emplace(SName(name), SIdValue(id));
        if (!result.second) return false;
        ids.erase(entry.iterator);
        if (managed) result.first->first.setManaged();
        entry.iterator = result.first;
        return true;
    }

    // 0x5e9e7c. Removes the entry at id when it is set and either only the collection uses the value
    // (count 1) or force is true. The node is erased, the entry reset, count decremented, next free
    // id lowered to id when that is smaller, and the trailing unset entries are cut from the vector
    // only when at least one set entry remains (an all-unset vector keeps its length).
    bool remove(std::uint16_t id, bool force = false) {
        if (id >= entries.size()) return false;
        SEntry& entry = entries[id];
        if (!Traits::isSet(entry.value)) return false;
        if (Traits::useCount(entry.value) != 1 && !force) return false;
        ids.erase(entry.iterator);
        entry.reset();
        if (id < nextId) nextId = id;
        --count;
        std::size_t last = entries.size();
        while (last > 0 && !Traits::isSet(entries[last - 1].value)) --last;
        if (last > 0 && last < entries.size()) entries.resize(last);
        return true;
    }

    // 0x5e9f78. In-order walk over the names; the successor is taken before each removal. Returns
    // the number removed (uint16 in the original, stored as int16 here).
    std::int16_t removeAll(bool force = false) {
        std::int16_t removed = 0;
        auto it = ids.begin();
        while (it != ids.end()) {
            const auto next = std::next(it);
            if (remove(it->second.id, force)) ++removed;
            it = next;
        }
        return removed;
    }

    void removeUnused() { removeAll(false); }  // 0x5ea048 / 0x5e67a8

    // Entry table and name map at the original offsets (see the header comment).
    std::vector<SEntry> entries;  // +0x18..+0x20
    Map ids;                      // +0x00..+0x10
    std::uint16_t nextId = 0;     // +0x24
    std::uint16_t count = 0;      // +0x26

private:
    SEntry invalidEntry{};  // the original's shared invalid entry
};

}  // namespace glitch::core::detail::sid_layout

// Open questions (not coded as fact):
//   - SIdValue+0x18 holds int 39 in every ctor and in insert/rename; its meaning is not recovered.
//   - The string header (SIdValue+0x00..+0x17) is written as an empty std::string; no function here
//     reads it, but getProperties hands out that same memory. The real Props layout is unknown.
//   - insert does not test the bool of insert_unique (0x5ea53c): a duplicate name re-links to the
//     existing node. Whether callers guarantee uniqueness is not recovered.
//   - SShaderParameterDef (20-byte entries, see the header comment) is not handled by this template.
//   - The managed flag: the original sets it on the key after insertion (insert, rename); the name
//     is not copied, so the caller's buffer becomes owned when managed is true.
