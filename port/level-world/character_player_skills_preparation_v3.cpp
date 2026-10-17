#include "character_player_skills_preparation_v3.hpp"
#include <algorithm>
#include <cstring>
#include <limits>
#include <map>
#include <stdexcept>
#include <utility>

namespace dh2::character_player_skills_preparation_v3 {
namespace {
struct Range {std::uintptr_t begin,end;};
bool range(const void* p,std::size_t n,std::size_t alignment,Range& r) {
    auto a=reinterpret_cast<std::uintptr_t>(p);
    if(!p || a%alignment || a>std::numeric_limits<std::uintptr_t>::max()-n)return false;
    r={a,a+n};return true;
}
bool overlap(Range a,Range b){return a.begin<b.end && b.begin<a.end;}
bool valid_properties(const data::PropertyView* p) {
    Range r;if(!range(p,sizeof(*p),alignof(data::PropertyView),r))return false;
    const std::int32_t* sheets[]={p->defaults,p->types,p->base,p->saved,p->gear,p->resolved};
    for(auto* words:sheets)
        if(!range(words,224*sizeof(std::int32_t),alignof(std::int32_t),r))return false;
    if(p->group_count>10000 || (p->group_count &&
       !range(p->groups,p->group_count*sizeof(data::PropertyBuffGroup),alignof(data::PropertyBuffGroup),r)))return false;
    for(std::uint32_t i=0;i<p->group_count;++i) {
        const auto& g=p->groups[i];if(g.count>10000 || (g.count &&
           !range(g.sheets,g.count*sizeof(std::int32_t*),alignof(std::int32_t*),r)))return false;
        for(std::uint32_t j=0;j<g.count;++j)
            if(!range(g.sheets[j],224*sizeof(std::int32_t),alignof(std::int32_t),r))return false;
    }
    return dh2_property_validate(p)==0;
}
}
struct Owner::Impl {
    struct Instance {constructor::State state{};Arguments arguments;};
    std::shared_ptr<const player_skill_tables_adapter::Tables> tables;
    Inputs inputs;Services native;
    source::State current{};
    std::vector<std::uintptr_t> skill_slots,faery_slots;
    std::map<std::uintptr_t,std::unique_ptr<Instance>> instances;
    std::map<std::uintptr_t,std::unique_ptr<Arguments>> temporaries;
    std::map<std::uintptr_t,std::unique_ptr<std::string>> paths;
    constructor::Globals ctor_globals{};
    constructor::Services ctor_services{};
    character_faery_selection::Globals faery_globals{};
    source::Services services{};
    bool busy=false;
    bool prepared=false;
    std::uint32_t active_timer_leases=0;

