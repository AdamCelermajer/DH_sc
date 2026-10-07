#pragma once
#include "world_item_live_owner_v5.hpp"
#include "character_target_bindings.hpp"
#include "../game-data/ai.hpp"
namespace dh2::character {
// Extends the SAME CharAI target service only for genuine canonical pooled
// Item receivers; no Character registration or duplicate target authority.
class LootItemTargetSourceV47 {
 WorldItemLiveOwnerV5& items_;world::CanonicalObjectManagerV1& manager_;
 TargetServices16 preceding_;const data::AiTables& ai_;data::PropertyView& owner_properties_;
 void* context_{};bool(*owner_position_)(void*,std::uintptr_t,const float*&,std::string&){};
 std::string error_;
 static int invoke(void*,TargetState48*,const TargetRequest24*,std::uint32_t*);
public:
 LootItemTargetSourceV47(WorldItemLiveOwnerV5& items,world::CanonicalObjectManagerV1& manager,
  TargetServices16 preceding,const data::AiTables& ai,data::PropertyView& owner_properties,
  void* context,bool(*owner_position)(void*,std::uintptr_t,const float*&,std::string&));
 TargetServices16 borrow()noexcept{return {this,invoke};}
 const std::string& error()const noexcept{return error_;}
};
}

