// MenuMinimap bodies, batch g1-game-ui-MenuMinimap-b3671b-0 (13 rows: the whole class).
// The "menu_MapSheet" map page is a HUDMinimap that adds no members. It overrides Show, RegisterDisplayCallback,
// RegisterToMenuManager, InitAll, CreateMapCamera and DisableMapCamera. Its RenderMap is the display callback that
// RegisterDisplayCallback registers as "RenderMap".
// Rebuilt from the Hex-Rays pseudocode of libDungeonHunter2.so. Offsets in comments are the original 32-bit ARM offsets
// and are never used for addressing.
//
// PARTIAL rows: DisableMapCamera, CreateMapCamera and RenderMap. Each partial function says which part is missing.
// Layout requests behind them (format "<Class> +0xNN <type> <evidence>"; the integrator applies them to the owner headers):
//   glitch::IDevice +0x10 <object pointer>
//       RenderMap reads it through Singleton<Application>::s_inst.m_device (Application +0x10, dword_99F73C + 16). The
//       object's +0xcc is the end of a pointer array, and its last element has a vtable slot +0x0c that takes an int[4]
//       rect and holds the old rect at +0x14 .. +0x20 (viewport swap in RenderMap). Not declared: glitch-core-io
//       os/device.hpp has IDevice fields only from +0x28 on.
//   glitch::IDevice +0x1c glitch::scene::CSceneManager*
//       The scene manager (dword_99F73C + 28). Same request as game-ui HUDMinimap-93879d-0 (HUDMinimap::Show, Update, Hide).
//   CSceneManager +0xe4 <pointer>
//       Active camera. RenderMap tests it for null before the scene vtable +0x3c call (dword +228).
//   CSceneManager +0x250 uint8_t
//       Flag set to 0 before SetRoomVisibility and to 1 after the viewport restore (dword +592 of the scene manager).
//   CSceneManager vtable +0x3c <slot taking an ISceneNode*>
//       Called with the map node when CSceneManager +0xe4 is set (RenderMap). Not declared in layouts/scene.hpp.
//   Arrays::CamAnimSetTable <typed accessor>
//       CreateMapCamera needs the row of "MiniMap" (name table m_memberNames, pydata-accessors member_names.hpp) and the
//       clip id at +0x10 of the 28-byte row (members, size: w7_arrays_02.hpp). The two declarations cannot be included
//       together, and Structs::CamAnimSet is not declared.
//
// Destructor D0 (0x435088) and the scalar deleting form are compiler-generated and have no body here (row skipped).

#include <array>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "dh2/Application.hpp"
#include "dh2/CameraBase.hpp"
#include "dh2/CameraLevel.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/GameSWFUtils.hpp"
#include "dh2/Level.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/MenuMinimap.hpp"
#include "dh2/RenderFX.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/ZoomHandler.hpp"

// Global assert level (libDungeonHunter2.so gAssertLevel). Declared the same way as in the HUDMinimap bodies.
extern int gAssertLevel;

namespace {

// The "IsDisablingMinimap" debug switch, read the same way as in HUDMinimap. Hex-Rays prints the name passed by Show and
// RenderMap as an empty string, but the assembly loads the literal "IsDisablingMinimap" (18 bytes, end at +0x12 of the
// string), so both functions read the same switch as HUDMinimap.
bool IsMinimapDisabled() {
    DebugSwitches::s_inst.load();
    std::string name("IsDisablingMinimap");
    return DebugSwitches::s_inst.GetSwitch(name);
}

}  // namespace

// 0x434dc0 _ZN11MenuMinimap7InitAllEv
// The same steps as HUDMinimap::InitAll, but each one is a virtual call through this object, so the MenuMinimap overrides
// are reached: RegisterToMenuManager (vtable +0x7c), InitAllIcons (+0x70), InitAllObjects (+0x5c).
void MenuMinimap::InitAll() {
    RegisterToMenuManager();  // vtable +0x7c
    InitAllIcons();           // vtable +0x70
    InitAllObjects();         // vtable +0x5c
}

// 0x434df8 _ZN11MenuMinimap16DisableMapCameraEv
// PARTIAL: when EnableGameCamera (vtable +0x94) returns null, this returns null like the original. When it returns a camera,
// the original calls ZoomHandler::setCamera with that camera (passed as a CameraLevel*) and returns the float that call gives
// (the zoom sensitivity, carried in r0). This override is typed CameraBase* like HUDMinimap::DisableMapCamera, so that value
// cannot be carried and nullptr is returned on that path instead. The only caller in the tree, HUDMinimap::Hide, discards it.
CameraBase* MenuMinimap::DisableMapCamera() {
    CameraBase* camera = EnableGameCamera();  // vtable +0x94
    if (camera == nullptr) {
        return nullptr;
    }
    // The original passes the pointer on without a check. Level::m_camera is a CameraLevel, so the downcast is valid.
    Singleton<Application>::s_inst.m_zoomHandler->setCamera(static_cast<CameraLevel*>(camera));  // dword_99F77C = s_inst +0x50
    return nullptr;  // PARTIAL: the binary returns the float from setCamera on this path (see above)
}

