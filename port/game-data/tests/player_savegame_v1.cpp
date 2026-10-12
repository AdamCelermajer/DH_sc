#include "../player_savegame_v1.hpp"
#include "../item_inventory_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
using namespace dh2::data;
using B=std::vector<std::uint8_t>;
void require(bool b){if(!b)throw std::runtime_error("player owner audit mismatch");}
B file(const std::string& p){std::ifstream f(p,std::ios::binary);require(bool(f));return B(std::istreambuf_iterator<char>(f),{});}
void put(B& b,std::uint32_t x){for(int i=0;i<4;++i)b.push_back(std::uint8_t(x>>(8*i)));}
struct R{const B& b;std::size_t p{};std::uint32_t w(){require(p+4<=b.size());std::uint32_t n=0;for(int i=0;i<4;++i)n|=std::uint32_t(b[p++])<<(8*i);return n;}B bytes(std::size_t n){require(n<=b.size()-p);B a(b.begin()+p,b.begin()+p+n);p+=n;return a;}};
B snapshot(PlayerSavegameV1& a,std::uint32_t calls){B b;put(b,a.skills().size());for(auto& s:a.skills()){put(b,std::uint32_t(s.id));b.push_back(std::uint8_t(s.level));b.push_back(std::uint8_t(s.level>>8));b.push_back(s.flag);b.push_back(0);}for(auto& map:a.skill_slots()){put(b,map.size());for(auto& pair:map){put(b,std::uint32_t(pair.first));put(b,pair.second);}}put(b,calls);return b;}
int main(int argc,char** argv){try{
 require(argc==3);B gold=file(argv[1]);R r{gold};require(r.bytes(4)==B({'P','G','S','1'}));std::string root=argv[2],error;SkillTables tables;
 B data=file(root+"/skills_pyarray.bin"),names=file(root+"/skills_pyarraynames.bin"),schema=file(root+"/skills_pystructnames.bin");require(tables.load({data.data(),data.size()},{names.data(),names.size()},{schema.data(),schema.size()},error));auto borrowed=tables.borrow();
 auto cases=r.w();std::uint32_t comparisons=0,queries=0;for(std::uint32_t k=0;k<cases;++k){std::vector<std::int32_t> ids;auto n=r.w();while(n--)ids.push_back(static_cast<std::int32_t>(r.w()));PlayerSavegameV1 owner;require(owner.slot()==-1&&owner.level()==0&&owner.class_id()==-1&&owner.name().empty()&&!owner.has_skill_slots());owner.set_character(UINT64_C(0x1234567800000099));std::uint32_t calls=0;SavedSkillUpdateServicesV1 services{&calls,[](void* p,std::uintptr_t character,std::string&){require(character==UINT64_C(0x1234567800000099));++*static_cast<std::uint32_t*>(p);return true;}};
 auto ops=r.w();while(ops--){auto op=r.bytes(r.w());R q{op};switch(q.w()){
  case 0:if(!owner.skills_initialized())++calls;require(owner.initialize_skills(ids,error));break;
  case 1:{auto i=q.w();auto v=q.w();require(owner.set_skill_level(i,static_cast<std::int32_t>(v),error));break;}
  case 2:{auto key=q.w();auto i=q.w();require(owner.set_skill_in_slot(static_cast<std::int32_t>(key),i,services,error));break;}
  case 3:{auto blob=q.bytes(q.w());std::size_t used=0;require(owner.load_skills({blob.data(),blob.size()},borrowed,used,error)==0&&used==blob.size());break;}
  default:require(false);
 }auto expected=r.bytes(r.w());auto actual=snapshot(owner,calls);if(actual!=expected){std::cerr<<"snapshot case "<<k<<" comparison "<<comparisons<<" error "<<error<<" sizes "<<actual.size()<<"/"<<expected.size()<<'\n';for(std::size_t z=0;z<actual.size()&&z<expected.size();++z)if(actual[z]!=expected[z]){std::cerr<<"first byte "<<z<<" actual "<<int(actual[z])<<" expected "<<int(expected[z])<<'\n';break;}}require(actual==expected);++comparisons;
 for(std::uint32_t i=0;i<ids.size();++i){require(owner.skill_id(i)==ids[i]&&owner.skill_level(i)==owner.skills()[i].level);auto slot=owner.skill_slot(i);if(slot!=-1)require(owner.skill_in_slot(slot)==static_cast<std::int32_t>(i));queries+=3;}
 }
 }
 require(r.p==gold.size());std::uint32_t guards=0;PlayerSavegameV1 a;std::vector<std::int32_t> ids{1,2};require(!a.initialize_skills(ids,error));++guards;a.set_character(0x100000001);require(a.initialize_skills(ids,error));auto before=snapshot(a,0);SavedSkillUpdateServicesV1 absent;
 require(!a.set_skill_in_slot(0,0,absent,error)&&snapshot(a,0)==before);++guards;require(!a.set_skill_level(2,0,error)&&snapshot(a,0)==before);++guards;require(!a.set_skill_in_slot(-1,0,absent,error)&&snapshot(a,0)==before);++guards;
 std::size_t used=0;B truncated;put(truncated,0);put(truncated,1);put(truncated,7);require(a.load_skills({truncated.data(),truncated.size()},borrowed,used,error)==-2&&a.skill_in_slot(7)==0&&used==12);++guards;
 B name;put(name,8);name.insert(name.end(),{'P','r','i','n','c','e',0,0});require(a.load_name({name.data(),name.size()},used,error)&&a.name()==std::string("Prince\0",7));++guards;
 B level;put(level,0xffffffff);require(a.load_level({level.data(),level.size()},used,error)&&a.level()==-1);++guards;
 std::uint32_t nested=0;SavedSkillUpdateServicesV1 service{&nested,[](void* p,std::uintptr_t,std::string&){++*static_cast<std::uint32_t*>(p);return false;}};require(!a.set_skill_in_slot(3,1,service,error)&&a.skill_in_slot(3)==1&&nested==1);++guards;
 std::cout<<"{\"validation\":\"PASS\",\"cases\":"<<cases<<",\"comparisons\":"<<comparisons<<",\"query_checks\":"<<queries<<",\"guards\":"<<guards<<",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
