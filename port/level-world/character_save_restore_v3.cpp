#include "character_save_restore_v3.hpp"
#include <cstring>
namespace dh2::character {
namespace {
bool fail(std::string& e,const char* t){if(e.empty())e=t;return false;}
struct Dispatch {
 CharacterSaveRestoreResultV3& result;const CharacterSaveRestoreBorrowV3& f;CharacterSaveRestoreServicesV3 s;std::string& e;
 bool call(std::uint32_t entry,CharacterSaveRestoreResponseV3& out,std::uint32_t a=0,std::uint32_t b=0,std::uintptr_t payload=0,std::uintptr_t subject=0){++result.calls;result.last_entry=entry;out={};const CharacterSaveRestoreRequestV3 request{entry,a,b,subject?subject:f.base.identity,payload};return s.invoke&&s.invoke(s.context,request,out,e)?true:fail(e,"Required reached whole Character serialization helper");}
 bool call(std::uint32_t entry,std::uint32_t a=0,std::uint32_t b=0,std::uintptr_t p=0,std::uintptr_t subject=0){CharacterSaveRestoreResponseV3 out;return call(entry,out,a,b,p,subject);}
};
bool sheet_write(SavegameStreamV2& stream,const data::PropertySheet& sheet,std::uint32_t tag,bool produced,std::string& e){if(!produced)return fail(e,"Required produced legacy Structs property wire tag");return stream.write_u32(tag,e)&&stream.write({reinterpret_cast<const std::uint8_t*>(sheet.data()),896},e);}
bool sheet_read(SavegameStreamV2& stream,data::PropertySheet& sheet,std::uint32_t& tag,bool& produced,std::string& e){if(!stream.read_u32(tag,e))return false;produced=true;return stream.read(sheet.data(),896,e);}
bool point_read(SavegameStreamV2& s,float* p,std::string& e){return p?s.read(p,12,e):fail(e,"Required same source Point3D field");}
bool point_write(SavegameStreamV2& s,const float* p,std::string& e){return p?s.write({reinterpret_cast<const std::uint8_t*>(p),12},e):fail(e,"Required same source Point3D field");}
bool object_service(void* p,const world::ObjectSaveRestoreRequestV3& r,std::string&){auto& d=*static_cast<Dispatch*>(p);return d.call(r.entry,r.argument0,r.argument1,r.payload,r.subject);}
bool group(Dispatch& d,CharacterSaveGroupBorrowV3& out){if(!d.f.group3fc)return fail(d.e,"Required actual CharAI group3fc slot");CharacterSaveRestoreResponseV3 response;if(!d.call(0x3fc,response,0,0,0,*d.f.group3fc))return false;out=response.group;return true;}
}
bool CharacterLegacyPropertyTagsV3::construct_fresh(std::string& e){if(saved_produced||resolved_produced){e="Legacy Structs tag constructor cannot replay";return false;}saved=resolved=legacy_character_properties_tag_v3;saved_produced=resolved_produced=true;return true;}
bool CharacterSaveMetadataV3::construct_fresh(std::string& e){if(pose_produced)return fail(e,"Character save metadata C1 cannot replay");if(!tags.construct_fresh(e))return false;respawn_rotation1468.fill(0);respawn_position1474.fill(0);pose_produced=true;return true;}
bool CharacterSaveMetadataV3::adopt_observed(std::uint32_t saved,std::uint32_t resolved,const float* rotation,const float* position,std::string& e){if(pose_produced||tags.saved_produced||tags.resolved_produced||!rotation||!position)return fail(e,"Required fresh metadata adoption with actual observed fields");tags.saved=saved;tags.resolved=resolved;tags.saved_produced=tags.resolved_produced=true;std::memcpy(respawn_rotation1468.data(),rotation,12);std::memcpy(respawn_position1474.data(),position,12);pose_produced=true;return true;}
bool character_serialize_v3(CharacterSaveRestoreResultV3& result,SavegameStreamV2& stream,const CharacterSaveRestoreBorrowV3& f,CharacterSaveRestoreServicesV3 services,std::string& e){
 result={};e.clear();Dispatch d{result,f,services,e};if(!world::gameobject_serialize_v3(stream,f.base,e))return false;
 if(!f.machine||!f.machine->fsm||f.machine->fsm->character!=f.base.identity||!f.machine->fsm->state)return fail(e,"Required same actual Character state-info owner");
 std::int32_t state;if(dh2_character_native_fsm_get_integer(&state,f.machine->fsm,0)!=1)return fail(e,"Required actual nullable SM_GetState");if(!stream.write_u32(static_cast<std::uint32_t>(state),e))return false;
 CharacterSaveRestoreResponseV3 response;if(!d.call(0x3a49f0,response))return false;
 if(!response.word){if(!f.properties||!f.tags)return fail(e,"Required same NPC property sheets/tag metadata");if(!sheet_write(stream,f.properties->saved,f.tags->saved,f.tags->saved_produced,e)||!sheet_write(stream,f.properties->resolved,f.tags->resolved,f.tags->resolved_produced,e))return false;}
 if(!point_write(stream,f.position160,e)||!point_write(stream,f.rotation16c,e))return false;
 if(!f.life||f.life->dead>255)return fail(e,"Required same dead1449 byte authority");auto dead=static_cast<std::uint8_t>(f.life->dead);if(!stream.write({&dead,1},e)||!point_write(stream,f.respawn_rotation1468,e)||!point_write(stream,f.respawn_position1474,e))return false;
 if(!f.group3fc)return fail(e,"Required actual CharAI group3fc slot");if(*f.group3fc){CharacterSaveGroupBorrowV3 g;if(!group(d,g)||!g.field24)return fail(e,"Required actual group field24");if(!stream.write_u32(static_cast<std::uint32_t>(*g.field24),e))return false;if(!group(d,g)||!g.byte28)return fail(e,"Required actual group byte28");if(!stream.write({g.byte28,1},e))return false;if(!group(d,g)||!g.byte29)return fail(e,"Required actual group byte29");if(!stream.write({g.byte29,1},e))return false;}
 result.completed=1;return true;
}
bool character_deserialize_v3(CharacterSaveRestoreResultV3& result,SavegameStreamV2& stream,const CharacterSaveRestoreBorrowV3& f,CharacterSaveRestoreServicesV3 services,std::string& e){
 result={};e.clear();Dispatch d{result,f,services,e};CharacterSaveRestoreResponseV3 response;
 if(!d.call(0x3a3064,response))return false;if(response.word){if(!d.call(0x31f594,response,0xf3))return false;if(response.word){result.completed=1;return true;}}
 if(!f.buffs)return fail(e,"Required SAME CharProperties buff owner");BuffResult24 buff_result;const int buff_status=dh2_character_buffs_remove_all(&buff_result,f.buffs);if(buff_status!=1)return fail(e,"Required whole source RemoveAllBuffs timer/FX/recalc");
 if(!world::gameobject_deserialize_v3(stream,f.base,{&d,object_service},e))return false;std::uint32_t saved_state;if(!stream.read_u32(saved_state,e))return false;
 if(!d.call(0x3a49f0,response))return false;if(response.word){if(!d.call(0x38b0f0,1))return false;}
 else{if(!f.properties||!f.tags)return fail(e,"Required same NPC property sheets/tag metadata");if(!sheet_read(stream,f.properties->saved,f.tags->saved,f.tags->saved_produced,e)||!sheet_read(stream,f.properties->resolved,f.tags->resolved,f.tags->resolved_produced,e))return false;}
 if(!point_read(stream,f.position160,e)||!point_read(stream,f.rotation16c,e))return false;if(f.publish_pose)f.publish_pose(f.publication_context);
 std::uint8_t saved_dead;if(!stream.read(&saved_dead,1,e)||!point_read(stream,f.respawn_rotation1468,e)||!point_read(stream,f.respawn_position1474,e))return false;
 if(!f.group3fc)return fail(e,"Required actual CharAI group3fc slot");if(*f.group3fc){std::uint32_t value;if(!stream.read_u32(value,e))return false;CharacterSaveGroupBorrowV3 g;if(!group(d,g)||!g.field24)return fail(e,"Required actual group field24");std::memcpy(g.field24,&value,4);if(!group(d,g)||!g.byte28)return fail(e,"Required actual group byte28");if(!stream.read(g.byte28,1,e))return false;if(!group(d,g)||!g.byte29)return fail(e,"Required actual group byte29");if(!stream.read(g.byte29,1,e))return false;}
 if(!f.life)return fail(e,"Required same dead1449 authority");if(f.life->dead){if(!saved_dead){if(!d.call(0x3a59ac,0,1)||!d.call(0x3c1a00,0))return false;if(!f.controller_locked8)return fail(e,"Required SAME Character controller byte8");*f.controller_locked8=0;}}
 else if(saved_dead){if(!f.properties)return fail(e,"Required same restored HP property owner");if(!d.call(0x3e07a0,36,0))return false;}
 f.life->dead=saved_dead;
 if(!d.call(0x3cfd7c)||!d.call(0x3cfde4))return false;
 if(saved_state<=17&&(0x1005u&(1u<<saved_state))){if(!d.call(0x3c01c0,response))return false;if(!response.word){if(!d.call(0x3a5248,response)||!d.call(0x3c1a74,response.word))return false;}f.life->dead=1;}
 else if(saved_state<=17&&(0x20002u&(1u<<saved_state))){if(!d.call(0x3c01d4,response))return false;if(!response.word&&!d.call(0x3c1a64))return false;}
 else if(f.life->dead){f.life->dead=1;if(!d.call(0x3e07a0,36,0)||!d.call(0x3d6cdc)||!d.call(0x3a5248,response)||!d.call(0x3c1a74,response.word))return false;}
 else{if(!f.physical2dc)return fail(e,"Required SAME source physical2dc slot");if(!*f.physical2dc&&!d.call(0x3b4088))return false;if(!d.call(0x3c1a00,0))return false;}
 if(!f.target||!f.target->owner||f.target->owner->identity!=f.base.identity)return fail(e,"Required SAME CharAI target authority");if(dh2_character_ai_set_target(f.target,0,0,f.target_services)||dh2_character_ai_sync_last_target(f.target))return fail(e,"Required whole same AI_SetTarget(NULL,false)/SyncLastTarget");
 if(!d.call(0x3a3064,response))return false;bool initial=response.word!=0;if(!initial){if(!d.call(0x3a49f0,response))return false;if(response.word){if(!d.call(0x31f594,response,0xf2))return false;initial=response.word!=0;}}
 const float* position;const float* rotation;if(initial){position=f.initial_position1450;rotation=f.initial_rotation145c;}else{if(!d.call(0x3935dc,response)||!response.point)return fail(e,"Required actual GetTargetPosition");position=response.point;rotation=f.rotation16c;}
 if(!position||!rotation)return fail(e,"Required actual selected restore pose");if(!d.call(0x393db4,1,0,reinterpret_cast<std::uintptr_t>(position))||!d.call(0x3938a0,0,0,reinterpret_cast<std::uintptr_t>(rotation)))return false;
 if(!f.base.visual2d8)return fail(e,"Required actual VisualObject2d8 slot");if(*f.base.visual2d8&&!d.call(0x38ba74,0,0,0,*f.base.visual2d8))return false;
 if(!f.attached2e0)return fail(e,"Required SAME attached2e0 camera-anchor slot");if(*f.attached2e0&&!d.call(0x2e0,0,0,0,*f.attached2e0))return false;
 result.completed=1;return true;
}
}
