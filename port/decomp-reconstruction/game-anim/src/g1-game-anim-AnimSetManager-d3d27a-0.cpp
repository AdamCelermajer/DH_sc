// Batch g1-game-anim-AnimSetManager-d3d27a-0: AnimSetManager, the owner of the AnimationSet objects keyed by
// set id. Declarations come from dh2/AnimSetManager.hpp. Set ids count down from -1: Create() decrements
// m_nextSetId before it uses the id. The original STLport rb-tree at +0x04 is the std::map m_sets here.
// Every lookup uses the lower_bound search that the original Exists() performs.

#include "dh2/AnimSetManager.hpp"

#include <cstdio>
#include <cstdlib>
#include <string>

#include "dh2/Animation.hpp"
#include "dh2/AnimationSet.hpp"
#include "dh2/AnimatorSet.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/global/AnimationSet_GetAnimation.hpp"

// Engine assert level, declared in core-platform/memory.hpp. That header is not included here: it pulls
// core-platform/deps/glitch_core.hpp, which redefines the glitch-shared core types that AnimationSet.hpp uses.
extern int gAssertLevel;

// 0x475364 _ZN14AnimSetManagerC2Ev
// 0x4753b4 _ZN14AnimSetManagerC1Ev
// Both constructors have the same body, so one C++ constructor covers the complete-object and base-object
// variants. +0x04 m_sets starts empty (the map default), +0x1c m_nextSetId = -1. The vptr store is the compiler's.
AnimSetManager::AnimSetManager() : m_nextSetId(-1) {}

// 0x475404 _ZNK14AnimSetManager6ExistsEi
// lower_bound on the key tree: the set exists when the first key >= setId is equal to setId.
bool AnimSetManager::Exists(int setId) const {
    auto it = m_sets.lower_bound(setId);
    return it != m_sets.end() && setId >= it->first;
}

// 0x475460 _ZN14AnimSetManager11DBG_SetNameEiPKc
// Debug name setter. The original walks the set tree from the root toward setId (reads only) and returns
// without storing the name, so this build has no observable effect. The arguments are unused.
void AnimSetManager::DBG_SetName(int setId, const char* name) {
    (void)setId;
    (void)name;
}

// 0x475488 _ZNK14AnimSetManager12DBG_DumpInfoEv
// Debug dump. The original walks every set in key order and, inside each AnimationSet, walks its Animation
// map (AnimationSet +0x08, the std::map m_animations). Neither walk makes a call or stores anything, so both walks are
// kept as they are and nothing else is done.
void AnimSetManager::DBG_DumpInfo() const {
    for (const auto& entry : m_sets) {
        for (const auto& anim : entry.second.m_animations) {
            (void)anim;
        }
    }
}

// 0x47559c _ZN14AnimSetManager9GetClipIdEii
// Reproduced: the -1 result for a missing set, the node that Exists() finds (lower_bound), the AnimationSet at node
// +0x14, the AnimationSet::GetAnimation(animIndex) call (which can load or replace an entry), and the read of the clip
// id at Animation +0x20 (asm: ADD R0,R5,#0x14 / BL GetAnimation / LDR R0,[R0,#0x20]). The Hex-Rays line
// `*((_DWORD *)GetAnimation(...) + 8)` is a DWORD index, so the byte offset is 0x20. That word is Animation::unk_20
// (the clip id, -1 for none, see dh2/Animation.hpp). GetAnimation returns s_invalidAnim on a miss, so the entry is
// never null.
int AnimSetManager::GetClipId(int setId, int animIndex) {
    if (!Exists(setId)) {
        return -1;
    }
    AnimationSet& set = m_sets.lower_bound(setId)->second;  // the node Exists() found
    return set.GetAnimation(animIndex)->unk_20;              // Animation +0x20: the clip id
}

// 0x475664 _ZN14AnimSetManager5FlushEv
// Drops every set (the original erases the tree at +0x04 when its count at +0x14 is non-zero), then resets the
// id counter to -1.
void AnimSetManager::Flush() {
    if (!m_sets.empty()) {
        m_sets.clear();
    }
    m_nextSetId = -1;
}

// 0x4756a8 _ZN14AnimSetManagerD1Ev
// 0x475728 _ZN14AnimSetManagerD2Ev
// The base-object and complete-object destructors have the same body, so one C++ destructor covers both. Flush()
// already empties the tree, so the original's second erase block (reached only when the count is non-zero) is
// dead and not repeated. The vptr store is the compiler's.
// 0x47570c _ZN14AnimSetManagerD0Ev is the compiler-generated deleting destructor (this destructor, then
// operator delete, which is CustomFree in the original). It has no body to write here.
AnimSetManager::~AnimSetManager() {
    Flush();
}

// 0x4761cc _ZN14AnimSetManager23GetSynchronizedAnimatorEi
// partial. Reproduced: the existence check, the CalculateCacheSize() step that follows the creation in the
// original, and the nullptr result for a missing set.
// Not reproduced:
//  - The AnimatorSynchronizedSet (0xc8 bytes) is not created. The original calls
//    AnimatorSynchronizedSet(intrusive_ptr<CAnimationSet>(m_animSet), std::vector<std::string>()). The engine
//    headers do not derive CDynamicAnimationSet from CAnimationSet, and CAnimationSet is incomplete in
//    AnimatorSynchronizedSet.hpp, so the intrusive_ptr cannot be built. Until the layout request is applied, this
//    function returns nullptr for an existing set.
//  - The counted use on m_animSet (++ at +0x04 before the AnimatorSynchronizedSet is built, and the drop after it),
//    which goes with the object that is not created.
//  - The branch that runs m_animSet->compile() (CDynamicAnimationSet vtable slot +0x38) when setCleared (+0x70)
//    is set. compile() is not declared in collada_scene_accessors.hpp.
//  - The getTimelineCtrl() call (ISceneNodeAnimator vtable slot +0x44) and setLoop(false) on the returned
//    controller (vtable slot +0x40). AnimatorSynchronizedSet does not inherit ISceneNodeAnimator in its header.
glitch::collada::ISceneNodeAnimator* AnimSetManager::GetSynchronizedAnimator(int setId) {
    if (!Exists(setId)) {
        return nullptr;
    }
    AnimationSet& set = m_sets.lower_bound(setId)->second;
    set.CalculateCacheSize();
    return nullptr;
}

