#include "../quest_condition_factory_v1.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
using namespace dh2::data::quest_condition_factory_v1;
namespace cl=dh2::data::quest_condition_list_v1;
namespace table=dh2::data::quest_table_bindings_v1;
namespace {
unsigned checks=0;
void require(bool value){++checks;if(!value)throw std::runtime_error("condition factory gate failed at "+std::to_string(checks));}
std::shared_ptr<std::vector<std::uint8_t>> file(const std::string& path){
    std::ifstream f(path,std::ios::binary|std::ios::ate);if(!f)throw std::runtime_error("cache missing");
    const auto size=f.tellg();auto data=std::make_shared<std::vector<std::uint8_t>>(std::size_t(size));f.seekg(0);
    if(!f.read(reinterpret_cast<char*>(data->data()),size))throw std::runtime_error("cache read failed");
    return data;
}
table::View definitions(const std::string& cache){
    auto packed=file(cache+"/v2quests_pyarray.bin"),names=file(cache+"/v2quests_pyarraynames.bin");table::Input input;
    require(!dh2_quests_open(&input.table,packed->data(),std::uint32_t(packed->size())));input.packed_owner=packed;
    input.names=names->data();input.names_size=names->size();input.names_owner=names;table::Owner owner;std::string error;require(owner.load(input,error));return owner.borrow();
}
constexpr Dispatch kinds[]{Dispatch::quest_in_state,Dispatch::quest_state_lower,Dispatch::quest_state_higher,Dispatch::player_in_level,Dispatch::level_in_state,Dispatch::player_at_level,Dispatch::event_in_state};
struct Object {Record record;std::uint32_t bytes=0,phase=0;explicit Object(std::uintptr_t id):record(id){record.comparator_8=-858993460;}};
struct Fixture {
    table::View view;std::map<std::uintptr_t,std::unique_ptr<Object>> objects;
    std::map<std::uintptr_t,std::unique_ptr<cl::Array>> arrays;
    cl::List list;std::unique_ptr<Runtime> runtime;
    std::vector<std::array<long long,4>> trace;
    int fail_op=-1,fail_at=1,seen=0,throwing=0,allocated=0,array_allocated=0;
    bool reenter=false;Status nested=Status::complete;
    explicit Fixture(table::View v):view(std::move(v)){}
    cl::Definition definition(unsigned row){return {view,view.list(*view.row(row),0),0};}
    Object& insert(std::uintptr_t id,std::uint32_t bytes){auto q=std::make_unique<Object>(id);auto* raw=q.get();q->bytes=bytes;q->record.fields.py_data_4=definition(0);objects.emplace(id,std::move(q));return *raw;}
    int event(int op,long long a=0,long long b=0,long long c=0){
        trace.push_back({op,a,b,c});if(reenter){reenter=false;Result r;nested=runtime->create(0,&r);}
        if(op==fail_op&&++seen==fail_at){if(throwing)throw std::runtime_error("declared allocator failure");return 1;}return 0;
    }
    Services services(){Services s;s.context=this;
        s.allocate=[](void* raw,std::uint32_t bytes,std::uint32_t tag,Record** out){auto& f=*static_cast<Fixture*>(raw);const auto id=0x10006000u+unsigned(f.allocated++)*0x100u;
            if(f.event(0,bytes,tag,id))return 1;
            auto& o=f.insert(id,bytes);o.phase=1;*out=&o.record;return 0;};
        s.deallocate=[](void* raw,Record* q){auto& f=*static_cast<Fixture*>(raw);if(f.event(1,q->ref.identity))return 1;f.objects.at(q->ref.identity)->phase=2;return 0;};return s;}
    cl::Services list_services(){cl::Services s;s.context=this;
        s.allocate=[](void* raw,cl::List&,std::uint32_t bytes,std::uint32_t tag,cl::Array** out){auto& f=*static_cast<Fixture*>(raw);const auto id=0x10004000u+unsigned(f.array_allocated++)*0x100u;
            if(f.event(2,bytes,tag,id))return 1;
            auto a=std::make_unique<cl::Array>();a->identity=id;a->slots.resize(bytes/4);*out=a.get();f.arrays.emplace(id,std::move(a));return 0;};
        s.factory=[](void* raw,cl::List&,std::int32_t type,ConditionRef** out){auto& f=*static_cast<Fixture*>(raw);Result r;const auto status=f.runtime->create(type,&r);if(status!=Status::complete)return 1;*out=&r.record->ref;return 0;};
        s.delete_virtual4=[](void* raw,cl::List&,ConditionRef* q){auto& f=*static_cast<Fixture*>(raw);Result r;return f.runtime->destroy(f.objects.at(q->identity)->record,true,&r)==Status::complete?0:1;};
        s.deallocate=[](void* raw,cl::List&,cl::Array* a){auto& f=*static_cast<Fixture*>(raw);return f.event(3,a->identity);};return s;}
    void setup(const std::vector<int>& row){fail_op=row[3];fail_at=row[4];throwing=row[5];runtime=std::make_unique<Runtime>(services());
        if((row[0]>=1&&row[0]<=4)||row[0]>=7){auto& o=insert(0x10010000,row[1]<3||row[1]==7?12:8);o.record.dispatch_0=row[0]>=7?Dispatch::base:row[1]==7?Dispatch::generic:kinds[row[1]];o.record.comparator_8=row[1]<3?row[1]:-858993460;}}
};
void output(const Fixture& f,unsigned status){
    auto def=[&](const cl::Definition& d){if(!d.list){std::cout<<"[-1,-1]";return;}int row=-1;for(unsigned i=0;i<f.view.count();++i)if(f.view.list(*f.view.row(i),0)==d.list)row=int(i);std::cout<<'['<<row<<','<<d.index<<']';};
    std::cout<<"{\"status\":"<<status<<",\"count\":"<<f.list.count_0<<",\"array\":"<<(f.list.children_4?f.list.children_4->identity:0)<<",\"definition\":";def(f.list.py_data_8);
    std::cout<<",\"objects\":[";bool comma=false;for(const auto& p:f.objects){if(comma)std::cout<<',';comma=true;const auto& o=*p.second;std::cout<<'['<<p.first<<','<<o.bytes<<','<<o.phase<<','<<unsigned(o.record.dispatch_0)<<','<<o.record.comparator_8<<',';def(o.record.fields.py_data_4);std::cout<<']';}
    std::cout<<"],\"arrays\":[";comma=false;for(const auto& p:f.arrays){if(comma)std::cout<<',';comma=true;std::cout<<'['<<p.first<<",[";for(unsigned i=0;i<p.second->slots.size();++i){if(i)std::cout<<',';std::cout<<(p.second->slots[i]?p.second->slots[i]->identity:0);}std::cout<<"]]";}
    std::cout<<"],\"trace\":[";comma=false;for(const auto& t:f.trace){if(comma)std::cout<<',';comma=true;std::cout<<'['<<t[0]<<','<<t[1]<<','<<t[2]<<','<<t[3]<<']';}std::cout<<"]}";
}
void guards(table::View view){
    const std::vector<int> base{0,0,0,-1,1,0};
    {Fixture f(view);f.setup(base);Result r;require(f.runtime->create(-1,&r)==Status::invalid_argument);require(f.runtime->create(7,&r)==Status::invalid_argument);require(f.trace.empty());}
    {Fixture f(view);f.setup(base);f.reenter=true;Result r;require(f.runtime->create(0,&r)==Status::complete&&f.nested==Status::reentrant);require(r.record&&r.record->ref.fields==&r.record->fields);}
    {Runtime missing({});Result r;require(missing.create(0,&r)==Status::service_unavailable);}
    {Fixture f(view);f.setup(base);auto s=f.services();s.allocate=[](void*,std::uint32_t,std::uint32_t,Record** out){*out=nullptr;return 0;};Runtime bad(s);Result r;require(bad.create(0,&r)==Status::source_fault);}
    {Fixture f(view);f.setup(base);auto s=f.services();s.allocate=[](void* raw,std::uint32_t bytes,std::uint32_t,Record** out){auto& f=*static_cast<Fixture*>(raw);auto& o=f.insert(0x10006000,bytes);o.record.ref.fields=nullptr;*out=&o.record;return 0;};Runtime bad(s);Result r;
     require(bad.create(0,&r)==Status::projection_changed);require(f.objects.at(0x10006000)->record.dispatch_0==Dispatch::uninitialized);}
    {Fixture f(view);f.setup(base);auto& q=f.insert(0x10006000,12).record;auto s=f.services();s.allocate=[](void* raw,std::uint32_t,std::uint32_t,Record** out){auto& f=*static_cast<Fixture*>(raw);*out=&f.objects.at(0x10006000)->record;return 0;};Runtime alias(s);
     const auto* original=q.fields.py_data_4.list;require(alias.create(0,reinterpret_cast<Result*>(&q))==Status::invalid_argument);
     require(q.fields.py_data_4.list==original&&q.fields.py_data_4.view&&q.dispatch_0==Dispatch::uninitialized&&q.comparator_8==-858993460&&q.ref.fields==&q.fields);}
    {Fixture f(view);f.setup(base);Result r;require(f.runtime->create(0,&r)==Status::complete);auto* q=r.record;auto s=f.services();s.deallocate=nullptr;Runtime partial(s);
     require(partial.destroy(*q,true,&r)==Status::service_unavailable);require(q->dispatch_0==Dispatch::generic&&q->comparator_8==0);}
    {Fixture f(view);f.setup(base);Result r;require(f.runtime->create(3,&r)==Status::complete);auto* q=r.record;
     require(f.runtime->construct_base(*q,reinterpret_cast<Result*>(q))==Status::invalid_argument);require(q->dispatch_0==Dispatch::player_in_level);q->ref.fields=nullptr;
     require(f.runtime->destroy(*q,false,&r)==Status::projection_changed);require(q->dispatch_0==Dispatch::player_in_level);}
    {Fixture f(view);f.setup(base);Result r;require(f.runtime->create(0,&r)==Status::complete);auto* q=r.record;auto s=f.services();s.deallocate=[](void* raw,Record* q){auto& f=*static_cast<Fixture*>(raw);f.objects.erase(q->ref.identity);return 0;};Runtime terminal(s);
     require(terminal.destroy(*q,true,&r)==Status::complete);require(f.objects.empty());}
    // A genuine list may clear its slot after the actual D0 erases the Record.
    {Fixture f(view);f.setup(base);auto s=f.services();s.deallocate=[](void* raw,Record* q){auto& f=*static_cast<Fixture*>(raw);f.objects.erase(q->ref.identity);return 0;};f.runtime=std::make_unique<Runtime>(s);
     cl::Runtime owner(f.list,f.list_services());cl::Result r;require(owner.assign_pydata(f.definition(0),1,&r)==cl::Status::complete);require(f.objects.size()==1);
     require(owner.destroy(&r)==cl::Status::complete&&f.objects.empty()&&!f.list.children_4);}
}
}
int main(int argc,char** argv){try{
    if(argc!=3)return 2;
    auto view=definitions(argv[1]);std::ifstream input(argv[2]);std::string line;std::cout<<"{\"results\":[";bool comma=false;
    while(std::getline(input,line)){std::istringstream stream(line);std::vector<int> row;int value;while(stream>>value)row.push_back(value);if(row.size()!=6)throw std::runtime_error("factory input width");Fixture f(view);f.setup(row);unsigned status=0;Result result;
        if(row[0]==0)status=unsigned(f.runtime->create(row[1],&result));
        else if(row[0]==1||row[0]==2)status=unsigned(f.runtime->construct_base(f.objects.at(0x10010000)->record,&result));
        else if(row[0]==3||row[0]==4)status=unsigned(f.runtime->destroy(f.objects.at(0x10010000)->record,row[0]==4,&result));
        else if(row[0]>=7)status=unsigned(f.runtime->destroy(f.objects.at(0x10010000)->record,row[0]==9,&result));
        else {cl::Runtime owner(f.list,f.list_services());cl::Result r;const auto d=f.definition(unsigned(row[2]));status=unsigned(owner.assign_pydata(d,std::int32_t(d.list->definition->count),&r));if(!status&&row[0]==5)status=unsigned(owner.destroy(&r));}
        if(comma)std::cout<<',';
        comma=true;output(f,status);}
    guards(view);std::cout<<"],\"native_checks\":"<<checks<<"}\n";return 0;
}catch(const std::exception& error){std::cerr<<error.what();return 1;}}
