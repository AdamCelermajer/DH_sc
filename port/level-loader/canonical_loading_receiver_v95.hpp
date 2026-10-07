#pragma once
#include <canonical_class_receiver_bindings_v1.hpp>
#include <type_traits>
#include <utility>
namespace dh2::world {
namespace loading_detail_v95 {
template<class T,class=void>struct BoolFinal:std::false_type{};
template<class T>struct BoolFinal<T,std::void_t<decltype(std::declval<T&>().init_final(std::declval<bool&>(),std::declval<std::string&>()))>>:std::true_type{};
}
template<class Receiver>void bind_gameobject_loading_fields_v95(const std::shared_ptr<Receiver>& actual,bool updatable,CanonicalClassReceiverV1& result){
 result.source_loading_fields_v95=[actual](CanonicalObjectLoadingFieldsV95& out,std::string& e){
  auto& b=actual->base();CanonicalObjectLoadingFieldsV95 value;
  value.receiver=actual;value.gameobject_base=&b;value.archetype48=b.string(0x48);value.enabled8a=b.byte(0x8a);
  value.minimum_ec=b.integer(0xec);value.disabled_f1=b.byte(0xf1);
  value.condition_a8=b.pointer(0xa8);value.tested_ac=b.byte(0xac);
  value.condition_cc=b.pointer(0xcc);value.tested_d0=b.byte(0xd0);
  if(!value.archetype48||!value.enabled8a||!value.minimum_ec||!value.disabled_f1||!value.condition_a8||!value.tested_ac||!value.condition_cc||!value.tested_d0){e="Required SAME produced ObjectBase loading fields";return false;}
  out=std::move(value);e.clear();return true;
 };
 result.source_is_updatable_v95=[actual,updatable](bool& value,std::string& e){(void)actual;value=updatable;e.clear();return true;};
}
// At actual C1: proven virtual38 literal, existing derived virtual58 method.
template<class Receiver>void bind_gameobject_loading_v95(const std::shared_ptr<Receiver>& actual,bool updatable,CanonicalClassReceiverV1& result){
 bind_gameobject_loading_fields_v95(actual,updatable,result);
 result.source_init_final_v95=[actual](std::string& e){
  if constexpr(loading_detail_v95::BoolFinal<Receiver>::value){bool eligible{};return actual->init_final(eligible,e);}
  else return actual->init_final(e);
 };
}
}
