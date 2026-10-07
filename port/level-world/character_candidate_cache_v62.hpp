#pragma once
#include "character_script_assets_v1.hpp"
#include "character_combat_sound_tables_v2.hpp"
#include "../game-data/animation_tables.hpp"
#include "../game-data/loot_tables_v2.hpp"
#include "../game-data/character_templates_v78.hpp"
namespace dh2::character {
// Actual shared resource tables for one source World. Authored script animation
// commands mutate only their original AnimTable step fields. No pose/Character,
// animation occurrence bank, mutable inventory or profile is copied here.
class CharacterCandidateCacheV62 {
 ScriptAssetServicesV1 files_;bool attempted_{},ready_{};
 data::Dictionary models_,animations_;data::AnimationTables tables_;
 data::LootTablesV2 loot_;
 CharacterCombatSoundTablesV2 sounds_v70_;
 std::shared_ptr<const data::CharacterTemplateTableV78> templates_;
 bool read(const char*,std::vector<std::uint8_t>&,std::string&);
public:
 explicit CharacterCandidateCacheV62(ScriptAssetServicesV1 files):files_(std::move(files)){}
 bool load(std::string&);
 bool ready()const noexcept{return ready_;}
 const data::Dictionary& models()const noexcept{return models_;}
 const data::Dictionary& animations()const noexcept{return animations_;}
 const data::AnimationTables& animation_tables()const noexcept{return tables_;}
 bool source_script_animation_steps_v116(std::int32_t base,std::int32_t first,std::int32_t second,std::string& e){
  const auto a=static_cast<std::int64_t>(base)+2,b=static_cast<std::int64_t>(base)+8;
  if(!ready_||a<0||b<0||static_cast<std::uint64_t>(a)>=tables_.sequences.size()||
     static_cast<std::uint64_t>(b)>=tables_.sequences.size()||tables_.sequences[std::size_t(a)].steps.empty()||
     tables_.sequences[std::size_t(b)].steps.empty()){
   e="Required actual Script_PlayActorAnim first-step row storage";return false;
  }
  //45e968/45e984: shared original first-step anim words, in source order.
  tables_.sequences[std::size_t(a)].steps[0].anim=first;
  tables_.sequences[std::size_t(b)].steps[0].anim=second;
  e.clear();return true;
 }
 data::LootTablesV2& loot()noexcept{return loot_;}
 const data::LootTablesV2& loot()const noexcept{return loot_;}
 const CharacterCombatSoundTablesV2& sounds_v70()const noexcept{return sounds_v70_;}
 const std::shared_ptr<const data::CharacterTemplateTableV78>& templates_v78()const noexcept{return templates_;}
 const ScriptAssetServicesV1& files()const noexcept{return files_;}
};
}