    Impl(std::shared_ptr<const player_skill_tables_adapter::Tables> t,Inputs i,Services s)
        :tables(std::move(t)),inputs(i),native(std::move(s)) {
        current.ai=i.active_ais;current.owner=i.character;current.assert_level=i.assert_level;
        current.skills.identity=reinterpret_cast<std::uintptr_t>(&skill_slots);
        current.faeries.identity=reinterpret_cast<std::uintptr_t>(&faery_slots);
        faery_globals={&tables->source_faeries(),i.assert_level};
        current.faery_binding={{i.character,0},&faery_globals,&native.faery,&tables->faery_names()};
        ctor_globals.assert_level=i.assert_level;
        ctor_services={this,construct,nullptr};
        services={this,invoke,&ctor_services,&ctor_globals};
    }
    std::vector<std::uintptr_t>& vector(source::List l) {
        if(l==source::List::skill)return skill_slots;
        if(l==source::List::faery)return faery_slots;
        throw std::invalid_argument("source vector list");
    }
    const std::vector<std::uintptr_t>& vector(source::List l) const {
        if(l==source::List::skill)return skill_slots;
        if(l==source::List::faery)return faery_slots;
        throw std::invalid_argument("source vector list");
    }
    bool valid_vectors() const noexcept {
        for(auto list:{source::List::skill,source::List::faery}) {
            const auto& v=vector(list);
            const auto& projected=list==source::List::skill?current.skills:current.faeries;
            const auto* begin=v.capacity()?v.data():nullptr;
            if(projected.identity!=reinterpret_cast<std::uintptr_t>(&v) || projected.begin!=begin ||
               projected.end!=(begin?begin+v.size():nullptr) ||
               projected.capacity!=(begin?begin+v.capacity():nullptr))return false;
        }
        return true;
    }
    void sync(source::List l) {
        auto& v=vector(l);auto& out=l==source::List::skill?current.skills:current.faeries;
        if(v.capacity()) {out.begin=v.data();out.end=v.data()+v.size();out.capacity=v.data()+v.capacity();}
        else out.begin=out.end=out.capacity=nullptr;
    }
    Arguments* args(std::uintptr_t id) {
        auto it=temporaries.find(id);if(it!=temporaries.end())return it->second.get();
        for(auto& pair:instances)if(pair.second->arguments.identity==id)return &pair.second->arguments;
        return nullptr;
    }
    std::int32_t raw(source::List l,std::uintptr_t receiver) {
        if(receiver!=inputs.character || current.owner!=inputs.character ||
           !valid_properties(inputs.properties))
            throw std::invalid_argument("actor property identity changed");
        return inputs.properties->resolved[l==source::List::skill?28:29];
    }
    std::string script(std::uintptr_t id)const {
        // Accept only retained real table string identities, never dereference
        // an arbitrary callback-supplied integer as a native pointer.
        for(const auto& row:tables->skills().skills)
            if(id==reinterpret_cast<std::uintptr_t>(row.script.c_str()))return std::string(row.script.c_str());
        for(const auto& row:tables->faeries().faeries)
            if(id==reinterpret_cast<std::uintptr_t>(row.spell_script.c_str()))return std::string(row.spell_script.c_str());
        throw std::invalid_argument("unowned source script identity");
    }
    std::int32_t external(const source::Request& request,const Arguments* arguments,source::Response* response) {
        if(!native.invoke)return -1;
        return native.invoke(native.context,&current,&request,arguments,response);
    }
    static std::int32_t invoke(void* context,source::State* state,const source::Request* request,source::Response* response) {
        auto& self=*static_cast<Impl*>(context);
        if(state!=&self.current || !request || !response)return -1;
        try{return self.invoke(*request,response);}catch(...){return -1;}
    }
    std::int32_t invoke(const source::Request& q,source::Response* out) {
        using Op=source::Operation;
        switch(q.operation) {
        case Op::get_skill_list: {
            auto* list=tables->skill_list(raw(source::List::skill,q.receiver));
            out->count=static_cast<std::uint32_t>(list->members.size());return 0;
        }
        case Op::get_skill: {
            auto* row=tables->skill(raw(source::List::skill,q.receiver),q.slot);
            if(!row)return -1;
            out->skill={static_cast<std::uintptr_t>(row->script_length),reinterpret_cast<std::uintptr_t>(row->script.c_str())};return 0;
        }
        case Op::get_faery_list:
            out->count=static_cast<std::uint32_t>(tables->faery_list(raw(source::List::faery,q.receiver))->members.size());return 0;
        case Op::get_faery_list_id:
            out->word=tables->faery_list_id(raw(source::List::faery,q.receiver));return 0;
        case Op::reserve: {
            auto& v=vector(q.list);auto& projected=q.list==source::List::skill?current.skills:current.faeries;
            if(q.receiver!=projected.identity || q.integer>4096 || !v.empty())return -1;
            v.reserve(q.integer);sync(q.list);return 0;
        }
        case Op::append_skill_script: {
            auto& v=vector(q.list);auto& projected=q.list==source::List::skill?current.skills:current.faeries;
            if(q.receiver!=projected.identity || v.size()>=4096 ||
               (q.script_name && !instances.count(q.script_name)))return -1;
            v.push_back(q.script_name);sync(q.list);return 0;
        }
        case Op::arguments_construct: {
            auto a=std::make_unique<Arguments>();a->identity=reinterpret_cast<std::uintptr_t>(a.get());
            out->identity=a->identity;temporaries.emplace(a->identity,std::move(a));return 0;
        }
        case Op::arguments_push_string:
        case Op::arguments_push_integer:
        case Op::arguments_set_string:
        case Op::arguments_set_number: {
            auto* a=args(q.arguments);if(!a)return -1;
            if(q.operation==Op::arguments_push_string) {
                if(!q.text || q.text_size>1048576)return -1;
                a->values.push_back({Value::Type::string,std::string(q.text,q.text_size),0});
            } else if(q.operation==Op::arguments_push_integer)
                a->values.push_back({Value::Type::integer,{},q.integer});
            else {
                if(a->values.size()!=2)return -1;
                if(q.operation==Op::arguments_set_string)a->values[0]={Value::Type::string,script(q.script_name),0};
                else a->values[1]={Value::Type::number,{},q.number_bits};
            }
            return 0;
        }
        case Op::arguments_destroy:
            return temporaries.erase(q.arguments)==1?0:-1;
        case Op::allocate_skill_script: {
            if(q.allocation_bytes!=0x1c || q.allocation_hint)return -1;
            auto instance=std::make_unique<Instance>();
            instance->state.identity=reinterpret_cast<std::uintptr_t>(&instance->state);
            out->identity=instance->state.identity;instances.emplace(out->identity,std::move(instance));return 0;
        }
        case Op::release_skill_script_allocation:
            if(std::find(skill_slots.begin(),skill_slots.end(),q.script_name)!=skill_slots.end() ||
               std::find(faery_slots.begin(),faery_slots.end(),q.script_name)!=faery_slots.end())return -1;
            return instances.erase(q.script_name)==1?0:-1;
        case Op::capture_script_path: {
            source::Response actual{};auto result=external(q,nullptr,&actual);if(result)return result;
            if(!actual.path || actual.path_size>1048576)return -1;
            auto p=std::make_unique<std::string>(actual.path,actual.path_size);
            out->identity=reinterpret_cast<std::uintptr_t>(p.get());out->path=p->c_str();out->path_size=p->size();
            paths.emplace(out->identity,std::move(p));return 0;
        }
        case Op::release_script_path:
            return paths.erase(q.receiver)==1?0:-1;
        case Op::call_script: {
            auto* a=q.arguments?args(q.arguments):nullptr;if(q.arguments && !a)return -1;
            return external(q,a,out);
        }
        case Op::debug_load:case Op::debug_get_switch:case Op::set_script_path:
        case Op::load_script:case Op::init_vcb:
            return external(q,nullptr,out);
        }
        return -1;
    }
    static std::int32_t construct(void* context,constructor::State* state,const constructor::Request* q) {
        auto& self=*static_cast<Impl*>(context);
        try {
            if(!state || !q)return -1;
            auto found=self.instances.find(state->identity);if(found==self.instances.end())return -1;
            auto& instance=*found->second;instance.state=*state;
            if(q->arguments_identity!=state->identity+0x0c)return -1;
            auto& a=instance.arguments;
            switch(q->operation) {
            case constructor::Operation::arguments_construct:a.identity=q->arguments_identity;a.values.clear();return 0;
            case constructor::Operation::arguments_push_string:
                a.values.push_back({Value::Type::string,self.script(reinterpret_cast<std::uintptr_t>(q->text)),0});return 0;
            case constructor::Operation::arguments_push_integer:
                a.values.push_back({Value::Type::integer,{},q->integer});return 0;
            default:return -1;
            }
        }catch(...){return -1;}
    }
    bool valid_output(source::Result* output)const {
        Range out;if(!range(output,sizeof(*output),alignof(source::Result),out))return false;
        auto overlaps=[&](const void* p,std::size_t n,std::size_t a=1) {
            if(!p || !n)return false;
            Range r;return !range(p,n,a,r) || overlap(r,out);
        };
        if(overlaps(this,sizeof(*this)) || overlaps(inputs.properties,sizeof(*inputs.properties)))return false;
        const auto& p=*inputs.properties;
        const std::int32_t* sheets[]={p.defaults,p.types,p.base,p.saved,p.gear,p.resolved};
        for(auto* words:sheets)
            if(overlaps(words,224*sizeof(std::int32_t),alignof(std::int32_t)))return false;
        if(overlaps(p.groups,p.group_count*sizeof(data::PropertyBuffGroup)))return false;
        for(std::uint32_t i=0;i<p.group_count;++i) {
            const auto& g=p.groups[i];if(overlaps(g.sheets,g.count*sizeof(std::int32_t*)))return false;
            for(std::uint32_t j=0;j<g.count;++j)if(overlaps(g.sheets[j],224*sizeof(std::int32_t)))return false;
        }
        for(const auto* v:{&skill_slots,&faery_slots})
            if(overlaps(v->data(),v->capacity()*sizeof(std::uintptr_t),alignof(std::uintptr_t)))return false;
        for(const auto& pair:instances) {
            if(overlaps(pair.second.get(),sizeof(Instance)))return false;
            const auto& values=pair.second->arguments.values;
            if(overlaps(values.data(),values.capacity()*sizeof(Value)))return false;
            for(const auto& value:values)if(overlaps(value.text.data(),value.text.size()+1))return false;
        }
        // Immutable backing is still forbidden as an output destination.
        for(const auto& row:tables->skills().skills)
            if(overlaps(&row,sizeof(row)) || overlaps(row.script.data(),row.script.size()+1) ||
               overlaps(row.table_name.data(),row.table_name.size()+1) || overlaps(row.skill_icon.data(),row.skill_icon.size()+1) ||
               overlaps(row.display_props.data(),row.display_props.size()*sizeof(std::int32_t)))return false;
        for(const auto& row:tables->faeries().faeries)
            if(overlaps(&row,sizeof(row)) || overlaps(row.spell_script.data(),row.spell_script.size()+1) ||
               overlaps(row.table_name.data(),row.table_name.size()+1))return false;
        if(overlaps(tables.get(),sizeof(*tables)))return false;
        const auto& faeries=tables->source_faeries();const auto& names=tables->faery_names();
        if(overlaps(faeries.list_rows,faeries.list_count*sizeof(character_faery_selection::FaeryListRow)) ||
           overlaps(faeries.faery_rows,faeries.faery_count*sizeof(character_faery_selection::FaeryRow)) ||
           overlaps(names.values,names.count*sizeof(std::uintptr_t)))return false;
        for(const auto* lists:{&tables->skills().skill_lists,&tables->faeries().faery_lists})
            for(const auto& list:*lists)
                if(overlaps(&list,sizeof(list)) || overlaps(list.name.data(),list.name.size()+1) ||
                   overlaps(list.members.data(),list.members.size()*sizeof(std::int32_t)))return false;
        return true;
    }
};
Owner::Owner(std::shared_ptr<Impl> impl):impl_(std::move(impl)){}
Owner::~Owner()=default;
Owner::TimerFieldLease::TimerFieldLease(std::shared_ptr<Impl> impl,std::uintptr_t character) noexcept
    :impl_(std::move(impl)),character_(character){}
Owner::TimerFieldLease::TimerFieldLease(TimerFieldLease&& other) noexcept
    :impl_(std::move(other.impl_)),character_(std::exchange(other.character_,0)){}
Owner::TimerFieldLease& Owner::TimerFieldLease::operator=(TimerFieldLease&& other) noexcept {
    if(this!=&other) {
        release();impl_=std::move(other.impl_);
        character_=std::exchange(other.character_,0);
    }
    return *this;
}
Owner::TimerFieldLease::~TimerFieldLease(){release();}
void Owner::TimerFieldLease::release() noexcept {
    if(impl_&&impl_->active_timer_leases)--impl_->active_timer_leases;
    impl_.reset();character_=0;
}
bool Owner::TimerFieldLease::slot(std::uintptr_t character,source::List list,
                                  std::uint32_t index,TimerFieldSlot& output) const noexcept {
    if(!impl_)return false;
    const auto& impl=*impl_;
    if(!impl.prepared||impl.busy||character!=character_||character!=impl.inputs.character||
       impl.current.owner!=character||!impl.valid_vectors()||
       (list!=source::List::skill&&list!=source::List::faery))return false;
    const auto& slots=impl.vector(list);
    if(index>=slots.size())return false;
    TimerFieldSlot candidate{};candidate.instance=slots[index];
    if(candidate.instance) {
        auto found=impl.instances.find(candidate.instance);
        if(found==impl.instances.end()||!found->second||
           found->second->state.identity!=candidate.instance||
           found->second->state.character!=character||
           found->second->state.dispatch_table!=constructor::DispatchTable::char_ai_skill_script||
           candidate.instance>std::numeric_limits<std::uintptr_t>::max()-0x0cu||
           found->second->arguments.identity!=candidate.instance+0x0cu)return false;
        auto* field=&found->second->state.last_skill_id_18;Range field_range;
        if(!range(field,sizeof(*field),alignof(std::int32_t),field_range))return false;
        candidate.field18=field;
    }
    output=candidate;return true;
}
std::optional<Owner::TimerFieldLease> Owner::lease_timer_fields(std::uintptr_t character) noexcept {
    if(!impl_||impl_->busy||!impl_->prepared||impl_->active_timer_leases==UINT32_MAX||
       character==0||character!=impl_->inputs.character||impl_->current.owner!=character||
       !impl_->valid_vectors())return std::nullopt;
    ++impl_->active_timer_leases;
    return TimerFieldLease(impl_,character);
}
std::unique_ptr<Owner> Owner::create(std::shared_ptr<const player_skill_tables_adapter::Tables> tables,
                                   const Inputs& inputs,const Services& services,std::string& error) {
    error.clear();Range p;
    if(!tables || !inputs.character || !inputs.active_ais ||
       !range(inputs.properties,sizeof(*inputs.properties),alignof(data::PropertyView),p) ||
       !valid_properties(inputs.properties) || !services.invoke || !services.faery.get_constant) {
        error="missing/invalid actor, tables, properties or required providers";return {};
    }
    try{return std::unique_ptr<Owner>(new Owner(std::make_shared<Impl>(std::move(tables),inputs,services)));}
    catch(const std::exception& e){error=e.what();return {};}
}
source::Status Owner::prepare(source::Result* output) {
    if(!impl_ || impl_->busy || impl_->active_timer_leases || !valid_properties(impl_->inputs.properties) ||
       !impl_->valid_output(output))return source::Status::invalid_argument;
    Range out,own;range(output,sizeof(*output),alignof(source::Result),out);
    range(this,sizeof(*this),alignof(Owner),own);
    if(overlap(out,own))return source::Status::invalid_argument;
    if(impl_->current.owner!=impl_->inputs.character)return source::Status::invalid_source_fact;
    if(!impl_->valid_vectors())return source::Status::invalid_source_fact;
    if(impl_->current.faery_binding.globals!=&impl_->faery_globals ||
       impl_->current.faery_binding.services!=&impl_->native.faery ||
       impl_->current.faery_binding.full_width_script_names!=&impl_->tables->faery_names())
        return source::Status::invalid_source_fact;
    impl_->busy=true;
    struct End {bool& busy;~End(){busy=false;}}end{impl_->busy};
    impl_->prepared=false;
    const auto status=source::prepare(&impl_->current,&impl_->services,output);
    impl_->prepared=status==source::Status::complete;
    return status;
}
bool Owner::delete_skill_instance(std::uint32_t index,std::uintptr_t identity,
                                  std::string& error) {
    error.clear();
    if(!impl_||impl_->busy||impl_->active_timer_leases||!impl_->prepared||
       !impl_->valid_vectors()||impl_->current.owner!=impl_->inputs.character||
       !identity||index>=impl_->skill_slots.size()||
       impl_->skill_slots[index]!=identity||
       std::find(impl_->faery_slots.begin(),impl_->faery_slots.end(),identity)!=impl_->faery_slots.end()) {
        error="AI_ReloadSkills delete requires the exact active skill slot and an unleased same-Character owner";
        return false;
    }
    auto found=impl_->instances.find(identity);
    if(found==impl_->instances.end()||!found->second||
       found->second->state.identity!=identity||
       found->second->state.character!=impl_->inputs.character||
       found->second->state.dispatch_table!=constructor::DispatchTable::char_ai_skill_script||
       found->second->arguments.identity!=identity+0x0cu) {
        error="AI_ReloadSkills deleting destructor received an unowned skill instance";
        return false;
    }
    // The source deleting destructor releases the instance and embedded
    // Arguments before AI_ReloadSkills publishes nullptr into the captured
    // slot. Keep vector begin/end/capacity stable across this operation.
    impl_->instances.erase(found);
    impl_->skill_slots[index]=0;
    impl_->sync(source::List::skill);
    return true;
}
bool Owner::reset_skill_end(std::string& error) {
    error.clear();
    if(!impl_||impl_->busy||impl_->active_timer_leases||!impl_->prepared||
       !impl_->valid_vectors()||impl_->current.owner!=impl_->inputs.character||
       std::any_of(impl_->skill_slots.begin(),impl_->skill_slots.end(),
                   [](std::uintptr_t value){return value!=0;})) {
        error="AI_ReloadSkills end reset requires all exact skill instances deleted";
        return false;
    }
    impl_->skill_slots.clear();
    impl_->sync(source::List::skill);
    return true;
}
source::State& Owner::state(){return impl_->current;}
const std::vector<std::uintptr_t>& Owner::slots(source::List list)const {
    if(list==source::List::skill)return impl_->skill_slots;
    if(list==source::List::faery)return impl_->faery_slots;
    throw std::invalid_argument("source vector list");
}
const constructor::State* Owner::instance(std::uintptr_t id)const {
    auto found=impl_->instances.find(id);return found==impl_->instances.end()?nullptr:&found->second->state;
}
const Arguments* Owner::instance_arguments(std::uintptr_t id)const {
    auto found=impl_->instances.find(id);return found==impl_->instances.end()?nullptr:&found->second->arguments;
}
} // namespace dh2::character_player_skills_preparation_v3
