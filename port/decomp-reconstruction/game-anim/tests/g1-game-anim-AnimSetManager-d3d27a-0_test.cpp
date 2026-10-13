// Test for batch g1-game-anim-AnimSetManager-d3d27a-0.
//
// Covers the id bookkeeping (Create counts down from -1, Flush resets it), the existence rule (AddAnim and
// AddTemplateAnim create a set only for a non-negative id), the LG_DEVICES flag, the GetClipId miss path and its
// GetAnimation call, and the GetAnimator use count (the AnimatorSet keeps one use on the set). The engine symbols that
// the batch source references are test doubles below: they record calls and do not model the engine.
// GetClipId's clip-id read (Animation +0x20) is not reproduced, so this test only checks the call and the placeholder
// -1 that GetClipId returns for an existing set. Update that check when the Animation layout request is applied.

#include <cstdio>

#include "dh2/AnimSetManager.hpp"
#include "dh2/AnimationSet.hpp"
#include "dh2/AnimatorSet.hpp"
#include "dh2/global/AnimationSet_GetAnimation.hpp"

namespace {

int g_failures = 0;

#define EXPECT(cond)                                                    \
    do {                                                                \
        if (!(cond)) {                                                  \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++g_failures;                                               \
        }                                                               \
    } while (0)

int g_createCalls = 0;
int g_loadCalls = 0;
int g_lastLoadArg = 0;
int g_getAnimCalls = 0;
int g_lastGetArg = 0;
int g_cacheCalls = 0;
boost::intrusive_ptr<AnimationSet> g_kept;  // the use an AnimatorSet keeps on its set (test double)

}  // namespace

// Test doubles. These are test-only definitions, not game code.
Animation::~Animation() {}
AnimationSet::~AnimationSet() {}

int AnimationSet::CreateAnimSet() {
    ++g_createCalls;
    return 0;
}

Animation* AnimationSet::LoadAnimation(int animId) {
    ++g_loadCalls;
    g_lastLoadArg = animId;
    return nullptr;
}

Animation* AnimationSet::GetAnimation(int animId) {
    ++g_getAnimCalls;
    g_lastGetArg = animId;
    return nullptr;
}

void AnimationSet::CalculateCacheSize() {
    ++g_cacheCalls;
}

// AnimatorSet embeds an AnimApplicator (+0x58), so its constructor needs these too. The test never runs the applicator.
AnimApplicator::AnimApplicator(glitch::collada::ISceneNodeAnimator* animator) : m_animator(animator) {}
AnimApplicator::~AnimApplicator() {}
void AnimApplicator::SetRefNode(glitch::scene::ISceneNode*) {}
void AnimApplicator::ResetDelta(unsigned int) {}
void AnimApplicator::AnimateNode(unsigned int) {}

// The engine AnimatorSet keeps a counted use on the set it plays (m_animationSet, +0x94).
AnimatorSet::AnimatorSet(const boost::intrusive_ptr<AnimationSet>& animationSet) : m_applicator(nullptr) {
    g_kept = animationSet;
}

// The constructor sets the vptr, so the vtable is emitted here and every virtual slot needs a definition.
AnimatorSet::~AnimatorSet() {}
int AnimatorSet::getType() const { return 13; }
void AnimatorSet::computeAnimationValues(unsigned int) {}
void AnimatorSet::applyAnimationValues(unsigned int) {}
void* AnimatorSet::getAnimationValue(int, int, void* out) { return out; }
void AnimatorSet::animateNode(glitch::scene::ISceneNode*, unsigned int) {}
void AnimatorSet::updateTime(unsigned int) {}
Animation* AnimatorSet::setCurrentAnimation(int) { return nullptr; }
void AnimatorSet::onUnbind(glitch::scene::ISceneNode*) {}

namespace glitch {
bool IReferenceCounted::drop() const {
    return --referenceCounter == 0;
}
}  // namespace glitch

bool LG_DEVICES = false;
int gAssertLevel = 0;

