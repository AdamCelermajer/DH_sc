// HUDMinimap bodies, batch g1-game-ui-HUDMinimap-93879d-1 (part 2 of 2 of HUDMinimap).
// Rebuilt from the Hex-Rays pseudocode of libDungeonHunter2.so. Offsets in comments are the original 32-bit ARM
// offsets. PARTIAL functions say in their comment which part is missing.
//
// Missing piece behind the partial functions: the scene manager. The original reads it as the device field at +0x1c
// (dword_99F73C + 28, where dword_99F73C is Singleton<Application>::s_inst.m_device). glitch::IDevice does not declare
// that field, so Show, Update and RenderMap cannot reach the map node (SceneManager +0x28c, dh2/SceneManager.hpp).

#include <array>
#include <cstddef>
#include <list>
#include <string>

#include "gameswf/gameswf.h"            // gameswf::rect (complete type; MenuBase.hpp forward-declares it)
#include "gameswf/gameswf_character.h"  // gameswf::character and as_object (complete types for the icon casts)
#include "dh2/Application.hpp"
#include "dh2/CameraLevel.hpp"
#include "dh2/CameraTarget.hpp"
#include "dh2/Character.hpp"  // Character derives from GameObject, which CameraTarget::SetTarget takes
#include "dh2/DebugCachedCharacter.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/GameSWFUtils.hpp"
#include "dh2/HUDMinimap.hpp"
#include "dh2/Level.hpp"
#include "dh2/MenuFX.hpp"
#include "dh2/PlayerInfo.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/Point3D.hpp"
#include "dh2/Singleton.hpp"

namespace {

// The "IsDisablingMinimap" debug switch, read the same way in Show, Update and RenderMap: DebugSwitches::load on the
// singleton, then GetSwitch with a temporary string.
bool IsMinimapDisabled() {
    DebugSwitches::s_inst.load();
    std::string name("IsDisablingMinimap");
    return DebugSwitches::s_inst.GetSwitch(name);
}

// Sets x, y and z of each element to zero. The original does this with three word stores per element.
template <std::size_t N>
void ZeroPoints(Point3D<float> (&points)[N]) {
    for (Point3D<float>& p : points) {
        p.X = 0.0f;
        p.Y = 0.0f;
        p.Z = 0.0f;
    }
}

}  // namespace

// 0x41cbe0 _ZN10HUDMinimapC1EPKc
// 0x41cd74 _ZN10HUDMinimapC2EPKc
// Complete-object and base-object constructors have the same body (asm and Hex-Rays agree), so one definition serves
// both. The compiler does the list and DebugCachedCharacter initialisation: m_visitedRooms (+0xe4) starts as an empty
// list, and m_renderMap (+0x19c) runs DebugCachedCharacter(). The original does not clear the icon pointers unk_ec
// to unk_16c or the object pointers unk_3dc and unk_3fc, so they stay unset here as well.
HUDMinimap::HUDMinimap(const char* name) : MenuBase(name) {
    m_mapCam = nullptr;   // +0xe0
    unk_c4 = false;       // +0xc4
    unk_c8 = 0;           // +0xc8
    unk_cc = 0;           // +0xcc
    unk_d0 = 0;           // +0xd0
    unk_d4 = 0;           // +0xd4
    unk_d8 = 0;           // +0xd8
    unk_dc = 0;           // +0xdc
    ZeroPoints(unk_1cc);  // +0x1cc .. +0x1fc
    ZeroPoints(unk_1fc);  // +0x1fc .. +0x25c
    ZeroPoints(unk_25c);  // +0x25c .. +0x2bc
    ZeroPoints(unk_2bc);  // +0x2bc .. +0x34c
    ZeroPoints(unk_34c);  // +0x34c .. +0x3dc
}

// 0x41c17c _ZN10HUDMinimapD1Ev
// 0x41c20c _ZN10HUDMinimapD2Ev
// Identical bodies (no virtual bases), so one definition serves both. The original body is empty apart from the
// implicit parts: it sets the vptr, destroys m_renderMap (DebugCachedCharacter at +0x19c), frees the visited-room list
// nodes (m_visitedRooms at +0xe4) and then runs MenuBase::~MenuBase. The compiler destroys members in reverse
// declaration order, and that is the same order, so the body is empty.
HUDMinimap::~HUDMinimap() {}

// 0x41cce0 _ZN10HUDMinimap11GetInstanceEv
// Meyers singleton: the static local is HUDMinimap built with "menu_miniMap". The compiler emits the same guard and
// atexit registration that the original calls (_cxa_guard_acquire, _aeabi_atexit with ~HUDMinimap).
HUDMinimap* HUDMinimap::GetInstance() {
    static HUDMinimap mdt("menu_miniMap");
    return &mdt;
}

