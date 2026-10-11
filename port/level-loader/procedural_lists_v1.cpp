#include "procedural_lists_v1.hpp"
#include <cerrno>
#include <cstdlib>
#include <map>
#include <set>
#include <stdexcept>
namespace dh2::loader {
namespace {
std::string lower(std::string name) {
    for(auto& c:name) {
        const auto byte=static_cast<unsigned char>(c);
        if(byte>=128)throw std::runtime_error("Procedural list lowercase outside verified ASCII domain");
        if(c>='A'&&c<='Z')c=static_cast<char>(c+32);
    }
    return name;
}
std::string value(const XmlElementV1& e,const char* name) {
    const auto* s=e.attribute(name);if(!s)return {};
    if(s->size()>4096||s->find('\0')!=std::string::npos)
        throw std::runtime_error("Procedural list string outside checked domain");
    return *s;
}
std::int32_t chances(const XmlElementV1& e) {
    const auto* s=e.attribute("chances");if(!s)return 100;
    if(s->find('\0')!=std::string::npos||s->size()>4096)
        throw std::runtime_error("Procedural chances string outside checked domain");
    char* end=nullptr;errno=0;const auto number=std::strtol(s->c_str(),&end,10);
    if(end==s->c_str())return 100;
    if(errno==ERANGE||number<INT32_MIN||number>INT32_MAX)
        throw std::runtime_error("Procedural chances outside int32 domain");
    return static_cast<std::int32_t>(number);
}
}
bool interpret_procedural_lists_v1(XmlDocumentV1::Borrow document,std::uint32_t root,
                                  const std::vector<std::string>& block_names,
                                  ProceduralListPlanV1& output,std::string& error) {
    error.clear();
    try {
        if(!document||root>=document.elements().size())throw std::runtime_error("Procedural list root unavailable");
        if(block_names.size()>4096)throw std::runtime_error("Procedural block-key capacity exceeded");
        std::set<std::string> known;
        for(const auto& name:block_names) {
            if(name.size()>4096||name.find('\0')!=std::string::npos)
                throw std::runtime_error("Procedural block key outside checked domain");
            known.insert(name);
        }
        ProceduralListPlanV1 next;next.document=std::move(document);next.root=root;
        const auto& nodes=next.document.elements();std::map<std::string,std::uint32_t> selected;
        std::size_t total_elements=0;
        for(const auto li:nodes[root].children) {
            const auto& node=nodes.at(li);if(node.tag!="list")continue;
            if(next.declarations.size()>=4096)throw std::runtime_error("Procedural list capacity exceeded");
            ProceduralListDeclarationV1 list;list.element=li;list.name=value(node,"name");
            list.name=lower(std::move(list.name));
            // Original exact strcmp("true"), default true. "random" is unread.
            if(const auto* s=node.attribute("replacement"))list.replacement=*s=="true";
            for(const auto ei:node.children) {
                const auto& e=nodes.at(ei);if(e.tag!="elem")continue;
                if(++total_elements>65536)throw std::runtime_error("Procedural list element capacity exceeded");
                ProceduralListElementV1 element;element.element=ei;
                element.block_name=value(e,"name");element.gameplay=value(e,"gameplay");element.visual=value(e,"visual");
                element.chances=chances(e);
                if(element.block_name.size()>511)throw std::runtime_error("Procedural block validation exceeds original 512-byte buffer");
                // ValidBlock copies into a 512-byte buffer and lowercases only
                // the query. Block-map keys remain case-sensitive and unchanged.
                element.block_available=known.count(lower(element.block_name))!=0;
                // Invalid references remain visible. Original ValidBlock feeds
                // configurable assertions and does not remove the declaration.
                list.elements.push_back(std::move(element));
            }
            selected.emplace(list.name,static_cast<std::uint32_t>(next.declarations.size()));
            next.declarations.push_back(std::move(list));
        }
        for(const auto& entry:selected)next.selected_sources.push_back(entry.second);
        output=std::move(next);return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
struct ProceduralListsV1::Snapshot {
    ProceduralConnectionsV1::Borrow connections;
    ProceduralListPlanV1 plan;
};
const ProceduralConnectionsV1::Borrow& ProceduralListsV1::Borrow::connections()const {
    if(!snapshot_)throw std::logic_error("Procedural list snapshot unavailable");
    return snapshot_->connections;
}
const ProceduralListPlanV1& ProceduralListsV1::Borrow::plan()const {
    if(!snapshot_)throw std::logic_error("Procedural list snapshot unavailable");
    return snapshot_->plan;
}
bool ProceduralListsV1::prepare(ProceduralConnectionsV1::Borrow connections,std::string& error) {
    error.clear();
    try {
        if(!connections)throw std::runtime_error("Procedural connection plan unavailable");
        auto next=std::make_shared<Snapshot>();next->connections=std::move(connections);
        const auto& sources=next->connections.blocks().sources();std::vector<std::string> names;
        for(const auto i:next->connections.graph().selected_sources)names.push_back(sources.blocks().at(i).name);
        std::string detail;
        if(!interpret_procedural_lists_v1(sources.documents().at(0),sources.rule_root(),names,next->plan,detail))
            throw std::runtime_error(sources.documents().at(0).uri()+": "+detail);
        snapshot_=std::move(next);return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
}
