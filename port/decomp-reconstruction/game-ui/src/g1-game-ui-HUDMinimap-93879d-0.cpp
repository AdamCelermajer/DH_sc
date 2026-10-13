// HUDMinimap bodies, batch g1-game-ui-HUDMinimap-93879d-0 (20 functions, the first half of the class).
// The minimap menu ("menu_miniMap"): icon and object bookkeeping, the map camera and the visited rooms.
// Rebuilt from the Hex-Rays pseudocode of libDungeonHunter2.so. Offsets in comments are the original 32-bit ARM
// offsets; the code never uses them for addressing.
//
// PARTIAL rows: HideAllIcons, Hide, SetObjectsVisibility, UpdateIconPositions and CreateMapCamera. Each one needs a
// layout or declaration that this tree does not have yet. The layout requests are:
//   gameswf::character +0x9b uint8                   visible flag of an icon movie. Set or cleared by
//                                                   HideAllIcons, SetObjectsVisibility, UpdateIconPositions.
//                                                   The same request as game-ui FlashAnimManager.
//   glitch::IDevice +0x1c glitch::scene::CSceneManager*
//                                                   the scene manager of Singleton<Application>::s_inst.m_device.
//                                                   Read by UpdateIconPositions (guard: +0xe4 activeCamera) and by
//                                                   Hide (+0x28c, call target not identified). Application::Draw3D
//                                                   reads it too (chunk_002).
//   RenderFX::SetPosition(gameswf::character*, int, int)  0x7aa3f0 is the symbol. Not declared in dh2/RenderFX.hpp.
//   Arrays::CamAnimSetTable                         declared twice in pydata-accessors (member_names.hpp has
//                                                   m_memberNames, w7_arrays_02.hpp has size and members as
//                                                   uint8_t*). Both cannot be included together. The clip id of a
//                                                   row is the CamAnimSet +0x10 word (m_w4). Needs a typed accessor.

#include <cstdio>
#include <cstdlib>

#include "dh2/Application.hpp"
#include "dh2/CameraBase.hpp"
#include "dh2/CameraLevel.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/HUDMinimap.hpp"
#include "dh2/Level.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/Point2D.hpp"
#include "dh2/Point3D.hpp"
#include "dh2/RoomZone.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/global/Vec3f_Origin.hpp"
#include "gameswf/gameswf.h"                          // gameswf::rect (complete type; MenuBase.hpp forward-declares it)
#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"

// Global assert level (libDungeonHunter2.so gAssertLevel). core-platform/memory.hpp declares it too, but that header
// pulls the core-platform glitch_core.hpp, which clashes with the glitch-core-io headers that ISceneNode needs.
extern int gAssertLevel;

// 0x41b404 _ZN10HUDMinimap7InitAllEv
void HUDMinimap::InitAll() {
    RegisterToMenuManager();  // vtable +0x7c
    InitAllIcons();           // vtable +0x70
    InitAllObjects();         // vtable +0x5c
}

// 0x41b43c _ZN10HUDMinimap4HideEv
// PARTIAL: the tail is not made. After the icons are hidden and the flag is cleared, the original reads
// (device +0x1c) +0x28c and, when that is non-null, calls its vtable slot +0x48 with it. The device +0x1c member is
// not declared (layout request above), and the object at +0x28c is not identified, so the call is omitted.
void HUDMinimap::Hide() {
    DisableMapCamera();  // vtable +0x90
    HideAllIcons();      // vtable +0x6c
    unk_c4 = false;      // +0xc4, Show sets it
}

// 0x41b4a8 _ZN10HUDMinimap16DestroyMapCameraEv
void HUDMinimap::DestroyMapCamera() {
    if (m_mapCam != nullptr) {
        delete m_mapCam;  // vtable +0x04, the deleting destructor
        m_mapCam = nullptr;
    }
}

// 0x41b4d8 _ZN10HUDMinimap16DisableMapCameraEv
// The original tail-calls EnableGameCamera through vtable +0x94.
CameraBase* HUDMinimap::DisableMapCamera() {
    return EnableGameCamera();
}

// 0x41b4ec _ZN10HUDMinimap16ResetTranslationEv
// Clears the map camera's pan offset (+0x98), updates the camera, then updates the absolute position of its scene node
// (vtable +0xb8 with includeChildren = true).
void HUDMinimap::ResetTranslation() {
    CameraLevel* camera = m_mapCam;
    camera->m_panOffset = Vec3f_Origin;
    camera->Update();                              // vtable +0x10
    camera->m_node->updateAbsolutePosition(true);  // CameraBase::m_node (+0x04), slot K 0xb8
}

