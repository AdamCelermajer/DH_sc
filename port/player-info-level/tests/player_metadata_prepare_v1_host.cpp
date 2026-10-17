#include "../player_metadata_prepare_v1.hpp"
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
namespace m=dh2::player_metadata_prepare_v1;
using Row=std::array<std::int64_t,20>;
using Event=std::array<std::int64_t,3>;
constexpr std::uintptr_t SAVE=0x10003000;
std::pair<std::string,std::string> names(int index) {
    switch(index) {
        case 0:return {"Rogue","Rogue"};case 1:return {"Warrior","Rogue"};
        case 2:return {"","Mage"};case 3:return {"A",std::string("A\0B",3)};
        case 4:return {std::string("A\0B",3),"A"};
        case 5:return {"Warrior","\xd7\x9e\xd7\x9b\xd7\xa9\xd7\xa3"};
        case 6:return {std::string(45,'A'),std::string(46,'B')};default:return {"",""};
    }
}
struct Fixture {
    Row row;std::uint64_t serial=0;
    dh2::netstruct_members_v1::Memory memory{nullptr,[](void*,std::size_t size,int)->void*{return std::malloc(size);},[](void*,void* p){std::free(p);}};
    dh2::player_info_record_v1::Factory factory{&serial,memory,{},nullptr,[](void*,std::uintptr_t p){return p==SAVE?dh2::netstruct_members_v1::Status::complete:dh2::netstruct_members_v1::Status::provider_failed;}};
    m::Record record;dh2::data::PlayerSavegameV1 save;m::SaveRef save_ref{SAVE,&save};
    m::Services services{};std::vector<Event> trace;int attempts=0,fail_at=-1,throw_at=-1;
    bool allocated=false,constructed=false;m::Runtime* reenter=nullptr;m::Status nested=m::Status::complete;
    static Fixture& f(void* p){return *static_cast<Fixture*>(p);}
    int note(int op,std::int64_t a=0,std::int64_t b=0) {
        trace.push_back({op,a,b});int at=attempts++;
        if(reenter){m::Result out;nested=reenter->prepare({},&out);}
        if(at==throw_at)throw std::runtime_error("declared provider failure");
        return at==fail_at?1:0;
    }
    explicit Fixture(Row input):row(input) {
        if(factory.construct_record(record)!=dh2::netstruct_members_v1::Status::complete)throw std::runtime_error("actual Record constructor");
        record.local_66c=static_cast<std::uint8_t>(row[1]);record.save_slot_664=static_cast<int>(row[2]);
        record.loading_info_680=row[3]?SAVE:0;record.character_660=row[6]?0x10008000:0;
        record.at(0x360)->header.value=static_cast<int>(row[8]);record.at(0x310)->header.value=static_cast<int>(row[9]);
        auto [player_name,saved_name]=names(static_cast<int>(row[12]));record.at(0x2b0)->text=player_name;
        record.at(0x2b0)->header.revision=123;record.at(0x360)->header.revision=456;record.at(0x310)->header.revision=789;
        save.set_player_name(saved_name);save.set_class(static_cast<int>(row[10]));save.set_player_level(static_cast<int>(row[11]));
        serial=static_cast<std::uint64_t>(row[13]);services.context=this;
        services.is_active=[](void* c,m::Record* r,int* out){auto& v=f(c);if(r!=&v.record)return 1;int rc=v.note(0,v.row[0]);if(rc)return rc;
            if(v.row[7]>=0)v.record.character_660=v.row[7]?0x10008000:0;
            if(v.row[16]!=INT32_MIN)v.record.save_slot_664=static_cast<int>(v.row[16]);
            if(v.row[17])v.record.loading_info_680=SAVE;
            *out=static_cast<int>(v.row[0]);return 0;};
        services.allocate_save=[](void* c,unsigned size,unsigned tag,std::uintptr_t* out){auto& v=f(c);if(size!=0x198||tag)return 1;int rc=v.note(1,size,tag);if(rc)return rc;v.allocated=true;*out=SAVE;return 0;};
        services.construct_indexed_save=[](void* c,std::uintptr_t p,unsigned slot,int mask,bool skip,m::SaveRef** out){auto& v=f(c);if(p!=SAVE||mask!=1||skip||!v.allocated)return 1;int rc=v.note(2,static_cast<int>(slot),mask);if(rc)return rc;v.constructed=true;*out=&v.save_ref;return 0;};
        services.save_by_identity=[](void* c,std::uintptr_t p,m::SaveRef** out){auto& v=f(c);if(p!=SAVE)return 1;*out=&v.save_ref;return 0;};
        services.debug_name_11=[](void* c,std::uint8_t* out){auto& v=f(c);*out=static_cast<std::uint8_t>(v.row[4]);return v.note(3,*out);};
        services.game_state_name_28=[](void* c,std::uint8_t* out){auto& v=f(c);int rc=v.note(4,v.row[5]);if(rc)return rc;
            if(v.row[18]!=INT32_MIN)v.save.set_class(static_cast<int>(v.row[18]));
            if(v.row[19]!=INT32_MIN)v.save.set_player_level(static_cast<int>(v.row[19]));
            *out=static_cast<std::uint8_t>(v.row[5]);return 0;};
    }
    m::Result run() {m::Runtime runtime(record,services);m::Result out;auto status=runtime.prepare({static_cast<int>(row[14]),static_cast<int>(row[15])},&out);if(status!=out.status)throw std::runtime_error("result status");return out;}
    void print(const m::Result& out) {
        if(out.name_setters)trace.push_back({6,0,0});
        if(out.class_setters)trace.push_back({7,record.at(0x360)->header.value,0});
        if(out.level_setters)trace.push_back({8,record.at(0x310)->header.value,0});
        std::cout<<"{\"loading\":"<<(record.loading_info_680?1:0)<<",\"character\":"<<(record.character_660?1:0)<<",\"class\":"<<record.at(0x360)->header.value<<",\"level\":"<<record.at(0x310)->header.value<<",\"name\":\"";
        const char* hex="0123456789abcdef";for(unsigned char c:record.at(0x2b0)->text)std::cout<<hex[c>>4]<<hex[c&15];
        std::cout<<"\",\"serial\":"<<serial<<",\"revisions\":["<<record.at(0x2b0)->header.revision<<','<<record.at(0x360)->header.revision<<','<<record.at(0x310)->header.revision<<"],\"trace\":[";
        bool comma=false;for(auto e:trace){if(comma)std::cout<<',';comma=true;std::cout<<'['<<e[0]<<','<<e[1]<<','<<e[2]<<']';}std::cout<<"]}";
    }
};
unsigned policy_checks() {
    Row row{{1,1,2,0,0,0,0,-1,1,1,2,7,1,9,0,0,INT32_MIN,0,INT32_MIN,INT32_MIN}};
    Fixture baseline(row);auto prepared=baseline.run();if(prepared.status!=m::Status::complete)throw std::runtime_error("baseline");unsigned checks=0;
    for(bool throwing:{false,true})for(int i=0;i<baseline.attempts;++i) {
        Fixture f(row);if(throwing)f.throw_at=i;else f.fail_at=i;auto out=f.run();
        if(out.status!=m::Status::service_failed||f.attempts!=i+1||f.trace!=std::vector<Event>(baseline.trace.begin(),baseline.trace.begin()+i+1)||
           (f.record.loading_info_680!=0)!=(i>=3)||f.record.at(0x360)->header.value!=1||f.record.at(0x310)->header.value!=1)throw std::runtime_error("metadata failure prefix");
        ++checks;
    }
    {Fixture f(row);f.services.construct_indexed_save=nullptr;auto out=f.run();if(out.status!=m::Status::service_unavailable||!f.allocated||f.record.loading_info_680)throw std::runtime_error("allocated backing retained before ctor");++checks;}
    {Fixture f(row);f.services.debug_name_11=nullptr;auto out=f.run();if(out.status!=m::Status::service_unavailable||f.record.loading_info_680!=SAVE||!f.constructed)throw std::runtime_error("published backing retained");++checks;}
    {Fixture f(row);m::Runtime runtime(f.record,f.services);f.reenter=&runtime;m::Result out;runtime.prepare({},&out);if(out.status!=m::Status::complete||f.nested!=m::Status::reentrant)throw std::runtime_error("reentry");++checks;}
    {Fixture f(row);m::Runtime runtime(f.record,f.services);auto* alias=reinterpret_cast<m::Result*>(&f.record);if(runtime.prepare({},alias)!=m::Status::invalid_argument||!f.record.constructed||f.attempts)throw std::runtime_error("record output alias");++checks;}
    {Fixture f(row);f.save_ref.identity=0;auto out=f.run();if(out.status!=m::Status::missing_projection||f.record.loading_info_680||!f.constructed)throw std::runtime_error("constructor projection mismatch");++checks;}
    {Fixture f(row);f.record.constructed=false;auto out=f.run();f.record.constructed=true;if(out.status!=m::Status::invalid_record||f.attempts)throw std::runtime_error("invalid record");++checks;}
    {Fixture f(row);f.services.allocate_save=[](void*,unsigned,unsigned,std::uintptr_t* out){*out=0;return 0;};auto out=f.run();if(out.status!=m::Status::missing_projection||f.record.loading_info_680)throw std::runtime_error("missing allocation");++checks;}
    return checks;
}
int main(int argc,char** argv) {
    try {if(argc!=2)return 2;std::ifstream file(argv[1]);Row row;bool comma=false;std::cout<<"{\"results\":[";
        while(file>>row[0]){for(unsigned i=1;i<row.size();++i)file>>row[i];Fixture fixture(row);auto out=fixture.run();if(out.status!=m::Status::complete&&out.status!=m::Status::outside_domain)throw std::runtime_error("unexpected status");if(comma)std::cout<<',';comma=true;fixture.print(out);}
        std::cout<<"],\"policy_checks\":"<<policy_checks()<<"}\n";
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
