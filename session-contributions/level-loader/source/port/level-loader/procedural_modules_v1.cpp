#include "procedural_modules_v1.hpp"
#include <cmath>
#include <iomanip>
#include <locale>
#include <sstream>
#include <stdexcept>
namespace dh2::loader {
namespace {
void checked_text(const std::string& text){
    if(text.size()>=4096||text.find('\0')!=std::string::npos)throw std::runtime_error("Module text outside checked domain");
}
std::string mvx_name(const std::string& folder,const std::string& name){return folder+"/mvx/"+name+".mvx";}
std::string path_base(const std::string& folder,const std::string& target){
    const auto found=folder.find("data/");const auto offset=found==std::string::npos?4:found+5;
    if(offset>folder.size())throw std::runtime_error("Original generated module folder substring outside source");
    return "data/"+target+"/"+folder.substr(offset);
}
void set(ProceduralModuleV1& module,std::string name,std::optional<std::string> value){
    if(value){checked_text(*value);module.overrides[name]=*value;}
    module.setter_attempts.push_back({std::move(name),std::move(value)});
}
}
bool project_procedural_module_v1(const ProceduralLayoutTileV1& tile,std::uint32_t index,
    const ProceduralBlockV1& block,const std::string& folder,const std::string& target,
    XmlDocumentV1::Borrow mvx,ProceduralModuleV1& out,std::string& error){
    try{
        checked_text(tile.name);checked_text(folder);checked_text(target);
        if(index>INT32_MAX||block.width==INT32_MIN||block.height==INT32_MIN)
            throw std::runtime_error("Module index/dimension outside original int32 domain");
        ProceduralModuleV1 next;next.tile=index;next.mvx_uri=mvx_name(folder,tile.name);next.mvx_found=bool(mvx);
        const auto name=tile.name+"_"+std::to_string(index);
        if(name.size()>=256)throw std::runtime_error("Original module name format buffer exceeded");
        set(next,"name",name);set(next,"gametype",std::string("Module"));
        // Original float32 sequence, before promotion to sprintf's doubles.
        const float half_width=float(block.width-1)*0.5f;
        const float half_height=float(block.height-1)*0.5f;
        next.position={(float(tile.grid[0])+half_width)*block.unit_width,
                       (float(tile.grid[1])+half_height)*(-block.unit_height),tile.height};
        for(const auto value:next.position)if(!std::isfinite(value))throw std::runtime_error("Generated module position is not finite");
        std::ostringstream position;position.imbue(std::locale::classic());position<<std::fixed<<std::setprecision(6)
            <<double(next.position[0])<<','<<double(next.position[1])<<','<<double(next.position[2]);
        if(position.str().size()>=256)throw std::runtime_error("Original module position format buffer exceeded");
        set(next,"position",position.str());
        if(mvx){
            const XmlElementV1* first=nullptr;
            for(const auto root:mvx.roots())if(mvx.elements().at(root).tag=="Module"){
                for(const auto child:mvx.elements().at(root).children)if(mvx.elements().at(child).tag=="GameObject"){
                    first=&mvx.elements().at(child);break;
                }
                break;
            }
            if(first)for(const char* key:{"scale","xrefmax","xrefobject","dae","fog_color","is_solid"}){
                const auto* value=first->attribute(key);set(next,key,value?std::optional<std::string>(*value):std::nullopt);
            }
        }
        const auto base=path_base(folder,target);
        const std::pair<const char*,const std::string*> files[2]{{"mgp",&tile.list_element.gameplay},{"mvp",&tile.list_element.visual}};
        for(const auto& pair:files){
            checked_text(*pair.second);
            if(!pair.second->empty()){
                auto path=base+"/"+pair.first+"/"+*pair.second;
                if(path.size()>=256)throw std::runtime_error("Original module path format buffer exceeded");
                set(next,pair.first,std::move(path));
            }
        }
        out=std::move(next);error.clear();return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
bool prepare_procedural_modules_v1(const assets::ZipAssetPackV1& pack,const ProceduralLayoutResultV1& layout,
    ProceduralModulePlanV1& out,std::string& error){
    try{
        if(!layout.source_owner)throw std::runtime_error("Generated layout source owner unavailable");
        if(layout.generated!=!layout.tiles.empty())throw std::runtime_error("Generated layout status/tiles inconsistent");
        const auto& blocks=layout.source_owner.lists().connections().blocks();const auto& sources=blocks.sources();
        const auto& root=sources.documents().at(0).elements().at(sources.rule_root());
        const auto* target=root.attribute("target");if(!target)throw std::runtime_error("Original rule target unavailable");
        ProceduralModulePlanV1 next;next.layout=layout;std::map<std::string,std::uint32_t> captured;
        for(std::uint32_t i=0;i<layout.tiles.size();++i){
            const auto& tile=layout.tiles[i];const auto uri=mvx_name(sources.folder(),tile.name);
            std::string key;if(!assets::ZipAssetPackV1::key(uri,key,error))throw std::runtime_error(error);
            XmlDocumentV1::Borrow doc;std::uint32_t document=UINT32_MAX;
            if(const auto previous=captured.find(key);previous!=captured.end()){
                document=previous->second;doc=next.mvx_documents.at(document);
            }else{
                bool found=false;std::vector<std::uint8_t> raw;
                if(!pack.read(uri,found,raw,error))throw std::runtime_error(error);
                if(found){
                    XmlDocumentV1 parser;if(!parser.capture(key,std::move(raw),error))throw std::runtime_error(error);
                    // SetModuleMVXProperties ignores LoadFromBuffer's boolean.
                    // Retain any parser diagnostic and select its first matching children.
                    doc=parser.borrow();document=std::uint32_t(next.mvx_documents.size());
                    next.mvx_documents.push_back(doc);captured.emplace(key,document);
                }
            }
            if(tile.block_source>=blocks.blocks().size()||tile.name!=sources.blocks().at(tile.block_source).name)
                throw std::runtime_error("Generated tile/source identity inconsistent");
            ProceduralModuleV1 module;
            if(!project_procedural_module_v1(tile,i,blocks.blocks().at(tile.block_source),sources.folder(),*target,doc,module,error))return false;
            module.mvx_document=document;next.modules.push_back(std::move(module));
        }
        next.returned_index=next.modules.empty()?0:std::uint32_t(next.modules.size()-1);
        out=std::move(next);error.clear();return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
}
