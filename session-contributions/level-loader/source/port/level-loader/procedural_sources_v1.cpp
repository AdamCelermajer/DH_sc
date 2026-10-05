#include "procedural_sources_v1.hpp"
#include "procedural_file_list_v1.hpp"
#include <map>
#include <stdexcept>
namespace dh2::loader {
struct ProceduralSourcesV1::Snapshot {
    std::string identity,folder,file_list_uri;
    std::vector<std::uint8_t> file_list_bytes;
    std::vector<std::string> filenames;
    std::vector<XmlDocumentV1::Borrow> documents;
    std::vector<ProceduralBlockSourceV1> blocks;
    std::uint32_t root{};
};
const std::string& ProceduralSourcesV1::Borrow::identity()const {
    if(!snapshot_)throw std::logic_error("Procedural sources unavailable");
    return snapshot_->identity;
}
const std::string& ProceduralSourcesV1::Borrow::folder()const {
    if(!snapshot_)throw std::logic_error("Procedural sources unavailable");
    return snapshot_->folder;
}
const std::string& ProceduralSourcesV1::Borrow::file_list_uri()const {
    if(!snapshot_)throw std::logic_error("Procedural sources unavailable");
    return snapshot_->file_list_uri;
}
const std::vector<std::uint8_t>& ProceduralSourcesV1::Borrow::file_list_bytes()const {
    if(!snapshot_)throw std::logic_error("Procedural sources unavailable");
    return snapshot_->file_list_bytes;
}
const std::vector<std::string>& ProceduralSourcesV1::Borrow::filenames()const {
    if(!snapshot_)throw std::logic_error("Procedural sources unavailable");
    return snapshot_->filenames;
}
const std::vector<XmlDocumentV1::Borrow>& ProceduralSourcesV1::Borrow::documents()const {
    if(!snapshot_)throw std::logic_error("Procedural sources unavailable");
    return snapshot_->documents;
}
const std::vector<ProceduralBlockSourceV1>& ProceduralSourcesV1::Borrow::blocks()const {
    if(!snapshot_)throw std::logic_error("Procedural sources unavailable");
    return snapshot_->blocks;
}
std::uint32_t ProceduralSourcesV1::Borrow::rule_root()const {
    if(!snapshot_)throw std::logic_error("Procedural sources unavailable");
    return snapshot_->root;
}
bool ProceduralSourcesV1::prepare(const assets::ZipAssetPackV1& pack,std::string identity,
                                 const std::string& definition,std::string& error) {
    error.clear();try {
        if(identity.empty()||definition.empty()||identity.find('\0')!=identity.npos||definition.find('\0')!=definition.npos)
            throw std::runtime_error("Missing procedural identity or definition");
        auto next=std::make_shared<Snapshot>();next->identity=std::move(identity);
        std::map<std::string,std::uint32_t> captured;
        auto read=[&](const std::string& authored,std::vector<std::uint8_t>& bytes)->std::string {
            bool found=false;std::string why,key;
            if(!assets::ZipAssetPackV1::key(authored,key,why))throw std::runtime_error(why);
            if(!pack.read(authored,found,bytes,why))throw std::runtime_error("Read failure for "+authored+": "+why);
            if(!found)throw std::runtime_error("Missing procedural dependency: "+authored);
            return key;
        };
        auto document=[&](const std::string& authored,const char* tag)->std::pair<std::uint32_t,std::uint32_t>{
            std::vector<std::uint8_t> bytes;const auto key=read(authored,bytes);std::uint32_t id;
            const auto prior=captured.find(key);
            if(prior==captured.end()){
                if(next->documents.size()>=4096)throw std::runtime_error("Procedural document limit exceeded");
                XmlDocumentV1 xml;std::string why;if(!xml.capture(key,std::move(bytes),why))throw std::runtime_error(why);
                id=static_cast<std::uint32_t>(next->documents.size());next->documents.push_back(xml.borrow());captured.emplace(key,id);
            }else id=prior->second;
            const auto& doc=next->documents.at(id);
            // Original caller chooses Child(tag,0) without checking parse bool.
            for(auto root:doc.roots())if(doc.elements().at(root).tag==tag)return {id,root};
            throw std::runtime_error("Procedural root "+std::string(tag)+" absent in "+key+": "+doc.diagnostic().message);
        };
        // Original LoadRuleFile rfind('/') only: bare names gain data/scene/.
        const auto definition_uri=definition.find('/')==definition.npos?"data/scene/"+definition:definition;
        const auto rule=document(definition_uri,"rules");next->root=rule.second;
        const auto& element=next->documents.at(rule.first).elements().at(rule.second);
        if(const auto* folder=element.attribute("folder"))next->folder=*folder;
        next->file_list_uri=read(next->folder+"/mgx/mgxlist.txt",next->file_list_bytes);
        const auto& raw=next->file_list_bytes;
        next->filenames=procedural_file_list_v1(std::string_view(raw.empty()?"":reinterpret_cast<const char*>(raw.data()),raw.size()));
        for(const auto& filename:next->filenames){
            // Original LoadBlocks uses strstr, rather than an extension check.
            if(filename.find(".mgx")==filename.npos)continue;
            const auto block=document(next->folder+"/mgx/"+filename,"Module");
            std::string name=filename;name.erase(name.rfind(".mgx"),4);
            next->blocks.push_back({filename,std::move(name),block.first,block.second});
        }
        snapshot_=std::move(next);return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
}
