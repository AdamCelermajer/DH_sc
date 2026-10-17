#include "../character_player_skills_preparation_v3.hpp"
#include "../character_skill_cooldown_services.hpp"
extern "C" {
#include "../../pydata-constants/constants.h"
}
#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>
namespace p=dh2::character_player_skills_preparation_v3;
namespace k=p::source;
namespace d=dh2::data;
namespace cooldown=dh2::character_skill_cooldown_services;
using Tables=dh2::player_skill_tables_adapter::Tables;
static_assert(!std::is_copy_constructible<Tables>::value && !std::is_move_constructible<Tables>::value,"stable table owner");
void check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
std::vector<std::uint8_t> read(const std::filesystem::path& path){
    std::ifstream f(path,std::ios::binary);check(bool(f),"cache file absent");
    return {std::istreambuf_iterator<char>(f),{}};
}
d::Bytes bytes(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
void quote(const std::string& text) {
    std::cout<<'"';for(unsigned char c:text) {
        if(c=='"' || c=='\\')std::cout<<'\\'<<char(c);
        else if(c<32){const char hex[]="0123456789abcdef";std::cout<<"\\u00"<<hex[c>>4]<<hex[c&15];}
        else std::cout<<char(c);
    }std::cout<<'"';
}
struct CooldownBridge {p::Owner* owner; p::Owner::TimerFieldLease* lease;std::uintptr_t character;};
std::int32_t cooldown_list_count(void* raw,std::uintptr_t character,std::uint32_t kind,std::uint32_t* output) {
    auto& bridge=*static_cast<CooldownBridge*>(raw);
    if(!bridge.owner||!bridge.lease||character!=bridge.character||!output||kind>1)return 1;
    const auto list=kind==0?k::List::skill:k::List::faery;
    *output=std::uint32_t(bridge.owner->slots(list).size());return 0;
}
std::int32_t cooldown_slot(void* raw,std::uintptr_t character,std::uint32_t kind,
                           std::uint32_t index,cooldown::Slot* output) {
    auto& bridge=*static_cast<CooldownBridge*>(raw);
    if(!bridge.lease||!output||kind>1)return 1;
    p::Owner::TimerFieldSlot source_slot{};
    const auto list=kind==0?k::List::skill:k::List::faery;
    if(!bridge.lease->slot(character,list,index,source_slot))return 1;
    *output={source_slot.instance,source_slot.field18};return 0;
}
struct Event {std::uint32_t op;std::string text;std::uint32_t number=0;int argc=-1;std::uintptr_t receiver=0;};
struct Fixture {
    std::filesystem::path cache;
    std::shared_ptr<const Tables> tables;
    d::PropertyState storage;d::PropertyView properties;
    std::unique_ptr<p::Owner> owner;
    std::string path="data/scripts/ai/";
    std::vector<Event> events;
    std::uint32_t queries=0,assertions=0,loads=0,loaded_bytes=0;
    int fail_after=-1;bool throw_failure=false,load_false=false,reenter=false,mutate_list=false,mutate_owner=false,mutate_ais=false;
    std::uintptr_t character=0x1234567800000001ull,ais=0x1234567800000002ull;
    k::Status nested=k::Status::complete;
    k::Result result{};
    Fixture(std::filesystem::path c,std::shared_ptr<const Tables> t,const d::PropertyRules& rules,
            const d::CharacterTable& characters,const d::ClassTables& classes,const std::string& name)
        :cache(std::move(c)),tables(std::move(t)) {
        auto it=std::find(characters.names.begin(),characters.names.end(),name);
        check(it!=characters.names.end(),"player base row absent");
        d::reset_properties(rules,storage,&characters.rows.at(it-characters.names.begin()));
        std::string error;check(d::recalc_properties_with_class(classes,rules,storage,error),error.c_str());
        properties=d::property_view(rules,storage);
        p::Services services;services.context=this;services.invoke=invoke;services.faery={this,constant,assertion};
        owner=p::Owner::create(tables,{character,ais,&properties,0},services,error);
        check(bool(owner),error.c_str());
    }
    static std::int32_t constant(void* c,dh2::character_faery_selection::Character* ch,
       const dh2::character_faery_selection::Request* q,dh2::character_faery_selection::Response* out) {
        auto& self=*static_cast<Fixture*>(c);check(ch->identity==self.character,"wrong faery actor");
        check(std::string(q->category)=="FaeryTypes" && std::string(q->key)=="COUNT","wrong source constant");
        auto raw=read(self.cache/"data/pydata/faeries_pycst.bin");dh2_pycst_view view{};dh2_pycst_result value{};
        check(!dh2_pycst_open(&view,raw.data(),std::uint32_t(raw.size())),"constant file malformed");
        check(!dh2_pycst_get(&view,q->category,std::uint32_t(std::strlen(q->category)),q->key,
             std::uint32_t(std::strlen(q->key)),&value) && value.found,"COUNT absent");
        out->word=value.value;++self.queries;return 0;
    }
    static std::int32_t assertion(void* c,dh2::character_faery_selection::Character*,
                                 const dh2::character_faery_selection::Request*) {
        ++static_cast<Fixture*>(c)->assertions;return 0;
    }
    static std::int32_t invoke(void* c,k::State* state,const k::Request* q,const p::Arguments* args,k::Response* out) {
        auto& s=*static_cast<Fixture*>(c);using Op=k::Operation;
        std::string text=q->text?std::string(q->text,q->text_size):q->saved_path?
            std::string(q->saved_path,q->saved_path_size):q->script_name?
            std::string(reinterpret_cast<const char*>(q->script_name)):std::string{};
        std::uint32_t number=0;int argc=args?int(args->values.size()):q->operation==Op::call_script?0:-1;
        if(args) {
            check(args->identity==q->arguments && args->values.size()==2,"unowned declaration arguments");
            check(args->values[0].type==p::Value::Type::string && args->values[1].type==p::Value::Type::number,
                  "declaration types differ");
            text=args->values[0].text;number=args->values[1].word;
        }
        s.events.push_back({std::uint32_t(q->operation),text,number,argc,q->receiver});
        const auto count=int(s.events.size());
        if(s.fail_after==count) {if(s.throw_failure)throw std::runtime_error("explicit fixture provider failure");return 1;}
        if(s.reenter && count==1) {
            k::Result untouched;std::memset(&untouched,0xa5,sizeof(untouched));auto saved=untouched;
            s.nested=s.owner->prepare(&untouched);
            check(!std::memcmp(&untouched,&saved,sizeof(saved)),"reentry cleared output");
        }
        if(q->operation==Op::debug_load && count==1 && s.mutate_list)s.storage.resolved[28]=-1;
        if(q->operation==Op::debug_load && count==1 && s.mutate_owner)state->owner^=0x100;
        if(q->operation==Op::debug_load && count==1 && s.mutate_ais)state->ai^=0x100;
        if(q->operation!=Op::debug_load && q->operation!=Op::debug_get_switch)
            check(q->receiver==state->ai,"AIS receiver not freshly read");
        switch(q->operation) {
        case Op::debug_load:break; // Explicit host Debug fixture; not a live persistence proof.
        case Op::debug_get_switch:check(text=="Lua_LoadMemUsage","wrong debug key");out->word=0;break;
        case Op::capture_script_path:out->path=s.path.data();out->path_size=s.path.size();break;
        case Op::set_script_path:s.path=text;break;
        case Op::load_script: {
            auto file=read(s.cache/s.path/(text+".luac"));
            check(!file.empty(),"loaded source script empty");++s.loads;s.loaded_bytes+=std::uint32_t(file.size());
            out->loaded=s.load_false?0:1;break; // Bytes exist; Lua execution is NOT claimed.
        }
        case Op::call_script:check(std::string(q->text,q->text_size)=="DeclareSkill","wrong function");break;
        case Op::init_vcb:check(q->receiver==state->ai,"active AIS stale");break;
        default:throw std::runtime_error("unimplemented external source operation");
        }
        return 0;
    }
    k::Status run(){return owner->prepare(&result);}
    void verify() {
        check(path=="data/scripts/ai/","path not restored");
        for(auto list:{k::List::skill,k::List::faery}) {
            const auto& slots=owner->slots(list);
            for(std::size_t i=0;i<slots.size();++i)if(slots[i]) {
                auto* instance=owner->instance(slots[i]);auto* args=owner->instance_arguments(slots[i]);
                check(instance && instance->identity==slots[i] && instance->character==character,"instance owner lost");
                check(instance->last_skill_id_18==-1 && instance->skill_index_14==(list==k::List::skill?i:0xffffffffu),"constructor fields differ");
                check(args && args->identity==slots[i]+0xc && args->values.size()==2 &&
                      args->values[0].text==instance->script_name && args->values[1].word==instance->skill_index_14,
                      "constructor arguments not retained");
                check(slots[i]>0xffffffffu,"pointer was narrowed");
            }
        }
    }
    void print(const std::string& name,const std::string& scenario) {
        std::cout<<"{\"name\":";quote(name);std::cout<<",\"scenario\":";quote(scenario);
        std::cout<<",\"skill_list\":"<<storage.resolved[28]<<",\"faery_list\":"<<storage.resolved[29];
        std::cout<<",\"events\":[";bool comma=false;for(const auto& e:events) {
            if(comma){std::cout<<',';}comma=true;
            std::cout<<'['<<e.op<<',';quote(e.text);std::cout<<','<<e.number<<','<<e.argc<<']';
        }
        std::cout<<"],\"skills\":[";comma=false;for(auto id:owner->slots(k::List::skill)) {
            if(comma){std::cout<<',';}comma=true;std::cout<<(id?"true":"false");
        }
        std::cout<<"],\"faeries\":[";comma=false;for(auto id:owner->slots(k::List::faery)) {
            if(comma){std::cout<<',';}comma=true;std::cout<<(id?"true":"false");
        }
        std::cout<<"],\"instances\":[";comma=false;for(auto list:{k::List::skill,k::List::faery})
            for(auto id:owner->slots(list))if(id) {
                if(comma){std::cout<<',';}comma=true;auto* instance=owner->instance(id);
                std::cout<<'[';quote(instance->script_name);std::cout<<','<<instance->skill_index_14<<']';
            }
        std::cout<<"],\"source_result\":{\"skills\":"<<result.skill_slots<<",\"faeries\":"<<result.faery_slots
                 <<",\"nulls\":"<<result.null_appends<<",\"allocated\":"<<result.script_allocations
                 <<",\"init_vcb\":"<<result.init_vcb_called<<"},\"constant_queries\":"<<queries<<",\"loaded_bytes\":"<<loaded_bytes<<'}';
    }
};
int main(int argc,char** argv) {try {
    check(argc==2,"pass explicit cache directory");const std::filesystem::path cache=argv[1];const auto py=cache/"data/pydata";
    auto load=[&](const char* stem,const char* suffix){return read(py/(std::string(stem)+suffix+".bin"));};
    auto sr=load("skills","_pyarray"),sn=load("skills","_pyarraynames"),ss=load("skills","_pystructnames");
    auto fr=load("faeries","_pyarray"),fn=load("faeries","_pyarraynames"),fs=load("faeries","_pystructnames");
    d::SkillTables skills;d::FaeryTables faeries;std::string error;
    check(d::load_skill_tables(bytes(sr),bytes(sn),bytes(ss),skills,error),error.c_str());
    check(d::load_faery_tables(bytes(fr),bytes(fn),bytes(fs),faeries,error),error.c_str());
    auto tables=Tables::create(std::move(skills),std::move(faeries),error);check(bool(tables),error.c_str());
    // Decoder inputs and moved-from table containers can die; retained owner
    // spans still resolve the real decoded members/strings.
    sr.clear();sn.clear();ss.clear();fr.clear();fn.clear();fs.clear();skills={};faeries={};
    auto cr=load("character_properties","_pyarray"),cn=load("character_properties","_pyarraynames"),cs=load("character_properties","_pystructnames");
    d::CharacterTable characters;d::PropertyRules rules;
    check(d::load_characters(bytes(cr),bytes(cn),bytes(cs),characters,error) && d::load_property_rules(characters,rules,error),error.c_str());
    auto ar=load("character_classes","_pyarray"),an=load("character_classes","_pyarraynames"),as=load("character_classes","_pystructnames");
    d::ClassTables classes;check(d::load_classes(bytes(ar),bytes(an),bytes(as),classes,error),error.c_str());
    unsigned guards=0,selector_cases=0,timer_field_lease_guards=0;
    std::cout<<"{\"validation\":\"PASS\",\"table_counts\":["<<tables->skills().skill_lists.size()<<','<<tables->skills().skills.size()<<','
             <<tables->faeries().faery_lists.size()<<','<<tables->faeries().faeries.size()<<"],\"selectors\":[";
    bool comma=false;for(int raw:{-2147483647,-2,-1,0,1,2,3,4,35,36,127,2147483647}) {
        if(comma){std::cout<<',';}comma=true;++selector_cases;
        std::cout<<'['<<raw<<','<<tables->skill_list_id(raw)<<','<<tables->faery_list_id(raw)<<']';
    }
    check(tables->skill_list(-1)->name=="DEFAULT" && tables->skill_list(36)->members.empty(),"Skill3 fallback wrong");
    check(tables->faery_list(-1)->name=="DEFAULT" && tables->faery_list(4)->members==std::vector<std::int32_t>({2,4,5,6,3}),"Faery0 fallback wrong");guards+=2;
    for(std::size_t id=0;id<tables->skills().skill_lists.size();++id) {
        const auto* list=tables->skill_list(int(id));check(!tables->skill(int(id),std::uint32_t(list->members.size())),"bad slot accepted");++guards;
        for(std::size_t slot=0;slot<list->members.size();++slot)check(tables->skill(int(id),std::uint32_t(slot)),"actual row reference failed");
    }
    std::cout<<"],\"cases\":[";comma=false;unsigned count=0;
    for(const char* name:{"KnightPlayerBase","MagePlayerBase","RoguePlayerBase"})for(const char* scenario:{"normal","load_false","repeat"}) {
        Fixture fixture(cache,tables,rules,characters,classes,name);fixture.load_false=std::string(scenario)=="load_false";
        check(fixture.run()==k::Status::complete,"actual preparation failed");fixture.verify();
        check(fixture.owner->slots(k::List::skill).size()==tables->skill_list(fixture.storage.resolved[28])->members.size() &&
              fixture.owner->slots(k::List::faery).size()==tables->faery_list(fixture.storage.resolved[29])->members.size(),"actual player slot counts differ");
        if(std::string(scenario)=="repeat") {auto skills_before=fixture.owner->slots(k::List::skill);auto faery_before=fixture.owner->slots(k::List::faery);
            fixture.events.clear();fixture.queries=0;check(fixture.run()==k::Status::complete,"repeat failed");
            check(skills_before==fixture.owner->slots(k::List::skill) && faery_before==fixture.owner->slots(k::List::faery) && !fixture.queries,"repeat rebuilt instances");}
        if(comma){std::cout<<',';}comma=true;fixture.print(name,scenario);++count;
    }
    {
        Fixture f(cache,tables,rules,characters,classes,"KnightPlayerBase");
        check(!f.owner->lease_timer_fields(f.character),"timer lease available before skill preparation");
        ++guards;++timer_field_lease_guards;
        check(f.run()==k::Status::complete,"timer lease source preparation failed");
        auto lease=f.owner->lease_timer_fields(f.character);
        check(bool(lease),"prepared owner did not issue timer lease");++timer_field_lease_guards;
        for(auto list:{k::List::skill,k::List::faery}) {
            const auto& slots=f.owner->slots(list);bool found_live=false;
            for(std::uint32_t i=0;i<slots.size();++i)if(slots[i]) {
                p::Owner::TimerFieldSlot slot{};
                check(lease->slot(f.character,list,i,slot) && slot.instance==slots[i] && slot.field18,
                      "retained source instance did not yield its field18");
                const auto* instance=f.owner->instance(slot.instance);
                const auto* args=f.owner->instance_arguments(slot.instance);
                check(instance && instance->identity==slot.instance && instance->character==f.character &&
                      args && args->identity==slot.instance+0x0cu,
                      "timer lease changed retained instance/Arguments identity");
                check(*slot.field18==-1 && instance->last_skill_id_18==-1,
                      "constructor field18 initial value changed");
                *slot.field18=std::int32_t(0x135+std::uint32_t(list));
                check(f.owner->instance(slot.instance)->last_skill_id_18==*slot.field18,
                      "timer write did not reach the sole retained instance field");
                found_live=true;++guards;timer_field_lease_guards++;break;
            }
            check(found_live,"actual selected player list has no retained source instance");
            ++timer_field_lease_guards;
        }
        CooldownBridge cooldown_bridge{f.owner.get(),&*lease,f.character};
        cooldown::Services cooldown_services{&cooldown_bridge,f.character,
            cooldown_list_count,cooldown_slot,nullptr};
        dh2_script_value skill_args[2]{};skill_args[0].type=3;skill_args[0].number=0;
        skill_args[1].type=3;skill_args[1].number=37;
        std::uint32_t cooldown_result=0xffffffffu;char cooldown_error[64]{};
        check(cooldown::skill(&cooldown_services,skill_args,2,nullptr,0,&cooldown_result,
                              cooldown_error,sizeof(cooldown_error))==0 && !cooldown_result &&
              f.owner->instance(f.owner->slots(k::List::skill)[0])->last_skill_id_18==37,
              "source cooldown service did not mutate retained Skill field18");++timer_field_lease_guards;
        dh2_script_value spell_args[1]{};spell_args[0].type=3;spell_args[0].number=41;
        cooldown_result=0xffffffffu;std::memset(cooldown_error,0,sizeof(cooldown_error));
        check(cooldown::spell(&cooldown_services,spell_args,1,nullptr,0,&cooldown_result,
                              cooldown_error,sizeof(cooldown_error))==0 && !cooldown_result,
              "source cooldown service rejected retained Faery slots");
        for(auto id:f.owner->slots(k::List::faery))if(id)
            check(f.owner->instance(id)->last_skill_id_18==41,
                  "source spell cooldown missed a retained Faery field");
        ++timer_field_lease_guards;
        const auto& skill_slots=f.owner->slots(k::List::skill);
        auto empty=std::find(skill_slots.begin(),skill_slots.end(),std::uintptr_t(0));
        check(empty!=skill_slots.end(),"null skill slot fixture absent");
        p::Owner::TimerFieldSlot empty_slot{0x123456u,nullptr};
        check(lease->slot(f.character,k::List::skill,
                         std::uint32_t(empty-skill_slots.begin()),empty_slot) &&
              !empty_slot.instance && !empty_slot.field18,
              "null source slot did not return an empty field view");++timer_field_lease_guards;

        p::Owner::TimerFieldSlot untouched{0x987654u,nullptr};
        check(!lease->slot(f.character+1,k::List::skill,0,untouched) && untouched.instance==0x987654u &&
              !untouched.field18,"foreign Character lookup wrote/returned a field");++timer_field_lease_guards;
        check(!lease->slot(f.character,k::List::none,0,untouched) && untouched.instance==0x987654u &&
              !untouched.field18,"invalid skill-list kind wrote/returned a field");++timer_field_lease_guards;
        check(!lease->slot(f.character,k::List::skill,std::uint32_t(skill_slots.size()),untouched) &&
              untouched.instance==0x987654u && !untouched.field18,
              "out-of-range skill slot wrote/returned a field");++timer_field_lease_guards;
        check(!f.owner->lease_timer_fields(f.character+1),"foreign Character acquired timer lease");
        ++guards;++timer_field_lease_guards;

        const auto original_owner=f.owner->state().owner;f.owner->state().owner^=0x100u;
        check(!lease->slot(f.character,k::List::skill,0,untouched) && untouched.instance==0x987654u &&
              !untouched.field18 && !f.owner->lease_timer_fields(f.character),
              "mutated source owner retained timer access");
        f.owner->state().owner=original_owner;++guards;++timer_field_lease_guards;

        k::Result blocked;std::memset(&blocked,0xa5,sizeof(blocked));const auto blocked_before=blocked;
        check(f.owner->prepare(&blocked)==k::Status::invalid_argument &&
              !std::memcmp(&blocked,&blocked_before,sizeof(blocked)),
              "prepare invalidated a live timer field lease");++guards;++timer_field_lease_guards;

        auto second=f.owner->lease_timer_fields(f.character);
        check(bool(second),"second scoped timer lease failed");++timer_field_lease_guards;
        p::Owner::TimerFieldLease moved(std::move(*second));second.reset();
        *lease=std::move(moved);
        p::Owner::TimerFieldSlot after_move{};
        check(lease->slot(f.character,k::List::skill,0,after_move) && after_move.field18 &&
              after_move.instance==skill_slots[0],"moved timer lease lost its retained field");
        ++timer_field_lease_guards;
        lease.reset();
        check(f.run()==k::Status::complete,"owner did not recover after timer lease ended");
        ++guards;++timer_field_lease_guards;
    }
    {
        Fixture f(cache,tables,rules,characters,classes,"KnightPlayerBase");
        check(f.run()==k::Status::complete,"lease lifetime preparation failed");
        auto lease=f.owner->lease_timer_fields(f.character);
        check(bool(lease),"lease lifetime borrow unavailable");++timer_field_lease_guards;
        const auto& slots=f.owner->slots(k::List::skill);
        check(!slots.empty()&&slots[0],"lease lifetime retained slot absent");
        p::Owner::TimerFieldSlot slot{};
        check(lease->slot(f.character,k::List::skill,0,slot)&&slot.instance==slots[0]&&slot.field18,
              "lease lifetime field unavailable");
        auto* field=slot.field18;const auto id=slot.instance;
        f.owner.reset();
        p::Owner::TimerFieldSlot after_retire{};
        check(lease->slot(f.character,k::List::skill,0,after_retire)&&
              after_retire.instance==id&&after_retire.field18==field&&*field==-1,
              "lease did not retain original field storage after owner wrapper retirement");
        *field=0x2468;check(*after_retire.field18==0x2468,
              "lease lost mutable field after owner wrapper retirement");
        lease.reset();++timer_field_lease_guards;
    }
    {Fixture f(cache,tables,rules,characters,classes,"KnightPlayerBase");f.reenter=true;check(f.run()==k::Status::complete && f.nested==k::Status::invalid_argument,"busy reentry accepted");++guards;}
    {Fixture f(cache,tables,rules,characters,classes,"KnightPlayerBase");f.mutate_list=true;check(f.run()==k::Status::complete && f.owner->slots(k::List::skill).empty(),"fresh SkillList read lost");++guards;}
    {Fixture f(cache,tables,rules,characters,classes,"KnightPlayerBase");f.mutate_owner=true;check(f.run()==k::Status::service_failed && f.owner->slots(k::List::skill).empty(),"wrong actor reused properties");++guards;}
    {Fixture f(cache,tables,rules,characters,classes,"KnightPlayerBase");f.mutate_ais=true;check(f.run()==k::Status::complete && f.owner->state().ai==(f.ais^0x100),"fresh AIS change lost");++guards;}
    unsigned failure_cases=0;Fixture baseline(cache,tables,rules,characters,classes,"KnightPlayerBase");check(baseline.run()==k::Status::complete,"baseline failed");
    for(std::size_t at=1;at<=baseline.events.size();++at)for(bool throws:{false,true}) {
        Fixture f(cache,tables,rules,characters,classes,"KnightPlayerBase");f.fail_after=int(at);f.throw_failure=throws;
        check(f.run()==k::Status::service_failed,"provider failure accepted");
        check(f.events.size()==at,"provider failure continued external effects");++failure_cases;
        // Any completed appends/instances remain retained; no rollback claim.
        for(auto id:f.owner->slots(k::List::skill))if(id)check(f.owner->instance(id),"failure discarded earlier instances");
    }
    {Fixture f(cache,tables,rules,characters,classes,"KnightPlayerBase");
        auto* alias=reinterpret_cast<k::Result*>(f.storage.resolved.data());auto saved=f.storage.resolved;
        check(f.owner->prepare(alias)==k::Status::invalid_argument && saved==f.storage.resolved,"property output alias accepted");++guards;
        check(f.owner->prepare(reinterpret_cast<k::Result*>(reinterpret_cast<unsigned char*>(&f.result)+1))==k::Status::invalid_argument,"misaligned output accepted");++guards;
        check(f.owner->prepare(nullptr)==k::Status::invalid_argument,"null output accepted");++guards;
        auto old_state=f.owner->state();
        check(f.owner->prepare(reinterpret_cast<k::Result*>(&f.owner->state()))==k::Status::invalid_argument &&
              !std::memcmp(&old_state,&f.owner->state(),sizeof(old_state)),"state output alias accepted");++guards;
        auto* rows=const_cast<dh2::character_faery_selection::FaeryRow*>(tables->source_faeries().faery_rows);auto first=rows[0];
        check(f.owner->prepare(reinterpret_cast<k::Result*>(rows))==k::Status::invalid_argument &&
              !std::memcmp(&first,rows,sizeof(first)),"owned table output alias accepted");++guards;
        f.owner->state().skills.identity^=0x100;check(f.run()==k::Status::invalid_source_fact,"foreign vector identity accepted");++guards;
    }
    {Fixture f(cache,tables,rules,characters,classes,"KnightPlayerBase");f.properties.resolved=reinterpret_cast<std::int32_t*>(
        reinterpret_cast<unsigned char*>(f.storage.resolved.data())+1);auto untouched=f.result;
        check(f.run()==k::Status::invalid_argument && !std::memcmp(&untouched,&f.result,sizeof(untouched)),"misaligned property words accepted");++guards;}
    {auto bad_skills=tables->skills();auto copied_faeries=tables->faeries();bad_skills.skill_lists.resize(3);
        check(!Tables::create(std::move(bad_skills),std::move(copied_faeries),error),"missing Skill3 fallback accepted");++guards;}
    {auto copied_skills=tables->skills();auto bad_faeries=tables->faeries();bad_faeries.faery_lists.clear();
        check(!Tables::create(std::move(copied_skills),std::move(bad_faeries),error),"missing Faery0 fallback accepted");++guards;}
    {auto bad_skills=tables->skills();auto copied_faeries=tables->faeries();++bad_skills.skills[0].script_length;
        check(!Tables::create(std::move(bad_skills),std::move(copied_faeries),error),"mismatched serialized length accepted");++guards;}
    {auto bad_skills=tables->skills();auto copied_faeries=tables->faeries();bad_skills.skill_lists[14].members[0]=-1;
        auto bad=Tables::create(std::move(bad_skills),std::move(copied_faeries),error);check(bool(bad),"invalid row fixture tables rejected early");
        Fixture f(cache,bad,rules,characters,classes,"KnightPlayerBase");check(f.run()==k::Status::service_failed &&
            f.owner->slots(k::List::skill).empty() && f.path=="data/scripts/skills/","invalid reached row accepted or source path rolled back");++guards;}
    {auto copied_skills=tables->skills();auto copied_faeries=tables->faeries();
        auto only=Tables::create(std::move(copied_skills),std::move(copied_faeries),error);check(bool(only),"retention input invalid");
        std::weak_ptr<const Tables> retained=only;
        Fixture f(cache,only,rules,characters,classes,"KnightPlayerBase");only.reset();f.tables.reset();
        check(!retained.expired() && f.run()==k::Status::complete,"Owner did not retain tables");f.verify();
        f.owner.reset();check(retained.expired(),"table retention leaked");++guards;}
    std::cout<<"],\"host_cases\":"<<count<<",\"selector_cases\":"<<selector_cases<<",\"guards\":"<<guards
             <<",\"timer_field_lease_guards\":"<<timer_field_lease_guards
             <<",\"provider_failure_cases\":"<<failure_cases<<",\"lua_executed\":false,\"native_wired\":false}\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
