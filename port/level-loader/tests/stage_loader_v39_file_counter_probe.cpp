#include "stage_loader_v39_file_counter.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh2::loader;
static void check(bool b,const char* reason){if(!b)throw std::runtime_error(reason);}
struct Context:std::enable_shared_from_this<Context>{
 enum class Phase{partial,complete};struct Ctor{Phase value{Phase::complete};Phase phase()const{return value;}} ctor;
 struct Fields{std::uint32_t phase30{},field130{8},field134{},field138{},field13c{};std::uintptr_t field140{0xfacebeef};} source;
 struct LoadingFieldsV26{std::shared_ptr<void> level_owner;std::uintptr_t identity;std::uint32_t* progress30;std::uint32_t* state130;};
 struct Borrow{std::shared_ptr<void> owner;std::uintptr_t identity;Fields* fields;};
 std::uintptr_t identity(){return reinterpret_cast<std::uintptr_t>(this);}const Ctor* constructor_owner_v3(){return &ctor;}
 Borrow constructor_borrow_v3(){return {shared_from_this(),identity(),&source};}
 bool loading_fields_v26(LoadingFieldsV26& out,std::string&){out={shared_from_this(),identity(),&source.phase30,&source.field130};return true;}
};
int main(){try{unsigned cases=0;std::string error;
 for(auto initial:{0u,1u,5u,0x7fffffffu,0x80000000u,0xfffffffeu,0xffffffffu}){
  auto level=std::make_shared<Context>();level->source.field13c=initial;FileCounterBorrowV39 actual;check(borrow_file_counter_v39(level,actual,error),"actual borrow shape failed");
  auto increment=actual_file_counter_increment_v39(actual);check(increment(error)&&level->source.field13c==std::uint32_t(initial+1)&&level->source.field140==0xfacebeef,"ADD32 differs or stream pointer140 changed");check(increment(error)&&level->source.field13c==std::uint32_t(initial+1),"post-counter producer replay");++cases;
 }
 {auto level=std::make_shared<Context>();level->ctor.value=Context::Phase::partial;FileCounterBorrowV39 out;check(!borrow_file_counter_v39(level,out,error)&&!out.actual_level_owner,"incomplete C1 accepted");++cases;}
 {auto level=std::make_shared<Context>();level->source.field130=7;level->source.field13c=5;FileCounterBorrowV39 actual;check(borrow_file_counter_v39(level,actual,error),"state7 C1 borrow");auto increment=actual_file_counter_increment_v39(actual);check(!increment(error)&&level->source.field13c==5,"increment before130 transition");level->source.field130=8;check(!increment(error)&&level->source.field13c==5,"failed producer resumed silently");++cases;}
 std::cout<<"PASS actual_word13c_shape_cases="<<cases<<" native_add32_boundary_cases=7 field140_preserved=1 actual_production_C1_executed=0\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
