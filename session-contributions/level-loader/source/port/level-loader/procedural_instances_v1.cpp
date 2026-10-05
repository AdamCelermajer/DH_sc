#include "procedural_instances_v1.hpp"
#include <stdexcept>
namespace dh2::loader {
bool initialize_procedural_rule_instance_v1(
    std::shared_ptr<const ProceduralRulePlanV1> plan,const std::vector<std::uint32_t>& path,
    bool parent,ProceduralRandomV1& random,ProceduralRuleInstanceV1& output,std::string& error) {
    try {
        if(!plan||!plan->lists.document)throw std::runtime_error("Runtime rule source plan unavailable");
        const auto* rule=&plan->rule;
        for(const auto child:path) {
            if(child>=rule->children.size())throw std::runtime_error("Runtime rule source path unavailable");
            rule=&rule->children[child];
        }
        ProceduralRuleInstanceV1 next;next.source=std::move(plan);next.source_path=path;next.kind=rule->kind;
        next.parent_present=parent&&rule->kind!=ProceduralRuleKindV1::root;
        if(next.parent_present) {
            constexpr const char* directions[]{"north","east","south","west"};
            for(std::int32_t i=0;i<4;++i)if(rule->exit==directions[i])next.exit_direction=i;
        }
        auto candidate_random=random;
        switch(rule->kind) {
        case ProceduralRuleKindV1::root:
            next.length=0;next.block_names=rule->block_names;break;
        case ProceduralRuleKindV1::force_block:
            next.block_names=rule->block_names;break;
        case ProceduralRuleKindV1::end_path:
            break;
        case ProceduralRuleKindV1::path:
            if(!rule->id_hash)next.length=candidate_random.between(rule->length[0],rule->length[1]);
            else {
                bool found=false;
                for(const auto& pool:next.source->pools) {
                    for(const auto& element:pool.elements)if(element.id_hash==rule->id_hash) {
                        next.length=element.sizes[2];found=true;break;
                    }
                    if(found)break;
                }
                // A nonzero ID with no matching pool leaves the base length 1.
            }
            break;
        default:throw std::runtime_error("Unknown runtime rule kind");
        }
        output=std::move(next);random=candidate_random;error.clear();return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
bool initialize_procedural_rule_instance_v1(
    ProceduralRulesV1::Borrow source,const std::vector<std::uint32_t>& path,
    bool parent,ProceduralRandomV1& random,ProceduralRuleInstanceV1& output,std::string& error) {
    try {
        if(!source)throw std::runtime_error("Runtime rule source owner unavailable");
        auto owner=std::make_shared<ProceduralRulesV1::Borrow>(source);
        std::shared_ptr<const ProceduralRulePlanV1> plan(owner,&owner->plan());
        auto candidate_random=random;ProceduralRuleInstanceV1 next;
        if(!initialize_procedural_rule_instance_v1(std::move(plan),path,parent,candidate_random,next,error))return false;
        next.source_owner=std::move(source);output=std::move(next);random=candidate_random;error.clear();return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
}
