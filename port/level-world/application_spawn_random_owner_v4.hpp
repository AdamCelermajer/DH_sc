#pragma once
#include "game_object_spawn_probability_v1.hpp"
#include "canonical_gameobject_base_owner_v1.hpp"
namespace dh2::world {
// One application-lifetime owner for original Random globals. Do not construct
// per actor, level, GL recreation or loader candidate. Both seeds start at ELF
// BSS zero; globalinitializer3136dc clears both counters.
class ApplicationSpawnRandomOwnerV4 {
 data::LootRandom8V2 channels_[2]{};
public:
 data::LootRandom8V2& channel(unsigned index){return channels_[index?1:0];}
 // Source writers outside the constructor must borrow these SAME fields.
};
struct CanonicalSpawnApplicationServicesV4 {
 std::shared_ptr<void> application_lease;
 ApplicationSpawnRandomOwnerV4* random{};
 std::function<bool(bool&,std::string&)> online_byte5,handle_as_player;
 std::function<bool(std::string&)> set_visible_false,mark_for_deletion;
};
bool canonical_check_spawn_probability_v4(CanonicalGameObjectBaseOwnerV1&,
 const CanonicalSpawnApplicationServicesV4&,std::int32_t&,std::int32_t&,std::string&);
}
