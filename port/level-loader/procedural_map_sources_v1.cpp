#include "procedural_map_sources_v1.hpp"
#include <sstream>
#include <stdexcept>
namespace dh2::loader {
namespace {
std::string escaped(const std::string& input){
    std::string out;
    for(const unsigned char c:input){
        switch(c){case '&':out+="&amp;";break;case '<':out+="&lt;";break;case '>':out+="&gt;";break;
            case '"':out+="&quot;";break;case '\r':out+="&#13;";break;case '\n':out+="&#10;";break;case '\t':out+="&#9;";break;
            default:if(c<32)throw std::runtime_error("Unsupported XML attribute byte");out+=char(c);}
    }
    return out;
}
}
bool prepare_procedural_map_sources_v1(const assets::ZipAssetPackV1& pack,ProceduralModulePlanV1 plan,
    ProceduralMapSourcesV1& out,std::string& error){
    try{
        if(!plan.layout.source_owner||!plan.layout.generated||plan.modules.size()!=plan.layout.tiles.size())
            throw std::runtime_error("No generated layout for map source assembly");
        ProceduralMapSourcesV1 next;next.modules=std::make_shared<const ProceduralModulePlanV1>(std::move(plan));
        const auto& original=next.modules->layout.source_owner.lists().connections().blocks().sources();
        const auto& root=original.documents().at(0).elements().at(original.rule_root());
        std::ostringstream xml;xml<<"<Level";
        // Authored rule-root configuration stays in both the raw rule and this
        // inspection root; it is not filtered through unverified defaults.
        for(const auto& attribute:root.attributes)xml<<' '<<attribute.first<<"=\""<<escaped(attribute.second)<<'"';
        xml<<">\n";
        for(const auto& module:next.modules->modules){
            xml<<"<GameObject";
            for(const auto& attribute:module.overrides)xml<<' '<<attribute.first<<"=\""<<escaped(attribute.second)<<'"';
            xml<<"/>\n";
        }
        xml<<"</Level>";const auto text=xml.str();XmlDocumentV1 document;
        const auto uri="generated-inspection/"+original.identity()+"/"+std::to_string(next.modules->layout.seed)+".mlx";
        if(!document.capture(uri,std::vector<std::uint8_t>(text.begin(),text.end()),error))return false;
        if(!document.borrow().parsed())throw std::runtime_error("Derived inspection document parse failed");
        next.inspection_root=document.borrow();FixedSourcesV1 sources;
        if(!sources.prepare_document(pack,original.identity(),next.inspection_root,next.modules,error))return false;
        next.sources=sources.borrow();out=std::move(next);error.clear();return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
}
