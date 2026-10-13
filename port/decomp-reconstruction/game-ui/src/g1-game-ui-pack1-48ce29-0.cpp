// Batch g1-game-ui-pack1-48ce29-0: AnchorForward (constructors, destructors, Reset, Update) and Dragable (constructors,
// destructors, SendEvent, ResetPosition, OnEvent).
//
// Rebuilt from the Hex-Rays pseudocode of libDungeonHunter2.so and checked against the assembly where the pseudocode
// drops arguments. Offsets in comments are the original 32-bit ARM offsets (AnchorForward is 0x64 bytes there, Dragable
// 0x54), so the host layouts in the headers are not the same bytes.
//
// Facts used here:
//   - AnchorForward C1 (0x477a80) and C2 (0x477cd4) have one body. Hex-Rays shows C1 calling AnchorBase with no arguments,
//     but the assembly passes the GameObject and the AnchorType (read from the stack) to AnchorBase(GameObject*, AnchorType),
//     as C2 does. The threshold is the fifth argument, stored at +0x24 as a float.
//   - Dragable C1 (0x4124c4) and C2 (0x413240) have one body. C1 only inlines the destruction of the std::string.
//   - gameswf::rect::twips_to_pixels (0x7959a8) divides each of the four floats by 20.0.
//   - GameObject fields used by the anchors: m_position (+0x160), m_hasHeading (+0x1b5), m_heading (+0x1b8), and
//     GameObject::GetLookAtVec fills the look direction. The Character state machine is its CharStateMachine base (+0x4fc).
//   - ASSERT messages keep the original source path. gAssertLevel 2 stops the program, 1 prints and continues, as in the
//     other batches of this tree.
#include <array>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "dh2/AnchorForward.hpp"
#include "dh2/Application.hpp"
#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/DragAndDrop.hpp"
#include "dh2/Dragable.hpp"
#include "dh2/Dropable.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/GameSWFUtils.hpp"
#include "dh2/MenuFX.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/global/MenuManagerGlobals.hpp"

// Global assert level (libDungeonHunter2.so gAssertLevel). core-platform/memory.hpp declares it too, but it is not included
// here, as in the other batches.
extern int gAssertLevel;

// Screen height, the second of the two screen-size globals (Width_Screen is declared in dh2/global/MenuManagerGlobals.hpp).
// The owner is the screen-size setup of the menu code.
extern int Height_Screen;

namespace {

constexpr const char* kAnchorForwardSource =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\ObjectsSub\\Anchors\\AnchorForward.cpp";

// The original ASSERT in AnchorForward.cpp. gAssertLevel 2 stops the program (the original stores to address 0 to crash).
void AnchorForwardAssert(const char* expression, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expression, kAnchorForwardSource, line);
    }
}

// gameswf::rect::twips_to_pixels (0x7959a8): each of the four floats is divided by 20.0, since 20 twips make one pixel.
void TwipsToPixels(float (&rect)[4]) {
    for (float& value : rect) {
        value = value / 20.0f;
    }
}

// The common tail of the Update paths (LABEL_35 of the original): the object's position is recorded, and the anchor's
// look direction is refreshed from the object.
void RecordPositionAndLook(AnchorForward& anchor, GameObject* go) {
    anchor.unk_2c = go->m_position;
    go->GetLookAtVec(anchor.unk_58);
}

void CopyPosition(AnchorForward& anchor, const Point3D<float>& p) {
    anchor.m_position.X = p.X;
    anchor.m_position.Y = p.Y;
    anchor.m_position.Z = p.Z;
}

// LABEL_31 of AnchorForward::Update: the anchor is not following a target. The speed is capped at 0.4 * m_MAX_DISTANCE,
// m_position moves from the object's position along the look direction, and the look direction is refreshed.
void UpdateIdle(AnchorForward& anchor, GameObject* go) {
    const float cap = anchor.m_MAX_DISTANCE * 0.4f;
    if (anchor.unk_54 > cap) {
        anchor.unk_54 = cap;
    }
    const float speed = anchor.unk_54;
    anchor.m_position.X = speed * anchor.unk_58.X + go->m_position.X;
    anchor.m_position.Y = speed * anchor.unk_58.Y + go->m_position.Y;
    anchor.m_position.Z = speed * anchor.unk_58.Z + go->m_position.Z;
    RecordPositionAndLook(anchor, go);
}

