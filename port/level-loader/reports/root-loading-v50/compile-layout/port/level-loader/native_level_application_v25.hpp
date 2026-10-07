#pragma once
#include "level_constructor_bindings_v4.hpp"
#include "canonical_level_context_v1.hpp"
#include "../level-world/canonical_level_config_module_v1.hpp"
namespace dh2::loader {
// Native Application facet for the source Level C1. Files/cache/static cells
// have Application lifetime; each Level has its own Lua44/Save_ec receivers.
// This is not GSLevel publication and does not claim Level::Init readiness.
struct NativeLevelFilesV25 {
 std::shared_ptr<void> owner;
 std::function<bool(const std::string&,std::vector<std::uint8_t>&,bool&,std::string&)> script;
 std::function<bool(const std::string&,bool&,std::vector<std::uint8_t>&,std::string&)> saved;
};
class NativeLevelApplicationV25 : public std::enable_shared_from_this<NativeLevelApplicationV25> {
 NativeLevelFilesV25 files_;
 std::shared_ptr<scripts::LuaScriptCacheOwnerV13> scripts_;
 static bool saved(void*,const std::string&,bool&,std::vector<std::uint8_t>&,std::string&);
public:
 std::uint32_t debug_level_load_count{};
 world::ModuleRuntimeGlobalsV1 module_globals;
 explicit NativeLevelApplicationV25(NativeLevelFilesV25);
 // Caller supplies SAME design/VM-native/network receivers. No offline value,
 // deferred fake bindings or replacement saved file is supplied by this facet.
 LevelConstructorApplicationV4 bind(LevelConstructorApplicationV4);
 const std::shared_ptr<scripts::LuaScriptCacheOwnerV13>& script_cache()const noexcept{return scripts_;}
};
class NativeLevelConnectionV25 {
 std::shared_ptr<CanonicalLevelContextV1> candidate_;
 std::shared_ptr<LevelConstructorBindingsV4> bindings_;
 bool allocation_attempted_{},attempted_{},complete_{};
 std::string error_;
public:
 bool allocate(LevelSourceRequestV1,std::shared_ptr<void>,std::string&);
 bool construct_allocated(LevelConstructorArgumentsV3,LevelConstructorApplicationV4,std::string&);
 bool construct(LevelSourceRequestV1,LevelConstructorArgumentsV3,LevelConstructorApplicationV4,std::string&);
 const std::shared_ptr<CanonicalLevelContextV1>& candidate()const noexcept{return candidate_;}
 const std::shared_ptr<LevelConstructorBindingsV4>& bindings()const noexcept{return bindings_;}
 bool complete()const noexcept{return complete_;}
 const std::string& error()const noexcept{return error_;}
};
}
