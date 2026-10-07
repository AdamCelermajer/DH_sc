#pragma once
#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>
namespace dh2::world {
struct SpawnGroupChoiceV108 {std::int32_t character4{},weight8{},quantity_c{};};
//Immutable source row view. Its fields are read from the SAME process Arrays,
//not a second table cache or a saved-state projection.
struct SpawnGroupDefinitionV108 {
 std::shared_ptr<const void> owner;bool local_only4{};std::int32_t delay8{};
 std::uint32_t count_c{};
 std::function<bool(std::uint32_t,SpawnGroupChoiceV108&,std::string&)> choice;
};
struct SpawnSpotBorrowV108 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 const std::string* group374{};const float* position160{};
};
struct SpawnGroupServicesV108 {
 std::shared_ptr<void> owner;
 std::function<bool(std::uint32_t&,std::string&)> application_dt;
 std::function<bool(const char*,std::int32_t&,std::string&)> group_id;
 std::function<bool(std::int32_t,SpawnGroupDefinitionV108&,std::string&)> definition;
 std::function<bool(std::uintptr_t,bool&,std::string&)> is_zonable_c4;
 std::function<bool(std::uintptr_t,std::uint8_t&,std::uint8_t&,std::string&)> zone_flags;
 std::function<bool(std::int32_t,std::int32_t&,std::string&)> random;
 std::function<bool(const char*,const char*,bool,bool,std::uintptr_t&,std::string&)> spawn;
 std::function<bool(std::uintptr_t,std::int32_t,const float*,std::string&)> init_spawned;
 std::function<bool(std::uintptr_t,std::uintptr_t,std::string&)> place;
 std::function<bool(const char*,std::string&)> log;
};
//Source GetInstance3ea730 C1: one process map, empty at real first construction.
//InsSpawn/DelSpawn alone produce/remove members; update never fabricates them.
class SpawnGroupManagerV108 final {
 struct Member {std::weak_ptr<void> owner;std::uintptr_t identity;const std::string* group;const float* position;};
 struct Group {std::list<Member> spots;std::int32_t timer8{};};
 std::map<std::int32_t,Group> groups_; //actual tree4/list/timer source storage
 bool busy_{};
 static std::uint32_t next_spawn_name_v108();
public:
 SpawnGroupManagerV108()=default;
 bool insert(const SpawnSpotBorrowV108&,const SpawnGroupServicesV108&,std::string&);
 bool erase(const SpawnSpotBorrowV108&,const SpawnGroupServicesV108&,std::string&);
 bool update(double ignored,const SpawnGroupServicesV108&,std::string&);
};
std::shared_ptr<SpawnGroupManagerV108> process_spawn_group_manager_v108();
}