// The branch of AnchorForward::Update, with unk_38 clear, for an object with a heading whose target moves or attacks.
// The anchor steps toward the position ahead of the object along its heading, at a speed that depends on the turn to the
// heading and on the time of the step.
void UpdateMoving(AnchorForward& anchor, GameObject* go) {
    Point3D<float> heading = go->m_heading;  // v48..v50: a copy of the object's heading, normalized below
    const float maxDistance = anchor.m_MAX_DISTANCE;
    // v9: the angle between the look direction and the heading, taken before the heading is normalized.
    const float turn = anchor.unk_58.angle(heading);

    // The squared distance the object moved since the last record (unk_2c, the record of LABEL_35).
    const float dx = anchor.unk_2c.X - go->m_position.X;
    const float dy = anchor.unk_2c.Y - go->m_position.Y;
    const float dz = anchor.unk_2c.Z - go->m_position.Z;
    // v14: the full speed while the target moves and the object has moved more than 0.05 (squared), half of it otherwise.
    float speedMax = maxDistance * 0.5f;
    if (anchor.unk_28->SM_IsMoving(false) && (dx * dx + dy * dy + dz * dz) > 0.05f) {
        speedMax = maxDistance;
    }

    heading.normalize();  // Point3D<float>::normalize(&v48), which returns the same object (v15 == &v48).
    // v16..v18: the point speedMax ahead of the object along the normalized heading.
    const float targetX = speedMax * heading.X + go->m_position.X;
    const float targetY = speedMax * heading.Y + go->m_position.Y;
    const float targetZ = speedMax * heading.Z + go->m_position.Z;

    // Speed update. v20 is m_DIST_PERSEC.
    const float distPerSec = anchor.m_DIST_PERSEC;
    if (turn > 1.0f) {
        float speed = anchor.unk_54 - distPerSec * 0.25f;
        if (speed <= 0.0f) {
            speed = 0.0f;
        }
        anchor.unk_54 = speed;
    } else {
        // v43: the half of m_MAX_DISTANCE truncated to a whole number of units, which is the floor of the speed.
        const float floorSpeed = static_cast<float>(static_cast<int>(maxDistance * 0.5f));
        if (anchor.unk_54 < floorSpeed) {
            anchor.unk_54 = floorSpeed;
        } else {
            float speed = distPerSec + anchor.unk_54;
            if (speedMax < speed) {
                speed = speedMax;
            }
            anchor.unk_54 = speed;
        }
    }

    // v16..v18 against m_position: snap to the target when they coincide or the speed is at its cap, else step by the speed.
    const float ex = targetX - anchor.m_position.X;
    const float ey = targetY - anchor.m_position.Y;
    const float ez = targetZ - anchor.m_position.Z;
    const bool coincides = (ex * ex + ey * ey + ez * ez) <= 0.0f;
    if (coincides || (speedMax - anchor.unk_54) <= 0.05f) {
        anchor.m_position.X = targetX;
        anchor.m_position.Y = targetY;
        anchor.m_position.Z = targetZ;
    } else {
        const float speed = anchor.unk_54;  // v22
        anchor.m_position.X = speed * heading.X + go->m_position.X;
        anchor.m_position.Y = speed * heading.Y + go->m_position.Y;
        anchor.m_position.Z = speed * heading.Z + go->m_position.Z;
    }
    RecordPositionAndLook(anchor, go);
}

