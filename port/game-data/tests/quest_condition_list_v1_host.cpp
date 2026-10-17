#include "../quest_condition_list_v1.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
using namespace dh2::data::quest_condition_list_v1;
namespace table=dh2::data::quest_table_bindings_v1;
namespace {
unsigned checks=0;
void require(bool value){++checks;if(!value)throw std::runtime_error("condition list gate failed at "+std::to_string(checks));}
std::shared_ptr<std::vector<std::uint8_t>> file(const std::string& path){
    std::ifstream f(path,std::ios::binary|std::ios::ate);if(!f)throw std::runtime_error("cache missing");
    const auto size=f.tellg();auto data=std::make_shared<std::vector<std::uint8_t>>(std::size_t(size));f.seekg(0);
    if(!f.read(reinterpret_cast<char*>(data->data()),size))throw std::runtime_error("cache read failed");
    return data;
}
table::View definitions(const std::string& cache){
    auto packed=file(cache+"/v2quests_pyarray.bin"),names=file(cache+"/v2quests_pyarraynames.bin");table::Input input;
    require(!dh2_quests_open(&input.table,packed->data(),std::uint32_t(packed->size())));
    input.packed_owner=packed;input.names=names->data();input.names_size=names->size();input.names_owner=names;
    table::Owner owner;std::string error;require(owner.load(input,error));return owner.borrow();
}
struct Object {ConditionFields fields;ConditionRef ref;unsigned phase=0;explicit Object(std::uintptr_t id):ref{id,&fields}{}};
struct Fixture {
    table::View view;List list;std::unique_ptr<Runtime> runtime;
    std::map<std::uintptr_t,std::unique_ptr<Array>> arrays;
    std::map<std::uintptr_t,std::unique_ptr<Object>> objects;
    std::vector<std::array<long long,4>> trace;
    int row=0,mutation=0,fail_op=-1,fail_at=1,fail_seen=0,throwing=0,zero_eval=-1,allocated=0,created=0,evals=0;
    bool reenter=false;Status nested=Status::complete;
    explicit Fixture(table::View v):view(std::move(v)){}
    Object& object(std::uintptr_t id){auto o=std::make_unique<Object>(id);auto* raw=o.get();objects.emplace(id,std::move(o));return *raw;}
    Array& array(std::uintptr_t id,unsigned count){auto a=std::make_unique<Array>();a->identity=id;a->slots.resize(count);auto* raw=a.get();arrays.emplace(id,std::move(a));return *raw;}
    Definition definition(unsigned r,unsigned i=0){return {view,view.list(*view.row(r),0),i};}
    int event(int op,long long a=0,long long b=0,long long c=0){
        trace.push_back({op,a,b,c});if(reenter){reenter=false;Result result;nested=runtime->evaluate(&result);}
        if(op==fail_op&&++fail_seen==fail_at){if(throwing)throw std::runtime_error("declared condition provider failure");return 1;}return 0;
    }
    Services services(){
        Services s;s.context=this;
        s.allocate=[](void* raw,List& list,std::uint32_t bytes,std::uint32_t tag,Array** out){auto& f=*static_cast<Fixture*>(raw);const auto id=0x10004000u+unsigned(f.allocated++)*0x100u;
            if(f.event(0,bytes,tag,id))return 1;
            auto& a=f.array(id,bytes/4);*out=&a;if(f.mutation==1)list.count_0=0;return 0;};
        s.factory=[](void* raw,List& list,std::int32_t kind,ConditionRef** out){auto& f=*static_cast<Fixture*>(raw);const auto id=0x10006000u+unsigned(f.created++)*0x100u;
            if(f.event(1,kind,id))return 1;
            auto& o=f.object(id);o.phase=1;*out=&o.ref;if(f.mutation==2)list.count_0=1;
            if(f.mutation==3&&f.created==1)list.children_4=f.arrays.at(0x10002000).get();
            return 0;};
        s.delete_virtual4=[](void* raw,List& list,ConditionRef* child){auto& f=*static_cast<Fixture*>(raw);if(f.event(2,child->identity))return 1;f.objects.at(child->identity)->phase=2;
            if(f.mutation==4)list.count_0=1;
            if(f.mutation==5)list.children_4=f.arrays.at(0x10002000).get();
            return 0;};
        s.deallocate=[](void* raw,List& list,Array* array){auto& f=*static_cast<Fixture*>(raw);if(f.event(3,array->identity))return 1;if(f.mutation==6)list.children_4=f.arrays.at(0x10002000).get();return 0;};
        s.eval_virtual8=[](void* raw,List& list,ConditionRef* child,std::uint32_t* out){auto& f=*static_cast<Fixture*>(raw);const auto value=f.evals++==f.zero_eval?0u:7u;if(f.event(4,child->identity,value))return 1;
            *out=value;if(f.mutation==7)list.count_0=1;if(f.mutation==8)list.children_4=f.arrays.at(0x10002000).get();return 0;};return s;
    }
    void setup(const std::vector<int>& input){
        row=input[1];list.count_0=input[3];list.py_data_8=definition(0);mutation=input[6];fail_op=input[7];fail_at=input[8];throwing=input[9];zero_eval=input[10];
        for(unsigned which=0;which<2;++which){auto& a=array(0x10001000u+which*0x1000u,unsigned(input[4]));
            for(unsigned i=0;i<a.slots.size();++i){auto& o=object(0x10010000u+which*0x1000u+i*0x100u);a.slots[i]=int(i)==input[5]?nullptr:&o.ref;}}
        list.children_4=input[4]<0?nullptr:arrays.at(0x10001000).get();runtime=std::make_unique<Runtime>(list,services());
    }
    Status invoke(unsigned mode,Result* result){switch(mode){case 0:case 1:return runtime->construct(result);case 2:return runtime->assign_pydata(definition(unsigned(row)),requested_count,result);case 3:case 4:return runtime->destroy(result);default:return runtime->evaluate(result);}}
    int requested_count=0;
};
void output(const Fixture& f,Status status,const Result& result){
    auto def=[&](const Definition& d){if(!d.list){std::cout<<"[-1,-1]";return;}int row=-1;for(unsigned i=0;i<f.view.count();++i)if(f.view.list(*f.view.row(i),0)==d.list)row=int(i);std::cout<<'['<<row<<','<<d.index<<']';};
    std::cout<<"{\"status\":"<<unsigned(status)<<",\"count\":"<<f.list.count_0<<",\"array\":"<<(f.list.children_4?f.list.children_4->identity:0)<<",\"definition\":";def(f.list.py_data_8);
    std::cout<<",\"evaluation\":"<<result.evaluation<<",\"arrays\":[";bool comma=false;
    for(const auto& p:f.arrays){if(comma)std::cout<<',';comma=true;std::cout<<'['<<p.first<<",[";for(unsigned i=0;i<p.second->slots.size();++i){if(i)std::cout<<',';std::cout<<(p.second->slots[i]?p.second->slots[i]->identity:0);}std::cout<<"]]";}
    std::cout<<"],\"objects\":[";comma=false;for(const auto& p:f.objects){if(comma)std::cout<<',';comma=true;std::cout<<'['<<p.first<<','<<p.second->phase<<',';def(p.second->fields.py_data_4);std::cout<<']';}
    std::cout<<"],\"trace\":[";comma=false;for(const auto& t:f.trace){if(comma)std::cout<<',';comma=true;std::cout<<'['<<t[0]<<','<<t[1]<<','<<t[2]<<','<<t[3]<<']';}std::cout<<"]}";
}
void guards(table::View view){
    const std::vector<int> base{2,0,1,0,0,-1,0,-1,1,0,-1,0};
    {Fixture f(view);f.setup(base);f.requested_count=1;f.reenter=true;Result r;require(f.invoke(2,&r)==Status::complete&&f.nested==Status::reentrant);require(r.published==1);}
    {Fixture f(view);f.setup(base);Result r;require(f.runtime->assign_pydata(f.definition(0),1,reinterpret_cast<Result*>(&f.list))==Status::invalid_argument);require(f.trace.empty());}
    {Fixture f(view);f.setup(base);auto d=f.definition(0);Result r;require(f.runtime->assign_pydata(d,1,reinterpret_cast<Result*>(&d))==Status::invalid_argument);require(f.trace.empty());require(f.runtime->assign_pydata(d,1,&r)==Status::complete);require(f.runtime->evaluate(reinterpret_cast<Result*>(f.list.children_4))==Status::invalid_argument);}
    {Fixture f(view);f.setup(base);Runtime missing(f.list,{});Result r;require(missing.assign_pydata(f.definition(0),1,&r)==Status::service_unavailable);require(f.list.count_0==1&&f.list.py_data_8.list==f.definition(0).list);}
    {Fixture f(view);f.setup(base);auto services=f.services();services.factory=nullptr;Runtime missing(f.list,services);Result r;require(missing.assign_pydata(f.definition(0),1,&r)==Status::service_unavailable);require(f.list.children_4&&f.list.children_4->slots.size()==1&&!f.list.children_4->slots[0]);}
    {Fixture f(view);f.setup(base);Result r;auto d=f.definition(0);d.index=UINT32_MAX;require(f.runtime->assign_pydata(d,1,&r)==Status::source_fault);require(f.list.children_4&&f.trace.size()==1);}
    {Fixture f(view);f.setup(base);Result r;require(f.runtime->assign_pydata({},0,&r)==Status::complete&&f.trace.empty());require(f.runtime->assign_pydata({},1,&r)==Status::source_fault&&f.trace.size()==1);}
    {Fixture f(view);f.setup(base);Result r;require(f.runtime->assign_pydata(f.definition(0),1,&r)==Status::complete);const auto old=f.list.children_4;auto* condition=old->slots[0];require(f.runtime->assign_pydata(f.definition(0),1,&r)==Status::complete);require(f.list.children_4!=old&&old->slots[0]==condition&&condition->fields->py_data_4.view);require(f.runtime->destroy(&r)==Status::complete);require(old->slots[0]==condition&&f.objects.at(condition->identity)->phase==1);}
    for(unsigned missing=0;missing<3;++missing){Fixture f(view);f.setup(base);Result r;
        require(f.runtime->assign_pydata(f.definition(0),1,&r)==Status::complete);auto* array=f.list.children_4;auto* child=array->slots[0];auto services=f.services();
        if(missing==0)services.eval_virtual8=nullptr;
        if(missing==1)services.delete_virtual4=nullptr;
        if(missing==2)services.deallocate=nullptr;
        Runtime partial(f.list,services);require((missing==0?partial.evaluate(&r):partial.destroy(&r))==Status::service_unavailable);
        require(f.list.children_4==array);require(array->slots[0]==(missing==2?nullptr:child));
        require(f.objects.at(child->identity)->phase==(missing==2?2u:1u));}
    {Fixture f(view);f.setup(base);auto services=f.services();services.allocate=[](void* raw,List& list,std::uint32_t,std::uint32_t,Array** out){auto& f=*static_cast<Fixture*>(raw);*out=&f.array(0x10004000,1);list.count_0=0;return 1;};
     auto* old=f.list.children_4;Runtime partial(f.list,services);Result r;require(partial.assign_pydata(f.definition(0),1,&r)==Status::service_failed);require(f.list.count_0==0&&f.list.children_4==old&&f.arrays.count(0x10004000)==1);}
    {Fixture f(view);f.setup(base);auto services=f.services();services.factory=[](void* raw,List& list,std::int32_t,ConditionRef** out){auto& f=*static_cast<Fixture*>(raw);auto& child=f.object(0x10006000);child.phase=1;*out=&child.ref;list.count_0=0;return 1;};
     Runtime partial(f.list,services);Result r;require(partial.assign_pydata(f.definition(0),1,&r)==Status::service_failed);require(f.list.count_0==0&&!f.list.children_4->slots[0]&&f.objects.count(0x10006000)==1);}
    {Fixture f(view);f.setup(base);auto services=f.services();services.allocate=[](void*,List&,std::uint32_t,std::uint32_t,Array** out){*out=nullptr;return 0;};
     Runtime partial(f.list,services);Result r;require(partial.assign_pydata(f.definition(0),1,&r)==Status::source_fault);require(!f.list.children_4&&f.list.count_0==1);}
    {Fixture f(view);f.setup(base);auto services=f.services();services.factory=[](void*,List&,std::int32_t,ConditionRef** out){*out=nullptr;return 0;};
     Runtime partial(f.list,services);Result r;require(partial.assign_pydata(f.definition(0),1,&r)==Status::source_fault);require(r.published==1&&!f.list.children_4->slots[0]);}
    {Fixture f(view);f.setup(base);auto services=f.services();services.factory=[](void* raw,List&,std::int32_t,ConditionRef** out){auto& f=*static_cast<Fixture*>(raw);auto& child=f.object(0x10006000);child.ref.fields=nullptr;*out=&child.ref;return 0;};
     Runtime partial(f.list,services);Result r;require(partial.assign_pydata(f.definition(0),1,&r)==Status::source_fault);require(r.published==1&&f.list.children_4->slots[0]==&f.objects.at(0x10006000)->ref);}
    {Fixture f(view);f.setup(base);auto services=f.services();services.allocate=[](void* raw,List&,std::uint32_t bytes,std::uint32_t tag,Array** out){auto& f=*static_cast<Fixture*>(raw);require(bytes==0&&tag==0);*out=&f.array(0x10004000,0);return 0;};
     Runtime partial(f.list,services);Result r;require(partial.assign_pydata(f.definition(0),0x40000000,&r)==Status::unsafe_storage);require(f.list.children_4&&f.list.count_0==0x40000000&&r.published==0);}
    {Fixture f(view);f.setup(base);Definition copied=f.definition(0);table::ListRef foreign=*copied.list;copied.list=&foreign;Result r;
     require(f.runtime->assign_pydata(copied,1,&r)==Status::source_fault);require(f.list.children_4&&f.trace.size()==1);}
    {Fixture f(view);f.setup(base);Result r;require(f.runtime->assign_pydata(f.definition(0),1,&r)==Status::complete);auto* actual=f.list.children_4->slots[0];f.view={};view={};require(actual->fields->py_data_4.view.count()==64);}
}
}
int main(int argc,char** argv){try{
    if(argc!=3)return 2;
    auto view=definitions(argv[1]);std::ifstream input(argv[2]);std::string line;std::cout<<"{\"results\":[";bool comma=false;
    while(std::getline(input,line)){std::istringstream stream(line);std::vector<int> row;int value;while(stream>>value)row.push_back(value);if(row.size()!=12)throw std::runtime_error("condition input width");Fixture f(view);f.setup(row);f.requested_count=row[2];Result result;auto status=f.invoke(unsigned(row[0]),&result);if(row[11]&&status==Status::complete)status=f.runtime->destroy(&result);if(comma)std::cout<<',';comma=true;output(f,status,result);}
    guards(view);std::cout<<"],\"native_checks\":"<<checks<<"}\n";return 0;
}catch(const std::exception& error){std::cerr<<error.what();return 1;}}
