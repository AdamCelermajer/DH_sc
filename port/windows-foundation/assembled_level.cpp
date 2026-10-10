#include "assembled_level.hpp"
#include "level_manifest.hpp"
#include <algorithm>
#include <cctype>
#include <limits>
#include <map>
#include <stdexcept>

namespace dh::foundation {
namespace {
std::filesystem::path find_asset(AssetCatalog& assets,const std::filesystem::path& manifest,
                                 std::string uri) {
    std::replace(uri.begin(),uri.end(),'\\','/');
    std::string lower=uri;std::transform(lower.begin(),lower.end(),lower.begin(),[](unsigned char c){return static_cast<char>(std::tolower(c));});
    std::vector<std::filesystem::path> paths{uri,manifest.parent_path()/uri,std::filesystem::path("original-cache")/uri,
                                           lower,manifest.parent_path()/lower,std::filesystem::path("original-cache")/lower};
    const std::string prefix="data/iphone/";
    if(lower.compare(0,prefix.size(),prefix)==0) {
        const auto alias="data/"+lower.substr(prefix.size());paths.push_back(alias);paths.push_back(std::filesystem::path("original-cache")/alias);
    }
    for(const auto& path:paths) {try{assets.resolve(path);return path;}catch(const std::exception&) {}}
    throw std::runtime_error("Level resource is missing: "+uri);
}
void merge(OriginalScene& combined,OriginalScene&& part) {
    if(combined.mesh.vertices.size()+part.mesh.vertices.size()>std::numeric_limits<std::uint32_t>::max())
        throw std::runtime_error("Assembled level exceeds 32-bit vertex domain");
    const auto vertexBase=static_cast<std::uint32_t>(combined.mesh.vertices.size());
    const auto indexBase=combined.mesh.indices.size();
    combined.mesh.vertices.insert(combined.mesh.vertices.end(),part.mesh.vertices.begin(),part.mesh.vertices.end());
    for(auto index:part.mesh.indices) combined.mesh.indices.push_back(index+vertexBase);
    for(auto range:part.mesh.ranges) {range.firstIndex+=indexBase;combined.mesh.ranges.push_back(range);}
    combined.materials.insert(combined.materials.end(),std::make_move_iterator(part.materials.begin()),std::make_move_iterator(part.materials.end()));
    combined.minimum={std::min(combined.minimum.x,part.minimum.x),std::min(combined.minimum.y,part.minimum.y),std::min(combined.minimum.z,part.minimum.z)};
    combined.maximum={std::max(combined.maximum.x,part.maximum.x),std::max(combined.maximum.y,part.maximum.y),std::max(combined.maximum.z,part.maximum.z)};
    combined.nodeCount+=part.nodeCount;combined.instanceCount+=part.instanceCount;combined.triangleCount+=part.triangleCount;
    combined.notices.insert(combined.notices.end(),part.notices.begin(),part.notices.end());
}
}
bool load_level(AssetCatalog& assets,const std::filesystem::path& manifestRelative,
                OriginalScene& output,std::string& error) {
    std::vector<LevelModuleZone> zones;
    return load_level_with_module_zones(assets,manifestRelative,output,zones,error);
}
bool load_level_with_module_zones(AssetCatalog& assets,const std::filesystem::path& manifestRelative,
                OriginalScene& output,std::vector<LevelModuleZone>& zones,std::string& error) {
    try {
        std::vector<ModulePlacement> placements;
        const auto manifestBytes=assets.read(manifestRelative);
        if(!decode_level_manifest(manifestBytes,placements,error)) return false;
        OriginalScene combined;combined.source=manifestRelative.generic_string();
        const auto infinity=std::numeric_limits<float>::infinity();
        combined.minimum={infinity,infinity,infinity};combined.maximum={-infinity,-infinity,-infinity};
        std::map<std::filesystem::path,std::vector<std::uint8_t>> cache;
        std::vector<LevelModuleZone> loadedZones;
        for(std::size_t placementIndex=0;placementIndex<placements.size();++placementIndex) {
            const auto& module=placements[placementIndex];
            if(!module.visible) {combined.notices.push_back("Hidden module skipped: "+module.name);continue;}
            if(!module.activateCondition.empty()) {
                combined.notices.push_back("Conditional module requires campaign evaluation: "+module.name+" ["+module.activateCondition+"]");continue;
            }
            const auto path=find_asset(assets,manifestRelative,module.assetPath);
            auto entry=cache.find(path);if(entry==cache.end()) entry=cache.emplace(path,assets.read(path)).first;
            OriginalScene part;
            if(!decode_original_scene_module(entry->second,module.authoredNode,module.placement,part,error)) {
                error=module.name+": "+error;return false;
            }
            // RoomZone source (features/map_visit): the module room box, or its visible bounds.
            LevelModuleZone zone;
            zone.id=static_cast<std::uint32_t>(placementIndex);zone.name=module.name;
            const auto box=part.hasModuleBounds?part.moduleMinimum:part.minimum;
            const auto boxMax=part.hasModuleBounds?part.moduleMaximum:part.maximum;
            zone.bounds={box.x,box.y,box.z,boxMax.x,boxMax.y,boxMax.z};
            zone.firstRange=combined.mesh.ranges.size();zone.rangeCount=part.mesh.ranges.size();
            loadedZones.push_back(std::move(zone));
            merge(combined,std::move(part));
        }
        // P17 PROCGEN2: the Decor and AnimatedDecor declarations of each visible module's visual document (mvp), in
        // their module-local poses (the swamp cave water caustics, props). Merged like module geometry; no room zone.
        // A missing visual or decor file is a named notice: the module geometry is still valid without it.
        std::size_t decorCount=0;
        for(const auto& module:placements) {
            if(!module.visible||!module.activateCondition.empty()||module.visualPath.empty()) continue;
            std::vector<ModulePlacement> decor;
            try {
                const auto visualPath=find_asset(assets,manifestRelative,module.visualPath);
                auto visual=cache.find(visualPath);if(visual==cache.end()) visual=cache.emplace(visualPath,assets.read(visualPath)).first;
                std::string decorError;
                if(!decode_module_decor(visual->second,module.placement,decor,decorError)) throw std::runtime_error(decorError);
            } catch(const std::exception& ex) {
                combined.notices.push_back("Module visual skipped: "+module.name+" ["+ex.what()+"]");continue;
            }
            for(const auto& item:decor) {
                try {
                    const auto path=find_asset(assets,manifestRelative,item.assetPath);
                    auto entry=cache.find(path);if(entry==cache.end()) entry=cache.emplace(path,assets.read(path)).first;
                    OriginalScene part;std::string partError;
                    if(!decode_original_scene_module(entry->second,item.authoredNode,item.placement,part,partError))
                        throw std::runtime_error(partError);
                    merge(combined,std::move(part));++decorCount;
                } catch(const std::exception& ex) {
                    combined.notices.push_back("Decor skipped: "+item.name+" ["+ex.what()+"]");
                }
            }
        }
        if(combined.mesh.indices.empty()) {error="Level has no visible unconditional module geometry";return false;}
        // P17 PROCGEN2: the LevelConfig skybox, decoded as a whole BRES scene (the same decoder as module geometry).
        // A missing or invalid skybox is a named notice; the level still loads without it.
        const auto skyName=decode_level_skybox(manifestBytes);
        if(!skyName.empty()) {
            try {
                auto sky=std::make_shared<OriginalScene>();
                std::string skyError;
                if(!decode_original_scene_module(assets.read(std::filesystem::path(skyName)),std::string(),identity(),*sky,skyError))
                    throw std::runtime_error(skyError);
                combined.notices.push_back("Skybox loaded: "+skyName+" triangles="+std::to_string(sky->triangleCount));
                combined.skybox=std::move(sky);
            } catch(const std::exception& ex) {
                combined.notices.push_back("Skybox skipped: "+skyName+" ["+ex.what()+"]");
            }
        }
        combined.notices.push_back("Decor instantiated: "+std::to_string(decorCount)+" Decor/AnimatedDecor declarations (authored pose, no animation playback).");
        combined.notices.push_back("Geometry preview only: module gameplay objects, quests, lights, skybox and campaign predicates are not instantiated.");
        output=std::move(combined);zones=std::move(loadedZones);error.clear();return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
}
}
