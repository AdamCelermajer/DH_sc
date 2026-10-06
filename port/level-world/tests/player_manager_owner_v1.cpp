#include "../player_manager_owner_v1.hpp"
#include <fstream>
#include <array>
#include <vector>
#include <cstring>
#include <iostream>
#include <stdexcept>
using namespace dh2::player;
struct Fixture {
 std::array<std::int16_t,10> base{};
 static bool invoke(void* v,const PlayerManagerRequestV1& q,PlayerManagerResponseV1& r,std::string& e){
  auto& f=*static_cast<Fixture*>(v);using O=PlayerManagerOperationV1;
  if(q.operation==O::online_enabled){r.value=0;return true;}
  if(q.operation==O::construct_player_info){*q.player=PlayerInfoFieldsV1{};return true;}
  if(q.operation==O::character_initialization){if(q.id==7)return true;q.player->character660=std::uintptr_t(q.id+100);f.base[std::size_t(q.id)]=q.id==2?290:263;q.player->character_base_id13c8=&f.base[std::size_t(q.id)];++*q.character_count6c4;return true;}
  e="required fixture receiver";return false;
 }
};
int main(int argc,char** argv){try{
 if(argc!=2)throw std::runtime_error("path");std::ifstream file(argv[1],std::ios::binary);std::vector<char> data((std::istreambuf_iterator<char>(file)),{});std::size_t at{};auto word=[&](){std::uint32_t v;if(at+4>data.size())throw std::runtime_error("span");std::memcpy(&v,data.data()+at,4);at+=4;return v;};
 if(word()!=0x314f4d50)throw std::runtime_error("magic");Fixture f;PlayerManagerOwnerV1 owner({&f,Fixture::invoke});std::string e;if(!owner.initialize(e))throw std::runtime_error(e);unsigned inputs=word();std::vector<int> ids;
 for(unsigned i=0;i<inputs;++i){int id=int(word()),controller=int(word()),index=int(word());bool local=word()!=0;if(!owner.add_player(id,controller,index,local,e))throw std::runtime_error(e);ids.push_back(id);}
 for(auto id:ids)if(!owner.add_character(id,e))throw std::runtime_error(e);
 auto count=word();unsigned checks{};
 for(unsigned i=0;i<count;++i){auto kind=word();int index=int(word());bool flag=word()!=0;std::array<std::uint32_t,5> gold;for(auto& v:gold)v=word();std::array<std::uint32_t,5> actual{};
  if(kind==4){int result;if(!owner.class_count(index,result,e))throw std::runtime_error(e);actual[0]=std::uint32_t(result);}
  else{PlayerInfoFieldsV1* p;bool ok=kind==0?owner.get_by_internal(index,flag,p,e):kind==1?owner.get_player(index,flag,p,e):kind==2?owner.get_local_player(index,flag,p,e):owner.get_remote_player(index,flag,p,e);if(!ok)throw std::runtime_error(e);actual={std::uint32_t(p->internal670),std::uint32_t(bool(p->character660)),std::uint32_t(p->friendly678),std::uint32_t(p->local_remote67c),p->local66c};}
  if(actual!=gold)throw std::runtime_error("source query mismatch "+std::to_string(i));++checks;
 }
 if(at!=data.size())throw std::runtime_error("tail");std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"network_constructor_fixture\":true,\"canonical_world\":false}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
