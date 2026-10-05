#include "fixed_sources_v1.hpp"
#include "cached_level_file_v1.hpp"
#include <functional>
#include <map>
#include <set>
#include <stdexcept>
namespace dh2::loader {
namespace {
// Description discovery only: these callbacks do not create or enable objects.
struct SourceChildrenV1 final:LevelFileWalkServicesV1 {
    std::vector<std::uint32_t> children;
    bool parse_result(bool,std::string&)override{return true;}
    bool load_element(const XmlDocumentV1::Borrow&,std::uint32_t index,std::string&)override {
        children.push_back(index);return true;
    }
    bool release_load_state(std::string&)override{return true;}
};
struct CapturedSourceV1 {
    XmlDocumentV1::Borrow document;
    std::vector<std::uint32_t> selected_children;
};
CapturedSourceV1 capture_source_v1(const assets::ZipAssetPackV1& pack,const std::string& authored,
    const char* root_tag,const XmlDocumentV1::Borrow& derived) {
    SourceChildrenV1 recipient;CapturedSourceV1 captured;std::string error;
    if(derived) {
        // Native generated inspection roots retain their original owner below;
        // this route does not claim original PropertyMap serialization.
        XmlDocumentV1 parser;
        if(!parser.capture_level_buffer(derived.uri(),derived.source(),error))throw std::runtime_error(error);
        captured.document=parser.borrow();LevelFileWalkV1 walk;
        if(!prepare_level_file_walk_v1(captured.document,root_tag,walk,error))throw std::runtime_error(error);
        for(;;) {
            const auto result=step_level_file_walk_v1(walk,recipient);
            if(result==LevelFileWalkStepV1::failed) {
                const auto cause=walk.error;
                if(!discard_level_file_walk_v1(walk,recipient,error))
                    throw std::runtime_error(cause+"; source cleanup failed: "+error);
                throw std::runtime_error(cause);
            }
            if(result==LevelFileWalkStepV1::complete)break;
        }
    }else {
        CachedLevelFileV1 file(pack);
        for(;;) {
            const auto result=file.step(authored,root_tag,recipient);
            if(file.source()&&!captured.document)captured.document=file.source();
            if(result==LevelFileWalkStepV1::failed) {
                const auto cause=file.error();
                if(!file.discard(recipient,error))throw std::runtime_error(cause+"; source cleanup failed: "+error);
                throw std::runtime_error(cause);
            }
            if(result==LevelFileWalkStepV1::complete)break;
        }
    }
    if(!captured.document)throw std::runtime_error("XML source capture unavailable: "+authored);
    captured.selected_children=std::move(recipient.children);return captured;
}
}
struct FixedSourcesV1::Snapshot {
    std::string identity;
    std::vector<XmlDocumentV1::Borrow> documents;
    std::vector<ModuleSourceLinksV1> links;
    std::shared_ptr<const void> source_owner;
};
const std::string& FixedSourcesV1::Borrow::identity()const {
    if(!snapshot_)throw std::logic_error("Fixed sources unavailable");
    return snapshot_->identity;
}
const std::vector<XmlDocumentV1::Borrow>& FixedSourcesV1::Borrow::documents()const {
    if(!snapshot_)throw std::logic_error("Fixed sources unavailable");
    return snapshot_->documents;
}
const std::vector<ModuleSourceLinksV1>& FixedSourcesV1::Borrow::module_links()const {
    if(!snapshot_)throw std::logic_error("Fixed sources unavailable");
    return snapshot_->links;
}
bool FixedSourcesV1::prepare(const assets::ZipAssetPackV1& pack,std::string identity,
                            const std::string& definition,std::string& error) {
    return prepare_input(pack,std::move(identity),definition,{},{},error);
}
bool FixedSourcesV1::prepare_document(const assets::ZipAssetPackV1& pack,std::string identity,
    XmlDocumentV1::Borrow document,std::shared_ptr<const void> owner,std::string& error){
    if(!document||!owner){error="Derived root document/source owner unavailable";return false;}
    const auto definition=document.uri();
    return prepare_input(pack,std::move(identity),definition,std::move(document),std::move(owner),error);
}
bool FixedSourcesV1::prepare_input(const assets::ZipAssetPackV1& pack,std::string identity,
    const std::string& definition,XmlDocumentV1::Borrow input,std::shared_ptr<const void> owner,std::string& error){
    error.clear();
    try {
        if(identity.empty()||identity.find('\0')!=std::string::npos||definition.empty())
            throw std::runtime_error("Missing level identity or definition");
        auto next=std::make_shared<Snapshot>();next->identity=std::move(identity);next->source_owner=std::move(owner);
        std::map<std::string,std::uint32_t> captured;
        std::set<std::uint32_t> loading;
        std::function<std::uint32_t(const std::string&,const char*)> load;
        load=[&](const std::string& authored,const char* root_tag)->std::uint32_t {
            const bool derived=input&&authored==definition;
            auto captured_source=capture_source_v1(pack,authored,root_tag,derived?input:XmlDocumentV1::Borrow{});
            const auto& doc=captured_source.document;const auto& resolved=doc.uri();
            if(const auto old=captured.find(resolved);old!=captured.end()) {
                if(loading.count(old->second))throw std::runtime_error("Cyclic module XML reference: "+resolved);
                const auto& prior=next->documents.at(old->second);
                bool matched=false;
                for(auto root:prior.roots())if(prior.elements().at(root).tag==root_tag)matched=true;
                if(!matched)throw std::runtime_error("Referenced XML root mismatch: "+resolved);
                return old->second;
            }
            if(next->documents.size()>=4096)throw std::runtime_error("Source graph exceeds preparation domain");
            if(!doc.parsed())throw std::runtime_error("XML parse failure in "+resolved+": "+doc.diagnostic().message);
            bool recognized=false;
            for(auto root:doc.roots())if(doc.elements().at(root).tag==root_tag)recognized=true;
            if(!recognized)throw std::runtime_error("Required XML root "+std::string(root_tag)+" absent in "+resolved+"; rule generation is not implemented by fixed source preparation");
            const auto index=static_cast<std::uint32_t>(next->documents.size());
            captured.emplace(resolved,index);loading.insert(index);next->documents.push_back(doc);
            const auto& tree=doc.elements();
            for(auto node:captured_source.selected_children) {
                const auto& element=tree.at(node);
                const auto* type=element.attribute("gametype");
                if(!type||*type!="Module")continue;
                const auto* alt_gameplay=element.attribute("alt_mgp");
                const auto* alt_visual=element.attribute("alt_mvp");
                // Original _ChooseXmls only enters alternative selection
                // when both alternative lists are nonempty.
                if(alt_gameplay&&!alt_gameplay->empty()&&alt_visual&&!alt_visual->empty())
                    throw std::runtime_error("Alternate module selection unresolved at "+resolved+":"+std::to_string(node));
                ModuleSourceLinksV1 link;link.document=index;link.element=node;
                const auto link_index=next->links.size();next->links.push_back(link);
                if(const auto* uri=element.attribute("mgp");uri&&!uri->empty())link.gameplay=load(*uri,"Module");
                if(const auto* uri=element.attribute("mvp");uri&&!uri->empty())link.visual=load(*uri,"Module");
                next->links[link_index]=link;
            }
            loading.erase(index);return index;
        };
        load(definition,"Level");snapshot_=std::move(next);return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
}
