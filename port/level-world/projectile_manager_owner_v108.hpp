#pragma once
#include "canonical_object_manager_v1.hpp"
#include <array>
#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <functional>
#include <exception>
#include <memory>
#include <string>
#include <vector>
namespace dh2::world {
//Same process ProjectileManager singleton9a2878. This is the real empty
//3e60fc C1 and whole Flush3e646c, not a substitute implementation of Spawn.
//Positive source _Create publication must lend its actual canonical receiver
//and virtual40/ObjectBase.Delete methods; a failed callback retains the prefix.
class ProjectileManagerOwnerV108 final {
public:
 struct Row {
  std::shared_ptr<void> receiver;std::uintptr_t identity{};
  std::uint8_t active4{};
  std::function<bool(bool,std::string&)> visible40;
  std::function<bool(std::string&)> object_delete;
  //Actual Main receiver methods/cell; Row.receiver pins these source fields.
  std::function<bool(std::string&)> stop3938f8;
  std::uint8_t* byte85{};
  std::function<bool(std::uintptr_t,std::string&)> set_manager3e4fe4;
 };

 struct CreateNativeV96 {
  std::shared_ptr<void> owner; //independent Main resource/constructor authority.
  //Original App.ObjectManager38 loan, only reached after source name format.
  std::function<bool(std::shared_ptr<CanonicalObjectManagerV1>&,std::string&)> object_manager;
  //Genuine ObjectManager.Spawn34b724 over SAME manager/templates/class factory.
  std::function<bool(CanonicalObjectManagerV1&,const char*,const char*,std::int32_t,bool,target_providers::Handle16&,std::string&)> spawn;
  //Read-only typed projection AFTER actual raw type_f4==9/10. Constructs no
  //receiver and performs no Init/reset/visibility; it pins the SAME allocation.
  std::function<bool(const CanonicalObjectBorrowV1&,bool,Row&,std::string&)> row;
 };
private:
 std::vector<Row> templates4_,ordinary10_,lasers24_;
 std::uint32_t created1c_{},created30_{}; //C1 zero, literal _Create name counters.
 std::uint32_t next20_{},next34_{}; //literal C1 zero, Flush does not reset them.
 bool busy_{},failed_{};std::string failure_;
 //Failed original Spawn/publication prefix stays pinned, never passive D0.
 std::shared_ptr<void> create_primitive_v96_;
 std::shared_ptr<CanonicalObjectManagerV1> create_manager_v96_;
 target_providers::Handle16 create_handle_v96_{};
 CanonicalObjectBorrowV1 create_object_v96_;
 Row create_row_v96_;
 void clear_create_receipt_v96(){create_row_v96_={};create_object_v96_={};create_handle_v96_={};create_manager_v96_.reset();create_primitive_v96_.reset();}