// The branch of AnchorForward::Update with unk_38 set. The anchor sits on the object while the object is not following a
// target, and otherwise moves ahead of the object along its raw heading for a 350 ms countdown, with the state (unk_3c)
// describing which of those applies.
void UpdateFollowing(AnchorForward& anchor, GameObject* go) {
    // v26: the full speed while the target moves, half of it otherwise. The original dereferences the target here without
    // testing it for null, so a null target crashes it; this body does the same.
    const bool targetMoving = anchor.unk_28->SM_IsMoving(false);
    const float v26 = targetMoving ? anchor.m_MAX_DISTANCE : anchor.m_MAX_DISTANCE * 0.5f;
    const float hx = go->m_heading.X;  // v27..v29: the raw heading, not normalized in this branch
    const float hy = go->m_heading.Y;
    const float hz = go->m_heading.Z;

    const bool notFollowing = !go->m_hasHeading || anchor.unk_28 == nullptr ||
                              (!targetMoving && !anchor.unk_28->SM_IsAttacking());
    if (notFollowing) {
        if (anchor.unk_4c) {
            // The countdown of the previous step runs down by the frame time. At zero or less it ends, with state 4.
            const int remaining = anchor.unk_50 - ApplicationGetInstance().GetDt();
            if (remaining <= 0) {
                anchor.unk_4c = false;
                anchor.unk_3c = 4;
            }
            anchor.unk_50 = remaining;
        } else {
            anchor.unk_3c = 1;
        }
        anchor.unk_40 = go->m_position;
        CopyPosition(anchor, go->m_position);
        return;
    }

    if (anchor.unk_4c) {
        anchor.unk_3c = 2;
    } else {
        // Distance from the position where the countdown started (unk_40). Within 250 units (250 squared is 62500) the
        // object has not moved far enough: state 0, and the anchor sits on the object.
        const float dx = go->m_position.X - anchor.unk_40.X;
        const float dy = go->m_position.Y - anchor.unk_40.Y;
        const float dz = go->m_position.Z - anchor.unk_40.Z;
        if ((dx * dx + dy * dy + dz * dz) < 62500.0f) {
            anchor.unk_3c = 0;
            CopyPosition(anchor, go->m_position);
            return;
        }
        anchor.unk_3c = 3;
    }

    anchor.m_position.X = v26 * hx + go->m_position.X;
    anchor.m_position.Y = v26 * hy + go->m_position.Y;
    anchor.m_position.Z = v26 * hz + go->m_position.Z;
    anchor.unk_50 = 350;
    anchor.unk_4c = true;
}

}  // namespace

// 0x477a80 AnchorForward::AnchorForward(GameObject*, float, float, float, AnchorBase::AnchorType)  (C1)
// 0x477cd4 AnchorForward::AnchorForward(GameObject*, float, float, float, AnchorBase::AnchorType)  (C2, same body)
// The base constructor takes the object and the type. The three floats are the max distance (+0x1c), the distance per second
// (+0x20) and the threshold (+0x24). The rest of the members start at zero, the state (+0x3c) at 5. The ASSERTs check the
// three floats in the original order. Finally, the object's handle is resolved to a Character (+0x28) and Reset runs.
AnchorForward::AnchorForward(GameObject* go, float maxDistance, float distPerSec, float threshold,
                             AnchorBase::AnchorType type)
    : AnchorBase(go, type) {
    m_MAX_DISTANCE = maxDistance;                   // +0x1c
    m_DIST_PERSEC = distPerSec;                     // +0x20
    m_THRESHOLD = threshold;                        // +0x24
    unk_28 = nullptr;                               // +0x28
    unk_2c = Point3D<float>{0.0f, 0.0f, 0.0f};      // +0x2c
    unk_38 = false;                                 // +0x38
    unk_3c = 5;                                     // +0x3c
    unk_40 = Point3D<float>{0.0f, 0.0f, 0.0f};      // +0x40
    unk_4c = false;                                 // +0x4c
    unk_50 = 0;                                     // +0x50
    unk_54 = 0.0f;                                  // +0x54
    unk_58 = Point3D<float>{0.0f, 0.0f, 0.0f};      // +0x58

    if (m_MAX_DISTANCE < 0.0f) {
        AnchorForwardAssert("m_MAX_DISTANCE >= 0.0f", 48);
    }
    if (m_DIST_PERSEC < 0.0f) {
        AnchorForwardAssert("m_DIST_PERSEC >= 0.0f", 49);
    }
    if (m_THRESHOLD < 0.0f || m_THRESHOLD > 1.0f) {
        AnchorForwardAssert("0.0f <= m_THRESHOLD && m_THRESHOLD <= 1.0f", 50);
    }
    if (go != nullptr) {
        ObjectHandle handle = go->GetHandle();
        unk_28 = handle.operator Character*();
    }
    AnchorForward::Reset();  // a direct call in the original, not the virtual one
}

