#pragma once
#include "source_process_arrays_v101.hpp"
#include <projectile_precache_source_v96.hpp>
#include <visual_fx_manager_libraries_v63.hpp>
#include <cstring>
namespace model_renderer {
inline bool bind_actual_projectile_fx_rows_v96(const std::shared_ptr<dh2::android_ui::SourceProcessArraysV101>& arrays,
 const std::shared_ptr<dh2::fx::VisualFxManagerLibrariesV63>& fx,
 dh2::loader::ProjectilePrecacheSourcesV96& out,std::string& e){
 const auto* group=arrays?arrays->group("ProjectileTable"):nullptr;
 if(!arrays||!arrays->ready()||!group||!group->records_loaded||group->declared_rows!=group->rows.size()||!fx){e="Required SAME completed process ProjectileTable/FX library";return false;}
 if(out.table_owner||out.table_count||out.table_fx_set14||out.register_fx_set){e="Actual projectile FX table providers already bound";return false;}
 out.table_owner=arrays;
 out.table_count=[arrays](std::uint32_t& count,std::string& e){auto actual=arrays->group("ProjectileTable");if(!actual||!actual->records_loaded||actual->declared_rows!=actual->rows.size()){e="Required current actual projectile array count";return false;}count=actual->declared_rows;e.clear();return true;};
 out.table_fx_set14=[arrays](std::uint32_t index,std::int32_t& value,std::string& e){auto actual=arrays->group("ProjectileTable");if(!actual||!actual->records_loaded||index>=actual->rows.size()){e="Required current same projectile row";return false;}
  const auto& fields=actual->rows[index].fields;
  // Structs.Projectile.read4edbb8: b4,b5,i8,ic,b10,i14. Existing registered
  // process decoder owns exact source values, not another schema/table decode.
  if(fields.size()!=20||fields[5].kind!=dh2::android_ui::ProcessArrayValueV101::Kind::word){e="Required actual source Projectile scalar14";return false;}
  std::memcpy(&value,&fields[5].bits,sizeof(value));e.clear();return true;};
 out.register_fx_set=[fx](std::int32_t id,std::string& e){return fx->register_set_to_load(id,e);};
 e.clear();return true;
}
}
