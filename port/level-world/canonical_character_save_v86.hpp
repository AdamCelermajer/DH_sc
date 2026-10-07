#pragma once
#include "retained_character_save_connection_v3.hpp"
namespace dh2::world {struct CanonicalCharacterCandidateRecordV60;}
namespace dh2::character {
//Character.Clean positive14e8 deleting body, independent of OBJS serializer.
//The caller performs the original14e8 NULL store only after this completes.
bool destroy_canonical_character_save_v108(world::CanonicalCharacterCandidateRecordV60&,std::uintptr_t,std::string&);
struct CanonicalCharacterSaveServicesV86 {
 std::shared_ptr<void> provider; //independent native transport, World/App weak
 std::function<bool(std::uint32_t,bool&,std::string&)> current_level_byte;
 std::function<bool(std::uintptr_t,std::string&)> sync_visibility;
 std::function<bool(std::uintptr_t,bool,std::string&)> set_visible;
 std::function<bool(std::uintptr_t,std::string&)> update_anchor;
 BuffServices16 effects;
};
//Whole Character serializer composition on its real canonical record. Owns
//only missing legacy wire tags, callbacks, and the sole NPC CharProperties
//Buff owner; player Buff/Save/FSM/controllers/physical/visual remain existing.
class CanonicalCharacterSaveV86 final {
 std::weak_ptr<world::CanonicalCharacterCandidateRecordV60> record_;
 CanonicalCharacterSaveServicesV86 services_;
 CharacterSaveMetadataV3 metadata_;
 BuffOwner* npc_buffs_{};BuffServices16 buff_services_{};
 BuffBindings32 buff_bindings_{};
 std::unique_ptr<RetainedCharacterSaveConnectionV3> connection_;
 std::string error_;bool attempted_{},failed_{};
 static int buff(void*,data::PropertyView*,const BuffRequest32*,std::uintptr_t*);
 static bool invoke(void*,const CharacterSaveRestoreRequestV3&,CharacterSaveRestoreResponseV3&,std::string&);
 bool extra(RetainedCharacterSaveExtraV3&,std::string&);
 bool script_lifecycle(bool initialize,std::string&);
 bool callback(const CharacterSaveRestoreRequestV3&,CharacterSaveRestoreResponseV3&,std::string&);
public:
 CanonicalCharacterSaveV86(std::weak_ptr<world::CanonicalCharacterCandidateRecordV60>,CanonicalCharacterSaveServicesV86);
 ~CanonicalCharacterSaveV86();
 CanonicalCharacterSaveV86(const CanonicalCharacterSaveV86&)=delete;
 bool initialize(std::string&);
 bool bind(level::LevelSaveObjectBorrowV2&,std::string&);
 BuffOwner* buffs()noexcept;
 bool borrow_buffs(BuffOwner*&,std::string&);
 //Called before Script/FX/props destruction. Required failed release is not
 //represented as successful teardown or silently discarded.
 bool close(std::string&);
};
}
