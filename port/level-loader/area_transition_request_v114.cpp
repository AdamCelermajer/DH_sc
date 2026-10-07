#include "area_transition_request_v114.hpp"
#include <algorithm>
#include <cctype>
#include <cstring>
#include <exception>
#include <limits>

namespace dh2::loader {
bool borrow_area_transition_level_v114(const data::LevelTables& tables,const char* name,
 std::int32_t& row,const data::LevelRecord*& record,std::string& e){
 if(!name||tables.level_names.size()!=tables.levels.size()){
  e="Require actual LevelList names/records and non-NULL destination CString";return false;
 }
 auto found=std::find(tables.level_names.begin(),tables.level_names.end(),name);
 if(found==tables.level_names.end()){row=-1;record=nullptr;e.clear();return true;}
 auto index=std::size_t(found-tables.level_names.begin());
 if(index>std::size_t(std::numeric_limits<std::int32_t>::max())){e="LevelList index outside original signed member ID";return false;}
 row=std::int32_t(index);record=&tables.levels[index];e.clear();return true;
}
bool borrow_area_transition_filename_level_v114(const data::LevelTables& tables,const char* filename,
 std::int32_t& row,const data::LevelRecord*& record,std::string& e){
 if(!filename||tables.level_names.size()!=tables.levels.size()){
  e="Require actual LevelList and original filename CString";return false;
 }
 // Same ordered body as existing LevelConstructorV3::construct level_lookup.
 // Never lower caller name, use an asset-root alias, or choose the last match.
 for(std::size_t i=0;i<tables.levels.size();++i){
  const auto& source=tables.levels[i];
  if(source.file.size()>1023||source.file.find('\0')!=std::string::npos){
   e="Level filename outside original CString1024 domain";return false;
  }
  std::string lower=source.file;
  std::transform(lower.begin(),lower.end(),lower.begin(),[](unsigned char ch){return char(std::tolower(ch));});
  if(!std::strstr(filename,lower.c_str()))continue;
  if(i>std::size_t(std::numeric_limits<std::int32_t>::max())){e="LevelList index outside original signed member ID";return false;}
  row=std::int32_t(i);record=&source;e.clear();return true;
 }
 row=-1;record=nullptr;e.clear();return true;
}
bool project_area_transition_request_v114(const AreaTransitionConfirmationV114& confirmed,
 const data::LevelTables& tables,const data::PlayerSavegameV1& save,std::uintptr_t actor,
 std::int32_t difficulty,std::int32_t previous_difficulty,
 AreaTransitionRestoreReceiptV114 restore,AreaTransitionProjectionServicesV114 leaves,
 std::shared_ptr<const AreaTransitionRequestV114>& out,std::string& e)try{
 auto current=[&](){if(!leaves.current){e="Require actual current scope before transition projection";return false;}return leaves.current(e);};
 if(!current())return false;
 if(!actor||save.destroyed_v108()||save.character()!=actor||save.slot()<0||difficulty<0||difficulty>=3||
    confirmed.destination.empty()||confirmed.destination.find('\0')!=std::string::npos){
  e="Require SAME live selected Character/Save/difficulty and genuine confirmed destination";return false;
 }
 if(!restore.file||restore.profile_slot!=save.slot()||restore.selected_character!=actor||
    restore.private_directory.empty()||restore.file->bytes.size()<4){
  e="Require independent current-profile serialized restore receipt for SAME live actor/slot";return false;
 }
 if(!confirmed.profile_slot||!confirmed.destination_domain){
  e="Require original explicit confirmation profileSlot and destination domain";return false;
 }
 const auto slot=*confirmed.profile_slot;
 if(slot!=save.slot()||slot!=restore.profile_slot){
  e="Original confirmation profileSlot differs from actual selected Save/restore; unsupported before retirement";return false;
 }
 std::int32_t row{-1};const data::LevelRecord* definition{};std::string filename;
 if(*confirmed.destination_domain==AreaTransitionDestinationDomainV114::level_member_name){
  if(!borrow_area_transition_level_v114(tables,confirmed.destination.c_str(),row,definition,e))return false;
  if(row==-1||!definition){e="Confirmed member name is absent from actual LevelList";return false;}
  filename=definition->file;
  std::int32_t native_row{-1};const data::LevelRecord* native_definition{};
  if(!borrow_area_transition_filename_level_v114(tables,filename.c_str(),native_row,native_definition,e))return false;
  if(native_row!=row){e="Confirmed member differs from original first-substring C1 filename row";return false;}
 }else if(*confirmed.destination_domain==AreaTransitionDestinationDomainV114::raw_filename){
  filename=confirmed.destination; // FS_LoadLevel args are already filename authority.
  if(!borrow_area_transition_filename_level_v114(tables,filename.c_str(),row,definition,e))return false;
  if(row==-1||!definition){e="Original confirmed filename has no supported C1 LevelList row";return false;}
 }else{e="Unknown original confirmation destination domain";return false;}
 if(filename.empty()||filename.find('\0')!=std::string::npos){
  e="Require original nonempty confirmation filename CString";return false;
 }
 // Application32bf50 -> GS386818: source20=profile slot, source24/28 are
 // separate supplied seeds only in the bypass branch. Native signed bits are
 // preserved, including negative saved seed values and raw zero timer result.
 std::uint32_t seed24=confirmed.explicit_seed24,seed28=confirmed.explicit_seed28;
 if(!confirmed.bypass_saved_seed){
  const auto raw=std::uint32_t(save.location().seeds[std::size_t(difficulty)]);
  seed24=raw;
  bool refresh=raw==0;
  if(raw){
   if(!leaves.debug_load){e="Require original Debug.Load for nonzero saved seed";return false;}
   if(!leaves.debug_load(e)||!current())return false;
   bool force{};
   if(!leaves.debug_switch){e="Require actual DontUsePlayerSeed switch";return false;}
   if(!leaves.debug_switch("DontUsePlayerSeed",force,e)||!current())return false;
   refresh=force;
  }
  if(refresh){
   if(!leaves.real_time){e="Require actual Timer.getRealTime for native saved-seed branch";return false;}
   if(!leaves.real_time(seed24,e)||!current())return false;
  }
  seed28=seed24; //32c124..128 duplicates original selected seed.
 }
 if(!current())return false;
 auto next=std::make_shared<AreaTransitionRequestV114>();
 next->application={filename,confirmed.entry,slot,confirmed.flag_f1,
  confirmed.use_spawn_point,confirmed.mode,confirmed.bypass_saved_seed,confirmed.explicit_seed24,confirmed.explicit_seed28};
 next->source.identity=tables.level_names[std::size_t(row)];
 next->source.definition=filename;
 next->source.kind=std::uint8_t(definition->scalar.words[5])?LevelSourceKindV1::procedural:LevelSourceKindV1::fixed;
 // Existing GS/C1 wire uses word20 as its raw third argument. Do not replace
 // it with generated seed24 or conflate source identity with the asset file.
 next->source.seed=std::uint32_t(slot);
 next->gs={filename,confirmed.entry,next->source.seed,seed24,seed28,
  std::uint8_t(confirmed.flag_f1),std::uint8_t(confirmed.use_spawn_point),previous_difficulty,confirmed.mode};
 next->destination_row=row;next->source_difficulty=difficulty;
 next->source_character=actor;next->restore=std::move(restore);
 if(!current())return false;
 if(save.destroyed_v108()||save.character()!=actor||save.slot()!=slot){
  e="Changed actual Save/slot during original seed selection";return false;
 }
 out=std::move(next);e.clear();return true;
}catch(const std::exception& x){e=x.what();return false;}
catch(...){e="Actual transition projection provider threw";return false;}
}


