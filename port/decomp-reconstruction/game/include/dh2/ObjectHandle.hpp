// ObjectHandle: id-based weak reference to an ObjectBase, resolved through ObjectManager.
// Original size 0xc (allocated with operator new(12) in the ObjectBase constructor).
#pragma once

#include <cstdint>

class ObjectBase;
class GameObject;
class Character;

class ObjectHandle {
public:
    ObjectHandle();                         // 0x33f4f4 / 0x33f50c
    explicit ObjectHandle(ObjectBase* object);  // 0x33f524 / 0x33f578, copies object->GetHandle()

    ObjectBase* GetObject(bool assertFound);        // 0x33fdc0: cached lookup in ObjectManager's id map
    ObjectBase* GetObject(bool assertFound) const;  // 0x33ff8c (4-byte thunk to the non-const form)

    operator GameObject const*();           // 0x33feac
    operator GameObject*();                 // 0x33fee4
    operator Character const*();            // 0x33ff1c
    operator Character*();                  // 0x33ff54
    template <typename T> operator T*();    // e.g. Container* at 0x4593f4

    std::uint32_t m_id;                     // +0x00 object id (key of ObjectManager's std::map<int, ObjectListItem>)
    ObjectBase* m_cached;                   // +0x04 cached resolve result
    std::uint32_t m_generation;             // +0x08 ObjectManager generation (ObjectManager+0x78) the cache is valid for; -1 initially
};
