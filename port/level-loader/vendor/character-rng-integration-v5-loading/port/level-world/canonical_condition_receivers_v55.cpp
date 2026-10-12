#include "canonical_condition_receivers_v55.hpp"
namespace dh2::world {
namespace {
constexpr CanonicalConditionProfileV55 profiles[]{{0x478b5c,0x969240,12,0x478fe8,0x479320,0x969200},{0x478b14,0x969220,12,0x478fe8,0x4792e4,0x969200},{0x478acc,0x9691e0,12,0x478fe8,0x4792a8,0x969200},{0x478a80,0x9691c0,8,0x478d78,0x479230,0x9691c0},{0x478a34,0x9691a0,8,0x478638,0x47917c,0x9691a0},{0x4789e8,0x969180,8,0x478640,0x47926c,0x969180},{0x47899c,0x969160,8,0x478db4,0x4791f4,0x969160}};
class QuestConditionV55 final:public CanonicalConditionReceiverV55 {
 std::int32_t comparator8_;
 std::int32_t* comparator_cell8()noexcept override{return &comparator8_;}
public:QuestConditionV55(std::shared_ptr<const void> world,const CanonicalConditionProfileV55& p,std::int32_t comparator):CanonicalConditionReceiverV55(std::move(world),p),comparator8_(comparator){}
};
class SimpleConditionV55 final:public CanonicalConditionReceiverV55 {
public:SimpleConditionV55(std::shared_ptr<const void> world,const CanonicalConditionProfileV55& p):CanonicalConditionReceiverV55(std::move(world),p){}
};
}
CanonicalConditionReceiverV55::CanonicalConditionReceiverV55(std::shared_ptr<const void> world,const CanonicalConditionProfileV55& p):world_(std::move(world)),profile_(&p),dispatch0_(p.vtable){}
CanonicalConditionFactoryV55::CanonicalConditionFactoryV55(std::shared_ptr<const void> world,ConditionEvaluationServicesV55 eval):world_(std::move(world)),eval_owner_(eval.owner),eval_(std::move(eval)){eval_.owner.reset();}
const CanonicalConditionProfileV55* CanonicalConditionFactoryV55::source_profile(unsigned op)noexcept{return op<7?&profiles[op]:nullptr;}
bool CanonicalConditionFactoryV55::construct(unsigned op,ActualConditionBorrowV55& out,std::string& error){
 auto authority=world_.lock();if(!authority||op>=7){error="Required actual retained Condition world/source factory index";return false;}
 std::shared_ptr<CanonicalConditionReceiverV55> receiver;
 if(op<3)receiver=std::make_shared<QuestConditionV55>(authority,profiles[op],static_cast<std::int32_t>(op));
 else receiver=std::make_shared<SimpleConditionV55>(authority,profiles[op]);
 const auto id=reinterpret_cast<std::uintptr_t>(receiver.get());receivers_[id]=receiver;
 ActualConditionBorrowV55 actual{id,receiver,&receiver->stub4_};out=std::move(actual);error.clear();return true;
}
bool CanonicalConditionFactoryV55::borrow(std::uintptr_t id,CanonicalConditionFieldsV55& out,std::string& error)const{
 const auto i=receivers_.find(id);auto receiver=i==receivers_.end()?nullptr:i->second.lock();
 auto authority=world_.lock();if(!receiver||receiver->retired_||!authority||receiver->world_.lock()!=authority||reinterpret_cast<std::uintptr_t>(receiver.get())!=id){error="Required same live canonical Condition receiver";return false;}
 CanonicalConditionFieldsV55 actual{receiver,authority,id,receiver->profile_,&receiver->dispatch0_,&receiver->stub4_,receiver->comparator_cell8()};out=std::move(actual);error.clear();return true;
}
bool CanonicalConditionFactoryV55::deleting_destructor(const ActualConditionBorrowV55& instance,std::string& error){
 CanonicalConditionFieldsV55 actual;
 if(!borrow(instance.receiver,actual,error))return false;
 if(instance.receiver_alias.get()!=actual.receiver_alias.get()||instance.live_stub4!=actual.stub4){error="Required checked original Condition D0 receiver alias";return false;}
 auto receiver=receivers_.at(instance.receiver).lock();
 // Original concrete D0 writes its proven destructor vtable, calls no-op
 // Condition D1, then CustomFree. Stub/discriminator remain untouched.
 *actual.source_dispatch0=actual.profile->deleting_vtable;receiver->retired_=true;receivers_.erase(instance.receiver);
 // Lifetime pins prevent native dangling pointers. Retired receiver can never
 // be evaluated/borrowed again; owner drops its pin after this real D0 route.
 error.clear();return true;
}
ActualConditionFactoriesV55 CanonicalConditionFactoryV55::factories(){
 ActualConditionFactoriesV55 s;s.owner=shared_from_this();std::weak_ptr<CanonicalConditionFactoryV55> weak=shared_from_this();
 for(unsigned op=0;op<7;++op)s.create[op]=[weak,op](ActualConditionBorrowV55& out,std::string& e){auto self=weak.lock();if(!self){e="Required retained canonical Condition factory";return false;}return self->construct(op,out,e);};
 s.deleting_destructor=[weak](const ActualConditionBorrowV55& actual,std::string& e){auto self=weak.lock();if(!self){e="Required retained canonical Condition factory";return false;}return self->deleting_destructor(actual,e);};return s;
}
bool CanonicalConditionFactoryV55::evaluate(std::uintptr_t id,bool& out,std::string& error)const{
 CanonicalConditionFieldsV55 actual;if(!borrow(id,actual,error))return false;
 if(!*actual.stub4){error="Required actual assigned Condition stub4";return false;}
 const auto evaluator=eval_owner_.lock();if(!evaluator||!eval_.invoke_original){error="Required main original Condition Eval "+std::to_string(actual.profile->eval);return false;}
 bool value;if(!eval_.invoke_original(actual,value,error))return false;out=value;error.clear();return true;
}
}
