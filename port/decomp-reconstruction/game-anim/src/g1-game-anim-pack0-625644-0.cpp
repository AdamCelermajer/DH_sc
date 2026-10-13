// Functions of AnimatedDecor, Animation and AnimatorSynchronizedSet from libDungeonHunter2.so (batch
// g1-game-anim-pack0-625644-0). Rebuilt from the Hex-Rays pseudocode in recovered/native/hexrays-all and checked
// against the assembly. Class layouts: game/include/dh2/AnimatedDecor.hpp, Animation.hpp and AnimatorSynchronizedSet.hpp.
#include "dh2/AnimatedDecor.hpp"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "core-platform/lc_api.hpp"
#include "dh2/Animation.hpp"
#include "dh2/AnimApplicator.hpp"
#include "dh2/AnimController.hpp"
#include "dh2/AnimatorSynchronizedSet.hpp"
#include "dh2/Application.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/PODecor.hpp"
#include "dh2/Random.hpp"
#include "dh2/VisualObject.hpp"

// Global assert level (libDungeonHunter2.so gAssertLevel). core-platform/memory.hpp declares it too, but that header is
// not included in game TUs: its deps/glitch_core.hpp repeats the glitch::core types.
extern int gAssertLevel;

namespace {

// Assert text of the random clip callback: the original's Block.cpp path and line.
const char kBlockAssertFile[] = "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Block.cpp";
const int kBlockAssertLine = 345;

// The random clip pick shared by InitPost's "randomall" branch and the clip callback. The original calls the
// controller's GetNumClip(0) (vtable +0x10), draws a clip index in [0, count - 1) from the ordinary random stream
// (the Random::GetRandom clone .clone.3 at 0x388c58 is GetRandom(range, false)), then plays that clip with
// PlayClip(clip, loop = false, 0, 0) (vtable +0x1c). Returns PlayClip's result.
bool PlayRandomClip(AnimController* anim) {
    const int clipCount = anim->GetNumClip(0u);
    const unsigned int clip = Random::GetRandom(clipCount - 1, false);
    return anim->PlayClip(clip, false, 0, 0u);
}

}  // namespace

// 0x3883dc AnimatedDecor::IsAnimated() const
bool AnimatedDecor::IsAnimated() const {
    return true;
}

// 0x3884f8 AnimatedDecor::IsZonable() const
// Thunk: the original tail-branches to GameObject::MeetCondition (0x38ab60). Forwarded, not reimplemented.
bool AnimatedDecor::IsZonable() const {
    return GameObject::MeetCondition();
}

// 0x388cec AnimatedDecor::__CallbackRandomAll(glitch::scene::ITimelineController*, void*)
// The second argument is the decor (the user data that InitPost passes to SetCallbacks). The first argument, the
// timeline, is not used. The original returns int (PlayClip's result, or the fprintf result on the log path). The
// animation callbacks are void and the caller discards that value, so this port returns void.
void AnimatedDecor::__CallbackRandomAll(glitch::scene::ITimelineController* /*timeline*/, void* user) {
    AnimatedDecor* decor = static_cast<AnimatedDecor*>(user);
    if (PlayRandomClip(decor->m_visualObject->m_animController)) {
        return;
    }
    if (gAssertLevel == 2) {
        // The original writes to address 0 here to force a crash.
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "hasAnim == true", kBlockAssertFile, kBlockAssertLine);
    }
}

// 0x389098 AnimatedDecor::~AnimatedDecor() (D1, complete object)
// The original releases m_startAnim (+0x37c), resets the vptrs to Decor's, then calls GameObject::~GameObject. It makes no
// separate call to Decor::~Decor (0x388428): that function does the same vptr resets and the same GameObject call, so the
// C++ base destructor gives the same effects. The C++ order is this body (empty), the member m_startAnim, the Decor base
// destructor, then GameObject. The vptr stores are compiler-generated.
AnimatedDecor::~AnimatedDecor() {}

// 0x38910c AnimatedDecor::~AnimatedDecor() (D0, scalar deleting destructor)
// skipped: the original is ~AnimatedDecor() followed by CustomFree(this). The C++ delete expression produces it.

// 0x389128 AnimatedDecor::InitPost()
// Rebuilt: the flag, the Decor::InitPost base call, the MeetCondition branch, the startanim choice (idle when empty,
// "randomall", or the named clip with a clip-0 fallback), the mesh box, the physics object and the final Update.
// The physics world is Application::m_physicalWorld: the original's global dword_99F770 is Singleton<Application>::s_inst
// + 0x44 (asm: LDR R3,[R3,#(dword_99F770 - 0x99F72C)]), and the header places m_physicalWorld at +0x44.
void AnimatedDecor::InitPost() {
    unk_10c = 1;  // +0x10c, written before the Decor base InitPost
    Decor::InitPost();
    if (!MeetCondition()) {
        SetVisible(false);  // +0x40 vtable call. Its argument register holds the MeetCondition result, 0
        return;
    }
    VisualObject* visual = m_visualObject;  // +0x2d8
    if (visual == nullptr) {
        return;
    }
    if (m_startAnim.empty()) {  // +0x37c. The original compares the two data words of the string
        m_startAnim = "idle";
    }
    AnimController* anim = visual->m_animController;  // +0x38 of the visual object
    if (LC_API_STRCASECMP(m_startAnim.c_str(), "randomall") != 0) {
        if (!anim->HasClip(m_startAnim.c_str(), 0u) || !anim->PlayClip(m_startAnim.c_str(), true, 0, 0u)) {
            anim->PlayClip(0u, true, 0, 0u);
        }
    } else {
        PlayRandomClip(anim);
        anim->SetCallbacks(&AnimatedDecor::__CallbackRandomAll, this, nullptr, this);
    }
    visual->ApplyMeshBox();
    if (visual->m_hasColBox) {  // visual +0x28
        // operator new(0x28), then PODecor(m_physicalWorld, this, unused). The third argument is not used by the
        // constructor, and the original's r3 holds a residue at that call (operator new clobbers it), so false is passed.
        PODecor* physics = new PODecor(ApplicationGetInstance().m_physicalWorld, this, false);
        SetPhysicalObject(physics, false);  // GameObject::SetPhysicalObject(po, false)
    }
    Update();  // +0x2c vtable call, the original's tail call
}

