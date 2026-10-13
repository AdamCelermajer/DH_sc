// MenuCharacterSelect bodies, batch g1-game-ui-MenuCharacterSelect-c1e600-0 (the whole class, one batch).
// Rebuilt from the Hex-Rays pseudocode of libDungeonHunter2.so. Offsets in comments are the original 32-bit
// ARM offsets. A function marked PARTIAL says in its comment which part is missing and why.
//
// Blockers shared by several functions (see the ledger notes and the layout requests):
//   - m_animator and m_animSet are void* (constructAnimator() and the object its vtable +0x44 returns). Every
//     call on them goes through an untyped vtable, so UpdateAnim and SetAnim are partial.
//   - The class-select scene (m_sceneRoot, the scene manager at device +0x1c, the light and camera nodes) is
//     driven through untyped glitch::scene vtables, so Show, Hide and RenderClassSelectPane are partial.
//   - gameswf::character has no DH2 declaration in the tree. The upstream gameswf-r1714 header (under
//     vendor-gameswf/upstream) declares a tu_string m_name, but its offset is not verified against the binary
//     (+0x44 is the name word the original reads). Reads of that field are therefore missing: the instance-name
//     reads for RenderFX::SetVisible in Update, and the class_select lookup in Show (RenderFX::Find returns
//     as_object*, and the original uses the result as a character).
//   - RenderFX::Event has no name for the float at +0x0c (the drag x). OnEvent's drag part is missing.

#include "dh2/Application.hpp"
#include "dh2/AnimSetManager.hpp"
#include "dh2/Character.hpp"
#include "dh2/CharAnimator.hpp"
#include "dh2/CharStateMachine.hpp"
#include "dh2/DebugCachedCharacter.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/MenuCharacterSelect.hpp"
#include "dh2/MenuFX.hpp"
#include "dh2/MenuMainMenu.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/RenderFX.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/StringManager.hpp"
#include "dh2/global/ObjectManagerInstance.hpp"
#include "pydata-accessors/member_names.hpp"
#include "pydata-accessors/structs_records.hpp"
#include "pydata-accessors/w7_pydata12.hpp"
#include "vendor-gameswf/as_value.hpp"

// core-platform/memory.hpp declares gAssertLevel. It is not included here, as in FlashAnimManager.
extern int gAssertLevel;

namespace {

// Writes the StrID constant's text into one character as HTML. The original reads the constant and the string
// in this order, after the character lookup that the caller has already done.
void FormatClassText(MenuFX* menu, gameswf::character* target, const char* constantName) {
    const int constant = ApplicationGetInstance().m_pyDataConstants->getConstant("StrID", constantName);
    const char* text = ApplicationGetInstance().m_stringManager->getString(constant);
    menu->FormatHTML(target, "%s", text);
}

// Sets the CurrentClass variable of the root movie's menu_SelectClass clip to the class member name.
// The gameswf as_value is destroyed at the end of this function, which is where the original calls drop_refs.
void SetCurrentClassOnRoot(const char* className) {
    MenuFX* root = MenuManager::GetInstance()->GetRoot();
    gameswf::as_value value;
    value.set_string(className);
    root->InvokeASCallback("_root.menu_SelectClass", "CurrentClass", &value, 1);
}

}  // namespace

// 0x427fd8 MenuCharacterSelect::IsAnimOver()
bool MenuCharacterSelect::IsAnimOver() {
    return m_animTime >= m_animEnd;  // +0xe0 >= +0xe8
}

// 0x427ffc MenuCharacterSelect::UpdateAnim()
// PARTIAL: the original first calls vtable slot +0x10 of m_animator (void*, no typed interface in the tree) with
// (m_animator, m_sceneRoot, (unsigned)m_animTime). That call is missing here. The time advance and the clamp to
// the end time are reconstructed, and they run after the call in the original, as they do here.
bool MenuCharacterSelect::UpdateAnim() {
    // MISSING: m_animator->vtable[+0x10](m_animator, m_sceneRoot, static_cast<unsigned int>(m_animTime)).
    const float frameTime = static_cast<float>(static_cast<unsigned int>(ApplicationGetInstance().GetDt()));
    const float time = m_animTime + frameTime;  // +0xe0 + (float)(unsigned)GetDt()
    const float end = m_animEnd;                // +0xe8
    m_animTime = time;
    const bool over = time >= end;
    if (over) {
        m_animTime = end;
    }
    return over;
}