// 0x41b55c _ZN10HUDMinimap10SetEnemiesEv
void HUDMinimap::SetEnemies() {}

// 0x41b560 _ZN10HUDMinimap10SetObjectsEv
void HUDMinimap::SetObjects() {}

// 0x41b564 _ZN10HUDMinimap13SetObjectivesEv
void HUDMinimap::SetObjectives() {}

// 0x41b568 _ZN10HUDMinimap12InitAllIconsEv
// Clears every icon pointer: unk_ec (4), unk_fc (8), unk_11c (8), unk_13c (12) and unk_16c (12). The original returns
// the advanced pointer, which the header does not declare.
void HUDMinimap::InitAllIcons() {
    for (gameswf::character*& icon : unk_ec) {
        icon = nullptr;
    }
    for (gameswf::character*& icon : unk_fc) {
        icon = nullptr;
    }
    for (gameswf::character*& icon : unk_11c) {
        icon = nullptr;
    }
    for (gameswf::character*& icon : unk_13c) {
        icon = nullptr;
    }
    for (gameswf::character*& icon : unk_16c) {
        icon = nullptr;
    }
}

// 0x41b5ec _ZN10HUDMinimap14InitAllObjectsEv
// Clears unk_3dc (8) and unk_3fc (8).
void HUDMinimap::InitAllObjects() {
    for (GameObject*& object : unk_3dc) {
        object = nullptr;
    }
    for (GameObject*& object : unk_3fc) {
        object = nullptr;
    }
}

// 0x41b628 _ZN10HUDMinimap12HideAllIconsEv
// PARTIAL: for every non-null icon the original clears its visible flag (+0x9b, layout request above). Order: unk_ec
// (4), unk_fc (8), unk_11c (8), unk_13c (12), unk_16c (12). The flag is not declared, so the stores are not made and
// the icon arrays are only visited.
void HUDMinimap::HideAllIcons() {
    for (gameswf::character* icon : unk_ec) {
        static_cast<void>(icon);  // PARTIAL: icon +0x9b = 0 not made
    }
    for (gameswf::character* icon : unk_fc) {
        static_cast<void>(icon);  // PARTIAL
    }
    for (gameswf::character* icon : unk_11c) {
        static_cast<void>(icon);  // PARTIAL
    }
    for (gameswf::character* icon : unk_13c) {
        static_cast<void>(icon);  // PARTIAL
    }
    for (gameswf::character* icon : unk_16c) {
        static_cast<void>(icon);  // PARTIAL
    }
}

// 0x41b790 _ZN10HUDMinimap17GetMapScreenCoordERK7Point3DIfEffff
// The projected point (screen X and Y from CameraBase::GetScreenCoord) is scaled by the scale arguments, halved and
// offset. Y is negated before the scale is applied. The original returns the Point2D through its hidden pointer.
Point2D<float> HUDMinimap::GetMapScreenCoord(const Point3D<float>& position, float scaleX, float scaleY, float offsetX,
                                             float offsetY) {
    Point2D<float> projected{0.0f, 0.0f};
    CameraBase::GetScreenCoord(position, projected);
    Point2D<float> screen{};
    screen.X = (scaleX * projected.X) * 0.5f + offsetX;
    screen.Y = (-projected.Y * scaleY) * 0.5f + offsetY;
    return screen;
}

// 0x41b868 _ZN10HUDMinimap20IsInsideVisitedRoomsERK7Point3DIfE
// True when some visited room contains the position (RoomZone::HasInside, X and Y).
bool HUDMinimap::IsInsideVisitedRooms(const Point3D<float>& position) {
    for (RoomZone* room : m_visitedRooms) {
        if (room->HasInside(position)) {
            return true;
        }
    }
    return false;
}

// 0x41b8b4 _ZN10HUDMinimap20SetObjectsVisibilityEP5Level
// PARTIAL: the visible flags of the icons are not stored (layout request above). The test for each icon is kept.
// Over unk_d4 entries of unk_3dc: an object is visible on the map when it is non-null, the level is non-null, its
// entry point id equals the level's, and it lies in a visited room. Over unk_d0 entries of unk_3fc: visible when it
// is non-null and lies in a visited room. The original short-circuits in the same order.
void HUDMinimap::SetObjectsVisibility(Level* level) {
    for (int i = 0; i < unk_d4; ++i) {
        GameObject* object = unk_3dc[i];
        const bool visible = object != nullptr && level != nullptr && object->m_entryPointId == level->m_entryPointId &&
                             IsInsideVisitedRooms(object->m_position);
        static_cast<void>(visible);  // PARTIAL: unk_fc[i] +0x9b = visible not stored
    }
    for (int i = 0; i < unk_d0; ++i) {
        GameObject* object = unk_3fc[i];
        const bool visible = object != nullptr && IsInsideVisitedRooms(object->m_position);
        static_cast<void>(visible);  // PARTIAL: unk_11c[i] +0x9b = visible not stored
    }
}

