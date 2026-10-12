#include "source_reference_repairs_v1.hpp"
#include "resource_paths_v1.hpp"
#include <stdexcept>
namespace dh2::loader {
namespace {
struct Repair {const char* authored;const char* resolved;const char* reason;};
constexpr Repair known[] {
    {"data/3d/modules/void_maze/mgp/vm011_corner_voidmaze_ne_00_01.mgp",
     "data/3d/modules/void_maze/mgp/vm01_corner_voidmaze_ne_00_01.mgp",
     "Original Void Maze rule has vm011 instead of cached vm01"},
    {"data/3d/modules/void_maze/mvp/vm01_x_voidmaze_nswe_00",
     "data/3d/modules/void_maze/mvp/vm01_x_voidmaze_nswe_00.mvp",
     "Original Void Maze 02 visual reference omits the .mvp extension"}
};
bool exists(const assets::ZipAssetPackV1& pack,const std::string& authored) {
    for(const auto& candidate:compiled_level_paths_v1(authored)) {
        bool found=false;std::vector<std::uint8_t> bytes;std::string error;
        if(!pack.read(candidate,found,bytes,error))throw std::runtime_error(error);
        if(found)return true;
    }return false;
}
}
std::string original_backup_definition_v1(const std::string& definition) {
    if(definition.empty()||definition.find('\0')!=std::string::npos)
        throw std::invalid_argument("Invalid original backup definition");
    const auto slash=definition.find_last_of("/\\");
    const auto start=slash==std::string::npos?0:slash+1;
    if(start==definition.size())throw std::invalid_argument("Backup resource basename absent");
    auto result=definition;result[start]='x';
    const auto dot=result.find('.',start);
    if(dot!=std::string::npos)result.resize(dot);
    result+="_BACKUP.mlx";return result;
}
bool repair_procedural_references_v1(const assets::ZipAssetPackV1& pack,
    ProceduralModulePlanV1& plan,std::string& error) {
    error.clear();
    try {
        auto next=plan;
        for(auto& module:next.modules)for(const char* property:{"mgp","mvp"}) {
            auto value=module.overrides.find(property);if(value==module.overrides.end())continue;
            const Repair* selected=nullptr;
            for(const auto& candidate:compiled_level_paths_v1(value->second)) {
                std::string key,why;
                if(!assets::ZipAssetPackV1::key(candidate,key,why))throw std::runtime_error(why);
                for(const auto& repair:known)if(key==repair.authored){selected=&repair;break;}
                if(selected)break;
            }
            if(!selected||exists(pack,value->second))continue;
            if(!exists(pack,selected->resolved))throw std::runtime_error(
                "Known source repair target unavailable: "+value->second+" -> "+selected->resolved);
            next.reference_repairs.push_back({module.tile,property,value->second,selected->resolved,selected->reason});
            value->second=selected->resolved;
        }
        plan=std::move(next);return true;
    }catch(const std::exception& caught){error=caught.what();return false;}
}
}
