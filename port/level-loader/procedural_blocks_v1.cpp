#include "procedural_blocks_v1.hpp"
#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
namespace dh2::loader {
namespace {
bool ascii_equal(const std::string& a,const char* b) {
    std::size_t i=0;
    for(;i<a.size()&&b[i];++i) {
        unsigned char c=a[i];if(c>=128)throw std::runtime_error("MGX case comparison outside ASCII domain");
        if(c>='A'&&c<='Z')c+=32;
        if(c!=static_cast<unsigned char>(b[i]))return false;
    }
    return i==a.size()&&!b[i];
}
void query_float(const XmlElementV1& e,const char* name,float& value) {
    if(const auto* s=e.attribute(name)) {
        double parsed{};
        if(std::sscanf(s->c_str(),"%lf",&parsed)==1)value=static_cast<float>(parsed);
    }
}
void query_int(const XmlElementV1& e,const char* name,std::int32_t& value) {
    if(const auto* s=e.attribute(name)) {
        char* end=nullptr;errno=0;const auto v=std::strtol(s->c_str(),&end,10);
        if(end!=s->c_str()) {
            if(errno==ERANGE||v<INT32_MIN||v>INT32_MAX)throw std::runtime_error("MGX integer outside int32 domain");
            value=static_cast<std::int32_t>(v);
        }
    }
}
std::vector<std::string> types(const std::string* source) {
    if(!source)return {};
    std::string s=*source;
    if(s.size()>4096||s.find('\0')!=std::string::npos)throw std::runtime_error("MGX link type outside checked domain");
    const auto first=s.find(' ');
    if(first!=std::string::npos&&first+1<s.size()) {
        auto write=first;
        for(auto read=first+1;read<s.size();++read)if(s[read]!=' ')s[write++]=s[read];
        // Original writes neither a new terminator nor a shorter finish pointer.
    }
    std::vector<std::string> result;std::size_t start=0;
    for(;;) {
        const auto comma=s.find(',',start);
        result.push_back(s.substr(start,comma==std::string::npos?comma:comma-start));
        if(comma==std::string::npos)break;
        start=comma+1;
    }
    return result;
}
std::array<float,3> point(const std::string* s) {
    if(!s||s->size()>255||s->find('\0')!=std::string::npos)
        throw std::runtime_error("MGX exit position outside original StrToObj buffer/input domain");
    std::array<float,3> result{};std::size_t pos=0;
    for(unsigned i=0;i<3;++i) {
        pos=s->find_first_not_of(',',pos);if(pos==std::string::npos)break;
        const auto end=s->find(',',pos);
        const auto token=s->substr(pos,end==std::string::npos?end:end-pos);
        result[i]=static_cast<float>(std::strtod(token.c_str(),nullptr));
        if(!std::isfinite(result[i]))throw std::runtime_error("MGX exit position outside finite domain");
        if(end==std::string::npos)break;
        pos=end+1;
    }
    return result;
}
std::int32_t grid_coordinate(float distance,float unit) {
    const float quotient=distance/unit;
    const float adjusted=std::ceil(quotient)-1.0f;
    if(!std::isfinite(adjusted)||adjusted<-2147483648.0f||adjusted>=2147483648.0f)
        throw std::runtime_error("MGX exit grid outside finite int32 domain");
    const auto value=static_cast<std::int32_t>(adjusted);
    return value<0?0:value;
}
}
bool interpret_procedural_block_v1(XmlDocumentV1::Borrow document,std::uint32_t root,
                                  ProceduralBlockV1& output,std::string& error) {
    error.clear();
    try {
        if(!document||root>=document.elements().size())
            throw std::runtime_error("MGX element root unavailable");
        ProceduralBlockV1 next;next.document=std::move(document);next.root=root;
        const auto& elements=next.document.elements();const auto& module=elements[root];
        query_float(module,"unit_width",next.unit_width);query_float(module,"unit_height",next.unit_height);
        query_int(module,"block_width",next.width);query_int(module,"block_height",next.height);
        if(!std::isfinite(next.unit_width)||!std::isfinite(next.unit_height)||
           next.unit_width==0||next.unit_height==0||next.width==INT32_MIN)
            throw std::runtime_error("MGX dimensions outside checked arithmetic domain");
        bool started=false;
        for(const auto index:module.children) {
            const auto& object=elements.at(index);
            if(!started)started=object.tag=="GameObject";
            if(!started)continue;
            const auto* gametype=object.attribute("gametype");
            if(!gametype)throw std::runtime_error("MGX unfiltered sibling lacks gametype");
            if(ascii_equal(*gametype,"link")) {
                ProceduralLinkDeclarationV1 declaration;declaration.element=index;
                declaration.link_types=types(object.attribute("linktype"));
                const auto* direction=object.attribute("direction");
                if(!direction)declaration.result=ProceduralLinkResultV1::missing_direction;
                else if(declaration.link_types.empty()||declaration.link_types.back().empty()||declaration.link_types.back()=="0")
                    declaration.result=ProceduralLinkResultV1::zero_or_empty_tail;
                else {
                    if(next.exits.size()>=8)throw std::runtime_error("MGX original eight-exit capacity exceeded");
                    const auto p=point(object.attribute("position"));
                    ProceduralExitV1 exit;exit.element=index;exit.index=static_cast<std::uint32_t>(next.exits.size());
                    static const char* names[]={"north","east","south","west"};
                    exit.direction=4;
                    for(unsigned i=0;i<4;++i)if(ascii_equal(*direction,names[i])){exit.direction=i;break;}
                    const float half_height=(static_cast<float>(next.height)*next.unit_height)*0.5f;
                    const float negative_half_width=(static_cast<float>(-next.width)*next.unit_width)*0.5f;
                    exit.grid={grid_coordinate(p[0]-negative_half_width,next.unit_width),
                               grid_coordinate(half_height-p[1],next.unit_height)};
                    exit.height=p[2];exit.link_types=declaration.link_types;
                    declaration.result=ProceduralLinkResultV1::accepted;declaration.exit_index=exit.index;
                    next.exits.push_back(std::move(exit));
                }
                next.link_declarations.push_back(std::move(declaration));
            }
            if(object.next_sibling_is_non_element)
                throw std::runtime_error("MGX original unfiltered traversal encounters non-element sibling");
        }
        output=std::move(next);return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
struct ProceduralBlocksV1::Snapshot {
    ProceduralSourcesV1::Borrow sources;
    std::vector<ProceduralBlockV1> blocks;
};
const ProceduralSourcesV1::Borrow& ProceduralBlocksV1::Borrow::sources()const {
    if(!snapshot_)throw std::logic_error("MGX block snapshot unavailable");
    return snapshot_->sources;
}
const std::vector<ProceduralBlockV1>& ProceduralBlocksV1::Borrow::blocks()const {
    if(!snapshot_)throw std::logic_error("MGX block snapshot unavailable");
    return snapshot_->blocks;
}
bool ProceduralBlocksV1::prepare(ProceduralSourcesV1::Borrow sources,std::string& error) {
    error.clear();
    try {
        if(!sources)throw std::runtime_error("MGX source snapshot unavailable");
        auto next=std::make_shared<Snapshot>();next->sources=std::move(sources);
        for(const auto& source:next->sources.blocks()) {
            ProceduralBlockV1 block;std::string detail;
            if(!interpret_procedural_block_v1(next->sources.documents().at(source.document),source.root,block,detail))
                throw std::runtime_error(next->sources.documents().at(source.document).uri()+": "+detail);
            next->blocks.push_back(std::move(block));
        }
        snapshot_=std::move(next);return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
}