// 0x41b9bc _ZN10HUDMinimap19UpdateIconPositionsEPS_N7gameswf4rectE
// PARTIAL: the whole body is not rebuilt. The original runs only when (device +0x1c) +0xe4 (the scene manager's active
// camera) is non-null, and that device member is not declared (layout request above). Then it places the five icon
// groups. For each group it calls GetMapScreenCoord with the rect's X range, Y range and their centres (the rect is
// {m_x_min, m_x_max, m_y_min, m_y_max}), and passes the result to RenderFX::SetPosition (not declared). Icons from
// unk_ec, unk_16c, unk_13c get their flag set (+0x9b). Group unk_ec first copies the position of each player's
// character (+0x660 of PlayerInfo, +0x160 of GameObject) into unk_1cc. Group unk_16c calls the virtual SetEnemies()
// before each icon, and the positions are unk_34c. Nothing is written here.
void HUDMinimap::UpdateIconPositions(HUDMinimap* menu, gameswf::rect rect) {
    static_cast<void>(menu);
    static_cast<void>(rect);
}

// 0x41be34 _ZN10HUDMinimap16EnableGameCameraEv
// Activates the current level's camera (Level +0x128) when there is a level and a camera. Returns that camera, or null.
CameraBase* HUDMinimap::EnableGameCamera() {
    Level* level = Singleton<Application>::s_inst.GetCurrentLevel();
    CameraBase* camera = nullptr;
    if (level != nullptr) {
        camera = level->m_camera;
        if (camera != nullptr) {
            camera->SetActive();
        }
    }
    return camera;
}

// 0x41be78 _ZN10HUDMinimap15EnableMapCameraEv
void HUDMinimap::EnableMapCamera() {
    if (m_mapCam != nullptr) {
        m_mapCam->SetActive();
    }
}

// 0x41be88 _ZN10HUDMinimap15CreateMapCameraEv
// Creates the map camera once. If it exists, nothing else happens.
// PARTIAL: the original then calls CameraLevel::Load(m_mapCam, "data/3D/camera/CameraTests.bdae", index,
// "PlayerCamera_Default") with index = the row of "MiniMap" in Arrays::CamAnimSetTable (-1 when absent), and at the
// end CameraLevel::PlayAnim(clip, 0, false) with clip = the row's clip id. Both are not made: the table has two
// declarations in this tree (see the file comment). The setup calls that remain run in the original order.
void HUDMinimap::CreateMapCamera() {
    if (m_mapCam != nullptr) {
        return;
    }
    m_mapCam = new CameraLevel();  // operator new(0xa8) and the CameraLevel constructor
    if (m_mapCam == nullptr) {
        // ASSERT(m_mapCam) failed. Same policy as the other gAssertLevel asserts in the tree.
        if (gAssertLevel == 2) {
            std::abort();  // the original writes address 0 here to force a crash
        } else if (gAssertLevel == 1) {
            std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "m_mapCam",
                         "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Menus\\HUDMinimap.cpp", 272);
        }
    }
    // PARTIAL: CameraLevel::Load(...) is not made (see above).
    m_mapCam->m_useObjectPosition = true;  // +0x85
    m_mapCam->EnableDamping(false);        // CameraTarget::EnableDamping(m_mapCam, 0)
    m_mapCam->SetData(0.42963f, 1.0f, 0.0f, 100000.0f, false);
    m_mapCam->m_zoomTarget = 1.0f;   // +0x8c
    m_mapCam->m_zoomCurrent = 0.0f;  // +0x88
    // PARTIAL: CameraLevel::PlayAnim(clip, 0, false) is not made (see above).
}

// 0x41c058 _ZN10HUDMinimap23RegisterDisplayCallbackEv
// Registers RenderMap as the display callback "RenderMap" of the owning MenuFX (+0x04), with this as the user pointer.
void HUDMinimap::RegisterDisplayCallback() {
    m_menu->RegisterDisplayCallback("RenderMap", &HUDMinimap::RenderMap, this);
}

// 0x41c088 _ZN10HUDMinimap21RegisterToMenuManagerEv
void HUDMinimap::RegisterToMenuManager() {
    MenuManager::GetInstance()->RegisterMenu(this);
}