// 0x4762c4 _ZN14AnimSetManager11GetAnimatorEi
// partial. Reproduced: the existence check, the AnimatorSet (0xa4 bytes) built on the set through a counted
// intrusive_ptr, and CalculateCacheSize() after the creation.
// Not reproduced:
//  - The branch that runs m_animSet->compile() (CDynamicAnimationSet vtable slot +0x38) when setCleared (+0x70)
//    is set. compile() is not declared in collada_scene_accessors.hpp.
//  - The getTimelineCtrl() call (ISceneNodeAnimator vtable slot +0x44) and setLoop(false) on the returned
//    controller (vtable slot +0x40). AnimatorSet does not inherit ISceneNodeAnimator in its header.
// The return is the object address as glitch::collada::ISceneNodeAnimator*. In the original the engine base is the
// primary sub-object at +0x00, so the address is the same. The reinterpret_cast stands in for the upcast until the
// base is declared in AnimatorSet.hpp.
glitch::collada::ISceneNodeAnimator* AnimSetManager::GetAnimator(int setId) {
    if (!Exists(setId)) {
        return nullptr;
    }
    AnimationSet& set = m_sets.lower_bound(setId)->second;
    AnimatorSet* animator = nullptr;
    {
        // The original takes a use on the set (++ at +0x04), builds the AnimatorSet from a local counted pointer that
        // holds that use, then drops the local pointer. A converting intrusive_ptr would not add the use, so the
        // add-ref is explicit here.
        boost::intrusive_ptr_add_ref(&set);
        boost::intrusive_ptr<AnimationSet> local(&set);
        animator = new AnimatorSet(local);
    }
    set.CalculateCacheSize();
    return reinterpret_cast<glitch::collada::ISceneNodeAnimator*>(animator);
}

// 0x476398 _ZN14AnimSetManager15AddTemplateAnimEii
// partial. Reproduced: creation of a missing set (the id must be non-negative), the CreateAnimSet call, the
// LG_DEVICES flag, and LoadAnimation(animDict) on the set.
// Not reproduced: the CColladaDatabase built from the loaded animation's file name (Animation +0x14 m_fileName) with
// CColladaDatabase::DefaultFactory, the CDynamicAnimationSet::setDefaultAnimationLibrary(database) call on m_animSet
// (+0x20), and the database destructor. The engine headers declare setDefaultAnimationLibrary, but declare no
// CColladaDatabase constructor or destructor and no DefaultFactory.
void AnimSetManager::AddTemplateAnim(int setId, int animDict) {
    if (!Exists(setId)) {
        if (setId < 0) {
            return;
        }
        AnimationSet& created = m_sets[setId];
        created.CreateAnimSet();
        if (LG_DEVICES) {
            created.unk_3c = true;  // +0x3c
        }
    }
    AnimationSet& set = m_sets[setId];
    set.LoadAnimation(animDict);
}

// 0x476464 _ZN14AnimSetManager6CreateEv
// Reproduced: the id decrement, the existence check on the new id with the level-1 assert message, and the creation
// of the set (CreateAnimSet, LG_DEVICES flag). Returns the new id.
// At gAssertLevel == 2 with the id already taken, the original stores 0 to address 0 (dword_0), a deliberate fault
// that ends the process. Here that is std::abort(), the same convention as AnimatedFX::ForceMaterial and
// AnimController (the process ends at this point; the signal differs, SIGABRT for SIGSEGV).
int AnimSetManager::Create() {
    --m_nextSetId;
    if (Exists(m_nextSetId)) {
        if (gAssertLevel == 2) {
            std::abort();
        } else if (gAssertLevel == 1) {
            std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "Exists(m_nextSetId) == false",
                         "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\ObjectsSub\\Visual\\Animations\\AnimSetManager.cpp",
                         72);
        }
    }
    AnimationSet& set = m_sets[m_nextSetId];
    set.CreateAnimSet();
    if (LG_DEVICES) {
        set.unk_3c = true;  // +0x3c
    }
    return m_nextSetId;
}

// 0x47653c _ZN14AnimSetManager7AddAnimEii
// Reproduced: the creation of a missing set (the id must be non-negative), the CreateAnimSet call and the LG_DEVICES
// flag, then LoadAnimation(animDict) when the set's unk_3c flag (+0x3c) is clear. After the load the original calls
// DebugSwitches::load() on DebugSwitches::s_inst and then GetSwitch("isTracingAnimSetManager"), and discards the
// switch value (the string is built and freed inside the function). Both calls are kept.
void AnimSetManager::AddAnim(int setId, int animDict) {
    if (!Exists(setId)) {
        if (setId < 0) {
            return;
        }
        AnimationSet& created = m_sets[setId];
        created.CreateAnimSet();
        if (LG_DEVICES) {
            created.unk_3c = true;  // +0x3c
        }
    }
    AnimationSet& set = m_sets[setId];
    if (!set.unk_3c) {
        set.LoadAnimation(animDict);
        DebugSwitches::s_inst.load();
        (void)DebugSwitches::s_inst.GetSwitch(std::string("isTracingAnimSetManager"));  // value discarded
    }
}
