// Include after renderer_canonical_world_v4.inc and V49 typed headers.
// Metadata owns ONLY missing-field sibling producers; actual player remains
// the existing ScriptCharacterObject/Save/World/Gear/FSM.
static void prepare_player_source_fields_v49(const std::shared_ptr<WorldScriptContext>& world){
 std::string e;
 if(!world||!world->player_object||!world->player_canonical)
  throw std::runtime_error("Required SAME player before missing source fields");
 auto& meta=*world->player_canonical;const auto id=world->player_object->identity;
 if(!meta.loot_fields_v49)meta.loot_fields_v49=std::make_shared<dh2::character::LootPlayerFieldAssociationV47>(id);
 if(meta.loot_fields_v49->character()!=id)throw std::runtime_error("Player missing-field authority changed receiver");
 // Actual C1 manager/properties facade, reused later by canonical publication.
 if(!world->canonical_world)world->canonical_world=std::make_shared<RendererCanonicalWorldV4>(*world);
 if(!meta.source_visibility_v49)meta.source_visibility_v49=
  std::make_unique<dh2::character::PlayerSourceVisibilityV49>(
   meta.loot_fields_v49,world->player_object,meta.class_name20,&meta.source_static84);
 if(!meta.source_visibility_v49->publish_missing_default(world->canonical_world->properties,e))
  throw std::runtime_error(e);
}
static void publish_player_source_save_v49(PlayerSkillsRuntime& t){
 if(!t.world||!t.world->player_canonical||!t.world->player_object||!t.save)
  throw std::runtime_error("Required SAME executed Save C1/SetCharacter prefix");
 auto& meta=*t.world->player_canonical;
 if(!meta.loot_fields_v49)throw std::runtime_error("Required prepared player source14e8 authority");
 std::string e;if(!meta.loot_fields_v49->source_save_store_3b36d8(t.save,e))throw std::runtime_error(e);
}
template<class SameRootContracts> static bool populate_loot_missing_contracts_v49(PlayerSkillsRuntime& t,
 SameRootContracts& out,std::string& e){
 if(!t.world||!t.world->player_canonical||!t.world->player_canonical->loot_fields_v49||!player_equipment){
  e="Required SAME prepared player fields/Gear for loot";return false;
 }
 out.player_fields=t.world->player_canonical->loot_fields_v49;
 dh2::data::LootTablesV2::Borrow tables;dh2::data::ItemPowerTablesV5::Borrow powers;
 dh2::data::LootRandom8V2* rng{};
 // Real existing inventory's ItemTextOwner/formatter, no second text cache.
 return player_equipment->loot_sources_v8(tables,powers,out.item_text,rng,e);
}