// 0x428078 MenuCharacterSelect::SetAnim(char const*)
// PARTIAL, body not reconstructed. Every effect of this function is an untyped call on m_animSet (void*), and the
// values it stores (the start and end time, and the seek time) come from those calls. In the original order:
//   index = set->vtable[+0x18](name)         ; index of the clip called name
//   if (index < 0) assert "idx >= 0" at MenuCharacterSelect.cpp:495 (level 2 aborts, level 1 prints)
//   set->vtable[+0x10](index)
//   set->vtable[+0x40](0)
//   m_animStart = set->vtable[+0x20](index)  ; start time (stored as float at +0xe4)
//   m_animEnd   = set->vtable[+0x24](index)  ; end time (stored as float at +0xe8)
//   m_animTime  = m_animStart                ; +0xe0
//   return set->vtable[+0x0c]((int)m_animStart)
void MenuCharacterSelect::SetAnim(const char* name) {
    (void)name;
}

// 0x4281b8 MenuCharacterSelect::OnEvent(RenderFX::Event &)
// PARTIAL: the button part (event kind 2) is reconstructed. The drag part (kind 5) is missing: it reads the pointer
// x as the float at RenderFX::Event +0x0c, which the header does not name (layout request). Kind-5 events therefore
// reach only the final MenuBase::OnEvent. The class-change sound (VoxSoundManager::PlayMenu with the index of the
// "MenuTab" entry of Arrays::Sounds) is missing too: VoxSoundManager has no PlayMenu declaration in the tree.
void MenuCharacterSelect::OnEvent(RenderFX::Event& e) {
    if (m_inputEnabled) {  // +0xfc
        const int kind = static_cast<int>(e.type);  // +0x08 of the event
        if (kind == 2) {
            // Raw pointer compare, as the original does: the event target is an as_object*, the button is a character*.
            if (static_cast<const void*>(e.target) == static_cast<const void*>(m_btnRight.GetChar())) {
                if (m_classIndex <= 1) {  // +0xec
                    ++m_classIndex;
                }
            } else if (static_cast<const void*>(e.target) == static_cast<const void*>(m_btnLeft.GetChar())) {
                if (m_classIndex > 0) {
                    --m_classIndex;
                }
            }
        } else if (kind != 5) {
            m_dragging = false;  // +0xf4
        }
        // kind 5 (drag): MISSING. See the comment above.
    }
    MenuBase::OnEvent(e);
}

// 0x4283c0 MenuCharacterSelect::Hide()
// PARTIAL: two teardown calls are missing. (1) m_sceneRoot (CRootSceneNode) vtable +0x68, when m_sceneRoot is not null.
// (2) vtable +0x68 of the scene manager that the device at dword_99F73C holds at +0x1c. Neither type has a typed
// interface in the tree. The rest follows the original order: the MenuMainMenu hand-over, the AnimSetManager flush
// and the ObjectManager flush.
void MenuCharacterSelect::Hide() {
    if (!IsValidMenu()) {  // vtable +0x3c
        return;
    }
    MenuBase::Hide();
    // MISSING (1): if (m_sceneRoot != nullptr) m_sceneRoot->vtable[+0x68]().
    // MISSING (2): device +0x1c scene manager ->vtable[+0x68]().
    dword_99F764->Flush();
    Singleton<AnimSetManager>::s_inst.Flush();
    // The original calls MenuMainMenu::GetInstance() before each static call below. The result is passed on in a
    // register, but the callees take no object. The calls are kept for the side effect of the first-use construction.
    (void)MenuMainMenu::GetInstance();
    MenuMainMenu::CreateAvatarCamera();
    (void)MenuMainMenu::GetInstance();
    MenuMainMenu::m_isDestroyed = false;
    (void)MenuMainMenu::GetInstance();
    MenuMainMenu::mSavegameSlot = m_savedSavegameSlot;  // +0x160
    (void)MenuMainMenu::GetInstance();
    MenuMainMenu::SetupScene();
    (void)MenuMainMenu::GetInstance();
    MenuMainMenu::SetupCharacter();
}

