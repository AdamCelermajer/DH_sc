#include "../player_metadata_prepare_v1.hpp"
#include "../player_info_activity_v1.hpp"
#include "../../android-native/app/src/main/cpp/native_player_profile.hpp"
#include "data.hpp"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>

namespace p=dh2::player_info_record_v1;
namespace m=dh2::player_metadata_prepare_v1;
namespace profile=dh2::native::player_profile;
using Raw=std::vector<std::uint8_t>;
namespace {
unsigned checks=0;
void require(bool value){if(!value)throw std::runtime_error("metadata composition check "+std::to_string(checks+1));++checks;}
Raw read(const std::filesystem::path& path){std::ifstream f(path,std::ios::binary);require(bool(f));return Raw(std::istreambuf_iterator<char>(f),{});}
dh2::data::Bytes bytes(const Raw& raw){return {raw.data(),raw.size()};}
void put(Raw& raw,std::uint32_t value){for(unsigned i=0;i<4;++i)raw.push_back(std::uint8_t(value>>(i*8)));}
Raw words(std::initializer_list<std::uint32_t> values){Raw raw;for(auto value:values)put(raw,value);return raw;}
Raw text(const std::string& value){Raw raw;put(raw,std::uint32_t(value.size()+1));raw.insert(raw.end(),value.begin(),value.end());raw.push_back(0);return raw;}
Raw campaign(const std::string& name,const std::string& class_name,unsigned level) {
    const std::vector<std::pair<std::string,Raw>> sections{{"PNAM",text(name)},{"PCLS",text(class_name)},
        {"PLVL",words({level})},{"PDFL",words({0,1})},{"LNAM",words({0x10002,1,2,3,4,5,6,7,8,9})},
        {"LEPT",words({0,0,0})},{"LUSP",Raw{1,0,1}}};
    Raw raw;put(raw,std::uint32_t(sections.size()));
    for(const auto& section:sections){put(raw,std::uint32_t(section.second.size()));raw.insert(raw.end(),section.first.begin(),section.first.end());raw.insert(raw.end(),section.second.begin(),section.second.end());}
    return raw;
}
void write(const std::filesystem::path& path,const Raw& raw){std::ofstream f(path,std::ios::binary|std::ios::trunc);require(bool(f));f.write(reinterpret_cast<const char*>(raw.data()),std::streamsize(raw.size()));require(bool(f));}
// This test adapter lends platform allocation/deletion and table/file owners.
// The actual selected Factory, Record D1, Prepare, Metadata/Transport and Save
// index/readers execute; this is not a replacement NativeHostPlayer.
struct Owner {
    struct Entry {profile::Metadata owner;m::SaveRef source{owner.save_identity(),&owner.save()};};
    std::map<std::uintptr_t,std::unique_ptr<Entry>> entries;
    std::uint64_t serial=0;unsigned deleted=0,allocations=0,constructors=0;
    const dh2::data::CharacterTable& table;std::filesystem::path directory;int difficulty=0;
    std::string error;bool fail_after_publish=false;unsigned objects=0;
    p::Factory factory;
    explicit Owner(const dh2::data::CharacterTable& data,std::filesystem::path path)
        :table(data),directory(std::move(path)),factory(&serial,
          {this,[](void* c,std::size_t size,int tag)->void*{auto& s=*static_cast<Owner*>(c);if(tag==0){require(size==sizeof(p::Record));++s.objects;}return std::malloc(size);},
                [](void*,void* value){std::free(value);}}, {},this,
          [](void* c,std::uintptr_t identity){auto& s=*static_cast<Owner*>(c);if(!s.entries.erase(identity))return p::Status::missing_provider;++s.deleted;return p::Status::complete;}){}
    m::Services services() {
        m::Services services{};services.context=this;
        services.is_active=[](void*,p::Record* record,int* out){bool active=false;const dh2::player_info_activity_v1::Services queries{nullptr,[](void*,std::uint8_t* out)->int{*out=0;return 0;}};
            const auto status=dh2::player_info_activity_v1::player_is_active(*record,queries,&active);*out=active;return status==p::Status::complete?0:1;};
        services.allocate_save=[](void* c,unsigned size,unsigned tag,std::uintptr_t* out){auto& s=*static_cast<Owner*>(c);require(size==0x198&&tag==0);auto entry=std::make_unique<Entry>();*out=entry->source.identity;++s.allocations;return s.entries.emplace(*out,std::move(entry)).second?0:1;};
        services.construct_indexed_save=[](void* c,std::uintptr_t identity,unsigned slot,int mask,bool skip,m::SaveRef** out){auto& s=*static_cast<Owner*>(c);auto entry=s.entries.find(identity);require(entry!=s.entries.end()&&mask==1&&!skip&&slot<=INT32_MAX);++s.constructors;
            if(!entry->second->owner.load(std::int32_t(slot),s.directory,s.table,s.difficulty,s.error))return 1;
            *out=&entry->second->source;return 0;};
        services.save_by_identity=[](void* c,std::uintptr_t identity,m::SaveRef** out){auto& s=*static_cast<Owner*>(c);auto entry=s.entries.find(identity);if(entry==s.entries.end())return 1;*out=&entry->second->source;return 0;};
        services.debug_name_11=[](void* c,std::uint8_t* out){auto& s=*static_cast<Owner*>(c);*out=0;return s.fail_after_publish?1:0;};
        services.game_state_name_28=[](void*,std::uint8_t* out){*out=0;return 0;};return services;
    }
    p::Record* create(int slot){p::Record* record=nullptr;require(factory.create_record(&record)==p::Status::complete&&record);record->save_slot_664=slot;return record;}
    m::Status prepare(p::Record& record,m::Result& result){m::Runtime runtime(record,services());return runtime.prepare({},&result);}
};
}
int main(int argc,char** argv){try {
    require(argc==3);const std::filesystem::path cache=argv[1],directory=argv[2];std::filesystem::create_directories(directory);
    auto values=read(cache/"character_properties_pyarray.bin"),names=read(cache/"character_properties_pyarraynames.bin"),fields=read(cache/"character_properties_pystructnames.bin");
    dh2::data::CharacterTable table;std::string error;require(dh2::data::load_characters(bytes(values),bytes(names),bytes(fields),table,error));
    const auto knight=std::find(table.names.begin(),table.names.end(),"KnightPlayerBase");require(knight!=table.names.end());
    const auto first=std::int32_t(knight-table.names.begin()),second=first+1;require(std::size_t(second)<table.names.size()&&table.names[first]!=table.names[second]);
    write(directory/"dh2_000.savegame",campaign("MetadataLease0",table.names[first],17));
    write(directory/"dh2_001.savegame",campaign("MetadataLease1",table.names[second],5));
    write(directory/"dh2_002.savegame",words({UINT32_MAX}));
    Owner owner(table,directory);m::Result result;
    auto* record=owner.create(0);require(owner.prepare(*record,result)==m::Status::complete&&result.disposition==m::Disposition::prepared);
    require(result.published_save_writes==1&&record->loading_info_680&&owner.entries.size()==1&&owner.allocations==1&&owner.constructors==1);
    const auto identity=record->loading_info_680;const auto& saved=owner.entries.at(identity)->owner;
    require(identity==saved.save_identity()&&saved.save().character()==0&&saved.save().slot()==0&&saved.profile_identity());
    require(record->character_660==0&&record->at(0x360)->header.value==first&&record->at(0x310)->header.value==17&&record->at(0x2b0)->text=="MetadataLease0");
    const auto serial=owner.serial;require(owner.prepare(*record,result)==m::Status::complete&&record->loading_info_680==identity);
    require(!result.published_save_writes&&!result.name_setters&&!result.class_setters&&!result.level_setters&&owner.serial==serial&&owner.allocations==1&&saved.receipt().file_opens==1);
    require(owner.factory.delete_record(record)==p::Status::complete&&owner.entries.empty()&&owner.deleted==1);
    record=owner.create(1);require(owner.prepare(*record,result)==m::Status::complete);
    require(record->at(0x360)->header.value==second&&record->at(0x310)->header.value==5&&record->at(0x2b0)->text=="MetadataLease1"&&owner.entries.at(record->loading_info_680)->owner.save().slot()==1);
    require(owner.factory.delete_record(record)==p::Status::complete&&owner.entries.empty()&&owner.deleted==2);
    // Real corrupt-file/index rejection leaves allocation owned but does not
    // publish680 or substitute default class/level/name into this Record.
    record=owner.create(2);require(owner.prepare(*record,result)==m::Status::service_failed&&result.last_operation==m::Operation::construct_indexed_save);
    require(!record->loading_info_680&&owner.entries.size()==1&&record->at(0x360)->header.value==-1&&record->at(0x310)->header.value==-1);
    require(owner.factory.delete_record(record)==p::Status::complete&&owner.entries.size()==1&&owner.deleted==2);
    // A failed delivery AFTER constructor/publication retains the actual680
    // lease; genuine Record D1 invokes its existing factory deleting provider.
    owner.fail_after_publish=true;record=owner.create(0);require(owner.prepare(*record,result)==m::Status::service_failed&&result.last_operation==m::Operation::debug_name);
    require(record->loading_info_680&&result.published_save_writes==1&&owner.entries.size()==2);
    require(owner.factory.delete_record(record)==p::Status::complete&&owner.entries.size()==1&&owner.deleted==3);
    require(owner.objects==4&&owner.allocations==4&&owner.constructors==4);
    std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"real_record_deletions\":4,\"metadata_deleting_calls\":"<<owner.deleted<<",\"corrupt_unpublished_allocation_retained\":true,\"two_distinct_slots\":true,\"scope\":\"Actual selected Factory/Record/Activity/Prepare/Save index readers with native Metadata transport and synthetic profiles; platform lease map is declared test adapter, not NativeHostPlayer/live teardown\"}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
