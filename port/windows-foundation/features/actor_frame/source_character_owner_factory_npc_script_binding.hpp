#pragma once

#include "../../../level-world/character_script_assets_v1.hpp"
#include "../../../level-world/canonical_character_candidate_v60.hpp"

namespace dh2::windows_foundation {

// Binds the source AI script bytes and same-record Character aliases used at
// CanonicalCharacterCandidateRecordV60::load_script. Host, level, controller,
// timer-expiry and gameplay services remain owned by their actual providers.
class SourceCharacterOwnerFactoryNpcScriptBindingV1 {
 character::CharacterScriptAssetsV1 assets_;
 bool loaded_{};
public:
 bool load(const character::ScriptAssetServicesV1&,
  data::SkillTables::Borrow,std::string& error);

 bool bind_resources(world::CanonicalCharacterCandidateRecordV60&,
  character::CharacterScriptSessionInput&,
  const dh2_script_object_services*,std::string& error)const;

 character::CharacterScriptAssetsV1::Borrow borrow()const noexcept{
  return assets_.borrow();
 }
};

}
