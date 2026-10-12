// glitch::IReferenceCounted: the reference-counted base used by glitch-video, glitch-core-io and
// glitch-scene. Shared copy, owned by the integrator. Names and signatures match the original.
//
// Evidence (libDungeonHunter2.so, ARM, 32-bit layout):
//   vtable _ZTVN6glitch17IReferenceCountedE, 20 bytes: +0x00 D1 (empty body, 0x31d580),
//          +0x04 D0 deleting dtor (0x31fd58: sets the base vptr, then CustomFree(this)),
//          +0x08 onDelete (empty, 0x31d5cc).
//   object: +0x00 vptr, +0x04 reference counter (int).
//   drop() (0x31d584): if (count-- != 1) return 0; else call vtable +0x08 (onDelete), then
//          vtable +0x04 (deleting dtor), return 1.
//   No IReferenceCounted constructor symbol exists: the base constructor is inlined into the derived
//          constructors, which store their vptr and set the counter to 1 at +0x04
//          (CMemoryWriteFile C1/C2 0x6b4d04 / 0x6b4d58, CPakReader C1 0x57002c).
//
// Port note: the original frees through the deleting destructor (CustomFree); here drop() uses
// delete, which runs the same virtual deleting destructor.
#pragma once

namespace glitch {

class IReferenceCounted {
public:
    // Virtual destructor occupies vtable slots +0x00 (D1) and +0x04 (D0), as in the original.
    virtual ~IReferenceCounted() = default;
    // vtable +0x08. Called by drop() right before the object is deleted (empty in the base).
    virtual void onDelete() {}
    // 0x31d584 _ZNK6glitch17IReferenceCounted4dropEv
    // Decrements the count; on zero calls onDelete() and deletes the object.
    // Returns true when the object was deleted.
    bool drop() const {
        const int previous = referenceCounter;
        referenceCounter = previous - 1;
        if (previous != 1) return false;
        // The original calls the onDelete slot (+0x08) first, then the deleting destructor (+0x04).
        IReferenceCounted* self = const_cast<IReferenceCounted*>(this);
        self->onDelete();
        delete self;
        return true;
    }
    // +0x04 in the original. Mutable because drop() is const (_ZNK).
    mutable int referenceCounter = 1;
};

}  // namespace glitch
