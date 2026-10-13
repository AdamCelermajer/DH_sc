// Batch g1-game-world-RootSceneNode-e402f1-1 (part 2 of 2): the two RootSceneNode constructors, 0x35d824 (C1, complete
// object) and 0x35d910 (C2, base object). The declaration comes from dh2/RootSceneNode.hpp, which needed no change.
//
// One C++ definition covers both symbols, as elsewhere in game-world. The binary forms differ in how the virtual base
// is built, and the compiler reproduces that part from the C++ class:
//   - C1 is called with the database only. It writes the vptr and count of the secondary subobject at +0x20c and +0x210
//     (IReferenceCounted vtable + 8 and 1), constructs the engine base through the VTT (the base call takes VTT+4 and the
//     database), then stores the final secondary vptr at +0x20c.
//   - C2 takes the VTT as its second argument. It stores the primary vptr from VTT[0] and the construction vptrs of the
//     virtual base from VTT[13] and VTT[14], and it does not touch +0x20c or +0x210 (the complete object did that).
//
// Partial, for two reasons that are not expressible in this tree:
//   1. The engine base glitch::collada::CRootSceneNode is default-constructed. The original calls its constructor with the
//      database (engine rows 0x65b734 and 0x65b844, partial: body not rebuilt). dh2/RootSceneNode.hpp needs the engine
//      header to declare that constructor, and that header is not edited in this batch (layout request in the ledger).
//   2. The final vptr of the secondary subobject at +0x20c (the RootSceneNode vtable + 0x124) is not stored. C++ has no
//      address for a vtable of this class, and the secondary subobject is not a C++ base here.
//
// Everything else is exact: every member the original writes is written here, with the same value (see the comments).
// The two std::string members are default constructed, which is the original empty state (a 16-byte buffer with a zero
// first byte, built by _M_allocate_block).

#include "dh2/RootSceneNode.hpp"

// 0x35d824 _ZN13RootSceneNodeC1ERKN6glitch7collada16CColladaDatabaseE
// partial: complete-object constructor. Base constructor not bound (see point 1 above); the secondary vptr at +0x20c is
// not stored (point 2). The primary vptr is set by the compiler from the class.
RootSceneNode::RootSceneNode(const glitch::collada::CColladaDatabase& database) {
    (void)database;  // passed to the engine base constructor in the original; that call is not bound here

    // +0x210: count of the secondary subobject (IReferenceCounted-style count, set to 1 before the base call in the original).
    m_secondaryRefCount = 1;
    // +0x20c m_secondaryVptr: not written (point 2). The original leaves the final RootSceneNode vtable address here.

    // m_fileName (+0x1bc) and m_name (+0x1d4): empty, as default constructed.
    // +0x1ec: scene displacement is off until _EnableDisplacement runs.
    m_displacementEnabled = false;
    // +0x1f0 and +0x1f4: animation roots are found on demand by GetAnimRoot.
    m_animRoot = nullptr;
    m_animRootAlt = nullptr;
    // +0x1f8: the displacement node is created by _EnableDisplacement.
    m_displacementNode = nullptr;
    // +0x1fc: no animation time has been passed to onAnimate yet.
    m_lastAnimTime = 0;
    // +0x200: IsVisibleFastTest reads this as a gate before the game object culling test.
    m_cullBeforeUpdate = true;
    // +0x204: no game object yet.
    m_object = nullptr;
    // +0x208: onAnimate applies the pending update once, then clears it.
    unk_208 = true;
    // +0x209: no visibility change requested.
    m_visibleRequested = false;
    // +0x20a: onAnimate has not skipped an animation yet.
    unk_20a = false;
}

// 0x35d910 _ZN13RootSceneNodeC2ERKN6glitch7collada16CColladaDatabaseE
// partial: base-object constructor with the same C++ definition as C1 above; the compiler emits this symbol from it. The
// binary form takes the VTT (second argument) and stores the primary vptr from VTT[0] and the construction vptrs of the
// virtual base from VTT[13] and VTT[14]; these stores are not reproduced. Unlike C1, this form does not write +0x20c or
// +0x210 (the complete object does), so the m_secondaryRefCount store above is extra on this path. Same missing base
// constructor and secondary vptr as C1. No derived class in the tree constructs RootSceneNode through this form.
