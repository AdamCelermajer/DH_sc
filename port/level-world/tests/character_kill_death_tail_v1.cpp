#include "../character_kill_death_tail_v1.hpp"
#include "../object_update_culling.hpp"

#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh2::character_kill_death_tail_v1;
namespace {
constexpr std::uintptr_t character=0x100000001ull;
constexpr std::uintptr_t killer_object=0x200000002ull;
constexpr std::uintptr_t killer_character=0x300000003ull;

void check(bool value,const char* why) {
    if (!value) throw std::runtime_error(why);
}

enum class Step {preflight,drop_loot,killer_credit,convert,xp,virtual_54,
                 field_14e4,objective_tail};
struct Fixture {
    std::vector<Step> trace;
    Step fail_at=Step::preflight;
    bool should_fail=false,ready=true,xp_credit=true;
    std::int32_t virtual_result=0;
    std::uint8_t field_value=0;
    dh2::object_update_culling::Object object_base{
        character,UINT32_MAX,0,0,{0,0}};
    unsigned xp_calls=0;

    static bool preflight(void* raw,const Request&,std::string& error) {
        auto& f=*static_cast<Fixture*>(raw);f.trace.push_back(Step::preflight);
        if(!f.ready){error="future owner unavailable";return false;}return true;
    }
    static bool drop(void* raw,std::uintptr_t c,std::uintptr_t k,std::string& error) {
        auto& f=*static_cast<Fixture*>(raw);f.trace.push_back(Step::drop_loot);
        check(c==character&&k==killer_object,"DropLoot identities changed");
        if(f.should_fail&&f.fail_at==Step::drop_loot){error="drop failed";return false;}return true;
    }
    static bool convert(void* raw,std::uintptr_t c,std::uintptr_t k,
                        std::uintptr_t& out,bool& credit,std::string& error) {
        auto& f=*static_cast<Fixture*>(raw);f.trace.push_back(Step::convert);
        check(c==character&&k==killer_object,"killer conversion inputs changed");
        if(f.should_fail&&f.fail_at==Step::convert){error="convert failed";return false;}
        out=killer_character;credit=f.xp_credit;return true;
    }
    static bool credit(void* raw,std::uintptr_t c,std::uintptr_t k,std::string& error) {
        auto& f=*static_cast<Fixture*>(raw);f.trace.push_back(Step::killer_credit);
        check(c==character&&k==killer_object,"OnKill credit identities changed");
        if(f.should_fail&&f.fail_at==Step::killer_credit){error="credit failed";return false;}return true;
    }
    static bool xp(void* raw,std::uintptr_t k,std::uintptr_t c,std::string& error) {
        auto& f=*static_cast<Fixture*>(raw);f.trace.push_back(Step::xp);++f.xp_calls;
        check(k==killer_character&&c==character,"DistributeXP identities changed");
        if(f.should_fail&&f.fail_at==Step::xp){error="xp failed";return false;}return true;
    }
    static bool virtual54(void* raw,std::uintptr_t c,std::int32_t& source_result,
                          std::string& error) {
        auto& f=*static_cast<Fixture*>(raw);f.trace.push_back(Step::virtual_54);
        check(c==character,"vtable+0x54 Character changed");
        source_result=f.virtual_result;
        if(f.should_fail&&f.fail_at==Step::virtual_54){error="virtual failed";return false;}
        return true;
    }
    static bool source_virtual54(void* raw,std::uintptr_t c,
                                 std::int32_t& source_result,
                                 std::string& error) {
        auto& f=*static_cast<Fixture*>(raw);
        f.trace.push_back(Step::virtual_54);
        if(c!=character||f.object_base.identity!=c){
            error="ObjectBase IsRemotelyUpdated identity mismatch";return false;
        }
        dh2::object_update_culling::RemoteResult remote{};
        if(dh2::object_update_culling::is_remotely_updated(
               &f.object_base,&remote)!=dh2::object_update_culling::Status::complete){
            error="ObjectBase IsRemotelyUpdated source leaf failed";return false;
        }
        source_result=static_cast<std::int32_t>(remote.raw);
        return true;
    }
    static bool field(void* raw,std::uintptr_t c,std::uint8_t& value,std::string& error) {
        auto& f=*static_cast<Fixture*>(raw);f.trace.push_back(Step::field_14e4);
        check(c==character,"Character+0x14e4 owner changed");
        if(f.should_fail&&f.fail_at==Step::field_14e4){error="field failed";return false;}
        value=f.field_value;return true;
    }
    static bool objective(void* raw,std::uintptr_t c,std::uintptr_t k,
                          std::int32_t virtual_result,std::uint8_t field,
                          std::string& error) {
        auto& f=*static_cast<Fixture*>(raw);f.trace.push_back(Step::objective_tail);
        check(c==character&&k==killer_object&&virtual_result==0&&field==0,
              "Kill objective tail received ungated source facts");
        if(f.should_fail&&f.fail_at==Step::objective_tail){error="objective failed";return false;}
        return true;
    }
    Services services(){return {this,preflight,drop,credit,convert,xp,virtual54,field,objective};}
};

Runtime make_runtime(Fixture& f,bool forced=false) {
    return Runtime({character,killer_object,forced?1u:0u},f.services());
}
}