// 0x434e38 _ZN11MenuMinimap15CreateMapCameraEv
// Creates the map camera once. If it exists, nothing else happens (the original then returns this, and in the other path
// PlayAnim's bool result; the slot is void, so neither value is returned here).
// PARTIAL: the original then calls CameraLevel::Load(m_mapCam, "data/3D/camera/CameraTests.bdae", index,
// "PlayerCamera_Default") with index = the row of "MiniMap" in Arrays::CamAnimSetTable (-1 when absent), and at the end
// CameraLevel::PlayAnim(clip, 0, false) with clip = the +0x10 word of the row at m_camTableIndex (28-byte rows). Neither call
// is made. The table has two declarations in this tree (see the file comment), so the row search cannot be written. The
// setup calls that remain run in the original order, except that the flags below stand in front of the missing Load.
void MenuMinimap::CreateMapCamera() {
    if (m_mapCam != nullptr) {
        return;
    }
    m_mapCam = new CameraLevel();  // operator new(0xa8) and the CameraLevel constructor
    if (m_mapCam == nullptr) {
        // ASSERT(m_mapCam) failed. Same policy as the other gAssertLevel asserts in the tree (see HUDMinimap::CreateMapCamera).
        if (gAssertLevel == 2) {
            std::abort();  // the original writes address 0 here to force a crash
        } else if (gAssertLevel == 1) {
            std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "m_mapCam",
                         "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Menus\\MenuMinimap.cpp", 179);
        }
    }
    // PARTIAL: CameraLevel::Load(...) is not made (see above).
    m_mapCam->m_useObjectPosition = true;                              // +0x85 (byte set to 1)
    m_mapCam->EnableDamping(false);                                    // CameraTarget::EnableDamping(m_mapCam, 0)
    m_mapCam->SetData(0.42963f, 1.7778f, 0.0f, 100000.0f, false);      // CameraBase::SetData; aspect 1.7778 (HUDMinimap uses 1.0)
    m_mapCam->m_zoomTarget = 1.0f;                                     // +0x8c
    m_mapCam->m_zoomCurrent = 0.0f;                                    // +0x88
    // PARTIAL: CameraLevel::PlayAnim(clip, 0, false) is not made (see above).
}

// 0x435054 _ZN11MenuMinimapD1Ev
// 0x4350a4 _ZN11MenuMinimapD2Ev
// Identical bodies (Hex-Rays and asm agree), so one definition serves both. The original sets the vptr to the MenuMinimap
// vtable and calls ~HUDMinimap. The compiler emits both for an empty body.
MenuMinimap::~MenuMinimap() {}

// 0x4350d8 _ZN11MenuMinimapC1Ev
// 0x43519c _ZN11MenuMinimapC2Ev
// Identical bodies, so one definition serves both. The base is HUDMinimap("menu_MapSheet"); the compiler stores the
// MenuMinimap vptr after the base constructor, as the original does. MenuMinimap adds no members.
MenuMinimap::MenuMinimap() : HUDMinimap("menu_MapSheet") {}

// 0x435118 _ZN11MenuMinimap11GetInstanceEv
// Meyers singleton: the static local is a MenuMinimap built with "menu_MapSheet" (HUDMinimap::GetInstance has its own).
// The compiler emits the guard and the atexit registration of ~MenuMinimap that the original calls by hand.
MenuMinimap* MenuMinimap::GetInstance() {
    static MenuMinimap mdt;
    return &mdt;
}

// 0x43500c _ZN11MenuMinimap23RegisterDisplayCallbackEv
// Registers RenderMap as the display callback "RenderMap" of the owning MenuFX (+0x04), with this as the user pointer.
void MenuMinimap::RegisterDisplayCallback() {
    m_menu->RegisterDisplayCallback("RenderMap", &MenuMinimap::RenderMap, this);
}

// 0x43503c _ZN11MenuMinimap21RegisterToMenuManagerEv
void MenuMinimap::RegisterToMenuManager() {
    MenuManager::GetInstance()->RegisterMenu(this);
}

