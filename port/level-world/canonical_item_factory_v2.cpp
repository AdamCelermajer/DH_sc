#include "canonical_item_factory_v2.hpp"
#include <cstring>
#include <new>
#include <stdexcept>
namespace dh2::world {
CanonicalItemFactoryV2::CanonicalItemFactoryV2(CanonicalObjectManagerV1& m,CanonicalPropertyMapV1& p,std::shared_ptr<void> world,data::LootTablesV2::Borrow tables,data::LootAudioVisualV8::Borrow av,CanonicalItemFactoryServicesV2 s):manager_(m),map_(p),world_(std::move(world)),tables_(std::move(tables)),audiovisual_(std::move(av)),services_(s){
 if(!world_||!tables_||!audiovisual_)throw std::invalid_argument("Item factory requires actual same World/LootTables/audiovisual leases");
}
bool CanonicalItemFactoryV2::construct_receiver(const CanonicalFactoryEntryV1& f,CanonicalClassReceiverV1& out,std::string& error){
 if(!f.name||std::strcmp(f.name,"Item")||f.original_address!=0x340d38){error="Required original Item factory catalog selector";return false;}
 using Item=character::RetainedWorldItemObjectV1;
 // Real native object address is its identity, not a synthetic pool index.
 void* storage=::operator new(sizeof(Item),std::nothrow);
 if(!storage){out={};return true;}Item* object{};
 try{object=new(storage)Item(reinterpret_cast<std::uintptr_t>(storage),world_,tables_,audiovisual_,services_.item);}
 catch(...){::operator delete(storage);throw;}
 std::shared_ptr<Item> item(object);out=canonical_class_receiver_v1(item);
 // Exact original ItemObject::InitPost3ebca0 is bx lr. It deliberately does
 // NOT call GameObject.InitPost: InitOnce later calls that qualified base.
 out.init_post=[](std::string&){return true;};
 out.position=[item](std::array<float,3>& result,std::string& error){const auto* p=item->base().vector3(0x160);if(!p){error="Required Item position160";return false;}std::copy_n(p,3,result.begin());return true;};
 records_.emplace(out.object.identity,Record{item,out});return true;
}
bool CanonicalItemFactoryV2::construct(void* p,const CanonicalFactoryEntryV1& f,CanonicalClassReceiverV1& out,std::string& e){return static_cast<CanonicalItemFactoryV2*>(p)->construct_receiver(f,out,e);}
bool CanonicalItemFactoryV2::resolve(void* p,target_providers::Handle16& h,bool refresh,const CanonicalObjectBorrowV1*& out,std::string& e){auto& t=*static_cast<CanonicalItemFactoryV2*>(p);if(!t.services_.resolve){e="Required actual Item Spawn Handle::GetObject";return false;}return t.services_.resolve(t.services_.context,h,refresh,out,e);}
bool CanonicalItemFactoryV2::condition(void* p,const CanonicalObjectBorrowV1& object,bool tested,std::string& e){auto& t=*static_cast<CanonicalItemFactoryV2*>(p);if(!t.services_.test_enable_condition){e="Required actual Item ObjectBase.TestEnableCondition";return false;}return t.services_.test_enable_condition(t.services_.context,object,tested,e);}
bool CanonicalItemFactoryV2::debug(void* p,const char* type,std::string& e){auto& t=*static_cast<CanonicalItemFactoryV2*>(p);if(!t.services_.unknown_type_debug){e="Required original Item factory Debug policy";return false;}return t.services_.unknown_type_debug(t.services_.context,type,e);}
bool CanonicalItemFactoryV2::accepted(void* p,const CanonicalObjectBorrowV1& object,bool& value,std::string& e){auto& t=*static_cast<CanonicalItemFactoryV2*>(p);if(!t.find(object.identity)){e="Required same retained Item IsUpdatable receiver";return false;}value=true;return true;}// Exact3ebbf4 movr0#1;bx lr.
bool CanonicalItemFactoryV2::pending(void* p,const CanonicalObjectBorrowV1& object,std::string& e){return static_cast<CanonicalItemFactoryV2*>(p)->manager_.append_pending(object,e);}
bool CanonicalItemFactoryV2::dispatch(void* p,const CanonicalObjectBorrowV1& object,const CanonicalClassReceiverV1*& out,std::string& e){auto& t=*static_cast<CanonicalItemFactoryV2*>(p);auto f=t.records_.find(object.identity);if(f==t.records_.end()||f->second.dispatch.object.lease!=object.lease){e="Required actual retained Item dispatch";out=nullptr;return false;}out=&f->second.dispatch;return true;}
bool CanonicalItemFactoryV2::spawn(const char* type,const char* name,bool deferred,bool network,std::shared_ptr<character::RetainedWorldItemObjectV1>& out,std::string& e){
 out.reset();if(!type||std::strcmp(type,"Item")){e="Item factory cannot substitute for another source class";return false;}
 CanonicalSpawnAttemptV1 attempt(manager_,map_,{this,construct,debug,resolve,condition,accepted,pending,dispatch});
 if(!attempt.spawn(type,name,deferred,network,e))return false;
 const auto* actual=manager_.object(attempt.handle().key);if(!actual)return true;out=find(actual->identity);
 if(!out){e="Required published retained Item after source Spawn";return false;}return true;
}
std::shared_ptr<character::RetainedWorldItemObjectV1> CanonicalItemFactoryV2::find(std::uintptr_t id)const noexcept{auto i=records_.find(id);return i==records_.end()?nullptr:i->second.item;}
}