 bool fail(std::string& e){failed_=true;if(failure_.empty())failure_=e.empty()?"Projectile Flush interrupted at source callback":e;e=failure_;return false;}
 bool flush_pool(std::vector<Row>& pool,std::string& e){
  const auto count=pool.size(); //Original captures count once per pool.
  for(std::size_t index=0;index<count;++index){
   if(index>=pool.size()||!pool[index].identity||!pool[index].receiver||!pool[index].visible40){e="Projectile Flush lost an actual captured pool receiver";return false;}
   auto visible=pool[index].visible40;auto pin=pool[index].receiver;
   if(!visible(false,e))return false;
   //Original reloads the pool pointer/row after the virtual callback.
   if(index>=pool.size()||!pool[index].identity||!pool[index].receiver||!pool[index].object_delete){e="Projectile Flush lost the reloaded Delete receiver";return false;}
   auto remove=pool[index].object_delete;pin=pool[index].receiver;
   if(!remove(e))return false;
  }
  pool.clear(); //Source erase(begin,end) retains allocation capacity.
  return true;
 }
public:
 bool publish_created_receiver_v108(bool laser,Row row,std::string& e){
  if(failed_||!row.identity||!row.receiver||!row.visible40||!row.object_delete){e="Required actual source-created Projectile receiver/methods";return false;}
  (laser?lasers24_:ordinary10_).push_back(std::move(row));e.clear();return true;
 }
 bool flush(std::string& e){
  if(failed_){e=failure_;return false;}if(busy_){e="Projectile Flush reentered";return false;}
  busy_=true;struct Scope{bool& busy;~Scope(){busy=false;}}scope{busy_};
  try{if(!flush_pool(ordinary10_,e)||!flush_pool(lasers24_,e))return fail(e);e.clear();return true;}
  catch(const std::exception& ex){e=ex.what();return fail(e);}catch(...){e="Projectile Flush provider threw";return fail(e);}
 }
 // Native failure cleanup is separate from replaying original _Create. The
 // LevelD1 owning-thread barrier authorizes this SAME original Flush order.
 // Failed C1/Spawn receipts remain pinned until actual class unpublication.
 bool flush_for_retirement_v111(const std::shared_ptr<CanonicalObjectManagerV1>& manager,
  const std::function<bool(std::string&)>& quiescent,std::string& e){
  if(busy_||!manager||!quiescent||!quiescent(e)){
   if(e.empty())e="Required SAME idle projectile source retirement";return false;
  }
  if(create_manager_v96_&&(create_manager_v96_.get()!=manager.get()||
     create_manager_v96_.owner_before(manager)||manager.owner_before(create_manager_v96_))){
   e="Projectile failed Spawn belongs to a different source ObjectManager";return false;
  }
  for(const auto* pool:{&ordinary10_,&lasers24_})for(const auto& row:*pool){
   bool present=false;std::int32_t key{};const CanonicalObjectBorrowV1* object{};
   bool next=manager->source_ordered_begin_v38(key,object);
   while(next){if(object&&object->identity==row.identity&&object->lease&&row.receiver&&
      !row.receiver.owner_before(object->lease)&&!object->lease.owner_before(row.receiver)){present=true;break;}
    next=manager->source_ordered_next_v38(key,key,object);}
   if(!present){e="Projectile source Flush pool receiver is not owned by retiring manager";return false;}
  }
  busy_=true;struct Guard{bool& b;~Guard(){b=false;}} guard{busy_};
  try{if(!flush_pool(ordinary10_,e)||!flush_pool(lasers24_,e))return fail(e);e.clear();return true;}
  catch(const std::exception& ex){e=ex.what();return fail(e);}
  catch(...){e="Projectile source retirement Flush threw; prefix retained";return fail(e);}
 }
 bool retire_constructor_receipts_v111(const std::shared_ptr<CanonicalObjectManagerV1>& manager,
  const std::function<bool(std::string&)>& source_unpublished,std::string& e){
  if(busy_||!manager||!ordinary10_.empty()||!lasers24_.empty()||
     !source_unpublished||!source_unpublished(e)){
   if(e.empty())e="Projectile native receipts precede actual source pool/class retirement";return false;
  }
  if(create_manager_v96_&&(create_manager_v96_.get()!=manager.get()||
      create_manager_v96_.owner_before(manager)||manager.owner_before(create_manager_v96_))){
   e="Projectile native failed receipt belongs to another retiring source manager";return false;
  }
  // Native safety-latch release ONLY after genuine D0/map/transport/journal
  // teardown. Source name counters/cursors/pool capacity are never reset.
  clear_create_receipt_v96();failed_=false;failure_.clear();e.clear();return true;
 }


