#include "../player_save_load_owner_v1.hpp"
#include "../player_profile_filename_v1.hpp"
#include <algorithm>
#include <array>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::data;
namespace {
using B = std::vector<std::uint8_t>;
using Event = std::array<std::uint32_t, 6>;
void check(bool value, const char* why) { if (!value) throw std::runtime_error(why); }
B file(const std::filesystem::path& p) {
    std::ifstream stream(p, std::ios::binary);
    check(bool(stream), "fixture file unavailable");
    return {std::istreambuf_iterator<char>(stream), {}};
}
struct Reader {
    B b; std::size_t at{};
    template<class T> T scalar() {
        check(sizeof(T) <= b.size() - at, "truncated fixture");
        T value; std::memcpy(&value, b.data() + at, sizeof(value)); at += sizeof(value); return value;
    }
    std::uint32_t word() { return scalar<std::uint32_t>(); }
    B bytes(std::size_t n) { check(n <= b.size() - at, "truncated fixture block"); B out(b.begin()+at, b.begin()+at+n); at += n; return out; }
    B block() { return bytes(word()); }
};
constexpr const char* tags[]{"PNAM","PLVL","PCLS","PDFL","LNAM","LEPT","LUSP","LVLS","SKIL","FAES","CFEE","QEST","PROP","GEAR","FTVL"};
std::uint32_t token(std::uintptr_t identity) {
    check(identity <= 3, "unexpected source fixture identity");
    return static_cast<std::uint32_t>(identity);
}
struct Fixture {
    PlayerSavegameV1 save;
    PlayerSaveProfileV1 canonical;
    std::shared_ptr<int> lease = std::make_shared<int>(1);
    std::vector<Event> events;
    unsigned flags{}; std::uint64_t length{};
    std::size_t failure=SIZE_MAX;
    PlayerSaveLoadOwnerV1* runtime{};
    bool replace_after_faeries{}, replace_on_online{}, clear_after_name{}, throw_failure{}, reentry{};
    PlayerSaveProfileV1 profile(unsigned identity) { return {identity, lease, {}}; }
    bool invoke(const PlayerSaveLoadRequestV1& request, PlayerSaveLoadResponseV1& response, std::string& error) {
        check(request.save==&save, "same canonical Save required");
        Event event{std::uint32_t(request.operation), request.argument, token(request.profile.identity), 0, 0, 0};
        switch(request.operation) {
        case PlayerSaveLoadOpV1::filename:
            check(request.argument==std::uint32_t(save.slot()), "fresh slot required"); response.text="source_profile"; break;
        case PlayerSaveLoadOpV1::create_profile:
            check(request.filename&&!std::strcmp(request.filename,"source_profile"), "delivered source filename required"); response.profile=profile(2); break;
        case PlayerSaveLoadOpV1::load_section: {
            check(request.section&&request.writer_enabled, "source tag/writer required");
            unsigned tag=0;while(tag<15&&std::strcmp(tags[tag],request.section))++tag;
            check(tag<15,"actual source tag required");event[3]=tag;event[4]=request.reader_enabled;
            if (clear_after_name&&tag==0) canonical={};
            if (replace_after_faeries&&tag==9) canonical=profile(2);
            break;
        }
        case PlayerSaveLoadOpV1::online:
            response.flag=flags&1;if(replace_on_online)canonical=profile(3);break;
        case PlayerSaveLoadOpV1::hosting_quest_flag:response.flag=flags&2;break;
        case PlayerSaveLoadOpV1::local_hosting:response.flag=flags&4;break;
        case PlayerSaveLoadOpV1::load_volatile_flag:response.flag=flags&8;break;
        case PlayerSaveLoadOpV1::volatile_stream:response.profile=profile(3);break;
        case PlayerSaveLoadOpV1::stream_size:response.amount=length;break;
        case PlayerSaveLoadOpV1::stream_seek:check(request.argument==0,"source zero seek");break;
        case PlayerSaveLoadOpV1::quest_definition:response.value=37;break;
        case PlayerSaveLoadOpV1::unpack_quests:
            check(request.argument==0&&request.definition==37,"source quest unpack arguments");event[5]=37;break;
        default:break;
        }
        events.push_back(event);
        if(reentry) {
            std::string nested;check(runtime&&!runtime->load(1,nested),"reentry rejected");
            check(runtime->delivered_calls()==events.size(),"reentry retains source prefix");
        }
        if(events.size()==failure) {
            error="required source delivery rejected";
            if(throw_failure)throw std::runtime_error("provider throw");
            return false;
        }
        return true;
    }
    PlayerSaveLoadServicesV1 services() {
        return {lease,[this](const auto& q,auto& r,auto& e){return invoke(q,r,e);}};
    }
};
void word(B& out, std::uint32_t n) { for(unsigned i=0;i<4;++i) out.push_back(std::uint8_t(n>>(8*i))); }
B campaign(const std::vector<std::pair<std::string,B>>& entries) {
    B out;word(out,static_cast<std::uint32_t>(entries.size()));
    for(const auto& row:entries) {
        check(row.first.size()==4,"fixture tag length");word(out,static_cast<std::uint32_t>(row.second.size()));
        out.insert(out.end(),row.first.begin(),row.first.end());out.insert(out.end(),row.second.begin(),row.second.end());
    }
    return out;
}
PlayerSaveProfileV1 indexed(const B& bytes) {
    auto index=std::make_shared<PlayerProfileIndexV1>();std::string error;
    check(index->load({bytes.data(),bytes.size()},error),"actual tagged profile required");
    return {reinterpret_cast<std::uintptr_t>(index.get()),index,index->borrow()};
}
int source_index(Reader& r, unsigned& sections, unsigned& guards) {
    check(r.bytes(4)==B({'P','I','X','1'}),"index gold magic");auto count=r.word();B nonempty;
    for(unsigned i=0;i<count;++i) {
        auto blob=r.block();Reader expected{r.block()};PlayerProfileIndexV1 index;std::string error;
        check(index.load({blob.data(),blob.size()},error),"source index load");auto view=index.borrow();
        check(view&&view.bytes()==blob,"retained indexed bytes");auto rows=expected.word();
        while(rows--) {
            auto name=expected.block();std::string key(name.begin(),name.end());auto offset=expected.word(),size=expected.word();
            auto found=view.section(key.c_str());check(found&&found->offset==offset&&found->size==size,"original STL last tag result");
            auto span=view.payload(key.c_str());check(span.data==view.bytes().data()+offset&&span.size==size,"source payload span");++sections;
        }
        check(expected.at==expected.b.size()&&!view.section("missing"),"exact source index rows");
        check(!index.load({blob.data(),blob.size()},error),"borrowed reload rejected");++guards;
        if(blob.size()>20&&nonempty.empty())nonempty=blob;
    }
    check(r.at==r.b.size(),"index gold consumed");PlayerProfileIndexV1 old;B empty(4,0);std::string error;
    check(old.load({empty.data(),empty.size()},error),"valid source count zero");
    for(std::size_t n=0;n<nonempty.size();++n) {
        check(!old.load({nonempty.data(),n},error)&&old.borrow().bytes()==empty,"truncated index atomic publication");++guards;
    }
    B corrupt(4,255);check(!old.load({corrupt.data(),4},error),"source corruption marker");++guards;
    PlayerProfileIndexV1::Borrow retained;
    { PlayerProfileIndexV1 temporary;check(temporary.load({nonempty.data(),nonempty.size()},error),"retained fixture");retained=temporary.borrow(); }
    check(retained.bytes()==nonempty,"borrow outlives index handle");++guards;
    // Actual low-level rejected callback retains payload-start cursor.
    auto blob=campaign({{"PNAM",B{1,2,3}}});unsigned callbacks=0;
    ProfileIndexSpan24V1 span{blob.data(),static_cast<std::uint32_t>(blob.size()),0,0,0};
    ProfileIndexServices16V1 service{&callbacks,[](void* p,const auto*){++*static_cast<unsigned*>(p);return false;}};
    check(dh2_player_profile_v1_index(&span,&service)==-3&&span.cursor==12&&callbacks==1,"index failed storage prefix");++guards;
    return static_cast<int>(count);
}
int source_load(Reader& r,unsigned& boundaries,unsigned& failures,unsigned& guards) {
    check(r.bytes(4)==B({'P','S','L','1'}),"load gold magic");auto count=r.word();std::string error;
    for(unsigned j=0;j<count;++j) {
        auto mask=r.scalar<std::int32_t>();auto present=r.word();auto slot=r.scalar<std::int32_t>();auto flags=r.word();
        auto length=r.scalar<std::uint64_t>();auto final=r.word(),n=r.word();std::vector<Event> expected;
        for(unsigned k=0;k<n;++k){Event event;for(auto& value:event)value=r.word();expected.push_back(event);}boundaries+=n;
        Fixture f;f.flags=flags;f.length=length;f.save.set_slot(slot);if(present)f.canonical=f.profile(1);
        PlayerSaveLoadOwnerV1 runtime(f.save,f.canonical,f.services());
        check(runtime.load(mask,error),"original whole SG_Load delivery");
        check(f.events==expected&&token(f.canonical.identity)==final&&&runtime.save()==&f.save,"whole source mask/slot/global order");
        for(unsigned failed=1;failed<=n;++failed) {
            Fixture rejected;rejected.flags=flags;rejected.length=length;rejected.failure=failed;rejected.save.set_slot(slot);
            if(present)rejected.canonical=rejected.profile(1);
            PlayerSaveLoadOwnerV1 partial(rejected.save,rejected.canonical,rejected.services());
            check(!partial.load(mask,error),"mandatory service refusal");
            check(rejected.events==std::vector<Event>(expected.begin(),expected.begin()+failed),"source required failure prefix");
            check(partial.delivered_calls()==failed&&partial.reached_phase()==expected[failed-1][0]+1,"source reached boundary diagnostic");
            check(error=="required source delivery rejected","provider error retained");++failures;
        }
    }
    check(r.at==r.b.size(),"load gold consumed");
    Fixture dynamic;dynamic.canonical=dynamic.profile(1);dynamic.flags=1;dynamic.replace_after_faeries=true;dynamic.replace_on_online=true;
    PlayerSaveLoadOwnerV1 runtime(dynamic.save,dynamic.canonical,dynamic.services());check(runtime.load(4,error),"mutable canonical profile source");
    check(runtime.delivered_calls()>=9&&runtime.delivered_section_calls()==8,
          "profile-backed mask4 dispatches all eight section requests");++guards;
    auto cfee=std::find_if(dynamic.events.begin(),dynamic.events.end(),[](const auto& e){return e[0]==2&&e[3]==10;});
    auto quests=std::find_if(dynamic.events.begin(),dynamic.events.end(),[](const auto& e){return e[0]==2&&e[3]==11;});
    check(cfee!=dynamic.events.end()&&(*cfee)[2]==2&&quests!=dynamic.events.end()&&(*quests)[2]==3,"CFEE captured profile vs fresh following tags");++guards;
    Fixture clear;clear.canonical=clear.profile(1);clear.clear_after_name=true;
    PlayerSaveLoadOwnerV1 cleared(clear.save,clear.canonical,clear.services());check(cleared.load(1,error)&&clear.events.size()==7,"one source block null guard");
    check(clear.events[1][2]==0&&clear.events[6][2]==0,"fresh null profile passed to later source boundaries");++guards;
    Fixture nested;nested.canonical=nested.profile(1);nested.reentry=true;
    PlayerSaveLoadOwnerV1 outer(nested.save,nested.canonical,nested.services());nested.runtime=&outer;check(outer.load(1,error),"successful outer/reentry guard");++guards;
    Fixture thrown;thrown.canonical=thrown.profile(1);thrown.throw_failure=true;thrown.failure=3;
    PlayerSaveLoadOwnerV1 exceptional(thrown.save,thrown.canonical,thrown.services());
    check(!exceptional.load(1,error)&&thrown.events.size()==3&&error=="required source delivery rejected","thrown service prefix/error");
    thrown.throw_failure=false;thrown.failure=SIZE_MAX;thrown.events.clear();check(exceptional.load(1,error)&&thrown.events.size()==7,"explicit later source load allowed");++guards;
    Fixture absent;PlayerSaveLoadOwnerV1 unbound(absent.save,absent.canonical);
    check(!unbound.load(2,error)&&unbound.reached_phase()==4&&unbound.delivered_calls()==1,"missing reached init_levels fails");++guards;
    Fixture inspection;inspection.save.set_slot(-1);
    PlayerSaveLoadOwnerV1 no_profile(inspection.save,inspection.canonical,inspection.services());
    check(no_profile.load(4,error)&&!inspection.canonical.identity&&
          no_profile.delivered_calls()==1&&no_profile.delivered_section_calls()==0&&
          inspection.events.size()==1&&inspection.events[0][0]==std::uint32_t(PlayerSaveLoadOpV1::online),
          "slot -1 mask4 skips profile sections but completes the online guard");++guards;
    check(!unbound.publish_profile({1,{},{}},error)&&!absent.canonical.identity,"incoherent publication rejected");++guards;
    return static_cast<int>(count);
}
int source_metadata(Reader& r,const CharacterTable& characters,unsigned& failures) {
    check(r.bytes(4)==B({'P','M','D','2'}),"metadata gold magic");auto count=r.word();std::string error;
    for(unsigned k=0;k<count;++k) for(unsigned kind=0;kind<7;++kind) {
        auto blob=r.block(),expected=r.block();PlayerSavegameV1 save;std::size_t consumed=999;
        std::int32_t selected=0;
        const auto invoke=[&](PlayerSavegameV1& owner,std::size_t n) {
            if(kind==0)return owner.load_name({blob.data(),n},consumed,error);
            if(kind==1)return owner.load_level({blob.data(),n},consumed,error);
            if(kind==2)return owner.load_class({blob.data(),n},characters.names,consumed,error);
            if(kind==3)return owner.load_difficulty({blob.data(),n},&selected,
                [](void* p,std::int32_t value,std::string&){*static_cast<std::int32_t*>(p)=value;return true;},consumed,error);
            return kind==4?owner.load_level_name({blob.data(),n},consumed,error):kind==5?owner.load_level_entry_points({blob.data(),n},consumed,error):owner.load_use_spawn_points({blob.data(),n},consumed,error);
        };
        check(invoke(save,blob.size())&&consumed==blob.size(),"original full metadata body");
        B actual;
        if(kind==0)actual.assign(save.name().begin(),save.name().end());
        else if(kind==1)word(actual,std::uint32_t(save.level()));
        else if(kind==2)word(actual,std::uint32_t(save.class_id()));
        else if(kind==3){word(actual,std::uint32_t(selected));word(actual,std::uint32_t(save.unlocked_difficulty()));}
        else if(kind==4) {
            const auto& f=save.level_name_fields();word(actual,f.level_id);
            for(const auto* rows:{&f.word50,&f.word5c,&f.quest_wordfc,&f.quest_word15c})for(auto x:*rows)word(actual,std::uint32_t(x));
            check(save.level_name_loaded(),"level ID has genuine producer");
        } else if(kind==5)for(auto x:save.level_entry_points())word(actual,std::uint32_t(x));
        else {actual.assign(save.use_spawn_points().begin(),save.use_spawn_points().end());check(save.use_spawn_points_loaded(),"spawn bytes have genuine producer");}
        check(actual==expected,"source reader destinations");
        if(k==0&&kind>=4) {
            auto changed=blob;for(auto& byte:changed)byte^=0xa5;
            const auto short_size=kind==6?1u:4u;
            bool result=kind==4?save.load_level_name({changed.data(),short_size},consumed,error):kind==5?save.load_level_entry_points({changed.data(),short_size},consumed,error):save.load_use_spawn_points({changed.data(),short_size},consumed,error);
            check(!result&&consumed==short_size,"same Save partial reload prefix");
            if(kind==4) {
                std::uint32_t first;std::memcpy(&first,changed.data(),4);
                check(save.level_name_fields().level_id==first&&save.level_name_loaded(),"existing loaded LNAM first store retained");
                check(std::uint32_t(save.level_name_fields().word50[0])==Reader{expected,4}.word(),"unreached LNAM prior field retained");
            } else if(kind==5) {
                std::uint32_t first,second;std::memcpy(&first,changed.data(),4);std::memcpy(&second,blob.data()+4,4);
                check(std::uint32_t(save.level_entry_points()[0])==first&&std::uint32_t(save.level_entry_points()[1])==second,"same Save LEPT keeps earlier/new and later/old stores");
            } else check(save.use_spawn_points()[0]==changed[0]&&save.use_spawn_points()[1]==blob[1]&&save.use_spawn_points_loaded(),"same Save LUSP retains prior suffix");
            ++failures;
        }
        for(std::size_t n=0;kind>=4&&n<blob.size();++n) {
            PlayerSavegameV1 partial;check(!invoke(partial,n),"truncated metadata refusal");
            auto completed=kind==6?n:n/4*4;check(consumed==completed,"completed read byte prefix");
            if(kind==5)for(std::size_t i=0;i<3;++i) {
                std::uint32_t value=0;if(i<n/4)std::memcpy(&value,blob.data()+4*i,4);
                check(std::uint32_t(partial.level_entry_points()[i])==value,"LEPT reached writes retained");
            }
            if(kind==6)for(std::size_t i=0;i<3;++i)check(partial.use_spawn_points()[i]==(i<n?blob[i]:0),"LUSP reached writes retained");
            if(kind==4) {
                const auto& f=partial.level_name_fields();std::uint32_t values[10]{};
                std::memcpy(values,blob.data(),completed);check(f.level_id==values[0],"LNAM first unsigned prefix");
                for(unsigned i=0;i<3;++i)check(std::uint32_t(f.word50[i])==values[1+3*i]&&std::uint32_t(f.word5c[i])==values[2+3*i]&&std::uint32_t(f.quest_wordfc[i])==values[3+3*i]&&f.quest_wordfc[i]==f.quest_word15c[i],"LNAM ordered destination prefix");
                check(!partial.level_name_loaded(),"incomplete LNAM never ready");
            }
            ++failures;
        }
    }
    check(r.at==r.b.size(),"metadata fixture consumed");return static_cast<int>(count);
}
struct Metadata {
    PlayerSavegameV1 save;PlayerSaveProfileV1 profile;CharacterTable characters;
    std::vector<std::string> calls;std::int32_t selected=-77;bool reject_difficulty{};
    std::filesystem::path cache;
    PlayerMetadataServicesV1 services() {
        return {&characters,this,[](void* p,std::int32_t n,std::string& error){auto& f=*static_cast<Metadata*>(p);f.selected=n;if(f.reject_difficulty){error="difficulty store failed";return false;}return true;}};
    }
    bool invoke(const PlayerSaveLoadRequestV1& q,PlayerSaveLoadResponseV1& response,std::string& error) {
        check(q.save==&save,"same metadata Save");
        if(q.operation==PlayerSaveLoadOpV1::filename) {
            response.text=player_profile_filename_v1(q.argument,false,false);return true;
        }
        if(q.operation==PlayerSaveLoadOpV1::create_profile) {
            check(!cache.empty()&&q.filename,"required profile file boundary");
            response.profile=indexed(file(cache/q.filename));return true;
        }
        check(q.operation==PlayerSaveLoadOpV1::load_section&&q.save==&save,"metadata has no invented initializer/global operation");
        calls.emplace_back(q.section);std::size_t used=0;return load_player_metadata_section_v1(q,services(),used,error);
    }
    PlayerSaveLoadServicesV1 load_services() {
        return {profile.owner,[this](const auto& q,auto& r,auto& e){return invoke(q,r,e);}};
    }
};
CharacterTable character_table(const std::filesystem::path& cache) {
    CharacterTable characters;
    const auto records=file(cache/"data/pydata/character_properties_pyarray.bin");
    const auto names=file(cache/"data/pydata/character_properties_pyarraynames.bin");
    const auto schema=file(cache/"data/pydata/character_properties_pystructnames.bin");std::string error;
    check(load_characters({records.data(),records.size()},{names.data(),names.size()},{schema.data(),schema.size()},characters,error),"genuine CharacterTable required");
    return characters;
}
void actual_private_metadata(const std::filesystem::path& cache,const CharacterTable& characters,unsigned& guards) {
    Metadata fixture;const auto bytes=file(cache/player_profile_filename_v1(0,false,false));fixture.profile=indexed(bytes);
    fixture.characters=characters;std::string error;
    fixture.save.set_slot(0);fixture.save.set_character(0x12345678);
    fixture.cache=cache;auto services=fixture.load_services();fixture.profile={};
    PlayerSaveLoadOwnerV1 runtime(fixture.save,fixture.profile,std::move(services));
    check(runtime.load(1,error)&&fixture.calls==std::vector<std::string>(tags,tags+7),"whole real campaign mask1 closure");
    check(fixture.save.level()==1&&fixture.save.class_id()==263&&fixture.selected==0&&fixture.save.unlocked_difficulty()==0,"anonymized actual saved metadata");
    check(fixture.save.name().size()>0&&fixture.save.level_name_loaded()&&fixture.save.use_spawn_points_loaded(),"all metadata source producers reached");
    check(!fixture.save.skills_initialized()&&fixture.save.skills().empty()&&fixture.save.character()==0x12345678&&runtime.delivered_calls()==9,"metadata preserves gameplay/skill authority");
    const auto prior_name=fixture.save.name();const auto prior_calls=fixture.calls.size();
    auto& alias=const_cast<std::string&>(fixture.save.name());
    check(!runtime.load(1,alias)&&fixture.save.name()==prior_name&&fixture.calls.size()==prior_calls&&runtime.delivered_calls()==9,"error alias preserves canonical Save and delivered prefix");++guards;
    PlayerSaveLoadRequestV1 aliased{PlayerSaveLoadOpV1::load_section};aliased.save=&fixture.save;aliased.profile=fixture.profile;aliased.section="PNAM";
    std::size_t unchanged=777;
    check(!load_player_metadata_section_v1(aliased,fixture.services(),unchanged,alias)&&unchanged==777&&fixture.save.name()==prior_name,"metadata output alias rejected before writes");++guards;
    B level_words(40,0);std::size_t consumed=555;
    check(!fixture.save.load_level_name({level_words.data(),level_words.size()},consumed,alias)&&consumed==555&&fixture.save.name()==prior_name,"direct new reader alias guard");++guards;
    // Reached PDFL store survives a later input failure; no fallback value.
    Metadata rejected;B difficulty;word(difficulty,2);rejected.profile=indexed(campaign({{"PDFL",difficulty}}));rejected.characters=fixture.characters;
    PlayerSaveLoadOwnerV1 partial(rejected.save,rejected.profile,rejected.load_services());
    check(!partial.load(1,error)&&rejected.selected==2&&rejected.calls.size()==4&&rejected.save.unlocked_difficulty()==0,"PDFL external store then truncated saved word prefix");++guards;
    rejected.profile=indexed(campaign({{"PDFL",B{2,0,0,0,1,0,0,0}}}));rejected.calls.clear();rejected.reject_difficulty=true;
    check(!partial.load(1,error)&&rejected.selected==2&&rejected.save.unlocked_difficulty()==0&&error=="difficulty store failed","mandatory PDFL store refusal prefix");++guards;
    // Genuine missing and zero-size tags perform no field reads.
    Metadata missing;missing.profile=indexed(campaign({{"PLVL",B{}}}));
    PlayerSaveLoadOwnerV1 empty(missing.save,missing.profile,missing.load_services());
    check(empty.load(1,error)&&missing.calls.size()==7&&missing.save.class_id()==-1&&!missing.save.level_name_loaded(),"source missing/zero tag no-read branch");++guards;
}
}
int main(int argc,char** argv) {
    try {
        if(argc==2&&!std::strcmp(argv[1],"--mask4-dispatch")) {
            std::string error;
            Fixture published;published.canonical=published.profile(1);
            PlayerSaveLoadOwnerV1 with_profile(published.save,published.canonical,published.services());
            check(with_profile.load(4,error)&&with_profile.delivered_calls()>=9&&
                  with_profile.delivered_section_calls()==8,
                  "published profile dispatches eight mask4 section requests plus online guard");
            Fixture inspection;inspection.save.set_slot(-1);
            PlayerSaveLoadOwnerV1 without_profile(inspection.save,inspection.canonical,inspection.services());
            check(without_profile.load(4,error)&&!inspection.canonical.identity&&
                  without_profile.delivered_calls()==1&&without_profile.delivered_section_calls()==0&&
                  inspection.events.size()==1&&
                  inspection.events[0][0]==std::uint32_t(PlayerSaveLoadOpV1::online),
                  "slot -1 skips mask4 sections but completes online guard");
            std::cout<<"{\"validation\":\"PASS\",\"published_profile_section_requests\":8,"
                     "\"published_profile_total_deliveries\":"<<with_profile.delivered_calls()
                     <<",\"slot_minus_one_section_requests\":0,"
                     "\"slot_minus_one_total_deliveries\":"<<without_profile.delivered_calls()
                     <<",\"mismatches\":0}\n";
            return 0;
        }
        if(argc==5&&!std::strcmp(argv[1],"--filename")) {
            const auto slot=std::stoull(argv[2]);check(slot<=UINT32_MAX,"uint32 slot required");
            std::cout<<'"'<<player_profile_filename_v1(static_cast<std::uint32_t>(slot),std::stoi(argv[3])!=0,std::stoi(argv[4])!=0)<<"\"\n";
            return 0;
        }
        check(argc==3,"reference/cache arguments required");std::filesystem::path ref=argv[1],cache=argv[2];
        unsigned sections=0,guards=0,boundaries=0,failures=0,read_failures=0;
        Reader index{file(ref/"fixtures.bin")},load{file(ref/"load-fixtures.bin")},metadata{file(ref/"metadata-fixtures.bin")};
        auto indexed_cases=source_index(index,sections,guards);auto load_cases=source_load(load,boundaries,failures,guards);
        const auto characters=character_table(cache);
        auto metadata_cases=source_metadata(metadata,characters,read_failures);actual_private_metadata(cache,characters,guards);
        std::cout<<"{\"validation\":\"PASS\",\"index_original_cases\":"<<indexed_cases<<",\"section_checks\":"<<sections
            <<",\"load_original_cases\":"<<load_cases<<",\"ordered_boundaries\":"<<boundaries<<",\"required_failure_prefixes\":"<<failures
            <<",\"metadata_original_cases\":"<<metadata_cases<<",\"metadata_truncation_prefixes\":"<<read_failures<<",\"guards\":"<<guards
            <<",\"private_campaign_mask1\":true,\"saved_class_id\":263,\"saved_level\":1,\"selected_difficulty\":0,\"unlocked_difficulty\":0,\"mismatches\":0}\n";
    } catch(const std::exception& e) { std::cerr<<e.what()<<'\n';return 1; }
}
