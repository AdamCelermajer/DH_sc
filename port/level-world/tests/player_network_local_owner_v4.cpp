#include "player_network_local_owner_v4.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh2::player;
namespace {unsigned checks{};void check(bool v){++checks;if(!v)throw std::runtime_error("source network-local check "+std::to_string(checks));}
struct Fixture {std::shared_ptr<void> lease=std::make_shared<int>(1);MatchingLocalSelectionOwnerV4 matching;PlayerNetworkLocalOwnerV4 network{matching};unsigned constructions{},queries{};
 static bool invoke(void* raw,const PlayerManagerRequestV1& q,PlayerManagerResponseV1& out,std::string& e){auto& f=*static_cast<Fixture*>(raw);
  if(q.operation==PlayerManagerOperationV1::construct_player_info){check(q.player);++f.constructions;*q.player=PlayerInfoFieldsV1{};check(!f.network.borrow(*q.player));return f.network.construct(*q.player,f.lease,e);}
  if(q.operation==PlayerManagerOperationV1::online_enabled){++f.queries;out.value=0;return true;} // Explicit offline source service fixture.
  e="Required undeclared network transport";return false;
 }};}
int main(){try{
 Fixture f;PlayerManagerOwnerV1 manager({&f,Fixture::invoke});std::string e;check(manager.initialize(e)&&f.constructions==1&&f.matching.source_mode()==1);check(f.matching.source_unallocated_mode_store(0,e));
 check(manager.add_player(0,0,0,true,e)&&f.constructions==2);
 PlayerInfoFieldsV1* info{};check(manager.get_by_internal(0,false,info,e)&&info);
 auto backing=f.network.borrow(*info);check(backing&&backing->receiver==info&&backing->parent_lease==f.lease&&backing->owner1a0==-1);
 bool local{};check(f.network.is_local(*info,local,e)&&local&&f.matching.source_mode()==1);
 std::shared_ptr<MatchingLocalIdentityOwnerV4> matching;check(f.matching.get(matching,e)&&matching->member()==-1&&matching->server_member()==-2&&!matching->is_server());
 const auto actual_identity=matching->identity();std::shared_ptr<MatchingLocalIdentityOwnerV4> again;check(f.matching.get(again,e)&&again==matching&&again->identity()==actual_identity);
 info->local66c=0;check(f.network.is_local(*info,local,e)&&local); // local66c is NOT original virtual50.
 for(unsigned joined=0;joined<2;++joined)for(auto member:{-1,0,4})for(auto server:{-2,0,4})for(auto owner:{-1,0,3,4}){
  matching->source_identity_store(joined,member,server);backing->owner1a0=owner;
  const bool is_server=joined&&member>=0&&member==server;
  check(matching->is_server()==is_server);
  check(f.network.is_local(*info,local,e)&&local==(is_server&&owner<0?true:owner==member));
 }
 PlayerInfoFieldsV1 absent;local=true;check(!f.network.is_local(absent,local,e)&&local);
 f.network.erased(*info);check(!f.network.borrow(*info));check(f.network.construct(*info,f.lease,e)&&f.network.borrow(*info)->owner1a0==-1);
 MatchingLocalSelectionOwnerV4 other;check(other.source_unallocated_mode_store(3,e));again.reset();check(!other.get(again,e)&&!again&&other.source_mode()==3);
 check(!f.matching.source_unallocated_mode_store(3,e)&&f.matching.source_mode()==1);
 std::cout<<"PASS source Matching0->Local1 identity/C1/Reset, PlayerInfo owner1a0 BEFORE manager publication, virtual50 arithmetic72cases, no local66c substitution, lifecycle absence and other-mode required boundaries; offline manager delivery fixture, no network/room transport claim checks="<<checks<<"\n";
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
