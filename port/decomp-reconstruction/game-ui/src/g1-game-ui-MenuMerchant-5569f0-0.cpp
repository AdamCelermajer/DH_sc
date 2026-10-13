// MenuMerchant bodies, batch g1-game-ui-MenuMerchant-5569f0-0 (part 1 of 1 of this class).
// Rebuilt from the Hex-Rays pseudocode and the assembly of libDungeonHunter2.so. Offsets in comments are the original
// 32-bit ARM offsets. Functions that are partial say in their comment which step is missing.
//
// Facts shared by the bodies:
//   - The class's own member is unk_c4 (+0xc4). The statics m_inventoryPaneCam and m_oldCam are defined here.
//   - "vtable +0xNN" in the comments is the offset at the call site in the pseudocode. decomp_vtable.py prints the same
//     slots 12 bytes higher for classes with a virtual base (the vtable header carries a virtual-base offset first, see
//     glitch-scene-collada-gui/scene/ISceneNode.hpp). Example: ISceneNode::remove is "+0x68" here and "+0x74" in the table.
//   - The compiler emits the vptr stores and the base-class destructor chain, so no body writes them. The deleting
//     destructor (D0, 0x433604) is compiler-generated and is not rebuilt.
//   - The PlayerManager is Singleton<Application>::s_inst.m_playerManager (the original's dword_99F76C, s_inst +0x40;
//     see dh2/global/Globals.hpp). dh2/global/IsMultiplayerGame.hpp declares dword_99F76C as an extern, but no game
//     source defines it (only test stand-ins do), so these bodies use the member.
//   - The scene manager is glitch::IDevice +0x1c (dword_99F73C + 28) and the video driver is IDevice +0x10. dh2 declares
//     neither on glitch::IDevice, so every step that uses them is missing and marked PARTIAL (layout requests).
//   - RootSceneNode (VisualObject +0x08) derives from glitch::collada::CRootSceneNode, which has no ISceneNode base in
//     the tree. The ISceneNode calls on it (addChild, getPosition, setPosition, getRotation, setRotation) are missing.

#include <array>

#include "dh2/Application.hpp"
#include "dh2/AnchorBase.hpp"
#include "dh2/CameraLevel.hpp"
#include "dh2/Character.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/GameSWFUtils.hpp"
#include "dh2/Level.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuFX.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/MenuMerchant.hpp"
#include "dh2/PlayerInfo.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/RenderFX.hpp"
#include "dh2/RootSceneNode.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/VisualObject.hpp"
#include "glitch-core-io/core/reference_counted.hpp"
#include "glitch-scene-collada-gui/scene/CCameraSceneNode.hpp"
#include "glitch-scene-collada-gui/scene/ICameraSceneNode.hpp"

// Statics. The avatar camera is created by CreateAvatarCamera and dropped by DestroyAvatarCamera and the destructor.
// m_oldCam is the scene's active camera saved when the avatar camera is created (not written by the reconstructed steps).
glitch::scene::CCameraSceneNode* MenuMerchant::m_inventoryPaneCam = nullptr;
glitch::scene::ICameraSceneNode* MenuMerchant::m_oldCam = nullptr;

// 0x432b38 _ZN12MenuMerchant19DestroyAvatarCameraEv
// PARTIAL. Reconstructed: the avatar camera is removed from its scene (ISceneNode::remove, vtable +0x68), its static
// reference is dropped, and the static pointer is cleared. Missing: the last step, CSceneManager::setActiveCamera(scene,
// m_oldCam), which the original makes unconditionally after the clear. The scene is glitch::IDevice +0x1c, which dh2
// does not declare (layout request).
void MenuMerchant::DestroyAvatarCamera() {
    if (!m_inventoryPaneCam) {
        return;
    }
    m_inventoryPaneCam->remove();                                                   // ISceneNode vtable +0x68
    static_cast<glitch::IReferenceCounted*>(m_inventoryPaneCam)->drop();
    m_inventoryPaneCam = nullptr;
    // PARTIAL: CSceneManager::setActiveCamera(scene, m_oldCam) is missing (scene = IDevice +0x1c).
}

