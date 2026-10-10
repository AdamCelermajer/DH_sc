#include "runtime_death_rewards_v1.hpp"
#include "runtime_world_item_adapter_v1.hpp"
#include "../../playable_actor_world.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_actor_properties.hpp"
#include "../../save_store.hpp"
#include "../../../game-data/loot_table_selection_v8.hpp"
#include "../../../game-data/loot_item_selection_v8.hpp"
#include "../../../game-data/loot_power_creation_v7.hpp"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::loot;
using namespace dh2::data;

namespace {
void check(bool value, const std::string& message) { if (!value) throw std::runtime_error(message); }
std::int32_t asr8(std::int32_t value) { return value >> 8; }
Bytes view(const std::vector<std::uint8_t>& bytes) { return {bytes.data(), bytes.size()}; }
struct Context {
    std::map<ActorId, RuntimeDeathActorV1> actors;
    RuntimeWorldItemAdapterV1* store{};
    unsigned spawned{};
    bool fail_spawn{};
    static bool resolve(void* p, ActorId id, RuntimeDeathActorV1& out, std::string& error) {
        auto& self=*static_cast<Context*>(p);auto found=self.actors.find(id);
        if(found==self.actors.end()){error="missing actual test actor binding";return false;}
        out=found->second;return true;
    }
    static bool spawn(void* p,const RuntimeWorldItemRecordV1& item,const ActorState& source,
                      const ActorState*,std::string& error) {
        auto& self=*static_cast<Context*>(p);
        check(item.source_actor==source.id&&item.item_id>=0&&item.quantity>0&&item.authored_item&&item.authored_entry,
              "spawn receiver did not get a real Loot/Item table descriptor");
        ++self.spawned;
        if(self.fail_spawn){error="explicit reached world spawn failure";return false;}
        if(self.store){
            RuntimeWorldItemIdV1 identity{};
            if(!self.store->publish_death_drop(item,source,identity,error))return false;
        }
        return true;
    }
};
bool entry(void*,const LootEntryRequestV8& request,std::int32_t& value,std::string&) {
    switch(request.operation){
    case LootEntryOperationV8::debug_load: case LootEntryOperationV8::debug_query: value=0;return true;
    case LootEntryOperationV8::mage_count: case LootEntryOperationV8::rogue_count: value=0;return true;
    case LootEntryOperationV8::warrior_count: value=1;return true;
    case LootEntryOperationV8::assertion: value=0;return true;
    }
    return false;
}
struct Profile { std::shared_ptr<CharacterState> state; };
std::shared_ptr<CharacterState> make_profile(const std::string& id,const std::string& name,
                                              const OriginalCombatProperties& properties) {
    auto state=std::make_shared<CharacterState>(make_default_character(id,name,"warrior"));
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

int main(int argc,char** argv){try{
    check(argc==4,"supply original shared asset root, original loot-power cache, and design cache");AssetCatalog assets(argv[1]);std::string error;
    OriginalPropertyDatabase database;check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
    AiTables ai;check(load_original_ai_tables(assets,"original-cache/data/pydata",ai,error),error);
    auto read=[&](const char* name){return assets.read(std::filesystem::path("original-cache/data/pydata")/name);};
    auto item_records=read("loot_table_pyarray.bin"),item_names=read("loot_table_pyarraynames.bin"),item_schema=read("loot_table_pystructnames.bin");
    const std::string power_cache=argv[2];
    auto power_file=[&](const char* name){std::ifstream f(std::filesystem::path(power_cache)/name,std::ios::binary);check(bool(f),"missing original loot-power cache file");return std::vector<std::uint8_t>{std::istreambuf_iterator<char>(f),{}};};
    std::array<std::vector<std::uint8_t>,9> power_raw;
    const char* power_names[]={"item_powers_pyarray.bin","item_powers_pyarraynames.bin","item_powers_pystructnames.bin","item_powers_monopoly_pyarray.bin","item_powers_monopoly_pyarraynames.bin","item_powers_monopoly_pystructnames.bin","num_prob_records_v7.bin","loot_table_pyarraynames.bin","loot_table_pystructnames.bin"};
    for(unsigned i=0;i<9;++i)power_raw[i]=power_file(power_names[i]);
    LootPowerInputsV7 power_input{view(power_raw[0]),view(power_raw[1]),view(power_raw[2]),view(power_raw[3]),view(power_raw[4]),view(power_raw[5]),view(power_raw[6]),view(power_raw[7]),view(power_raw[8])};
    ItemPowerTablesV5 power_definitions;check(power_definitions.load(power_input.powers,power_input.power_names,power_input.power_schema,error),error);
    LootPowerResourcesV7 powers;check(powers.load(power_input,power_definitions.borrow(),error),error);
    LootTablesV2 loot_owner;check(loot_owner.load(view(item_records),view(item_names),view(item_schema),error),error);
    auto read_design=[&](const char* name){std::ifstream f(std::filesystem::path(argv[3])/name,std::ios::binary);check(bool(f),"missing original design-settings cache file");return std::vector<std::uint8_t>{std::istreambuf_iterator<char>(f),{}};};
    auto design_records=read_design("design_pyarray.bin"),design_names=read_design("design_pyarraynames.bin"),design_schema=read_design("design_pystructnames.bin");
    DesignSettingsOwner design_owner;check(design_owner.load(view(design_records),view(design_names),view(design_schema),error),error);
    const auto design=design_owner.borrow().rows().front();
    OriginalCombatFacts player_facts{},enemy_facts{};
    OriginalCombatProperties player_props,enemy_props;
    check(build_original_combat_properties(database,"RoguePlayerBase",{256,true},{},player_facts,player_props,error),error);
    check(build_original_combat_properties(database,"Swamp_LizadMan_Type1",{std::nullopt,true},{},enemy_facts,enemy_props,error),error);
    const auto barrel = std::find(loot_owner.borrow().loot_names().begin(),
                                  loot_owner.borrow().loot_names().end(), "Barrel_Level_01");
    check(barrel != loot_owner.borrow().loot_names().end(), "actual Barrel_Level_01 row unavailable");
    const auto barrel_id = static_cast<std::int32_t>(std::distance(loot_owner.borrow().loot_names().begin(),barrel));
    // Keep real source character rows and real source table snapshots, while
    // choosing the authored gold-bearing Loot row for deterministic bonus tests.
    enemy_props.sheets.resolved[9]=barrel_id;
    player_props.sheets.resolved[195]=50*256;
    dh2::data::CombatRandom combat_random{0x12345678u,0};PlayableActorWorld world(ai,combat_random);
    auto bind=[&](ActorId id,const char* row,OriginalCombatProperties props,bool player){
        ActorState actor;actor.id=id;actor.definition_id=row;actor.class_id="warrior";actor.faction_id=props.sheets.resolved[0];
        actor.health=original_signed256(props.sheets.resolved[36]);actor.max_health=original_signed256(props.sheets.resolved[38]);
        actor.resource=original_signed256(props.sheets.resolved[41]);actor.max_resource=original_signed256(props.sheets.resolved[43]);
        actor.transform.position={float(id-1),0.f,0.f};
        PlayableActorTraits traits;traits.is_player=player;
        check(world.bind_actor(actor,std::move(props),traits,error),error);
    };
    bind(1,"RoguePlayerBase",player_props,true);bind(2,"Swamp_LizadMan_Type1",enemy_props,false);
    RuntimeWorldItemAdapterV1 world_items(loot_owner.borrow());
    Context context;context.store=&world_items;context.actors[1]={1,make_profile("player","Player",player_props)};context.actors[2]={1,{}};
    auto* live_rogue=world.find_actor(1);
    check(live_rogue&&live_rogue->health==original_signed256(player_props.sheets.resolved[36]),
          "Fresh Rogue fixture did not start from its source-refilled HP");
    constexpr float damaged_rogue_hp=81.0f;
    check(damaged_rogue_hp>0.0f&&damaged_rogue_hp<live_rogue->health&&
          apply_actor_damage(*live_rogue,live_rogue->health-damaged_rogue_hp)>0.0f&&
          live_rogue->health==damaged_rogue_hp,
          "Same-world Rogue damage fixture did not reach 81 HP");
    constexpr float spent_rogue_mp=12.25f;
    check(spent_rogue_mp>=0.0f&&spent_rogue_mp<live_rogue->resource,
          "Same-world Rogue MP fixture is outside source current/max range");
    live_rogue->resource=spent_rogue_mp;
    check(context.actors[1].character->stats.health>damaged_rogue_hp&&
          world.combat_properties(1)->sheets.resolved[36]==player_props.sheets.resolved[36],
          "Stale source HP versus live ActorState setup was not reproduced");
    const auto assert_live_vitals=[&](float hp,float mp,const char* message){
        const auto* actor=world.find_actor(1);const auto* source=world.combat_properties(1);
        check(actor&&source&&actor->health==hp&&actor->resource==mp&&
              source->sheets.resolved[36]==std::int32_t(std::lround(double(hp)*256.0))&&
              source->sheets.resolved[41]==std::int32_t(std::lround(double(mp)*256.0))&&
              context.actors[1].character->stats.health==original_signed256(source->sheets.resolved[36])&&
              context.actors[1].character->stats.resource==original_signed256(source->sheets.resolved[41]),
              message);
    };
    const auto* enemy_live=world.combat_properties(2);check(enemy_live,"enemy properties absent");
    const auto loot_id=enemy_live->sheets.resolved[9];
    check(loot_id>=-1&&(loot_id<0||std::size_t(loot_id)<loot_owner.borrow().loots().size()),"actual enemy loot property outside cache");
    std::uint32_t rng_before=0;
    std::uint32_t chosen_seed=1;
    if(loot_id>=0){
        for(;chosen_seed<10000;++chosen_seed){
            LootRandom8V2 probe{chosen_seed,0};LootTableSelectionV8 table(loot_owner.borrow(),powers.borrow(),probe,{nullptr,entry});
            std::vector<const LootEntry32V2*> entries;std::vector<LootItemInfoV8> items;
            if(!table.select(loot_id,entries,error))continue;
            LootItemSelectionV8 expand(loot_owner.borrow(),probe,{nullptr,entry});
            if(expand.expand(entries,false,items,error)&&
               std::any_of(items.begin(),items.end(),[](const auto& item){return item.item&&item.item->record.words[22]==13;}))break;
        }
        check(chosen_seed<10000,"actual authored NPC table did not produce any item under tested shared-stream seeds");
    }
    LootRandom8V2 loot_rng{chosen_seed,0};
    RuntimeDeathRewardServicesV1 services;services.loot_tables=loot_owner.borrow();services.loot_powers=powers.borrow();services.loot_entry={nullptr,entry};
    services.gameplay_rng=&loot_rng;services.xp_design=&design;services.properties=&database;
    services.current_difficulty=0;services.unlocked_difficulty=0;services.max_level=100;services.context=&context;
    services.resolve_character=Context::resolve;services.spawn_world_item=Context::spawn;
    RuntimeDeathRewardsV1 runtime;std::vector<RuntimeDeathRewardOutcomeV1> results;
    DamageEvent death;death.applied=true;death.attacker=1;death.target=2;death.health_removed=world.find_actor(2)->health;
    death.target_died=apply_actor_damage(*world.find_actor(2),death.health_removed)>0;
    check(death.target_died&&!world.find_actor(2)->alive(),"test death did not use shared ActorState damage function");
    const auto xp_before=context.actors[1].character->experience;
    rng_before=loot_rng.calls;
    check(runtime.consume_events(world,{death},services,results,error),error);
    check(results.size()==1&&results[0].state==RuntimeDeathRewardStateV1::completed&&results[0].xp_recipients==1,
          "actual source death did not complete one recipient XP route");
    check(context.actors[1].character->experience>xp_before,"original source XP formulas did not update canonical CharacterState");
    check(context.actors[1].character->stats.level==1&&
          context.actors[1].character->stats.health==damaged_rogue_hp&&
          world.find_actor(1)->health==damaged_rogue_hp,
          "Non-level XP sync replaced live Rogue damage with stale source-sheet HP");
    assert_live_vitals(damaged_rogue_hp,spent_rogue_mp,
          "First ordinary XP did not synchronize live HP/MP into the published source sheet");
    check(loot_rng.calls>=rng_before,"original loot kernel did not use caller gameplay RNG");
    check(results[0].gold_bonus_source==RuntimeGoldValueBonusSourceV1::same_world_killer_property195&&
          results[0].diagnostic==RuntimeDeathRewardDiagnosticV1::none,
          "gold value did not record its same-world killer property195 source");
    RuntimeWorldItemIdV1 first_gold_id{};
    std::vector<RuntimeWorldItemRenderV1> live_drops;
    check(world_items.render_items(live_drops,error),error);
    for(const auto& rendered:live_drops){
        RuntimeWorldItemEntryV1 item;check(world_items.inspect(rendered.identity,item,error),error);
        if(item.authored_item->record.words[22]==13){
            check(item.source_outcome.resolved_gold_value.has_value(),"property-backed gold value was not retained");
            first_gold_id=rendered.identity;break;
        }
    }
    check(first_gold_id!=invalid_runtime_world_item_v1,"actual Barrel_Level_01 gold was not published");
    // Match source creation's ordering: selection first, then CalcLootItemValue
    // with the SAME stream and killer word195. Pickup must consume no RNG.
    LootRandom8V2 expected_rng{chosen_seed,0};
    LootTableSelectionV8 expected_table(loot_owner.borrow(),powers.borrow(),expected_rng,{nullptr,entry});
    std::vector<const LootEntry32V2*> expected_entries;
    check(expected_table.select(barrel_id,expected_entries,error),error);
    LootItemSelectionV8 expected_expand(loot_owner.borrow(),expected_rng,{nullptr,entry});
    std::vector<LootItemInfoV8> expected_items;
    check(expected_expand.expand(expected_entries,false,expected_items,error),error);
    std::vector<std::int32_t> expected_gold_values;
    for(const auto& item:expected_items)if(item.item&&item.item->record.words[22]==13){
        std::int32_t value{};
        check(dh2_loot_item_value_v7(&value,&expected_rng,&item.item->record,nullptr,0,
                                     player_props.sheets.resolved[195])==0,
              "source gold-value expectation failed");
        expected_gold_values.push_back(value);
    }
    RuntimeWorldItemEntryV1 first_gold;check(world_items.inspect(first_gold_id,first_gold,error),error);
    check(!expected_gold_values.empty()&&first_gold.source_outcome.resolved_gold_value==expected_gold_values.front()&&
          loot_rng.seed==expected_rng.seed&&loot_rng.calls==expected_rng.calls,
          "death reward value differs from source post-selection RNG/property195 order");
    auto pickup_character=make_default_character("loot-pickup","Loot Pickup","warrior");
    RuntimeWorldItemPickupReceiptV1 pickup;
    const auto rng_before_pickup=loot_rng;
    check(world_items.pickup(first_gold_id,pickup_character,pickup,error),error);
    check(pickup.completed&&pickup_character.gold==std::uint64_t(expected_gold_values.front())&&
          loot_rng.seed==rng_before_pickup.seed&&loot_rng.calls==rng_before_pickup.calls,
          "property-backed gold generation-to-pickup changed value or consumed pickup RNG");
    const auto xp_after=context.actors[1].character->experience;const auto spawned=context.spawned;const auto calls=loot_rng.calls;
    check(runtime.consume_events(world,{death},services,results,error),error);
    check(results.size()==1&&results[0].state==RuntimeDeathRewardStateV1::completed&&
          context.actors[1].character->experience==xp_after&&context.spawned==spawned&&loot_rng.calls==calls&&
          world.find_actor(1)->health==damaged_rogue_hp&&world.find_actor(1)->resource==spent_rogue_mp&&
          world.combat_properties(1)->sheets.resolved[36]==std::int32_t(std::lround(double(damaged_rogue_hp)*256.0))&&
          world.combat_properties(1)->sheets.resolved[41]==std::int32_t(std::lround(double(spent_rogue_mp)*256.0)),
          "duplicate death redelivered loot, RNG or XP");
    bind(3,"Swamp_LizadMan_Type1",enemy_props,false);
    context.actors[3]={1,{}};
    constexpr float second_ordinary_hp=70.0f,second_ordinary_mp=5.25f;
    auto* second_live_rogue=world.find_actor(1);
    check(second_live_rogue&&second_live_rogue->health>second_ordinary_hp&&
          apply_actor_damage(*second_live_rogue,second_live_rogue->health-second_ordinary_hp)>0.0f,
          "Second ordinary XP fixture did not apply same-world Rogue damage");
    second_live_rogue->resource=second_ordinary_mp;
    DamageEvent no_killer_death;no_killer_death.applied=true;no_killer_death.attacker=invalid_actor_id;
    no_killer_death.target=3;no_killer_death.health_removed=world.find_actor(3)->health;
    no_killer_death.target_died=apply_actor_damage(*world.find_actor(3),no_killer_death.health_removed)>0;
    check(no_killer_death.target_died,"zero-bonus fixture did not apply a genuine ActorState death");
    check(world_items.render_items(live_drops,error),error);
    RuntimeWorldItemIdV1 previous_last_id{};
    for(const auto& rendered:live_drops)previous_last_id=std::max(previous_last_id,rendered.identity);
    loot_rng={chosen_seed,0};
    RuntimeDeathRewardsV1 zero_bonus_runtime;
    check(zero_bonus_runtime.consume_events(world,{no_killer_death},services,results,error),error);
    check(results.size()==1&&results[0].gold_bonus_source==RuntimeGoldValueBonusSourceV1::absent_killer_zero_baseline&&
          results[0].diagnostic==RuntimeDeathRewardDiagnosticV1::none,
          "absent/deleted killer did not use the verified source 0/0 bonus baseline");
    LootRandom8V2 zero_expected_rng{chosen_seed,0};
    LootTableSelectionV8 zero_table(loot_owner.borrow(),powers.borrow(),zero_expected_rng,{nullptr,entry});
    std::vector<const LootEntry32V2*> zero_entries;
    check(zero_table.select(barrel_id,zero_entries,error),error);
    LootItemSelectionV8 zero_expand(loot_owner.borrow(),zero_expected_rng,{nullptr,entry});
    std::vector<LootItemInfoV8> zero_items;
    check(zero_expand.expand(zero_entries,false,zero_items,error),error);
    std::vector<std::int32_t> zero_gold_values;
    for(const auto& item:zero_items)if(item.item&&item.item->record.words[22]==13){
        std::int32_t value{};
        check(dh2_loot_item_value_v7(&value,&zero_expected_rng,&item.item->record,nullptr,0,0)==0,
              "zero-baseline source gold-value expectation failed");
        zero_gold_values.push_back(value);
    }
    check(!zero_gold_values.empty()&&loot_rng.seed==zero_expected_rng.seed&&loot_rng.calls==zero_expected_rng.calls,
          "absent-killer gold path did not preserve source zero-bonus RNG order");
    assert_live_vitals(second_ordinary_hp,second_ordinary_mp,
          "Second ordinary XP changed synchronized source/live Rogue vitals");
    RuntimeWorldItemIdV1 zero_gold_id{};
    check(world_items.render_items(live_drops,error),error);
    for(const auto& rendered:live_drops){
        if(rendered.identity<=previous_last_id)continue;
        RuntimeWorldItemEntryV1 item;check(world_items.inspect(rendered.identity,item,error),error);
        if(item.authored_item->record.words[22]==13){
            check(item.source_outcome.resolved_gold_value==zero_gold_values.front(),
                  "absent-killer drop did not use source zero-bonus value");zero_gold_id=rendered.identity;break;
        }
    }
    check(zero_gold_id!=invalid_runtime_world_item_v1,"zero-bonus source gold item was not published");
    auto zero_pickup=make_default_character("zero-loot-pickup","Zero Loot Pickup","warrior");
    RuntimeWorldItemPickupReceiptV1 zero_pickup_receipt;const auto zero_rng_before_pickup=loot_rng;
    check(world_items.pickup(zero_gold_id,zero_pickup,zero_pickup_receipt,error),error);
    check(zero_pickup.gold==std::uint64_t(zero_gold_values.front())&&loot_rng.seed==zero_rng_before_pickup.seed&&
          loot_rng.calls==zero_rng_before_pickup.calls,"zero-baseline gold pickup rerolled or changed source value");

    // Force the next *actual source XP award* to cross the live source
    // threshold. Damage/spend again so the level-up must first synchronize
    // current HP/MP, then source LevelUp must refill both from its new maxima.
    auto* live_rogue_after_xp=world.find_actor(1);
    check(live_rogue_after_xp&&live_rogue_after_xp->health>61.0f&&live_rogue_after_xp->resource>4.5f,
          "Rogue level-up fixture cannot apply a second vital loss");
    live_rogue_after_xp->health=61.0f;live_rogue_after_xp->resource=4.5f;
    auto levelup_source=*world.combat_properties(1);
    const auto threshold_raw=levelup_source.sheets.resolved[34];
    check(threshold_raw>0,"Actual source Rogue XP threshold is unavailable");
    PropertyRules rules;check(load_property_rules(database.characters,rules,error),error);
    auto levelup_view=property_view(rules,levelup_source.sheets);
    const auto xp_before_threshold=threshold_raw-1;
    check(!dh2_property_set(&levelup_view,33,xp_before_threshold),
          "Could not stage source XP immediately below its authored threshold");
    check(world.update_combat_properties(1,levelup_source,*world.traits(1),error),error);
    context.actors[1].character->experience=std::uint64_t(xp_before_threshold>>8);
    bind(4,"Swamp_LizadMan_Type1",enemy_props,false);context.actors[4]={1,{}};
    LootRandom8V2 levelup_rng{chosen_seed,0};services.gameplay_rng=&levelup_rng;
    DamageEvent levelup_death;levelup_death.applied=true;levelup_death.attacker=1;levelup_death.target=4;
    levelup_death.health_removed=world.find_actor(4)->health;
    levelup_death.target_died=apply_actor_damage(*world.find_actor(4),levelup_death.health_removed)>0;
    check(levelup_death.target_died,"Level-up source enemy death did not use shared ActorState damage");
    RuntimeDeathRewardsV1 levelup_runtime;
    check(levelup_runtime.consume_events(world,{levelup_death},services,results,error),error);
    const auto* leveled_source=world.combat_properties(1);const auto* leveled_actor=world.find_actor(1);
    check(leveled_source&&leveled_actor&&context.actors[1].character->stats.level==2&&
          asr8(leveled_source->sheets.resolved[19])==2&&
          leveled_source->sheets.resolved[36]==leveled_source->sheets.resolved[38]&&
          leveled_source->sheets.resolved[41]==leveled_source->sheets.resolved[43]&&
          leveled_actor->health==original_signed256(leveled_source->sheets.resolved[36])&&
          leveled_actor->resource==original_signed256(leveled_source->sheets.resolved[41])&&
          context.actors[1].character->stats.health==leveled_actor->health&&
          context.actors[1].character->stats.resource==leveled_actor->resource,
          "Actual source XP level-up failed to refill and publish same-world HP/MP");
    const auto save_path=std::filesystem::temp_directory_path()/"dh2-runtime-death-rewards-v1-xp-vitals.dhsave";
    std::error_code ignored;std::filesystem::remove(save_path,ignored);
    check(save_character(save_path,*context.actors[1].character,error),error);
    CharacterState loaded_after_levelup;
    check(load_character(save_path,loaded_after_levelup,error),error);
    std::filesystem::remove(save_path,ignored);
    check(loaded_after_levelup.stats.level==context.actors[1].character->stats.level&&
          loaded_after_levelup.experience==context.actors[1].character->experience&&
          loaded_after_levelup.source_stat_points==context.actors[1].character->source_stat_points&&
          loaded_after_levelup.source_skill_points==context.actors[1].character->source_skill_points&&
          loaded_after_levelup.stats.health==context.actors[1].character->stats.health&&
          loaded_after_levelup.stats.resource==context.actors[1].character->stats.resource,
          "Strict SaveStore reload lost source XP/points/refilled vitals");
    runtime.clear();loot_rng={chosen_seed,0};services.gameplay_rng=&loot_rng;
    Context bad=context;bad.actors[2].binding_lifecycle=2;bad.fail_spawn=true;
    services.context=&bad;RuntimeDeathRewardsV1 failed;
    check(!failed.consume_events(world,{death},services,results,error),"reached world spawn failure was ignored");
    const auto failed_spawns=bad.spawned;
    check(!failed.consume_events(world,{death},services,results,error)&&bad.spawned==failed_spawns,
          "failed death prefix allowed duplicate retry");
    std::cout<<"{\"validation\":\"PASS\",\"player_definition\":\"RoguePlayerBase\",\"live_hp_before_xp\":"<<damaged_rogue_hp
             <<",\"stale_source_hp\":"<<original_signed256(player_props.sheets.resolved[36])
             <<",\"live_mp_before_xp\":"<<spent_rogue_mp<<",\"nonlevel_xp_preserved_live_vitals\":true"
             <<",\"second_ordinary_hp\":"<<second_ordinary_hp<<",\"second_ordinary_mp\":"<<second_ordinary_mp
             <<",\"candidate_source_vitals_synced\":true,\"levelup_source_refill\":true,\"xp_save_reload\":true"
             <<",\"actual_loot_table\":"<<loot_id
             <<",\"killer_property195\":"<<player_props.sheets.resolved[195]
             <<",\"gold_bonus_source\":\"same_world_property195\",\"gold_value\":"<<expected_gold_values.front()
             <<",\"zero_bonus_baseline\":\"absent_killer_0_0\",\"zero_bonus_gold_value\":"<<zero_gold_values.front()
             <<",\"loot_rng_calls\":"<<calls<<",\"world_items\":"<<spawned
             <<",\"xp_recipients\":1,\"duplicate_death_suppressed\":true,\"failure_prefix_retained\":true}\n";
    return 0;
}catch(const std::exception& exception){std::cerr<<exception.what()<<'\n';return 1;}}