int main() {
    // Ids and creation: the first Create gives -2 and the counter counts down.
    {
        AnimSetManager mgr;
        EXPECT(!mgr.Exists(-2));
        EXPECT(mgr.Create() == -2);
        EXPECT(mgr.Exists(-2));
        EXPECT(mgr.Create() == -3);
        EXPECT(g_createCalls == 2);
        EXPECT(!mgr.m_sets.at(-2).unk_3c);  // LG_DEVICES is false here

        LG_DEVICES = true;
        EXPECT(mgr.Create() == -4);
        EXPECT(mgr.m_sets.at(-4).unk_3c);  // LG_DEVICES sets the flag on the new set
        LG_DEVICES = false;

        mgr.Flush();
        EXPECT(!mgr.Exists(-2));
        EXPECT(mgr.m_sets.empty());
        EXPECT(mgr.Create() == -2);  // Flush resets the id counter to -1
    }

    // AddAnim and AddTemplateAnim: a missing set is created only for a non-negative id.
    {
        AnimSetManager mgr;
        g_loadCalls = 0;
        mgr.AddAnim(-5, 1);
        EXPECT(!mgr.Exists(-5));  // negative id: nothing is created
        EXPECT(g_loadCalls == 0);

        mgr.AddAnim(3, 7);  // created, unk_3c clear, so it loads
        EXPECT(mgr.Exists(3));
        EXPECT(g_loadCalls == 1 && g_lastLoadArg == 7);
        mgr.AddAnim(3, 9);  // existing set, unk_3c clear, so it loads again
        EXPECT(g_loadCalls == 2 && g_lastLoadArg == 9);

        LG_DEVICES = true;
        mgr.AddAnim(4, 1);  // created with unk_3c set, so AddAnim does not load
        EXPECT(mgr.Exists(4));
        EXPECT(mgr.m_sets.at(4).unk_3c);
        EXPECT(g_loadCalls == 2);

        mgr.AddTemplateAnim(5, 2);  // AddTemplateAnim loads whatever the flag says
        EXPECT(mgr.Exists(5));
        EXPECT(g_loadCalls == 3 && g_lastLoadArg == 2);
        LG_DEVICES = false;

        mgr.AddTemplateAnim(-1, 3);  // negative id: nothing is created and nothing loads
        EXPECT(!mgr.Exists(-1));
        EXPECT(g_loadCalls == 3);
    }

    // GetClipId: a missing set returns -1 with no GetAnimation call. An existing set makes the GetAnimation call
    // and returns the placeholder -1 (the clip id itself is not reproduced yet).
    {
        AnimSetManager mgr;
        g_getAnimCalls = 0;
        EXPECT(mgr.GetClipId(9, 0) == -1);
        EXPECT(g_getAnimCalls == 0);

        mgr.AddAnim(9, 1);
        EXPECT(mgr.GetClipId(9, 2) == -1);
        EXPECT(g_getAnimCalls == 1 && g_lastGetArg == 2);
    }

    // GetAnimator: a missing set returns nullptr. An existing set gets one more use, kept by the AnimatorSet, and
    // CalculateCacheSize runs once.
    {
        AnimSetManager mgr;
        EXPECT(mgr.GetAnimator(6) == nullptr);
        EXPECT(g_cacheCalls == 0);

        mgr.AddAnim(6, 1);
        AnimationSet& set = mgr.m_sets.at(6);
        const int before = set.referenceCounter;
        g_cacheCalls = 0;
        glitch::collada::ISceneNodeAnimator* anim = mgr.GetAnimator(6);
        EXPECT(anim != nullptr);
        EXPECT(set.referenceCounter == before + 1);  // the AnimatorSet keeps one use
        EXPECT(g_cacheCalls == 1);

        g_kept = static_cast<AnimationSet*>(nullptr);  // the AnimatorSet releases its use
        EXPECT(set.referenceCounter == before);
        ::operator delete(static_cast<void*>(anim));  // the test double does not free the object
    }

    // DBG_SetName and DBG_DumpInfo leave the sets alone.
    {
        AnimSetManager mgr;
        mgr.AddAnim(2, 1);
        mgr.m_sets.at(2).m_animations[7];  // one Animation entry, so the inner walk visits a node
        mgr.DBG_SetName(2, "name");
        mgr.DBG_DumpInfo();
        EXPECT(mgr.Exists(2));
        EXPECT(mgr.m_sets.at(2).m_animations.size() == 1);
    }

    if (g_failures) {
        std::printf("%d failure(s)\n", g_failures);
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
