#include "player_initial_skill_grants_v1.hpp"

#include <cstring>

namespace dh2::player_initial_skill_grants_v1 {
namespace {
namespace progression = player_skill_progression_v1;
namespace slots = player_saved_skill_slots_v1;

struct Range { std::uintptr_t begin, end; };
template<class T> bool range(const T* pointer, Range& out) {
    const auto at = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || at % alignof(T) || at > UINTPTR_MAX - sizeof(T)) return false;
    out = {at, at + sizeof(T)}; return true;
}
bool overlap(Range a, Range b) { return a.begin < b.end && b.begin < a.end; }
bool storage(const void* pointer,std::size_t count,std::size_t size,std::size_t alignment,Range& out) {
    const auto at=reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || at%alignment || count>SIZE_MAX/size || at>UINTPTR_MAX-count*size) return false;
    out={at,at+count*size}; return true;
}
bool valid(const Bindings* bindings, const Result* out) {
    Range controls[2]{};
    if (!range(bindings, controls[0]) || !range(out, controls[1]) ||
        overlap(controls[0], controls[1])) return false;
    Range borrowed[6]{};
    if (!bindings->character || !range(bindings->current_savegame, borrowed[0]) ||
        !range(bindings->rules, borrowed[1]) || !range(bindings->properties, borrowed[2]) ||
        !range(bindings->view, borrowed[3]) || !range(bindings->tables, borrowed[4]) ||
        !range(bindings->inventory, borrowed[5])) return false;
    for (unsigned i=0; i<6; ++i) {
        for (const auto control : controls) if (overlap(borrowed[i], control)) return false;
        for (unsigned j=0; j<i; ++j) if (overlap(borrowed[i], borrowed[j])) return false;
    }
    if (*bindings->current_savegame) {
        Range saved{};
        if (!range(*bindings->current_savegame, saved)) return false;
        for (const auto control : controls) if (overlap(saved, control)) return false;
        for (const auto field : borrowed) if (overlap(saved, field)) return false;
    }
    const auto& view=*bindings->view;
    if (view.group_count>10000) return false;
    if (view.group_count) {
        Range groups{};
        if (!storage(view.groups,view.group_count,sizeof(data::PropertyBuffGroup),alignof(data::PropertyBuffGroup),groups)) return false;
        for (const auto control:controls) if (overlap(groups,control)) return false;
        std::uint32_t total=0;
        for (std::uint32_t i=0;i<view.group_count;++i) {
            const auto& group=view.groups[i]; Range sheets{};
            if (group.count>10000 || total>100000-group.count) return false;
            total+=group.count;
            if (!group.count) continue;
            if (!storage(group.sheets,group.count,sizeof(group.sheets[0]),alignof(decltype(group.sheets[0])),sheets)) return false;
            for (const auto control:controls) if (overlap(sheets,control)) return false;
            for (std::uint32_t j=0;j<group.count;++j) {
                Range sheet{};
                if (!storage(group.sheets[j],224,sizeof(std::int32_t),alignof(std::int32_t),sheet)) return false;
                for (const auto control:controls) if (overlap(sheet,control)) return false;
            }
        }
    }
    return true;
}
std::int32_t fixed(std::int32_t value) {
    const auto bits = std::uint32_t(value) << 8;
    std::int32_t result{}; std::memcpy(&result, &bits, sizeof(result)); return result;
}

struct Adapter {
    Bindings bindings;
    Result& out;
    std::string& error;
    progression::Services source{this,current_savegame,invoke};

