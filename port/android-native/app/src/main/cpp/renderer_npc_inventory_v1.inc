// Include npc_inventory_owner_v1.hpp. Retain exactly one owner per NPC. The
// supplied LootTables borrow is the immutable SAME world/player-equipment
// cache lease, not a newly parsed second table or mutable player inventory.
struct RendererNpcInventoryV1 {
 MonsterScriptHandle& npc;
 dh2::character::NpcInventoryOwnerV1 owner;
 RendererNpcInventoryV1(MonsterScriptHandle& m,dh2::data::LootTablesV2::Borrow tables):
  npc(m),owner(m.object->identity,std::move(tables),equipment_random,m.object->properties){
  if(!m.session||m.session->properties()!=m.object->properties)
   throw std::runtime_error("NPC inventory requires same live Character property owner");
 }
 dh2::character::skills::WorldAttackInventoryBorrowV1 attack_borrow()const noexcept{return owner.attack_borrow();}
};
