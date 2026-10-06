#include "fixed_declarations_v1.hpp"
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <map>
#include <stdexcept>
namespace dh2::loader {
struct FixedDeclarationsV1::Snapshot {
    FixedMapV1::Borrow map;
    std::vector<ObjectDeclarationV1> declarations;
};
const FixedMapV1::Borrow& FixedDeclarationsV1::Borrow::map()const {
    if(!snapshot_)throw std::logic_error("Declarations unavailable");
    return snapshot_->map;
}
const std::vector<ObjectDeclarationV1>& FixedDeclarationsV1::Borrow::declarations()const {
    if(!snapshot_)throw std::logic_error("Declarations unavailable");
    return snapshot_->declarations;
}
const XmlDocumentV1::Borrow& FixedDeclarationsV1::Borrow::document(const ObjectDeclarationV1& d)const {
    return map().sources().documents().at(d.document);
}
const XmlElementV1& FixedDeclarationsV1::Borrow::element(const ObjectDeclarationV1& d)const {
    return document(d).elements().at(d.element);
}
namespace {
std::optional<std::array<float,3>> tuple(const XmlElementV1& e,const char* field) {
    const auto* value=e.attribute(field);if(!value)return {};
    const char* at=value->c_str();std::array<float,3> result{};
    // Checked native projection domain. Full raw spelling remains available;
    // CStrProps malformed-input behavior has not been generalized here.
    for(unsigned j=0;j<3;++j){
        char* end=nullptr;errno=0;result[j]=std::strtof(at,&end);
        if(end==at||errno==ERANGE||!std::isfinite(result[j]))throw std::runtime_error(std::string("Invalid authored ")+field);
        at=end;while(*at==' '||*at=='\t')++at;
        if(j<2){if(*at!=',')throw std::runtime_error(std::string("Invalid authored ")+field+" separator");++at;}
        else if(*at)throw std::runtime_error(std::string("Trailing authored ")+field+" data");
    }return result;
}
}
bool FixedDeclarationsV1::prepare(FixedMapV1::Borrow map,std::string& error) {
    error.clear();try {
        if(!map||map.sources().documents().empty())throw std::runtime_error("No prepared map for declarations");
        auto next=std::make_shared<Snapshot>();next->map=std::move(map);
        std::map<std::pair<std::uint32_t,std::uint32_t>,const ModuleSourceLinksV1*> links;
        for(const auto& link:next->map.sources().module_links())
            if(!links.emplace(std::make_pair(link.document,link.element),&link).second)
                throw std::runtime_error("Duplicate module source dependency");
        auto collect=[&](std::uint32_t document,const char* tag,std::uint32_t module,DeclarationOriginV1 origin,std::array<float,3> offset){
            const auto& doc=next->map.sources().documents().at(document);std::uint32_t selected=no_source_v1;
            for(auto root:doc.roots())if(doc.elements().at(root).tag==tag){
                if(selected!=no_source_v1)throw std::runtime_error("Multiple matching roots require original caller policy: "+doc.uri());
                selected=root;
            }
            if(selected==no_source_v1)throw std::runtime_error("Declaration root missing: "+doc.uri());
            const auto& children=doc.elements().at(selected).children;
            for(unsigned order=0;order<children.size();++order){
                if(next->declarations.size()>=100000)throw std::runtime_error("Declaration occurrence limit exceeded");
                ObjectDeclarationV1 d;d.document=document;d.element=children[order];d.module=module;d.origin=origin;d.source_order=order;d.module_offset=offset;
                const auto& e=doc.elements().at(d.element);
                try{d.authored_position=tuple(e,"position");d.rotation_degrees=tuple(e,"rotation");d.scale=tuple(e,"scale");}
                catch(const std::exception& failure){throw std::runtime_error(doc.uri()+":"+std::to_string(d.element)+": "+failure.what());}
                if(d.authored_position){d.translated_position=*d.authored_position;
                    // ObjectManager::LoadFromXML at 0x34ba8c..0x34bad8 adds
                    // the current module translation to game objects after
                    // the IsGameObject check (+0x20), not an InitPre call.
                    // This is an authored projection, not final runtime position.
                    for(unsigned j=0;j<3;++j){(*d.translated_position)[j]+=offset[j];
                        if(!std::isfinite((*d.translated_position)[j]))throw std::runtime_error("Translated authored position overflow");}
                }
                next->declarations.push_back(std::move(d));
            }
        };
        // Grouping is a source occurrence index. It is deliberately not an
        // assertion about original InitPost execution/publication order.
        collect(0,"Level",no_source_v1,DeclarationOriginV1::level,{});
        for(unsigned id=0;id<next->map.modules().size();++id){
            const auto& module=next->map.modules()[id];const auto found=links.find({module.document,module.element});
            if(found==links.end())throw std::runtime_error("Unprepared module source dependencies");
            const auto& link=*found->second;
            if(link.gameplay!=no_source_v1)collect(link.gameplay,"Module",id,DeclarationOriginV1::gameplay,module.transform.position);
            if(link.visual!=no_source_v1)collect(link.visual,"Module",id,DeclarationOriginV1::visual,module.transform.position);
        }
        snapshot_=std::move(next);return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
}
