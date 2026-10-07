#include "script_command_receivers_v59.hpp"
#include <exception>
namespace dh2::loader {
bool CanonicalScriptCommandV59::bind_execution_v96(ScriptCommandBehaviorV59 source,std::string& e){
 auto actual=behaviors_.actual_owner.lock();auto supplied=source.actual_owner.lock();
 if(busy_||released_||!actual||!supplied||actual.get()!=supplied.get()||
    actual.owner_before(supplied)||supplied.owner_before(actual)||!source.execute){
  e="Execution binding requires idle SAME actual command/Application authority";return false;
 }
 behaviors_.execute=std::move(source.execute);behaviors_.blocking=std::move(source.blocking);
 behaviors_.update=std::move(source.update);e.clear();return true;
}
bool CanonicalScriptCommandV59::blocking_v96(bool& out,std::string& e){
 if(busy_||released_||!initialized_){e="IsBlocking requires idle initialized SAME command";return false;}
 CheckedCommandBorrowV59 actual;if(!checked_data_borrow(actual,e))return false;
 //Original eight-byte MOV r0,#0/BX LR class bodies from shipping ELF;
 //these are real literal methods, not missing engine callbacks accepted.
 switch(descriptor_.kind){
 case 1:case 2:case 3:case 4:case 6:case 9:case 10:case 11:case 13:case 14:
 case 15:case 16:case 17:case 18:case 19:case 20:case 21:case 24:case 25:
 case 27:case 28:case 29:case 30:case 31:case 32:case 33:case 34:case 35:
 case 36:case 37:case 38:case 39:case 41:case 42:case 43:case 44:case 46:
 case 47:case 48:case 49:case 50:case 51:case 52:case 53:case 56:case 57:
 case 58:case 59:case 60:case 61:case 62:case 63:case 64:case 65:case 66:
 case 67:case 69:case 70:case 71:case 72:case 73:case 74:case 75:case 76:
 case 77:case 78:case 79:out=false;e.clear();return true;
 case 68:out=true;e.clear();return true; //ShowTrophies455908.
 case 26:{std::uint32_t elapsed{},duration{};
  if(!read_operand_word(16,4,elapsed,e)||!read_operand_word(20,4,duration,e))return false;
  std::int32_t elapsed_signed{},duration_signed{};std::memcpy(&elapsed_signed,&elapsed,4);std::memcpy(&duration_signed,&duration,4);
  out=elapsed_signed<duration_signed;e.clear();return true; //Wait455754 signedCMP.
 }
 default:break;
 }
 auto owner=behaviors_.actual_owner.lock();
 if(!owner||!behaviors_.blocking){e="Required actual "+std::string(descriptor_.class_name)+" IsBlocking source body";return false;}
 struct Busy{bool& b;Busy(bool& v):b(v){b=true;}~Busy(){b=false;}}guard(busy_);
 try{return behaviors_.blocking(actual,out,e);}catch(const std::exception& ex){e=ex.what();return false;}
}
bool CanonicalScriptCommandV59::update_v96(std::string& e){
 if(busy_||released_||!initialized_){e="Update requires idle initialized SAME command";return false;}
 CheckedCommandBorrowV59 actual;if(!checked_data_borrow(actual,e))return false;
 //Every original receiver except Wait inherits ScriptCmdImpl::Update45562c,
 //a literal BX LR. No engine effect is suppressed behind this base method.
 if(descriptor_.kind!=26){e.clear();return true;}
 auto owner=behaviors_.actual_owner.lock();
 if(!owner||!behaviors_.update){e="Required actual Script_Wait Update/GetDt body";return false;}
 struct Busy{bool& b;Busy(bool& v):b(v){b=true;}~Busy(){b=false;}}guard(busy_);
 try{return behaviors_.update(actual,e);}catch(const std::exception& ex){e=ex.what();return false;}
}
}
