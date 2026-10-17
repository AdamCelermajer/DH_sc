#include "../player_offline_registration_v1.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
namespace r=dh2::player_offline_registration_v1;
using Row=std::array<std::int64_t,16>;
using Event=std::array<std::int64_t,3>;
struct Fields {
    dh2::character_level_member::IntMember level{};
    r::PlayerInfo projection{};
    std::int32_t slot=-1,controller=-1,id=-1,member=-1,number=-1,group=-1;
    std::uint8_t local=1,state=0;
    explicit Fields(int key=-1,bool fallback=false) {
        projection={fallback?0x10001008u:0x10003000u+(key+1)*0x1000u,key,&level};
    }
};
struct Fixture {
    Row row;Fields fallback{-1,true},temporary;
    std::map<int,Fields> entries;
    std::vector<r::PlayerInfo*> pointers;
    r::Registry registry{0x10001000,nullptr,0,&fallback.projection};
    dh2::player_locality_v1::Services queries{};
    r::Services services{};
    std::array<std::uint8_t,4> connected{};
    float axis=0,minimum=0,maximum=0;std::uint8_t pressed=0;
    std::vector<Event> events;
    int attempts=0,fail_at=-1,throw_at=-1;
    r::Runtime* reenter=nullptr;r::Status nested=r::Status::complete;
    int note(int op,std::int64_t a=0,std::int64_t b=0) {
        events.push_back({op,a,b});int index=attempts++;
        if(reenter){r::Result result;nested=reenter->update_player_numbers(&result);}
        if(index==throw_at)throw std::runtime_error("declared provider failure");
        return index==fail_at?1:0;
    }
    Fields& backing(r::PlayerInfo* p) {
        if(p==&temporary.projection)return temporary;
        if(p==&fallback.projection)return fallback;
        for(auto& [key,value]:entries){(void)key;if(p==&value.projection)return value;}
        throw std::runtime_error("foreign projection");
    }
    void rebuild(){pointers.clear();for(auto& [key,value]:entries){(void)key;pointers.push_back(&value.projection);}registry.entries=pointers.data();registry.entry_count=static_cast<unsigned>(pointers.size());}
    static void copy(Fields& d,const Fields& s){d.slot=s.slot;d.controller=s.controller;d.local=s.local;d.id=s.id;d.member=s.member;d.number=s.number;d.group=s.group;d.state=s.state;}
    explicit Fixture(Row input):row(input) {
        for(int key=0;key<4;++key)if(row[5]&(1<<key)){
            auto& f=entries.try_emplace(key,key).first->second;
            f.slot=static_cast<int>(row[9]);f.controller=10+key;f.local=(row[6]&(1<<key))?7:0;
            f.id=42+key;f.member=55+key;f.number=66+key;f.group=77+key;f.state=static_cast<std::uint8_t>(row[10]);
        }
        rebuild();for(int i=0;i<4;++i)connected[i]=(row[8]&(1<<i))?1:0;
        auto number=[](std::int64_t bits){std::uint32_t word=static_cast<std::uint32_t>(bits);float v;std::memcpy(&v,&word,4);return v;};
        axis=number(row[11]);minimum=number(row[12]);maximum=number(row[13]);pressed=static_cast<std::uint8_t>(row[14]);
        queries.context=this;queries.online=[](void* raw,std::uint8_t* out){auto& f=*static_cast<Fixture*>(raw);*out=0;return f.note(1);};
        queries.internal_id_player=[](void* raw,const r::Registry* registry,int id,unsigned flag,r::PlayerInfo** out){
            auto& f=*static_cast<Fixture*>(raw);if(registry!=&f.registry)return 1;
            int status=f.note(6,id,flag);if(status)return status;
            dh2::player_manager_host_level::Services s{};s.context=&f;s.read_online_byte_5=[](void*,std::uint8_t* v){*v=0;return 0;};
            dh2::player_manager_host_level::Result result;
            return dh2::player_manager_host_level::get_player_by_internal_id(registry,&s,id,flag,out,&result)==dh2::player_manager_host_level::Status::complete?0:1;
        };
        services.context=this;services.queries=&queries;
        services.fields=[](void* raw,r::PlayerInfo* p,r::SourceFields* out){auto& f=static_cast<Fixture*>(raw)->backing(p);
            *out={p,&f.slot,&f.controller,&f.local,&f.id,&f.member,&f.number,&f.group,&f.state,nullptr,nullptr};return 0;};
        services.construct_temporary=[](void* raw,r::PlayerInfo** out){auto& f=*static_cast<Fixture*>(raw);int status=f.note(2);if(status)return status;f.temporary.slot=f.temporary.controller=f.temporary.id=f.temporary.member=f.temporary.number=f.temporary.group=-1;f.temporary.local=1;f.temporary.state=0;*out=&f.temporary.projection;return 0;};
        services.map_subscript=[](void* raw,r::Registry* registry,int key,r::PlayerInfo** out){auto& f=*static_cast<Fixture*>(raw);if(registry!=&f.registry)return 1;int status=f.note(3,key);if(status)return status;auto& entry=f.entries.try_emplace(key,key).first->second;f.rebuild();*out=&entry.projection;return 0;};
        services.assign=[](void* raw,r::PlayerInfo* d,const r::PlayerInfo* s){auto& f=*static_cast<Fixture*>(raw);int status=f.note(4,d->internal_id);if(status)return status;copy(f.backing(d),f.backing(const_cast<r::PlayerInfo*>(s)));return 0;};
        services.destroy_temporary=[](void* raw,r::PlayerInfo* p){auto& f=*static_cast<Fixture*>(raw);if(p!=&f.temporary.projection)return 1;return f.note(5);};
        services.input_manager=[](void* raw,std::uintptr_t* out){auto& f=*static_cast<Fixture*>(raw);*out=0x10013000;return f.note(7);};
        services.gamepad_count=[](void* raw,std::uintptr_t input,int* out){auto& f=*static_cast<Fixture*>(raw);if(input!=0x10013000)return 1;*out=static_cast<int>(f.row[7]);return f.note(8,*out);};
        services.gamepad=[](void* raw,std::uintptr_t input,unsigned index,r::InputDevice* out){auto& f=*static_cast<Fixture*>(raw);if(input!=0x10013000||index>=4)return 1;*out={0x10020000u+index*0x1000u,&f.connected[index],&f.pressed,&f.axis,&f.minimum,&f.maximum};return f.note(9,index);};
        services.joining_controller=[](void* raw,r::Registry* registry,unsigned index,int id){auto& f=*static_cast<Fixture*>(raw);if(registry!=&f.registry)return 1;return f.note(10,index,id);};
        services.current_level=[](void* raw,std::uintptr_t* out){auto& f=*static_cast<Fixture*>(raw);*out=f.row[15]?0x10014000:0;return f.note(11,*out!=0);};
        services.hud_root=[](void* raw,std::uintptr_t* out){auto& f=*static_cast<Fixture*>(raw);*out=0x10015000;return f.note(12);};
        services.numeric_value=[](void* raw,int number,std::uintptr_t* out){auto& f=*static_cast<Fixture*>(raw);*out=0x10016000;return f.note(13,number);};
        services.invoke_as=[](void* raw,std::uintptr_t hud,const char* movie,const char* function,std::uintptr_t value){auto& f=*static_cast<Fixture*>(raw);if(hud!=0x10015000||value!=0x10016000||std::strcmp(movie,"menu_HUD_0")||std::strcmp(function,"onNewPlayerLocal"))return 1;return f.note(14);};
        services.drop_as_value=[](void* raw,std::uintptr_t value){if(value!=0x10016000)return 1;return static_cast<Fixture*>(raw)->note(15);};
        services.set_state=[](void* raw,r::PlayerInfo* p,std::uint8_t value){auto& f=*static_cast<Fixture*>(raw);int status=f.note(16,p->internal_id,value);if(status)return status;f.backing(p).state=value;return 0;};
    }
    r::Result run(bool& found){r::Result result;r::Runtime runtime(registry,services);r::Status status;
        if(row[0]==0)status=r::is_player_in_local_map(&registry,static_cast<int>(row[1]),&found,&result);
        else if(row[0]==1)status=runtime.add_player(static_cast<int>(row[1]),static_cast<int>(row[2]),static_cast<int>(row[3]),static_cast<unsigned>(row[4]),&result);
        else if(row[0]==2)status=runtime.update_player_numbers(&result);
        else status=runtime.check_local_controllers(&result);
        if(status!=result.status)throw std::runtime_error("status mismatch");
        return result;
    }
    void print(bool found){std::cout<<"{\"found\":"<<(found?"true":"false")<<",\"players\":[";bool comma=false;
        for(auto& [key,f]:entries){if(comma)std::cout<<',';comma=true;std::cout<<'['<<key<<','<<f.slot<<','<<f.controller<<','<<unsigned(f.local)<<','<<f.id<<','<<f.member<<','<<f.number<<','<<f.group<<','<<unsigned(f.state)<<']';}
        std::cout<<"],\"trace\":[";comma=false;for(auto& e:events){if(comma)std::cout<<',';comma=true;std::cout<<'['<<e[0]<<','<<e[1]<<','<<e[2]<<']';}std::cout<<"]}";
    }
};
unsigned failure_checks(){unsigned checks=0;
    for(int scenario=0;scenario<4;++scenario){Row row{{scenario?3:1,0,-1,0,1,scenario?1:0,5,1,0,-1,scenario==1?2:0,0x3f800000,0,0,0,1}};
        Fixture baseline(row);bool found=false;auto good=baseline.run(found);if(good.status!=r::Status::complete)throw std::runtime_error("failure baseline");
        for(int kind=0;kind<2;++kind)for(unsigned index=0;index<baseline.events.size();++index){Fixture failed(row);if(kind)failed.throw_at=static_cast<int>(index);else failed.fail_at=static_cast<int>(index);auto result=failed.run(found);
            if(result.status!=r::Status::service_failed||failed.events.size()!=index+1||!std::equal(failed.events.begin(),failed.events.end(),baseline.events.begin()))throw std::runtime_error("failure did not retain exact provider prefix");
            ++checks;}
        Fixture reentry(row);r::Runtime runtime(reentry.registry,reentry.services);reentry.reenter=&runtime;r::Result result;
        auto status=scenario?runtime.check_local_controllers(&result):runtime.add_player(0,-1,0,1,&result);
        if(status!=r::Status::complete||reentry.nested!=r::Status::reentrant)throw std::runtime_error("source owner reentry guard");
        ++checks;
    }
    return checks;
}
int main(int argc,char** argv){try{if(argc!=3)return 2;unsigned failures=failure_checks();std::ofstream policies(argv[2]);policies<<"{\"validation\":\"PASS\",\"failure_and_reentry_cases\":"<<failures<<"}\n";std::ifstream stream(argv[1]);if(!stream)return 2;Row row;std::cout<<'[';bool comma=false;
    while(stream>>row[0]){for(unsigned i=1;i<row.size();++i)if(!(stream>>row[i]))return 2;Fixture f(row);bool found=false;auto result=f.run(found);if(result.status!=r::Status::complete)throw std::runtime_error("caller did not complete");if(comma)std::cout<<',';comma=true;f.print(found);}std::cout<<"]\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
