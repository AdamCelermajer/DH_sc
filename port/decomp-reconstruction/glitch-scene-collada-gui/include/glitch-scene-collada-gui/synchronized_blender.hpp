// glitch::collada::CSceneNodeAnimatorSynchronizedBlender: synchronises several
// segment animators. Most accessors forward through virtual calls on the
// segment objects (not yet mapped). Only the non-virtual helpers are here.
#pragma once

#include <cstdint>

namespace glitch::collada {

class CSceneNodeAnimatorSynchronizedBlender {
public:
    struct SynchronizationData {
        std::int32_t key = 0;  // first word; the ordering predicate compares it
    };

    // 0x660f6c _ZN6glitch7collada13ordering_funcERKNS0_37CSceneNodeAnimatorSynchronizedBlender19SynchronizationDataES4_
    static bool orderingFunc(const SynchronizationData& a, const SynchronizationData& b);

    // 0x367b34 _ZNK6glitch7collada37CSceneNodeAnimatorSynchronizedBlender29getSynchronizationElapsedTimeEv
    // Original returns 0.
    std::uint32_t getSynchronizationElapsedTime() const;
};

}  // namespace glitch::collada
