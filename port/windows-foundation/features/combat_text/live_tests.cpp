#include "combat_text_live.hpp"
#include "../../source_root_scopes.hpp"
#include "../../source_world_objects.hpp"
#include <filesystem>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
void check(bool v,const std::string&e){if(!v)throw std::runtime_error(e);}
int main(int argc,char**argv){try{
 check(argc==2,"Repository root required");const auto repo=std::filesystem::path(argv[1]);AssetCatalog assets(repo/".local-inputs/windows-shared-assets");std::string error;
 OriginalPropertyDatabase database;OriginalMeleeBindings bindings;check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);check(bindings.load(assets,"original-melee-bindings.xml",error),error);
 CombatSessionConfig config;config.diagnosticRngSeed=1234;config.playerId=1;config.playerProfileId="KnightPlayerBase";config.tableRoot="original-cache/data/pydata";
 auto data=assets.read(config.tableRoot+"/loot_table_pyarray.bin"),names=assets.read(config.tableRoot+"/loot_table_pyarraynames.bin"),schema=assets.read(config.tableRoot+"/loot_table_pystructnames.bin");dh2::data::ItemTable items;
 check(dh2::data::load_items({data.data(),data.size()},{names.data(),names.size()},{schema.data(),schema.size()},items,error),error);config.mainItemId=items.identifiers.at(664);config.equippedItemIds={config.mainItemId};
 ActorCustomization customization;customization.skin_id_contains="_default_warrior-mesh-skin";customization.expected_controller_count=4;customization.allow_missing_animation_targets=true;
 OriginalCombatVisualPlan plan;check(build_original_combat_visual_plan(assets,bindings,config.playerProfileId,customization,"live-combat-text-fixture",plan,error),error);config.playerVisualConfig=plan.config;config.playerVisualConfig.clips={{"idle",plan.phase("Idle",0,{0})->resolvedPath},{"walk",plan.phase("Idle",0,{0})->resolvedPath}};
 CombatSessionProfile knight;knight.action={"AttackStatic",0,{0,1}};knight.initialIdle={"Idle",0,{0}};knight.damageMarkerNames={"attack_mainhand"};knight.propertyOptions={256,true};config.profiles.emplace(config.playerProfileId,knight);
 CombatSessionProfile lizard;lizard.action={"Attack",0,{0,1}};lizard.initialIdle={"Idle",0,{0}};lizard.damageMarkerNames={"attack_mainhand"};lizard.propertyOptions={std::nullopt,true};lizard.customization.allow_missing_animation_targets=true;config.profiles.emplace("Swamp_LizadMan_Type1",lizard);
 ActorPopulation population;PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";placed.definition.stableId=2;placed.definition.sourceId="source-text-boundary-fixture";placed.transform={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};population.actors().push_back(std::move(placed));CharacterVisual player;CombatSession session;
 check(session.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),error);
 std::vector<ActorDefinition>definitions;check(load_actor_definitions(assets,"original-cache/data/scene/001_swamp.mlx",definitions,error),error);SourceWorldObjects objects;check(objects.load(definitions,error),error);SourceRootScopes scopes;check(scopes.build(assets,"original-cache/data/scene/001_swamp.mlx",definitions,objects,error),error);
 CombatTextLiveServices services;services.delivery=CombatTextDeliveryMode::synchronous_snapshot;
 services.design.debug=[&](const char*key,std::string&e){bool ignored=false;return scopes.debug_switch(key,ignored,e);};
 services.design.player_character=[&](std::uintptr_t&out,std::string&){out=session.player_id();return true;};services.design.player_name=[](std::uintptr_t,std::string&out,std::string&){out="TextFixture";return true;};
 // Explicit geometry/projection boundary fixtures, not a claimed original body
 // plan/Camera owner. They borrow the SAME live actor transform to test capture.
 float height=30;services.target_position=[&](ActorId id,std::array<float,3>&out,std::string&e){const auto*a=session.actor(id);if(!a){e="Actor absent";return false;}out=a->transform.position;return true;};services.bounds_height=[&](ActorId,float&out,std::string&){out=height;return true;};
 std::vector<Vec3>projected;services.project=[&](Vec3 p,float&x,float&y,std::string&){projected.push_back(p);x=p.x+240;y=160-p.z;return true;};std::size_t drawn=0;services.glyph_draw=[&](const auto&quads,std::string&){drawn+=quads.size();return true;};
 CombatTextLiveAdapter adapter;check(adapter.load(assets,session,services,error),error);
 const auto hp=session.actor(2)->health;session.world()->set_resolution_observer([&](const auto&r,std::string&e){return adapter.capture_resolution(r,e);});
 float damage=0;check(session.world()->resolve_damage("actor-1/melee",*session.actor(1),*session.actor(2),"attack_mainhand",damage,error),error);
 check(session.actor(2)->health==hp,"Presentation observer mutated HP");check(!adapter.captured_events().empty(),"Genuine original melee outcome did not reach text capture");
 const auto captured=adapter.captured_events().front().position;session.actor(2)->transform.position[0]+=500;height=90;
 check(adapter.after_host_update(1,33,1,1,error),error);check(!projected.empty()&&projected.front().x==captured.x&&projected.front().z==captured.z,"Post-result motion changed captured target/bounds point");
 const auto count=adapter.active_count();check(adapter.after_host_update(1,100000,1,1,error)&&adapter.active_count()==count,"Same frame duplicated text/advanced a second clock");check(adapter.draw(1,1,error)&&drawn>0,error);
 adapter.clear_for_reload();PlayableCombatResolution zero;zero.attacker=1;zero.victim=2;zero.melee.original.amount=0;check(adapter.capture_resolution(zero,error)&&adapter.captured_events().empty(),"Zero damage fabricated MISS");zero.melee.original.outcomes=1;check(adapter.capture_resolution(zero,error)&&adapter.captured_events().front().text=="Miss",error);
 std::uint32_t dt=33;std::int32_t phase=16;dh2::ui::CombatFlashTickBorrowV1 clock{&dt,reinterpret_cast<std::uintptr_t>(&phase),nullptr};check(!adapter.after_source_update(2,clock,1,1,error),"Positive selected source Level accepted missing phase provider");clock.load_phase=&phase;check(adapter.after_source_update(2,clock,1,1,error)&&adapter.active_count()==1,error);dt=1;check(adapter.after_source_update(3,clock,1,1,error),error);
 adapter.clear_for_reload();check(adapter.active_count()==0&&adapter.captured_events().empty(),"Reload retained stale presentation");
 check(std::string(original_combat_text_font_resource())=="data/Fontin SmallCaps.ttf"&&original_combat_text_frame_rate()==30,"Actual SWF font/fps provenance differs");
 std::cout<<"PASS SAME actual combat resolution observer, HP unchanged, pre-motion source-point snapshot, authentic localization/font, one caller clock/frame, source clock-provider guards\n";return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
