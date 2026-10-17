#include "../player_offline_registry_v1.hpp"
#include "../player_local_selection_v1.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>
namespace p=dh2::player_info_record_v1;
namespace r=dh2::player_offline_registration_v1;
namespace s=dh2::player_local_selection_v1;
namespace h=dh2::player_manager_host_level;
unsigned checks=0;
void require(bool value,const char* message){++checks;if(!value)throw std::runtime_error(message);}
struct Memory {
    unsigned allocations=0,frees=0;
    dh2::netstruct_members_v1::Memory services(){return {this,[](void* raw,std::size_t size,int tag)->void*{require(tag==2,"record member allocation tag");auto& m=*static_cast<Memory*>(raw);++m.allocations;return std::malloc(size);},[](void* raw,void* data){++static_cast<Memory*>(raw)->frees;std::free(data);}};}
};
struct Context {
    h::PlayerRegistry* registry=nullptr;
    dh2::player_offline_registry_v1::Owner* owner=nullptr;
    int last_slot=-1;
    s::SavegameManager save{0x12,&last_slot};
    static int online(void*,std::uint8_t* out){*out=0;return 0;}
    dh2::player_locality_v1::Services queries(){dh2::player_locality_v1::Services q{};q.context=this;q.online=online;
        q.internal_id_player=[](void* raw,const h::PlayerRegistry* registry,int id,unsigned flag,h::PlayerInfoProjection** out){auto& c=*static_cast<Context*>(raw);if(registry!=c.registry)return 1;h::Services s{};s.read_online_byte_5=online;h::Result result;return h::get_player_by_internal_id(registry,&s,id,flag,out,&result)==h::Status::complete?0:1;};return q;}
    s::Services selection(const dh2::player_locality_v1::Services& q){s::Services s{};s.context=this;s.queries=&q;
        s.local_controller_66c=[](void* raw,h::PlayerInfoProjection* player,std::uint8_t* out){auto* record=static_cast<Context*>(raw)->owner->record(player);if(!record)return 1;*out=record->local_66c;return 0;};
        s.internal_id_670=[](void* raw,h::PlayerInfoProjection* player,int* out){auto* record=static_cast<Context*>(raw)->owner->record(player);if(!record)return 1;*out=record->internal_id_670;return 0;};
        s.save_slot_664=[](void* raw,h::PlayerInfoProjection* player,int** out){auto* record=static_cast<Context*>(raw)->owner->record(player);if(!record)return 1;*out=&record->save_slot_664;return 0;};
        s.application=[](void*,std::uintptr_t* out){*out=0x11;return 0;};
        s.savegame_manager_4c=[](void* raw,std::uintptr_t app,s::SavegameManager** out){if(app!=0x11)return 1;*out=&static_cast<Context*>(raw)->save;return 0;};
        s.player_manager_40=[](void* raw,std::uintptr_t app,const h::PlayerRegistry** out){if(app!=0x11)return 1;*out=static_cast<Context*>(raw)->registry;return 0;};return s;}
};
int main(){try{Memory memory;
    {
        std::uint64_t serial=0;p::Factory factory(&serial,memory.services());p::Record fallback;
        require(factory.construct_record(fallback)==p::Status::complete,"full fallback construction");
        h::PlayerInfoProjection fallback_view{0x100,-1,&fallback.at(0x310)->header};h::PlayerRegistry registry{0x101,nullptr,0,&fallback_view};
        dh2::input_manager_v1::Manager input{};dh2::input_manager_v1::construct_win32(input,{});
        dh2::player_offline_registry_v1::Owner owner(registry,factory,fallback,input);Context context{&registry,&owner};
        auto queries=context.queries();auto deliveries=owner.services(queries);r::Runtime runtime(registry,deliveries);r::Result result;
        require(serial==13,"one shared initial serial");require(runtime.check_local_controllers(&result)==r::Status::complete,"real input registration caller");
        require(result.controllers_examined==4&&result.added_players==1&&result.renumber_calls==1,"controller0 forced present, other inputs absent");
        require(registry.entry_count==1&&registry.entries[0]!=&fallback_view,"canonical registered node, distinct fallback");
        auto* first=registry.entries[0];auto* record=owner.record(first);
        require(record&&record->base.serial==&serial&&record->base.network.count==33,"one full selected record/counter");
        require(record->internal_id_670==0&&record->controller_668==0&&record->local_66c==1&&record->member_674==-1,"source registration words");
        require(record->number_678==0&&record->group_number_67c==0&&record->save_slot_664==-1&&record->character_660==0,"ordinals do not invent slot/Character");
        require(serial==39&&owner.retained_lifecycle_storage()==3,"outer/default construction, two copies and source retirement");
        require(record->base.network.members[0]!=&record->base.members[0],"source borrowed pointer aliases preserved");
        require(fallback.save_slot_664==-1&&fallback.internal_id_670==-1,"source fallback unchanged");
        auto selection=context.selection(queries);s::Result assigned;
        require(s::assign_save_slot_to_player(&selection,7,0,&assigned)==s::Status::complete,"real authored selection composition");
        require(assigned.player==first&&record->save_slot_664==7&&context.last_slot==7&&fallback.save_slot_664==-1,"slot assigned to registered canonical record");
        require(runtime.check_local_controllers(&result)==r::Status::complete&&result.added_players==0&&serial==39,"repeated scan preserves owner/lifecycle");
        const unsigned prior_allocations=memory.allocations;
        require(runtime.add_player(0,22,3,0,&result)==r::Status::complete&&result.duplicate_adds==1&&result.renumber_calls==0,"source duplicate immediate return");
        require(memory.allocations==prior_allocations&&record->controller_668==0&&serial==39,"duplicate creates no new authority");
        input.gamepads[2].connected_758=1;
        require(runtime.check_local_controllers(&result)==r::Status::complete&&result.added_players==1&&registry.entry_count==2,"later genuine input connection");
        require(registry.entries[0]==first&&record->save_slot_664==7&&serial==65,"stable node and one counter across map growth");
        auto* second=owner.record(registry.entries[1]);require(second&&second->internal_id_670==2&&second->number_678==1&&second->group_number_67c==1,"source sorted ordinals over connected IDs");
        require(s::assign_save_slot_to_player(&selection,9,1,&assigned)==s::Status::complete&&assigned.player==registry.entries[1]&&second->save_slot_664==9,"ordinal selection reaches second canonical record");
        require(p::set_character_scalar(*record,p::character_level,8,0)==p::Status::complete,"real level setter on selected backing");
        h::Services host{};host.read_online_byte_5=Context::online;h::Result hosting;
        require(h::get_hosting_level(&registry,&host,&hosting)==h::Status::complete&&hosting.player_identity==first->identity&&hosting.character_level_330==8,"hosting selector reads the actual registered level member");
        require(fallback.at(0x310)->header.value==-1&&second->at(0x310)->header.value==-1,"no parallel/fallback property overwrite");
        second->local_66c=0;
        require(runtime.update_player_numbers(&result)==r::Status::complete&&second->group_number_67c==0&&second->number_678==1,"remote and local ordinal groups remain separate");
        require(owner.retained_lifecycle_storage()==6,"retired source lifecycle storage bounded by registrations");
    }
    require(memory.allocations==memory.frees,"all original byte payloads retired at teardown");
    std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"byte_allocations\":"<<memory.allocations<<",\"byte_frees\":"<<memory.frees<<",\"scope\":\"Actual selected full records, input owner, registration, canonical map, authored slot selection and hosting-level composition. Native joining, network, Character660 and gameplay remain open.\"}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
