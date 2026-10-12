#include "static_decor_inspection_v1.hpp"
#include <iomanip>
#include <limits>
#include <locale>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
namespace dh2::loader {
namespace {
std::string escaped(const std::string& value) {
    std::string out;
    for(char c:value)switch(c){case '&':out+="&amp;";break;case '<':out+="&lt;";break;
        case '>':out+="&gt;";break;case '"':out+="&quot;";break;case '\n':out+="&#10;";break;
        case '\r':out+="&#13;";break;case '\t':out+="&#9;";break;default:out+=c;}
    return out;
}
std::string point(const std::array<float,3>& value) {
    std::ostringstream s;s.imbue(std::locale::classic());s<<std::setprecision(std::numeric_limits<float>::max_digits10)
        <<value[0]<<','<<value[1]<<','<<value[2];return s.str();
}
}
bool prepare_static_decor_inspection_v1(const assets::ZipAssetPackV1& pack,
    FixedDeclarationsV1::Borrow source,StaticDecorInspectionV1& out,std::string& error) {
    error.clear();
    try {
        if(!source)throw std::runtime_error("Decor inspection source unavailable");
        StaticDecorInspectionV1 next;next.source_owner=source;std::string xml="<Level>";
        std::map<std::string,bool> mesh_assets;
        std::map<std::string,std::set<std::string>> asset_selectors;
        for(const auto& declaration:source.declarations()) {
            const auto& e=source.element(declaration);const auto* type=e.attribute("gametype");
            if(!type||*type!="AnimatedDecor")continue;
            std::string reason;
            for(const char* gate:{"template","activate_cond","condition_desc"})
                if(const auto* v=e.attribute(gate);v&&!v->empty())reason="Runtime template/condition provider required";
            if(!declaration.translated_position||!declaration.rotation_degrees||!declaration.scale)
                reason="Complete authored source pose required";
            const auto* dae=e.attribute("dae");const auto* xref=e.attribute("xrefobject");
            if(!dae||dae->empty()||!xref||xref->empty())reason="Authored visual resource/selector required";
            if(const auto* visible=e.attribute("visible");visible&&*visible!="0"&&*visible!="1")reason="Runtime visibility property provider required";
            if(!reason.empty()){next.skipped.push_back({declaration,std::move(reason)});continue;}
            std::string key;
            if(!assets::ZipAssetPackV1::key(*dae,key,error))throw std::runtime_error(error);
            auto known=mesh_assets.find(key);
            if(known==mesh_assets.end()) {
                bool found=false;std::vector<std::uint8_t> bytes;
                if(!pack.read(*dae,found,bytes,error))throw std::runtime_error(error);
                if(!found)throw std::runtime_error("Missing authored decor scene: "+*dae);
                resources::BresView view{};
                if(dh2_bres_open(&view,bytes.data(),bytes.size())!=resources::BresError::ok)
                    throw std::runtime_error("Invalid decor BRES: "+key);
                scene::Scene inspected;std::string scene_error;
                const bool loaded=scene::load(view,inspected,scene_error);
                if(!loaded&&scene_error!="Scene has no visible geometry")
                    throw std::runtime_error(key+": "+scene_error);
                if(loaded)for(const auto& node:inspected.graph)asset_selectors[key].insert(node.id);
                known=mesh_assets.emplace(key,loaded).first;
            }
            if(!known->second) {
                next.skipped.push_back({declaration,"No static mesh geometry; particle/runtime visual provider required"});
                continue;
            }
            if(!asset_selectors.at(key).count(*xref+"-node")) {
                next.skipped.push_back({declaration,"Authored selector outside module-node inspection; canonical AnimatedDecor node resolver required"});
                continue;
            }
            bool visible=declaration.module==no_source_v1||source.map().modules().at(declaration.module).authored_visible;
            if(const auto* v=e.attribute("visible"))visible=visible&&*v!="0";
            // A retained derived geometry input reuses the checked module scene
            // assembly. These names/indices never become runtime class/save IDs.
            xml+="<GameObject gametype=\"Module\" name=\"decor-inspection-"+std::to_string(declaration.source_order)+
                "\" dae=\""+escaped(*dae)+"\" xrefobject=\""+escaped(*xref)+"\" position=\""+
                point(*declaration.translated_position)+"\" rotation=\""+point(*declaration.rotation_degrees)+
                "\" scale=\""+point(*declaration.scale)+"\" visible=\""+(visible?"1":"0")+"\"/>";
            next.declarations.push_back(declaration);
        }
        xml+="</Level>";
        if(!next.declarations.empty()) {
            XmlDocumentV1 document;
            if(!document.capture("derived/static-decor-inspection.mlx",std::vector<std::uint8_t>(xml.begin(),xml.end()),error))throw std::runtime_error(error);
            auto owner=std::make_shared<const FixedDeclarationsV1::Borrow>(source);FixedSourcesV1 sources;
            if(!sources.prepare_document(pack,source.map().sources().identity(),document.borrow(),owner,error))throw std::runtime_error(error);
            FixedMapV1 map;if(!map.prepare_geometry_inspection(pack,sources.borrow(),error))throw std::runtime_error(error);next.map=map.borrow();
            if(next.map.modules().size()!=next.declarations.size())throw std::runtime_error("Decor inspection/source count mismatch");
        }
        out=std::move(next);return true;
    }catch(const std::exception& caught){error=caught.what();return false;}
}
}
