#include "gameobject_scene_root_registry_v1.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::world;
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
 std::cout<<"Scene root membership/source ForceRegister/failure prefix PASS; visibility/animator endpoints declared fixtures\n";
}
