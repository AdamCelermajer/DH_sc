#include "runtime_world_item_adapter_v1.hpp"
#include "runtime_world_item_interaction_v1.hpp"
#include "runtime_loot_source_owner_v1.hpp"
#include "runtime_session_death_rewards_v1.hpp"
#include "../../combat_session.hpp"
#include "../../original_combat_visual_plan.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_actor_properties.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::loot;
using namespace dh2::data;

namespace {
void check(bool ok,const std::string& error){if(!ok)throw std::runtime_error(error);}
std::int32_t asr8(std::int32_t value){return value>>8;}
struct Context {
    std::map<ActorId,RuntimeDeathActorV1> characters;
    RuntimeWorldItemAdapterV1* store{};
    std::int32_t current_difficulty{},unlocked_difficulty{};
    bool rewards_suppressed{};
    std::size_t source_debug_requests{},one_kill_queries{},difficulty_reads{};
    std::size_t admission_reads{};
    static bool read_admission(void* raw,RuntimeDeathRewardAdmissionV1& out,std::string& error){
        auto& self=*static_cast<Context*>(raw);++self.admission_reads;
        out.rewards_suppressed=self.rewards_suppressed;error.clear();return true;
    }
    static bool resolve(void* raw,ActorId id,RuntimeDeathActorV1& out,std::string& error){
        auto& self=*static_cast<Context*>(raw);const auto found=self.characters.find(id);
        if(found==self.characters.end()){error="same-session CharacterState binding absent";return false;}
        out=found->second;error.clear();return true;
    }
    static bool read_difficulty(void* raw,std::int32_t& current,std::int32_t& unlocked,
                                std::string& error){
        auto& self=*static_cast<Context*>(raw);++self.difficulty_reads;
        current=self.current_difficulty;unlocked=self.unlocked_difficulty;error.clear();return true;
    }
    static bool source_entry(void* raw,const LootEntryRequestV8&,std::int32_t& value,
                             std::string& error){
        auto& self=*static_cast<Context*>(raw);++self.source_debug_requests;
        value=0;error.clear();return true;
    }
    static bool query_debug(void* raw,const char* key,bool& value,std::string& error){
        auto& self=*static_cast<Context*>(raw);
        if(!key||std::string(key)!="OneKillLevelUp"){
            error="Unexpected source XP Debug key";return false;
        }
        ++self.one_kill_queries;value=false;error.clear();return true;
    }
};
struct PickupContext {ActorId player{invalid_actor_id};std::shared_ptr<CharacterState> state;};
bool resolve_pickup(void* raw,ActorId player,std::shared_ptr<CharacterState>& out,std::string& error){
    auto& self=*static_cast<PickupContext*>(raw);
    if(player!=self.player||!self.state){error="same-session player CharacterState mismatch";return false;}
    out=self.state;error.clear();return true;
}
std::shared_ptr<CharacterState> player_state(const OriginalCombatProperties& properties){
    auto state=std::make_shared<CharacterState>(make_default_character("session-loot-player","Session Loot Player","warrior"));
    state->stats.level=std::uint32_t(std::max(1,asr8(properties.sheets.resolved[19])));
    state->experience=std::uint64_t(std::max(0,asr8(properties.sheets.resolved[33])));
    state->source_stat_points=std::uint32_t(std::max(0,asr8(properties.sheets.resolved[148])));
    state->source_skill_points=std::uint32_t(std::max(0,asr8(properties.sheets.resolved[157])));
    state->stats.health=original_signed256(properties.sheets.resolved[36]);
    state->stats.max_health=original_signed256(properties.sheets.resolved[38]);
    state->stats.resource=original_signed256(properties.sheets.resolved[41]);
    state->stats.max_resource=original_signed256(properties.sheets.resolved[43]);
    return state;
}
}

