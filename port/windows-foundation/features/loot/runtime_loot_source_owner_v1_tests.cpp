#include "runtime_loot_source_owner_v1.hpp"
#include "../../playable_actor_world.hpp"
#include "../../original_combat_properties.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::loot;
using namespace dh::foundation::frontend::creation;

namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
}

int main(int argc,char** argv) try {
    check(argc==2,"Supply actual AssetCatalog root");
    AssetCatalog assets(argv[1]);
    dh2::data::LootRandom8V2 random{0x8badf00du,37};
    RuntimeCreationSourceOwnerV1 creation;std::string error;
    check(load_runtime_creation_source_v1(assets,random,creation,error),error);
    check(creation.valid(),"Existing RuntimeCreationSourceOwner did not load");

    RuntimeLootSourceOwnerV1 loot_owner;
    check(loot_owner.load(assets,creation,error),error);
    check(loot_owner.valid(),"Full original loot source owner is incomplete");
    const auto source=loot_owner.borrow();
    check(source.valid(),"RuntimeLootSourceV1 borrow is incomplete");
    check(source.properties.get()==creation.properties.get(),
          "Loot source replaced the RuntimeCreationSourceOwner property database");
    check(&source.loot.items()==&creation.source().loot.items()&&
          &source.loot.loots()==&creation.source().loot.loots()&&
          &source.loot.loot_names()==&creation.source().loot.loot_names(),
          "Loot source did not retain the exact six-table creation Loot snapshot");
    check(source.item_power_tables&&source.power_resources&&source.audiovisual&&source.design_settings,
          "Original power/audio/design source owners are absent");
    check(source.power_resources.quantities().size()>0&&
          source.power_resources.quantity_names().size()==source.power_resources.quantities().size(),
          "Actual LootTables suffix did not populate the original NumProbArray quantity owner");
    check(source.audiovisual.rows().size()==source.audiovisual.names().size()&&
          !source.audiovisual.rows().empty(),"Actual LootAudioVisual rows/names are incomplete");
    check(!source.design_settings.rows().empty()&&
          source.design_settings.word(0,0)!=nullptr,
          "Actual DesignSettings row projection is absent");
    check(source.source_merchant_rows>0,"V88 merchant suffix prefix was not bounded/decoded");

    std::int32_t normal_cap{},hard_cap{},very_hard_cap{};
    check(source.max_level_for_difficulty(0,normal_cap,error),error);
    check(source.max_level_for_difficulty(1,hard_cap,error),error);
    check(source.max_level_for_difficulty(2,very_hard_cap,error),error);
    check(normal_cap>0&&hard_cap>0&&very_hard_cap==100,
          "CharacterDesign MaxLevel constants do not match actual source caps");
    std::int32_t untouched=71;
    check(!source.max_level_for_difficulty(3,untouched,error)&&untouched==71&&!error.empty(),
          "Out-of-range difficulty did not fail without changing the output");
    check(random.seed==0x8badf00du&&random.calls==37,
          "RuntimeLootSourceOwner created/advanced a private RNG instead of preserving the caller owner");
    check(creation.source_hashes.size()==13&&loot_owner.source_hashes().size()==25,
          "Loot source hash receipts do not include the original prefix plus twelve actual extra assets");
    for(const auto& receipt:loot_owner.source_hashes())
        check(!receipt.asset_path.empty()&&receipt.byte_count>0&&receipt.sha256.size()==64,
              "Loot source hash receipt is incomplete");

    // Source PlayerManager class counts compare Character::InitPre's cached
    // CharacterTable base ID, not the selectable ClassTables class ID.
    check(creation.properties->characters.names.size()>325&&
          creation.properties->characters.names[263]=="KnightPlayerBase"&&
          creation.properties->characters.names[290]=="MagePlayerBase"&&
          creation.properties->characters.names[325]=="RoguePlayerBase",
          "Original CharacterTable base IDs do not match PlayerManager source constants");
    dh2::data::AiTables ai;check(load_original_ai_tables(assets,"original-cache/data/pydata",ai,error),error);
    dh2::data::CombatRandom world_random{0x1a2b3c4du,0};
    PlayableActorWorld world(std::move(ai),world_random);
    auto bind_player=[&](ActorId id,const char* base,const char* selectable_class){
        OriginalCombatProperties combat;
        check(build_original_combat_properties(*creation.properties,base,{256,true},{},{},combat,error),error);
        ActorState actor;actor.id=id;actor.definition_id=base;actor.class_id=selectable_class;
        actor.faction_id=combat.sheets.resolved[0];
        actor.health=original_signed256(combat.sheets.resolved[36]);
        actor.max_health=original_signed256(combat.sheets.resolved[38]);
        actor.resource=original_signed256(combat.sheets.resolved[41]);
        actor.max_resource=original_signed256(combat.sheets.resolved[43]);
        PlayableActorTraits traits;traits.is_player=true;
        check(world.bind_actor(std::move(actor),std::move(combat),traits,error),error);
    };
    bind_player(1,"KnightPlayerBase","KnightPlayerClass");
    bind_player(2,"MagePlayerBase","MagePlayerClass");
    bind_player(3,"RoguePlayerBase","RoguePlayerClass");
    std::int32_t warrior_count=-1,mage_count=-1,rogue_count=-1;
    check(source_player_class_count_v1(world,*creation.properties,dh2::data::LootEntryOperationV8::warrior_count,warrior_count,error),error);
    check(source_player_class_count_v1(world,*creation.properties,dh2::data::LootEntryOperationV8::mage_count,mage_count,error),error);
    check(source_player_class_count_v1(world,*creation.properties,dh2::data::LootEntryOperationV8::rogue_count,rogue_count,error),error);
    check(warrior_count==1&&mage_count==1&&rogue_count==1,
          "Loot class counts did not use the same-world source CharacterTable base IDs");
    auto unclassified=OriginalCombatProperties{};
    ActorState bad_player;bad_player.id=4;bad_player.definition_id="KnightPlayerClass";
    bad_player.class_id="KnightPlayerBase";bad_player.health=bad_player.max_health=1.0f;
    bad_player.faction_id=unclassified.sheets.resolved[0];
    PlayableActorTraits bad_traits;bad_traits.is_player=true;
    check(world.bind_actor(std::move(bad_player),std::move(unclassified),bad_traits,error),error);
    std::int32_t unchanged_count=71;
    check(!source_player_class_count_v1(world,*creation.properties,dh2::data::LootEntryOperationV8::warrior_count,unchanged_count,error)&&
          unchanged_count==71&&!error.empty(),
          "Unknown player CharacterTable source identity was counted or changed output");
    check(world_random.seed==0x1a2b3c4du&&world_random.calls==0,
          "Pure source player class-count query advanced gameplay RNG");

    // A failed re-load must preserve the complete prior snapshot and exact
    // creation-source identity.
    const auto prior=loot_owner.borrow();const auto prior_hash_count=loot_owner.source_hashes().size();
    const auto scratch=std::filesystem::temp_directory_path()/(
        "dh-runtime-loot-empty-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::filesystem::create_directories(scratch);AssetCatalog empty_assets(scratch);
    check(!loot_owner.load(empty_assets,creation,error)&&!error.empty(),
          "Missing source loot assets unexpectedly replaced the live snapshot");
    check(loot_owner.valid()&&loot_owner.source_hashes().size()==prior_hash_count&&
          loot_owner.borrow().properties.get()==prior.properties.get()&&
          &loot_owner.borrow().loot.items()==&prior.loot.items()&&
          loot_owner.borrow().character_max_level==prior.character_max_level,
          "Failed loot source reload changed prior pinned owners/borrows");
    std::error_code cleanup;std::filesystem::remove(scratch,cleanup);
    check(!cleanup,"Temporary empty asset root cleanup failed");
    std::cout<<"{\"validation\":\"PASS\",\"creation_hashes\":"<<creation.source_hashes.size()
             <<",\"loot_source_hashes\":"<<loot_owner.source_hashes().size()
             <<",\"merchant_rows\":"<<source.source_merchant_rows
             <<",\"quantity_tables\":"<<source.power_resources.quantities().size()
             <<",\"item_power_rows\":"<<source.item_power_tables.rows().size()
             <<",\"audiovisual_rows\":"<<source.audiovisual.rows().size()
             <<",\"design_rows\":"<<source.design_settings.rows().size()
             <<",\"max_level_caps\":["<<normal_cap<<','<<hard_cap<<','<<very_hard_cap<<"]"
             <<",\"player_base_rows\":[263,290,325],\"player_class_counts\":["
             <<warrior_count<<','<<mage_count<<','<<rogue_count<<"]"
             <<",\"caller_rng_unchanged\":true,\"reload_failure_atomic\":true}\n";
    return 0;
} catch(const std::exception& exception){std::cerr<<exception.what()<<'\n';return 1;}
