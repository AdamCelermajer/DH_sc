#pragma once

// Structs::X record classes of slice structs_5 (23 classes; ItemPowerStrength and ItemPowerAllStats are
// skipped, see the ledger). Hand-written from the pseudocode with the conventions of structs_records.hpp:
// m_wN is word N (offset 4*N), m_offN an unaligned offset, and the original 32-bit offset is in each comment.
// Freed pointers are void*; string buffers are char*. Element tables hold polymorphic records (NOTES.md,
// "Arrays element ABI"). Each read is void: the original's return value is not modelled.
#include <cstdint>

#include "pydata-accessors/deps/stream_reader.hpp"

namespace Structs {

class ItemPowerEntryList {
public:
    virtual ~ItemPowerEntryList();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;  // +0x4, element count
    void* m_w2 = nullptr;    // +0x8, members (stride 12, ItemPowerEntry)
};
void DeletingDestructor(ItemPowerEntryList* self);

class TriggerTrap {
public:
    virtual ~TriggerTrap();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::int32_t m_w1 = 0;  // +0x4
    std::int32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc, string length
    char* m_w4 = nullptr;  // +0x10, string buffer
    std::int32_t m_w5 = 0;  // +0x14
    std::int32_t m_w6 = 0;  // +0x18
};
void DeletingDestructor(TriggerTrap* self);

class TriggerObject {
public:
    virtual ~TriggerObject();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::int32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8, string length
    char* m_w3 = nullptr;  // +0xc, string buffer
    std::int32_t m_w4 = 0;  // +0x10
    std::int32_t m_w5 = 0;  // +0x14
};
void DeletingDestructor(TriggerObject* self);

class TimerTrap {
public:
    virtual ~TimerTrap();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::int32_t m_w1 = 0;  // +0x4
    std::int32_t m_w2 = 0;  // +0x8
    std::int32_t m_w3 = 0;  // +0xc
    std::uint32_t m_w4 = 0;  // +0x10, string length
    char* m_w5 = nullptr;  // +0x14, string buffer
    std::int32_t m_w6 = 0;  // +0x18
    std::int32_t m_w7 = 0;  // +0x1c
};
void DeletingDestructor(TimerTrap* self);

class ProjectileTrap {
public:
    virtual ~ProjectileTrap();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::int32_t m_w1 = 0;  // +0x4
    std::int32_t m_w2 = 0;  // +0x8
    std::int32_t m_w3 = 0;  // +0xc
    bool m_w4 = false;  // +0x10
    std::int32_t m_w5 = 0;  // +0x14
    std::uint32_t m_w6 = 0;  // +0x18, string length
    char* m_w7 = nullptr;  // +0x1c, string buffer
    std::int32_t m_w8 = 0;  // +0x20
    std::int32_t m_w9 = 0;  // +0x24
};
void DeletingDestructor(ProjectileTrap* self);

class OpenableContainer {
public:
    virtual ~OpenableContainer();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::int32_t m_w1 = 0;  // +0x4
    std::int32_t m_w2 = 0;  // +0x8
    bool m_w3 = false;  // +0xc
    std::int32_t m_w4 = 0;  // +0x10
    std::uint32_t m_w5 = 0;  // +0x14, string length
    char* m_w6 = nullptr;  // +0x18, string buffer
    std::int32_t m_w7 = 0;  // +0x1c
    std::int32_t m_w8 = 0;  // +0x20
    std::int32_t m_w9 = 0;  // +0x24
};
void DeletingDestructor(OpenableContainer* self);

class ExplosiveTrap {
public:
    virtual ~ExplosiveTrap();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::int32_t m_w1 = 0;  // +0x4
    std::int32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc, string length
    char* m_w4 = nullptr;  // +0x10, string buffer
    std::int32_t m_w5 = 0;  // +0x14
};
void DeletingDestructor(ExplosiveTrap* self);

class Door {
public:
    virtual ~Door();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;  // +0x4, string length
    char* m_w2 = nullptr;  // +0x8, string buffer
    std::int32_t m_w3 = 0;  // +0xc
    std::int32_t m_w4 = 0;  // +0x10
    std::int32_t m_w5 = 0;  // +0x14
};
void DeletingDestructor(Door* self);

class DestructibleContainer {
public:
    virtual ~DestructibleContainer();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::int32_t m_w1 = 0;  // +0x4
    std::int32_t m_w2 = 0;  // +0x8
    std::int32_t m_w3 = 0;  // +0xc
    std::int32_t m_w4 = 0;  // +0x10
    std::int32_t m_w5 = 0;  // +0x14
    std::int32_t m_w6 = 0;  // +0x18
    std::int32_t m_w7 = 0;  // +0x1c
    bool m_w8 = false;  // +0x20
    std::int32_t m_w9 = 0;  // +0x24
    std::int32_t m_w10 = 0;  // +0x28
    std::uint32_t m_w11 = 0;  // +0x2c, string length
    char* m_w12 = nullptr;  // +0x30, string buffer
    std::int32_t m_w13 = 0;  // +0x34
    std::int32_t m_w14 = 0;  // +0x38
    std::int32_t m_w15 = 0;  // +0x3c
    std::int32_t m_w16 = 0;  // +0x40
};
void DeletingDestructor(DestructibleContainer* self);

class FaerySpellList {
public:
    virtual ~FaerySpellList();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;  // +0x4, count of int32 words
    void* m_w2 = nullptr;    // +0x8, int32 array
};
void DeletingDestructor(FaerySpellList* self);

class FootstepEffect {
public:
    virtual ~FootstepEffect();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::int32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8, string length
    char* m_w3 = nullptr;  // +0xc, string buffer
    std::uint32_t m_w4 = 0;  // +0x10, count of m_w5
    void* m_w5 = nullptr;  // +0x14, int32 array
    std::uint32_t m_w6 = 0;  // +0x18, count of m_w7
    void* m_w7 = nullptr;  // +0x1c, int32 array
};
void DeletingDestructor(FootstepEffect* self);

class AnimFX {
public:
    virtual ~AnimFX();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::int32_t m_w1 = 0;  // +0x4
    bool m_w2 = false;  // +0x8
    std::int32_t m_w3 = 0;  // +0xc
    bool m_w4 = false;  // +0x10
    bool m_off17 = false;  // +0x11
    std::int32_t m_w5 = 0;  // +0x14
    std::int32_t m_w6 = 0;  // +0x18
    std::int32_t m_w7 = 0;  // +0x1c
    bool m_w8 = false;  // +0x20
    bool m_off33 = false;  // +0x21
    float m_w9 = 0.0f;  // +0x24
    std::uint32_t m_w10 = 0;  // +0x28, string length
    char* m_w11 = nullptr;  // +0x2c, string buffer
};
void DeletingDestructor(AnimFX* self);

class AnimFXTpl {
public:
    virtual ~AnimFXTpl();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    bool m_w1 = false;  // +0x4
    std::int32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc, element count
    void* m_w4 = nullptr;  // +0x10, members (stride 48, AnimFX)
    std::int32_t m_w5 = 0;  // +0x14
};
void DeletingDestructor(AnimFXTpl* self);

class DialogStepList {
public:
    virtual ~DialogStepList();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;  // +0x4, element count
    void* m_w2 = nullptr;    // +0x8, members (stride 20, DialogStep)
};
void DeletingDestructor(DialogStepList* self);

class ConditionalDialog {
public:
    virtual ~ConditionalDialog();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;  // +0x4, element count
    void* m_w2 = nullptr;    // +0x8, members (stride 16, DialogCondition)
};
void DeletingDestructor(ConditionalDialog* self);

class DialogCondition {
public:
    virtual ~DialogCondition();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::int32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8, string length
    char* m_w3 = nullptr;  // +0xc, string buffer
};
void DeletingDestructor(DialogCondition* self);

class DialogActor {
public:
    virtual ~DialogActor();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::int32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8, string length
    char* m_w3 = nullptr;  // +0xc, string buffer
    std::int32_t m_w4 = 0;  // +0x10
};
void DeletingDestructor(DialogActor* self);

class ColladaFile {
public:
    virtual ~ColladaFile();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;  // +0x4, string length
    char* m_w2 = nullptr;    // +0x8, string buffer
};
void DeletingDestructor(ColladaFile* self);

class CharTemplate {
public:
    virtual ~CharTemplate();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;  // +0x4, element count
    void* m_w2 = nullptr;    // +0x8, members (stride 8)
};
void DeletingDestructor(CharTemplate* self);

class StatListList {
public:
    virtual ~StatListList();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;  // +0x4, count of int32 words
    void* m_w2 = nullptr;    // +0x8, int32 array
};
void DeletingDestructor(StatListList* self);

class StatAutoAssignTargetList {
public:
    virtual ~StatAutoAssignTargetList();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;  // +0x4, element count
    void* m_w2 = nullptr;    // +0x8, members (stride 16, StatAutoAssignTarget)
};
void DeletingDestructor(StatAutoAssignTargetList* self);

class ClassFuncList {
public:
    virtual ~ClassFuncList();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;  // +0x4, element count
    void* m_w2 = nullptr;    // +0x8, members (stride 24)
};
void DeletingDestructor(ClassFuncList* self);

class AnimTpl {
public:
    virtual ~AnimTpl();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::int32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8, element count
    void* m_w3 = nullptr;    // +0xc, members (AnimStep, stride 56)
    std::int32_t m_w4 = 0;  // +0x10
};
void DeletingDestructor(AnimTpl* self);

class AIFactions {
public:
    virtual ~AIFactions();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;  // +0x4, element count
    void* m_w2 = nullptr;    // +0x8, members (stride 12, AIFaction)
};
void DeletingDestructor(AIFactions* self);

}  // namespace Structs
