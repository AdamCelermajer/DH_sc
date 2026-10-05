#include "fixed_map_v1.hpp"
#include "user_properties_v1.hpp"
#include <algorithm>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <map>
#include <set>
#include <stdexcept>
namespace dh2::loader {
struct FixedMapV1::Snapshot {
    FixedSourcesV1::Borrow sources;
    std::vector<MapAssetV1> assets;
    std::vector<MapModuleV1> modules;
    scene::Scene scene;
    std::vector<MapInstanceV1> instances;
    std::unique_ptr<floors::World> navigation;
    std::vector<MapFloorMetadataV1> floor_metadata;
};
const FixedSourcesV1::Borrow& FixedMapV1::Borrow::sources()const {
    if(!snapshot_)throw std::logic_error("Map unavailable");
    return snapshot_->sources;
}
const std::vector<MapAssetV1>& FixedMapV1::Borrow::assets()const {
    if(!snapshot_)throw std::logic_error("Map unavailable");
    return snapshot_->assets;
}
const std::vector<MapModuleV1>& FixedMapV1::Borrow::modules()const {
    if(!snapshot_)throw std::logic_error("Map unavailable");
    return snapshot_->modules;
}
const scene::Scene& FixedMapV1::Borrow::scene()const {
    if(!snapshot_)throw std::logic_error("Map unavailable");
    return snapshot_->scene;
}
const std::vector<MapInstanceV1>& FixedMapV1::Borrow::instances()const {
    if(!snapshot_)throw std::logic_error("Map unavailable");
    return snapshot_->instances;
}
const floors::World& FixedMapV1::Borrow::navigation()const {
    if(!snapshot_)throw std::logic_error("Map unavailable");
    return *snapshot_->navigation;
}
const std::vector<MapFloorMetadataV1>& FixedMapV1::Borrow::floor_metadata()const {
    if(!snapshot_)throw std::logic_error("Map unavailable");
    return snapshot_->floor_metadata;
}
namespace {
std::string attribute(const XmlElementV1& e,const char* name) {
    if(const auto* value=e.attribute(name))return *value;
    return {};
}
std::array<float,3> point(const XmlElementV1& e,const char* name,std::array<float,3> fallback) {
    const auto* text=e.attribute(name);if(!text)return fallback;
    // Checked adapter domain, not a claim about CStrProps malformed-input
    // permissiveness. Reject unsupported syntax instead of inventing defaults.
    const char* at=text->c_str();std::array<float,3> out{};
    for(unsigned i=0;i<3;++i) {
        char* end=nullptr;errno=0;out[i]=std::strtof(at,&end);
        if(end==at||errno==ERANGE||!std::isfinite(out[i]))throw std::runtime_error(std::string("Invalid ")+name+" tuple");
        at=end;while(*at==' '||*at=='\t')++at;
        if(i<2){if(*at!=',')throw std::runtime_error(std::string("Invalid ")+name+" separator");++at;}
        else if(*at)throw std::runtime_error(std::string("Trailing ")+name+" tuple data");
    }
    return out;
}
MapGeometryKindV1 kind(const scene::Node& n,bool root) {
    if(root)return MapGeometryKindV1::module_root;
    if(n.name.find("_floor")!=std::string::npos)return MapGeometryKindV1::floor;
    if(n.name.find("_exit")!=std::string::npos)return MapGeometryKindV1::exit;
    if(n.name.find("_minimap")!=std::string::npos)return MapGeometryKindV1::minimap;
    if(n.name.find("_mesh")!=std::string::npos)return MapGeometryKindV1::mesh;
    return MapGeometryKindV1::unclassified;
}
}
bool FixedMapV1::prepare(const assets::ZipAssetPackV1& pack,FixedSourcesV1::Borrow sources,std::string& error) {
    error.clear();try {
        if(!sources||sources.documents().empty())throw std::runtime_error("No prepared level sources");
        auto next=std::make_shared<Snapshot>();next->sources=std::move(sources);
        next->navigation=std::make_unique<floors::World>();
        std::map<std::string,std::uint32_t> asset_ids;
        std::map<std::pair<std::uint32_t,std::uint32_t>,const ModuleSourceLinksV1*> links;
        for(const auto& link:next->sources.module_links())
            if(!links.emplace(std::make_pair(link.document,link.element),&link).second)
                throw std::runtime_error("Duplicate source module link");
        auto asset=[&](const std::string& authored) {
            if(authored.empty())throw std::runtime_error("Module scene requires resolved dae property");
            std::string key;if(!assets::ZipAssetPackV1::key(authored,key,error))throw std::runtime_error(error);
            if(const auto old=asset_ids.find(key);old!=asset_ids.end())return old->second;
            if(next->assets.size()>=512)throw std::runtime_error("Map asset limit exceeded");
            bool found=false;std::vector<std::uint8_t> bytes;
            // Exact authored asset lookup; Level::LoadFile's XML search policy
            // must not be applied indiscriminately to native visual assets.
            if(!pack.read(authored,found,bytes,error))throw std::runtime_error(error);
            if(!found)throw std::runtime_error("Missing authored module scene: "+authored);
            MapAssetV1 value;value.uri=key;
            value.owner=std::make_shared<const std::vector<std::uint8_t>>(std::move(bytes));
            if(dh2_bres_open(&value.view,value.owner->data(),value.owner->size())!=resources::BresError::ok)
                throw std::runtime_error("Invalid module BRES: "+key);
            if(!scene::load(value.view,value.source,error))throw std::runtime_error(key+": "+error);
            value.material_offset=next->scene.materials.size();
            next->scene.materials.insert(next->scene.materials.end(),value.source.materials.begin(),value.source.materials.end());
            const auto id=std::uint32_t(next->assets.size());next->assets.push_back(std::move(value));asset_ids.emplace(key,id);return id;
        };
        std::set<std::uint32_t> visiting;
        std::function<void(std::uint32_t,const char*,std::uint32_t,std::array<float,3>)> visit;
        visit=[&](std::uint32_t doc_id,const char* root_tag,std::uint32_t parent,std::array<float,3> offset) {
            if(!visiting.insert(doc_id).second)throw std::runtime_error("Recursive module expansion");
            const auto& doc=next->sources.documents().at(doc_id);
            for(auto root:doc.roots()) {
                if(doc.elements().at(root).tag!=root_tag)continue;
                for(auto element:doc.elements().at(root).children) {
                    const auto& e=doc.elements().at(element);
                    if(attribute(e,"gametype")!="Module")continue;
                    if(next->modules.size()>=512)throw std::runtime_error("Map module limit exceeded");
                    if(!attribute(e,"template").empty()||!attribute(e,"templateName").empty())
                        throw std::runtime_error("Module template resolution service required at "+doc.uri()+":"+std::to_string(element));
                    MapModuleV1 m;m.document=doc_id;m.element=element;m.parent=parent;
                    m.authored_name=attribute(e,"name");m.xrefobject=attribute(e,"xrefobject");
                    if(m.authored_name.empty()||m.xrefobject.empty())
                        throw std::runtime_error("Module requires authored name and xrefobject at "+doc.uri()+":"+std::to_string(element));
                    m.activate_cond=attribute(e,"activate_cond");m.condition_desc=attribute(e,"condition_desc");
                    if(!m.activate_cond.empty()||!m.condition_desc.empty())
                        throw std::runtime_error("Conditional module requires gameplay evaluator at "+doc.uri()+":"+std::to_string(element));
                    const auto visible=attribute(e,"visible");
                    if(!visible.empty()&&visible!="0"&&visible!="1")throw std::runtime_error("Module visibility outside checked adapter domain");
                    m.authored_visible=visible!="0";
                    m.transform.position=point(e,"position",{});m.transform.rotation_degrees=point(e,"rotation",{});
                    m.transform.scale=point(e,"scale",{1,1,1});
                    // Original ObjectManager::LoadFromXML adds Level's module
                    // position offset. It does not rotate child declarations.
                    for(unsigned i=0;i<3;++i)m.transform.position[i]+=offset[i];
                    if(!project_visual_transform_v1(m.transform,error))throw std::runtime_error(error);
                    m.asset=asset(attribute(e,"dae"));
                    const auto& source=next->assets.at(m.asset).source;
                    // SceneManager::LoadScene at 0x359990 appends literal -node
                    // to xrefobject before CColladaDatabase::constructNode.
                    const auto selector=m.xrefobject+"-node";
                    auto selected=std::find_if(source.graph.begin(),source.graph.end(),[&](const scene::Node& n){return n.id==selector;});
                    if(selected==source.graph.end())throw std::runtime_error("Missing authored module node: "+selector);
                    if(std::find_if(selected+1,source.graph.end(),[&](const scene::Node& n){return n.id==selector;})!=source.graph.end())
                        throw std::runtime_error("Ambiguous authored module node: "+selector);
                    const auto source_root=std::uint32_t(selected-source.graph.begin());
                    const auto module_id=std::uint32_t(next->modules.size());m.root=next->scene.graph.size();
                    std::vector<std::int32_t> mapping(source.graph.size(),-1);
                    for(unsigned i=source_root;i<source.graph.size();++i) {
                        const auto& n=source.graph[i];
                        if(i!=source_root&&(n.parent<0||mapping.at(n.parent)<0))continue;
                        if(next->scene.graph.size()>=100000)throw std::runtime_error("Placed map node limit exceeded");
                        auto copied=n;
                        // New internal scene IDs only; never substitute these
                        // for authored object identities or persistence keys.
                        copied.id="module-instance/"+std::to_string(module_id)+"/"+n.id;
                        copied.parent=i==source_root?-1:mapping.at(n.parent);
                        if(i==source_root) {
                            std::copy(m.transform.position.begin(),m.transform.position.end(),copied.translation);
                            std::copy(m.transform.quaternion.begin(),m.transform.quaternion.end(),copied.quaternion);
                            std::copy(m.transform.scale.begin(),m.transform.scale.end(),copied.scale);
                        }
                        mapping[i]=next->scene.graph.size();next->scene.graph.push_back(std::move(copied));
                    }
                    for(const auto& i:source.instances) {
                        if(mapping.at(i.node_index)<0)continue;
                        if(next->scene.instances.size()>=100000)throw std::runtime_error("Placed map instance limit exceeded");
                        auto copied=i;copied.node_index=mapping.at(i.node_index);copied.node=next->scene.graph.at(copied.node_index).id;
                        for(auto& material:copied.materials)material+=next->assets.at(m.asset).material_offset;
                        const auto instance_id=std::uint32_t(next->scene.instances.size());
                        next->scene.instances.push_back(std::move(copied));
                        next->instances.push_back({instance_id,m.asset,module_id,kind(source.graph.at(i.node_index),i.node_index==source_root)});
                    }
                    next->modules.push_back(m);
                    const auto link=links.find({doc_id,element});
                    if(link==links.end())throw std::runtime_error("Unprepared module source dependencies");
                    if(link->second->gameplay!=no_source_v1)visit(link->second->gameplay,"Module",module_id,m.transform.position);
                    if(link->second->visual!=no_source_v1)visit(link->second->visual,"Module",module_id,m.transform.position);
                }
            }
            visiting.erase(doc_id);
        };
        visit(0,"Level",no_source_v1,{});
        if(next->modules.empty())throw std::runtime_error("No authored map modules");
        next->scene.nodes=next->scene.graph.size();
        if(!scene::update_world(next->scene,error))throw std::runtime_error(error);
        // Preserve every prepared geometry instance with its kind. Do not
        // silently erase floor/exit/minimap or unclassified source geometry.
        // The existing floor adapter rejects unresolved metadata. This stage
        // resolves it through the original UserProperties semantics and flag
        // kernel first. A separate geometry-only input graph keeps that older
        // adapter's gate satisfied; authoritative raw properties stay intact.
        scene::Scene extraction;extraction.graph=next->scene.graph;
        for(const auto& i:next->instances)if(i.kind==MapGeometryKindV1::floor) {
            const auto& instance=next->scene.instances.at(i.scene_instance);
            MapFloorMetadataV1 metadata;metadata.scene_instance=i.scene_instance;metadata.floor_record=next->navigation->records.size();
            if(!decode_user_properties_v1(next->scene.graph.at(instance.node_index).user_properties,metadata.properties,error))
                throw std::runtime_error(error);
            if(const auto tags=metadata.properties.find("floortypes");tags!=metadata.properties.end())
                if(dh2_floor_source_flags(&metadata.flags,tags->second.data(),tags->second.size())!=0)
                    throw std::runtime_error("Resolved floor type flags rejected");
            extraction.graph.at(instance.node_index).user_properties.clear();
            if(!floors::append(next->assets.at(i.asset).view,extraction,instance,i.module,*next->navigation,error))
                throw std::runtime_error("Module "+next->modules.at(i.module).authored_name+" floor: "+error);
            next->navigation->records.back()->flags=metadata.flags;
            next->floor_metadata.push_back(std::move(metadata));
        }
        // PFWorld::LoadRoom at 0x523edc..0x5240cc removes a candidate
        // navigation room when no floor was found; it does not reject its
        // visual module. Such modules remain explicitly present in this map.
        if(!floors::build_graph(*next->navigation,error)||!floors::post_load(*next->navigation,error))throw std::runtime_error(error);
        snapshot_=std::move(next);return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
}
