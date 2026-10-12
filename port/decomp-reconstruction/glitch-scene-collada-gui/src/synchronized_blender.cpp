#include "glitch-scene-collada-gui/synchronized_blender.hpp"

namespace glitch::collada {

// 0x660f6c _ZN6glitch7collada13ordering_funcERKNS0_37CSceneNodeAnimatorSynchronizedBlender19SynchronizationDataES4_
bool CSceneNodeAnimatorSynchronizedBlender::orderingFunc(const SynchronizationData& a,
                                                         const SynchronizationData& b) {
    return a.key < b.key;
}

// 0x367b34 _ZNK6glitch7collada37CSceneNodeAnimatorSynchronizedBlender29getSynchronizationElapsedTimeEv
std::uint32_t CSceneNodeAnimatorSynchronizedBlender::getSynchronizationElapsedTime() const {
    return 0;
}

}  // namespace glitch::collada
