#include "../quest_table_bindings_v1.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
using namespace dh2::data::quest_table_bindings_v1;
namespace {
unsigned checks=0;
void require(bool value,const char* why="quest table gate failed"){++checks;if(!value)throw std::runtime_error(std::string(why)+" at "+std::to_string(checks));}
std::shared_ptr<std::vector<std::uint8_t>> file(const std::string& path){
    std::ifstream f(path,std::ios::binary|std::ios::ate);if(!f)throw std::runtime_error("cache missing");const auto size=f.tellg();auto p=std::make_shared<std::vector<std::uint8_t>>(static_cast<std::size_t>(size));f.seekg(0);if(!f.read(reinterpret_cast<char*>(p->data()),size))throw std::runtime_error("cache read failed");return p;
}
Input input(std::shared_ptr<std::vector<std::uint8_t>> packed,std::shared_ptr<std::vector<std::uint8_t>> names){
    Input i;require(!dh2_quests_open(&i.table,packed->data(),std::uint32_t(packed->size())));i.packed_owner=packed;i.names=names->data();i.names_size=names->size();i.names_owner=names;return i;
}
void quote(const char* text){
    std::cout<<'"';for(const auto* p=reinterpret_cast<const unsigned char*>(text);*p;++p){
        if(*p=='"'||*p=='\\')std::cout<<'\\'<<char(*p);
        else if(*p=='\n')std::cout<<"\\n";else if(*p=='\r')std::cout<<"\\r";else if(*p=='\t')std::cout<<"\\t";
        else std::cout<<char(*p);
    }std::cout<<'"';
}
void ints(const std::int32_t* data,unsigned count){std::cout<<'[';for(unsigned i=0;i<count;++i){if(i)std::cout<<',';std::cout<<data[i];}std::cout<<']';}
void hex(const View& view,dh2_quest_span span){Span bytes;std::string error;require(view.bytes(span,&bytes,error));std::cout<<'"';constexpr char digits[]="0123456789abcdef";for(unsigned i=0;i<bytes.size;++i)std::cout<<digits[bytes.data[i]>>4]<<digits[bytes.data[i]&15];std::cout<<'"';}
void objective(const View& view,const dh2_quest_objective& o){std::cout<<"{\"common\":";ints(o.common,3);std::cout<<",\"strings\":[";hex(view,o.strings[0]);std::cout<<',';hex(view,o.strings[1]);std::cout<<"],\"args\":";ints(o.args,3);std::cout<<'}';}
void rows(const View& view){
    std::string error;std::cout<<'[';
    for(unsigned i=0;i<view.count();++i){
        const auto* row=view.row(i);const auto* r=view.record(*row);require(row&&r);require(row->identity==view.rows_identity()+std::size_t(i)*0x11c&&view.resolve(row->identity)==row);
        if(i)std::cout<<',';
        std::cout<<"{\"index\":"<<i<<",\"ids\":";ints(r->ids,4);std::cout<<",\"lists\":[";
        for(unsigned group=0;group<5;++group){
            const auto* list=view.list(*row,group);require(list&&list->definition==&r->lists[group]);std::uintptr_t count=0,pointer=0;
            require(view.read_word(*row,0x14+group*8,&count,error)&&count==list->definition->count);
            require(view.read_word(*row,0x18+group*8,&pointer,error)&&view.resolve_list(pointer)==list);
            if(group)std::cout<<',';
            std::cout<<'[';
            for(unsigned j=0;j<list->definition->count;++j){
                dh2_quest_span span;require(view.list_record(*list,j,&span,error));if(j)std::cout<<',';
                if(group==1){dh2_quest_objective value;require(view.objective(*list,j,&value,error));objective(view,value);}
                else{Span bytes;require(view.bytes(span,&bytes,error)&&bytes.size==12);std::int32_t words[3];for(unsigned k=0;k<3;++k){const auto* p=bytes.data+k*4;const auto v=std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);std::memcpy(&words[k],&v,4);}ints(words,3);}
            }
            std::cout<<']';
        }
        const auto* accept=view.resolve_stub(row->identity+0x3c);const auto* end=view.resolve_stub(row->identity+0x68);
        require(accept&&end&&accept->definition==&r->accept&&end->definition==&r->end&&accept->row==row&&end->row==row);
        std::uintptr_t state=0,act=0;require(view.read_word(*row,0x9c,&state,error)&&state==std::uint32_t(r->state));require(view.read_word(*row,0x118,&act,error)&&act==std::uint32_t(r->act));
        std::cout<<"],\"accept\":";objective(view,*accept->definition);std::cout<<",\"end\":";objective(view,*end->definition);
        std::cout<<",\"target_level\":"<<r->target_level<<",\"repeatable\":"<<r->repeatable<<",\"state\":"<<r->state<<",\"scripts\":[";
        for(unsigned j=0;j<14;++j){if(j)std::cout<<',';hex(view,r->scripts[j]);}
        std::cout<<"],\"priority\":"<<r->priority<<",\"act\":"<<r->act<<",\"name\":";quote(view.definition_name(i));std::cout<<'}';
    }std::cout<<']';
}
void negative_and_lifetime(Owner& owner,const Input& initial){
    std::string error;const auto stable=owner.borrow();const auto identity=stable.rows_identity();const auto* first=stable.row(0);
    for(std::uint32_t size=0;size<initial.table.size;++size){Input bad=initial;bad.table.size=size;require(!owner.load(bad,error));require(owner.borrow().rows_identity()==identity&&stable.row(0)==first);}
    for(std::size_t size=0;size<initial.names_size;++size){Input bad=initial;bad.names_size=size;require(!owner.load(bad,error));require(owner.borrow().rows_identity()==identity);}
    {Input bad=initial;bad.table.count+=1;require(!owner.load(bad,error));require(owner.borrow().rows_identity()==identity);}
    {Input bad=initial;bad.packed_owner.reset();require(!owner.load(bad,error));require(owner.borrow().rows_identity()==identity);}
    {Input bad=initial;bad.names_owner.reset();require(!owner.load(bad,error));require(owner.borrow().rows_identity()==identity);}
    {auto bad_bytes=std::make_shared<std::vector<std::uint8_t>>(initial.table.bytes,initial.table.bytes+initial.table.size);bad_bytes->push_back(0);Input bad=initial;bad.table.bytes=bad_bytes->data();bad.table.size=std::uint32_t(bad_bytes->size());bad.packed_owner=bad_bytes;require(!owner.load(bad,error));require(owner.borrow().rows_identity()==identity);}
    {auto bad_names=std::make_shared<std::vector<std::uint8_t>>(initial.names,initial.names+initial.names_size);bad_names->push_back(0);Input bad=initial;bad.names=bad_names->data();bad.names_size=bad_names->size();bad.names_owner=bad_names;require(!owner.load(bad,error));require(owner.borrow().rows_identity()==identity);}
    require(!stable.row(stable.count())&&!stable.resolve(identity-1)&&!stable.resolve(identity+1)&&!stable.resolve(identity+std::size_t(stable.count())*0x11c));
    require(!stable.resolve_stub(identity)&&!stable.resolve_stub(identity+0x3d)&&!stable.definition_name(stable.count())&&!stable.resolve_list(1));
    {PyDataRef copied=*first;std::uintptr_t out=999;require(!stable.read_word(copied,0x9c,&out,error)&&out==999);require(!stable.record(copied));}
    {std::uintptr_t out=999;require(!stable.read_word(*first,0x98,&out,error)&&out==999);require(!stable.read_word(*first,0x14,reinterpret_cast<std::uintptr_t*>(const_cast<PyDataRef*>(first)),error));}
    {const auto* record=stable.record(*first);const auto before=record->ids[0];require(!stable.read_word(*first,0x9c,reinterpret_cast<std::uintptr_t*>(const_cast<std::int32_t*>(record->ids)),error));require(record->ids[0]==before);}
    {const auto* list=stable.list(*first,0);ListRef copied=*list;dh2_quest_span out{777,888};require(!stable.list_record(copied,0,&out,error)&&out.offset==777&&out.size==888);require(!stable.list_record(*list,list->definition->count,&out,error)&&out.offset==777&&out.size==888);}
    {dh2_quest_span bad{initial.table.size,1};Span out{initial.table.bytes,777};require(!stable.bytes(bad,&out,error)&&out.data==initial.table.bytes&&out.size==777);}
    {const auto* list=stable.list(*first,0);dh2_quest_objective out{};out.common[0]=999;require(!stable.objective(*list,0,&out,error)&&out.common[0]==999);}
    require(owner.load(initial,error));const auto fresh=owner.borrow();require(fresh.rows_identity()!=identity&&stable.resolve(identity)==first&&fresh.resolve(identity)==nullptr);require(stable.definition_name(0)!=fresh.definition_name(0)&&std::strcmp(stable.definition_name(0),fresh.definition_name(0))==0);
    {auto packed=std::make_shared<std::vector<std::uint8_t>>(initial.table.bytes,initial.table.bytes+initial.table.size);auto names=std::make_shared<std::vector<std::uint8_t>>(initial.names,initial.names+initial.names_size);std::weak_ptr<const void> weak_packed=packed,weak_names=names;Owner local;View lease;
     {auto in=input(packed,names);require(local.load(in,error));lease=local.borrow();}
     packed.reset();names.reset();local=Owner{};require(!weak_packed.expired()&&!weak_names.expired());require(lease.count()==64&&lease.record(*lease.row(0))->act==8);lease=View{};require(weak_packed.expired()&&weak_names.expired());}
    {auto empty=std::make_shared<std::vector<std::uint8_t>>(4,0);const auto in=input(empty,empty);Owner local;require(local.load(in,error));const auto v=local.borrow();require(bool(v)&&v.count()==0&&v.rows_identity()==0&&!v.row(0));}
}
} // namespace
int main(int argc,char** argv){try{
    if(argc!=2)return 2;
    const std::string cache=argv[1];auto packed=file(cache+"/v2quests_pyarray.bin"),names=file(cache+"/v2quests_pyarraynames.bin");const auto actual=input(packed,names);Owner owner;std::string error;require(owner.load(actual,error));const auto view=owner.borrow();require(view.count()==64);
    std::cout<<"{\"rows\":";rows(view);negative_and_lifetime(owner,actual);std::cout<<",\"native_checks\":"<<checks<<",\"packed_truncations\":"<<actual.table.size<<",\"names_truncations\":"<<actual.names_size<<"}\n";return 0;
}catch(const std::exception& error){std::cerr<<error.what();return 1;}}
