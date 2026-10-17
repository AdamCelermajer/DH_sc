#include "../quest_savegame_v1.hpp"
#include "../player_savegame_v1.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace dh2::data::quest_savegame_v1;
namespace {
struct Object {QuestFields fields;QuestRef ref;int phase=0;explicit Object(std::uintptr_t id):ref{id,&fields}{fields.row_8=-858993460;fields.definition_name_14=0xcccccccc;fields.character_60=0xcccccccc;}};
struct Fixture {
    std::array<std::int32_t,3> acts{};QuestSavegame save{acts};std::unique_ptr<Runtime> runtime;
    std::map<std::uintptr_t,std::unique_ptr<Object>> objects;
    std::vector<std::array<long long,4>> trace;
    int count[3]{},difficulty=0,mutation=0,fail_op=-1,fail_at=1,fail_seen=0,throwing=0;
    int allocated=0,reinit_calls=0;std::uintptr_t table=0x10030000;
    bool reenter=false;Status nested=Status::complete;
    Object& object(std::uintptr_t id){return *objects.at(id);}
    int event(int op,long long a=0,long long b=0,long long c=0){
        trace.push_back({op,a,b,c});
        if(op==fail_op&&++fail_seen==fail_at){if(throwing)throw std::runtime_error("declared provider failure");return 1;}
        if(reenter){reenter=false;Result result;nested=runtime->init_quests(&result);}
        return 0;
    }
    Object& insert(std::uintptr_t id){auto p=std::make_unique<Object>(id);auto* raw=p.get();objects.emplace(id,std::move(p));return *raw;}
    Services services(){
        Services s;s.context=this;
        s.table_count=[](void* raw,std::uint32_t* value){auto& f=*static_cast<Fixture*>(raw);const auto d=f.difficulty++;const int count=f.count[d];if(f.event(0,d,count))return 1;*value=std::uint32_t(count);return 0;};
        s.table_rows=[](void* raw,std::uintptr_t* value){auto& f=*static_cast<Fixture*>(raw);if(f.event(1,f.table))return 1;*value=f.table;return 0;};
        s.allocate=[](void* raw,std::uint32_t bytes,std::uint32_t tag,std::uintptr_t* value){auto& f=*static_cast<Fixture*>(raw);const auto id=0x10006000u+std::uint32_t(f.allocated)*0x100u;if(f.event(2,bytes,tag,id))return 1;++f.allocated;f.insert(id);*value=id;return 0;};
        s.quest_construct=[](void* raw,std::uintptr_t id,std::int32_t difficulty,QuestRef** value){auto& f=*static_cast<Fixture*>(raw);if(f.event(3,id,difficulty))return 1;auto& q=f.object(id);q.phase=1;q.fields.row_8=-1;q.fields.definition_name_14=0;if(f.mutation==1)f.save.character_5c=200u+unsigned(f.allocated);*value=&q.ref;return 0;};
        s.owner_children=[](void* raw,QuestRef* ref){auto& f=*static_cast<Fixture*>(raw);if(f.event(4,ref->identity,ref->fields->character_60))return 1;f.object(ref->identity).phase=2;if(f.mutation==2)f.save.character_5c+=7;if(f.mutation==6){ref->fields->row_8=999;ref->fields->definition_name_14=999;}return 0;};
        s.definition_name=[](void* raw,std::uint32_t row,std::uintptr_t* value){auto& f=*static_cast<Fixture*>(raw);const auto name=0x10020000u+row*0x20u;if(f.event(5,row,name))return 1;*value=name;if(f.mutation==3)f.table+=0x100;return 0;};
        s.assign_pydata=[](void* raw,QuestRef* ref,std::uintptr_t row){auto& f=*static_cast<Fixture*>(raw);if(f.event(6,ref->identity,row))return 1;f.object(ref->identity).phase=3;return 0;};
        s.reinit=[](void* raw,QuestRef* ref){auto& f=*static_cast<Fixture*>(raw);if(f.event(7,ref->identity,ref->fields->row_8,ref->fields->character_60))return 1;f.object(ref->identity).phase=4;if(f.mutation==4&&f.reinit_calls++==0){f.count[1]=1;f.count[2]=0;}return 0;};
        s.quest_destruct=[](void* raw,QuestRef* ref){auto& f=*static_cast<Fixture*>(raw);if(f.event(8,ref->identity))return 1;f.object(ref->identity).phase=5;return 0;};
        s.deallocate=[](void* raw,std::uintptr_t id){auto& f=*static_cast<Fixture*>(raw);if(f.event(9,id))return 1;f.object(id).phase=6;return 0;};
        return s;
    }
    void setup(const std::vector<long long>& row){
        for(unsigned d=0;d<3;++d){count[d]=int(row[1+d]);for(int i=0;i<row[4+d];++i){const auto id=0x10010000u+d*0x1000u+unsigned(i)*0x100u;auto& q=insert(id);q.phase=4;q.fields.row_8=i;q.fields.definition_name_14=0x10020000u+unsigned(i)*0x20u;q.fields.character_60=std::uintptr_t(row[7]);save.quests[d].push_back((int(d)*100+i)==row[9]?nullptr:&q.ref);}}
        save.character_5c=std::uintptr_t(row[7]);mutation=int(row[8]);fail_op=int(row[10]);fail_at=int(row[11]);throwing=int(row[12]);
        for(unsigned i=0;i<3;++i){save.byte_28[i]=std::uint8_t(91+i);save.word_2c[i]=71+int(i);save.word_38[i]=81+int(i);save.word_44[i]=21+int(i);save.word_50[i]=31+int(i);}
        runtime=std::make_unique<Runtime>(save,services());
    }
};
void array(std::ostream& out,const std::vector<long long>& values){out<<'[';for(unsigned i=0;i<values.size();++i){if(i)out<<',';out<<values[i];}out<<']';}
void result(std::ostream& out,const Fixture& f,Status status){
    out<<"{\"status\":"<<unsigned(status)<<",\"character\":"<<f.save.character_5c<<",\"fields\":[";
    for(unsigned i=0;i<3;++i){if(i)out<<',';array(out,{f.save.byte_28[i],f.save.word_2c[i],f.save.word_38[i],f.save.word_44[i],f.save.word_50[i]});}
    out<<"],\"vectors\":[";
    for(unsigned d=0;d<3;++d){if(d)out<<',';std::vector<long long> ids;for(const auto* q:f.save.quests[d])ids.push_back(q?static_cast<long long>(q->identity):0);array(out,ids);}
    out<<"],\"objects\":[";bool comma=false;
    for(const auto& entry:f.objects){if(comma)out<<',';comma=true;const auto& q=*entry.second;array(out,{static_cast<long long>(entry.first),q.fields.row_8,static_cast<long long>(q.fields.definition_name_14),static_cast<long long>(q.fields.character_60),q.phase});}
    out<<"],\"trace\":[";comma=false;
    for(const auto& t:f.trace){if(comma)out<<',';comma=true;array(out,{t[0],t[1],t[2],t[3]});}out<<"]}";
}
unsigned checks=0;
void require(bool value){++checks;if(!value)throw std::runtime_error("native semantic gate failed at "+std::to_string(checks));}
void failure_checks(){
    const std::vector<long long> base{2,3,3,3,0,0,0,123,0,-1,-1,1,0,0};
    {dh2::data::PlayerSavegameV1 actual;
     require(actual.level_name_fields().quest_wordfc==std::array<std::int32_t,3>{{1,1,1}});
     require(actual.level_name_fields().quest_word15c==std::array<std::int32_t,3>{{1,1,1}});
     auto& first=actual.source_quest_log_b8();auto& second=actual.source_quest_log_118();
     actual.set_character(UINT64_C(0x12345678000000a1));
     require(first.character_5c==actual.character()&&second.character_5c==actual.character());
     actual.set_character(0);require(!first.character_5c&&!second.character_5c);
     first.word_44[1]=19;second.word_44[2]=27;
     require(actual.level_name_fields().quest_wordfc[1]==19&&actual.level_name_fields().quest_word15c[2]==27);
     Runtime a(first,{}),b(second,{});Result r;require(a.construct(&r)==Status::complete);require(b.construct(&r)==Status::complete);
     require(actual.level_name_fields().quest_wordfc==std::array<std::int32_t,3>{{1,1,1}}&&actual.level_name_fields().quest_word15c==std::array<std::int32_t,3>{{1,1,1}});
     require(a.construct(reinterpret_cast<Result*>(first.word_44.data()))==Status::invalid_argument);}
    for(int fail=0;fail<8;++fail)for(int occurrence:{1,2,5})for(int throwing:{0,1}){
        if(fail==0&&occurrence==5)continue;
        auto row=base;row[10]=fail;row[11]=occurrence;row[12]=throwing;Fixture f;f.setup(row);Result r;const auto status=f.runtime->init_quests(&r);
        require(status==Status::service_failed);require(f.fail_seen==occurrence);
        std::size_t published=0;for(const auto& vector:f.save.quests)for(const auto* q:vector)if(q)++published;
        require(published==r.published);for(const auto& entry:f.objects){const auto& q=*entry.second;if(q.phase>=2)require(q.fields.character_60==123);if(q.phase>=3)require(q.fields.row_8>=0&&q.fields.definition_name_14!=0);}
    }
    for(int missing=0;missing<8;++missing){
        Fixture f;f.setup(base);auto s=f.services();
        switch(missing){case 0:s.table_count=nullptr;break;case 1:s.table_rows=nullptr;break;case 2:s.allocate=nullptr;break;case 3:s.quest_construct=nullptr;break;case 4:s.owner_children=nullptr;break;case 5:s.definition_name=nullptr;break;case 6:s.assign_pydata=nullptr;break;case 7:s.reinit=nullptr;break;}
        Runtime runtime(f.save,s);Result r;require(runtime.init_quests(&r)==Status::service_unavailable);require(r.published==0);
        if(missing>=3)require(f.objects.size()==1);
        if(missing>=5)require(f.objects.begin()->second->fields.character_60==123);
        if(missing>=6)require(f.objects.begin()->second->fields.row_8==0);
    }
    {Fixture f;f.setup(base);f.reenter=true;Result r;require(f.runtime->init_quests(&r)==Status::complete);require(f.nested==Status::reentrant);require(r.published==9);}
    {Fixture f;f.setup(base);Result r;require(f.runtime->init_quests(&r)==Status::complete);f.difficulty=0;f.trace.clear();Result second;require(f.runtime->init_quests(&second)==Status::complete);require(second.published==0&&second.reinitialized==9);for(const auto& t:f.trace)require(t[0]==0||t[0]==7);}
    {Fixture f;f.setup(base);require(f.runtime->init_quests(reinterpret_cast<Result*>(&f.save))==Status::invalid_argument);require(f.trace.empty()&&f.save.quests[0].empty());}
    {Fixture f;f.setup(base);auto s=f.services();s.quest_construct=[](void* raw,std::uintptr_t id,std::int32_t,QuestRef** out){auto& owner=*static_cast<Fixture*>(raw);auto& q=owner.object(id);q.ref.identity=id+1;*out=&q.ref;return 0;};Runtime runtime(f.save,s);Result r;require(runtime.init_quests(&r)==Status::missing_projection);require(!f.save.quests[0][0]);}
    {Fixture f;f.setup(base);auto s=f.services();s.allocate=[](void*,std::uint32_t,std::uint32_t,std::uintptr_t* out){*out=0;return 0;};Runtime runtime(f.save,s);Result r;require(runtime.init_quests(&r)==Status::source_fault);require(f.save.quests[0].size()==3&&f.objects.empty());}
    {Fixture f;f.setup(base);auto s=f.services();s.quest_construct=[](void* raw,std::uintptr_t id,std::int32_t,QuestRef** out){auto& owner=*static_cast<Fixture*>(raw);auto& q=owner.object(id);q.ref.fields=reinterpret_cast<QuestFields*>(&owner.save);*out=&q.ref;return 0;};Runtime runtime(f.save,s);Result r;require(runtime.init_quests(&r)==Status::invalid_argument);require(f.save.character_5c==123&&f.save.quests[0][0]==nullptr);}
    {auto row=base;row[4]=1;Fixture f;f.setup(row);Result r;require(f.runtime->construct(&r)==Status::invalid_argument);require(f.save.word_2c[0]==71);}
    {Fixture f;f.setup(base);auto s=f.services();s.owner_children=[](void* raw,QuestRef*){auto& f=*static_cast<Fixture*>(raw);f.save.quests[0].resize(1);return 0;};Runtime runtime(f.save,s);Result r;require(runtime.init_quests(&r)==Status::unsafe_storage);require(f.save.quests[0][0]==nullptr&&f.objects.size()==1);}
    for(int failed:{8,9})for(int throwing:{0,1}){
        Fixture f;f.setup(base);Result r;require(f.runtime->init_quests(&r)==Status::complete);f.fail_op=failed;f.fail_at=2;f.throwing=throwing;require(f.runtime->destroy(&r)==Status::service_failed);require(f.save.quests[0][0]==nullptr&&f.save.quests[0][1]!=nullptr);require(f.object(f.save.quests[0][1]->identity).phase==(failed==8?4:5));
    }
}
} // namespace
int main(int argc,char** argv){try{
    if(argc!=2)return 2;
    std::ifstream input(argv[1]);std::string line;bool comma=false;std::cout<<"{\"results\":[";
    while(std::getline(input,line)){std::istringstream stream(line);std::vector<long long> row;long long value=0;while(stream>>value)row.push_back(value);if(row.size()!=14)throw std::runtime_error("bad row");Fixture f;f.setup(row);Result r;Status status;
        if(row[0]<2)status=f.runtime->construct(&r);else if(row[0]==2)status=f.runtime->init_quests(&r);else status=f.runtime->destroy(&r);
        if(row[13]&&status==Status::complete){f.difficulty=0;status=f.runtime->init_quests(&r);}
        if(comma)std::cout<<',';
        comma=true;result(std::cout,f,status);
    }
    failure_checks();dh2::data::PlayerSavegameV1 actual;std::cout<<"],\"failure_checks\":"<<checks<<",\"blank_quest_act_words\":[";
    const auto& names=actual.level_name_fields();array(std::cout,{names.quest_wordfc[0],names.quest_wordfc[1],names.quest_wordfc[2]});std::cout<<',';array(std::cout,{names.quest_word15c[0],names.quest_word15c[1],names.quest_word15c[2]});
    std::cout<<"],\"set_player_projections\":[";bool separator=false;
    for(bool present:{false,true})for(std::uintptr_t identity:{0u,1u,123u,0x7fffffffu,0xffffffffu}){
     dh2::data::PlayerSavegameV1 saved;saved.set_character(99);
     if(present)saved.set_character(identity);
     if(separator)std::cout<<',';
     separator=true;
     array(std::cout,{static_cast<long long>(saved.character()),static_cast<long long>(saved.source_quest_log_b8().character_5c),static_cast<long long>(saved.source_quest_log_118().character_5c)});
    }
    std::cout<<"]}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what();return 1;}}