// 0x432ee8 _ZN12MenuMerchant19RenderCharacterPaneERN7gameswf12render_stateEPv
// PARTIAL. Reconstructed: the pane rectangle from the "avatarpane" movie clip, truncated to integer pixels; the pane
// camera's aspect ratio (width over height); the local character's animator update; the model root's absolute position
// update; the character's position and camera anchor; the camera target, activation and update; and SyncRotation.
// Missing, in the original order:
//   1. The viewport switch. The video driver is IDevice +0x10 (dword_99F73C + 16) and its +0xcc word points at the
//      viewport owner minus 4. The owner's rectangle (+0x14, four words) is saved, the pane rectangle is set through
//      the owner's vtable (+0x0c), and the saved rectangle is restored on the way out.
//   2. scene->setActiveCamera(m_inventoryPaneCam), with scene = IDevice +0x1c.
//   3. The scene update, CSceneManager::update(Application::GetDt(), false) (vtable +0x60), and the scene time
//      (CSceneManager +0x254) that RootSceneNode::PlainOnAnimate takes.
//   4. The model root's ISceneNode steps: getRotation and Euler angles (converted to radians with 0.017453 on x and y),
//      getPosition, setPosition({0, 0, 0}), quaternion::set(x, y, -0.5), and setRotation.
//   5. CSceneManager::drawAll(root) when the scene has an active camera (CSceneManager +0xe4).
// The early return for a missing character skips the viewport restore, which is missing (step 1).
void MenuMerchant::RenderCharacterPane(gameswf::render_state& state, void* user) {
    (void)state;  // a1 is not read by the original
    MenuMerchant* menu = static_cast<MenuMerchant*>(user);  // the MenuMerchant (this) given to RegisterDisplayCallback
    RenderFX* fx = menu->m_menu;                            // MenuBase +0x04, the owning MenuFX
    gameswf::character* context = menu->GetCurrentMenuContext();
    gameswf::character* pane = fx->Find("avatarpane", context);
    const std::array<float, 4> bounds = GameSWFUtils::GetAbsoluteBoundingRect(pane);  // xmin, xmax, ymin, ymax
    gameswf::root* root = fx->GetRoot();
    const float invScaleX = GameSWFUtils::GetInvPixelScaleX(root);
    root = fx->GetRoot();
    const float invScaleY = GameSWFUtils::GetInvPixelScaleY(root);
    // Integer pixel rectangle in the original order: v30 = xmin, v31 = ymin, v32 = xmax, v33 = ymax (truncated).
    const int paneRect[4] = {
        static_cast<int>(bounds[0] / invScaleX),
        static_cast<int>(bounds[2] / invScaleY),
        static_cast<int>(bounds[1] / invScaleX),
        static_cast<int>(bounds[3] / invScaleY),
    };
    // PARTIAL: step 1, the viewport switch (saved rectangle and the pane rectangle set on the viewport owner), is missing.
    m_inventoryPaneCam->setAspectRatio(static_cast<float>(paneRect[2] - paneRect[0]) /
                                       static_cast<float>(paneRect[3] - paneRect[1]));  // vtable +0x138
    // PARTIAL: step 2, scene->setActiveCamera(m_inventoryPaneCam), is missing.

    Level* level = Singleton<Application>::s_inst.GetCurrentLevel();
    Character* character = Singleton<Application>::s_inst.m_playerManager->GetLocalPlayer(0, true)->unk_660;  // PlayerInfo +0x660
    if (!character) {
        // PARTIAL: the original restores the saved viewport here (step 1) before it returns.
        return;
    }
    character->CharAnimator::Update();                                              // CharAnimator at Character +0x49c
    // PARTIAL: steps 3 and 4 are missing: the scene update and PlainOnAnimate, the root's getRotation, Euler conversion,
    // getPosition, setPosition({0, 0, 0}), quaternion::set(x, y, -0.5) and setRotation.
    RootSceneNode* model = character->m_visualObject->m_node;                       // VisualObject +0x08 (+0x2d8 of the character)
    model->updateAbsolutePosition(true);                                            // vtable +0xb8
    // PARTIAL: step 5, if (scene->activeCamera) scene->drawAll(model), and the viewport restore (step 1) are missing.
    character->GameObject::SetPosition(character->m_position, true);                // GameObject +0x160
    AnchorBase* anchor = character->m_cameraAnchor;                                 // GameObject +0x2e0
    if (anchor) {
        anchor->Reset();                                                            // vtable +0x08
        anchor->Update();                                                           // vtable +0x0c
    }
    CameraLevel* camera = level->m_camera;                                          // Level +0x128
    camera->CameraTarget::SetTarget(character, 0);
    camera->CameraBase::SetActive();
    camera->Update();                                                               // vtable +0x10
    character->m_visualObject->SyncRotation();
}

