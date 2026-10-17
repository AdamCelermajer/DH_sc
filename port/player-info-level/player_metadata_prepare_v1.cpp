#include "player_metadata_prepare_v1.hpp"
#include <cstring>
#include <string_view>

namespace dh2::player_metadata_prepare_v1 {
namespace {
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn) {
    if (!a || !b || !an || !bn) return false;
    auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
    if (an>UINTPTR_MAX-x || bn>UINTPTR_MAX-y) return true;
    return x<y+bn && y<x+an;
}
template<class T> bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
template<class F,class... A> bool send(Result& r,Operation op,void* context,F fn,A... args) {
    r.last_operation=op;
    if (!fn) {r.status=Status::service_unavailable;return false;}
    ++r.service_calls;
    try {if (fn(context,args...)==0) return true;} catch (...) {}
    r.status=Status::service_failed;return false;
}
bool save_valid(SaveRef* save,std::uintptr_t identity,Result& r,const Result* out) {
    if (!aligned(save) || !identity || save->identity!=identity || !aligned(save->save)) {
        r.status=Status::missing_projection;return false;
    }
    if (overlap(out,sizeof(*out),save,sizeof(*save)) || overlap(out,sizeof(*out),save->save,sizeof(*save->save)) ||
        overlap(out,sizeof(*out),save->save->name().data(),save->save->name().size())) {
        r.status=Status::invalid_argument;return false;
    }
    return true;
}
bool setter(Result& r,Operation op,netstruct_members_v1::Status status) {
    r.last_operation=op;r.setter_status=status;
    if (status==netstruct_members_v1::Status::complete) return true;
    r.status=Status::setter_failed;return false;
}
} // namespace
Status Runtime::prepare(const SetterResidues& residues,Result* out) {
    if (!aligned(out) || overlap(out,sizeof(*out),this,sizeof(*this)) ||
        overlap(out,sizeof(*out),&record_,sizeof(record_)) ||
        overlap(out,sizeof(*out),&residues,sizeof(residues))) return Status::invalid_argument;
    if (busy_) {out->status=Status::reentrant;return out->status;}
    Result r;
    if (!record_.constructed || !record_.at(0x2b0)->constructed || !record_.at(0x360)->constructed ||
        !record_.at(0x310)->constructed) {r.status=Status::invalid_record;*out=r;return r.status;}
    for (const auto& m:record_.members)
        if (overlap(out,sizeof(*out),m.text.data(),m.text.size()) ||
            (m.bytes.size>0 && overlap(out,sizeof(*out),m.bytes.data,static_cast<std::size_t>(m.bytes.size)))) return Status::invalid_argument;
    busy_=true;struct Guard{bool& busy;~Guard(){busy=false;}}guard{busy_};
    auto run=[&]() {
        r.captured_character=record_.character_660;
        std::int32_t active=0;
        if (!send(r,Operation::is_active,services_.context,services_.is_active,&record_,&active)) return;
        if (!active) {r.disposition=Disposition::inactive;return;}
        if (!record_.local_66c) {r.disposition=Disposition::not_local;return;}
        const auto slot=record_.save_slot_664;
        if (slot==-1) {r.disposition=Disposition::no_slot;return;}
        std::uintptr_t identity=record_.loading_info_680;r.captured_save=identity;
        SaveRef* save=nullptr;
        if (!identity) {
            if (!send(r,Operation::allocate_save,services_.context,services_.allocate_save,
                      std::uint32_t{0x198},std::uint32_t{0},&identity)) return;
            r.captured_save=identity;
            if (!identity) {r.status=Status::missing_projection;return;}
            if (!send(r,Operation::construct_indexed_save,services_.context,services_.construct_indexed_save,
                      identity,static_cast<std::uint32_t>(slot),std::int32_t{1},false,&save) || !save_valid(save,identity,r,out)) return;
            r.last_operation=Operation::publish_680;
            record_.loading_info_680=identity;++r.published_save_writes;
        } else if (!send(r,Operation::save_by_identity,services_.context,services_.save_by_identity,
                         identity,&save) || !save_valid(save,identity,r,out)) return;
        std::uint8_t flag=0;
        if (!send(r,Operation::debug_name,services_.context,services_.debug_name_11,&flag)) return;
        if (flag) {r.status=Status::outside_domain;return;}
        if (!send(r,Operation::game_state_name,services_.context,services_.game_state_name_28,&flag)) return;
        if (flag) {r.status=Status::outside_domain;return;}
        r.last_operation=Operation::compare_name;
        auto* name=record_.at(0x2b0);
        // Source std::string::compare(char const*) and subsequent char-pointer
        // constructor treat Save+2c as a C string, including embedded NULs.
        std::string_view saved_name(save->save->name().c_str());
        if (std::string_view(name->text)!=saved_name) {
            if (!setter(r,Operation::set_name,player_info_record_v1::set_character_name(record_,saved_name))) return;
            ++r.name_setters;
        }
        if (!r.captured_character) {
            // These are fresh reads after the name delivery, from that same
            // captured Save owner; no preview/metadata snapshot is substituted.
            const auto class_value=save->save->class_id();
            if (record_.at(0x360)->header.value!=class_value) {
                if (!setter(r,Operation::set_class,player_info_record_v1::set_character_scalar(
                            record_,0x360,class_value,residues.character_class))) return;
                ++r.class_setters;
            }
            const auto level_value=save->save->level();
            if (record_.at(0x310)->header.value!=level_value) {
                if (!setter(r,Operation::set_level,player_info_record_v1::set_character_scalar(
                            record_,0x310,level_value,residues.character_level))) return;
                ++r.level_setters;
            }
        }
        r.disposition=Disposition::prepared;r.last_operation=Operation::continuation;
    };
    run();if(r.status!=Status::invalid_argument)*out=r;return r.status;
}
} // namespace dh2::player_metadata_prepare_v1
