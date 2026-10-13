// MenuCharMenu_InvMain bodies, batch g1-game-ui-MenuCharMenu_InvMain-17e1b1-0 (part 1 of 1 of this class).
// Rebuilt from the Hex-Rays pseudocode and the assembly of libDungeonHunter2.so. Offsets in comments are the
// original 32-bit ARM offsets. Partial functions say in their comment which part is missing.
//
// Facts shared by the bodies:
//   - The class has no members of its own (MenuBase layout). m_inventoryPaneCam is static and defined here.
//   - The compiler emits the vptr stores of the constructors and destructors, so no body writes them.
//   - The compiler-generated deleting destructor (D0, 0x452bcc) is not rebuilt: delete emits it.
//   - The scene manager (glitch::scene::CSceneManager) is the word at IDevice +0x1c, read through dword_99F73C
//     (the glitch device, Singleton<Application>::s_inst.m_device at +0x10). dh2 does not declare IDevice +0x1c, so
//     every access to the scene manager is missing and the bodies that need it are partial (layout request).
//   - RootSceneNode (the model root, VisualObject +0x08) is an ISceneNode in the original, but the dh2 class does not
//     declare that base yet. The ISceneNode calls on it (addChild, getRotation, setRotation, getPosition, setPosition)
//     are therefore missing, and the bodies that make them are partial (layout request).
//   - MenuManager +0x60 is MenuManager::unk_60 here: the camera saved by CreateAvatarCamera, restored by
//     DestroyAvatarCamera.
//   - PlayerInfo +0x660 is declared as PlayerInfo::unk_660 (Character*); the original name is character_660.

#include "dh2/AnchorBase.hpp"
#include "dh2/Application.hpp"
#include "dh2/CameraLevel.hpp"
#include "dh2/Character.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/Level.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuCharMenu_InvDetails.hpp"
#include "dh2/MenuCharMenu_InvMain.hpp"
#include "dh2/MenuFX.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/PlayerInfo.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/RenderFX.hpp"
#include "dh2/RootSceneNode.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/global/IsMultiplayerGame.hpp"
#include "glitch-core-io/core/reference_counted.hpp"
#include "glitch-scene-collada-gui/scene/CCameraSceneNode.hpp"
#include "glitch-scene-collada-gui/scene/ICameraSceneNode.hpp"
#include "glitch-shared/glitch_core.hpp"

// Static avatar camera. Created by CreateAvatarCamera, dropped and cleared by DestroyAvatarCamera and the destructor.
glitch::scene::ICameraSceneNode* MenuCharMenu_InvMain::m_inventoryPaneCam = nullptr;

// 0x452e20 _ZN20MenuCharMenu_InvMainC1Ev (complete-object constructor)
// 0x452e68 _ZN20MenuCharMenu_InvMainC2Ev (base-object constructor, the same bytes)
// MenuBase("menu_InventorySheetMain"), then Init(). The vptr store is emitted by the compiler.
MenuCharMenu_InvMain::MenuCharMenu_InvMain() : MenuBase("menu_InventorySheetMain") {
    Init();
}

// 0x452b64 _ZN20MenuCharMenu_InvMainD1Ev (complete-object destructor)
// 0x452be8 _ZN20MenuCharMenu_InvMainD2Ev (base-object destructor, the same bytes)
// Drops the static avatar camera and clears the pointer. MenuBase::~MenuBase runs after it (implicit here).
MenuCharMenu_InvMain::~MenuCharMenu_InvMain() {
    if (m_inventoryPaneCam) {
        static_cast<glitch::IReferenceCounted*>(m_inventoryPaneCam)->drop();
        m_inventoryPaneCam = nullptr;
    }
}

// 0x452bcc _ZN20MenuCharMenu_InvMainD0Ev (deleting destructor)
// Compiler-generated: runs ~MenuCharMenu_InvMain and then CustomFree(this). The delete expression emits it, so no
// body is written here.

// 0x452e00 _ZN20MenuCharMenu_InvMain4InitEv
// Registers this menu with the MenuManager singleton, then initialises the details page
// (MenuCharMenu_InvDetails::Init). Both are tail calls in the original.
void MenuCharMenu_InvMain::Init() {
    MenuManager::GetInstance()->RegisterMenu(this);
    MenuCharMenu_InvDetails::GetInstance()->Init();
}

// 0x452eb0 _ZN20MenuCharMenu_InvMain11GetInstanceEv
// Function-local static, constructed on first use. The guard and the atexit registration of the original are what
// the compiler emits for a static local with a non-trivial constructor and destructor.
MenuCharMenu_InvMain* MenuCharMenu_InvMain::GetInstance() {
    static MenuCharMenu_InvMain instance;
    return &instance;
}