// 0x4332b0 _ZN12MenuMerchant4HideEv
// Destroys the avatar camera, runs MenuBase::Hide (non-virtual), then saves the local player's character when there is
// one. The original reads the player a second time for the save, and so does this body. DestroyAvatarCamera is partial
// (0x432b38), and MenuBase::Hide is partial in this tree (0x424af4); this body itself matches the pseudocode.
void MenuMerchant::Hide() {
    DestroyAvatarCamera();
    MenuBase::Hide();
    if (Singleton<Application>::s_inst.m_playerManager->GetLocalPlayer(0, true)->unk_660) {
        Singleton<Application>::s_inst.m_playerManager->GetLocalPlayer(0, true)->unk_660->SG_Save();  // Character::SG_Save 0x3bc4a8
    }
}

// 0x433318 _ZN12MenuMerchant18CreateAvatarCameraEv
// PARTIAL. Reconstructed: the camera (CCameraSceneNode, id -1, position (0, -800, 200), target (0, 0, 200), not
// far-to-infinity), the character lookup, the up vector, aspect ratio, FOV, near and far planes, one extra reference, and
// the idle state of the character.
// Missing, in the original order:
//   1. m_oldCam = scene->activeCamera (CSceneManager +0xe4), when the static camera is clear. The scene is IDevice +0x1c.
//   2. scene->rootNode->addChild(camera) (CSceneManager +0x04, ISceneNode vtable +0x5c).
//   3. visual->m_node->addChild(camera) on the character's model root (see the RootSceneNode note above).
//   4. The drop of the creation reference after steps 2 and 3. It is not done here: while the attachments are missing,
//      the drop would free the camera at once.
//   5. scene->setActiveCamera(camera), after the attachments.
void MenuMerchant::CreateAvatarCamera() {
    if (m_inventoryPaneCam) {
        return;
    }
    // PARTIAL: step 1, m_oldCam = scene->activeCamera, is missing.
    const glitch::core::vector3d<float> position{0.0f, -800.0f, 200.0f};            // v9: 0, 0xC4480000, 0x43480000
    const glitch::core::vector3d<float> target{0.0f, 0.0f, 200.0f};                 // v8: 0, 0, 0x43480000
    m_inventoryPaneCam = new glitch::scene::CCameraSceneNode(-1, position, target, false);
    // PARTIAL: steps 2 to 5 are missing: the addChild calls on the scene root and on the character's model root, the
    // drop of the creation reference, and scene->setActiveCamera.

    Character* character = Singleton<Application>::s_inst.m_playerManager->GetLocalPlayer(0, true)->unk_660;
    if (character) {
        VisualObject* visual = character->m_visualObject;                           // GameObject +0x2d8
        // PARTIAL: if (visual) visual->m_node->addChild(m_inventoryPaneCam) is missing (step 3).
        (void)visual;
    }

    m_inventoryPaneCam->setUpVector(glitch::core::vector3d<float>{0.0f, 0.0f, 1.0f});  // vtable +0x114, v7 = (0, 0, 1)
    m_inventoryPaneCam->setAspectRatio(1.66775239f);                                // vtable +0x138, bits 0x3fd578e9
    m_inventoryPaneCam->setFOV(0.558519006f);                                       // vtable +0x13c, bits 0x3f0efb1a
    ++static_cast<glitch::IReferenceCounted*>(m_inventoryPaneCam)->referenceCounter;  // one extra reference (+0x04)
    m_inventoryPaneCam->setNearValue(10.0f);                                        // vtable +0x130, bits 0x41200000
    m_inventoryPaneCam->setFarValue(1000.0f);                                       // vtable +0x134, bits 0x447a0000

    Character* local = Singleton<Application>::s_inst.m_playerManager->GetLocalPlayer(0, true)->unk_660;  // read again
    if (local) {
        if (!local->CharStateMachine::SM_IsDead()) {                                // CharStateMachine at Character +0x4fc
            local->CharStateMachine::SM_SetIdleState(false);
        }
    }
}

