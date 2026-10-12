#pragma once
#include "level_constructor_v3.hpp"
#include "../level-world/generic_lua_script_owner_v13.hpp"
#include "../level-world/level_savegame_runtime_v1.hpp"
namespace dh2::loader {
// Application supplies its actual shared file cache, immutable design rows,
// source static cells and private-files backend. No Level, VM, cache/profile or
// networking authority is manufactured by this composition.
struct LevelConstructorApplicationV4 {
 std::shared_ptr<void> owner;
 std::uint32_t* debug_level_load_count{};std::uint32_t* module_id_global{};
 const data::LevelTables* levels{};
 std::shared_ptr<scripts::LuaScriptCacheOwnerV13> lua_cache;
 scripts::LuaScriptServicesV13 lua;
 std::size_t private_vm_limit{}; // explicit native memory policy
 level::LevelSavegameApplicationV1 saves;
 std::function<bool(std::uint8_t&,std::string&)> online_byte5;
 std::function<bool(bool&,std::string&)> local_player_hosting;
 std::function<bool(std::uint8_t&,std::string&)> player_manager_byte719;
 std::function<bool(std::int32_t&,std::string&)> online_state34;
 std::function<bool(bool&,std::string&)> matching_is_host;
};
class LevelConstructorBindingsV4 final : public std::enable_shared_from_this<LevelConstructorBindingsV4> {
 LevelConstructorApplicationV4 application_;
 // Typed weak directory for the SAME receivers retained by Level fields44/ec.
 // Neither receiver is duplicated or retained back through its containing Level.
 std::weak_ptr<scripts::GenericLuaScriptOwnerV13> script_;
 std::weak_ptr<level::LevelSavegameRuntimeV1> save_;
 bool script_attempted_{},save_attempted_{};
 explicit LevelConstructorBindingsV4(LevelConstructorApplicationV4 a):application_(std::move(a)){}
public:
 static std::shared_ptr<LevelConstructorBindingsV4> create(LevelConstructorApplicationV4,std::string&);
 LevelConstructorServicesV3 services();
 std::shared_ptr<scripts::GenericLuaScriptOwnerV13> script()const{return script_.lock();}
 std::shared_ptr<level::LevelSavegameRuntimeV1> save()const{return save_.lock();}
};
}
