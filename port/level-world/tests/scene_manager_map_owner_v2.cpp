#include "../scene_manager_map_owner_v2.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::world;
// Typed receiver protocol fixture; real floor-clone projection is a separate
// required integration provider. This does not claim production map binding.
struct Node {std::uintptr_t parent{};std::uint32_t flags{0x60f};std::vector<int> calls;};
SceneMapNodeBorrowV2 borrow(std::shared_ptr<Node> n){
 SceneMapNodeBorrowV2 b;b.owner=n;b.identity=reinterpret_cast<std::uintptr_t>(n.get());b.parentec=&n->parent;b.flags11c=&n->flags;
 b.cached_position=[n](std::array<float,3>& p,std::string&){n->calls.push_back(1);p={12,23,34};return true;};
 b.set_position=[n](const std::array<float,3>& p,std::string&){assert(p[0]==12&&p[1]==23&&p[2]==34);n->calls.push_back(2);n->flags|=8;return true;};
 b.detach=[n](std::string&){n->calls.push_back(3);n->parent=0;return true;};
 b.optimize_static=[n](std::string&){n->calls.push_back(4);return true;};
 b.notify_parent_visibility=[n](bool v,std::string&){assert(!v);n->calls.push_back(5);return true;};return b;
}
int main(){
 auto manager=std::make_shared<GameObjectSceneRootRegistryV1>();auto map=std::make_shared<SceneManagerMapOwnerV2>(manager);std::string e;
 auto first=std::make_shared<Node>(),second=std::make_shared<Node>();
 assert(map->add(borrow(first),e));assert((first->calls==std::vector<int>{4,5}));assert(first->parent==map->identity());assert((first->flags&0x40)!=0);assert(!map->visible()&&!(map->flags()&1));assert(manager->roots().size()==1);
 second->parent=99;assert(map->add(borrow(second),e));assert((second->calls==std::vector<int>{1,2,3,4,5}));
 first->calls.clear();assert(map->add(borrow(first),e));assert((first->calls==std::vector<int>{1,2,4,5}));assert(map->children().back()==reinterpret_cast<std::uintptr_t>(first.get()));
 auto failure=std::make_shared<Node>();auto missing=borrow(failure);missing.notify_parent_visibility={};assert(!map->add(std::move(missing),e));assert(failure->parent==map->identity());assert(map->children().size()==3);assert(!map->release(e));
 for(auto id:map->children())assert(map->remove(id,e));assert(map->release(e));assert(manager->roots().empty());
 std::weak_ptr<SceneManagerMapOwnerV2> weak=map;map.reset();assert(weak.expired());
 std::cout<<"Scene map protocol PASS retained order/prefix/lifetime\n";
}