// 0x389dd4 AnimatedDecor::DeclareProperties()
// The Decor base runs first (its own properties), then the startanim property, as in the original.
void AnimatedDecor::DeclareProperties() {
    Decor::DeclareProperties();
    AddProperty<std::string>("startanim", m_startAnim);  // PropertyMap at ObjectBase +0x04, m_startAnim at +0x37c
}

// 0x364ad4 Animation::~Animation()
// PARTIAL. The original destroys the engine CColladaDatabase at this + 0x18 (ADD R0, R0, #0x18, then its destructor)
// and then releases the STL string block at this + 0x00 (std::priv::_String_base::_M_deallocate_block). Neither member
// is declared in game/include/dh2/Animation.hpp yet (see the layout requests), so this destructor releases neither.
Animation::~Animation() {}

// 0x368efc AnimatorSynchronizedSet::getType() const
int AnimatorSynchronizedSet::getType() const {
    return 15;
}

// 0x368f24 AnimatorSynchronizedSet::animateNode(glitch::scene::ISceneNode*, unsigned int)
// PARTIAL. The original first runs glitch::collada::CSceneNodeAnimatorSet::animateNode on the base sub-object, which is
// not reachable (the base is not inherited, see the header). The applicator step is rebuilt: AnimApplicator::AnimateNode
// on +0x84. The function returns nothing. Hex-Rays shows float because its last instruction is a tail branch
// (B AnimApplicator::AnimateNode) that leaves r0 as it found it, and AnimApplicator::AnimateNode is void.
void AnimatorSynchronizedSet::animateNode(glitch::scene::ISceneNode* node, unsigned int time) {
    (void)node;
    m_applicator.AnimateNode(time);
}

// 0x368f54 AnimatorSynchronizedSet::~AnimatorSynchronizedSet() (D1)
// PARTIAL. The original runs ~AnimApplicator on +0x84 and then the CSceneNodeAnimatorSynchronizedSet base destructor
// (through the VTT entry). The applicator is a member here, so it is destroyed after this body, as in the original.
// The base is not inherited, so its destructor does not run.
AnimatorSynchronizedSet::~AnimatorSynchronizedSet() {}

// 0x368fcc AnimatorSynchronizedSet::~AnimatorSynchronizedSet() (D0, scalar deleting destructor)
// skipped: the original is ~AnimatorSynchronizedSet() followed by CustomFree(this). The C++ delete expression produces it.

// 0x368fe8 AnimatorSynchronizedSet::~AnimatorSynchronizedSet() (D2, VTT base-object variant)
// PARTIAL, no separate C++ form. The VTT variant resets the vptrs of the base sub-object, runs ~AnimApplicator on +0x84
// and the base destructor. It is covered by the D1 definition above and has the same blockers.

// 0x36904c AnimatorSynchronizedSet::AnimatorSynchronizedSet(boost::intrusive_ptr<glitch::collada::CAnimationSet> const&, std::vector<std::string> const&) (C1)
// PARTIAL. The original stores 1 at +0xc4, calls the CSceneNodeAnimatorSynchronizedSet base constructor with the animation
// set and the names (through the VTT), then builds the AnimApplicator at +0x84 with this as its animator. Not rebuilt: the
// base constructor (the base is not inherited, so the arguments are not forwarded to it), and the animator argument, which
// is nullptr here because the ISceneNodeAnimator base is not declared. The vptr stores are compiler-generated.
AnimatorSynchronizedSet::AnimatorSynchronizedSet(
        const boost::intrusive_ptr<glitch::collada::CAnimationSet>& animationSet,
        const std::vector<std::string>& names)
    : m_applicator(nullptr) {
    (void)animationSet;
    (void)names;
    unk_0c4 = 1;  // +0xc4, written before the base constructor in the original
}

// 0x3690dc AnimatorSynchronizedSet::AnimatorSynchronizedSet(boost::intrusive_ptr<glitch::collada::CAnimationSet> const&, std::vector<std::string> const&) (C2, VTT base-object variant)
// PARTIAL, no separate C++ form. The VTT variant passes the VTT entry (+4) and the arguments to the base constructor, then
// builds the AnimApplicator. It is the same as C1 except that it does not store 1 at +0xc4. The C1 definition above covers
// it, with the same blockers.
