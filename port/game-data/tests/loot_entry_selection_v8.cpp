#include "../loot_entry_selection_v8.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
using namespace dh2::data;
namespace {
unsigned checks{};void check(bool v){++checks;if(!v)throw std::runtime_error("Original loot entry result/query/RNG mismatch");}
struct Fixture {int infinite{},counts[3]{};std::vector<unsigned> calls;
 static bool service(void* p,const LootEntryRequestV8& q,int& out,std::string& error){auto& f=*static_cast<Fixture*>(p);using O=LootEntryOperationV8;switch(q.operation){
 case O::debug_load:f.calls.push_back(0);out=0;return true;
 case O::debug_query:if(!q.key)return false;if(!std::strcmp(q.key,"InfiniteLootDrops")){f.calls.push_back(1);out=f.infinite;return true;}if(!std::strcmp(q.key,"isTracingItemPctRoll")){f.calls.push_back(2);out=0;return true;}return false;
 case O::mage_count:f.calls.push_back(3);out=f.counts[0];return true;
 case O::rogue_count:f.calls.push_back(4);out=f.counts[1];return true;
 case O::warrior_count:f.calls.push_back(5);out=f.counts[2];return true;
 default:error="Required original assertion receiver";return false;
 }};
};
unsigned read(std::ifstream& f){unsigned v;f.read(reinterpret_cast<char*>(&v),4);check(bool(f));return v;}
}
int main(int argc,char** argv){try{check(argc==2);std::ifstream file(argv[1],std::ios::binary);char magic[4];file.read(magic,4);check(!std::memcmp(magic,"LES8",4));const unsigned n=read(file);for(unsigned i=0;i<n;++i){unsigned op=read(file),count=read(file);LootRandom8V2 random{read(file),read(file)};Fixture f;f.infinite=int(read(file));for(auto& c:f.counts)c=int(read(file));std::vector<LootEntry32V2> entries(count);std::vector<const LootEntry32V2*> pointers;for(auto& e:entries){file.read(reinterpret_cast<char*>(e.words),32);check(bool(file));pointers.push_back(&e);}unsigned bytes=read(file),wanted=read(file),seed=read(file),calls=read(file),transcript=read(file);check(bytes==16+transcript*4);std::vector<unsigned> expected(transcript);for(auto& v:expected)v=read(file);LootEntrySelectionV8 selector(random,{&f,Fixture::service});std::string error;unsigned actual=0;bool flag;int weight;bool ok=false;switch(op){case 0:ok=selector.uses_percent(entries[0],flag,error);actual=flag;break;case 1:ok=selector.effective_weight(entries[0],weight,error);std::memcpy(&actual,&weight,4);break;case 2:ok=selector.percent_roll(entries[0],flag,error);actual=flag;break;case 3:case 4:ok=selector.weighted_index(pointers,actual,op==3,error);break;}if(!ok||actual!=wanted||random.seed!=seed||random.calls!=calls||f.calls!=expected){std::cerr<<"case "<<i<<" op "<<op<<" expected "<<wanted<<" actual "<<actual<<" error "<<error<<'\n';return 1;}check(true);}
 LootRandom8V2 random{123456789,0};LootEntrySelectionV8 missing(random,{});LootEntry32V2 entry{};bool flag=false;std::string error;check(!missing.uses_percent(entry,flag,error)&&!error.empty()&&random.calls==0);
 std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<n<<",\"checks\":"<<checks<<",\"exact_Query_RNG_transcripts\":true,\"full_AddLoot\":false}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