int main(){try{
    constexpr std::array<Step,8> source_order={Step::preflight,Step::drop_loot,
        Step::killer_credit,Step::convert,Step::xp,Step::virtual_54,
        Step::field_14e4,Step::objective_tail};
    {
        Fixture f;auto runtime=make_runtime(f);Result result{};std::string error;
        check(runtime.run(&result,error)==Status::complete&&
              f.trace==std::vector<Step>(source_order.begin(),source_order.end())&&
              result.drop_loot_completed&&result.killer_conversion_completed&&
              result.killer_credit_completed&&
              result.xp_completed&&result.virtual_54_completed&&
              result.character_14e4_completed&&result.character_14e4==0&&
              result.objective_tail_enabled&&!result.objective_tail_skipped&&
              result.objective_tail_completed,
              "Character::Kill native tail differs from source order");
        check(runtime.run(&result,error)==Status::consumed&&f.xp_calls==1,
              "completed death tail replayed DistributeXP");
    }
    {
        Fixture f;f.ready=false;auto runtime=make_runtime(f);Result result{};std::string error;
        check(runtime.run(&result,error)==Status::missing_owner&&f.trace.size()==1&&
              f.trace[0]==Step::preflight&&!result.drop_loot_attempted,
              "post-XP provider readiness was not checked before DropLoot");
        f.ready=true;
        check(runtime.run(&result,error)==Status::complete&&f.xp_calls==1,
              "side-effect-free preflight rejection consumed the death episode");
    }
    {
        Fixture f;f.should_fail=true;f.fail_at=Step::virtual_54;
        auto runtime=make_runtime(f);Result result{};std::string error;
        check(runtime.run(&result,error)==Status::provider_failed&&
              result.xp_completed&&result.virtual_54_attempted&&
              !result.virtual_54_completed&&!result.character_14e4_attempted&&
              f.xp_calls==1,"post-XP failure lost the retained source prefix");
        f.should_fail=false;
        check(runtime.run(&result,error)==Status::consumed&&f.xp_calls==1,
              "failure after XP replayed the irreversible award");
    }
    {
        Fixture f;f.should_fail=true;f.fail_at=Step::xp;
        auto runtime=make_runtime(f);Result result{};std::string error;
        check(runtime.run(&result,error)==Status::provider_failed&&
              result.xp_attempted&&!result.xp_completed&&
              !result.virtual_54_attempted&&f.xp_calls==1,
              "DistributeXP failure lost its attempted source prefix");
        f.should_fail=false;
        check(runtime.run(&result,error)==Status::consumed&&f.xp_calls==1,
              "failed DistributeXP callback was replayed");
    }
    {
        Fixture f;f.should_fail=true;f.fail_at=Step::field_14e4;
        auto runtime=make_runtime(f);Result result{};std::string error;
        check(runtime.run(&result,error)==Status::provider_failed&&
              result.xp_completed&&result.virtual_54_completed&&
              result.character_14e4_attempted&&!result.character_14e4_completed,
              "field-read failure did not retain the +0x54 prefix");
        f.should_fail=false;
        check(runtime.run(&result,error)==Status::consumed&&f.xp_calls==1,
              "Character+0x14e4 failure replayed XP or the vtable callback");
    }
    {
        Fixture f;f.should_fail=true;f.fail_at=Step::objective_tail;
        auto runtime=make_runtime(f);Result result{};std::string error;
        check(runtime.run(&result,error)==Status::provider_failed&&
              result.xp_completed&&result.objective_tail_attempted&&
              !result.objective_tail_completed&&f.xp_calls==1,
              "objective-tail failure lost the completed Kill source prefix");
        f.should_fail=false;
        check(runtime.run(&result,error)==Status::consumed&&f.xp_calls==1,
              "failed objective tail replayed XP or earlier Kill effects");
    }
    {
        Fixture f;f.should_fail=true;f.fail_at=Step::killer_credit;
        auto runtime=make_runtime(f);Result result{};std::string error;
        check(runtime.run(&result,error)==Status::provider_failed&&
              result.drop_loot_completed&&result.killer_credit_attempted&&
              !result.killer_conversion_attempted&&f.xp_calls==0,
              "OnKill credit failure crossed the conversion/XP boundary");
    }
    {
        Fixture f;f.xp_credit=false;auto runtime=make_runtime(f);Result result{};std::string error;
        check(runtime.run(&result,error)==Status::complete&&f.xp_calls==0&&
              f.trace==std::vector<Step>({Step::preflight,Step::drop_loot,
                  Step::killer_credit,Step::convert,Step::virtual_54,
                  Step::field_14e4,Step::objective_tail}),
              "non-credit killer incorrectly reached DistributeXP");
    }
    {
        Fixture f;f.virtual_result=1;auto runtime=make_runtime(f);Result result{};std::string error;
        check(runtime.run(&result,error)==Status::complete&&
              result.virtual_54_completed&&result.virtual_54_result==1&&
              result.objective_tail_skipped&&!result.character_14e4_attempted&&
              f.trace==std::vector<Step>({Step::preflight,Step::drop_loot,
                  Step::killer_credit,Step::convert,Step::xp,Step::virtual_54}),
              "nonzero vtable+0x54 did not bypass the +0x14e4/quest tail");
    }
    {
        Fixture f;f.object_base.remote_word_110=0;
        auto services=f.services();services.virtual_54=Fixture::source_virtual54;
        Runtime runtime({character,killer_object,0},services);Result result{};std::string error;
        check(runtime.run(&result,error)==Status::complete&&
              result.virtual_54_result==1&&result.objective_tail_skipped&&
              !result.character_14e4_attempted&&f.trace.back()==Step::virtual_54,
              "source ObjectBase +0x54 remote predicate did not gate Kill tail");
    }
    {
        Fixture f;f.field_value=1;auto runtime=make_runtime(f);Result result{};std::string error;
        check(runtime.run(&result,error)==Status::complete&&
              result.character_14e4_completed&&result.objective_tail_skipped&&
              !result.objective_tail_enabled&&f.trace.back()==Step::field_14e4,
              "nonzero Character+0x14e4 did not bypass the quest tail");
    }
    {
        Fixture f;auto runtime=make_runtime(f,true);Result result{};std::string error;
        check(runtime.run(&result,error)==Status::complete&&result.returned_after_drop&&
              f.trace==std::vector<Step>({Step::preflight,Step::drop_loot})&&
              f.xp_calls==0,"forced Character::Kill did not return after DropLoot");
    }
    std::cout<<"CHARACTER KILL DEATH TAIL PASS 11\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
