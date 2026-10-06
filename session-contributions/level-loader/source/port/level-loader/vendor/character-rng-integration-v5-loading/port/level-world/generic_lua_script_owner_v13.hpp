#pragma once
#include "../script-runtime/script_runtime.h"
#include "../script-runtime/script_int_bindings.hpp"
#include "../script-runtime/script_scalar_bindings.h"
#include "../script-runtime/script_design_bindings.h"
#include "../script-runtime/script_object_bridge.h"
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
namespace dh2::scripts {
struct LuaScriptCacheServicesV13 {
 std::shared_ptr<void> owner;void* context{};
 bool(*read_file)(void*,const std::string&,std::vector<std::uint8_t>&,bool& found,std::string&){};
};
// Actual global LuaManager's filename/file-stream authority; shared across
// private LuaScript Instances. It is NOT a shared interpreter/Lua VM.
class LuaScriptCacheOwnerV13 {
 LuaScriptCacheServicesV13 services_;
 std::map<std::string,std::shared_ptr<const std::vector<std::uint8_t>>> files_;
public:
 explicit LuaScriptCacheOwnerV13(LuaScriptCacheServicesV13 services):services_(std::move(services)){}
 bool file(const std::string&,std::shared_ptr<const std::vector<std::uint8_t>>&,bool&,std::string&);
 void flush_buffered_files()noexcept{files_.clear();} // whole379fe8 owned buffers then map reset
 std::size_t cached_count()const noexcept{return files_.size();}
};
struct LuaNativeBindingV13 {
 dh2_script_function values{};dh2_script_scoped_values_function scoped_values{};
 void* context{};bool object_results{};std::shared_ptr<void> owner;
};
struct LuaScriptServicesV13 {
 std::shared_ptr<void> owner;void* context{};
 bool(*resolve_native)(void*,const char* actual_name,std::uint32_t actual_callback,LuaNativeBindingV13&,std::string&){};
 dh2_script_scalar_bindings scalar{};
 dh2_script_design_bindings design{};
 dh2_script_int_identity integer_identity{};
 dh2_script_int_format_fraction integer_fraction{};
 void* integer_context{};
 const dh2_script_object_services* objects{};
};
struct LuaBindingObservationV13 {std::string name;std::uint32_t callback{};bool genuine_builtin{},actual_backend{},installed{};};
class GenericLuaScriptOwnerV13 {
 struct Impl;std::unique_ptr<Impl> impl_;
 explicit GenericLuaScriptOwnerV13(std::unique_ptr<Impl>);
public:
 static std::unique_ptr<GenericLuaScriptOwnerV13> create(bool source_defer_bindings,
  std::shared_ptr<LuaScriptCacheOwnerV13> same_global_cache,LuaScriptServicesV13,
  std::size_t vm_limit,std::string&);
 ~GenericLuaScriptOwnerV13();
 GenericLuaScriptOwnerV13(const GenericLuaScriptOwnerV13&)=delete;
 bool bind_functions(std::string&);
 bool assign_path(const std::string&,std::string&);
 bool load(const char* requested,bool& source_loaded,std::string&);
 // Source Call overload that projects then discards ReturnValues. The bool
 // is source protected-call success, not a script's first returned value.
 bool call(const char* actual_name,const dh2_script_value*,std::uint32_t,bool& source_success,std::string&);
 dh2_script_vm* vm_borrow()const noexcept;
 const std::string& path()const noexcept;
 const std::set<std::string>& loaded_files()const noexcept;
 const std::vector<LuaBindingObservationV13>& bindings()const noexcept;
 std::uintptr_t identity()const noexcept;
 int last_source_status()const noexcept;
};
}
