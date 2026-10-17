#include "quest_instance_v1.hpp"
#include <utility>

namespace dh2::data::quest_instance_v1 {
namespace scalar=quest_runtime_fields_v1;
namespace condition=quest_condition_list_v1;
namespace objective=quest_objective_list_v1;
namespace reward=quest_reward_list_v1;
namespace {
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn) {
    const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
    return an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y||(an&&bn&&x<y+bn&&y<x+an);
}
template<class Status> std::int32_t delivered(Status status) {return status==Status::complete?0:1;}
}
Instance::Instance(std::uintptr_t identity,quest_table_bindings_v1::View definitions,Services services):
    definitions_(std::move(definitions)),services_(services),record_(identity),
    condition_runtime_(conditions_,services_.conditions),
    objective_runtime_(objectives_,services_.objectives),
    reward_runtime_(rewards_,services_.rewards),runtime_(record_,{this,invoke}) {}

std::int32_t Instance::invoke(void* raw,const scalar::Request& request,scalar::Response* answer) {
    auto& owner=*static_cast<Instance*>(raw);
    if(request.quest!=&owner.record_||!answer)return 1;
    const auto at=[&](std::uint32_t offset){return request.target==owner.record_.ref.identity+offset;};
    condition::Result cr;objective::Result orr;reward::Result rr;
    using Op=scalar::Operation;
    switch(request.operation) {
    case Op::construct_conditions:
        return at(0x20)?delivered(owner.condition_runtime_.construct(&cr)):1;
    case Op::construct_objectives:
        return at(0x2c)?delivered(owner.objective_runtime_.construct(&orr)):1;
    case Op::construct_rewards:
        return at(0x38)?delivered(owner.reward_runtime_.construct(&rr)):1;
    case Op::assign_conditions: {
        const auto* list=owner.definitions_.resolve_list(request.value);
        if(!at(0x20)||!list||list->kind!=0)return 1;
        return delivered(owner.condition_runtime_.assign_pydata({owner.definitions_,list,0},request.count,&cr));
    }
    case Op::assign_objectives: {
        const auto* list=owner.definitions_.resolve_list(request.value);
        if(!at(0x2c)||!list||list->kind!=1)return 1;
        return delivered(owner.objective_runtime_.assign_pydata({owner.definitions_,list,0,nullptr},request.count,&orr));
    }
    case Op::assign_rewards: {
        const auto* list=owner.definitions_.resolve_list(request.value);
        const auto difficulty=owner.record_.difficulty_10;
        if(!at(0x38)||!list||difficulty<0||difficulty>2||list->kind!=std::uint32_t(difficulty+2))return 1;
        return delivered(owner.reward_runtime_.assign_pydata({owner.definitions_,list,0},request.count,&rr));
    }
    case Op::objective_owners:
        return at(0x2c)?delivered(owner.objective_runtime_.set_owner(request.value,&orr)):1;
    case Op::reward_owners:
        return at(0x38)?delivered(owner.reward_runtime_.set_owner(request.value,&rr)):1;
    case Op::create_objective: {
        const auto* stub=owner.definitions_.resolve_stub(request.target);
        if(!stub||stub->row!=request.row||stub->offset!=request.offset)return 1;
        objective::ObjectiveRef* ref=nullptr;
        const auto status=owner.objective_runtime_.create_objective({owner.definitions_,nullptr,0,stub},&ref,&orr);
        if(status!=objective::Status::complete)return 1;
        answer->action=ref?&ref->action:nullptr;return 0;
    }
    case Op::read_py_word: {
        if(!request.row||request.target!=request.row->identity+request.offset)return 1;
        std::string error;
        return owner.definitions_.read_word(*request.row,request.offset,&answer->value,error)?0:1;
    }
    case Op::load_objectives:
        return at(0x2c)&&request.stream?delivered(owner.objective_runtime_.load(*request.stream,&orr)):1;
    case Op::destroy_rewards:
        return at(0x38)?delivered(owner.reward_runtime_.destroy(&rr)):1;
    case Op::destroy_objectives:
        return at(0x2c)?delivered(owner.objective_runtime_.destroy(&orr)):1;
    case Op::destroy_conditions:
        return at(0x20)?delivered(owner.condition_runtime_.destroy(&cr)):1;
    case Op::action_virtual:case Op::remove_markers:case Op::unregister_objectives:case Op::read_stream_word:
        return owner.services_.leaves.invoke?
            owner.services_.leaves.invoke(owner.services_.leaves.context,request,answer):1;
    default:return 1;
    }
}
bool Instance::output(const scalar::Result* result) const noexcept {
    if(!result||reinterpret_cast<std::uintptr_t>(result)%alignof(scalar::Result)||
       overlap(result,sizeof(*result),this,sizeof(*this)))return false;
    const auto separate=[&](const auto* pointer){return !pointer||
        !overlap(result,sizeof(*result),pointer,sizeof(*pointer));};
    const auto array=[&](const auto* pointer){
        if(!pointer)return true;
        if(!separate(pointer)||overlap(result,sizeof(*result),pointer->slots.data(),pointer->slots.size()*sizeof(pointer->slots[0])))return false;
        for(const auto* child:pointer->slots)if(child&&(!separate(child)||!separate(child->fields)))return false;
        return true;
    };
    if(!array(conditions_.children_4)||!array(objectives_.children_4)||!array(rewards_.children_4)||
       overlap(result,sizeof(*result),rewards_.text_8.data(),rewards_.text_8.size()+1))return false;
    for(std::uint32_t i=0;i<definitions_.count();++i){
        const auto* row=definitions_.row(i);
        if(!separate(row))return false;
        for(std::uint32_t kind=0;kind<5;++kind)if(!separate(definitions_.list(*row,kind)))return false;
        if(!separate(definitions_.resolve_stub(row->identity+0x3c))||
           !separate(definitions_.resolve_stub(row->identity+0x68)))return false;
    }
    return true;
}
scalar::Status Instance::construct(std::int32_t difficulty,scalar::Result* out) {
    return output(out)?runtime_.construct(difficulty,out):scalar::Status::invalid_argument;
}
scalar::Status Instance::assign_pydata(std::uintptr_t actual_row,scalar::Result* out) {
    const auto* row=definitions_.resolve(actual_row);
    return output(out)&&row?runtime_.assign_pydata(*row,out):scalar::Status::invalid_argument;
}
scalar::Status Instance::owner_children(scalar::Result* out) {
    return output(out)?runtime_.owner_children(out):scalar::Status::invalid_argument;
}
scalar::Status Instance::reinit(scalar::Result* out) {
    return output(out)?runtime_.reinit(out):scalar::Status::invalid_argument;
}
scalar::Status Instance::load_quest_data(player_saved_quests_v1::StreamRef& stream,std::uint32_t flag,scalar::Result* out) {
    return output(out)?runtime_.load_quest_data(stream,flag,out):scalar::Status::invalid_argument;
}
scalar::Status Instance::destroy(scalar::Result* out) {
    return output(out)?runtime_.destroy(out):scalar::Status::invalid_argument;
}
}