// 0x41c9e4 _ZN10HUDMinimap4ShowEv
// PARTIAL: the head of Show is rebuilt. After MenuBase::Show the original takes v9 = scene manager (device +0x1c)
// ->m_nodeMapRoot (+0x28c). If v9 is set it runs: v9 vtable +0x48 (false), unk_c4 = 1, SetRoomVisibility(v9),
// SetEnemies, SetObjects, SetObjectives, SetIconFrames(this), then ResetTranslation (+0x98). None of that tail is
// written, because the device +0x1c field is not declared (layout request).
void HUDMinimap::Show() {
    if (IsMinimapDisabled()) {
        MenuBase::Show();
        return;
    }
    RegisterDisplayCallback();                                                  // +0x78
    m_renderMap.RefreshCache("RenderMap", m_menu, GetCurrentMenuContext());     // "RenderMap" clip: menu movie, current menu context as root
    unk_c8 = Singleton<Application>::s_inst.m_playerManager->m_numCharacters;   // dword_99F76C + 1732 (+0x6c4)
    HideAllIcons();                                                             // +0x6c
    CreateMapCamera();                                                          // +0x84
    if (m_mapCam) {
        PlayerInfo* local = Singleton<Application>::s_inst.m_playerManager->GetLocalPlayer(0, true);
        m_mapCam->SetTarget(local->unk_660, 0);                                 // CameraTarget::SetTarget, transition 0
        m_mapCam->Update();                                                     // CameraLevel::Update, vtable +0x10
    }
    MenuBase::Show();
    // PARTIAL: the tail that depends on the scene manager's map node (see the comment above) is missing.
}

// 0x41c7b0 _ZN10HUDMinimap6UpdateEv
// PARTIAL: the disabled branch is complete. In the enabled branch the original reads v5 = scene manager (device +0x1c)
// ->m_nodeMapRoot before EnableMapCamera, calls v5 vtable +0x48 (false) when it is set, and after the camera target
// calls the scene manager's vtable +0x3c (v5) when it is set. Both calls are missing (device +0x1c is not declared).
void HUDMinimap::Update() {
    if (IsMinimapDisabled()) {
        MenuBase::Update();
        HideAllIcons();  // +0x6c
        return;
    }
    MenuBase::Update();
    Level* level = Singleton<Application>::s_inst.GetCurrentLevel();
    if (!level || level->m_loadStep != 38 || !m_mapCam || !unk_c4) {  // loaded level (38), map camera, unk_c4
        return;
    }
    unk_c8 = Singleton<Application>::s_inst.m_playerManager->m_numCharacters;  // +0xc8 from +0x6c4
    HideAllIcons();                                                            // +0x6c
    // PARTIAL: v5 = scene manager (device +0x1c)->m_nodeMapRoot; if (v5) v5 vtable +0x48 (false) is missing.
    EnableMapCamera();                                                         // +0x8c
    std::array<float, 4> bounds = GameSWFUtils::GetAbsoluteBoundingRect(m_renderMap.GetChar());
    gameswf::rect viewRect;
    viewRect.m_x_min = bounds[0];
    viewRect.m_x_max = bounds[1];
    viewRect.m_y_min = bounds[2];
    viewRect.m_y_max = bounds[3];
    SetEnemies();      // +0x50
    SetObjectives();   // +0x58
    if (m_mapCam) {
        PlayerInfo* local = Singleton<Application>::s_inst.m_playerManager->GetLocalPlayer(0, true);
        m_mapCam->SetTarget(local->unk_660, 0);
        m_mapCam->Update();  // vtable +0x10
    }
    // PARTIAL: if (v5) the scene manager's vtable +0x3c (v5) is missing here, see above.
    UpdateIconPositions(this, viewRect);  // +0x74
    EnableGameCamera();                   // +0x94
}

