// AnchorBase: base of the camera/object anchors (AnchorForward, AnchorGroup, ...).
// Original 32-bit size: 0x1c (28); AnchorForward members start at +0x1c.
#pragma once

#include <cstdint>

#include "glitch-shared/glitch_core.hpp"

class GameObject;

class AnchorBase {
public:
    // Enumerators are not recovered; AnchorForward passes it straight through.
    enum AnchorType : int32_t;

    AnchorBase(GameObject* go, AnchorType type);              // 0x4770d0 / 0x4771a0
    virtual ~AnchorBase();                                    // slot +0x00, +0x04 (0x47706c, 0x4770b4)
    virtual void Reset();                                     // +0x08  0x477074  m_position = go position
    virtual void Update();                                    // +0x0c  0x477094  same body as Reset

    // vptr                                                   // +0x00
    AnchorType m_type;                                        // +0x04
    GameObject* m_go;                                         // +0x08  asserted non-null in the ctor
    glitch::core::vector3d<float> m_position;                 // +0x0c  copied from GameObject +0x160..+0x168
    bool m_enabled;                                           // +0x18  set true in the ctor; role is unconfirmed
};