// 0x433548 _ZN12MenuMerchant6UpdateEv
// Clears the menu's own flag (unk_c4 at +0xc4) every frame, then runs MenuBase::Update (non-virtual; partial in this
// tree, see 0x421fe0).
void MenuMerchant::Update() {
    unk_c4 = false;
    MenuBase::Update();
}

// 0x433554 _ZN12MenuMerchant4ShowEv
// Registers the "avatarpane" display callback on the owning MenuFX (MenuBase +0x04), with this menu as the user pointer,
// creates the avatar camera, then runs MenuBase::Show (non-virtual; partial in this tree, see 0x425450). The return value
// of RegisterDisplayCallback is not used; the original passes it on to CreateAvatarCamera, which ignores it.
void MenuMerchant::Show() {
    RenderFX* fx = m_menu;
    fx->RegisterDisplayCallback("avatarpane", RenderCharacterPane, this);
    CreateAvatarCamera();
    MenuBase::Show();
}

// 0x43359c _ZN12MenuMerchantD1Ev (complete-object destructor)
// 0x433620 _ZN12MenuMerchantD2Ev (base-object destructor, the same bytes)
// Drops the static avatar camera and clears the pointer. The vptr store is the compiler's, and MenuBase::~MenuBase runs
// after the body (implicit here). One definition serves D1 and D2.
MenuMerchant::~MenuMerchant() {
    if (m_inventoryPaneCam) {
        static_cast<glitch::IReferenceCounted*>(m_inventoryPaneCam)->drop();
        m_inventoryPaneCam = nullptr;
    }
}

// 0x433604 _ZN12MenuMerchantD0Ev
// Compiler-generated deleting destructor: runs ~MenuMerchant, then CustomFree(this). The delete expression emits it, so
// no body is written here.

// 0x433688 _ZN12MenuMerchantC1Ev (complete-object constructor)
// 0x4336d4 _ZN12MenuMerchantC2Ev (base-object constructor, the same bytes)
// MenuBase("menu_Merchant"), then RegisterMenu(this) on the MenuManager singleton. The vptr store is the compiler's.
// RegisterMenu has an empty body in this tree (partial, see 0x42ee94), so the registration itself is not yet performed.
// One definition serves C1 and C2.
MenuMerchant::MenuMerchant() : MenuBase("menu_Merchant") {
    MenuManager::GetInstance()->RegisterMenu(this);
}

// 0x433720 _ZN12MenuMerchant11GetInstanceEv
// Function-local static, constructed on first use. The guard variable, the constructor call and the atexit registration
// of the destructor are what the compiler emits for a static local with a non-trivial constructor and destructor.
MenuMerchant* MenuMerchant::GetInstance() {
    static MenuMerchant instance;
    return &instance;
}
