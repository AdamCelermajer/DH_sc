// TriggerZoneExitLevel: a TriggerZone that offers a fast travel point to the level named by "levelName".
// Reconstructed from the Hex-Rays pseudocode of libDungeonHunter2.so. The layout is in dh2/TriggerZoneExitLevel.hpp.
// Offsets in the comments are the original 32-bit ones.
//
// Include conflict (affects InitPost and Update, both PARTIAL). dh2/TriggerZone.hpp pulls in
// net-transport/deps/byte_array.hpp, which defines an inline CustomFree. Character.hpp (through CharProperties and
// pydata-accessors/structs_records.hpp), structs_records.hpp itself and pydata-accessors/member_names.hpp each pull in
// pydata-accessors/deps/stream_reader.hpp, which defines a second inline CustomFree. member_names.hpp and
// w7_arrays_00.hpp also define the same Arrays structs. A translation unit that includes the owner header therefore
// cannot include Character.hpp, structs_records.hpp or member_names.hpp until those duplicates are resolved in their
// deps headers (outside this batch: those headers are read-only here).
#include "dh2/TriggerZoneExitLevel.hpp"

#include "dh2/Application.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/Level.hpp"
#include "dh2/global/ctrl_camera.hpp"  // ApplicationGetInstance()

// 0x39c9a4 TriggerZoneExitLevel::TriggerZoneExitLevel(ObjectBase::GO_IDS)   (C1)
// 0x39ca40 TriggerZoneExitLevel::TriggerZoneExitLevel(ObjectBase::GO_IDS)   (C2, the same body)
TriggerZoneExitLevel::TriggerZoneExitLevel(ObjectBase::GO_IDS id)
    : TriggerZone(id),
      m_levelIndex(-1),             // +0x7d4, -1 until InitPost
      m_fastTravelUnlocked(false),  // +0x818
      m_fastTravelShown(false) {    // +0x819
    // The original also stores the three vtable pointers (C++ does this itself) and leaves m_levelName (+0x7dc) and
    // m_fastTravelId (+0x800) empty, which the std::string default constructor gives. It does not write m_levelID
    // (+0x7d8), m_entrypointID (+0x7f4), m_mapLoc (+0x7f8) or m_question (+0x7fc), so they have no initializer here.
}

// 0x39c68c TriggerZoneExitLevel::~TriggerZoneExitLevel()   (D1, complete object)
// 0x39c708 TriggerZoneExitLevel::~TriggerZoneExitLevel()   (D2, base object; the same body)
TriggerZoneExitLevel::~TriggerZoneExitLevel() {
    // The original stores the vtable pointers, releases m_fastTravelId (+0x800) and then m_levelName (+0x7dc), and
    // calls ~TriggerZone. Here the members are destroyed after this body in reverse declaration order, which frees
    // the two strings in the same order, and then the base destructor runs.
}

// 0x39c8c8 TriggerZoneExitLevel::InitPost()
// PARTIAL: the name lookup is not built (see the include conflict above), so m_levelIndex stays -1.
void TriggerZoneExitLevel::InitPost() {
    TriggerZone::InitPost();
    // The original resolves m_levelName when it is not empty (its two string pointers at +0x7ec and +0x7f0 differ;
    // the end pointer is at +0x7ec, as in std::priv::_String_base::_M_allocate_block, 0x31167c) with
    // Arrays::GetMemberIDByString<Arrays::LevelList>(m_levelName.c_str()) and stores the id at +0x7d4 (m_levelIndex).
    // That template is declared in pydata-accessors/member_names.hpp, which cannot be included here.
}

// 0x39c8f8 TriggerZoneExitLevel::DeclareProperties()
void TriggerZoneExitLevel::DeclareProperties() {
    TriggerZone::DeclareProperties();
    // The int properties are the compiler clone PropertyMap::AddProperty<int>.clone.1 (0x39c83c). It is the
    // three-argument template with defaultValue -1 (it stores -1 at Property +0x20), so the call is written that way.
    AddProperty<int>("levelID", m_levelID, -1);
    AddProperty<std::string>("levelName", m_levelName);
    AddProperty<int>("entrypointID", m_entrypointID, -1);
    AddProperty<int>("maploc", m_mapLoc, -1);
    AddProperty<int>("question", m_question, -1);
    AddProperty<std::string>("fasttravel", m_fastTravelId);
}

// 0x39c33c TriggerZoneExitLevel::Update()
// PARTIAL. Rebuilt: the idle sound, the refresh of unk_3c0 (+0x3c0, with the online test stubbed) and the return while
// the current level is not loaded. Not rebuilt, in the original order:
//   1. The first test reads COnline::isOnlineGame_ (GetOnline() +0x05). In net-session-online/include/net-session-online/
//      c_online.hpp that flag is protected with no accessor, so it is stubbed to false (offline) below and GetOnline() is
//      not called. In an online game the original skips the refresh when IsRemotelyUpdated() is true; this build always
//      refreshes. A public accessor for the flag is needed (layout request).
//   2. Reading the local player (PlayerManager::GetLocalPlayer(0, true), PlayerInfo +0x660 Character) and the
//      fast travel unlock (Character::SG_SetFastTravelIdUnlocked(m_fastTravelId, true, -1), once unk_3c0 is set and
//      m_fastTravelUnlocked is clear). Needs Character.hpp (include conflict above).
//   3. The touch test: GameObject::IsTouching(character), PlayerManager::IsLocalPlayerHosting() and
//      GameObject::MeetCondition(). The first needs Character.hpp (include conflict above).
//   4. GameObject::RequireOnlineUpdate(), then, when m_levelIndex is not -1 and m_fastTravelShown is clear, the
//      DisplayFastTravel call with show = true, or with show = false when the test in step 3 fails and the panel is
//      shown. The panel call takes the localized level name (StringManager::getString of the LevelList row's +0x24 word,
//      Structs::LevelDeclaration::m_w9), the level name and the entry point (+0x7f4). It needs member_names.hpp and
//      structs_records.hpp (include conflict above). The local player's Character null case is the hide path.
void TriggerZoneExitLevel::Update() {
    // GameObject +0x370 m_idleSoundId, -1 when the object has no idle sound.
    if (m_idleSoundId >= 0) {
        GameObject::UpdateIdleSound();
    }

    constexpr bool kOnlineGameFlag = false;  // stub for COnline +0x05, see step 1 above
    if (!kOnlineGameFlag || !IsRemotelyUpdated()) {
        unk_3c0 = Trigger::GetNumPlayerActivating();  // +0x3c0
    }

    // Level::m_isLoaded (+0x144). A null current level passes through, as in the original.
    Level* level = ApplicationGetInstance().GetCurrentLevel();
    if (level != nullptr && !level->m_isLoaded) {
        return;
    }

    // Steps 2 to 4 above are not rebuilt in this translation unit.
}
