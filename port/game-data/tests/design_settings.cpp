#include "../design_settings.hpp"
#include "../../level-world/character_enemy_spotted.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dh2::data;
namespace {
using Raw=std::vector<std::uint8_t>;unsigned checks=0,guards=0,rows=0,tables=0,lookups=0,wordchecks=0;
void require(bool ok,unsigned line){++checks;if(!ok)throw std::runtime_error("DesignSettings line "+std::to_string(line));}
#define check(x) require(bool(x),__LINE__)
std::uint32_t word(std::istream& f){std::uint32_t w=0;f.read(reinterpret_cast<char*>(&w),4);check(f);return w;}
Raw blob(std::istream& f){Raw b(word(f));if(!b.empty())f.read(reinterpret_cast<char*>(b.data()),b.size());check(f);return b;}
Raw file(const char* p){std::ifstream f(p,std::ios::binary);check(f);return {std::istreambuf_iterator<char>(f),{}};}
void push(Raw& b,std::uint32_t w){for(unsigned i=0;i<4;++i)b.push_back(static_cast<std::uint8_t>(w>>(8*i)));}
Bytes bytes(const Raw& b){return {b.data(),b.size()};}
struct Threat {
 unsigned calls=0,adds=0;const std::uint32_t* word=nullptr;
 static int invoke(void* p,dh2::character::EnemySpottedState16*,const dh2::character::EnemySpottedRequest48* r,std::uint32_t* out){auto& t=*static_cast<Threat*>(p);++t.calls;*out=0;if(r->service==dh2::character::enemy_add_aggro){check(r->word==*t.word&&r->word==0x41200000);*out=r->word;++t.adds;}return 0;}
};
}
int main(int argc,char** argv){try{
 check(argc==5);std::ifstream gold(argv[1],std::ios::binary);check(word(gold)==0x31535344);auto nr=word(gold),nt=word(gold),nq=word(gold),nf=word(gold);check(nr==385&&nt==97);
 DesignSettingsProjection176 result{},expected{};
 for(unsigned i=0;i<nr;++i){auto input=blob(gold);gold.read(reinterpret_cast<char*>(&expected),176);check(gold);std::uint32_t used=0;check(!dh2_design_settings_decode_record(&result,&used,input.data(),input.size())&&used==172&&!std::memcmp(&result,&expected,176));++rows;wordchecks+=44;}
 for(unsigned i=0;i<nt;++i){auto input=blob(gold);auto count=word(gold);std::vector<DesignSettingsProjection176> expected(count),output(count);if(count)gold.read(reinterpret_cast<char*>(expected.data()),count*176);check(gold);std::uint32_t used=0,actual=UINT32_MAX;check(!dh2_design_settings_decode_table(output.data(),count,&actual,&used,input.data(),input.size())&&actual==count&&used==4+count*172);check(!count||!std::memcmp(output.data(),expected.data(),count*176));++tables;wordchecks+=count*44;}
 auto record=file(argv[2]),names=file(argv[3]),schema=file(argv[4]);std::string error;auto owner=std::make_unique<DesignSettingsOwner>();check(owner->load(bytes(record),bytes(names),bytes(schema),error));auto pinned=owner->borrow();check(pinned.rows().size()==1&&pinned.row_names().size()==1&&pinned.fields().size()==43&&pinned.records_consumed()==176&&pinned.names_consumed()==15);check(pinned.field_index("EnemySpottedAggro")==11&&pinned.row_index("Default")==0);
 for(unsigned i=0;i<nq+nf;++i){auto raw=blob(gold);raw.push_back(0);auto want=word(gold);auto found=i<nq?pinned.row_index(reinterpret_cast<const char*>(raw.data())):pinned.field_index(reinterpret_cast<const char*>(raw.data()));check(static_cast<std::uint32_t>(found)==want);++lookups;}
 for(unsigned i=0;i<43;++i)check(design_settings_field_kind(i)==word(gold));check(gold.peek()==EOF&&design_settings_field_kind(43)==design_unknown);check(*pinned.enemy_spotted_aggro_bits()==0x41200000&&!pinned.word(1,0)&&!pinned.word(0,43));
 // Atomic owner failure and reload pinning are explicit native contracts.
 check(!owner->load(bytes(record),bytes(names),bytes(schema),error)&&error.find("pinned")!=std::string::npos);++guards;
 const auto* threat=pinned.enemy_spotted_aggro_bits();auto schema_used=pinned.schema_consumed();check(schema_used<schema.size());pinned={};
 for(unsigned n=0;n<176;++n){check(!owner->load({record.data(),n},bytes(names),bytes(schema),error)&&*owner->borrow().enemy_spotted_aggro_bits()==0x41200000);++guards;}
 for(unsigned n=0;n<15;++n){check(!owner->load(bytes(record),{names.data(),n},bytes(schema),error)&&owner->borrow().rows().size()==1);++guards;}
 for(unsigned n=0;n<schema_used;++n){check(!owner->load(bytes(record),bytes(names),{schema.data(),n},error)&&owner->borrow().fields().size()==43);++guards;}
 auto wrong=schema;wrong[8]^=1;check(!owner->load(bytes(record),bytes(names),bytes(wrong),error));++guards;
 DesignSettingsProjection176 sentinel;std::memset(&sentinel,0xa5,sizeof sentinel);result=sentinel;std::uint32_t used=0xa5a5a5a5,count=0xbcbcbcbc;
 for(unsigned n=0;n<172;++n){check(dh2_design_settings_decode_record(&result,&used,record.data()+4,n)==1&&!std::memcmp(&result,&sentinel,176)&&used==0xa5a5a5a5);++guards;}
 check(dh2_design_settings_decode_table(&result,0,&count,&used,record.data(),record.size())==2&&count==0xbcbcbcbc&&used==0xa5a5a5a5&&!std::memcmp(&result,&sentinel,176));++guards;
 check(dh2_design_settings_decode_record(&result,reinterpret_cast<std::uint32_t*>(&result),record.data()+4,172)==1);++guards;
 check(dh2_design_settings_decode_table(&result,1,&count,&count,record.data(),record.size())==1);++guards;
 alignas(8)Raw storage(192);check(dh2_design_settings_decode_record(reinterpret_cast<DesignSettingsProjection176*>(storage.data()+1),&used,record.data()+4,172)==1);++guards;
 // Complete source ordered names allow duplicates and first-match lookup.
 Raw duplicate;push(duplicate,3);for(const char* s:{"Default","Default","Other"}){push(duplicate,std::strlen(s));duplicate.insert(duplicate.end(),s,s+std::strlen(s));}
 Raw three;push(three,3);for(unsigned i=0;i<3;++i)three.insert(three.end(),record.begin()+4,record.begin()+176);DesignSettingsOwner duplicated;check(duplicated.load(bytes(three),bytes(duplicate),bytes(schema),error));check(duplicated.borrow().row_index("Default")==0&&duplicated.borrow().row_index("Other")==2);
 Raw empty(4);DesignSettingsOwner zero;check(zero.load(bytes(empty),bytes(empty),bytes(schema),error));check(zero.borrow().rows().empty()&&!zero.borrow().enemy_spotted_aggro_bits());
 // Snapshot owns bits/names after all input storage and owner are destroyed.
 pinned=owner->borrow();check(pinned.enemy_spotted_aggro_bits()==threat);owner.reset();record.clear();record.shrink_to_fit();names.clear();names.shrink_to_fit();schema.clear();schema.shrink_to_fit();check(*threat==0x41200000&&pinned.field_index("EnemySpottedAggro")==11&&pinned.row_index("Default")==0);
 // Actual source coordinator consumes the pinned word, replacing gold-borrowed
 // threat. Its world/debug/group/state/aggro services remain explicit fixtures.
 using namespace dh2::character;TargetOwner16 projected{1,0,0,0};TargetState48 ai{2,&projected,0,0,0,0,0,0,0,0};TargetEventState32 prefix{&ai,0,0,{},0};EnemySpottedState16 event{&prefix,0};Threat t;t.word=threat;EnemySpottedServices24 services{&t,Threat::invoke,threat};check(!dh2_character_enemy_spotted(&event,3,&services)&&t.adds==1&&t.calls==15);
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"record_comparisons\":"<<rows<<",\"table_comparisons\":"<<tables<<",\"word_comparisons\":"<<wordchecks<<",\"lookup_comparisons\":"<<lookups<<",\"native_guards\":"<<guards<<",\"actual_cache_rows\":1,\"pinned_after_owner_and_input_destruction\":true,\"owned_threat_enemy_prefix_calls\":"<<t.calls<<",\"whole_Application_registration\":false}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
