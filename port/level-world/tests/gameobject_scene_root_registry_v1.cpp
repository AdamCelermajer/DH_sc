#include "gameobject_scene_root_registry_v1.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::world;
namespace {
struct CampaignRootFixture {
 std::uint32_t flags{0x60f};std::uintptr_t parent{};
 unsigned phases{},parent_references{},drops{};bool fail_drop{true};
 std::function<bool(std::string&)> after_drop;
};
void failed_campaign_clear_reuse(bool draw_phase){
 GameObjectSceneRootRegistryV1 manager;const auto same_manager=manager.identity();
 SceneUpdateTransportV102 transport;std::string error;
 auto failed=std::make_shared<CampaignRootFixture>();std::weak_ptr<CampaignRootFixture> pinned=failed;
 GameObjectSceneRootBorrowV1 root;root.owner=failed;root.identity=0x600;
 root.flags11c=&failed->flags;root.parentec=&failed->parent;
 root.notify_visibility=[](bool,auto&){return true;};root.remove_animators=[](auto&){return true;};
 root.acquire_parent_reference_v110=[pinned](auto&){++pinned.lock()->parent_references;return true;};
 root.release_parent_reference_v110=[pinned](auto& e){auto receiver=pinned.lock();++receiver->drops;
  if(receiver->fail_drop){e="actual campaign parent drop pending";return false;}
  --receiver->parent_references;return !receiver->after_drop||receiver->after_drop(e);
 };
 root.scene_phase_v69=[pinned,&manager](std::uint32_t stamp,auto& e){
  auto receiver=pinned.lock();++receiver->phases;assert(stamp==17);
  std::string clear_error;assert(!manager.retire_source_cached_aliases_v106(clear_error));
  e="actual campaign traversal failed";return false;
 };
 assert(manager.add_child(root,error)&&failed->parent_references==1);
 const auto update=[&](std::uint64_t epoch,float delta,std::uint32_t stamp){
  return draw_phase?manager.source_scene_phase_v69(epoch,stamp,error):manager.source_update_v102(delta,false,transport,error);
 };
 assert(!update(10,17.f,17)&&error=="actual campaign traversal failed"&&failed->phases==1);
 assert(!manager.retire_source_cached_aliases_v106(error)&&failed->parent==same_manager);
 error.clear();assert(!update(11,100.f,100)&&error=="actual campaign traversal failed"&&failed->phases==1);
 assert(!manager.remove_root_parent_reference_v1(root.identity,error)&&failed->parent==0&&manager.roots().empty());
 root={};failed.reset();assert(!pinned.expired()); //Pending drop AND failed traversal still pin the reached receiver.
 assert(!manager.retire_source_cached_aliases_v106(error)&&!pinned.expired());
 error.clear();assert(!update(11,100.f,100)&&error=="actual campaign traversal failed");
 auto next=std::make_shared<CampaignRootFixture>();unsigned delivered{};
 root.owner=next;root.identity=0x700;root.flags11c=&next->flags;root.parentec=&next->parent;
 root.notify_visibility=[](bool,auto&){return true;};root.remove_animators=[](auto&){return true;};
 root.scene_phase_v69=[&](std::uint32_t stamp,auto&){++delivered;assert(stamp==(draw_phase?34u:17u));return true;};
 pinned.lock()->fail_drop=false;pinned.lock()->after_drop=[&](auto& e){return manager.add_child(root,e);};
 //A completed drop can synchronously republish a root. That clear is not an
 //empty reuse boundary and must retain the current campaign's failure latch.
 assert(!manager.retire_source_cached_aliases_v106(error)&&next->parent==same_manager);
 error.clear();assert(!update(11,100.f,100)&&error=="actual campaign traversal failed"&&delivered==0);
 assert(manager.release_visual_root(root.identity,error));
 const auto counters=manager.fields();
 assert(manager.retire_source_cached_aliases_v106(error)&&error.empty()&&pinned.expired());
 assert(manager.identity()==same_manager&&manager.fields().counter440==counters.counter440&&
  manager.fields().cadence444==counters.cadence444&&manager.fields().force448==counters.force448);
 assert(manager.add_child(root,error)&&next->parent==same_manager);
 assert(update(11,0.f,34)&&error.empty()&&delivered==1); //Same registry; source time is preserved across clear.
 assert(manager.release_visual_root(root.identity,error)&&manager.retire_source_cached_aliases_v106(error));
}
void timer_sampled_scene_clock(){
 GameObjectSceneRootRegistryV1 manager;std::string error;std::uint32_t flags=0x60f;std::uintptr_t parent{};
 std::vector<std::uint32_t> samples;std::uint32_t timer=100,pose{};
 auto owner=std::make_shared<int>(1);GameObjectSceneRootBorrowV1 root;root.owner=owner;root.identity=0x800;
 root.flags11c=&flags;root.parentec=&parent;root.notify_visibility=[](bool,auto&){return true;};
 root.remove_animators=[](auto&){return true;};
 root.scene_phase_v69=[&](std::uint32_t time,auto&){samples.push_back(time);pose=time;return true;};
 assert(manager.add_child(root,error));SceneUpdateTransportV102 transport;transport.provider=owner;
 transport.timer=[&](std::uint32_t& out,auto&){out=timer;return true;};
 // This is the source CSceneManager Timer sentinel. A zero-delta menu update
 // by itself preserves time254; sampling the actual frame timer is what lets
 // the retained character scene animator advance between menu frames.
 assert(manager.source_update_v102(-123456.f,false,transport,error)&&samples.back()==100&&pose==100);
 timer=116;assert(manager.source_update_v102(-123456.f,false,transport,error)&&samples.back()==116&&pose==116);
 assert(manager.source_update_v102(0.f,false,transport,error)&&samples.back()==116&&pose==116);
 assert((samples==std::vector<std::uint32_t>{100,116,116}));
 assert(manager.release_visual_root(root.identity,error));
}
}
int main(){
 auto lease=std::make_shared<int>(1);GameObjectSceneRootRegistryV1 manager(0x400);
 assert(manager.fields().counter440==0&&manager.fields().cadence444==4&&!manager.fields().force448);
 std::uint32_t flags=0x60f;std::uintptr_t parent=0;unsigned removes=0;bool fail=false;std::string error;
 GameObjectSceneRootBorrowV1 b;b.owner=lease;b.identity=0x500;b.flags11c=&flags;b.parentec=&parent;
 // Declared actual-node endpoint fixtures: this test audits manager-owned
 // membership/order/flags, not a fabricated successful visual animator detach.
 b.notify_visibility=[&](bool visible,auto&){assert(visible);return true;};
 b.remove_animators=[&](auto& e){++removes;if(fail){e="actual animator receiver required";return false;}return true;};
 assert(manager.add_child(b,error)&&parent==manager.identity()&&(flags&0x40));
 assert(manager.fields().hierarchy_dirty288&&manager.fields().render_dirty289);
 assert(manager.roots()==std::vector<std::uintptr_t>{0x500});
 assert(manager.add_child(b,error)&&manager.roots().size()==1);
 manager.force_register();assert(manager.fields().force448==1&&manager.fields().render_dirty289==1);
 fail=true;assert(!manager.release_visual_root(b.identity,error)&&parent==manager.identity()&&manager.roots().size()==1&&removes==1);
 fail=false;assert(manager.release_visual_root(b.identity,error)&&parent==0&&manager.roots().empty()&&removes==2);
 assert(error=="actual animator receiver required");
 parent=0x900;assert(!manager.add_child(b,error)&&manager.roots().empty()&&parent==0x900);
 failed_campaign_clear_reuse(false);failed_campaign_clear_reuse(true);timer_sampled_scene_clock();
 std::cout<<"Scene root membership/source ForceRegister/failure prefix PASS; visibility/animator endpoints declared fixtures\n";
 std::cout<<"Failed campaign traversal/retained clear prefix/SAME Scene reuse PASS; recursive and draw-epoch paths\n";
}
