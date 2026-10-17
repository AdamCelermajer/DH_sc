#include "../player_savegame_v1.hpp"
#include <array>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <stdexcept>
#include <vector>
using namespace dh2::data;
using Event=std::array<unsigned,4>;
struct Fixture {
    std::map<void*,unsigned> identities;
    std::vector<Event> events;
    std::array<std::vector<std::int32_t>,2> defaults{{{1,2,3},{4,5,6}}};
    std::array<std::uint32_t,2> counts{{3,3}};
    std::array<bool,6> seen{};
    unsigned next=16,table=0,mutation=0,zero_null=0,attempts=0,cursor=0;
    int fail_at=-1,throw_at=-1;
    bool changed=false,reenter=false,nested=true;
    bool pending=false;unsigned pending_slot=0,pending_row=0;std::int32_t pending_word=0;
    bool pending_publish=false;unsigned publish_slot=0,publish_count=0;
    PlayerSavegameV1 save; // Retired before allocator callback context members.
    SavedStateArrayV1& slot(unsigned i){return *(i%2?save.source_world_map_states(i/2):save.source_level_states(i/2));}
    static void release(void* context,void* pointer) {
        auto& f=*static_cast<Fixture*>(context);f.identities.erase(pointer);std::free(pointer);
    }
    static void* allocate(void* context,std::size_t bytes,int tag) {
        auto& f=*static_cast<Fixture*>(context);f.events.push_back({2,static_cast<unsigned>(bytes),static_cast<unsigned>(tag),f.next});
        const int attempt=static_cast<int>(f.attempts++);
        if(attempt==f.throw_at)throw std::runtime_error("state allocator failure");
        if(attempt==f.fail_at)return nullptr;
        void* pointer=bytes||!f.zero_null?std::malloc(bytes?bytes:4):nullptr;
        if(pointer){std::memset(pointer,0x5a,bytes?bytes:4);f.identities[pointer]=f.next;}
        while(f.cursor<6&&(f.cursor%2!=f.table||f.slot(f.cursor).words))++f.cursor;
        if(f.cursor>=6)throw std::runtime_error("fixture missing reached saved array");
        f.publish_slot=f.cursor++;f.publish_count=static_cast<unsigned>(bytes/4);f.pending_publish=true;
        ++f.next;
        if(!f.changed&&f.mutation&&f.table==0) {
            if(f.mutation==1)f.counts[0]=1;
            if(f.mutation==3)f.defaults[0]={101,202,303};
            if(f.mutation==1||f.mutation==3)f.changed=true;
        }
        if(!f.changed&&f.mutation==4&&f.table==1){f.counts[1]=0;f.changed=true;}
        return pointer;
    }
    bool step(){const int attempt=static_cast<int>(attempts++);if(attempt==throw_at)throw std::runtime_error("state table failure");return attempt!=fail_at;}
    void published() {
        if(pending_publish){auto& a=slot(publish_slot);if(a.count!=publish_count)throw std::runtime_error("published saved-array capacity differs");events.push_back({4,publish_slot,a.words?identities.at(a.words):0,a.count});pending_publish=false;}
        if(pending) {
            auto& a=slot(pending_slot);if(a.words[pending_row]!=pending_word)throw std::runtime_error("missing preceding source saved state store");
            events.push_back({5,identities.at(a.words),pending_row,static_cast<unsigned>(pending_word)});pending=false;
        }
    }
    static bool count(void* context,SavedStateTableV1 t,std::uint32_t* value,std::string&) {
        auto& f=*static_cast<Fixture*>(context);f.table=static_cast<unsigned>(t);f.published();
        *value=f.counts[f.table];f.events.push_back({1,f.table,*value,0});
        if(f.reenter) {f.reenter=false;std::string error;f.nested=f.save.initialize_level_states(f.services(),error);}
        return f.step();
    }
    static bool word(void* context,SavedStateTableV1 t,unsigned row,std::int32_t* value,std::string&) {
        auto& f=*static_cast<Fixture*>(context);const unsigned table=static_cast<unsigned>(t);
        *value=f.defaults[table].at(row);f.events.push_back({3,table,row,static_cast<unsigned>(*value)});
        if(!f.step())return false;
        unsigned selected=0;
        // Later difficulties of the same table use the most recent allocation.
        unsigned newest=0;
        for(unsigned d=0;d<3;++d){const unsigned s=d*2+table;auto* p=f.slot(s).words;if(p&&f.identities.at(p)>=16&&f.identities.at(p)<100&&f.identities.at(p)>newest){newest=f.identities.at(p);selected=s;}}
        f.pending=true;f.pending_slot=selected;f.pending_row=row;f.pending_word=*value;
        if(!f.changed&&f.mutation==2&&table==0&&row==0){f.counts[0]=1;f.changed=true;}
        return true;
    }
    SavedLevelStateServicesV1 services(){return {this,count,word,{this,allocate,release}};}
    void seed(unsigned mask) {
        for(unsigned i=0;i<6;++i)if(mask&(1u<<i)) {
            auto& array=slot(i);array.words=static_cast<int*>(std::malloc(8));array.count=2;array.memory={this,allocate,release};
            array.words[0]=1000+static_cast<int>(i);array.words[1]=-2000-static_cast<int>(i);identities[array.words]=100+i;seen[i]=true;
        }
    }
    void project(std::FILE* output) {
        for(unsigned i=0;i<6;++i) {
            auto& array=slot(i);unsigned values[]={array.words?identities.at(array.words):0,array.count};
            std::fwrite(values,sizeof(values),1,output);
            if(array.words)std::fwrite(array.words,4,array.count,output);
        }
        unsigned length=static_cast<unsigned>(events.size());std::fwrite(&length,4,1,output);
        for(const auto& event:events)std::fwrite(event.data(),sizeof(event),1,output);
    }
};
static unsigned policies() {
    unsigned checks=0;std::string error;
    {Fixture f;auto services=f.services();services.count=nullptr;if(f.save.initialize_level_states(services,error)||f.slot(0).words)return 0;++checks;}
    {Fixture f;auto services=f.services();services.memory.release=nullptr;if(f.save.initialize_level_states(services,error)||f.slot(0).words)return 0;++checks;}
    {Fixture f;auto services=f.services();services.memory.allocate=nullptr;if(f.save.initialize_level_states(services,error)||f.slot(0).words)return 0;++checks;}
    {Fixture f;auto services=f.services();services.default_word=nullptr;if(f.save.initialize_level_states(services,error)||!f.slot(0).words||f.slot(1).words)return 0;++checks;}
    {Fixture f;f.counts[0]=65537;if(f.save.initialize_level_states(f.services(),error)||f.slot(0).words)return 0;++checks;}
    {Fixture f;f.reenter=true;if(!f.save.initialize_level_states(f.services(),error)||f.nested)return 0;++checks;}
    {Fixture f;f.seed(63);if(!f.save.initialize_level_states({},error)||!f.events.empty())return 0;++checks;}
    {Fixture f;f.seed(63);if(f.save.source_level_states(3)||f.save.source_world_map_states(99))return 0;++checks;}
    Fixture baseline;if(!baseline.save.initialize_level_states(baseline.services(),error))return 0;
    for(unsigned step=0;step<baseline.attempts;++step)for(bool throwing:{false,true}) {
        Fixture f;if(throwing)f.throw_at=static_cast<int>(step);else f.fail_at=static_cast<int>(step);
        error.clear();if(f.save.initialize_level_states(f.services(),error))return 0;
        if(f.events.size()>baseline.events.size()||!std::equal(f.events.begin(),f.events.end(),baseline.events.begin()))return 0;
        // Every completed source allocation and earlier default store remains
        // owned/readable at a later provider failure; no rollback is inferred.
        for(unsigned i=0;i<6;++i)if(f.slot(i).words&&f.slot(i).count!=3)return 0;
        ++checks;
    }
    return checks;
}
int main(int argc,char** argv) {
    if(argc!=3)return 2;
    auto* input=std::fopen(argv[1],"rb");auto* output=std::fopen(argv[2],"wb");if(!input||!output)return 3;
    unsigned count;if(std::fread(&count,4,1,input)!=1)return 4;
    const unsigned policy_checks=policies();if(!policy_checks)return 5;
    for(unsigned row=0;row<count;++row) {
        unsigned fields[6];if(std::fread(fields,sizeof(fields),1,input)!=1)return 6;
        Fixture f;f.counts={fields[1],fields[2]};f.mutation=fields[3];f.zero_null=fields[4];f.seed(fields[0]);
        std::string error;if(!f.save.initialize_level_states(f.services(),error))return 7;
        if(fields[5]){f.cursor=0;if(!f.save.initialize_level_states(f.services(),error))return 8;}
        f.project(output);
    }
    std::fclose(input);std::fclose(output);std::printf("%u\n",policy_checks);return 0;
}
