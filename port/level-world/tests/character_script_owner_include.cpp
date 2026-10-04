// Dedicated new integration audit; historical owner fixtures stay unchanged.
#define main previous_owner_audit_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_script_owner.cpp"
#pragma GCC diagnostic pop
#undef main
#include "character_script_virtual.hpp"
namespace {
std::string read_file(const char* name){std::ifstream f(name,std::ios::binary);check(bool(f));return {(std::istreambuf_iterator<char>(f)),{}};}
void require(bool value,unsigned line){++checks;if(!value)throw std::runtime_error("include owner line "+std::to_string(line)+" check "+std::to_string(checks));}
}
#define check(value) require((value),__LINE__)
namespace {
struct IncludeFixture:Fixture {
 std::map<std::string,std::string> resources;
 std::vector<std::string> requested;
 struct Context {IncludeFixture* fixture;uintptr_t identity;};
 std::map<uintptr_t,Context> contexts;
 ScriptOwnerServices persistent{this,&invoke};
 dh2_script_include_scope saved{},current{};
 unsigned callbacks=0,guards=0,nested=0;
 bool recursion=false;
 IncludeFixture(const std::string& common):Fixture(common){}
 ~IncludeFixture(){owner.reset();} // Contexts survive all VM finalizers.
 static int provide(void* pointer,const dh2_script_include_scope* scope,const char* name,char* error,size_t capacity){
  auto& c=*static_cast<Context*>(pointer);auto& t=*c.fixture;++t.callbacks;
  check(scope->vm&&dh2_script_include_scope_valid(scope)==1);
  const auto requests=t.requested.size();const auto old=t.current;t.saved=*scope;t.current=*scope;
  struct Restore {IncludeFixture& t;dh2_script_include_scope old;~Restore(){t.current=old;}} restore{t,old};
  check(t.owner->include(c.identity+1,name,t.persistent,scope)==-1);
  auto wrong=*scope;wrong.generation++;check(t.owner->include(c.identity,name,t.persistent,&wrong)==-1);
  check(t.owner->include(c.identity,nullptr,t.persistent,scope)==-1);
  check(t.requested.size()==requests);t.guards+=3;
  if(old.vm){check(!dh2_script_include_scope_valid(&old));++t.nested;}
  ScriptSessionView view{};check(t.owner->find(c.identity,view));
  dh2_script_value value{};check(dh2_script_vm_get_global(scope->vm,"anything",&value)==-1);
  const auto step=t.owner->lifecycle().load_step;t.owner->lifecycle().load_step=0;
  check(t.owner->advance({0,0,nullptr,"Other"},t.persistent)==-2);
  t.owner->lifecycle().load_step=7;check(!t.owner->advance({0,0,nullptr,"Other"},t.persistent));t.owner->lifecycle().load_step=step;t.guards+=3;
  if(t.recursion&&!std::strcmp(name,"repeat"))t.resources["data/scripts/ai/repeat.luac"]=t.callbacks<3?"repeat_count=(repeat_count or 0)+1; Include('repeat')":"repeat_count=(repeat_count or 0)+1";
  const auto result=t.owner->include(c.identity,name,t.persistent,scope);
  // Outer scope is restored after a genuine nested callback.
  check(dh2_script_include_scope_valid(scope)==1);
  if(result&&capacity)std::snprintf(error,capacity,"owner Include native failure %d",result);
  if(old.vm)check(!dh2_script_include_scope_valid(&old)); // Still shadowed inside this callback.
  return result;
 }
 static int invoke(void* pointer,ScriptOwner& owner,const ScriptOwnerRequest& r,ScriptOwnerResponse& out){
  auto& t=*static_cast<IncludeFixture*>(pointer);
  if(r.service==owner_register_binding&&!std::strcmp(r.binding->name,"Include")){
   auto& context=t.contexts[r.session->identity];context={&t,r.session->identity};
   if(dh2_script_vm_bind_source_include(r.session->vm,&provide,&context))return 1;
  }
  if(r.service==owner_cached_file&&std::strcmp(r.filename,"data/scripts/ai/_commons.luac")){
   t.requested.emplace_back(r.filename);auto i=t.resources.find(r.filename);out.word=i!=t.resources.end();
   if(out.word){out.bytes=i->second.empty()?nullptr:i->second.data();out.size=i->second.size();}return 0;
  }
  if(r.service==owner_cached_file)t.requested.emplace_back(r.filename);
  return Fixture::service(&t,owner,r,out);
 }
 int start(const char* name="fixture"){return owner->advance({uint32_t(std::strlen(name)),0,name,"Other"},persistent);}
 void execute(const char* text){ScriptSessionView v{};check(owner->active(v));check(dh2_script_vm_load_source_file(v.vm,text,std::strlen(text))==0);check(dh2_script_vm_stack_size(v.vm)==5);}
 float number(const char* key){ScriptSessionView v{};check(owner->active(v));return get(v.vm,key).number;}
};
}
int main(int argc,char** argv){try{
 check(argc==3);const auto common=read_file(argv[1]);ScriptSessionView v{};
 IncludeFixture initial(common);initial.resources["data/scripts/ai/fixture.luac"]="Include('a'); initial_seen=a";
 initial.resources["data/scripts/ai/a.luac"]="a=23;AddToVFTable('OnUpdate','Absent')";
 check(!initial.start()&&initial.owner->active(v)&&v.loaded_files==3&&v.callback_flags==1&&initial.number("initial_seen")==23);
 check(dh2_script_vm_stack_size(v.vm)==5&&initial.callbacks==1);
 const auto identity=v.identity;const auto expired=initial.saved;const auto requests=initial.requested.size();const auto diagnostics=initial.owner->error();
 check(!dh2_script_include_scope_valid(&expired));check(initial.owner->include(identity,"a",initial.persistent,&expired)==-1&&initial.requested.size()==requests&&initial.owner->error()==diagnostics);
 initial.execute("Include('a');Include('missing');Include('empty');Include('');Include(17);Include({},'a')");
 // Empty requested name is source path + .luac; missing, loaded and nonstring
 // values never gain invented failure/acceptance behavior.
 check(initial.callbacks==5&&initial.requested.size()==requests+3);
 initial.resources["data/scripts/ai/empty.luac"]="";initial.resources["data/scripts/ai/.luac"]="empty_name=1";
 initial.execute("Include('empty');Include('');Include('empty')");check(initial.number("empty_name")==1&&initial.owner->active(v)&&v.loaded_files==5);
 initial.resources["data/scripts/ai/bad.luac"]="bad_prefix=1;error('genuine-source-error')";
 initial.execute("Include('bad');after_error=1");check(initial.number("bad_prefix")==1&&initial.number("after_error")==1&&initial.owner->last_source_load_status()>0&&initial.owner->last_vm_status()==-2);
 check(initial.owner->error().find("loadFile()")!=std::string::npos&&initial.owner->error().find("genuine-source-error")!=std::string::npos);
 initial.resources["data/scripts/ai/bad.luac"]="fixed=1";initial.execute("Include('bad')");check(initial.number("fixed")==1&&initial.owner->last_source_load_status()==0&&initial.owner->active(v)&&v.loaded_files==6);
 initial.resources["data/scripts/ai/parse.luac"]="broken ! lua";initial.execute("Include('parse');after_parse=1");check(initial.owner->last_source_load_status()>0&&initial.number("after_parse")==1);
 initial.resources["data/scripts/ai/unsupported.luac"]="error({})";
 const char* unsupported="Include('unsupported')";check(dh2_script_vm_load_source_file(v.vm,unsupported,std::strlen(unsupported))>0&&initial.owner->last_source_load_status()<0);
 // No cycle flag: same-file recursion executes before loaded-set insertion.
 IncludeFixture repeated(common);repeated.recursion=true;check(!repeated.start());repeated.execute("Include('repeat')");check(repeated.number("repeat_count")==3&&repeated.callbacks==3&&repeated.nested==2&&repeated.owner->active(v)&&v.loaded_files==2);
 const auto cached=repeated.requested.size();repeated.execute("Include('repeat')");check(repeated.requested.size()==cached&&repeated.number("repeat_count")==3);
 IncludeFixture chain(common);check(!chain.start());chain.resources["data/scripts/ai/x.luac"]="Include('y');x=1";chain.resources["data/scripts/ai/y.luac"]="Include('z');y=1";chain.resources["data/scripts/ai/z.luac"]="z=1";chain.execute("Include('x')");check(chain.nested==2&&chain.owner->active(v)&&v.loaded_files==4);
 check(initial.owner->active(v));IncludeFixture other(common);check(!other.start());ScriptSessionView second{};check(other.owner->active(second)&&v.vm!=second.vm&&get(second.vm,"a").type==DH2_SCRIPT_NIL);
 // Actual follower keeps its source load/alias flags with exact root loading.
 IncludeFixture follower(common);follower.resources["data/scripts/ai/follower.luac"]=read_file(argv[2]);check(!follower.start("follower")&&follower.owner->active(v)&&v.loaded_files==2&&v.callback_flags==962&&follower.owner->error().empty());
 Dl_info own{},runtime{},virtuals{};check(dladdr(reinterpret_cast<void*>(&dh2_character_script_binding),&own));check(dladdr(reinterpret_cast<void*>(&dh2_script_vm_load_source_file),&runtime));check(dladdr(reinterpret_cast<void*>(&dh2_character_script_init_vcb),&virtuals));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"include_callbacks\":"<<initial.callbacks+repeated.callbacks+chain.callbacks<<",\"atomic_and_busy_guards\":"<<initial.guards+repeated.guards+chain.guards<<",\"nested_callbacks\":4,\"same_file_preinsert_recursion\":true,\"retained_source_stack\":5,\"actual_follower_flags\":962,\"fake_game_globals\":false,\"owner_library\":\""<<own.dli_fname<<"\",\"runtime_library\":\""<<runtime.dli_fname<<"\",\"virtual_library\":\""<<virtuals.dli_fname<<"\"}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
