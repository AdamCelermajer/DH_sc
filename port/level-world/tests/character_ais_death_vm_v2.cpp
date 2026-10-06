#define main prior_owner_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_script_owner.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../character_ais_death_vm_v2.hpp"
namespace {
struct DeathFixture:Fixture {
 std::string script;std::uintptr_t selected{};unsigned nested_count{};
 explicit DeathFixture(const std::string& common):Fixture(common){}
 static int provider(void* p,ScriptOwner& o,const ScriptOwnerRequest& r,ScriptOwnerResponse& out){auto& f=*static_cast<DeathFixture*>(p);if(r.service==owner_cached_file&&std::strcmp(r.filename,"data/scripts/ai/_commons.luac")){out.word=1;out.bytes=f.script.data();out.size=f.script.size();return 0;}return Fixture::service(&f,o,r,out);}
 static int type(void*,uintptr_t,const char** n){*n="Character";return 0;}
 static int methods(void*,uintptr_t,const dh2_script_object_method** m,uint32_t* count){*m=nullptr;*count=0;return 0;}
 static int method(void*,const dh2_script_callback_scope*,uintptr_t,uint32_t,const dh2_script_value*,uint32_t,dh2_script_value*,uint32_t,uint32_t*,char*,size_t){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
 static int nested(void* p,const dh2_script_callback_scope* scope,const dh2_script_value*,uint32_t,char*,size_t){auto& f=*static_cast<DeathFixture*>(p);++f.nested_count;std::string e;return character_ais_death_vm_v2(*f.owner,f.selected,0x3dd440,202,scope,e);}
};
}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream input(argv[1],std::ios::binary);std::string common((std::istreambuf_iterator<char>(input)),{});check(common.size()==13535);
 DeathFixture f(common);check(!f.owner->advance({7,0,"fixture","Other"},{&f,DeathFixture::provider}));ScriptSessionView v{};check(f.owner->active(v)&&v.kind==script_external);f.selected=v.identity;
 uint32_t source{};check(f.owner->source_death_callback(source)&&source==0x3dd440);
 dh2_script_object_services objects{&f,DeathFixture::type,DeathFixture::methods,DeathFixture::method};check(!dh2_script_vm_set_source_objects(v.vm,&objects));
 const auto top=dh2_script_vm_stack_size(v.vm);std::string e;
 check(!character_ais_death_vm_v2(*f.owner,v.identity,source,202,nullptr,e)); // Real commons empty endpoint.
 load(v.vm,"calls=0;depth=0;function Death(killer) assert(killer._this~=nil);calls=calls+1;if depth==0 then depth=1;Nested();depth=0 end end;AddToVFTable('OnDied','Death')");
 check(!dh2_script_vm_bind_source_scoped(v.vm,"Nested",DeathFixture::nested,&f));
 check(!character_ais_death_vm_v2(*f.owner,v.identity,source,202,nullptr,e));check(get(v.vm,"calls").number==2&&f.nested_count==1&&dh2_script_vm_stack_size(v.vm)==top);
 load(v.vm,"function Death(killer) calls=calls+100;error('death-mutation-prefix')end");
 check(!character_ais_death_vm_v2(*f.owner,v.identity,source,202,nullptr,e));check(get(v.vm,"calls").number==102&&e.find("death-mutation-prefix")!=std::string::npos);
 dh2_script_callback_scope expired{v.vm,0};check(character_ais_death_vm_v2(*f.owner,v.identity,source,202,&expired,e)==-1);
 check(character_ais_death_vm_v2(*f.owner,v.identity,0x1234,202,nullptr,e)==DH2_SCRIPT_REQUIRED_FAILURE_STATUS);
 check(character_ais_death_vm_v2(*f.owner,UINT64_MAX,source,202,nullptr,e)==-1);
 f.owner->lifecycle().active=0;check(!f.owner->source_death_callback(source));
 // Captured receiver survives source active reload; original callback receiver
 // does not change midway through the void virtual dispatch.
 check(!character_ais_death_vm_v2(*f.owner,v.identity,0x3dd440,202,nullptr,e));check(get(v.vm,"calls").number==202);
 Fixture inherited(common);check(!inherited.owner->advance({0,0,nullptr,"Other"},{&inherited,Fixture::service}));ScriptSessionView d{};check(inherited.owner->active(d));check(inherited.owner->source_death_callback(source)&&source==0x3dbe90);check(!character_ais_death_vm_v2(*inherited.owner,d.identity,source,202,nullptr,e));
 std::cout<<"PASS retained actual selected AIS+24; actual commons/private VM/source-object attacker; fresh alias/global, protected source error prefix, nested scope, expired scope and unknown source rejection. External cache/register/type-method fixture boundaries explicit.\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