// 0x428498 MenuCharacterSelect::Update()
// PARTIAL: each class case has two RenderFX::SetVisible(menu, name, flag) calls, one for the btn_right character and
// one for the btn_left character. The name is read from gameswf::character +0x44 (a tu_string pointer), and
// gameswf::character is not defined in the tree. Those calls are missing (marked MISSING below). Everything else
// in the function is reconstructed, including the animation choice that calls SetAnim, which is partial.
void MenuCharacterSelect::Update() {
    if (m_shownClassIndex == m_classIndex) {  // +0xf0 == +0xec
        if (IsAnimOver()) {
            switch (m_classIndex) {
                case 1:
                    SetAnim("lol_2_idle");
                    break;
                case 2:
                    SetAnim("lol_3_idle");
                    break;
                case 0:
                    SetAnim("lol_1_idle");
                    break;
            }
            m_inputEnabled = true;  // +0xfc
        }
    } else {
        switch (m_classIndex) {
            case 1:  // Rogue
                FormatClassText(m_menu, m_classTitle.GetChar(), "MENU_CLASS_01");
                FormatClassText(m_menu, m_classDescription.GetChar(), "MENU_ROGUE_DESC");
                m_classMemberId = Arrays::GetMemberIDByString<Arrays::CharacterTable>("RoguePlayerBase");  // +0x164
                SetCurrentClassOnRoot("RoguePlayerBase");
                // MISSING: RenderFX::SetVisible(m_menu, name of m_btnRight's character, true).
                // MISSING: RenderFX::SetVisible(m_menu, name of m_btnLeft's character, true).
                SetAnim(m_shownClassIndex != 0 ? "lol_3_to_2" : "lol_1_to_2");
                break;
            case 2:  // Mage
                FormatClassText(m_menu, m_classTitle.GetChar(), "MENU_CLASS_02");
                FormatClassText(m_menu, m_classDescription.GetChar(), "MENU_MAGE_DESC");
                m_classMemberId = Arrays::GetMemberIDByString<Arrays::CharacterTable>("MagePlayerBase");
                SetCurrentClassOnRoot("MagePlayerBase");
                // MISSING: RenderFX::SetVisible(m_menu, name of m_btnRight's character, false).
                // MISSING: RenderFX::SetVisible(m_menu, name of m_btnLeft's character, true).
                SetAnim("lol_2_to_3");
                break;
            case 0:  // Knight
                FormatClassText(m_menu, m_classTitle.GetChar(), "MENU_CLASS_00");
                FormatClassText(m_menu, m_classDescription.GetChar(), "MENU_KNIGHT_DESC");
                m_classMemberId = Arrays::GetMemberIDByString<Arrays::CharacterTable>("KnightPlayerBase");
                SetCurrentClassOnRoot("KnightPlayerBase");
                // MISSING: RenderFX::SetVisible(m_menu, name of m_btnRight's character, true).
                // MISSING: RenderFX::SetVisible(m_menu, name of m_btnLeft's character, false).
                SetAnim("lol_2_to_1");
                break;
        }

        const int newClass = m_classIndex;
        m_inputEnabled = false;      // +0xfc
        m_shownClassIndex = newClass;  // +0xf0

        // Actor i plays its selected idle (row word +0x60, looping) when i is the shown class, and its
        // unselected idle (row word +0x5c, not looping) otherwise. The row is Character::GetCharAnimTable().
        // The pydata names of those words are m_w24 and m_w23; their meaning is not recovered.
        for (int i = 0; i < 3; ++i) {
            Character* actor = m_actors[i];
            const Structs::CharAnim* row = actor->GetCharAnimTable();
            CharStateMachine* machine = static_cast<CharStateMachine*>(actor);
            if (i == m_shownClassIndex) {
                machine->SM_SetAnimState(row->m_w24, true, false);
            } else {
                machine->SM_SetAnimState(row->m_w23, false, false);
            }
        }
    }

    UpdateAnim();
    for (Character* actor : m_actors) {
        // The original passes a null CharAnimator* when the actor is missing. This port skips the call instead.
        // Actors are spawned in Show, so the null case is not reached after Show.
        if (actor != nullptr) {
            actor->CharAnimator::Update();
        }
    }
}

// 0x428b74 MenuCharacterSelect::RenderClassSelectPane(gameswf::render_state &,void *)
// PARTIAL, body not reconstructed. The original (1) finds the "class_select" character and takes its absolute
// bounds, (2) converts the bounds to pixels with GameSWFUtils::GetInvPixelScaleX/Y, (3) saves the video driver's
// viewport, sets the bounds as the viewport, and (4) calls the scene manager's vtable +0x3c with 0, then restores the
// saved viewport. Step 1 needs the gameswf::character type (RenderFX::Find returns as_object*), and steps 3 and 4
// need the untyped driver and scene manager interfaces. The user pointer is this (the Hex-Rays *(a2+4) is m_menu).
void MenuCharacterSelect::RenderClassSelectPane(gameswf::render_state& state, void* user) {
    (void)state;
    (void)user;
}

