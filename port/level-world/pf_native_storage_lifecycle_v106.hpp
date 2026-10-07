#pragma once
#include "pf_flush_services_v1.hpp"
#include <stdexcept>
namespace dh2::world {
// Native backend ownership adaptation: the port's topology/link arrays replace
// original sparse-graph allocations. Receipts release THIS World storage, not
// emulated ARM graph instances or a second navigation authority. Constructed
// alongside a fresh ModulePFRooms before its first actual LoadRoom.
class PFNativeStorageLifecycleV106 final : public std::enable_shared_from_this<PFNativeStorageLifecycleV106> {
 struct Domain {enum Kind {topology,links} kind;bool retired{};explicit Domain(Kind k):kind(k){}};
 std::weak_ptr<floors::World> world_;
 std::shared_ptr<Domain> outer_,inner_;
 std::uint32_t initialized4_{};
 std::uintptr_t outer44_{},inner48_{};
 template<class T>static void release(T& value){T{}.swap(value);}
 bool graph(std::uintptr_t id,PFGraphDestructionV1& out,std::string& e){
  auto world=world_.lock();auto domain=id==outer44_?outer_:id==inner48_?inner_:nullptr;
  if(!world||!domain||domain->retired||reinterpret_cast<std::uintptr_t>(domain.get())!=id){e="Required SAME native PF storage domain";return false;}
  out.receiver=domain;out.identity=id;const auto weak=weak_from_this();
  out.native_d0=[weak,domain](std::string& e){auto self=weak.lock();auto world=self?self->world_.lock():nullptr;
   if(!self||!world||domain->retired){e="Retired native PF storage D0 receipt";return false;}
   // Original source has already dropped all PFFloor owners. Native numeric
   // nodes/edges have no virtual children; destroy their real array storage.
   if(domain->kind==Domain::topology){
    release(world->nodes);release(world->edges);release(world->invalid);release(world->validation);
    release(world->floor_graphs);release(world->validation_floors);world->graph={};
   }else{
    release(world->first_boundary);release(world->second_boundary);release(world->links);
    release(world->search_edges);release(world->search_offsets);release(world->search_nodes);release(world->search_heap);
    world->sewing={};world->route_graph={};world->route_search={};
   }
   domain->retired=true;
   if(domain==self->outer_)self->outer_.reset();else self->inner_.reset();
   e.clear();return true;
  };e.clear();return true;
 }
 explicit PFNativeStorageLifecycleV106(std::shared_ptr<floors::World> w):world_(std::move(w)){}
public:
 static std::shared_ptr<PFNativeStorageLifecycleV106> create_fresh(const std::shared_ptr<floors::World>& world){
  if(!world||world->sewn||!world->records.empty()||!world->nodes.empty()||!world->edges.empty()||!world->floor_graphs.empty())
   throw std::invalid_argument("PF lifecycle must be produced before first navigation allocation");
  return std::shared_ptr<PFNativeStorageLifecycleV106>(new PFNativeStorageLifecycleV106(world));
 }
 bool initialize_for_load(std::string& e){
  if(initialized4_){e.clear();return true;}
  // Preserve reached allocation prefixes. A failed second allocation may not
  // replace the already-created topology domain on a later attempt.
  if(!outer_)outer_=std::make_shared<Domain>(Domain::topology);
  outer44_=reinterpret_cast<std::uintptr_t>(outer_.get());
  auto inner=std::make_shared<Domain>(Domain::links);
  initialized4_=1;inner_=std::move(inner);inner48_=reinterpret_cast<std::uintptr_t>(inner_.get());e.clear();return true;
 }
 bool lend(floors::World& actual,std::uintptr_t id,
  std::function<bool(std::string&)> erase_objects,PFWorldFlushFieldsV1& out,std::string& e){
  auto world=world_.lock();if(!world||world.get()!=&actual||id!=identity()||!erase_objects){e="Required SAME native PF storage and actual floor-object registry";return false;}
  out.owner=shared_from_this();out.identity=identity();out.storage_owner=world;
  out.initialized4=&initialized4_;out.outer44=&outer44_;out.inner48=&inner48_;
  out.erase_floor_objects2c=std::move(erase_objects);const auto weak=weak_from_this();
  out.graph=[weak](auto id,auto& out,auto& e){auto self=weak.lock();if(!self){e="Retired native PF lifecycle";return false;}return self->graph(id,out,e);};
  e.clear();return true;
 }
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
};
}