 //Original _Create3e693c. This owns selection/name/counter/row publication,
 //not projectile motion, collision, AI or a second ObjectManager.Spawn body.
 bool create_source_v96(bool first,bool laser,const CreateNativeV96& native,
  const std::function<bool(std::string&)>& current,std::uintptr_t& out,std::string& e){
  out=0;if(failed_){e=failure_;return false;}if(busy_){e="Projectile _Create reentered source delivery";return fail(e);}
  busy_=true;struct Scope{bool& busy;~Scope(){busy=false;}}scope{busy_};
  auto live=[&]{if(failed_){e=failure_;return false;}const bool valid=current&&current(e);
   if(failed_){e=failure_;return false;}if(!valid&&e.empty())e="Required current Projectile _Create scope";return valid;
  };
  try{
   if(!live())return fail(e);
   auto& pool=laser?lasers24_:ordinary10_;auto& cursor=laser?next34_:next20_;
   const auto count=pool.size();
   if(count>0x1fffffffU){e="Actual projectile pool exceeds original row-index domain";return fail(e);}
   if(cursor>=count)cursor=0; //source reset even on empty pool before Spawn.
   const auto start=cursor;std::size_t selected=count;
   //Authentic laser forward branch reads pool10 flags (3e6aac), with count
   //from pool24. It is not silently repaired to scan laser active flags.
   const auto& forward=ordinary10_;
   for(std::size_t index=start;index<count;++index){
    if(index>=forward.size()){e="Original laser forward search exceeds actual ordinary pool10";return fail(e);}
    if(!forward[index].active4){selected=index;break;}
   }
   if(selected==count)for(std::size_t index=0;index<start;++index)if(!pool[index].active4){selected=index;break;}
   if(selected<count){
    cursor=static_cast<std::uint32_t>(selected+1);
    pool[selected].active4=1; //reuse ignores first bool, exactly native.
    //Original reloads pool base for pointer after row4 write; no callbacks.
    const auto pin=pool[selected].receiver;out=pool[selected].identity;
    if(out&&!pin){e="Required live SAME reused projectile receiver";return fail(e);}
    e.clear();return true;
   }
   std::array<char,32> name;
   const auto counter=laser?created30_:created1c_;
   std::snprintf(name.data(),name.size(),laser?"LTProjectile_%03u":"Projectile_%03u",static_cast<unsigned>(counter));
   if(!native.owner||!native.object_manager||!native.spawn){e="Required reached actual Projectile Spawn primitives";return fail(e);}
   create_primitive_v96_=native.owner;
   if(!native.object_manager(create_manager_v96_,e)||!live()||!create_manager_v96_){if(e.empty())e="Required SAME App.ObjectManager38 for Projectile Spawn";return fail(e);}
   if(!native.spawn(*create_manager_v96_,laser?"LaserTypeProjectile":"Projectile",name.data(),0,true,create_handle_v96_,e)||!live())return fail(e);
   const CanonicalObjectBorrowV1* object{};
   //Consume existing exact ObjectHandle.GetObject33fdc0 kernel with false,
   //rather than asking Main to implement another resolve/cache/map operation.
   if(!create_manager_v96_->resolve_handle_v4(create_handle_v96_,false,object,{},e))return fail(e);
   if(object)create_object_v96_=*object; //pin before any further native lender.
   if(!live())return fail(e);
   if(!object||!create_object_v96_.identity){clear_create_receipt_v96();e.clear();return true;} //authentic NULL result.
   if(!create_object_v96_.lease||!create_object_v96_.type_f4){e="Required SAME spawned receiver/raw type_f4";return fail(e);}
   if(*create_object_v96_.type_f4!=(laser?10U:9U)){clear_create_receipt_v96();e.clear();return true;} //source semantic NULL, published object remains Main-owned.
   if(!native.row){e="Required reached SAME typed Projectile row projection";return fail(e);}
   if(!native.row(create_object_v96_,laser,create_row_v96_,e)||!live())return fail(e);
   if(!create_row_v96_.receiver||create_row_v96_.identity!=create_object_v96_.identity||
      create_row_v96_.receiver.owner_before(create_object_v96_.lease)||create_object_v96_.lease.owner_before(create_row_v96_.receiver)){
    e="Projectile type projection differs from SAME spawned source allocation";return fail(e);
   }
   //Original rereads the actual counter AFTER Spawn/GetObject/type check.
   //These stores precede append allocation and SetManager, preserving failure.
   cursor=0;auto& actual_counter=laser?created30_:created1c_;++actual_counter;
   auto& destination=laser?lasers24_:ordinary10_;create_row_v96_.active4=first?1:0;
   if(destination.size()==destination.capacity()){
    const auto size=destination.size();const auto grown=size?std::min<std::size_t>(size*2,0x1fffffffU):1;
    if(grown<=size){e="Original projectile append exceeds source capacity domain";return fail(e);}
    destination.reserve(grown); //native vector doubles its row capacity.
   }
   destination.push_back(create_row_v96_);
   //Publication has REALLY occurred before SetManager3e4fe4; failed leaves
   //retain both this row and the actual constructor/manager receipt.
   if(!create_row_v96_.set_manager3e4fe4){e="Required SAME actual Projectile.SetManager3e4fe4";return fail(e);}
   if(!create_row_v96_.set_manager3e4fe4(identity(),e)||!live())return fail(e);
   out=create_row_v96_.identity;clear_create_receipt_v96();e.clear();return true;
  }catch(const std::exception& ex){e=ex.what();return fail(e);}catch(...){e="Projectile _Create provider threw; actual prefix retained";return fail(e);}
 }
 //Borrow SAME native pool10/24 storage; reserve does not create receivers.
 bool pool_capacity_v96(bool laser,std::size_t& out,std::string& e)const{
  if(failed_){e=failure_;return false;}out=(laser?lasers24_:ordinary10_).capacity();e.clear();return true;
 }
 bool pool_count_v96(bool laser,std::size_t& out,std::string& e)const{
  if(failed_){e=failure_;return false;}out=(laser?lasers24_:ordinary10_).size();e.clear();return true;
 }
 bool pool_reserve_v96(bool laser,std::size_t count,std::string& e){
  if(failed_){e=failure_;return false;}
  try{(laser?lasers24_:ordinary10_).reserve(count);e.clear();return true;}
  catch(const std::exception& ex){e=ex.what();return fail(e);}catch(...){e="Actual projectile pool reserve threw";return fail(e);}
 }
 bool pool_entry_v96(bool laser,std::size_t index,std::uintptr_t& id,std::shared_ptr<void>& pin,std::string& e)const{
  if(failed_){e=failure_;return false;}const auto& pool=laser?lasers24_:ordinary10_;
  if(index>=pool.size()){e="Actual projectile pool lost its captured index";return false;}
  id=pool[index].identity;pin=pool[index].receiver;e.clear();return true;
 }
 using MissingProjectileV96=std::function<bool(std::uintptr_t,bool,std::string&)>;
 //Whole DeSpawn3e61b4: ascending pointer search, original receiver methods,
 //actual byte85 store, reloaded row4 store, then actual cursor20/34 store.
 bool despawn_source_v96(std::uintptr_t id,bool laser,
  const std::function<bool(std::string&)>& current,
  const MissingProjectileV96& missing,std::string& e){
  if(failed_){e=failure_;return false;}if(busy_){e="Projectile DeSpawn reentered native delivery";return fail(e);}
  busy_=true;struct Scope{bool& busy;~Scope(){busy=false;}}scope{busy_};
  auto live=[&]{if(failed_){e=failure_;return false;}
   const bool valid=current&&current(e);if(failed_){e=failure_;return false;}
   if(!valid&&e.empty())e="Required current projectile native delivery scope";return valid;
  };
  try{
   if(!live())return fail(e);
   auto& pool=laser?lasers24_:ordinary10_;const auto count=pool.size();
   std::size_t index=0;for(;id&&index<count;++index)if(pool[index].identity==id)break;
   if(!id||index==count){
    //Native assertion modes may return or deliberately fault. Main lends its
    //real diagnostic delivery; no mode/default/NULL success is invented here.
    if(!missing){e="Required native Projectile DeSpawn NULL/not-found diagnostic";return fail(e);}
    if(!missing(id,laser,e)||!live())return fail(e);e.clear();return true;
   }
   const auto actual=pool[index]; //pins SAME original r6 across virtual/Stop.
   if(!actual.receiver||!actual.visible40){e="Required reached actual Projectile virtual40";return fail(e);}
   if(!actual.visible40(false,e)||!live())return fail(e);
   if(!actual.stop3938f8){e="Required SAME Projectile GameObject.Stop3938f8";return fail(e);}
   if(!actual.stop3938f8(e)||!live())return fail(e);
   if(!actual.byte85){e="Required SAME actual Projectile byte85 cell";return fail(e);}
   *actual.byte85=0; //not Row.active4 and not a shadow source-active byte.
   //Native reloads pool base after both callbacks and byte85; never use an
   //earlier vector element reference across possible reserve/publication.
   auto& reloaded=laser?lasers24_:ordinary10_;
   if(index>=reloaded.size()){e="DeSpawn lost reloaded source pool index after actual byte85 store";return fail(e);}
   reloaded[index].active4=0;
   if(laser)next34_=static_cast<std::uint32_t>(index);else next20_=static_cast<std::uint32_t>(index);
   e.clear();return true;
  }catch(const std::exception& ex){e=ex.what();return fail(e);}catch(...){e="Projectile DeSpawn provider threw";return fail(e);}
 }
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
};
inline std::shared_ptr<ProjectileManagerOwnerV108> projectile_manager_process_v108(){
 static const auto instance=std::make_shared<ProjectileManagerOwnerV108>();return instance;
}
}
