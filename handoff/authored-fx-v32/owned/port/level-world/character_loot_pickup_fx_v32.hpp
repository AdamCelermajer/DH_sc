#pragma once
#include "character_mesh_fx_owner_v4.hpp"
#include "character_loot_actor_binding_v31.hpp"
#include "world_loot_gameplay_v23.hpp"
namespace dh2::character {
// Source Item.Interact's function-static Effects set-name lookup. Retain ONE
// receiver per Application/pickup callsite, alongside the same FX manager.
// No resource, emitter, mesh, clock or shader owner is created here.
class CharacterLootPickupFxV32 {
 fx::CharacterMeshFxOwnerV4& fx_;data::EffectsTables::Borrow tables_;
 CharacterLootActorBindingV31& actors_;WorldItemLiveOwnerV5& items_;
 bool lookup_initialized_{};std::int32_t cached_set_{-1};
public:
 CharacterLootPickupFxV32(fx::CharacterMeshFxOwnerV4& fx,data::EffectsTables::Borrow tables,
  CharacterLootActorBindingV31& actors,WorldItemLiveOwnerV5& items)
  :fx_(fx),tables_(std::move(tables)),actors_(actors),items_(items){}
 bool route(const LootInteractRequestV8&,LootInteractResponseV8&,bool& handled,std::string&);
 std::int32_t cached_set()const noexcept{return cached_set_;}
 bool lookup_initialized()const noexcept{return lookup_initialized_;}
};
}
