#pragma once

// ScriptCmd base and one class per GetNewScriptCmdDataInstance<T> target.
// Each original allocation is a vtable word plus the words the constructor stores;
// the stored words are modelled as zero-initialised members (+0x4 per word).
#include <cstdint>

namespace Structs {

class ScriptCmd {
public:
    virtual ~ScriptCmd() = default;
};

struct FlushMessages : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
};

struct DoTutorial : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct LockTutorial : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8
};

struct SkipAllCharMenuTutorialMessages : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
};

struct SkipCharMenuTutorialMessage : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
};

struct EnqueueCharMenuTutorialMessage : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
    std::uint32_t m_w5 = 0;  // +0x14
};

struct SkipAllTutorialMessages : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
};

struct SkipTutorialMessage : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
};

struct EnqueueTutorialMessage : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc
};

struct BlockSaveGame : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
};

struct SaveGame : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
};

struct ShowTrophies : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
};

struct AwardEndGameTrophies : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
};

struct AwardTrophy : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8
};

struct EndGame : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
};

struct ChangeLevel : ScriptCmd {
    std::uint32_t m_w4 = 0;  // +0x10
};

struct RestartLevel : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
};

struct SetWorldMapLocationState : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc
};

struct SetLevelState : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc
};

struct SpawnContainer : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct DeactivateTriggerPlate : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct ActivateTriggerPlate : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct DeactivateProjectileTrap : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct ActivateProjectileTrap : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct CloseDoor : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct OpenDoor : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct SetStatic : ScriptCmd {
    std::uint32_t m_w4 = 0;  // +0x10
};

struct ReEquipHands : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct UnEquipHands : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct UnEquipItemFromSlot : ScriptCmd {
    std::uint32_t m_w4 = 0;  // +0x10
};

struct AutoEquip : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct DropLoot : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
    std::uint32_t m_w5 = 0;  // +0x14
};

struct SetActorMaster : ScriptCmd {
    std::uint32_t m_w4 = 0;  // +0x10
    std::uint32_t m_w7 = 0;  // +0x1c
};

struct SetActorPosition : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
    std::uint32_t m_w5 = 0;  // +0x14
};

struct PlayActorAnim : ScriptCmd {
    std::uint32_t m_w6 = 0;  // +0x18
};

struct KillActor : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct HideActor : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct ShowActor : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct LookActor : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
    std::uint32_t m_w5 = 0;  // +0x14
};

struct MoveActor : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
    std::uint32_t m_w6 = 0;  // +0x18
};

struct StopActor : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct AISetInt : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
    std::uint32_t m_w5 = 0;  // +0x14
};

struct AIChangeScript : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
    std::uint32_t m_w5 = 0;  // +0x14
};

struct AISetHP : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8
};

struct AIDoSkill : ScriptCmd {
    std::uint32_t m_w4 = 0;  // +0x10
};

struct AIResetTimer : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
};

struct AIEnableTimer : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
};

struct MarkCharacterAsScripted : ScriptCmd {
    std::uint32_t m_w4 = 0;  // +0x10
};

struct PutCharacterInIdle : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct SpawnCharacter : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct PutCharacterInLimbus : ScriptCmd {
    std::uint32_t m_w4 = 0;  // +0x10
};

struct IncFaeryLevel : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8
};

struct SetFaeryState : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc
};

struct Wait : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8
};

struct UnlockCharacter : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct LockCharacter : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct HideFlash : ScriptCmd {
    std::uint32_t m_w4 = 0;  // +0x10
};

struct ShowFlash : ScriptCmd {
    std::uint32_t m_w4 = 0;  // +0x10
};

struct StopEffect : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8
};

struct PlayEffect : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc // original stores a link-time address here; left null
    std::uint32_t m_w8 = 0;  // +0x20
};

struct PlayAnimByName : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
    std::uint32_t m_w6 = 0;  // +0x18
};

struct PlayAnimById : ScriptCmd {
    std::uint32_t m_w5 = 0;  // +0x14
};

struct LeaveSafeZone : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
};

struct EnterSafeZone : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
};

struct PlayLevelMusic : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8
};

struct StopSound : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w4 = 0;  // +0x10
};

struct PlaySound : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w4 = 0;  // +0x10
};

struct WaitDialog : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
};

struct StartDialogID : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8
};

struct StartDialog : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc
    std::uint32_t m_w4 = 0;  // +0x10
};

struct NilCmd : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
};

struct SetCameraTarget : ScriptCmd {
    std::uint32_t m_w4 = 0;  // +0x10
};

struct WaitCamera : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8
};

struct SetCameraClip : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc
};

struct PlayCamera : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
    std::uint32_t m_w2 = 0;  // +0x8
};

struct SetCamera : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
    std::uint32_t m_w5 = 0;  // +0x14
};

struct CONSOLE : ScriptCmd {
    std::uint32_t m_w3 = 0;  // +0xc
};

struct ExitCutSceneMode : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
};

struct EnterCutSceneMode : ScriptCmd {
    std::uint32_t m_w1 = 0;  // +0x4
};

struct ExecScript : ScriptCmd {
    std::uint32_t m_w5 = 0;  // +0x14
};

template <class T>
ScriptCmd* GetNewScriptCmdDataInstance();

}  // namespace Structs
