#include "object_enable_condition_v2.hpp"
namespace dh2::world {
bool object_set_enable_v2(ObjectEnableConditionBorrowV2 b,ObjectEnableConditionServicesV2 s,bool value,std::string& error){
 if(!b.enabled8a){error="Required same ObjectBase enabled8a";return false;}
 if(*b.enabled8a==static_cast<std::uint8_t>(value))return true;
 *b.enabled8a=static_cast<std::uint8_t>(value);
 if(!s.enabled_event){error="Required actual ObjectBase enabled virtual";return false;}
 return s.enabled_event(s.context,value,error);
}
bool object_test_enable_condition_v2(ObjectEnableConditionBorrowV2 b,ObjectEnableConditionServicesV2 s,bool mark,bool& enabled,std::string& error){
 if(!b.enabled8a){error="Required same ObjectBase enabled8a";return false;}
 enabled=*b.enabled8a!=0;
 if(!s.local_profile){error="Required actual local Character/Save projection";return false;}
 bool character=false,save=false;std::uint8_t byte14=0;
 if(!s.local_profile(s.context,character,save,byte14,error))return false;
 if(character&&(!save||!byte14))return true;
 if(!s.current_level){error="Required actual current Level projection";return false;}
 bool level=false;std::int32_t difficulty=0;
 if(!s.current_level(s.context,level,difficulty,error))return false;
 if(!b.disable_f1){error="Required same ObjectBase disable_f1";return false;}
 bool accepted=*b.disable_f1==0;
 if(level){
  if(!b.minimum_difficulty_ec){error="Required same ObjectBase minimum difficulty";return false;}
  auto minimum=*b.minimum_difficulty_ec;
  if(minimum==-1)minimum=0;
  if(minimum>difficulty)accepted=false;
 }
 if(accepted){
  if(!b.tested_ac||!b.condition_a8){error="Required same retained ConditionData fields";return false;}
  if(!*b.tested_ac&&*b.condition_a8){
   if(!s.condition_is_true){error="Required actual compiled Condition IsTrue";return false;}
   if(!s.condition_is_true(s.context,*b.condition_a8,accepted,error))return false;
  }
 }
 if(!object_set_enable_v2(b,s,accepted,error)){enabled=*b.enabled8a!=0;return false;}
 if(accepted&&mark)*b.tested_ac=1;
 enabled=*b.enabled8a!=0;return true;
}
bool object_test_disable_condition_v103(ObjectEnableConditionBorrowV2 b,ObjectEnableConditionServicesV2 s,bool mark,bool& enabled,std::string& error){
 if(!b.enabled8a){error="Required same ObjectBase enabled8a";return false;}enabled=*b.enabled8a!=0;
 if(!s.local_profile){error="Required actual local Character/Save projection";return false;}bool character{},save{};std::uint8_t byte14{};
 if(!s.local_profile(s.context,character,save,byte14,error))return false;
 if(character&&(!save||!byte14))return true;
 if(!b.tested_ac||!b.condition_a8){error="Required same ConditionData cc/d0 fields";return false;}
 if(*b.tested_ac||!*b.condition_a8)return true;
 bool condition{};if(!s.condition_is_true||!s.condition_is_true(s.context,*b.condition_a8,condition,error))return false;
 if(condition)return true;
 if(!object_set_enable_v2(b,s,false,error)){enabled=*b.enabled8a!=0;return false;}
 if(mark)*b.tested_ac=1;enabled=*b.enabled8a!=0;return true;
}
}
