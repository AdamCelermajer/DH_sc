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

}  // namespace glitch::collada
