#include "../player_savegame_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::data;using B=std::vector<std::uint8_t>;
void check(bool b){if(!b)throw std::runtime_error("saved faery mismatch");}
struct R{B b;std::size_t at{};std::uint32_t word(){check(at+4<=b.size());std::uint32_t x=0;for(int i=0;i<4;++i)x|=std::uint32_t(b[at++])<<(8*i);return x;}B bytes(std::size_t n){check(n<=b.size()-at);B a(b.begin()+at,b.begin()+at+n);at+=n;return a;}};
void word(B& b,std::uint32_t x){for(int i=0;i<4;++i)b.push_back(std::uint8_t(x>>(8*i)));}
B snapshot(PlayerSavegameV1& p){B b;for(auto& set:p.faeries())for(auto row:set){b.push_back(row.state);b.push_back(0);b.push_back(std::uint8_t(row.level));b.push_back(std::uint8_t(row.level>>8));}for(auto id:p.current_faeries())word(b,id);word(b,p.unlocked_difficulty());return b;}
int main(int argc,char** argv){try{check(argc==2);std::ifstream f(argv[1],std::ios::binary);check(bool(f));R r{{std::istreambuf_iterator<char>(f),{}}};check(r.bytes(4)==B({'F','S','G','1'}));auto count=r.word();std::uint32_t comparisons=0,queries=0,mismatches=0;std::string error;
 for(std::uint32_t k=0;k<count;++k){PlayerSavegameV1 owner;owner.initialize_faeries();auto n=r.word();while(n--){R op{r.bytes(r.word())};std::size_t used=0;bool mismatch=false;auto type=op.word();switch(type){
  case 0:owner.initialize_faeries();break;
  case 1:case 2:{auto id=op.word();auto value=op.word();auto difficulty=op.word();check(type==1?owner.set_faery_level(id,static_cast<std::int32_t>(value),difficulty,error):owner.set_faery_state(id,static_cast<std::int32_t>(value),difficulty,error));break;}
  case 3:case 4:case 5:{auto blob=op.bytes(op.word());if(type==3){check(owner.load_faeries({blob.data(),blob.size()},used,mismatch,error));mismatches+=mismatch;}else if(type==4)check(owner.load_current_faery({blob.data(),blob.size()},used,error));else{std::int32_t selected=-999;check(owner.load_difficulty({blob.data(),blob.size()},&selected,[](void* p,std::int32_t value,std::string&){*static_cast<std::int32_t*>(p)=value;return true;},used,error));check(selected==static_cast<std::int32_t>(blob[0]));}break;}
  default:check(false);
 }check(used==r.word());check(snapshot(owner)==r.bytes(76));++comparisons;for(std::uint32_t d=0;d<3;++d)for(std::uint32_t i=0;i<5;++i){check(owner.faery_level(i,d)==owner.faeries()[d][i].level);++queries;}}
 }
 check(r.at==r.b.size());PlayerSavegameV1 owner;std::size_t used=0;std::uint8_t b[4]{};check(!owner.load_current_faery({b,4},used,error));owner.initialize_faeries();check(!owner.set_faery_level(5,0,0,error));check(!owner.set_faery_state(0,0,3,error));check(!owner.load_difficulty({b,4},nullptr,nullptr,used,error));
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<comparisons<<",\"source_count_mismatch_stops\":"<<mismatches<<",\"level_queries\":"<<queries<<",\"guards\":4,\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