// 0x435308 _ZN11MenuMinimap4ShowEv
// With the IsDisablingMinimap switch set, only MenuBase::Show runs. Otherwise HUDMinimap::Show runs as a direct (non-virtual)
// call, then the zoom handler is bound to the map camera and its mouse pan flag (ZoomHandler +0x24) is set. The original
// returns the float from ZoomHandler::setCamera. The slot is void and no caller reads that value, so it is dropped.
void MenuMinimap::Show() {
    if (IsMinimapDisabled()) {
        MenuBase::Show();  // non-virtual call, as in the original
        return;
    }
    HUDMinimap::Show();  // non-virtual call, as in the original
    ZoomHandler* zoom = Singleton<Application>::s_inst.m_zoomHandler;  // dword_99F77C = s_inst +0x50
    static_cast<void>(zoom->setCamera(m_mapCam));  // m_mapCam is read after HUDMinimap::Show, which may create it
    zoom->m_mousePanEnabled = true;                // ZoomHandler +0x24
}

// 0x4353e4 _ZN11MenuMinimap9RenderMapERN7gameswf12render_stateEPv
// The display callback registered as RenderMap. The user pointer is the MenuMinimap. The original takes it as a MenuBase* and
// reads HUDMinimap fields (m_mapCam +0xe0, unk_c4 +0xc4), so this body reads them through MenuMinimap.
// PARTIAL: the steps that touch the scene manager and the device are missing (layout requests above). The original order:
//   (a) node = scene manager +0x28c; node->setVisible(true) when set                              MISSING
//   (b) EnableMapCamera (vtable +0x8c)                                                           done
//   (c) clip = RenderFX::Find("RenderMap", context); its absolute bounds                          done
//   (d) the device +0x10 object's viewport rect saved (+0x14 .. +0x20 of its last array element)  MISSING
//   (e) the minimap rect in device pixels, from the bounds and the inverse pixel scales          done (feeds f)
//   (f) the viewport set to the rect of (e) through the element's vtable +0x0c                    MISSING
//   (g) scene +0x250 = 0                                                                         MISSING
//   (h) SetRoomVisibility(node) (vtable +0x64)                                                   MISSING
//   (i) SetObjectsVisibility(level) (vtable +0x60)                                               done
//   (j) when node is set and scene +0xe4 is set: scene vtable +0x3c (node)                       MISSING
//   (k) scene +0x250 = 1                                                                         MISSING
//   (l) the viewport restored from (d)                                                           MISSING
//   (m) m_mapCam->Update() (CameraBase slot +0x10)                                               done
//   (n) EnableGameCamera (vtable +0x94)                                                          done
//   (o) node->setVisible(false) when node is set                                                 MISSING
void MenuMinimap::RenderMap(gameswf::render_state& /*render*/, void* menu) {
    MenuMinimap* map = static_cast<MenuMinimap*>(menu);
    if (IsMinimapDisabled()) {
        return;
    }
    Level* level = Singleton<Application>::s_inst.GetCurrentLevel();
    if (level == nullptr || level->m_loadStep != 38) {  // loaded level (38)
        return;
    }
    if (map == nullptr || map->m_mapCam == nullptr) {
        return;
    }
    if (!map->unk_c4 || map->m_mapCam->m_zoomAnimActive) {  // unk_c4 set and no zoom animation running
        return;
    }
    // PARTIAL (a): node = scene manager (device +0x1c)->m_nodeMapRoot, setVisible(true) when set. Missing (device +0x1c).
    map->EnableMapCamera();  // (b) vtable +0x8c
    gameswf::character* clip = map->m_menu->Find("RenderMap", map->GetCurrentMenuContext());  // RenderFX::Find(menu, "RenderMap", context)
    const std::array<float, 4> bounds = GameSWFUtils::GetAbsoluteBoundingRect(clip);       // xmin, xmax, ymin, ymax
    gameswf::root* root = map->m_menu->GetRoot();
    const float invScaleX = GameSWFUtils::GetInvPixelScaleX(root);
    const float invScaleY = GameSWFUtils::GetInvPixelScaleY(root);
    // (e) the minimap rect in device pixels: left, top, right, bottom, each float quotient truncated to int.
    const std::array<int, 4> viewport{static_cast<int>(bounds[0] / invScaleX), static_cast<int>(bounds[2] / invScaleY),
                                      static_cast<int>(bounds[1] / invScaleX), static_cast<int>(bounds[3] / invScaleY)};
    // PARTIAL (d), (f), (l): the active camera's viewport is saved, set to viewport and restored. Missing (device +0x10).
    static_cast<void>(viewport);
    // PARTIAL (g), (h), (j), (k): scene flags, SetRoomVisibility(node) and the scene vtable +0x3c call. Missing (device +0x1c).
    map->SetObjectsVisibility(level);  // (i) vtable +0x60
    // PARTIAL (k), (l): scene flag set to 1 and the viewport restored. Missing.
    map->m_mapCam->Update();   // (m) CameraLevel::Update, vtable +0x10
    map->EnableGameCamera();   // (n) vtable +0x94, the result is discarded
    // PARTIAL (o): node->setVisible(false) when node is set. Missing.
}
