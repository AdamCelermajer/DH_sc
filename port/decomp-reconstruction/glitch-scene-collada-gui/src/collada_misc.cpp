#include "glitch-scene-collada-gui/collada_misc.hpp"
#include "glitch-scene-collada-gui/collada/CColladaDatabase.hpp"

namespace glitch::collada {

CAnimationStreamingManager* CAnimationStreamingManager::Instance = nullptr;

// 0x60c59c _ZN6glitch7collada26CAnimationStreamingManager17getAnimationBlockERNS0_24SAnimationBlockSearchKeyERN5boost13intrusive_ptrINS0_15CAnimationBlockEEE
// PARTIAL: the original first asks the held block for the matching neighbour (CAnimationBlock::getBlock 0x60b350), then the manager's
// registry (0x60c220: a lower-bound search over the sorted block vector, a new CAnimationBlock built from the key when none matches),
// and replaces the held block by the one found. The registry search and the block constructors are not reconstructed (stand-ins in
// recon/slice_07.cpp), so no block is found and the held block is left unchanged.
void CAnimationStreamingManager::getAnimationBlock(SAnimationBlockSearchKey&, CAnimationBlock*&) {}

// 0x60bad0 _ZN6glitch7collada14CAnimationBlock4freeEv. PARTIAL: unregister then delete; the key data release and operator delete are not
// modelled, so this does nothing.
void CAnimationBlock::free() {}

// 0x60c31c _ZN6glitch7collada14CAnimationBlock16prepareNextBlockEv. PARTIAL: the next-block load (1.7 KB) is not reconstructed.
void CAnimationBlock::prepareNextBlock() {}

// 0x60c3f0 _ZN6glitch7collada14CAnimationBlock4grabEv. Increments the count; prepares the next block at the first grab when none is linked.
void CAnimationBlock::grab() {
    refCount += 1;
    if (refCount == 2 && next == nullptr) {
        prepareNextBlock();
    }
}

// 0x60bb1c _ZN6glitch7collada14CAnimationBlock4dropEv. Frees when the count falls to one (free is partial).
void CAnimationBlock::drop() {
    refCount -= 1;
    if (refCount == 1) {
        free();
    }
}

// 0x60b3cc _ZN6glitch7collada26CAnimationStreamingManagerC2Ev
// 0x60b410 _ZN6glitch7collada26CAnimationStreamingManagerC1Ev
// Zeroes the eight words (the member initialisers) and publishes the object through the static Instance pointer.
CAnimationStreamingManager::CAnimationStreamingManager() {
    Instance = this;
}

// 0x60b990 _ZN6glitch7collada26CAnimationStreamingManager16checkMemoryUsageEv
// PARTIAL, and this definition exists to give the symbol a home: the header has declared it since the
// class was modelled, but only the free stand-in CAnimationStreamingManager_checkMemoryUsage lived in
// recon/slice_07.cpp, so Application::Init's call (game-app/src/g1-game-app-Application-b291be-2.cpp:319)
// referred to a member that nothing defined and 127 test targets failed to link.
//
// The original evicts cached segments while cacheUsage (+0x08) exceeds cacheLimit (+0x18), walking the
// cache vector at +0x0c..+0x14, dropping each entry whose refcount word is 1, and logs
// "AnimationStreamingManager : Memory usage exceed maximum cache size" when it cannot evict enough. The
// cache vector and the block registry are not reconstructed, so there is nothing to walk yet: the body is
// empty and the ledger row stays partial on exactly that. OBSERVABLE CONSEQUENCE while it stays empty: the
// cache never shrinks, so cacheUsage only ever grows past the 512000 that Init stores.
void CAnimationStreamingManager::checkMemoryUsage() {}

}  // namespace glitch::collada