// 0x452b1c _ZN20MenuCharMenu_InvMain4ShowEv
// Registers the "avatarpane" display callback on the owning MenuFX (State +0x04, m_menu), with this menu as the user
// pointer, creates the avatar camera, then runs MenuBase::Show (non-virtual). The callback's return value is not used.
void MenuCharMenu_InvMain::Show() {
    m_menu->RegisterDisplayCallback("avatarpane", RenderCharacterPane, this);
    CreateAvatarCamera();
    MenuBase::Show();
}

// 0x4528a0 _ZN20MenuCharMenu_InvMain4HideEv
// Destroys the avatar camera, then runs MenuBase::Hide (non-virtual; the original branches to it).
void MenuCharMenu_InvMain::Hide() {
    DestroyAvatarCamera();
    MenuBase::Hide();
}

// 0x452810 _ZN20MenuCharMenu_InvMain19DestroyAvatarCameraEv
// PARTIAL. Reconstructed: the camera is removed from its scene (ISceneNode::remove, vtable +0x68), dropped, and the
// static pointer is cleared. Missing: the last step, CSceneManager::setActiveCamera(scene, unk_60) when MenuManager
// +0x60 (the saved camera) is set. The scene manager is IDevice +0x1c (dword_99F73C + 28), which dh2 does not declare.
void MenuCharMenu_InvMain::DestroyAvatarCamera() {
    if (!m_inventoryPaneCam) {
        return;
    }
    m_inventoryPaneCam->remove();                                                   // ISceneNode vtable +0x68
    static_cast<glitch::IReferenceCounted*>(m_inventoryPaneCam)->drop();
    m_inventoryPaneCam = nullptr;
    MenuManager* manager = MenuManager::GetInstance();
    if (manager->unk_60) {                                                          // MenuManager +0x60, the saved camera
        // PARTIAL: CSceneManager::setActiveCamera(scene, saved camera) is missing (scene is IDevice +0x1c).
    }
}

// 0x4528d0 _ZN20MenuCharMenu_InvMain18CreateAvatarCameraEv
// PARTIAL. Reconstructed: the camera (CCameraSceneNode, id -1, position (0, -800, 200), target (0, 0, 200), not
// far-to-infinity), the character lookup, the up vector, aspect ratio, FOV, near and far planes, one extra reference,
// and the idle state of the character.
// Missing:
//   1. MenuManager +0x60 = scene->m_activeCamera (CSceneManager +0xe4), when MenuManager +0x60 is clear
//      (the scene manager is IDevice +0x1c, see DestroyAvatarCamera);
//   2. scene->rootNode->addChild(camera) (CSceneManager +0x04, vtable +0x5c), before the character attachment;
//   3. visual->m_node->addChild(camera) (ISceneNode vtable +0x5c) on the character's model root, see the comment below;
//   4. scene->setActiveCamera(camera), after the attachments.
// The creation reference is not dropped here while the attachments (2 and 3) are missing: the drop would free the
// camera at once. Restore the drop after the two attachments, at the place it has in the original.
void MenuCharMenu_InvMain::CreateAvatarCamera() {
    if (m_inventoryPaneCam) {
        return;
    }
    MenuManager* manager = MenuManager::GetInstance();
    if (!manager->unk_60) {                                                         // MenuManager +0x60
        // PARTIAL: unk_60 = scene->m_activeCamera (CSceneManager +0xe4) is missing (step 1).
    }
    const glitch::core::vector3d<float> position{0.0f, -800.0f, 200.0f};            // v10 in the original
    const glitch::core::vector3d<float> target{0.0f, 0.0f, 200.0f};                 // v9 in the original
    m_inventoryPaneCam = new glitch::scene::CCameraSceneNode(-1, position, target, false);
    // PARTIAL: scene->rootNode->addChild(m_inventoryPaneCam) is missing (step 2).

    PlayerInfo* player = dword_99F76C->GetLocalPlayer(0, true);
    Character* character = player->unk_660;                                         // PlayerInfo +0x660
    if (character) {
        VisualObject* visual = character->m_visualObject;                           // GameObject +0x2d8
        // PARTIAL: if (visual) visual->m_node->addChild(m_inventoryPaneCam) is missing (step 3). RootSceneNode does not
        // declare its ISceneNode base yet, so addChild cannot be called on m_node.
        (void)visual;
    }
    // PARTIAL: the original drops the creation reference here; it is left out while steps 2 and 3 are missing.
    // PARTIAL: scene->setActiveCamera(m_inventoryPaneCam) is missing (step 4).

    m_inventoryPaneCam->setUpVector(glitch::core::vector3d<float>{0.0f, 0.0f, 1.0f});  // vtable +0x114
    m_inventoryPaneCam->setAspectRatio(1.66775239f);                                // vtable +0x138, bits 0x3fd578e9
    m_inventoryPaneCam->setFOV(0.558519006f);                                       // vtable +0x13c, bits 0x3f0efb1a
    ++static_cast<glitch::IReferenceCounted*>(m_inventoryPaneCam)->referenceCounter;  // one extra reference
    m_inventoryPaneCam->setNearValue(10.0f);                                        // vtable +0x130, bits 0x41200000
    m_inventoryPaneCam->setFarValue(1000.0f);                                       // vtable +0x134, bits 0x447a0000

    Character* local = dword_99F76C->GetLocalPlayer(0, true)->unk_660;              // PlayerInfo +0x660, read again
    if (local) {
        if (!local->SM_IsDead()) {                                                  // CharStateMachine at Character +0x4fc
            local->SM_SetIdleState(false);
        }
    }
}

