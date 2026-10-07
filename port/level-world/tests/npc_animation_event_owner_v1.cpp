// Genuine private ScriptOwner + actual13535 commons. The registration/cache
// boundaries below are explicit owner fixtures, not a full gameplay namespace.
#define main previous_owner_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_script_owner.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../npc_animation_event_owner_v1.hpp"
#include "../character_idle_events.hpp"
namespace {
struct Events:Fixture {
 AIEventOwner48 actor{};AIEventState64 ai{};std::int32_t lag{-17};
 std::unique_ptr<NpcAnimationEventOwnerV1> event;
 explicit Events(const std::string& c):Fixture(c){}
 static int cache(void* p,ScriptOwner& owner,const ScriptOwnerRequest& q,ScriptOwnerResponse& out){
  if(q.service==owner_cached_file&&std::strcmp(q.filename,"data/scripts/ai/_commons.luac")){out.word=0;return 0;}
  return Fixture::service(p,owner,q,out);
 }
 static int nested(void* p,const dh2_script_callback_scope* scope,const dh2_script_value*,std::uint32_t,char*,std::size_t){return static_cast<Events*>(p)->event->relay("nested",scope)?0:-1;}
};
}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream input(argv[1],std::ios::binary);std::string common((std::istreambuf_iterator<char>(input)),{});check(common.size()==13535);
 Events f(common);check(!f.owner->advance({7,0,"fixture-not-present","Other"},{&f,Events::cache}));ScriptSessionView v{};check(f.owner->active(v)&&v.kind==script_external);
 f.actor.owner=f.store.owner;f.ai.owner=&f.actor;f.ai.active=v.identity;f.ai.ais_virtuals=character_idle_external_keys();
 NpcAnimationEventBorrowV1 b{};b.ai=&f.ai;b.script=f.owner.get();b.character=f.store.owner;b.animator_lag=&f.lag;
 f.event=std::make_unique<NpcAnimationEventOwnerV1>(b,dh2::data::EffectsTables::Borrow{},nullptr);
 const auto top=dh2_script_vm_stack_size(v.vm);check(f.event->relay("fixture_event"));check(!f.event->source_lua_error()&&dh2_script_vm_stack_size(v.vm)==top);
 load(v.vm,"count=0;depth=0;function Observed(event,lag) assert(lag==-17);count=count+1;seen=event;if depth==0 then depth=1;NestedAnim();depth=0 end end;AddToVFTable('OnAnimEvent','Observed')");
 check(!dh2_script_vm_bind_source_scoped(v.vm,"NestedAnim",Events::nested,&f));check(f.event->relay("outer"));check(get(v.vm,"count").number==2&&get(v.vm,"seen").text==std::string("nested"));
 check(dh2_script_vm_stack_size(v.vm)==top);
 dh2_script_callback_scope expired{v.vm,0};check(!f.event->relay("outer",&expired));
 std::array<std::uintptr_t,51> unknown{};std::copy_n(character_idle_external_keys(),51,unknown.begin());unknown[0x94/4]=0x123456;f.ai.ais_virtuals=unknown.data();check(!f.event->relay("outer"));f.ai.ais_virtuals=character_idle_external_keys();
 f.ai.active=0;check(f.event->relay("outer"));f.ai.active=v.identity;
 // Footstep Lua prefix completes, then genuine absent World/FX fails. No fake
 // success or fabricated foot position is supplied to the positive FX path.
 check(!f.event->relay("step_left")&&f.event->error().find("actual NPC foot")!=std::string::npos);
 std::cout<<"PASS actual commons NPC OnAnimEvent; original selected virtual94; fresh alias, event+signed lag; scoped recursive privateVM delivery; stale/unknown rejection; foot reached-prefix failure explicit. Registration/cache fixtures declared.\n";
 return 0;
}catch(const std::exception& x){std::cerr<<x.what()<<'\n';return 1;}}
