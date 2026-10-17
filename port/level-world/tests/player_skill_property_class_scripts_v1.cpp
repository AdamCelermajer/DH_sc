// Shared immutable cache readers/name catalogue; its old protocol main is not
// executed. This fixture creates one VM and uses unchanged authored scripts.
#define main player_skill_session_protocol_main
#include "player_skill_session_v1.cpp"
#undef main
#include "../player_skill_property_services_v1.hpp"
#include "../character_player_scalar_services_v1.hpp"
#include "../character_current_equipped_faery_v1.hpp"
#include "../character_equipped_faery_element_v1.hpp"
#include "../character_player_buffs_v1.hpp"
#include "../character_coordinator.hpp"
#include "../../game-data/player_savegame_v1.hpp"
#include "../../game-data/player_faery_change_v1.hpp"

namespace ps=dh2::player_skill_property_services_v1;
namespace bs=dh2::character_player_buffs_v1;
namespace fs=dh2::character_current_spell_v1;
namespace scalar=dh2::character_player_scalar_services_v1;
namespace faery_change=dh2::data::player_faery_change_v1;
namespace skill_updates=dh2::player_skill_update_session_v1;
struct FaeryChangeContext {skill_updates::Runtime* updates;skill_updates::Result* result;};
bool update_all_for_faery_change(void* raw,std::string& error){
    auto& context=*static_cast<FaeryChangeContext*>(raw);
    return context.updates&&context.result&&context.updates->update(*context.result,error)==0;
}
struct SavedSkillUpdateContext {
    skill_updates::Runtime* updates;d::PlayerSavegameV1* save;
    const std::int32_t* difficulty;skill_updates::SelectedFaeryResult* result;
    std::uintptr_t character;
};
bool update_selected_after_slot_write(void* raw,std::uintptr_t character,std::string& error){
    auto& context=*static_cast<SavedSkillUpdateContext*>(raw);
    if(character!=context.character||!context.updates||!context.save||!context.difficulty||!context.result){
        error="saved slot UpdateSkills crossed the fixture Character";return false;
    }
    return context.updates->update_current_faery(*context.save,*context.difficulty,*context.result,error)==0;
}
struct ClassFixture {
    std::filesystem::path cache;Catalogue& cat;d::PropertyState state;d::PropertyView view;
    d::PropertySheet temp{};d::PlayerSavegameV1 save;
    const d::PlayerSavegameV1* saved=&save;std::int32_t difficulty=0,machine=3;
    dh2::native::debug_files::Backend debug;dh2::ais_player_init_vcb::State ais{AIS,0};
    dh2::character::Coordinator coordinator{CHAR,2};
    scalar::IntegerMap integers;scalar::State scalar_state{AIS,&integers};scalar::Bindings scalar_bindings{&scalar_state,nullptr};
    ps::Bindings properties{};
    dh2::character_faery_selection::Globals faery_globals;dh2::character_faery_selection::Services faery_services;
    fs::SavedBindings faery_saved{};fs::Bindings spell{};
    dh2::character_current_equipped_faery_v1::Bindings equipped{};
    dh2::character_equipped_faery_element_v1::Bindings element{};
    std::vector<d::ClassRow> rows;std::unique_ptr<bs::Owner> buffs;bs::CallbackBindings buff_callbacks{};
    std::unique_ptr<s::Session> session;std::unique_ptr<p::Owner> owner;
    std::unique_ptr<dh2::player_skill_update_session_v1::Runtime> updates;
    std::set<std::string> paths;std::vector<int> applied;std::ofstream* captures;
    unsigned capture_count=0;bool fail_apply=false;std::string last_request;
    ClassFixture(const std::filesystem::path& c,const std::filesystem::path& dir,Catalogue& catalogue,const char* name,std::ofstream& capture)
      :cache(c),cat(catalogue),faery_globals{&cat.tables->source_faeries(),0},faery_services{this,constant,nullptr},captures(&capture){
        const auto it=std::find(cat.characters.names.begin(),cat.characters.names.end(),name);check(it!=cat.characters.names.end(),"actual Player row absent");
        d::reset_properties(cat.rules,state,&cat.characters.rows.at(it-cat.characters.names.begin()));
        std::string error;check(d::recalc_properties_with_class(cat.classes,cat.rules,state,error),error.c_str());view=d::property_view(cat.rules,state);
        properties={CHAR,&cat.rules,&cat.classes,&state,&temp,false,&view};
        save.set_character(CHAR);check(save.initialize_skills(cat.tables->skills(),view.resolved[28],error),error.c_str());save.initialize_faeries();
        std::filesystem::create_directories(dir);auto seed=read(cache/"DebugSwitches.savegame");check(debug.initialize(std::filesystem::absolute(dir),seed.data(),seed.size(),error),error.c_str());
        for(const auto& row:cat.classes.rows)rows.push_back({row.data(),std::uint32_t(row.size())});
        buffs=bs::Owner::create({CHAR,&view,{this,buff_service},std::uint32_t(rows.size()),UINT32_MAX});check(bool(buffs),"sole buff owner create failed");buff_callbacks={buffs.get()};
        faery_saved={CHAR,&saved,&difficulty,&state.resolved[29],&faery_globals,&faery_services};
        spell={CHAR,fs::saved_services(&faery_saved)};equipped={CHAR,fs::saved_services(&faery_saved)};
        element={CHAR,dh2::character_equipped_faery_element_v1::saved_services(&faery_saved)};
        s::Configuration config;config.character=CHAR;config.ais=&ais;config.tables=cat.tables;config.debug=&debug.globals();config.debug_services=debug.services();
        config.providers.context=this;config.providers.resolve=resolve;config.providers.native=native;config.providers.faery=faery_services;
        session=s::Session::adopt(s::Vm(dh2_script_vm_create_deferred(8*1024*1024)),std::move(config),error);check(bool(session),error.c_str());
        check(!session->bind_ais_functions(error)&&!session->bind_character_functions(error),error.c_str());
        s::LoadResult common{};check(!session->load_resolved("data/scripts/ai/_commons.luac",&common,error)&&common.source_success,error.c_str());
        owner=p::Owner::create(cat.tables,{CHAR,AIS,&view,0},session->preparation_services(),error);check(bool(owner),error.c_str());
        p::source::Result prepared{};const auto prepared_status=owner->prepare(&prepared);
        check(prepared_status==p::source::Status::complete,session->last_error().c_str());
        check(prepared.script_allocations==13&&prepared.null_appends==8,"authored preparation instance count changed");
        updates=std::make_unique<dh2::player_skill_update_session_v1::Runtime>(*session,*owner,AIS,CHAR,machine);
    }
    ~ClassFixture(){updates.reset();coordinator.stop_timers();session.reset();owner.reset();bs::Result r{};buffs->retire(&r);buffs.reset();}
    static int resolve(void* raw,const std::string& path,s::Resource& out,std::string& error){
        auto& f=*static_cast<ClassFixture*>(raw);if(!std::filesystem::exists(f.cache/path)){error="actual script resource absent";return -1;}
        f.paths.insert(path);out={path,std::make_shared<const std::vector<std::uint8_t>>(read(f.cache/path))};return 0;
    }
    static int constant(void* raw,dh2::character_faery_selection::Character* character,const dh2::character_faery_selection::Request* q,dh2::character_faery_selection::Response* out){
        auto& f=*static_cast<ClassFixture*>(raw);check(character->identity==CHAR,"faery Character changed");dh2_pycst_view v{};dh2_pycst_result r{};
        check(!dh2_pycst_open(&v,f.cat.faery_constants.data(),std::uint32_t(f.cat.faery_constants.size()))&&
              !dh2_pycst_get(&v,q->category,std::uint32_t(std::strlen(q->category)),q->key,std::uint32_t(std::strlen(q->key)),&r)&&r.found,"actual faery constant unavailable");out->word=r.value;return 0;
    }
    static int buff_service(void* raw,d::PropertyView* view,const bs::Request* q,bs::Response* out){
        auto& f=*static_cast<ClassFixture*>(raw);check(view==&f.view&&q->character==CHAR,"second property or Character owner");
        switch(q->operation){
        case bs::Operation::timer_start:out->word=f.coordinator.start_timer(q->duration,q->repeat,q->event,q->subject);return out->word< -1?-1:0;
        case bs::Operation::timer_stop:return f.coordinator.stop_timer(std::uint32_t(q->id))<0?-1:0;
        case bs::Operation::timer_time_left:return dh2_character_timer_time_left(&out->elapsed,&out->duration,&f.coordinator.timers(),std::uint32_t(q->id))==1?0:-1;
        case bs::Operation::apply_class:return int(dh2_class_apply(f.rows.data(),std::uint32_t(f.rows.size()),q->id,q->sheet,view->resolved));
        case bs::Operation::recalculate:return int(dh2_class_recalc_base(f.rows.data(),std::uint32_t(f.rows.size()),f.state.base.data(),view));
        case bs::Operation::fx_release:return q->subject?-1:0;
        default:return -1; // Actual positive FX owner is a separate provider.
        }
    }
    static int native(void* raw,const s::NativeRequest& q,const dh2_script_value* a,std::uint32_t n,dh2_script_value* out,std::uint32_t cap,std::uint32_t* returned,char* error,std::size_t size){
        auto& f=*static_cast<ClassFixture*>(raw);f.last_request=q.name;check(q.character==CHAR,"full Character identity changed");*returned=0;
        if(q.domain==s::Domain::ais){
            using F=dh2::ais_native_bindings::Function;check(q.userdata==AIS,"AIS receiver changed");
            if(q.ais_function==F::trace)return dh2_script_game_trace(nullptr,a,n,out,cap,returned,error,size);
            if(q.ais_function==F::get_int||q.ais_function==F::set_int)return q.ais_function==F::get_int?scalar::get_int(&f.scalar_bindings,a,n,out,cap,returned,error,size):scalar::set_int(&f.scalar_bindings,a,n,out,cap,returned,error,size);
            if(q.ais_function==F::get_py_cst){
                check(n==2,"constant argument count changed");auto category=text(a[0]),key=text(a[1]);dh2_pycst_result r{};
                for(const auto* input:{&f.cat.design,&f.cat.ai_constants}){dh2_pycst_view v{};check(!dh2_pycst_open(&v,input->data(),std::uint32_t(input->size())),"actual constants malformed");check(!dh2_pycst_get(&v,category.data(),std::uint32_t(category.size()),key.data(),std::uint32_t(key.size()),&r),"constant query failed");if(r.found)break;}
                check(r.found,"required actual constant absent");result(out,cap,returned,float(r.value));return 0;
            }
            if(q.ais_function==F::get_py_oid||q.ais_function==F::get_py_struct){
                check(n==2,"name argument count changed");auto category=text(a[0]),key=text(a[1]);dh2_pynames_view v{};bool found=false;
                if(q.ais_function==F::get_py_oid){auto it=f.cat.names.find(category);if(it!=f.cat.names.end()){check(!dh2_pynames_open(&v,it->second.data(),std::uint32_t(it->second.size())),"actual OID names malformed");found=true;}}
                else for(const auto& row:dh2_struct_name_tables)if(category==row.name){check(!dh2_pynames_open(&v,row.bytes,row.size),"actual structure names malformed");found=true;break;}
                check(found,"unbound actual name category");std::int32_t id=-1;check(!dh2_pynames_get(&v,key.data(),std::uint32_t(key.size()),&id),"name lookup failed");result(out,cap,returned,float(id));return 0;
            }
        }else{
            using F=dh2::character_native_bindings::Function;check(q.userdata==CHAR,"Character receiver changed");
            if(q.character_function==F::character_create_buff)return bs::create_buff(&f.buff_callbacks,a,n,out,cap,returned,error,size);
            if(q.character_function==F::character_remove_buff)return bs::remove_buff(&f.buff_callbacks,a,n,out,cap,returned,error,size);
            if(q.character_function==F::character_apply_prop_class&&n>1&&a[1].type==DH2_SCRIPT_IDENTITY)return bs::apply_buff(&f.buff_callbacks,a,n,out,cap,returned,error,size);
            if(q.character_function==F::character_get_prop||q.character_function==F::character_set_prop||q.character_function==F::character_clear_props||q.character_function==F::character_apply_prop_class){
                const bool capture=q.character_function==F::character_apply_prop_class&&n==2&&a[1].type==DH2_SCRIPT_BOOLEAN&&a[1].boolean;
                auto before=f.temp,source=f.state.resolved;
                if(q.character_function==F::character_apply_prop_class){check(n&&a[0].type==DH2_SCRIPT_NUMBER,"class argument changed");f.applied.push_back(int(a[0].number));if(f.fail_apply&&int(a[0].number)==210)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
                const auto status=ps::invoke(&f.properties,q.character,q.character_function,a,n,out,cap,returned,error,size);
                if(capture&&!status){const std::int32_t id=int(a[0].number);f.captures->write(reinterpret_cast<const char*>(&id),4);for(const auto* sheet:{&before,&source,&f.temp})f.captures->write(reinterpret_cast<const char*>(sheet->data()),896);++f.capture_count;}
                return status;
            }
            if(q.character_function==F::character_get_current_skill_info||q.character_function==F::character_get_skill_id_from_oid){
                check(n&&a[0].type==DH2_SCRIPT_NUMBER,"skill info index absent");using namespace dh2::player_saved_skill_callbacks_v1;
                const auto r=q.character_function==F::character_get_current_skill_info?get_current_skill_info(&f.view,&f.cat.tables->skills(),&f.save,CHAR,int(a[0].number)):get_skill_id_from_oid(&f.view,&f.cat.tables->skills(),int(a[0].number));
                if(r.disposition==Disposition::no_return)return 0;
                check(r.disposition==Disposition::append_integer,"actual saved skill query failed");result(out,cap,returned,float(r.integer));return 0;
            }
            if(q.character_function==F::character_get_current_spell_info)return fs::current_spell_info_v1(&f.spell,a,n,out,cap,returned,error,size);
            if(q.character_function==F::character_get_equipped_faery_element)return dh2::character_equipped_faery_element_v1::equipped_faery_element_v1(&f.element,a,n,out,cap,returned,error,size);
            if(q.character_function==F::character_get_current_equipped_faery_id)return dh2::character_current_equipped_faery_v1::current_equipped_faery_id_v1(&f.equipped,a,n,out,cap,returned,error,size);
            if(q.character_function==F::character_get_current_equipped_faery_level)return dh2::character_current_equipped_faery_v1::current_equipped_faery_level_v1(&f.equipped,a,n,out,cap,returned,error,size);
        }
        if(error&&size)std::snprintf(error,size,"Unbound actual class-script provider: %s",q.name);
        return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
    }
};
int main(int argc,char** argv){try{
    check(argc==3,"cache and private test output required");Catalogue cat(argv[1]);std::filesystem::path output=argv[2];std::filesystem::create_directories(output);
    std::ofstream captures(output/"temporary-class-calls.bin",std::ios::binary);check(bool(captures),"private original comparison output unavailable");
    unsigned callbacks=0,cases=0,captured=0,failures=0,common_calls=0,selected_faery_updates=0,faery_change_callbacks=0;std::set<std::string> resources;
    for(const char* name:{"KnightPlayerBase","RoguePlayerBase","MagePlayerBase"}){
        ClassFixture f(argv[1],output/name,cat,name,captures);auto* vm=f.session->vm();std::string error;dh2::player_skill_update_session_v1::Result r{};
        auto update=[&](){const auto status=f.updates->update(r,error);if(status||r.callbacks!=13||r.lua_errors)throw std::runtime_error(std::string(name)+" actual update: "+error+" | "+f.last_request);callbacks+=r.callbacks;};
        update();
        check(f.session->vm()==vm&&f.properties.owner_view==&f.view&&f.view.group_count==1&&f.buffs->count()==1,"startup replaced VM/properties/buff owner");
        std::uint32_t celest_slot=0;bool celest_found=false;
        for(const auto id:f.owner->slots(p::source::List::faery)){
            const auto* script=f.owner->instance(id);
            if(script&&script->script_name==std::string("faerie_celest")){celest_found=true;break;}
            ++celest_slot;
        }
        check(celest_found&&celest_slot<5,"actual Celest source Faery slot missing");
        std::string selected_error;
        check(f.save.set_current_faery(celest_slot,0,selected_error),selected_error.c_str());
        dh2::player_skill_update_session_v1::SelectedFaeryResult selected{};
        SavedSkillUpdateContext saved_update{f.updates.get(),&f.save,&f.difficulty,&selected,CHAR};
        const d::SavedSkillUpdateServicesV1 saved_services{&saved_update,update_selected_after_slot_write};
        if(!(f.save.set_skill_in_slot(0,0,saved_services,selected_error)&&
              selected.status==dh2::character_ai_update_skills::Status::complete&&
              selected.source.decision==dh2::character_ai_update_skills::Decision::updated&&
              selected.source.faery_index==celest_slot&&selected.source.saved_slots==1&&selected.source.faery_updated==1&&
              selected.source.script_updates==2&&selected.callbacks==2&&f.save.skill_in_slot(0)==0&&
              !selected.lua_errors&&f.session->vm()==vm))
            throw std::runtime_error(std::string("actual selected Celest UpdateSkills path failed: ")+selected_error+" | "+f.last_request);
        ++selected_faery_updates;
        const auto next_faery=std::uint32_t((celest_slot+1)%5);
        skill_updates::Result changed_update{};FaeryChangeContext change_context{f.updates.get(),&changed_update};
        faery_change::Result changed{};std::string change_error;
        const faery_change::Services change_services{&change_context,update_all_for_faery_change};
        check(faery_change::change(&f.save,0,next_faery,change_services,&changed,change_error)==faery_change::Status::complete,
              change_error.c_str());
        check(changed.save_changed&&changed.skills_updated&&f.save.current_faery(0)==std::int32_t(next_faery)&&
              changed_update.callbacks==13&&changed_update.source.decision==dh2::character_ai_update_all_skills::Decision::completed&&
              f.session->vm()==vm,"actual ChangeFaery did not save then UpdateAllSkills in the same VM");
        faery_change_callbacks+=changed_update.callbacks;callbacks+=changed_update.callbacks;
        if(std::string(name)=="RoguePlayerBase")check(std::find(f.applied.begin(),f.applied.end(),210)!=f.applied.end(),"Rogue did not execute actual failing Roundhouse row210");
        update();
        update();
        // Execute the actual common helper with positive preview levels. A
        // positive whole passive update can require real target acquisition,
        // which belongs to a separate native activation milestone.
        const std::string prefix=std::string(name)=="KnightPlayerBase"?"Skill_Warrior_":std::string(name)=="RoguePlayerBase"?"Skill_Rogue_":"Skill_Mage_";
        for(std::size_t id=0;id<cat.classes.names.size();++id)if(cat.classes.names[id].rfind(prefix,0)==0){
            dh2_script_value arguments[2]{};arguments[0].type=arguments[1].type=DH2_SCRIPT_NUMBER;arguments[0].number=float(id);arguments[1].number=2;
            Return output;const auto status=f.session->call("SetTempProps",arguments,2,0,observe,&output,error);
            check(!status&&output.observed==1,error.c_str());++common_calls;
        }
        check(f.session->vm()==vm,"common preview helper replaced VM");
        captured+=f.capture_count;resources.insert(f.paths.begin(),f.paths.end());++cases;
    }
    {ClassFixture f(argv[1],output/"failure",cat,"RoguePlayerBase",captures);f.fail_apply=true;std::string error;dh2::player_skill_update_session_v1::Result r{};
        const auto status=f.updates->update(r,error);
        if(status!=-1||r.last_lua_status!=-5||r.callbacks!=3||f.applied!=std::vector<int>{208,210}||f.updates->retained_failed_returns()!=1){
            std::cerr<<"failure prefix status="<<status<<" lua="<<r.last_lua_status<<" callbacks="<<r.callbacks<<" retained="<<f.updates->retained_failed_returns()<<" classes=";for(auto id:f.applied)std::cerr<<id<<',';std::cerr<<" error="<<error<<'\n';
            throw std::runtime_error("required Roundhouse failure lost source prefix");
        }
        check(f.temp==f.state.resolved&&f.state.resolved[172]==0,"failed class did not retain ClearProps and SetProp prefix");++failures;
        captured+=f.capture_count;
    }
    captures.close();std::cout<<"{\"validation\":\"PASS\",\"class_cases\":"<<cases<<",\"unchanged_lua_update_callbacks\":"<<callbacks<<",\"selected_faery_script_updates\":"<<selected_faery_updates<<",\"change_faery_all_skill_callbacks\":"<<faery_change_callbacks<<",\"positive_level_common_calls\":"<<common_calls<<",\"temporary_class_calls\":"<<captured<<",\"required_failure_prefixes\":"<<failures<<",\"resources\":[";
    bool first=true;for(const auto& path:resources){if(!first)std::cout<<',';first=false;std::cout<<'"'<<path<<'"';}std::cout<<"]}\n";return 0;
}catch(const std::exception& e){std::cerr<<"class script property fixture FAIL: "<<e.what()<<'\n';return 1;}}
