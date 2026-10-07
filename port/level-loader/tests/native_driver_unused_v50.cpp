#include "../native_driver_unused_v50.hpp"
#include <iostream>
#include <vector>
using namespace dh2::loader;
static unsigned checks;
static void ck(bool ok,const char* why){++checks;if(!ok)throw std::runtime_error(why);}
struct Backend {
 std::shared_ptr<int> owner=std::make_shared<int>(0);
 NativeDriverUnusedOwnerV50 registry;
 std::map<UnusedKeyV50,int> actual_allocations;
 std::vector<int> operations;
 std::string error;
 bool fail_release{},reenter{};
 Backend(){
  ck(registry.bind_driver(owner,1,[this](std::string&){operations.push_back(1);return true;},
      [this](std::string&){operations.push_back(5);return true;},error),"Actual driver binds");
  for(auto d:{UnusedDomainV50::batch_baker,UnusedDomainV50::material_instance,UnusedDomainV50::material_renderer,UnusedDomainV50::texture})
   ck(registry.bind_collection(d,owner,error),"Actual allocator collection binds");
 }
 UnusedKeyV50 add(UnusedDomainV50 domain,int id){
  UnusedKeyV50 key{domain,1,std::uint64_t(id)};actual_allocations.emplace(key,id);
  ck(registry.register_allocation(key,owner,[this,key](bool lost,std::string& e){
   ck(!lost,"Cleanup must not pretend context loss");operations.push_back(10+int(key.domain));
   if(reenter){std::string ignored;registry.clean_glitch(ignored);}
   if(fail_release){e="injected allocation release failure";return false;}
   ck(registry.unregister_allocation(key,e),"Actual backend release hook acknowledges current resource");
   ck(actual_allocations.erase(key)==1,"Real allocation deleted exactly once");return true;
  },error),"Actual allocation registers");return key;
 }
};
int main(){try{
 {Backend b;
  auto baker=b.add(UnusedDomainV50::batch_baker,1);auto instance=b.add(UnusedDomainV50::material_instance,1);
  auto program=b.add(UnusedDomainV50::material_renderer,1);auto unused=b.add(UnusedDomainV50::texture,1);
  auto used=b.add(UnusedDomainV50::texture,2);
  NativeDriverUnusedOwnerV50::UseLease world,ui,fx;
  ck(b.registry.acquire(used,world,b.error),"Real raw World consumer lease");ui=world;fx=ui;
  ck(b.registry.snapshot().consumer_references==3,"Copies are real consumers");
  ck(!b.registry.unregister_allocation(used,b.error)&&b.actual_allocations.count(used),"Cannot release live raw GPU name");
  b.error.clear();ck(b.registry.clean_glitch(b.error),"Native source-order cleanup");
  ck(b.operations==std::vector<int>({1,10,11,12,5,13}),"Source six-stage order");
  ck(b.actual_allocations.size()==1&&b.actual_allocations.count(used),"Live Draw/UI/FX retained; unused actual resources released");
  ck(!b.actual_allocations.count(baker)&&!b.actual_allocations.count(instance)&&!b.actual_allocations.count(program)&&!b.actual_allocations.count(unused),"All unused backend domains actually deleted");
  world.reset();ui.reset();ck(b.registry.snapshot().consumer_references==1,"Last FX keeps resource alive");
  ck(b.registry.clean_glitch(b.error)&&b.actual_allocations.size()==1,"Resource remains used after partial owners release");
  fx.reset();ck(b.registry.clean_glitch(b.error)&&b.actual_allocations.empty(),"Last source consumer release allows genuine delete");
  ck(b.registry.snapshot().releases==5,"Exactly five actual backend deletions");
 }
 {Backend b;auto first=b.add(UnusedDomainV50::texture,1);auto later=b.add(UnusedDomainV50::texture,2);
  b.fail_release=true;ck(!b.registry.clean_glitch(b.error),"Injected GL/accounting release failure stops");
  ck(b.actual_allocations.count(first)&&b.actual_allocations.count(later),"Failed release preserves remaining actual ownership");
  auto trace=b.operations;auto receipt=b.error;b.fail_release=false;
  ck(!b.registry.clean_glitch(b.error)&&b.operations==trace&&b.error==receipt,"Partial release failure never replays GL callback");
 }
 {Backend b;auto key=b.add(UnusedDomainV50::texture,7);NativeDriverUnusedOwnerV50::UseLease old;
  ck(b.registry.acquire(key,old,b.error),"Old context consumer");
  ck(b.registry.abandon_context(1,b.error),"Context loss bookkeeping detach");
  ck(b.actual_allocations.size()==1&&b.operations.empty(),"Context loss issues no GL deletion; actual backend lost-release remains owner responsibility");
  ck(b.registry.bind_driver(b.owner,2,[](std::string&){return true;},[](std::string&){return true;},b.error),"New actual context binds");
  ck(b.registry.bind_collection(UnusedDomainV50::texture,b.owner,b.error),"New actual texture allocator binds");
  auto next=UnusedKeyV50{UnusedDomainV50::texture,2,7};unsigned releases{};
  ck(b.registry.register_allocation(next,b.owner,[&](bool,std::string&){++releases;return true;},b.error),"GLuint reuse in new context registers");
  old.reset();ck(b.registry.snapshot().consumer_references==0&&releases==0,"Old lease cannot release new same GLuint");
  ck(b.registry.unregister_lost_allocation(key,b.error)&&b.registry.snapshot().entries==1,"Lost backend accounting cannot touch reused live name");
  ck(!b.registry.unregister_lost_allocation(next,b.error)&&b.registry.snapshot().entries==1,"Live resource cannot fabricate loss");
  NativeDriverUnusedOwnerV50::UseLease stale;ck(!b.registry.acquire(key,stale,b.error),"Old generation cannot borrow reused handle");
 }
 {Backend b;b.reenter=true;b.add(UnusedDomainV50::texture,1);ck(!b.registry.clean_glitch(b.error)&&b.registry.snapshot().failed,"Nested cleanup cannot be swallowed by release callback");}
 {NativeDriverUnusedOwnerV50 unbound;std::string e;ck(!unbound.clean_glitch(e),"Missing driver cannot be successful empty cleanup");}
 {Backend b;auto key=b.add(UnusedDomainV50::texture,1);NativeDriverUnusedOwnerV50::UseLease first,second;
  ck(b.registry.acquire(key,first,b.error),"Move source consumer");second=std::move(first);ck(!first&&second&&b.registry.snapshot().consumer_references==1,"Move changes ownership without extra reference");
  second.reset();ck(b.registry.clean_glitch(b.error),"Move final release cleanup");}
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"native_backend_allocations\":\"fake tiny handles with real registry/consumer/release hooks\",\"whole_original_collection_abi\":false}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
