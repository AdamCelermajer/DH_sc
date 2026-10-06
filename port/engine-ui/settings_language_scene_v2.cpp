#include "settings_language_scene_v2.hpp"
namespace {
using namespace dh2::ui;
template<class T>bool valid(T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
int deliver(const SettingsSceneServices16V1& s,SettingsSceneOperationV1 op,std::uintptr_t id,std::uint32_t& result){SettingsSceneRequest16V1 request{op,0,id};result=0;return s.invoke(s.context,&request,&result)?-2:0;}
}
extern "C" int dh2_settings_v2_refresh_language_scene(dh2::ui::SettingsLanguageScene24V1* scene,const dh2::ui::SettingsSceneServices16V1* services)noexcept{
 if(!valid(scene)||!valid(services)||!services->invoke||!valid(scene->characters)||!valid(scene->object_end))return -1;
 auto* sentinel=scene->characters;auto* node=sentinel->next;std::uint32_t budget=65536,result=0;
 while(node!=sentinel){
  if(!budget--||!valid(node)||!node->character)return -1;const auto identity=node->character;
  if(deliver(*services,SettingsSceneOperationV1::is_player,identity,result))return -2;
  if(!result&&deliver(*services,SettingsSceneOperationV1::is_merchant,identity,result))return -2;
  if(result&&deliver(*services,SettingsSceneOperationV1::refresh_inventory,identity,result))return -2;
  node=node->next;
 }
 auto* end=scene->object_end;auto* item=scene->object_first;budget=65536;
 while(item!=end){
  if(!budget--||!valid(item))return -1;auto* object=item->object;
  if(object){
   if(!valid(object)||!object->identity||!valid(object->type))return -1;
   if(deliver(*services,SettingsSceneOperationV1::is_game_object,object->identity,result))return -2;
   if(result){
    if(*object->type==3&&deliver(*services,SettingsSceneOperationV1::refresh_item,object->identity,result))return -2;
    if(*object->type==14){if(!object->localization_valid)return -2;*object->localization_valid=0;}
   }
  }
  auto* right=item->right;
  if(right){do{if(!budget--||!valid(right))return -1;item=right;right=right->left;}while(right);}
  else {auto* parent=item->parent;if(!valid(parent))return -1;
   while(item==parent->right){if(!budget--)return -1;item=parent;parent=parent->parent;if(!valid(parent))return -1;}
   if(item->right!=parent)item=parent;
  }
 }
 return 0;
}