// 0x428cc0 MenuCharacterSelect::Init()
void MenuCharacterSelect::Init() {
    MenuManager::GetInstance()->RegisterMenu(this);
    if (m_menu != nullptr) {  // +0x04 of MenuFX::State
        m_classTitle.RefreshCache("class_title.text", m_menu, GetCurrentMenuContext());
        m_classDescription.RefreshCache("class_description.text", m_menu, GetCurrentMenuContext());
        m_classMemberId = Arrays::GetMemberIDByString<Arrays::CharacterTable>("KnightPlayerBase");  // +0x164
    }
}

// 0x428db4 MenuCharacterSelect::~MenuCharacterSelect()  (D2 body)
// 0x428d44 MenuCharacterSelect::~MenuCharacterSelect()  (D1 body, identical)
// The members are destroyed in reverse declaration order: m_classDescription (+0x198), m_classTitle (+0x168),
// m_btnLeft (+0x130), m_btnRight (+0x100), which is the original order. Then MenuBase::~MenuBase runs.
MenuCharacterSelect::~MenuCharacterSelect() {}

// 0x429a98 MenuCharacterSelect::MenuCharacterSelect()  (C1 body)
// 0x429b98 MenuCharacterSelect::MenuCharacterSelect()  (C2 body, identical)
MenuCharacterSelect::MenuCharacterSelect() : MenuBase("menu_SelectClass") {
    // The original object is the function-local static of GetInstance, so every field this constructor does not
    // write is zero (.bss). They are set to zero here.
    m_sceneRoot = nullptr;
    m_camera = nullptr;
    m_animator = nullptr;
    m_animSet = nullptr;
    for (Character*& actor : m_actors) {
        actor = nullptr;
    }
    m_animTime = 0.0f;
    m_animStart = 0.0f;
    m_animEnd = 0.0f;
    m_classIndex = 0;
    m_shownClassIndex = 0;
    m_dragStartX = 0.0f;
    m_inputEnabled = false;
    m_classMemberId = 0;

    m_dragging = false;        // +0xf4
    m_savedSavegameSlot = -1;  // +0x160
    Init();
}

// 0x428d98 MenuCharacterSelect::~MenuCharacterSelect()  (D0, deleting destructor)
// Skipped: the compiler emits the deleting destructor for a delete expression (destructor, then the storage release).

// 0x429b10 MenuCharacterSelect::GetInstance()
MenuCharacterSelect* MenuCharacterSelect::GetInstance() {
    static MenuCharacterSelect instance;  // guard variable and atexit registration are the compiler's
    return &instance;
}

// 0x428f38 MenuCharacterSelect::Show()
// PARTIAL. Reconstructed: the MenuMainMenu hand-over, the IsValidMenu check, MenuBase::Show, the reset of
// m_shownClassIndex to -1, and the read of the debug switch. Not reconstructed:
//   - the class_select lookup (RenderFX::Find returns as_object*, and gameswf::character is not defined in the
//     tree), the write of gameswf::character +0x9c = 1, RenderFX::SetEnabled, and RegisterDisplayCallback with
//     RenderClassSelectPane and this;
//   - the CLASS_SELECTION.bdae database, constructScene, the scene node setup (position, camera, ambient light),
//     the light point node, and constructAnimator with its animation set (untyped interfaces);
//   - the spawn of the three dummy game objects with their visual objects, the spawn of the three Player actors
//     with their CharProperties, equipment and animation sets, and the class-button pass (RefreshCache of btn_right
//     and btn_left, visibility by SetVisible).
void MenuCharacterSelect::Show() {
    // The original calls GetInstance before each static call here (see Hide for why).
    (void)MenuMainMenu::GetInstance();
    MenuMainMenu::DestroyScene();
    (void)MenuMainMenu::GetInstance();
    MenuMainMenu::DestroyAvatarCamera();
    (void)MenuMainMenu::GetInstance();
    m_savedSavegameSlot = MenuMainMenu::mSavegameSlot;  // +0x160
    (void)MenuMainMenu::GetInstance();
    MenuMainMenu::m_isDestroyed = true;
    (void)MenuMainMenu::GetInstance();
    MenuMainMenu::mSavegameSlot = -1;

    if (!IsValidMenu()) {  // vtable +0x3c
        return;
    }
    MenuBase::Show();
    m_shownClassIndex = -1;  // +0xf0

    DebugSwitches::s_inst.load();
    (void)DebugSwitches::s_inst.GetSwitch("isTracingMenuCharacterSelect");

    // MISSING from here on: see the comment above the function.
}
