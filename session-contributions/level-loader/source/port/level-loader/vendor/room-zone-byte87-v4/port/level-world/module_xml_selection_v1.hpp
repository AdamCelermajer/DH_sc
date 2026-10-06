#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
namespace dh2::world {
struct ModuleXmlFieldsV1 {std::string mgp378,mvp390,alt_mgp3a8,alt_mvp3c0,alt_prob3d8;};
struct ModuleXmlServicesV1 {
 std::shared_ptr<void> owner;
 // Source Random clone388c58; borrow the sole original channel/state.
 std::function<bool(std::int32_t,std::int32_t&,std::string&)> random;
};
bool module_choose_xmls_v1(const ModuleXmlFieldsV1&,const ModuleXmlServicesV1&,
                          std::string& mgp,std::string& mvp,std::string& error);
struct ModuleLevelLoadBorrowV1 {
 std::shared_ptr<void> owner;
 std::int32_t* object_module_id18c{};
 float* module_offset160{};
 // Whole actual Level::LoadFile; deliveredfalse stops with diagnostic,
 // deliveredtrue/sourcefalse repeats the original load attempt.
 std::function<bool(const std::string&,const char*,bool&,std::string&)> load_file;
};
bool module_load_v1(const ModuleXmlFieldsV1&,std::int32_t module_id,const float* position,
                    const ModuleXmlServicesV1&,const ModuleLevelLoadBorrowV1&,std::string&);
}