int main(int argc,char** argv) try{
    check(argc==2||argc==3||argc==4,"Supply repository root, optional source RNG seed, and optional suppressed mode");
    const auto repo=std::filesystem::path(argv[1]);
    const auto diagnostic_seed=argc>=3?std::uint32_t(std::stoul(argv[2])):146u;
    const bool suppressed_mode=argc==4&&std::string(argv[3])=="suppressed";
    AssetCatalog assets(repo/".local-inputs/windows-shared-assets"),
        loot_assets(repo/".local-inputs/windows-source-clock-v19-preview-9/assets"),
        bindings_assets(repo/".local-inputs/windows-melee-bindings");
    std::string error;OriginalMeleeBindings melee;
    check(melee.load(bindings_assets,"original-melee-bindings.xml",error),error);

    LootRandom8V2 creation_random{diagnostic_seed,0};
    frontend::creation::RuntimeCreationSourceOwnerV1 creation_source;
    check(frontend::creation::load_runtime_creation_source_v1(loot_assets,creation_random,
          creation_source,error),error);
    const auto& database=*creation_source.properties;

    ActorCustomization customization;customization.skin_id_contains="_default_warrior-mesh-skin";
    customization.expected_controller_count=4;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan visual_plan;
    check(build_original_combat_visual_plan(assets,melee,"KnightPlayerBase",customization,
          "death-drop-pickup-session",visual_plan,error),error);
    CombatSessionConfig config;config.diagnosticRngSeed=diagnostic_seed;config.playerId=1;
    config.playerProfileId="KnightPlayerBase";config.tableRoot="original-cache/data/pydata";
    config.playerVisualConfig=visual_plan.config;config.playerVisualConfig.motion_node_id="auto";
    const auto* idle=visual_plan.phase("Idle",0,{0});check(idle,"Original Knight idle clip missing");
    config.playerVisualConfig.clips={{"idle",idle->resolvedPath}};
    CombatSessionProfile player;player.initialIdle={"Idle",0,{0}};player.damageMarkerNames={"attack_mainhand"};
    OriginalAttackSelection attack;attack.state="AttackStatic";player.sequenceAction=attack;
    player.retainedPhaseClock=true;player.sourceCombo=true;player.propertyOptions={256,true};
    config.profiles.emplace("KnightPlayerBase",player);
    CombatSessionProfile enemy;enemy.action={"Attack",0,{0,1}};enemy.initialIdle={"Idle",0,{0,}};
    enemy.damageMarkerNames={"attack_mainhand"};enemy.propertyOptions={std::nullopt,true};
    enemy.customization.allow_missing_animation_targets=true;config.profiles.emplace("Swamp_LizadMan_Type1",enemy);
    ActorPopulation population;PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";
    placed.definition.stableId=2;placed.definition.sourceId="death-drop-pickup-session-v1";
    placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};
    placed.transform=placed.definition.placement;population.actors().push_back(std::move(placed));
    CharacterVisual player_visual;CombatSession session;
    check(session.initialize(assets,database,melee,config,player_visual,population,{0,0,0},customization,error),error);
    auto* world=session.world();check(world&&world->find_actor(1)&&world->find_actor(2),"CombatSession source actors absent");

    const auto& loot_tables=creation_source.loot_owner->borrow();
    const auto* source_victim_properties=world->combat_properties(2);
    check(source_victim_properties,"Source enemy original properties absent");
    const auto source_loot_id=source_victim_properties->sheets.resolved[9];
    const auto source_xp_raw=source_victim_properties->sheets.resolved[35];
    check(source_loot_id>=0&&std::size_t(source_loot_id)<loot_tables.loot_names().size(),
          "Source Swamp_LizadMan_Type1 profile has no authored Loot row");
    const auto& source_loot_name=loot_tables.loot_names()[std::size_t(source_loot_id)];
    check(!source_loot_name.empty()&&source_xp_raw>0,
          "Source enemy did not resolve actual property9 Loot row and property35 XP");
    auto* victim=session.actor(2);victim->health=1.0f;
    auto world_items=std::make_shared<RuntimeWorldItemAdapterV1>(loot_tables);
    Context context;context.store=world_items.get();context.characters[1]={1,player_state(*world->combat_properties(1))};
    context.characters[2]={1,{}};
    context.current_difficulty=0;context.unlocked_difficulty=0;
    context.rewards_suppressed=suppressed_mode;
    auto source_context_lease=std::make_shared<const std::uint8_t>(0);
    RuntimeSessionDeathRewardBindingsV1 host_bindings;
    host_bindings.gameplay_context_lease=source_context_lease;host_bindings.context=&context;
    host_bindings.read_reward_admission=Context::read_admission;
    host_bindings.read_difficulty=Context::read_difficulty;
    host_bindings.source_loot_entry=Context::source_entry;
    host_bindings.query_debug_switch=Context::query_debug;
    host_bindings.resolve_character=Context::resolve;
    RuntimeSessionDeathRewardsV1 death_reward_host;
    check(death_reward_host.bind(session,creation_source,loot_assets,world_items,
                                 host_bindings,error),error);

    InputActions input;input.attack=true;bool lethal_event_seen=false;
    for(unsigned frame=0;frame<1200&&!lethal_event_seen;++frame){
        const double dt=frame==0?0.0:0.016;
        check(session.update(dt,input,{0,0,0},session.actor(1)->transform.rotation[2],error),error);
        lethal_event_seen=std::any_of(session.events().begin(),session.events().end(),[](const DamageEvent& event){
            return event.applied&&event.target_died;
        });
    }
    check(lethal_event_seen&&!session.actor(2)->alive(),
          "Real CombatSession source attack did not emit a lethal DamageEvent");
    const auto xp_before=context.characters.at(1).character->experience;
    std::vector<RuntimeDeathRewardOutcomeV1> outcomes;
    const auto loot_before_consume=world->random_state();
    if(suppressed_mode){
        check(death_reward_host.after_update(session,outcomes,error),error);
        const auto rng_after_suppressed=world->random_state();
        check(outcomes.size()==1&&outcomes[0].state==RuntimeDeathRewardStateV1::completed&&
              outcomes[0].rewards_suppressed&&outcomes[0].spawned_items==0&&
              outcomes[0].selected_items==0&&outcomes[0].xp_recipients==0&&
              context.characters.at(1).character->experience==xp_before&&world_items->size()==0&&
              rng_after_suppressed.calls==loot_before_consume.calls,
              "Suppressed source Level gate changed XP/drop state or advanced Loot RNG");
        context.rewards_suppressed=false;
        check(death_reward_host.after_update(session,outcomes,error),error);
        const auto rng_after_unblocked_repeat=world->random_state();
        check(outcomes.size()==1&&outcomes[0].state==RuntimeDeathRewardStateV1::completed&&
              outcomes[0].rewards_suppressed&&outcomes[0].spawned_items==0&&
              outcomes[0].xp_recipients==0&&context.characters.at(1).character->experience==xp_before&&
              world_items->size()==0&&rng_after_unblocked_repeat.calls==loot_before_consume.calls&&
              context.admission_reads==2&&context.difficulty_reads==1,
              "Clearing the source Level gate replayed a previously suppressed death");
        std::cout<<"{\"validation\":\"PASS\",\"mode\":\"rewards_suppressed\",\"victim_definition\":\"Swamp_LizadMan_Type1\",\"source_level_gate\":true,\"suppressed_death_consumed_once\":true,\"unblock_no_replay\":true,\"xp_unchanged\":true,\"drops_unchanged\":true,\"loot_rng_unchanged\":true,\"admission_reads\":"<<context.admission_reads<<",\"difficulty_reads\":"<<context.difficulty_reads<<"}\n";
        return 0;
    }
    bool reward_ok=death_reward_host.after_update(session,outcomes,error);
    check(reward_ok,error);
    check(outcomes.size()==1&&outcomes[0].state==RuntimeDeathRewardStateV1::completed&&
          !outcomes[0].rewards_suppressed&&outcomes[0].spawned_items>0&&
          context.characters.at(1).character->experience>xp_before&&world_items->size()>0,
          "CombatSession death did not deliver source loot + XP into its shared world/store owners");
    check(context.admission_reads==1&&context.difficulty_reads==1&&context.one_kill_queries==1&&context.source_debug_requests>0,
          "Host did not query current difficulty and actual source Loot/XP Debug providers");
    const auto xp_after=context.characters.at(1).character->experience;
    const auto loot_after_consume=world->random_state();
    check(loot_after_consume.calls>loot_before_consume.calls,
          "RuntimeDeathRewards did not consume the caller's actual original Loot RNG");

    std::vector<RuntimeWorldItemRenderV1> drops;check(world_items->render_items(drops,error),error);
    const auto potion=std::find_if(drops.begin(),drops.end(),[&](const auto& item){
        return item.item_id==925&&item.item_identifier=="Potion0";
    });
    check(potion!=drops.end()&&potion->quantity==1,
          "Actual Barrel_Level_01 source selector did not publish its eligible Potion0 row");
    const auto potion_id=potion->identity;
    auto* live_player=session.actor(1);check(live_player,"Session player ActorState absent at pickup");
    PickupContext pickup_context{1,context.characters.at(1).character};
    RuntimeWorldItemInteractionServicesV1 pickup_services{&pickup_context,resolve_pickup,{}};
    RuntimeWorldItemSourceInteractV1 request{1,potion_id};RuntimeWorldItemInteractionV1 interaction;
    RuntimeWorldItemInteractionReceiptV1 pickup_receipt;
    check(interaction.dispatch(session,request,pickup_services,*world_items,pickup_receipt,error),error);
    check(pickup_receipt.pickup.completed&&pickup_receipt.player==1&&pickup_receipt.item==potion_id&&
          pickup_context.state->inventory.size()==1&&pickup_context.state->inventory.back().definition_id=="Potion0"&&
          pickup_context.state->inventory.back().quantity==1,
          "Same CombatSession source-eligible item was not transferred once to its shared CharacterState");
    const auto inventory_after=*pickup_context.state;
    check(!interaction.dispatch(session,request,pickup_services,*world_items,pickup_receipt,error)&&
          pickup_context.state->inventory.size()==inventory_after.inventory.size()&&
          pickup_context.state->experience==inventory_after.experience,
          "Duplicate source pickup changed inventory or XP");
    const auto gold_drop=std::find_if(drops.begin(),drops.end(),[&](const auto& item){
        return item.item_id==418&&item.item_identifier=="GoldStack01";
    });
    std::string drop_rows;
    for(const auto& item:drops)drop_rows+=(drop_rows.empty()?"":",")+std::to_string(item.item_id)+":"+item.item_identifier;
    check(gold_drop!=drops.end(),"Actual Swamp_LizadMan source outcome lacked GoldStack01; selected rows="+drop_rows);
    RuntimeWorldItemEntryV1 gold_entry;
    check(world_items->inspect(gold_drop->identity,gold_entry,error),error);
    check(gold_entry.source_outcome.resolved_gold_value&&*gold_entry.source_outcome.resolved_gold_value>0,
          "Source GoldStack item has no resolved AddLoot value");
    const auto gold_before_pickup=pickup_context.state->gold;
    RuntimeWorldItemSourceInteractV1 gold_request{1,gold_drop->identity};
    check(interaction.dispatch(session,gold_request,pickup_services,*world_items,pickup_receipt,error),error);
    const auto gold_after_pickup=pickup_context.state->gold;
    check(gold_after_pickup==gold_before_pickup+std::uint64_t(*gold_entry.source_outcome.resolved_gold_value),
          "Source GoldStack pickup did not credit the exact retained AddLoot value");
    const auto gold_inventory_after=*pickup_context.state;
    check(!interaction.dispatch(session,gold_request,pickup_services,*world_items,pickup_receipt,error)&&
          pickup_context.state->gold==gold_after_pickup&&
          pickup_context.state->inventory.size()==gold_inventory_after.inventory.size(),
          "Duplicate GoldStack pickup changed wallet or inventory");

    const auto saved_owner=pickup_context.state;
    const auto earned_experience=saved_owner->experience;
    const auto earned_gold=saved_owner->gold;
    const auto earned_inventory=saved_owner->inventory;
    const auto save_path=repo/".local-inputs"/("runtime-death-reward-save-"+
        std::to_string(diagnostic_seed)+".dhsave");
    std::error_code ignored;std::filesystem::remove(save_path,ignored);
    check(save_character(save_path,*saved_owner,error),error);
    saved_owner->experience=0;saved_owner->gold=0;saved_owner->inventory.clear();
    check(load_character(save_path,*saved_owner,error),error);
    check(saved_owner.get()==pickup_context.state.get()&&saved_owner->experience==earned_experience&&
          saved_owner->gold==earned_gold&&saved_owner->inventory.size()==earned_inventory.size()&&
          !earned_inventory.empty()&&saved_owner->inventory.back().instance_id==earned_inventory.back().instance_id&&
          saved_owner->inventory.back().definition_id=="Potion0"&&
          saved_owner->inventory.back().quantity==earned_inventory.back().quantity,
          "CharacterState SaveStore reload did not preserve earned XP, source gold, and Potion0 ownership");
    std::filesystem::remove(save_path,ignored);
    const auto rng_before_repeat=world->random_state();const auto spawn_before_repeat=context.characters.size();
    reward_ok=death_reward_host.after_update(session,outcomes,error);
    check(reward_ok,error);
    check(context.characters.at(1).character->experience==xp_after&&
          context.characters.at(1).character->gold==gold_after_pickup&&world_items->size()==drops.size()-2&&
          world->random_state().seed==rng_before_repeat.seed&&world->random_state().calls==rng_before_repeat.calls&&
          spawn_before_repeat==2,
          "Repeated CombatSession death receipt replayed XP, loot, or RNG after pickup");

    session.detach_for_restore();
    check(!death_reward_host.after_update(session,outcomes,error),
          "Death-reward host accepted an expired actor-binding lease during restore");
    check(session.rebind_after_restore(error),error);
    check(!death_reward_host.after_update(session,outcomes,error),
          "Death-reward host silently adopted a new actor-binding lease after restore");
    death_reward_host.reset();
    check(death_reward_host.bind(session,creation_source,loot_assets,world_items,host_bindings,error),error);
    check(death_reward_host.after_update(session,outcomes,error)&&outcomes.empty(),
          "Reset/rebind after restore replayed a cleared death batch");

    std::cout<<"{\"validation\":\"PASS\",\"death_event_source\":\"CombatSession::events\"," 
             <<"\"victim_definition\":\"Swamp_LizadMan_Type1\",\"loot_property9\":"<<source_loot_id
             <<",\"loot_table\":\""<<source_loot_name<<"\",\"xp_property35_raw\":"<<source_xp_raw<<','
             <<"\"session_rng_seed\":"<<diagnostic_seed<<','
             <<"\"source_item\":\"Potion0\","
             <<"\"death_xp_awarded\":true,\"item_pickup_once\":true,\"repeat_death_suppressed\":true,"
             <<"\"loot_rng_calls\":"<<world->random_state().calls<<",\"live_drops_after_pickup\":"<<world_items->size()<<','
             <<"\"host_bound\":true,\"source_debug_requests\":"<<context.source_debug_requests<<','
             <<"\"difficulty_reads\":"<<context.difficulty_reads<<",\"one_kill_queries\":"<<context.one_kill_queries<<','
             <<"\"initial_drop_count\":"<<drops.size()<<",\"gold_drop_item\":"<<gold_drop->item_id<<','
             <<"\"gold_value\":"<<*gold_entry.source_outcome.resolved_gold_value<<','
             <<"\"gold_earned\":"<<saved_owner->gold<<",\"save_reload_xp_gold_potion\":true,"
             <<"\"restore_old_lease_rejected\":true,\"restore_rebind_no_replay\":true}\n";
    return 0;
}catch(const std::exception& exception){std::cerr<<exception.what()<<'\n';return 1;}
