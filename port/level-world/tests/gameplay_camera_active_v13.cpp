#include "../gameplay_camera_active_v13.hpp"
#include <cassert>
#include <iostream>
#include <vector>
using namespace dh2;
int main(){std::string e;world::GameObjectSceneRootRegistryV1 manager;camera::GameplayCameraActiveV13 active;
 auto a=std::make_shared<int>(1),b=std::make_shared<int>(2),c=std::make_shared<int>(3);std::vector<int> calls;unsigned refs[4]{0,1,1,1};bool reject_drop=false,reenter=false;
 auto node=[&](unsigned i,std::shared_ptr<void> owner){world::GameObjectSceneCameraBorrowV13 n;n.identity=i;n.owner=owner;n.grab=[&,i](auto&){++refs[i];calls.push_back(10+i);return true;};n.drop=[&,i](auto& error){calls.push_back(20+i);if(reject_drop){error="declared drop failure fixture";return false;}--refs[i];if(reenter&&i==1){reenter=false;assert(manager.set_active_camera_v13({},error));}return true;};return n;};
 auto an=node(1,a),bn=node(2,b),cn=node(3,c);
 camera::CameraBaseBorrowV13 aa{a,101,[&](auto&){calls.push_back(31);return true;},[&](auto&){calls.push_back(41);return true;},an};
 camera::CameraBaseBorrowV13 bb{b,102,[&](auto&){calls.push_back(32);return true;},[&](auto&){calls.push_back(42);return true;},bn};
 camera::CameraBaseBorrowV13 cc{c,103,[&](auto&){calls.push_back(33);return true;},[&](auto&){calls.push_back(43);return true;},cn};
 assert(active.register_receiver(aa,e)&&active.register_receiver(bb,e)&&active.register_receiver(cc,e));assert(active.set_active(101,manager,e));assert(calls==std::vector<int>({11,31}));assert(manager.active_camera_v13().identity==1&&manager.fields().render_dirty289==1);
 calls.clear();assert(active.set_active(101,manager,e)&&calls.empty());assert(active.set_active(102,manager,e));assert(calls==std::vector<int>({41,12,21,32}));assert(refs[1]==1&&refs[2]==2);
 // Original source drop can reenter active selection. Outer manager store
 // wins, while CameraBase rereads its process pointer before Activated.
 bb.deactivated=[&](auto&){calls.push_back(42);return true;};active.destroy_receiver(102);assert(active.register_receiver(bb,e));
 // Install a source-node drop fixture which reenters only once.
 bool once=true;bn.drop=[&](auto& error){calls.push_back(22);--refs[2];if(once){once=false;assert(active.set_active(103,manager,error));}return true;};
 assert(manager.set_active_camera_v13({},e)&&manager.set_active_camera_v13(bn,e));calls.clear();assert(active.set_active(101,manager,e));assert(active.source_active()==103&&manager.active_camera_v13().identity==1&&calls.back()==33);
 active.destroy_receiver(101);assert(active.source_active()==103);active.destroy_receiver(103);assert(active.source_active()==0);
 calls.clear();reject_drop=true;assert(!manager.set_active_camera_v13(cn,e));assert(manager.active_camera_v13().identity==1&&calls==std::vector<int>({13,21}));reject_drop=false;assert(manager.set_active_camera_v13({},e)&&manager.active_camera_v13().identity==0);
 std::cout<<"Camera same SceneManager active slot/source ordering/reentry/failure-prefix PASS; callback mutation receivers declared fixtures\n";
}
