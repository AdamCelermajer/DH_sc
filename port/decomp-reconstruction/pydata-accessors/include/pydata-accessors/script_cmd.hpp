#pragma once

// HAND-MAINTAINED SOURCE (Track B decision 7). This file was first produced by gen_script_cmd.py, which is
// retired (Ghidra-era) and must not be rerun; edit this file directly.
//
// ScriptCmd base and one class per GetNewScriptCmdDataInstance<T> target. Member m_wN is the word at offset 4*N. Words that the
// constructor zeroes keep std::uint32_t; words a read stores take the read type; pointers that are
// freed are void*, string buffers are char*.
#include <cstdint>

#include "pydata-accessors/deps/stream_reader.hpp"

namespace Structs {

class ScriptCmd {
public:
    virtual ~ScriptCmd();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::int32_t m_w1 = 0;  // +0x4
};

struct AIChangeScript : ScriptCmd {
    ~AIChangeScript() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
    std::uint32_t m_w4 = 0;  // +0x10
    char* m_w5 = nullptr;  // +0x14
};

struct AIDoSkill : ScriptCmd {
    ~AIDoSkill() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc
    char* m_w4 = nullptr;  // +0x10
};

struct AIEnableTimer : ScriptCmd {
    ~AIEnableTimer() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    bool m_w2 = false;  // +0x8
};

struct AIResetTimer : ScriptCmd {
    ~AIResetTimer() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};

struct AISetHP : ScriptCmd {
    ~AISetHP() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
};

struct AISetInt : ScriptCmd {
    ~AISetInt() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
    std::uint32_t m_w4 = 0;  // +0x10
    char* m_w5 = nullptr;  // +0x14
    std::int32_t m_w6 = 0;  // +0x18
};

struct ActivateProjectileTrap : ScriptCmd {
    ~ActivateProjectileTrap() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
};

struct ActivateTriggerPlate : ScriptCmd {
    ~ActivateTriggerPlate() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
};

struct AutoEquip : ScriptCmd {
    ~AutoEquip() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
};

struct AwardEndGameTrophies : ScriptCmd {
    ~AwardEndGameTrophies() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};

struct AwardTrophy : ScriptCmd {
    ~AwardTrophy() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
};

struct BlockSaveGame : ScriptCmd {
    ~BlockSaveGame() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};

struct CONSOLE : ScriptCmd {
    ~CONSOLE() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
};

struct ChangeLevel : ScriptCmd {
    ~ChangeLevel() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc
    char* m_w4 = nullptr;  // +0x10
};

struct CloseDoor : ScriptCmd {
    ~CloseDoor() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
    bool m_w4 = false;  // +0x10
};

struct DeactivateProjectileTrap : ScriptCmd {
    ~DeactivateProjectileTrap() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
};

struct DeactivateTriggerPlate : ScriptCmd {
    ~DeactivateTriggerPlate() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
};

struct DoTutorial : ScriptCmd {
    ~DoTutorial() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
    std::int32_t m_w4 = 0;  // +0x10
};

struct DropLoot : ScriptCmd {
    ~DropLoot() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
    std::uint32_t m_w4 = 0;  // +0x10
    char* m_w5 = nullptr;  // +0x14
};

struct EndGame : ScriptCmd {
    ~EndGame() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};

struct EnqueueCharMenuTutorialMessage : ScriptCmd {
    ~EnqueueCharMenuTutorialMessage() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
    std::uint32_t m_w4 = 0;  // +0x10
    char* m_w5 = nullptr;  // +0x14
    std::int32_t m_w6 = 0;  // +0x18
};

struct EnqueueTutorialMessage : ScriptCmd {
    ~EnqueueTutorialMessage() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
    std::int32_t m_w3 = 0;  // +0xc
};

struct EnterCutSceneMode : ScriptCmd {
    ~EnterCutSceneMode() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};

struct EnterSafeZone : ScriptCmd {
    ~EnterSafeZone() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};

struct ExecScript : ScriptCmd {
    ~ExecScript() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    bool m_w2 = false;  // +0x8
    std::int32_t m_w3 = 0;  // +0xc
    std::uint32_t m_w4 = 0;  // +0x10, count of m_w5
    std::int32_t* m_w5 = nullptr;  // +0x14, m_w4 script ids
    bool m_w6 = false;  // +0x18
};

struct ExitCutSceneMode : ScriptCmd {
    ~ExitCutSceneMode() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};

struct FlushMessages : ScriptCmd {
    ~FlushMessages() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};

struct HideActor : ScriptCmd {
    ~HideActor() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
};

struct HideFlash : ScriptCmd {
    ~HideFlash() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc
    char* m_w4 = nullptr;  // +0x10
    bool m_w5 = false;  // +0x14
};

struct IncFaeryLevel : ScriptCmd {
    ~IncFaeryLevel() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
};

struct KillActor : ScriptCmd {
    ~KillActor() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
};

struct LeaveSafeZone : ScriptCmd {
    ~LeaveSafeZone() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};

struct LockCharacter : ScriptCmd {
    ~LockCharacter() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
};

struct LockTutorial : ScriptCmd {
    ~LockTutorial() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
};

struct LookActor : ScriptCmd {
    ~LookActor() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
    std::uint32_t m_w4 = 0;  // +0x10
    char* m_w5 = nullptr;  // +0x14
};

struct MarkCharacterAsScripted : ScriptCmd {
    ~MarkCharacterAsScripted() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    bool m_w2 = false;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc
    char* m_w4 = nullptr;  // +0x10
};

struct MoveActor : ScriptCmd {
    ~MoveActor() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
    bool m_w4 = false;  // +0x10
    std::uint32_t m_w5 = 0;  // +0x14
    char* m_w6 = nullptr;  // +0x18
    bool m_w7 = false;  // +0x1c
};

struct NilCmd : ScriptCmd {
    ~NilCmd() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};

struct OpenDoor : ScriptCmd {
    ~OpenDoor() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
    bool m_w4 = false;  // +0x10
};

struct PlayActorAnim : ScriptCmd {
    ~PlayActorAnim() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
    std::int32_t m_w3 = 0;  // +0xc
    std::int32_t m_w4 = 0;  // +0x10
    std::uint32_t m_w5 = 0;  // +0x14
    char* m_w6 = nullptr;  // +0x18
    bool m_w7 = false;  // +0x1c
};

struct PlayAnimById : ScriptCmd {
    ~PlayAnimById() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
    bool m_w3 = false;  // +0xc
    std::uint32_t m_w4 = 0;  // +0x10
    char* m_w5 = nullptr;  // +0x14
    bool m_w6 = false;  // +0x18
};

struct PlayAnimByName : ScriptCmd {
    ~PlayAnimByName() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
    bool m_w4 = false;  // +0x10
    std::uint32_t m_w5 = 0;  // +0x14
    char* m_w6 = nullptr;  // +0x18
    bool m_w7 = false;  // +0x1c
};

struct PlayCamera : ScriptCmd {
    ~PlayCamera() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
    bool m_w3 = false;  // +0xc
};

// Three ints with a vptr. Defined here because Structs::PlayEffect embeds one (it moved from structs_records.hpp).
class Pos3D {
public:
    virtual ~Pos3D();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::int32_t m_w1 = 0;  // +0x4
    std::int32_t m_w2 = 0;  // +0x8
    std::int32_t m_w3 = 0;  // +0xc
};

struct PlayEffect : ScriptCmd {
    ~PlayEffect() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8 effect id (FX set)
    Pos3D m_w3;  // +0xc embedded Pos3D (vptr at +0xc, ints at +0x10, +0x14 and +0x18)
    std::uint32_t m_w7 = 0;  // +0x1c length of m_w8
    char* m_w8 = nullptr;  // +0x20 string
    bool m_w9 = false;  // +0x24
};

struct PlayLevelMusic : ScriptCmd {
    ~PlayLevelMusic() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
};

struct PlaySound : ScriptCmd {
    ~PlaySound() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
    bool m_w3 = false;  // +0xc
    bool m_off13 = false;  // +0xd
    std::int32_t m_w4 = 0;  // +0x10
};

struct PutCharacterInIdle : ScriptCmd {
    ~PutCharacterInIdle() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
};

struct PutCharacterInLimbus : ScriptCmd {
    ~PutCharacterInLimbus() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    bool m_w2 = false;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc
    char* m_w4 = nullptr;  // +0x10
};

struct ReEquipHands : ScriptCmd {
    ~ReEquipHands() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
};

struct RestartLevel : ScriptCmd {
    ~RestartLevel() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};

struct SaveGame : ScriptCmd {
    ~SaveGame() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};

struct SetActorMaster : ScriptCmd {
    ~SetActorMaster() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    bool m_w2 = false;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc
    char* m_w4 = nullptr;  // +0x10
    bool m_w5 = false;  // +0x14
    std::uint32_t m_w6 = 0;  // +0x18
    char* m_w7 = nullptr;  // +0x1c
};

struct SetActorPosition : ScriptCmd {
    ~SetActorPosition() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
    std::uint32_t m_w4 = 0;  // +0x10
    char* m_w5 = nullptr;  // +0x14
};

struct SetCamera : ScriptCmd {
    ~SetCamera() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
    std::uint32_t m_w4 = 0;  // +0x10
    char* m_w5 = nullptr;  // +0x14
    std::int32_t m_w6 = 0;  // +0x18
    bool m_w7 = false;  // +0x1c
};

struct SetCameraClip : ScriptCmd {
    ~SetCameraClip() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
    std::int32_t m_w3 = 0;  // +0xc
};

struct SetCameraTarget : ScriptCmd {
    ~SetCameraTarget() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc
    char* m_w4 = nullptr;  // +0x10
    bool m_w5 = false;  // +0x14
};

struct SetFaeryState : ScriptCmd {
    ~SetFaeryState() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
    std::int32_t m_w3 = 0;  // +0xc
};

struct SetLevelState : ScriptCmd {
    ~SetLevelState() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
    std::int32_t m_w3 = 0;  // +0xc
};

struct SetStatic : ScriptCmd {
    ~SetStatic() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    bool m_w2 = false;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc
    char* m_w4 = nullptr;  // +0x10
};

struct SetWorldMapLocationState : ScriptCmd {
    ~SetWorldMapLocationState() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
    std::int32_t m_w3 = 0;  // +0xc
};

struct ShowActor : ScriptCmd {
    ~ShowActor() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
};

struct ShowFlash : ScriptCmd {
    ~ShowFlash() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc
    char* m_w4 = nullptr;  // +0x10
    bool m_w5 = false;  // +0x14
};

struct ShowTrophies : ScriptCmd {
    ~ShowTrophies() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};

struct SkipAllCharMenuTutorialMessages : ScriptCmd {
    ~SkipAllCharMenuTutorialMessages() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};

struct SkipAllTutorialMessages : ScriptCmd {
    ~SkipAllTutorialMessages() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};

struct SkipCharMenuTutorialMessage : ScriptCmd {
    ~SkipCharMenuTutorialMessage() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};

struct SkipTutorialMessage : ScriptCmd {
    ~SkipTutorialMessage() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};

struct SpawnCharacter : ScriptCmd {
    ~SpawnCharacter() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
};

struct SpawnContainer : ScriptCmd {
    ~SpawnContainer() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
};

struct StartDialog : ScriptCmd {
    ~StartDialog() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
    std::int32_t m_w3 = 0;  // +0xc
    std::int32_t m_w4 = 0;  // +0x10
};

struct StartDialogID : ScriptCmd {
    ~StartDialogID() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
};

struct StopActor : ScriptCmd {
    ~StopActor() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
};

struct StopEffect : ScriptCmd {
    ~StopEffect() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
};

struct StopSound : ScriptCmd {
    ~StopSound() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
    bool m_w3 = false;  // +0xc
    std::int32_t m_w4 = 0;  // +0x10
};

struct UnEquipHands : ScriptCmd {
    ~UnEquipHands() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
};

struct UnEquipItemFromSlot : ScriptCmd {
    ~UnEquipItemFromSlot() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc
    char* m_w4 = nullptr;  // +0x10
};

struct UnlockCharacter : ScriptCmd {
    ~UnlockCharacter() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::uint32_t m_w2 = 0;  // +0x8
    char* m_w3 = nullptr;  // +0xc
};

struct Wait : ScriptCmd {
    ~Wait() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    std::int32_t m_w2 = 0;  // +0x8
};

struct WaitCamera : ScriptCmd {
    ~WaitCamera() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
    float m_w2 = 0.0f;  // +0x8
};

struct WaitDialog : ScriptCmd {
    ~WaitDialog() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};

template <class T>
ScriptCmd* GetNewScriptCmdDataInstance();

// Deleting destructors (D0) of ScriptCmd and the classes above.
void DeletingDestructor(ScriptCmd* self);
void DeletingDestructor(AIChangeScript* self);
void DeletingDestructor(AIDoSkill* self);
void DeletingDestructor(AIEnableTimer* self);
void DeletingDestructor(AIResetTimer* self);
void DeletingDestructor(AISetHP* self);
void DeletingDestructor(AISetInt* self);
void DeletingDestructor(ActivateProjectileTrap* self);
void DeletingDestructor(ActivateTriggerPlate* self);
void DeletingDestructor(AutoEquip* self);
void DeletingDestructor(AwardEndGameTrophies* self);
void DeletingDestructor(AwardTrophy* self);
void DeletingDestructor(BlockSaveGame* self);
void DeletingDestructor(CONSOLE* self);
void DeletingDestructor(ChangeLevel* self);
void DeletingDestructor(CloseDoor* self);
void DeletingDestructor(DeactivateProjectileTrap* self);
void DeletingDestructor(DeactivateTriggerPlate* self);
void DeletingDestructor(DoTutorial* self);
void DeletingDestructor(DropLoot* self);
void DeletingDestructor(EndGame* self);
void DeletingDestructor(EnqueueCharMenuTutorialMessage* self);
void DeletingDestructor(EnqueueTutorialMessage* self);
void DeletingDestructor(EnterCutSceneMode* self);
void DeletingDestructor(EnterSafeZone* self);
void DeletingDestructor(ExitCutSceneMode* self);
void DeletingDestructor(FlushMessages* self);
void DeletingDestructor(HideActor* self);
void DeletingDestructor(HideFlash* self);
void DeletingDestructor(IncFaeryLevel* self);
void DeletingDestructor(KillActor* self);
void DeletingDestructor(LeaveSafeZone* self);
void DeletingDestructor(LockCharacter* self);
void DeletingDestructor(LockTutorial* self);
void DeletingDestructor(LookActor* self);
void DeletingDestructor(MarkCharacterAsScripted* self);
void DeletingDestructor(MoveActor* self);
void DeletingDestructor(NilCmd* self);
void DeletingDestructor(OpenDoor* self);
void DeletingDestructor(PlayActorAnim* self);
void DeletingDestructor(PlayAnimById* self);
void DeletingDestructor(PlayAnimByName* self);
void DeletingDestructor(PlayCamera* self);
void DeletingDestructor(PlayLevelMusic* self);
void DeletingDestructor(PlaySound* self);
void DeletingDestructor(PutCharacterInIdle* self);
void DeletingDestructor(PutCharacterInLimbus* self);
void DeletingDestructor(ReEquipHands* self);
void DeletingDestructor(RestartLevel* self);
void DeletingDestructor(SaveGame* self);
void DeletingDestructor(SetActorMaster* self);
void DeletingDestructor(SetActorPosition* self);
void DeletingDestructor(SetCamera* self);
void DeletingDestructor(SetCameraClip* self);
void DeletingDestructor(SetCameraTarget* self);
void DeletingDestructor(SetFaeryState* self);
void DeletingDestructor(SetLevelState* self);
void DeletingDestructor(SetStatic* self);
void DeletingDestructor(SetWorldMapLocationState* self);
void DeletingDestructor(ShowActor* self);
void DeletingDestructor(ShowFlash* self);
void DeletingDestructor(ShowTrophies* self);
void DeletingDestructor(SkipAllCharMenuTutorialMessages* self);
void DeletingDestructor(SkipAllTutorialMessages* self);
void DeletingDestructor(SkipCharMenuTutorialMessage* self);
void DeletingDestructor(SkipTutorialMessage* self);
void DeletingDestructor(SpawnCharacter* self);
void DeletingDestructor(SpawnContainer* self);
void DeletingDestructor(StartDialog* self);
void DeletingDestructor(StartDialogID* self);
void DeletingDestructor(StopActor* self);
void DeletingDestructor(StopEffect* self);
void DeletingDestructor(StopSound* self);
void DeletingDestructor(UnEquipHands* self);
void DeletingDestructor(UnEquipItemFromSlot* self);
void DeletingDestructor(UnlockCharacter* self);
void DeletingDestructor(ExecScript* self);
void DeletingDestructor(PlayEffect* self);
void DeletingDestructor(Wait* self);
void DeletingDestructor(WaitCamera* self);
void DeletingDestructor(WaitDialog* self);

}  // namespace Structs