    bool coherent() {
        const auto& view=*bindings.view;
        if (view.defaults!=bindings.rules->defaults.data() || view.types!=bindings.rules->types.data() ||
            view.base!=bindings.properties->base.data() || view.saved!=bindings.properties->saved.data() ||
            view.gear!=bindings.properties->gear.data() || view.resolved!=bindings.properties->resolved.data() ||
            dh2_property_validate(&view)) {
            out.dependency_status=Status::invalid_source_fact;
            error="initial skill live property view mismatch"; return false;
        }
        if (bindings.inventory->character()!=bindings.character ||
            bindings.inventory->properties()!=bindings.properties) {
            out.dependency_status=Status::invalid_source_fact;
            error="initial skill inventory/property owner mismatch"; return false;
        }
        const auto* save=*bindings.current_savegame;
        if (save && save->character()!=bindings.character) {
            out.dependency_status=Status::invalid_source_fact;
            error="initial skill save owner mismatch"; return false;
        }
        return true;
    }
    static int current_savegame(void* raw,std::uintptr_t character,data::PlayerSavegameV1** save) {
        auto& self=*static_cast<Adapter*>(raw);
        if (!save || character!=self.bindings.character || !self.coherent()) return -1;
        *save=*self.bindings.current_savegame; return 0;
    }
    int effect(const progression::Request* request,progression::Response* response) {
        if (!bindings.effects.invoke) {
            out.dependency_status=Status::missing_service;
            error="initial skill source effect unavailable: "+std::to_string(unsigned(request->operation)); return -1;
        }
        ++out.effect_calls;
        try {
            if (!bindings.effects.invoke(bindings.effects.context,request,response)) return 0;
        } catch (...) {}
        out.dependency_status=Status::service_failed;
        error="initial skill source effect failed: "+std::to_string(unsigned(request->operation)); return -1;
    }
    static int invoke(void* raw,const progression::Request* request,progression::Response* response) {
        auto& self=*static_cast<Adapter*>(raw);
        if (!request || !response || request->character!=self.bindings.character || !self.coherent()) return -1;
        *response={}; auto* save=*self.bindings.current_savegame;
        switch (request->operation) {
        case progression::Operation::has_skill_slots:
            if (!save) return 0;
            {
                slots::BoundSlotsV1 slot(*save,*self.bindings.inventory);
                const auto result=slot.has_skill_slots();
                if (result.status!=slots::Status::ok) return -1;
                response->word=std::uint32_t(result.value); return 0;
            }
        case progression::Operation::set_skill_in_slot:
            ++self.out.slot_assignments_attempted;
            if (!save) return 0; // Character SG_SetSkillInSlot null-save guard.
            {
                if (!self.bindings.slot_updates.update_skills) {
                    self.out.dependency_status=Status::missing_service;
                    self.error="initial skill AI_UpdateSkills service unavailable"; return -1;
                }
                slots::BoundSlotsV1 slot(*save,*self.bindings.inventory);
                if (slot.set_skill_in_slot(request->arguments[0],std::uint32_t(request->arguments[1]),
                                           self.bindings.slot_updates,self.error)==slots::Status::ok) return 0;
                self.out.dependency_status=Status::service_failed; return -1;
            }
        case progression::Operation::swap_equipment_set:
            self.bindings.inventory->swap_equipment(); ++self.out.equipment_swaps; return 0;
        case progression::Operation::increment_skill:
            ++self.out.increment_attempts;
            {
            const auto status=progression::increment_skill(self.bindings.view,self.bindings.tables,
                self.bindings.character,std::uint32_t(request->arguments[0]),request->arguments[1]!=0,
                &self.source,&self.out.increment);
            if (self.out.dependency_status==Status::complete) self.out.dependency_status=status;
            response->word=self.out.increment.source_return;
            return status==Status::complete?0:-1;
            }
        case progression::Operation::unlocked_difficulty:
            if (!save) {self.out.dependency_status=Status::invalid_source_fact;return -1;}
            response->word=std::uint32_t(save->unlocked_difficulty()); return 0;
        case progression::Operation::add_property:
            // Character::AddPropInt retains the source fixed-point conversion.
            if (dh2_property_add(self.bindings.view,request->arguments[0],fixed(request->arguments[1]))) {
                self.out.dependency_status=Status::invalid_source_fact;return -1;
            }
            ++self.out.property_adds; return 0;
        case progression::Operation::set_potion_capacity:
            self.bindings.inventory->project_potion_capacity(std::int8_t(std::uint8_t(request->arguments[0])));
            ++self.out.potion_stores; return 0;
        case progression::Operation::recalculate_properties:
            if (!self.bindings.full_recalculation.invoke) {
                self.out.dependency_status=Status::missing_service;
                self.error="initial skill full PROPS_Recalc service unavailable"; return -1;
            }
            ++self.out.effect_calls;
            try {
                if (!self.bindings.full_recalculation.invoke(self.bindings.full_recalculation.context,
                        self.bindings.view,request->arguments[0]!=0,self.error)) {
                    if (!self.coherent()) return -1;
                    return 0;
                }
            } catch (...) {}
            self.out.dependency_status=Status::service_failed;
            if (self.error.empty()) self.error="initial skill full PROPS_Recalc service failed";
            return -1;
        default: return self.effect(request,response);
        }
    }
};
} // namespace

Status initialize(const Bindings* bindings,Result* out,std::string& error) noexcept {
    if (!valid(bindings,out)) return Status::invalid_argument;
    *out={}; error.clear();
    try {
        Adapter adapter{*bindings,*out,error};
        if (!adapter.coherent()) return out->dependency_status;
        const auto status=progression::initialize_skill_slots(bindings->character,&adapter.source,&out->slots);
        if (out->dependency_status!=Status::complete) return out->dependency_status;
        return status;
    } catch (...) {
        out->dependency_status=Status::service_failed; return out->dependency_status;
    }
}
} // namespace dh2::player_initial_skill_grants_v1
