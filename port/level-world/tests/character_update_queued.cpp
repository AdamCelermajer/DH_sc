// Reuse the frozen prefix's explicit provider projections, never its runner.
#define main historical_startup_audit_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_update_startup.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../character_update_queued.hpp"
using Rows=std::vector<std::array<std::uint32_t,2>>;
using Order=std::array<std::uint32_t,2>;
struct QueueEvent {std::array<std::uint32_t,5> fields;Rows rows;bool operator==(const QueueEvent& o)const{return fields==o.fields&&rows==o.rows;}};
std::int32_t signed_word(std::uint32_t value){std::int32_t out;std::memcpy(&out,&value,4);return out;}
struct QueuedFixture:Fixture {
 CharacterDeferredQueue* retained=nullptr;std::array<DeferredQueueOwner16,33> actors{};
 DeferredQueueServices24 queue_services{this,queue_invoke,3,0};UpdateQueuedBinding24 binding{};
 std::vector<QueueEvent> queue_events;std::vector<Order> order;
 std::uint32_t clock=0,action=0;bool mutated=false;int queue_fail=-1;
 QueuedFixture(){for(std::uint32_t i=0;i<32;++i)actors[i]={i+1,0x6000+i+1};actors[32]={0x1234,0x4400};}
 ~QueuedFixture(){if(retained)require(dh2_character_deferred_queue_destroy(retained)==0);}
 const DeferredQueueOwner16* actor(std::uint32_t id){require((id>=1&&id<=32)||id==0x1234);return &actors[id==0x1234?32:id-1];}
 Rows rows(){std::array<DeferredQueueRow16,1024> raw{};std::uint32_t count=0;require(!dh2_character_deferred_queue_snapshot(retained,raw.data(),raw.size(),&count));Rows out;for(std::uint32_t i=0;i<count;++i)out.push_back({static_cast<std::uint32_t>(raw[i].key),static_cast<std::uint32_t>(raw[i].owner)});return out;}
 void sync(){queue.count=rows().size();queue.first=queue.count?1:0;}
 void assign(std::uint32_t key,std::uint32_t id){require(!dh2_character_deferred_queue_assign(retained,signed_word(key),actor(id),nullptr));}
 static int can(void* context,CanUpdateOwner40* input,const CanUpdateRequest24* request,CanUpdateResponse16* response){auto& f=*static_cast<QueuedFixture*>(context);f.order.push_back({1,static_cast<std::uint32_t>(f.can_events.size())});return Fixture::can(static_cast<Fixture*>(&f),input,request,response);}
 static int invoke(void* context,UpdateStartupOwner64* input,const UpdateStartupRequest40* request,UpdateStartupResponse16* response){
  auto& f=*static_cast<QueuedFixture*>(context);f.sync();f.order.push_back({0,static_cast<std::uint32_t>(f.events.size())});
  const int result=Fixture::invoke(static_cast<Fixture*>(&f),input,request,response);if(request->operation==update_real_time)response->word=f.clock;return result;
 }
 static int queue_invoke(void* context,CharacterDeferredQueue* q,const DeferredQueueRequest32* request){
  auto& f=*static_cast<QueuedFixture*>(context);require(q==f.retained&&!request->reserved);f.order.push_back({2,static_cast<std::uint32_t>(f.queue_events.size())});
  f.queue_events.push_back({{request->operation,static_cast<std::uint32_t>(request->owner),static_cast<std::uint32_t>(request->controller),request->final,static_cast<std::uint32_t>(request->key)},f.rows()});
  if(!f.mutated&&((request->operation==0&&f.action>=1&&f.action<=3)||(request->operation==1&&f.action==4))){
   f.mutated=true;
   if(f.action==1)f.assign(static_cast<std::uint32_t>(request->key),32);
   else if(f.action==2)f.assign(0x80000000,31);
   else if(f.action==3){std::uint32_t id=0;for(const auto& row:f.queue_events.back().rows)if(row[1]!=request->owner){id=row[1];break;}require(id);DeferredQueueToken24 token{q,0,0,0};require(!dh2_character_deferred_queue_unload(q,f.actor(id),&token,255,&f.queue_services));}
   else require(!dh2_character_deferred_queue_evict(q,0,&f.queue_services));
  }
  return static_cast<int>(request->operation)==f.queue_fail?1:0;
 }
 void reset(const std::array<std::uint32_t,26>& p,std::uint32_t time,std::uint32_t reentry){
  if(retained)require(!dh2_character_deferred_queue_destroy(retained));
  retained=dh2_character_deferred_queue_create();require(retained);
  Fixture::reset(p,0);services={this,invoke,(1u<<19)-1,0};eligibility_services={this,can,63,0};queue_services={this,queue_invoke,3,0};binding={retained,&actors[32],&queue_services};
  clock=time;action=reentry;mutated=false;queue_fail=-1;queue_events.clear();order.clear();
  for(std::uint32_t i=0;i<p[13];++i)assign(i,p[14]&&i==0?0x1234:i%32+1);
  sync();
 }
 int run(std::uint32_t& stage){int result=dh2_character_update_queued(&owner,&services,&binding,&stage);sync();return result;}
};
Rows read_rows(std::istream& input,std::uint32_t count){Rows out(count);for(auto& row:out)for(auto& value:row)value=word(input);return out;}
int main(int argc,char** argv){try{
 require(argc==2);std::ifstream input(argv[1],std::ios::binary);char magic[4];input.read(magic,4);require(!std::memcmp(magic,"CUQ1",4));const auto count=word(input);
 QueuedFixture f;std::uint32_t prefix_count=0,can_count=0,queue_count=0,ordered_entries=0,reentries=0,guards=0;
 for(std::uint32_t i=0;i<count;++i){
  std::array<std::uint32_t,26> p{};for(auto& v:p)v=word(input);const auto time=word(input),action=word(input),status=word(input),stage=word(input);
  Snapshot snapshot{};for(auto& v:snapshot)v=word(input);const auto np=word(input),nc=word(input),nq=word(input),no=word(input),nr=word(input);
  std::vector<Event> events(np);for(auto& e:events)for(auto& v:e)v=word(input);std::vector<CanEvent> can_events(nc);for(auto& e:can_events)for(auto& v:e)v=word(input);
  std::vector<QueueEvent> queue_events;for(std::uint32_t j=0;j<nq;++j){QueueEvent event;for(auto& v:event.fields)v=word(input);const auto n=word(input);event.rows=read_rows(input,n);ordered_entries+=n;queue_events.push_back(event);}
  std::vector<Order> order(no);for(auto& e:order)for(auto& v:e)v=word(input);const auto rows=read_rows(input,nr);
  f.reset(p,time,action);std::uint32_t output=99;require(f.run(output)==static_cast<int>(status));require(output==stage&&f.snapshot()==snapshot&&f.events==events&&f.can_events==can_events&&f.queue_events==queue_events&&f.order==order&&f.rows()==rows);
  prefix_count+=np;can_count+=nc;queue_count+=nq;ordered_entries+=nr;reentries+=f.mutated;
 }
 require(input.peek()==std::char_traits<char>::eof());std::array<std::uint32_t,26> p{};p[6]=1;p[7]=p[8]=p[9]=3;p[13]=9;p[15]=p[16]=p[20]=1;
 auto fail=[&](int expected){std::uint32_t stage=99;require(f.run(stage)==expected&&stage==99&&f.stats==0xfffffffe);++guards;};
 f.reset(p,0x80000000,0);f.queue_services.available=2;fail(2);require(f.rows().size()==9&&f.events.back()[0]==update_get_current_level);++guards;
 f.reset(p,0x80000000,0);f.queue_fail=0;fail(3);require(f.rows().size()==9);++guards;
 f.reset(p,0x80000000,0);f.queue_fail=1;fail(3);require(f.rows().size()==8&&f.events.back()[0]==update_get_current_level);++guards;
 for(int op:{update_load_and_init,update_is_monster,update_online,update_real_time}){f.reset(p,0x80000000,0);f.fail=op;fail(3);require(f.rows().size()==8);++guards;}
 f.reset(p,0x80000000,0);f.binding.actor=&f.actors[0];fail(1);require(f.events.empty());++guards;
 f.reset(p,0x80000000,0);f.binding.queue=nullptr;fail(1);require(f.events.empty());++guards;
 p[13]=0;f.reset(p,0x80000000,0);f.binding.queue_services=nullptr;f.owner.queue=reinterpret_cast<UpdateStartupQueue16*>(1);std::uint32_t stage=99;require(!f.run(stage)&&stage==update_controller_ready&&f.rows()==Rows({{0x80000000,0x1234}})&&f.stats==0xffffffff);++guards;
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<count<<",\"ordered_prefix_services\":"<<prefix_count<<",\"ordered_eligibility_services\":"<<can_count<<",\"ordered_queue_callbacks\":"<<queue_count<<",\"ordered_entries\":"<<ordered_entries<<",\"reentry_records\":"<<reentries<<",\"failure_and_contract_checks\":"<<guards<<",\"mismatches\":0}\n";
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
