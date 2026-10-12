#pragma once
#include "floors.hpp"
#include <functional>
#include <memory>
#include <string>
namespace dh2::world {
class ModuleFloorCloneV3;class SceneManagerMapOwnerV2;class ModulePFRoomsV3;
enum class PFWorldFlushPhaseV1 {idle,quiesce,actual_fields,rooms,floor_aux68,clone_drop40,floor_tail,room_storage,bounds_zero,object_map2c,exits84,outer44,inner48,initialized_zero,complete,failed};
struct SceneMapNodeBorrowV2;
struct FloorCloneDestructionV1 {
 std::shared_ptr<void> owner; // independent service; Main captures actual scopes weakly
 // Actual cached render/model/scene aliases already unpublished. No fabricated
 // count, guessed empty renderer, or passive C++ expiry is a native D0 receipt.
 std::function<bool(ModuleFloorCloneV3&,std::string&)> require_unpublished;
 // Genuine copied IMeshSceneNode derived/base D1, while SAME BRES and Record
 // are pinned. Does not delete this borrowed port allocation itself.
 std::function<bool(ModuleFloorCloneV3&,std::string&)> native_mesh_d1;
};
struct SceneMapDestructionV1 {
 std::shared_ptr<void> owner;
 // Actual SceneManager/draw/animator delivery quiescence before native parent
 // references drop. This must purge actual collected render aliases too.
 std::function<bool(SceneManagerMapOwnerV2&,std::string&)> quiesce;
 // Scene parent reference drop, not child D0: each PFFloor still owns mesh40.
 // SAME parentec/list clear happens FIRST (removeAll598818..59882c), then
 // this leaf drops the actual native parent/transport reference. The local
 // diagnostic C++ pin is not a synthetic native grab or a child constructor.
 std::function<bool(const SceneMapNodeBorrowV2&,std::string&)> child_parent_drop;
 // Actual map CSceneNode derived D1 prefix after root parent drop, then
 // base ISNode D1 removeAll happens in this SAME owner, followed by actual
 //114 scene-manager drop/list/name/IObject tail. Neither leaf deletes the
 // borrowed port allocation itself. No passive expiry is counted as D0.
 std::function<bool(SceneManagerMapOwnerV2&,std::string&)> native_map_d1_prefix,native_map_d1_tail;
};
struct PFGraphDestructionV1 {
 std::shared_ptr<void> receiver;std::uintptr_t identity{};
 std::function<bool(std::string&)> native_d0;
};
// Pointers/closures must project the SAME actual PFWorld fields, not a second
// World or source-looking copies initialized from sewn/vector sizes. The
// existing floor adapter does not yet expose native2c/44/48/4 producers.
struct PFWorldFlushFieldsV1 {
 std::shared_ptr<void> owner;
 std::uintptr_t identity{}; // actual PFWorld receiver, not floor-array address
 std::shared_ptr<floors::World> storage_owner; // SAME navigation storage lease
 std::uint32_t *initialized4{};
 std::uintptr_t *outer44{},*inner48{};
 // Whole actual floor->PFObject deque tree erase/reset. This is a required
 // real owner body, not guessed absence of PF objects. Existing ModulePFRooms
 // owns actual vector<pair<ExitDirection,Vec3f>>84; loader clears it directly.
 std::function<bool(std::string&)> erase_floor_objects2c;
 std::function<bool(std::uintptr_t,PFGraphDestructionV1&,std::string&)> graph;
};
struct PFWorldFlushServicesV1 {
 std::shared_ptr<void> owner; // independent; no containing World/Level capture
 // Prove source Scene virtual68/map-child/drop and draw/query/event quiescence
 // already completed. PF Flush never invents a SceneManager or clears it late.
 std::function<bool(ModulePFRoomsV3&,std::string&)> require_quiescent;
 std::function<bool(floors::World&,std::uintptr_t,PFWorldFlushFieldsV1&,std::string&)> actual_fields;
 // Exact PFFloorD1 boundary: free actual68 before mesh40.drop, then release
 // c0,b4,a8 storage /90,78 trees /28,4 strings before floor allocation free.
 // Main must lend genuine positive owners. Loader owns ordered slot/free
 // continuation; these leaves may not delete the borrowed Record themselves.
 std::function<bool(floors::Record&,std::string&)> floor_aux68_d1,floor_tail_d1;
 FloorCloneDestructionV1 clone;
};
}
