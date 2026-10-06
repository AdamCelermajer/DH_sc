#include "../status_message_owner_v23.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh2;
static unsigned checks{};static void check(bool value){++checks;if(!value)throw std::runtime_error("check "+std::to_string(checks));}
struct Fixture {
 unsigned local_queries{},calls{};std::uintptr_t movie{};bool fail{},reenter{};
 ui::StatusMessageOwnerV23* queue{};
 static bool player(void* raw,const player::PlayerManagerRequestV1& q,player::PlayerManagerResponseV1& r,std::string& e){
  auto& f=*static_cast<Fixture*>(raw);
  if(q.operation==player::PlayerManagerOperationV1::construct_player_info){*q.player={};return true;} // network subobject fixture, no AddCharacter claim
  if(q.operation==player::PlayerManagerOperationV1::online_enabled){++f.local_queries;r.value=0;return true;}
  e="Required actual player source method";return false;
 }
 static bool current(void* raw,std::uintptr_t& out,std::string&){out=static_cast<Fixture*>(raw)->movie;return true;}
 static bool invoke(void* raw,std::uintptr_t movie,const char* node,const char* method,double argument,std::string& e){
  auto& f=*static_cast<Fixture*>(raw);++f.calls;check(movie==f.movie&&std::string(node)=="_root"&&std::string(method)=="onStatusMessage"&&argument==0);
  check(f.queue->pending()>0); // callback observes source-published front
  if(f.reenter){std::string text;check(f.queue->peek(0,text,e));check(text=="first");f.reenter=false;check(f.queue->enqueue("nested",9,e));}
  if(f.fail){e="Actual scoped Flash required callback failed";return false;}return true;
 }
};
int main(){try{
 Fixture f;player::PlayerManagerOwnerV1 p({&f,Fixture::player});std::string e;check(p.initialize(e));
 auto pin=std::make_shared<int>(1);ui::StatusMessageOwnerV23 q(p,{pin,&f,Fixture::current,Fixture::invoke});f.queue=&q;
 check(q.enqueue("offline",17,e));check(q.pending()==1&&q.front()->metadata18==17&&f.calls==0); // source NULL movie retains queue
 std::string text;check(q.peek(0,text,e)&&text=="offline");check(q.pending()==1);check(!q.peek(1,text,e));
 check(q.stop_status(e)&&q.pending()==0);check(q.stop_status(e));
 f.movie=0x100;f.reenter=true;check(q.enqueue("first",3,e));check(q.pending()==2&&f.calls==1);
 check(q.stop_status(e)&&q.pending()==1&&f.calls==2);check(q.peek(0,text,e)&&text=="nested");
 check(q.stop_status(e)&&q.pending()==0);f.fail=true;
 check(!q.enqueue("failed callback",5,e));check(q.pending()==1&&q.front()->text=="failed callback"); // preserve mutation prefix, no rollback
 check(q.enqueue("later",6,e)&&q.pending()==2);check(!q.stop_status(e));check(q.pending()==1&&q.front()->text=="later");
 q.flush();check(q.pending()==0);check(!q.enqueue(nullptr,0,e));
 std::cout<<"PASS source StatusMsg queue/query/pop/null movie/reentry/failed callback prefix checks="<<checks<<"; Flash transport and PlayerInfo constructor fixture; no live HUD receipt\n";
 return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