// 0x4779fc AnchorForward::~AnchorForward()  (D1)
// 0x477a4c AnchorForward::~AnchorForward()  (D2, same body)
// The original stores the AnchorForward vtable and runs the AnchorBase destructor. The C++ destructor does the same.
AnchorForward::~AnchorForward() {}

// 0x477a30 AnchorForward::~AnchorForward()  (D0, deleting destructor)
// Skipped: the compiler generates the deleting destructor on the host (the D1 destructor, then operator delete).

// 0x4779dc AnchorForward::Reset()
// The base Reset (AnchorBase::Reset, a direct call) sets the position from the object, then unk_2c (+0x2c..+0x34) is zeroed.
void AnchorForward::Reset() {
    AnchorBase::Reset();
    unk_2c = Point3D<float>{0.0f, 0.0f, 0.0f};
}

// 0x47734c AnchorForward::Update()
// With m_enabled clear nothing happens. With unk_38 clear the state is 5 and the anchor either follows the idle path, or
// (when the heading is known and the target moves or attacks) the moving path. With unk_38 set the countdown path runs.
void AnchorForward::Update() {
    if (!m_enabled) {
        return;
    }
    GameObject* go = m_go;
    if (!unk_38) {
        unk_3c = 5;
        if (!go->m_hasHeading || unk_28 == nullptr) {
            UpdateIdle(*this, go);
            return;
        }
        if (!unk_28->SM_IsMoving(false) && !unk_28->SM_IsAttacking()) {
            UpdateIdle(*this, go);
            return;
        }
        UpdateMoving(*this, go);
        return;
    }
    UpdateFollowing(*this, go);
}

// 0x4124c4 Dragable::Dragable(MenuFX*, gameswf::character*, gameswf::character*)  (C1)
// 0x413240 Dragable::Dragable(MenuFX*, gameswf::character*, gameswf::character*)  (C2, same body)
// PARTIAL. Reconstructed: the menu and character pointers, the identity matrix, the twips-to-pixels conversion of m_bounds, the
// dead zone (the zone character's absolute bounds, or the screen times 20 when there is no zone), unk_1c, and the
// isTracingDragable debug switch read.
// Missing (the gameswf character layout is not declared in this tree; see the layout requests):
//   - m_bounds: the character's bounds query, its vtable slot +0x12c (slot 75), writes the rect into &m_bounds before the
//     conversion. Here m_bounds is zeroed instead, so the conversion divides zeros, not the rect the query writes.
//   - m_depth: the character's depth word (character +0x94, uint16). Here 0.
//   - unk_30, unk_34: the (int) truncations of the character's matrix tx and ty (matrix pointer at character +0x4c). Here 0.
//   - m_savedMatrix: the six floats of the character's matrix, copied over the identity. Here the identity stays.
Dragable::Dragable(MenuFX* menu, gameswf::character* character, gameswf::character* zone) {
    m_menu = menu;                                          // +0x04
    m_character = character;                                // +0x08
    m_savedMatrix[0] = 1.0f;                                // +0x3c a
    m_savedMatrix[1] = 0.0f;                                // +0x40 b
    m_savedMatrix[2] = 0.0f;                                // +0x44 tx
    m_savedMatrix[3] = 0.0f;                                // +0x48 c
    m_savedMatrix[4] = 1.0f;                                // +0x4c d
    m_savedMatrix[5] = 0.0f;                                // +0x50 ty

    // PARTIAL: the bounds query (see above). Until the query is declared, m_bounds is zeroed here, so the conversion
    // below is deterministic. The original converts the rect that the query wrote into m_bounds instead.
    for (float& value : m_bounds) {
        value = 0.0f;
    }
    TwipsToPixels(m_bounds);                                // +0x0c

    // PARTIAL: m_depth is character +0x94 (see above). Zero until the character layout is declared.
    m_depth = 0;                                            // +0x38

    if (zone != nullptr) {
        const std::array<float, 4> zoneBounds = GameSWFUtils::GetAbsoluteBoundingRect(zone);
        for (int i = 0; i < 4; ++i) {
            m_deadZone[i] = zoneBounds[i];                  // +0x20 .. +0x2c
        }
    } else {
        m_deadZone[0] = 0.0f;                               // +0x20
        m_deadZone[1] = static_cast<float>(Width_Screen) * 20.0f;   // +0x24
        m_deadZone[2] = 0.0f;                               // +0x28
        m_deadZone[3] = static_cast<float>(Height_Screen) * 20.0f;  // +0x2c
    }

    unk_1c = 0;                                             // +0x1c
    unk_30 = 0;                                             // +0x30 PARTIAL: (int) of the character matrix tx, see above
    unk_34 = 0;                                             // +0x34 PARTIAL: (int) of the character matrix ty, see above

    DebugSwitches::s_inst.load();
    DebugSwitches::s_inst.GetSwitch(std::string("isTracingDragable"));  // the result is discarded, as in the original
}

