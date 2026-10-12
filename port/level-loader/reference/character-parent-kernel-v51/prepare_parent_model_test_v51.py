from pathlib import Path
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/character-parent-kernel-v51';tmp=Path('C:/Users/adamc/AppData/Local/Temp')
(r/'parent_kernel_control_v51.cpp').write_bytes((tmp/'parent_kernel_control_v51.cpp').read_bytes())
test=(b/'reference/character-model-route-v45/character_model_route_v45_test.cpp').read_text()
test=test.replace('#include "character_model_route_v45.hpp"','#include "character_model_route_v45.hpp"\n#include "character_parent_init_post_v51.hpp"\n#include "condition_data_kernel_v51.hpp"\n#include "character_set_position_v7.hpp"')
old='if(q.source_entry==0x38be5c){++test.parent_calls;e="Required whole actual GameObject InitPost after produced model/scale";return false;}'
new='''if(q.source_entry==0x38be5c){++test.parent_calls;
   auto same=route->actual_factory.lock()->find(q.subject);auto alias=std::shared_ptr<const void>(same,same->actor.get());character::CharacterParentInitFieldsV48 actual;if(!record.actor->parent_init_fields_v48(alias,actual,e))return false;auto fields=character_gameobject_fields_v51(actual);
   GameObjectInitializationServicesV1 s;s.owner=same;
   s.condition_init=[fields](auto offset,auto& error)mutable{return condition_data_init_borrow_v51(fields,offset,ConditionDataInitServicesV3{},error);};
   s.check_spawn_probability=[&record](auto& value,auto& error){character::NpcInitPostRequestV1 check{0x38bd64,0,0,record.actor->shared_handle().cached,0,nullptr};character::NpcInitPostResponseV1 result;if(!CanonicalCharacterRecordV4::invoke(&record,check,result,error))return false;value=result.value;return true;};
   s.set_position=[same,actual](const float* p,bool destination,auto& error){character::CharacterPositionBorrowV7 b;b.receiver=same;b.identity=actual.identity;b.position160=actual.position160;b.relative144=actual.relative144;b.absolute12c=actual.absolute12c;b.destination1a8=actual.destination1a8;b.physical2dc=actual.physical2dc;b.attached2e0=actual.attached2e0;b.visual2d8=actual.visual2d8;b.publish_position=[same]{std::copy_n(same->actor->object->position.data(),3,same->actor->runtime.subobjects.position);};character::CharacterPositionResultV7 result;return character::character_set_position_v7(result,b,p,destination,{},error);};
   // Actual platform Device is deliberately missing. Whole parent runs its
   // genuine conditions/cache/pose prefix then reports this reached provider.
   return dh2::loader::character_parent_init_post_v51(record,q,response,s,e);
  }'''
assert old in test;test=test.replace(old,new).replace('error.find("whole actual")','error.find("Device::IsHighPerformance")').replace('whole_InitPost_success=0 app_rng0_calls=','shared_parent_actual_condition_cached_spawn_pose_prefix=1 required_actual_Device_stop=1 whole_InitPost_success=0 app_rng0_calls=')
(r/'parent_character_model_v51.cpp').write_text(test)
(r/'prepare_parent_model_test_v51.py').write_bytes(Path(__file__).read_bytes())
print('Prepared real ordered NPC/model ->sharedParent prefix ->requiredDevice stop; no visual success callback')
