#include "lifecycle_v36_counter_teardown.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh2::loader;
static void check(bool x,const char* reason){if(!x)throw std::runtime_error(reason);}
// Explicit binding shape fixture: this is not a real C1 or production context.
struct Context:std::enable_shared_from_this<Context> {
 enum class Phase {partial,complete};
 struct Constructor {Phase value{Phase::partial};Phase phase()const{return value;}} ctor;
 struct Fields {std::uint32_t phase30{},field130{},field134{},field138{};} source;
 struct LoadingFieldsV26 {std::shared_ptr<void> level_owner;std::uintptr_t identity{};std::uint32_t* progress30{},*state130{};};
 struct ConstructorBorrow {std::shared_ptr<void> owner;std::uintptr_t identity{};Fields* fields{};};
 bool shadow{},wrong_owner{};std::uint32_t shadow_progress{};std::shared_ptr<void> wrong=std::make_shared<int>();
 std::uintptr_t identity()const{return reinterpret_cast<std::uintptr_t>(this);}
 const Constructor* constructor_owner_v3()const{return &ctor;}
 ConstructorBorrow constructor_borrow_v3(){return {shared_from_this(),identity(),&source};}
 bool loading_fields_v26(LoadingFieldsV26& out,std::string&){out={wrong_owner?wrong:shared_from_this(),identity(),shadow?&shadow_progress:&source.phase30,&source.field130};return true;}
};
int main(){try{auto context=std::make_shared<Context>();LifecycleBorrowV36 borrow;std::string error;
 check(!borrow_lifecycle_fields_v36(context,borrow,error)&&!borrow.actual_level_owner,"incomplete C1 accepted");
 context->ctor.value=Context::Phase::complete;check(borrow_lifecycle_fields_v36(context,borrow,error),"completed binding rejected");
 check(borrow.fields.counter134==&context->source.field134&&borrow.fields.current138==&context->source.field138,"shadow counter fields");
 *borrow.fields.counter134=500;*borrow.fields.current138=3;check(context->source.field134==500&&context->source.field138==3,"borrow did not write actual fields");
 context->shadow=true;auto before=borrow.fields;check(!borrow_lifecycle_fields_v36(context,borrow,error)&&borrow.fields.progress30==before.progress30,"invalid shadow changed previous borrow");
 context->shadow=false;context->wrong_owner=true;check(!borrow_lifecycle_fields_v36(context,borrow,error),"different owner accepted");
 std::weak_ptr<Context> weak=context;context.reset();check(!weak.expired(),"borrow lost Level lease");borrow={};check(weak.expired(),"borrow leaked Level lease");
 unsigned teardown_cases=0;
 {auto c=std::make_shared<Context>();c->ctor.value=Context::Phase::complete;c->source.field130=7;LifecycleBorrowV36 b;check(borrow_lifecycle_fields_v36(c,b,error),"bind incomplete loaded source");int aborted=0,unloaded=0;LifecycleTeardownServicesV36 services;
  services.abort_incomplete=[&](const auto&,std::string&){++aborted;return LifecycleStepV36::complete;};services.unload_and_destroy_completed=[&](const auto&,std::string&){++unloaded;return LifecycleStepV36::complete;};
  auto cleanup=lifecycle_teardown_provider_v36(b,std::move(services));check(cleanup(error)==LifecycleStepV36::complete&&aborted==1&&unloaded==0,"incomplete abort invoked normal unload/save");++teardown_cases;}
 {auto c=std::make_shared<Context>();c->ctor.value=Context::Phase::complete;c->source.field130=38;LifecycleBorrowV36 b;check(borrow_lifecycle_fields_v36(c,b,error),"bind completed source");int calls=0,aborted=0;LifecycleTeardownServicesV36 services;
  services.abort_incomplete=[&](const auto&,std::string&){++aborted;return LifecycleStepV36::complete;};services.unload_and_destroy_completed=[&](const auto& actual,std::string&){++calls;*actual.fields.state130=0;return calls==1?LifecycleStepV36::pending:LifecycleStepV36::complete;};
  auto cleanup=lifecycle_teardown_provider_v36(b,std::move(services));std::weak_ptr<Context> weak=c;b={};c.reset();check(cleanup(error)==LifecycleStepV36::pending&&!weak.expired(),"normal unload lost receiver before destruction");check(cleanup(error)==LifecycleStepV36::complete&&calls==2&&aborted==0&&weak.expired(),"normal unload switched path or leaked lease");cleanup(error);check(calls==2,"completed cleanup replay");++teardown_cases;}
 {auto c=std::make_shared<Context>();c->ctor.value=Context::Phase::complete;c->source.field130=37;LifecycleBorrowV36 b;check(borrow_lifecycle_fields_v36(c,b,error),"bind prefinish source");auto cleanup=lifecycle_teardown_provider_v36(b,{});check(cleanup(error)==LifecycleStepV36::dependency_missing&&error.find("incomplete-Level abort")!=std::string::npos,"missing abort hidden");++teardown_cases;}
 std::cout<<"PASS binding_shape_cases=6 teardown_routing_cases="<<teardown_cases<<" actual_production_C1_executed=0\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
