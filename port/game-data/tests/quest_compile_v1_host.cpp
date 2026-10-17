#include "../quest_compile_v1.hpp"
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace qc=dh2::data::quest_compile_v1;
namespace qi=dh2::data::quest_instance_v1;
namespace of=dh2::data::quest_objective_factory_v1;
namespace rf=dh2::data::quest_reward_factory_v1;
namespace ol=dh2::data::quest_objective_list_v1;
namespace rl=dh2::data::quest_reward_list_v1;
namespace qt=dh2::data::quest_table_bindings_v1;
namespace qs=dh2::data::quest_savegame_v1;
using Word=std::uint32_t;
using Row=std::array<std::int32_t,17>;
std::vector<std::uint8_t> bytes(const std::string& path){std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error(path);return {std::istreambuf_iterator<char>(f),{}};}
struct Fixture {
    Row row;qi::Instance instance;std::array<std::unique_ptr<of::Record>,5> objectives;std::array<std::unique_ptr<rf::Record>,3> rewards;
    std::array<ol::Array,2> oa;std::array<rl::Array,2> ra;
    std::array<std::int32_t,3> acts{{1,1,1}};qs::QuestSavegame log{acts};
    qc::Runtime runtime;qc::LogRuntime logs;std::vector<Word> events;
    std::uint32_t calls=0,difficulties=0;bool changed=false;
    Fixture(Row input,qt::View view):row(input),instance(0x10001000,view,{}),runtime(instance,view,{this,resolve_objective,resolve_reward,member}),logs(log,{this,difficulty,resolve_quest}){
        auto& q=instance.record();q.state_0=row[1];q.byte_5c=std::uint8_t(row[2]);q.py_data_68=view.row(0);
        for(std::uint32_t i=0;i<objectives.size();++i){auto p=std::make_unique<of::Record>(0x10010000+i*0x100);p->done_14=std::uint8_t(19+i);p->compiled_8=std::uint8_t(29+i);p->quantity_20=100+i;objectives[i]=std::move(p);}
        q.action_18=row[12]==1?nullptr:&objectives[0]->ref.action;q.action_1c=row[12]==2?nullptr:&objectives[1]->ref.action;
        for(auto& array:oa)for(std::uint32_t i=0;i<3;++i)array.slots.push_back(&objectives[2+i]->ref);
        std::reverse(oa[1].slots.begin(),oa[1].slots.end());
        instance.objectives().count_0=row[3];instance.objectives().children_4=row[13]&1?nullptr:&oa[0];
        if(row[12]==3)oa[0].slots[0]=nullptr;
        for(std::uint32_t i=0;i<rewards.size();++i){auto p=std::make_unique<rf::Record>(0x10015000+i*0x100);p->compiled_8=std::uint8_t(39+i);p->dispatch_0=rf::Dispatch::gold;p->fields.py_data_c={view,view.list(*view.row(0),2),i};rewards[i]=std::move(p);}
        for(auto& array:ra)for(auto& p:rewards)array.slots.push_back(&p->ref);
        std::reverse(ra[1].slots.begin(),ra[1].slots.end());
        instance.rewards().count_0=row[4];instance.rewards().children_4=row[13]&2?nullptr:&ra[0];instance.rewards().text_8="old";
        if(row[12]==4)ra[0].slots[0]=nullptr;
        for(std::size_t i=0;i<3;++i){log.byte_28[i]=std::uint8_t(row[10]);for(int j=0;j<row[9]+int(i);++j)log.quests[i].push_back(&q.ref);}log.character_5c=0x10019000;
    }
    static of::Record* resolve_objective(void* raw,qc::ActionRef* ref){auto& self=*static_cast<Fixture*>(raw);for(auto& p:self.objectives)if(&p->ref.action==ref)return p.get();return nullptr;}
    static rf::Record* resolve_reward(void* raw,rl::RewardRef* ref){auto& self=*static_cast<Fixture*>(raw);for(auto& p:self.rewards)if(&p->ref==ref)return p.get();return nullptr;}
    std::uint32_t oid(const qc::ActionRef* ref)const{for(std::uint32_t i=0;i<objectives.size();++i)if(&objectives[i]->ref.action==ref)return i+1;return 0;}
    static std::int32_t member(void* raw,of::Record& object,const qc::MemberCall& call){
        auto& self=*static_cast<Fixture*>(raw);const auto id=self.oid(&object.ref.action);
        self.events.insert(self.events.end(),{1,id,call.function,Word(call.adjustment),Word(call.marker_priority),Word(call.quest_state)});++self.calls;
        const bool fail=self.row[6]>0&&self.calls==Word(self.row[6]);
        if(fail&&self.row[7]){object.quantity_20=0xf0000000+self.calls;object.done_14=5;}
        if(fail){if(self.row[8])throw std::runtime_error("provider");return 1;}
        if(call.is_virtual&&call.function==8){object.compiled_8=1;object.done_14=1;}
        if(!self.changed){self.changed=true;auto& q=self.instance.record();switch(self.row[5]){
            case 1:self.instance.objectives().count_0=0;break;
            case 2:self.instance.objectives().children_4=&self.oa[1];break;
            case 3:q.action_18=&self.objectives[3]->ref.action;q.action_1c=&self.objectives[4]->ref.action;break;
            case 4:q.state_0=self.row[14];break;
            case 5:q.byte_5c=q.byte_5c?0:1;break;
            case 6:self.instance.rewards().count_0=0;break;
            case 7:self.instance.rewards().children_4=&self.ra[1];break;
            case 8:self.log.quests[0].clear();break;
        }}return 0;
    }
    static std::int32_t difficulty(void* raw,std::uintptr_t character,std::int32_t* out){
        auto& self=*static_cast<Fixture*>(raw);if(character!=self.log.character_5c)return 1;
        *out=self.row[11]==3?std::int32_t((self.difficulties++)%3):self.row[11];
        self.events.insert(self.events.end(),{2,Word(*out),0,0,0,0});return 0;
    }
    static qc::Runtime* resolve_quest(void* raw,qs::QuestRef* ref){auto& self=*static_cast<Fixture*>(raw);return ref==&self.instance.record().ref?&self.runtime:nullptr;}
    std::vector<Word> execute(){
        qc::Result result;qc::Status status;
        switch(row[0]){
            case 0:status=runtime.compile(&result);break;
            case 1:status=runtime.invalidate_objectives(&result);break;
            case 2:status=runtime.compile_objectives(&result);break;
            case 3:status=runtime.register_objectives(&result);break;
            case 4:status=runtime.install_markers(7,-9,&result);break;
            case 5:status=runtime.invalidate_rewards(&result);break;
            case 6:status=runtime.compile_rewards(&result);break;
            case 7:status=logs.compile_quests(bool(row[15]),&result);break;
            default:status=runtime.loop_objectives(0x18,row[16],&result);break;
        }
        const auto& q=instance.record();std::vector<Word> out{Word(status!=qc::Status::complete),Word(q.state_0),q.byte_5c,oid(q.action_18),oid(q.action_1c),Word(instance.objectives().count_0),instance.objectives().children_4==&oa[0]?1u:instance.objectives().children_4==&oa[1]?2u:0u,Word(instance.rewards().count_0),instance.rewards().children_4==&ra[0]?1u:instance.rewards().children_4==&ra[1]?2u:0u,Word(instance.rewards().text_8.empty())};
        for(auto& p:objectives)out.insert(out.end(),{p->done_14,p->compiled_8,p->quantity_20});
        for(auto& p:rewards)out.insert(out.end(),{p->compiled_8,Word(bool(p->compiled_py_data_14.list))});
        for(auto value:log.byte_28)out.push_back(value);
        for(auto& v:log.quests)out.push_back(Word(v.size()));
        out.push_back(Word(events.size()/6));out.insert(out.end(),events.begin(),events.end());return out;
    }
};
int main(int argc,char** argv){try{
    if(argc!=4)return 2;
    const std::string cache=argv[3];
    auto data=std::make_shared<std::vector<std::uint8_t>>(bytes(cache+"/v2quests_pyarray.bin"));auto names=std::make_shared<std::vector<std::uint8_t>>(bytes(cache+"/v2quests_pyarraynames.bin"));dh2_quest_table table{};
    if(dh2_quests_open(&table,data->data(),std::uint32_t(data->size())))throw std::runtime_error("table");
    qt::Owner owner;std::string error;if(!owner.load({table,data,names->data(),names->size(),names},error))throw std::runtime_error(error);
    std::ifstream input(argv[1],std::ios::binary);std::ofstream output(argv[2],std::ios::binary);Word count=0;input.read(reinterpret_cast<char*>(&count),4);
    for(Word i=0;i<count;++i){Row row{};input.read(reinterpret_cast<char*>(row.data()),sizeof(row));Fixture fixture(row,owner.borrow());auto out=fixture.execute();Word size=Word(out.size());output.write(reinterpret_cast<char*>(&size),4);output.write(reinterpret_cast<char*>(out.data()),std::streamsize(out.size()*4));}
    std::uint32_t checks=0;Row row{};row[3]=row[4]=3;row[9]=1;
    {Fixture f(row,owner.borrow());qc::Result result;qc::Runtime missing(f.instance,owner.borrow(),{});if(missing.compile(&result)!=qc::Status::service_unavailable)throw std::runtime_error("missing");++checks;}
    {Fixture f(row,owner.borrow());if(f.runtime.compile(nullptr)!=qc::Status::invalid_argument)throw std::runtime_error("null output");++checks;}
    {Fixture f(row,owner.borrow());auto* out=reinterpret_cast<qc::Result*>(&f.instance.record());if(f.runtime.compile(out)!=qc::Status::invalid_argument)throw std::runtime_error("output alias");++checks;}
    {Fixture f(row,owner.borrow());qc::LogRuntime missing(f.log,{});qc::Result result;if(missing.compile_quests(true,&result)!=qc::Status::service_unavailable)throw std::runtime_error("difficulty");++checks;}
    {Fixture f(row,owner.borrow());f.row[6]=1;qc::Result result;if(f.logs.compile_quests(true,&result)==qc::Status::complete||f.log.byte_28[0]!=1)throw std::runtime_error("prefix flag");++checks;}
    {Fixture f(row,owner.borrow());qc::Result result;qc::Runtime missing(f.instance,owner.borrow(),{&f,Fixture::resolve_objective,Fixture::resolve_reward,nullptr});if(missing.compile(&result)!=qc::Status::service_unavailable||!f.instance.rewards().text_8.empty()||f.objectives[0]->done_14||f.objectives[4]->compiled_8)throw std::runtime_error("missing member prefix");++checks;}
    {Fixture f(row,owner.borrow());qc::Result result;qc::Runtime missing(f.instance,owner.borrow(),{&f,Fixture::resolve_objective,nullptr,Fixture::member});if(missing.compile(&result)!=qc::Status::service_unavailable||!f.instance.rewards().text_8.empty()||f.objectives[1]->done_14||!f.rewards[0]->compiled_8)throw std::runtime_error("missing reward prefix");++checks;}
    for(std::size_t i=0;i<5;++i){Fixture f(row,owner.borrow());auto* out=reinterpret_cast<qc::Result*>(f.objectives[i].get());const auto done=f.objectives[i]->done_14,compiled=f.objectives[i]->compiled_8;if(f.runtime.compile(out)!=qc::Status::invalid_argument||f.objectives[i]->done_14!=done||f.objectives[i]->compiled_8!=compiled)throw std::runtime_error("objective alias");++checks;}
    for(std::size_t i=0;i<3;++i){Fixture f(row,owner.borrow());auto* out=reinterpret_cast<qc::Result*>(f.rewards[i].get());const auto compiled=f.rewards[i]->compiled_8;if(f.runtime.compile(out)!=qc::Status::invalid_argument||f.rewards[i]->compiled_8!=compiled)throw std::runtime_error("reward alias");++checks;}
    {Fixture f(row,owner.borrow());qc::Result result;qc::Services services{&f,Fixture::resolve_objective,Fixture::resolve_reward,[](void*,of::Record&,const qc::MemberCall&)->std::int32_t{return 0;}};
        qc::Runtime runtime(f.instance,owner.borrow(),services);f.objectives[0]->ref.action.character_10=nullptr;if(runtime.compile(&result)!=qc::Status::projection_changed)throw std::runtime_error("projection");++checks;}
    {Fixture f(row,owner.borrow());qc::Result result;qc::Runtime runtime(f.instance,owner.borrow(),{&f,[](void*,qc::ActionRef*)->of::Record*{throw std::runtime_error("resolver");},Fixture::resolve_reward,Fixture::member});if(runtime.compile(&result)!=qc::Status::service_failed||f.objectives[0]->done_14!=19)throw std::runtime_error("resolver exception");++checks;}
    {Fixture f(row,owner.borrow());qc::Result result;f.log.byte_28[0]=1;qc::LogRuntime runtime(f.log,{&f,Fixture::difficulty,nullptr});if(runtime.compile_quests(false,&result)!=qc::Status::complete||runtime.compile_quests(true,&result)!=qc::Status::service_unavailable)throw std::runtime_error("force fresh continuation");++checks;}
    {Fixture f(row,owner.borrow());qc::Result result;const auto view=owner.borrow();const qt::PyDataRef* priority_one=nullptr;for(std::uint32_t i=0;i<view.count();++i)if(view.record(*view.row(i))->priority==1){priority_one=view.row(i);break;}
        f.instance.record().state_0=3;f.instance.record().py_data_68=priority_one;if(!priority_one||f.runtime.compile(&result)!=qc::Status::complete||f.events[f.events.size()-2]!=1||f.events.back()!=3)throw std::runtime_error("actual priority1");++checks;}
    {Fixture f(row,owner.borrow());struct Context{Fixture* fixture;qc::Runtime* runtime=nullptr;bool rejected=false;};Context context{&f};
        qc::Services services{&context,[](void* raw,qc::ActionRef* ref){return Fixture::resolve_objective(static_cast<Context*>(raw)->fixture,ref);},[](void* raw,rl::RewardRef* ref){return Fixture::resolve_reward(static_cast<Context*>(raw)->fixture,ref);},[](void* raw,of::Record& q,const qc::MemberCall& call)->std::int32_t{auto& c=*static_cast<Context*>(raw);qc::Result inner;c.rejected=c.runtime->compile(&inner)==qc::Status::reentrant;return Fixture::member(c.fixture,q,call);}};
        qc::Runtime runtime(f.instance,owner.borrow(),services);context.runtime=&runtime;qc::Result result;if(runtime.compile(&result)!=qc::Status::complete||!context.rejected)throw std::runtime_error("reentry");++checks;}
    {Fixture f(row,owner.borrow());struct Context{Fixture* fixture;qc::Runtime* runtime=nullptr;qc::LogRuntime* logs=nullptr;bool continued=false;};Context context{&f};
        qc::Services services{&context,[](void* raw,qc::ActionRef* ref){return Fixture::resolve_objective(static_cast<Context*>(raw)->fixture,ref);},[](void* raw,rl::RewardRef* ref){return Fixture::resolve_reward(static_cast<Context*>(raw)->fixture,ref);},[](void* raw,of::Record& q,const qc::MemberCall& call)->std::int32_t{auto& c=*static_cast<Context*>(raw);qc::Result inner;c.continued=c.logs->compile_quests(false,&inner)==qc::Status::complete&&inner.service_calls==1;return Fixture::member(c.fixture,q,call);}};
        qc::Runtime runtime(f.instance,owner.borrow(),services);context.runtime=&runtime;
        qc::LogRuntime logs(f.log,{&context,[](void* raw,std::uintptr_t character,std::int32_t* out){return Fixture::difficulty(static_cast<Context*>(raw)->fixture,character,out);},[](void* raw,qs::QuestRef* ref){auto& c=*static_cast<Context*>(raw);return ref==&c.fixture->instance.record().ref?c.runtime:nullptr;}});context.logs=&logs;
        qc::Result result;if(logs.compile_quests(true,&result)!=qc::Status::complete||!context.continued||f.log.byte_28[0]!=1)throw std::runtime_error("marked nested lookup");++checks;}
    std::cout<<checks;return 0;
}catch(const std::exception& e){std::cerr<<e.what();return 1;}}