// 0x41c4a0 _ZN10HUDMinimap9RenderMapERN7gameswf12render_stateEPv
// The display callback registered as "RenderMap"; the user pointer is the HUDMinimap. PARTIAL: the original does the
// following in order: (1) when the scene manager's map node v7 is set, v7 vtable +0x48 (true); (2) EnableMapCamera
// (done here); (3) the clip bounds (computed here, the value only feeds step 4); (4) the active camera's viewport is
// swapped to the minimap rect in device pixels, with the camera taken from device +0x10 and its pointer array at +0xcc,
// and the scale comes from RenderFX::GetRoot and GameSWFUtils::GetInvPixelScale; (5) SetRoomVisibility(v7) (not
// done); (6) SetObjectsVisibility(level) (done here); (7) the viewport and the scene manager flags are restored and
// then EnableGameCamera (done here) runs. Steps 1, 4, 5 and 7 need the device layout, so they are missing.
void HUDMinimap::RenderMap(gameswf::render_state& /*render*/, void* menu) {
    HUDMinimap* map = static_cast<HUDMinimap*>(menu);
    if (IsMinimapDisabled()) {
        return;
    }
    Level* level = Singleton<Application>::s_inst.GetCurrentLevel();
    if (!level || level->m_loadStep != 38) {
        return;
    }
    if (!map || !map->m_mapCam) {
        return;
    }
    if (!map->unk_c4 || map->m_mapCam->m_zoomAnimActive) {
        return;
    }
    map->EnableMapCamera();                                                  // +0x8c
    gameswf::character* clip = map->m_renderMap.GetChar();                  // DebugCachedCharacter::GetChar (asserts when unset)
    GameSWFUtils::GetAbsoluteBoundingRect(clip);                            // feeds the viewport step, which is missing
    // PARTIAL: steps (1), (4), (5) and the restore in (7) are missing (device +0x10 and +0x1c, see above).
    map->SetObjectsVisibility(level);                                       // +0x60
    map->EnableGameCamera();                                                // +0x94
}

// 0x41cff4 _ZN10HUDMinimap13SetIconFramesEPS_
// Builds the 44 icon characters from the names "MapIconsStatic<n>" in the menu's movie (menu->m_menu, the RenderFX).
// The counter unk_dc is set to 5 and incremented before each lookup, so the names run from 6 upward. The five groups,
// in the original order: unk_ec (4 icons, frame 3, lookup result null-checked before the frame call), unk_16c (12
// icons, frame 4), unk_13c (12, frame 0), unk_fc (8, frame 1) and unk_11c (8, frame 2). The frame call is
// character::goto_frame(int), vtable slot +0x14c (GameSWFUtils::GotoFrame calls the same slot). Only the first group
// tests the lookup for null, and the others call the frame method directly, as the original does.
void HUDMinimap::SetIconFrames(HUDMinimap* menu) {
    unk_dc = 5;  // +0xdc
    auto findIcon = [this, menu]() {
        std::string name = "MapIconsStatic" + std::to_string(++unk_dc);
        return static_cast<gameswf::character*>(menu->m_menu->Find(name.c_str()));
    };
    for (int i = 0; i < 4; ++i) {
        unk_ec[i] = findIcon();
        if (unk_ec[i]) {
            unk_ec[i]->goto_frame(3);
        }
    }
    for (int i = 0; i < 12; ++i) {
        unk_16c[i] = findIcon();
        unk_16c[i]->goto_frame(4);
    }
    for (int i = 0; i < 12; ++i) {
        unk_13c[i] = findIcon();
        unk_13c[i]->goto_frame(0);
    }
    for (int i = 0; i < 8; ++i) {
        unk_fc[i] = findIcon();
        unk_fc[i]->goto_frame(1);
    }
    for (int i = 0; i < 8; ++i) {
        unk_11c[i] = findIcon();
        unk_11c[i]->goto_frame(2);
    }
}

// 0x41d384 _ZN10HUDMinimap17SetRoomVisibilityEPN6glitch5scene10ISceneNodeE
// PARTIAL: only the first step is rebuilt, the clear of the visited-room list (+0xe4), which the original runs when the
// node is set. Missing: (1) the walk over the room list at ObjectManager +0x24 (dword_99F764 + 36). Each room is pushed
// onto m_visitedRooms when RoomZone::HasBeenVisited() is true, or when the "isUsingMapHack" switch is on. The walk is not
// written: ObjectManager::unk_24 is declared std::list<std::int32_t>, but the original reads each node's data word (+0x08)
// as a RoomZone* (layout request). (2) The loop over node->getChildren() that sets each child's visibility to
// IsInsideVisitedRooms(child->getAbsolutePosition()) through setVisible (ISceneNode vtable K 0x48). The glitch ISceneNode
// declares all of these, but the loop is left out because it reads the list that (1) builds. With (1) missing it would
// hide every child.
void HUDMinimap::SetRoomVisibility(glitch::scene::ISceneNode* node) {
    if (!node) {
        return;
    }
    m_visitedRooms.clear();  // +0xe4 list: the original frees every node and resets the sentinel
    // PARTIAL: the room walk (1) and the child visibility loop (2) are missing, see above.
}
