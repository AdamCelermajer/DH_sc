#include "player_add_character_v1.hpp"
#include <cstdio>
#include <cstring>

namespace dh2::player_add_character_v1 {
namespace {
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn) {
    if (!a || !b || !an || !bn) return false;
    auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
    if (an>UINTPTR_MAX-x || bn>UINTPTR_MAX-y) return true;
    return x<y+bn && y<x+an;
}
template<class T> bool aligned(const T* p) {
    return p && reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;
}
bool player_valid(const Player* p) {return p && p->identity && p->character_level_member;}
bool registry_valid(const Registry& p) {
    if (!p.manager_identity || !player_valid(p.manager_plus_8) || p.entry_count>4096 ||
        (p.entry_count && !p.entries)) return false;
    for (std::uint32_t i=0;i<p.entry_count;++i)
        if (!player_valid(p.entries[i]) || (i && p.entries[i-1]->internal_id>=p.entries[i]->internal_id)) return false;
    return true;
}
bool output_alias(const Result* out,const Registry& p,const Services& s,
                  const StackResidues& residues,const std::uint32_t& count) {
    if (overlap(out,sizeof(*out),&p,sizeof(p)) || overlap(out,sizeof(*out),&s,sizeof(s)) ||
        overlap(out,sizeof(*out),s.queries,s.queries?sizeof(*s.queries):0) ||
        overlap(out,sizeof(*out),&count,sizeof(count)) || overlap(out,sizeof(*out),&residues,sizeof(residues)) ||
        overlap(out,sizeof(*out),p.entries,p.entry_count*sizeof(*p.entries)) ||
        overlap(out,sizeof(*out),p.manager_plus_8,sizeof(*p.manager_plus_8))) return true;
    for (std::uint32_t i=0;i<p.entry_count;++i)
        if (overlap(out,sizeof(*out),p.entries[i],sizeof(*p.entries[i]))) return true;
    return false;
}
template<class F,class... A>
bool send(Result& r,Operation op,void* context,F fn,A... args) {
    r.last_operation=op;
    if (!fn) {r.status=Status::service_unavailable;return false;}
    ++r.service_calls;
    try {if (fn(context,args...)==0) return true;} catch (...) {}
    r.status=Status::service_failed;return false;
}
bool missing(Result& r) {r.status=Status::missing_projection;return false;}
bool borrowed_record(const Services& s,Player* p,Record*& record,Result& r,const Result* out) {
    if (!player_valid(p)) return missing(r);
    if (!send(r,Operation::record,s.context,s.record,p,&record)) return false;
    if (!aligned(record) || !record->constructed || !record->at(player_info_record_v1::character_class)->constructed ||
        p->character_level_member!=&record->at(0x310)->header) return missing(r);
    if (overlap(out,sizeof(*out),record,sizeof(*record))) {r.status=Status::invalid_argument;return false;}
    for (const auto& m:record->members) {
        if ((m.bytes.size>0 && overlap(out,sizeof(*out),m.bytes.data,static_cast<std::size_t>(m.bytes.size))) ||
            overlap(out,sizeof(*out),m.text.data(),m.text.size())) {r.status=Status::invalid_argument;return false;}
    }
    return true;
}
bool lookup(const Registry& registry,const Services& s,std::int32_t id,
            std::uint32_t flag,Player*& player,Result& r) {
    if (!s.queries) {r.status=Status::service_unavailable;r.last_operation=Operation::internal_id_player;return false;}
    if (!send(r,Operation::internal_id_player,s.queries->context,s.queries->internal_id_player,
              &registry,id,flag,&player)) return false;
    return player_valid(player)?true:missing(r);
}
bool local(const Services& s,Player* p,std::int32_t& value,Result& r) {
    if (!s.queries) {r.status=Status::service_unavailable;r.last_operation=Operation::is_local;return false;}
    return send(r,Operation::is_local,s.queries->context,s.queries->player_virtual_is_local,p,&value);
}
bool online(const Services& s,std::uint8_t& value,Result& r) {
    if (!s.queries) {r.status=Status::service_unavailable;r.last_operation=Operation::online;return false;}
    return send(r,Operation::online,s.queries->context,s.queries->online,&value);
}
bool fields(const Services& s,std::uintptr_t identity,CharacterFields& f,Result& r,const Result* out) {
    if (!identity) return missing(r);
    if (!send(r,Operation::character_fields,s.context,s.character_fields,identity,&f)) return false;
    if (f.identity!=identity) return missing(r);
    for (auto* p:{f.member_1f88,f.internal_id_1f8c})
        if (overlap(out,sizeof(*out),p,4)) {r.status=Status::invalid_argument;return false;}
    for (auto* p:f.position_145c)
        if (overlap(out,sizeof(*out),p,4)) {r.status=Status::invalid_argument;return false;}
    if (overlap(out,sizeof(*out),f.room_2f4,sizeof(*f.room_2f4)) ||
        overlap(out,sizeof(*out),f.no_room_2ef,1)) {r.status=Status::invalid_argument;return false;}
    return true;
}
bool diagnostic(const Services& s,Assertion reason,Result& r) {
    std::int32_t mode=0;
    if (!send(r,Operation::debug_mode,s.context,s.debug_mode,&mode)) return false;
    if (mode==2) {r.status=Status::source_fault;return false;}
    if (mode==1 && !send(r,Operation::source_assertion,s.context,s.source_assertion,reason)) return false;
    return true;
}
template<std::size_t N>
bool buffer(Record& record,std::uint32_t offset,std::array<std::uint8_t,N>& out,
            Operation op,Result& r) {
    r.last_operation=op;const auto* m=record.at(offset);
    if (!m || !m->constructed || m->kind!=netstruct_members_v1::Kind::byte_array) return missing(r);
    if (!m->bytes.data || m->bytes.size<=0) return true;
    if (static_cast<std::uint32_t>(m->bytes.size)>N) {r.status=Status::unsafe_buffer;return false;}
    // Original GetBuffer ignores the destination-size argument and copies the
    // entire payload. Oversized source payloads are rejected before native UB.
    std::memcpy(out.data(),m->bytes.data,static_cast<std::size_t>(m->bytes.size));
    return true;
}
bool remote_alignment(const Registry& registry,const Services& s,Player* p,
                      std::uintptr_t character,Result& r,const Result* out) {
    std::int32_t host=0;
    if (!send(r,Operation::is_host,s.context,s.is_host,p,&host)) return false;
    if (host) return true;
    Player* hosting=nullptr;
    if (!send(r,Operation::hosting_player,s.context,s.hosting_player,&registry,&hosting)) return false;
    Record* source=nullptr;if (!borrowed_record(s,hosting,source,r,out)) return false;
    const auto other=source->character_660;
    if (!other) return true;
    if (!send(r,Operation::set_position,s.context,s.set_position,character,other,true) ||
        !send(r,Operation::set_rotation,s.context,s.set_rotation,character,other) ||
        !send(r,Operation::set_initial_position,s.context,s.set_initial_position,character,other)) return false;
    CharacterFields destination,origin;
    if (!fields(s,character,destination,r,out) || !fields(s,other,origin,r,out)) return false;
    for (unsigned i=0;i<3;++i) {
        if (!aligned(destination.position_145c[i]) || !aligned(origin.position_145c[i])) return missing(r);
        *destination.position_145c[i]=*origin.position_145c[i];++r.source_writes;
    }
    if (!aligned(origin.room_2f4)) return missing(r);
    std::int32_t added=0;
    if (*origin.room_2f4 && !send(r,Operation::room_add,s.context,s.room_add,*origin.room_2f4,character,&added)) return false;
    if (added) return true;
    if (!send(r,Operation::add_no_room,s.context,s.add_no_room,character)) return false;
    if (!destination.no_room_2ef) return missing(r);
    *destination.no_room_2ef=1;++r.source_writes;
    return send(r,Operation::zone_entered,s.context,s.zone_entered,character);
}
} // namespace
Status Runtime::add_character(std::int32_t id,const StackResidues& residues,Result* out) {
    if (!aligned(out) || overlap(out,sizeof(*out),this,sizeof(*this))) return Status::invalid_argument;
    if (busy_) {out->status=Status::reentrant;return out->status;}
    Result result;
    if (!registry_valid(registry_)) {result.status=Status::invalid_registry;*out=result;return result.status;}
    if (output_alias(out,registry_,services_,residues,character_count_6c4_)) return Status::invalid_argument;
    busy_=true;struct Guard {bool& busy;~Guard(){busy=false;}} guard{busy_};
    auto run=[&]() {
        Player* player=nullptr;
        if (!lookup(registry_,services_,id,0,player,result)) return;
        result.player=player;Record* record=nullptr;
        if (!borrowed_record(services_,player,record,result,out)) return;
        const auto class_value=record->at(player_info_record_v1::character_class)->header.value;
        if (class_value==-1 || record->character_660) {result.skipped=true;return;}
        char name[64];std::snprintf(name,sizeof(name),"PlayerCharacter_%d",id);
        std::uintptr_t handle=0,character=0;
        if (!send(result,Operation::spawn,services_.context,services_.spawn,"Character",name,true,true,&handle) ||
            !send(result,Operation::resolve_character,services_.context,services_.resolve_character,handle,&character)) return;
        result.character=character;
        if (!character) {
            if (!diagnostic(services_,Assertion::spawn_failed,result)) return;
            record->character_660=0;++result.association_writes;
            // The source next dereferences the null Character. Stop at the
            // same reached prefix without invoking a fake native provider.
            result.status=Status::source_fault;return;
        }
        record->character_660=character;++result.association_writes;
        if (!send(result,Operation::initialize_save,services_.context,services_.initialize_save,character)) return;
        std::int32_t is_local=0;
        if (!local(services_,player,is_local,result)) return;
        if (is_local) {
            if (!send(result,Operation::save_slot,services_.context,services_.save_slot,character,
                      static_cast<std::uint32_t>(record->save_slot_664))) return;
        } else if (!send(result,Operation::save_class,services_.context,services_.save_class,character,
                         record->at(player_info_record_v1::character_class)->header.value)) return;
        CharacterFields f;if (!fields(services_,character,f,result,out)) return;
        if (!aligned(f.member_1f88) || !aligned(f.internal_id_1f8c)) {missing(result);return;}
        if (overlap(f.member_1f88,4,f.internal_id_1f8c,4)) {result.status=Status::invalid_argument;return;}
        *f.member_1f88=record->member_674;++result.source_writes;
        *f.internal_id_1f8c=record->internal_id_670;++result.source_writes;
        std::uint8_t is_online=0;
        if (!online(services_,is_online,result)) return;
        if (is_online && !remote_alignment(registry_,services_,player,character,result,out)) return;
        if (!send(result,Operation::init_all,services_.context,services_.init_all,character) ||
            !local(services_,player,is_local,result)) return;
        if (is_local) {
            std::int32_t active=0;
            if (!send(result,Operation::is_active,services_.context,services_.is_active,player,&active)) return;
            if (active && !send(result,Operation::init_camera,services_.context,services_.init_camera,character)) return;
        }
        if (!send(result,Operation::set_idle,services_.context,services_.set_idle,character,false)) return;
        auto slots=residues.skill_slots;
        if (!buffer(*record,0x3d8,slots,Operation::get_slots,result)) return;
        for (std::int32_t i=0;i<3;++i) {
            const auto raw=static_cast<std::int8_t>(slots[static_cast<unsigned>(i)]);
            const auto skill=raw<-1?-1:static_cast<std::int32_t>(raw);
            if (!send(result,Operation::save_skill_slot,services_.context,services_.save_skill_slot,
                      character,i,static_cast<std::uint32_t>(skill))) return;
            ++result.skill_slot_calls;
        }
        bool present=false;std::uint32_t count=0;
        if (!send(result,Operation::save_skill_count,services_.context,services_.save_skill_count,character,&present,&count)) return;
        if (present && count>30 && !diagnostic(services_,Assertion::too_many_skills,result)) return;
        auto levels=residues.skill_levels;
        if (!buffer(*record,0x400,levels,Operation::get_levels,result)) return;
        for (std::uint32_t i=0;i<30;++i) {
            if (!send(result,Operation::save_skill_count,services_.context,services_.save_skill_count,character,&present,&count)) return;
            const auto value=static_cast<std::int8_t>(levels[i]);
            if (present && count>i && value>=0) {
                if (!send(result,Operation::save_skill_level,services_.context,services_.save_skill_level,character,i,
                          static_cast<std::int32_t>(value))) return;
                ++result.skill_level_calls;
            }
        }
        if (!local(services_,player,is_local,result)) return;
        std::uint8_t state=1;
        if (!is_local) {
            auto* flag=record->at(0x4c8);
            if (!flag || !flag->constructed || flag->kind!=netstruct_members_v1::Kind::boolean) {missing(result);return;}
            state=flag->header.reserved[0];
        }
        if (!send(result,Operation::set_state,services_.context,services_.set_state,character,state)) return;
        std::uintptr_t level=0;
        if (!send(result,Operation::current_level,services_.context,services_.current_level,&level)) return;
        if (level && !send(result,Operation::quick_save,services_.context,services_.quick_save,level,false)) return;
        ++character_count_6c4_;++result.count_writes;
        if (!online(services_,is_online,result)) return;
        if (is_online) {
            Player* fresh=nullptr;Record* now=nullptr;
            if (!lookup(registry_,services_,id,1,fresh,result) || !borrowed_record(services_,fresh,now,result,out)) return;
            if (now->character_660!=character) {
                if (!send(result,Operation::remove_character,services_.context,services_.remove_character,&registry_,character)) return;
                result.removed_online_mismatch=true;return;
            }
        }
        if (!send(result,Operation::attach_controller,services_.context,services_.attach_controller,&registry_,id) ||
            !local(services_,player,is_local,result)) return;
        if (is_local) send(result,Operation::attach_light,services_.context,services_.attach_light,&registry_,id);
    };
    run();if (result.status!=Status::invalid_argument) *out=result;return result.status;
}
} // namespace dh2::player_add_character_v1
