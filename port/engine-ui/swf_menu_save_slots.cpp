#include "swf_menu_save_slots.hpp"
#include "gameswf/gameswf_function.h"
#include "gameswf/gameswf_object.h"
#include <cerrno>
#include <cmath>
#include <cstdio>
#include <limits>
#include <string>
#include <sys/stat.h>
namespace dh2::ui {
bool swf_front_pending_start_v1(const gameswf::fn_call& fn,std::int32_t& difficulty,std::string& error){
    if(fn.nargs!=1||!fn.arg(0).is_number()){error="Malformed authored launch difficulty";return false;}
    const double value=fn.arg(0).to_number();
    if(!std::isfinite(value)||value<0||value>2){error="Unsafe authored launch difficulty";return false;}
    difficulty=static_cast<std::int32_t>(value);error.clear();return true;
}

bool swf_front_assign_save_slot_args_v1(const gameswf::fn_call& fn,std::int32_t& local_index,
    std::int32_t& selected_slot,std::string& error){
    if(fn.nargs!=2||!fn.env||!fn.arg(0).is_number()||!fn.arg(1).is_number()){
        error="Malformed NativeAssignSaveSlotToPlayer AS call";return false;
    }
    // fn_call::arg(n) indexes from the top of the ActionScript operand stack.
    const double local=fn.arg(1).to_number();
    const double slot=fn.arg(0).to_number();
    if(!std::isfinite(local)||!std::isfinite(slot)||
       local<double(std::numeric_limits<std::int32_t>::min())||local>=2147483648.0||
       slot<double(std::numeric_limits<std::int32_t>::min())||slot>=2147483648.0){
        error="Unsafe NativeAssignSaveSlotToPlayer integer arguments";return false;
    }
    local_index=static_cast<std::int32_t>(local);
    selected_slot=static_cast<std::int32_t>(slot);
    error.clear();return true;
}

bool swf_front_create_save_slot_v1(const gameswf::fn_call& fn,void* context,
    bool (*create)(void*,const char*,const char*,std::uint32_t&,std::string&),std::string& error){
    if(fn.nargs!=2){error.clear();return true;}
    if(!fn.env||!fn.result||!create){error="Malformed persona creation call";return false;}
    const std::string name=fn.arg(0).to_tu_string().c_str();
    const std::string character=fn.arg(1).to_tu_string().c_str();
    if(character!="KnightPlayerBase"&&character!="RoguePlayerBase"&&character!="MagePlayerBase"){error.clear();return true;}
    std::uint32_t slot{};
    if(!create(context,name.c_str(),character.c_str(),slot,error))return false;
    fn.result->set_double(double(slot));error.clear();return true;
}
bool swf_front_erase_save_slot_v1(const gameswf::fn_call& fn,const SwfFrontEraseSlotServicesV1& services,std::string& error){
    if(fn.nargs<1||!fn.env){error="Malformed erase-slot AS call";return false;}
    const double number=fn.arg(0).to_number();
    if(!std::isfinite(number)||number<-2147483648.0||number>=2147483648.0){error="Unsafe erase-slot number";return false;}
    const auto slot=static_cast<std::int32_t>(number);
    if(slot<0){error.clear();return true;}
    if(slot>=4){error="Unsafe erase-slot index";return false;}
    if(!services.flush_jobs||!services.erase_files){error="Campaign erase services unavailable";return false;}
    if(!services.flush_jobs(services.context,error))return false;
    return services.erase_files(services.context,static_cast<std::uint32_t>(slot),error);
}
void swf_front_write_save_slot_details_v1(gameswf::as_object* object,const SwfFrontSaveSlotDetailsV1& details){
    if(!object)return;
    const bool used=details.in_use;
    object->set_member("SlotID",gameswf::as_value(details.slot_id));
    object->set_member("InUse",gameswf::as_value(used));
    object->set_member("PlayerName",gameswf::as_value(used?details.player_name.c_str():""));
    object->set_member("PlayerClass",gameswf::as_value(used?details.player_class.c_str():""));
    object->set_member("PlayerLVL",gameswf::as_value(used?details.player_level:-1));
    object->set_member("StringClassLVL",gameswf::as_value(used?details.string_class_level.c_str():""));
    object->set_member("PlayerLocation",gameswf::as_value(used?details.player_location.c_str():""));
    object->set_member("PlayerLastSave",gameswf::as_value(used?details.last_save.c_str():""));
    object->set_member("PlayerCurrentAct",gameswf::as_value(used?details.current_act:1));
    object->set_member("Difficulty",gameswf::as_value(used?details.difficulty:0));
    object->set_member("DifficultyUnlocked",gameswf::as_value(used?details.difficulty_unlocked:0));
}
// NativeGetSaveSlotDetails 0x44aa28 orders four occupied slots before empty
// slots. Connect the genuine all-files-absent branch; existing campaigns need
// the complete PlayerSavegame producer and must never be called empty.
bool swf_front_select_save_slot_v1(const std::array<bool,4>& occupied,std::uint32_t index,
    std::uint32_t& slot,bool& in_use,std::string& error){
    if(index>=4){error="Unsafe save-slot display index";return false;}
    std::array<std::uint32_t,4> used{};std::uint32_t count=0,first_free=4;
    for(std::uint32_t i=0;i<4;++i){
        if(occupied[i])used[count++]=i;
        else if(first_free==4)first_free=i;
    }
    in_use=index<count;slot=in_use?used[index]:first_free;
    error.clear();return true;
}
bool swf_front_save_slot_details(const gameswf::fn_call& fn,const SwfFrontSaveSlotServicesV1& services,std::string& error){
    if(fn.nargs<2||!fn.env||!fn.result){error="Malformed save-slot AS call";return false;}
    const double index=fn.arg(0).to_number();
    if(!std::isfinite(index)||index<0||index>=4){error="Unsafe save-slot display index";return false;}
    std::int32_t difficulty=-1;
    // NativeGetSaveSlotDetails tests nargs==3, rather than nargs>=3.
    if(fn.nargs==3){
        const double value=fn.arg(2).to_number();
        if(!std::isfinite(value)||value<-1||value>=3){error="Unsafe save-slot difficulty override";return false;}
        difficulty=static_cast<std::int32_t>(value);
    }
    if(!services.exists||!services.load_details){error="Save-slot profile services unavailable";return false;}
    std::array<bool,4> occupied{};
    for(std::uint32_t slot=0;slot<4;++slot)
        if(!services.exists(services.context,slot,occupied[slot],error))return false;
    std::uint32_t slot=0;bool in_use=false;
    if(!swf_front_select_save_slot_v1(occupied,static_cast<std::uint32_t>(index),slot,in_use,error))return false;
    SwfFrontSaveSlotDetailsV1 details;
    if(!services.load_details(services.context,slot,in_use,difficulty,details,error))return false;
    // Slot identity and occupancy come from the original ordered scan.
    details.slot_id=static_cast<std::int32_t>(slot);details.in_use=in_use;
    gameswf::gc_ptr<gameswf::as_object> object=fn.arg(1).is_object()?fn.arg(1).to_object():nullptr;
    if(!object)return true;
    swf_front_write_save_slot_details_v1(object.get_ptr(),details);
    fn.result->set_as_object(object.get_ptr());return true;
}
static bool front_file_exists(void* context,std::uint32_t slot,bool& occupied,std::string& error){
    const auto& directory=*static_cast<const std::string*>(context);
    char name[32];std::snprintf(name,sizeof(name),"dh2_%03u.savegame",slot);
    occupied=false;
    for(const auto& suffix:{std::string(),std::string(".bak")}){
        const auto path=directory+"/"+name+suffix;struct stat info{};
        if(!stat(path.c_str(),&info)){occupied=true;error.clear();return true;}
        if(errno!=ENOENT){error="Cannot inspect campaign save: "+path;return false;}
    }
    error.clear();return true;
}
static bool front_absent_profile(void*,std::uint32_t,bool occupied,std::int32_t,
    SwfFrontSaveSlotDetailsV1& details,std::string& error){
    if(occupied){error="Existing campaign save requires PlayerSavegame loading";return false;}
    details={};error.clear();return true;
}
bool swf_front_save_slot_details(const gameswf::fn_call& fn,const std::string& directory,std::string& error){
    SwfFrontSaveSlotServicesV1 services{const_cast<std::string*>(&directory),front_file_exists,front_absent_profile};
    return swf_front_save_slot_details(fn,services,error);
}
}