// 0x411e54 Dragable::~Dragable()  (D2)
// 0x411e58 Dragable::~Dragable()  (D1)
// Both are empty: the original destructor does nothing, and the member destructors are trivial.
Dragable::~Dragable() {}

// 0x412084 Dragable::SendEvent(int)
// Forwards the event to DragAndDrop::SendEvent with the menu and the character. The original passes the caller's r3 as a fourth
// argument that the callee never reads, so only the three real arguments are passed here.
int Dragable::SendEvent(int event) {
    return DragAndDrop::SendEvent(event, m_menu, m_character);
}

// 0x412984 Dragable::ResetPosition()
// PARTIAL. Reconstructed: the display callback is registered for m_character (the second RegisterDisplayCallback of the
// original, with no callback and no user data).
// Missing (gameswf character layout and calls not declared in this tree):
//   - gameswf::character::set_matrix of m_savedMatrix onto the character,
//   - the first RegisterDisplayCallback, for the character's weak target (character +0x3c proxy, check_proxy, and the
//     pointer at +0x40),
//   - the character's vtable slot +0x08 call with the argument 2. A non-zero result continues to the depth step,
//   - the depth step: when the character depth word (+0x94) differs from m_depth, the display list (the character's root,
//     +0x40 then +0xa8) moves the character to m_depth by gameswf::display_list::change_character_depth. The original
//     returns that call's result.
// This body returns 0 in place of the missing results. The callers (DragAndDrop::ResetPositions, Dragable::OnEvent)
// ignore the value.
int Dragable::ResetPosition() {
    m_menu->RegisterDisplayCallback(m_character, nullptr, nullptr);
    return 0;
}

// 0x412b00 Dragable::OnEvent(RenderFX::Event&)
// PARTIAL. Reconstructed: the event is for this Dragable only when its target is m_character (the original compares the
// first word of the event with the character pointer, and returns false otherwise). The isTracingDragable switch is read
// before anything else.
// Missing, so the event is not handled here (false, nothing moves, nothing is consumed):
//   - the target's "drag_locked" member: get_member on the target (vtable +0x20) with a gameswf tu_string, then
//     as_value::to_bool. When it is set the original returns false at once.
//   - the target's bounds (vtable +0x12c, slot 75, in twips) and the event's pointer position (RenderFX::Event +0x0c and
//     +0x10 floats),
//   - for event type 5, the drag itself: the position clamp to m_deadZone (for unk_1c 2, the half-size clamp with
//     Point2D<float>::normalize), RenderFX::SetPosition of the character at the position divided by 20, and the btn_skill
//     depth raise (the target's name at +0x44, display_list::get_highest_depth and change_character_depth, with the
//     FlushTextCallback and FlushCallback display callbacks, which are not declared in this tree),
//   - event type 7: ResetPosition, then MenuManager::consumeEvent, and the result 0,
//   - other types other than 6: MenuManager::consumeEvent, and the result 0,
//   - event type 6: the result 1, and event type 5 without the depth raise: the result 1.
bool Dragable::OnEvent(RenderFX::Event& e) {
    if (static_cast<const void*>(e.target) != static_cast<const void*>(m_character)) {
        return false;
    }
    DebugSwitches::s_inst.load();
    DebugSwitches::s_inst.GetSwitch(std::string("isTracingDragable"));  // the result is discarded, as in the original
    return false;
}