// 0x452468 _ZN20MenuCharMenu_InvMain19RenderCharacterPaneERN7gameswf12render_stateEPv
// PARTIAL. Missing, in the original order:
//   1. The pane rectangle: RenderFX::Find("avatarpane", context) on the owning menu's MenuFX. The two-argument
//      overload RenderFX::Find(char const*, gameswf::character*) (0x7a8a84) is not declared in dh2/RenderFX.hpp.
//      Then GameSWFUtils::GetAbsoluteBoundingRect and the root's inverse pixel scales, truncated to integers.
//   2. The viewport switch. The video driver is IDevice +0x10 and its +0xcc word points at the viewport owner
//      minus 4. The owner's rectangle at +0x14 (four words) is saved, the pane rectangle is set through its vtable
//      +0x0c, the pane camera's aspect ratio (width over height) is set, and the active camera (CSceneManager +0xe4)
//      is set to the pane camera.
//   3. The scene update, CSceneManager::update((float)Application::GetDt(), false) (vtable +0x60), and the scene
//      time (CSceneManager +0x254) that RootSceneNode::PlainOnAnimate takes.
//   4. CSceneManager::drawAll(root) when the scene has an active camera (CSceneManager +0xe4), then the viewport
//      restore to the saved rectangle.
//   5. The ISceneNode calls on the model root (getRotation, Euler angles to radians, quaternion::set(x, y, -0.5),
//      setRotation, getPosition and setPosition(0, 0, 0)), because RootSceneNode does not declare its ISceneNode base.
// The scene manager and the driver are IDevice +0x1c and +0x10 (dword_99F73C), which dh2 does not declare.
// Reconstructed: the character update, updateAbsolutePosition, the attachment and camera steps, and SyncRotation.
void MenuCharMenu_InvMain::RenderCharacterPane(gameswf::render_state& render, void* menu) {
    (void)render;  // a1 is not read by the original
    (void)menu;    // the owning menu; only the missing steps 1 to 4 read it
    Level* level = Singleton<Application>::s_inst.GetCurrentLevel();
    PlayerInfo* player = dword_99F76C->GetLocalPlayer(0, true);
    Character* character = player->unk_660;                                         // PlayerInfo +0x660
    if (!character) {
        return;
    }
    character->CharAnimator::Update();                                              // CharAnimator at +0x49c
    // PARTIAL: scene->update(dt, false) and RootSceneNode::PlainOnAnimate(root, scene time) are missing (step 3).
    RootSceneNode* root = character->m_visualObject->m_node;                        // VisualObject +0x08
    // PARTIAL: the ISceneNode steps on root are missing (step 5): getPosition (vtable +0xa0, result discarded), then
    // setPosition(0, 0, 0) (+0xa4), getRotation (+0x98, twice), the Euler conversion of the rotation in degrees, the
    // radians scale 0.017453, quaternion::set(x, y, -0.5) and setRotation (+0x9c). The z component of the Euler angles
    // is converted by the original but never read again.
    root->updateAbsolutePosition(true);                                             // vtable +0xb8
    // PARTIAL: if (scene->activeCamera) scene->drawAll(root), then the viewport restore, are missing (step 4).
    character->GameObject::SetPosition(character->m_position, true);                // GameObject +0x160
    AnchorBase* anchor = character->m_cameraAnchor;                                 // GameObject +0x2e0
    if (anchor) {
        anchor->Reset();                                                            // vtable +0x08
        anchor->Update();                                                           // vtable +0x0c
    }
    CameraLevel* camera = level->m_camera;                                          // Level +0x128
    camera->SetTarget(character, 0);
    camera->CameraBase::SetActive();
    camera->Update();                                                               // vtable +0x10
    character->m_visualObject->SyncRotation();                                      // VisualObject::SyncRotation
}
