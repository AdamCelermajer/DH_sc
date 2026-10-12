// Borrow the frozen corpus reader/service fixture; its historical main is not run.
#define main historical_lifecycle_audit_main
#include "character_script_lifecycle.cpp"
#undef main
#include "../character_deferred_script.hpp"
namespace {
int delivered(void* p,ScriptLifecycleState64* s,const ScriptLifecycleRequest32* r,ScriptLifecycleResponse16* response){invoke(p,s,r,response);return 0;}
struct Failure {std::vector<unsigned> calls;unsigned at;};
int fail(void* p,ScriptLifecycleState64* s,const ScriptLifecycleRequest32* r,ScriptLifecycleResponse16*){auto& f=*static_cast<Failure*>(p);f.calls.push_back(r->service);if(r->service==script_refresh_vitals)s->owner=identities[6];return r->service==f.at?-9:0;}
}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;Reader reader(argv[1]);check(reader.get<unsigned>()==0x314c5341,"ASL1");const auto count=reader.get<unsigned>();unsigned requests=0;
 for(unsigned i=0;i<count;++i){auto op=reader.get<unsigned>(),arg=reader.get<unsigned>();auto initial=reader.get<ScriptLifecycleState64>();Context c{reader.get<std::array<std::uint32_t,8>>(),{}};auto result=reader.get<unsigned>();auto expected=reader.get<ScriptLifecycleState64>();auto n=reader.get<unsigned>();std::vector<Call> calls(n);for(auto& row:calls)row=reader.get<Call>();auto s=encode(initial);DeferredScriptServices16 services{&c,delivered};check(unsigned(dh2_character_deferred_script(&s,op,arg,&services))==result,"Source return");auto after=decode(s);check(!std::memcmp(&after,&expected,64)&&c.calls==calls,"Source projection/order");requests+=n;}
 check(reader.at==reader.bytes.size(),"Trailing gold");unsigned guards=0,failures=0;Context context{};DeferredScriptServices16 services{&context,delivered};ScriptLifecycleState64 base{};base.owner=identities[1];
 auto reject=[&](ScriptLifecycleState64* s,unsigned op,unsigned arg,const DeferredScriptServices16* c){auto before=s?*s:base;auto n=context.calls.size();check(dh2_character_deferred_script(s,op,arg,c)==-1,"Malformed accepted");check(!s||!std::memcmp(&before,s,64),"Malformed effect");check(context.calls.size()==n,"Malformed callback");++guards;};
 reject(nullptr,4,1,&services);reject(&base,4,1,nullptr);auto absent=services;absent.invoke=nullptr;reject(&base,4,1,&absent);reject(&base,4,1,reinterpret_cast<const DeferredScriptServices16*>(&base));reject(&base,0,1,&services);reject(&base,4,2,&services);
 for(unsigned j=0;j<7;++j){auto s=base;if(j==0)s.owner=0;if(j==1)s.load_step=-1;if(j==2)s.delayed=256;if(j==3)s.scripted=256;if(j==4)s.reserved0=1;if(j==5)s.reserved1=1;if(j==6)s.reserved2=1;reject(&s,4,1,&services);}
 for(unsigned failed:{script_refresh_vitals,script_configure_skills,script_update_skills,script_ai_init_post,script_ai_init_final}){Failure f{{},failed};DeferredScriptServices16 c{&f,fail};auto s=base;check(dh2_character_deferred_script(&s,script_init_process,1,&c)==-2,"Failed service accepted");check(s.owner==identities[6]&&f.calls.back()==failed,"Failure prefix lost");check(f.calls.size()==failed-script_refresh_vitals+1,"Failure continuation");++failures;}
 // Active guard never enters loading/Init even with a failing provider.
 Failure f{{},script_refresh_vitals};DeferredScriptServices16 bad{&f,fail};base.active=identities[2];check(dh2_character_deferred_script(&base,4,1,&bad)==0&&f.calls.empty(),"Active guard");
 std::cout<<"{\"validation\":\"PASS\",\"gold_cases\":"<<count<<",\"ordered_services\":"<<requests<<",\"atomic_guards\":"<<guards<<",\"required_failure_prefixes\":"<<failures<<",\"mismatches\":0}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
