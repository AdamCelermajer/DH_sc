// Batch g1-game-anim-AnimationSet-4fa4a6-1 (part 2 of 2): AnimationSet::GetAnimation(int) and the
// name overload. Declarations come from dh2/AnimationSet.hpp. The lookup walks m_animations, which is
// the original STLport _Rb_tree (a lower_bound on the AnimDict id), and a miss goes through the cache
// rules (LoadAnimation or ReplaceAnimation, with s_invalidAnim as the "no entry" result).

#include "dh2/AnimationSet.hpp"

#include <cstdint>

#include "dh2/Animation.hpp"
#include "dh2/global/AnimationSet_GetAnimation.hpp"
#include "pydata-accessors/member_names.hpp"

// 0x3660f4 _ZN12AnimationSet12GetAnimationEi
// PARTIAL: on a hit the original stamps the entry's last-use time, glitch::os::Timer::getRealTime(),
// at node +0x3c, which is Animation +0x28. dh2/Animation.hpp does not declare that field yet
// (layout request), so the stamp is not written here. The lookup and every other branch are rebuilt.
Animation* AnimationSet::GetAnimation(int animId) {
    // lower_bound: the first entry whose key is not less than animId (the original walk at +0x08).
    auto it = m_animations.lower_bound(animId);
    if (it != m_animations.end() && it->first == animId) {
        // partial: missing last-use stamp, see the note above.
        return &it->second;
    }
    if (!unk_3c && LG_DEVICES) {
        return &s_invalidAnim;
    }
    // m_cacheSize (+0x30) above the node count (+0x18) means the cache has room: load directly.
    if (m_cacheSize > static_cast<std::int32_t>(m_animations.size())) {
        return LoadAnimation(animId);
    }
    Animation* result = ReplaceAnimation(animId);
    if (result == &s_invalidAnim) {
        return LoadAnimation(animId);
    }
    return result;
}

// 0x3661f4 _ZN12AnimationSet12GetAnimationEPKc
Animation* AnimationSet::GetAnimation(const char* name) {
    int animId = Arrays::GetMemberIDByString<Arrays::AnimDict>(name);
    return GetAnimation(animId);
}
