#include "procedural_rules_v1.hpp"
#include "procedural_random_v1.hpp"
#include <algorithm>
#include <cerrno>
#include <cstdlib>
#include <map>
#include <set>
#include <stdexcept>
namespace dh2::loader {
namespace {
std::string checked(const std::string& s) {
    if(s.size()>4096||s.find('\0')!=std::string::npos)throw std::runtime_error("Rule string outside checked domain");
    return s;
}
std::string lower(std::string s) {
    for(auto& c:s) {
        if(static_cast<unsigned char>(c)>=128)throw std::runtime_error("Rule lowercase outside verified ASCII domain");
        if(c>='A'&&c<='Z')c=static_cast<char>(c+32);
    }
    return s;
}
std::string lookup(const std::string& s) {
    if(s.size()>511)throw std::runtime_error("Rule lookup exceeds original 512-byte buffer");
    return lower(s);
}
std::int32_t integer(const std::string& s,bool* queried=nullptr) {
    checked(s);char* end=nullptr;errno=0;const auto number=std::strtol(s.c_str(),&end,10);
    if(queried)*queried=end!=s.c_str();
    if(end==s.c_str())return 0;
    if(errno==ERANGE||number<INT32_MIN||number>INT32_MAX)throw std::runtime_error("Rule integer outside int32 domain");
    return static_cast<std::int32_t>(number);
}
struct Reader {
    ProceduralRulePlanV1& plan;
    const std::vector<XmlElementV1>& nodes;
    std::set<std::string> blocks;
    std::map<std::string,std::int32_t> lists;
    std::size_t count{};
    ProceduralRuleV1 read(std::uint32_t index,ProceduralRuleKindV1 kind,unsigned depth) {
        if(++count>4096||depth>256)throw std::runtime_error("Rule tree capacity exceeded");
        ProceduralRuleV1 rule;rule.kind=kind;rule.element=index;
        if(index==UINT32_MAX)return rule;
        const auto& node=nodes.at(index);
        if(kind==ProceduralRuleKindV1::path) {
            if(const auto* s=node.attribute("dontGoBack")) {
                bool queried=false;const auto v=integer(*s,&queried);if(queried)rule.dont_go_back=v!=0;
            }
            if(const auto* s=node.attribute("length")) {
                const auto text=checked(*s);rule.length[0]=rule.length[1]=integer(text);
                const auto comma=text.find(',');if(comma!=std::string::npos)rule.length[1]=integer(text.substr(comma+1));
            }
        }
        // ForceBlock reads connectFrom but discards the returned attribute;
        // its constructor's direction 4 stays unchanged.
        const auto* raw_name=node.attribute("name");if(!raw_name)return rule;
        const auto name=checked(*raw_name);
        if(!name.empty()&&name[0]=='#') {
            rule.list_reference=true;const auto open=name.find('[');
            if(open==std::string::npos)rule.list_name=name.substr(1);
            else {
                const auto close=name.find(']');
                // Original unsigned substring count includes the closing bracket
                // and clamps absent/backward brackets to the remaining string.
                const auto last=close==std::string::npos?UINT32_MAX:static_cast<std::uint32_t>(close);
                const auto length=std::uint32_t(1)-static_cast<std::uint32_t>(open)+last;
                rule.list_index=integer(name.substr(open+1,length));
                rule.list_name=name.substr(1,open-1);
            }
            rule.list_validation=lists.count(lookup(rule.list_name))!=0;
            const auto found=lists.find(rule.list_name);if(found!=lists.end())rule.list_source=found->second;
        }else {
            if(name.find(',')!=std::string::npos)throw std::runtime_error("original explicit-block comma loop does not advance");
            rule.block_names.push_back(name);rule.block_validation.push_back(blocks.count(lookup(name))!=0);
        }
        if(const auto* s=node.attribute("exit"))rule.exit=checked(*s);
        if(const auto* s=node.attribute("id"))rule.id_hash=ProceduralRandomV1::hash(checked(*s));
        if(node.first_child_is_non_element)throw std::runtime_error("original unfiltered rule child is non-element");
        bool children_ok=true;
        for(const auto child:node.children) {
            if(rule.children.size()>=16)throw std::runtime_error("original sixteen-child capacity exceeded");
            const auto& c=nodes.at(child);const auto tag=lower(c.tag);ProceduralRuleKindV1 child_kind;
            if(tag=="path")child_kind=ProceduralRuleKindV1::path;
            else if(tag=="forceblock")child_kind=ProceduralRuleKindV1::force_block;
            else if(tag=="endpath")child_kind=ProceduralRuleKindV1::end_path;
            else throw std::runtime_error("original unknown/non-element child would dereference null");
            auto parsed=read(child,child_kind,depth+1);children_ok=children_ok&&parsed.read_result;
            rule.children.push_back(std::move(parsed));
            if(c.next_sibling_is_non_element)throw std::runtime_error("original unfiltered rule child is non-element");
        }
        if(!children_ok)return rule;
        if(rule.id_hash&&rule.length[0]>=0&&rule.length[1]>=0) {
            for(auto& pool:plan.pools) {
                for(auto& e:pool.elements)if(e.id_hash==rule.id_hash) {
                    e.sizes={rule.length[0],std::max(rule.length[0],rule.length[1]),rule.length[0]};break;
                }
            }
        }
        rule.read_result=true;return rule;
    }
};
}
bool interpret_procedural_rules_v1(const ProceduralListPlanV1& lists,const std::vector<std::string>& block_names,
                                  ProceduralRulePlanV1& output,std::string& error) {
    error.clear();
    try {
        if(!lists.document||lists.root>=lists.document.elements().size())throw std::runtime_error("Rule source root unavailable");
        if(block_names.size()>4096)throw std::runtime_error("Rule block-key capacity exceeded");
        ProceduralRulePlanV1 next;next.lists=lists;const auto& nodes=lists.document.elements();
        Reader reader{next,nodes,{},{},0};
        for(const auto& name:block_names)reader.blocks.insert(checked(name));
        for(const auto i:lists.selected_sources) {
            const auto& d=lists.declarations.at(i);reader.lists.emplace(checked(d.name),static_cast<std::int32_t>(i));
        }
        std::size_t elements=0;std::uint32_t root=UINT32_MAX;
        for(const auto index:nodes[lists.root].children) {
            const auto& node=nodes.at(index);
            if(node.tag=="RootRule"&&root==UINT32_MAX)root=index;
            if(node.tag!="pool")continue;
            if(next.pools.size()>=4096)throw std::runtime_error("Room pool capacity exceeded");
            ProceduralRoomPoolV1 pool;pool.element=index;const auto* size=node.attribute("size");
            if(!size)throw std::runtime_error("original atoi receives null input");
            pool.size=integer(*size);
            for(const auto child:node.children) {
                const auto& c=nodes.at(child);if(c.tag!="elem")continue;
                if(++elements>65536)throw std::runtime_error("Room pool element capacity exceeded");
                ProceduralPoolElementV1 e;e.element=child;
                if(const auto* s=c.attribute("id"))e.id_hash=ProceduralRandomV1::hash(checked(*s));
                if(const auto* s=c.attribute("recursive"))e.recursive=*s=="true";
                pool.elements.push_back(std::move(e));
            }
            next.pools.push_back(std::move(pool));
        }
        next.root_present=root!=UINT32_MAX;next.rule=reader.read(root,ProceduralRuleKindV1::root,0);
        output=std::move(next);return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
struct ProceduralRulesV1::Snapshot {
    ProceduralListsV1::Borrow lists;
    ProceduralRulePlanV1 plan;
};
const ProceduralListsV1::Borrow& ProceduralRulesV1::Borrow::lists()const {
    if(!snapshot_)throw std::logic_error("Procedural rule snapshot unavailable");
    return snapshot_->lists;
}
const ProceduralRulePlanV1& ProceduralRulesV1::Borrow::plan()const {
    if(!snapshot_)throw std::logic_error("Procedural rule snapshot unavailable");
    return snapshot_->plan;
}
bool ProceduralRulesV1::prepare(ProceduralListsV1::Borrow lists,std::string& error) {
    error.clear();
    try {
        if(!lists)throw std::runtime_error("Procedural list plan unavailable");
        auto next=std::make_shared<Snapshot>();next->lists=std::move(lists);
        const auto& connections=next->lists.connections();const auto& sources=connections.blocks().sources();
        std::vector<std::string> names;
        for(const auto i:connections.graph().selected_sources)names.push_back(sources.blocks().at(i).name);
        std::string detail;
        if(!interpret_procedural_rules_v1(next->lists.plan(),names,next->plan,detail))
            throw std::runtime_error(next->lists.plan().document.uri()+": "+detail);
        snapshot_=std::move(next);return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
}
