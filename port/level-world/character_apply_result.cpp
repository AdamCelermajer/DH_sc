#include "character_apply_result.hpp"
#include <cmath>
#include <cstring>
#include <limits>

namespace dh2::character_apply_result {
namespace {
template<class T>bool aligned(const T* p){return p&&std::uintptr_t(p)%alignof(T)==0;}
bool overlap(const void* a,std::size_t n,const void* b,std::size_t m){
    const auto x=std::uintptr_t(a),y=std::uintptr_t(b);
    return x>std::numeric_limits<std::uintptr_t>::max()-n||y>std::numeric_limits<std::uintptr_t>::max()-m||
           (x<y+m&&y<x+n);
}
std::int32_t signed_word(std::uint32_t x){std::int32_t y;std::memcpy(&y,&x,4);return y;}
std::uint32_t asr8(std::uint32_t x){return (x>>8)|((x&0x80000000u)?0xff000000u:0u);}
float floating(std::uint32_t x){float y;std::memcpy(&y,&x,4);return y;}
std::uint32_t bits(float x){std::uint32_t y;std::memcpy(&y,&x,4);return y;}
struct Call {
    const Globals* globals;Services services;data::CombatResult* attack;Result* report;
    Actor* attacker;Actor* defender;std::uintptr_t a,d;
    Status invoke(Operation op,std::uintptr_t subject,Reply& reply,std::uintptr_t peer=0,
                  std::uint32_t w0=0,std::uint32_t w1=0,std::uint32_t w2=0,std::uint32_t w3=0,
                  std::uintptr_t identity=0,const char* text=nullptr){
        Request q{op,subject,peer,identity,text,attack,a,d,w0,w1,w2,w3};reply={};
        report->last_operation=std::uint32_t(op);++report->calls;
        if(!services.invoke)return Status::service_unavailable;
        try{if(services.invoke(services.context,&q,&reply))return Status::service_failed;}
        catch(...){return Status::service_failed;}
        return Status::complete;
    }
    Status effect(Operation op,std::uintptr_t subject,std::uintptr_t peer=0,
                  std::uint32_t w0=0,std::uint32_t w1=0,std::uint32_t w2=0,std::uint32_t w3=0,
                  std::uintptr_t identity=0){
        Reply reply;return invoke(op,subject,reply,peer,w0,w1,w2,w3,identity);
    }
    Status unsupported(Status s,std::uint32_t pc){report->unsupported_pc=pc;return s;}
    Status is_player(std::uintptr_t subject,bool& value){
        Reply reply;auto s=invoke(Operation::is_player,subject,reply);
        if(s==Status::complete)value=reply.word!=0;
        return s;
    }
    Status player(std::uintptr_t subject,std::uint32_t pc){
        bool value=false;auto s=is_player(subject,value);
        return s==Status::complete&&value?unsupported(Status::unsupported_player,pc):s;
    }
    Status player_counter(std::uintptr_t subject,std::uint32_t property,
                          std::int32_t threshold,bool strict,const char* trophy){
        bool is_player_actor=false;auto s=is_player(subject,is_player_actor);
        if(s!=Status::complete||!is_player_actor)return s;
        s=effect(Operation::player_property_add_int,subject,0,property,1);if(s!=Status::complete)return s;
        Reply reply;s=invoke(Operation::player_property_get_int,subject,reply,0,property,0);if(s!=Status::complete)return s;
        const auto value=signed_word(reply.word);
        if(strict?value<=threshold:value<threshold)return Status::complete;
        s=invoke(Operation::player_is_local,globals->application,reply,subject);if(s!=Status::complete)return s;
        if(!reply.word)return Status::complete;
        s=invoke(Operation::trophy_lookup,0,reply,0,0,0,0,0,0,trophy);if(s!=Status::complete)return s;
        return effect(Operation::trophy_unlock,globals->application,0,reply.word);
    }
    Status player_stat(std::uintptr_t subject,std::uint32_t stat){
        Reply reply;auto s=invoke(Operation::player_by_character,
            globals->application,reply,subject,0);if(s!=Status::complete)return s;
        return effect(Operation::player_stat_increment,globals->application,0,
                      stat,reply.word);
    }
    Status inventory_gold_damage(){
        if(!services.read_inventory_gold)return Status::service_unavailable;
        std::int32_t gold=0;
        try{if(services.read_inventory_gold(services.context,a,&gold))return Status::service_failed;}
        catch(...){return Status::service_failed;}
        const auto scaled=std::uint32_t(gold)<<8;
        const auto amount=std::uint32_t(attack->amount);
        const auto applied=signed_word(scaled)>=signed_word(amount)?amount:scaled;
        attack->amount=signed_word(applied);
        const auto debit=std::uint32_t(0)-asr8(applied);
        return effect(Operation::inventory_add_gold,a,0,debit);
    }
    Status read_coop_factor(std::uint32_t offset,std::uint32_t& bits_value){
        if(!services.read_coop_factor)return Status::service_unavailable;
        try{if(services.read_coop_factor(services.context,offset,&bits_value))return Status::service_failed;}
        catch(...){return Status::service_failed;}
        return Status::complete;
    }
    Status begin_debug(std::uintptr_t debug,const char* key,std::uintptr_t& string,std::uint32_t& value){
        Reply reply;auto s=invoke(Operation::debug_load,debug,reply);if(s!=Status::complete)return s;
        ++report->debug_phases;
        s=invoke(Operation::string_construct,0,reply,0,0,0,0,0,0,key);if(s!=Status::complete)return s;
        string=reply.identity;if(!string)return Status::invalid_provider_result;
        s=invoke(Operation::debug_query,debug,reply,0,0,0,0,0,string,key);
        value=reply.word;return s;
    }
    Status destroy(std::uintptr_t string){return effect(Operation::string_destroy,string);}
    Status debug(const char* key){
        const auto owner=globals->debug_switches;std::uintptr_t string=0;std::uint32_t value=0;
        auto s=begin_debug(owner,key,string,value);return s==Status::complete?destroy(string):s;
    }
    Status status(Operation op,std::uintptr_t subject,std::uintptr_t peer=0,
                  std::uint32_t w0=0,std::uint32_t w1=0,std::uint32_t w2=0,std::uint32_t w3=0){
        auto s=effect(op,subject,peer,w0,w1,w2,w3);if(s==Status::complete)++report->status_calls;return s;
    }
};
}
Status execute(const Arguments* args,const Globals* globals,const Services* services,
               data::CombatResult* attack,Result* report){
    static_assert(sizeof(float)==4&&std::numeric_limits<float>::is_iec559,"binary32 required");
    if(!aligned(args)||!aligned(globals)||!aligned(services)||!aligned(attack)||!aligned(report))return Status::invalid_argument;
    const void* controls[]={args,globals,services,attack,report};
    const std::size_t sizes[]={sizeof(*args),sizeof(*globals),sizeof(*services),sizeof(*attack),sizeof(*report)};
    for(unsigned i=0;i<5;++i)for(unsigned j=i+1;j<5;++j)if(overlap(controls[i],sizes[i],controls[j],sizes[j]))return Status::invalid_argument;
    if(!aligned(args->attacker)||!aligned(args->defender))return Status::invalid_argument;
    const void* p[]={args,globals,services,attack,report,args->attacker,args->defender};
    const std::size_t n[]={sizeof(*args),sizeof(*globals),sizeof(*services),sizeof(*attack),sizeof(*report),sizeof(Actor),sizeof(Actor)};
    for(unsigned i=0;i<7;++i)for(unsigned j=i+1;j<7;++j){
        if(i==5&&j==6&&p[i]==p[j])continue;
        if(overlap(p[i],n[i],p[j],n[j]))return Status::invalid_argument;
    }
    if(!args->attacker->identity||!args->defender->identity)return Status::invalid_argument;
    if(args->attacker!=args->defender&&args->attacker->identity==args->defender->identity)return Status::invalid_argument;
    *report={};Call c{globals,*services,attack,report,args->attacker,args->defender,args->attacker->identity,args->defender->identity};
    Reply reply;auto s=c.invoke(Operation::online_mode,0,reply);if(s!=Status::complete)return s;
    if(reply.word)return c.unsupported(Status::unsupported_network,0x3b1440);
    c.attacker->combo_14d0=(attack->outcomes&3)?0:std::uint16_t(c.attacker->combo_14d0+1u);
    const auto debug=globals->debug_switches;std::uintptr_t outer=0,inner=0;std::uint32_t no_damage=0,god=0;
    s=c.begin_debug(debug,"NoDamages",outer,no_damage);if(s!=Status::complete)return s;
    bool skip_damage=no_damage!=0;
    if(!skip_damage){
        s=c.begin_debug(debug,"GOD",inner,god);if(s!=Status::complete)return s;
        if(!god){s=c.invoke(Operation::saved_option,globals->application,reply,0,0,0,0,0,0,"GOD");if(s!=Status::complete)return s;god=reply.word;}
        if(god){
            bool player_defender=false;s=c.is_player(c.d,player_defender);if(s!=Status::complete)return s;
            skip_damage=player_defender||c.defender->invulnerable_14f0!=0;
        }else skip_damage=c.defender->invulnerable_14f0!=0;
        s=c.destroy(inner);if(s!=Status::complete)return s;
    }
    s=c.destroy(outer);if(s!=Status::complete)return s;
    if(!skip_damage){
        if(attack->mask&0x400000u){s=c.inventory_gold_damage();if(s!=Status::complete)return s;}
        auto hit_amount=std::uint32_t(attack->amount);report->captured_hit_amount=hit_amount;
        if(signed_word(hit_amount)>0){
            s=c.invoke(Operation::coop_player_count,0,reply);if(s!=Status::complete)return s;
            if(signed_word(reply.word)>1){
                const auto player_count=signed_word(reply.word);
                s=c.invoke(Operation::is_monster,c.a,reply);if(s!=Status::complete)return s;
                std::uint32_t factor_bits=0;bool scale_amount=false,write_attack_amount=false;
                if(reply.word){
                    s=c.read_coop_factor(52u,factor_bits);if(s!=Status::complete)return s;
                    scale_amount=true;write_attack_amount=true;
                }else{
                    bool player_attacker=false;s=c.is_player(c.a,player_attacker);if(s!=Status::complete)return s;
                    if(player_attacker){
                        s=c.read_coop_factor(56u,factor_bits);if(s!=Status::complete)return s;
                        scale_amount=true;
                    }
                }
                if(scale_amount){
                    volatile float extra_players=float(player_count-1);
                    volatile float factor=floating(factor_bits);
                    volatile float scaled=extra_players*factor;
                    volatile float rounded=scaled+1.0f;
                    const float multiplier_value=rounded;
                    if(!std::isfinite(multiplier_value)||multiplier_value<-2147483648.0f||multiplier_value>=2147483648.0f)
                        return c.unsupported(Status::unsupported_coop_scaling,0x3b1580);
                    const auto multiplier=static_cast<std::int32_t>(multiplier_value);
                    if(write_attack_amount){
                        hit_amount=std::uint32_t(signed_word(hit_amount)*multiplier);
                        attack->amount=signed_word(hit_amount);
                    }else{
                        const auto signed_amount=signed_word(hit_amount);
                        if(multiplier==0||(signed_amount==std::numeric_limits<std::int32_t>::min()&&multiplier==-1))
                            return c.unsupported(Status::unsupported_coop_scaling,0x3b1b24);
                        hit_amount=std::uint32_t(signed_amount/multiplier);
                    }
                }
                report->captured_hit_amount=hit_amount;
            }
            s=c.invoke(Operation::effective_threat,c.a,reply);if(s!=Status::complete)return s;
            if((reply.word&0x7f800000u)==0x7f800000u)return c.unsupported(Status::unsupported_threat_value,0x3b15cc);
            const float per_damage=floating(reply.word);
            volatile float converted=float(attack->amount);
            volatile float amount=converted*0.00390625f;
            volatile float threat=per_damage*amount;
            report->threat=bits(threat);
            s=c.invoke(Operation::add_aggro,c.d,reply,c.a,report->threat);if(s!=Status::complete)return s;
            if(floating(reply.word)>0.0f){s=c.debug("isTracingThreatChange");if(s!=Status::complete)return s;}
            c.defender->push_death_53b=(attack->outcomes&0x80u)?std::uint8_t((attack->mask>>20)&1):0;
            if(c.defender->remote_update_word_110==-1){
                s=c.debug("isTracingChar_Attack");if(s!=Status::complete)return s;
                s=c.effect(Operation::hit_for,c.d,c.a,hit_amount);if(s!=Status::complete)return s;++report->hits;
            }
            if(attack->mask&0x200000u){
                std::uint32_t fx=std::uint32_t(attack->element);
                if(fx==0xffffffffu){
                    s=c.invoke(Operation::is_dead,c.d,reply);if(s!=Status::complete)return s;
                    s=c.invoke(reply.word?Operation::blood_death_fx:Operation::blood_fx,c.d,reply);if(s!=Status::complete)return s;fx=reply.word;
                }else fx+=0x7cu;
                s=c.invoke(Operation::target_position,c.d,reply);if(s!=Status::complete)return s;
                if(!reply.identity)return Status::invalid_provider_result;
                s=c.effect(Operation::play_anim_fx_set,globals->visual_fx,c.d,fx,0,0,0,reply.identity);if(s!=Status::complete)return s;
            }
            s=c.invoke(Operation::is_dead,c.d,reply);if(s!=Status::complete)return s;
            if(reply.word)attack->outcomes&=~0x160u;
            if(attack->outcomes&8u){
                bool player_attacker=false;s=c.is_player(c.a,player_attacker);if(s!=Status::complete)return s;
                if(player_attacker){s=c.effect(Operation::critical_camera,c.a);if(s!=Status::complete)return s;}
            }
        }
    }
    s=c.effect(Operation::regen_hp,c.a,0,std::uint32_t(attack->hp_leech));if(s!=Status::complete)return s;++report->regenerations;
    s=c.effect(Operation::regen_mp,c.a,0,std::uint32_t(attack->mp_leech));if(s!=Status::complete)return s;++report->regenerations;
    s=c.invoke(Operation::is_dead,c.d,reply);if(s!=Status::complete)return s;
    if(!reply.word){
        const auto duration=std::uint32_t(attack->dot_duration);
        if(signed_word(duration)>0){
            const auto amount=std::uint32_t(attack->dot_amount);
            if(signed_word(amount)>0){s=c.status(Operation::apply_dot,c.d,0,asr8(duration),amount,std::uint32_t(attack->dot_element));if(s!=Status::complete)return s;}
        }
        const std::uint32_t special=bool(attack->mask&0x18000000u);
        bool player_defender=false;s=c.is_player(c.d,player_defender);if(s!=Status::complete)return s;
        if(player_defender){
            s=c.invoke(Operation::state_machine_is_idle,c.d,reply,0,0);if(s!=Status::complete)return s;
            if(reply.word&&!(attack->outcomes&0x16u))
                attack->outcomes|=signed_word(std::uint32_t(attack->amount))>0?0x10u:2u;
        }
        auto flags=attack->outcomes&0xffu;
        if(flags&2u){
            s=c.status(Operation::dodge,c.d,c.a,0);if(s!=Status::complete)return s;
            s=c.player_counter(c.d,215,500,false,"epic_invisible");if(s!=Status::complete)return s;
            s=c.debug("isTracingChar_Attack");if(s!=Status::complete)return s;flags=attack->outcomes&0xffu;
        }
        if(flags&4u){
            s=c.status(Operation::block,c.d,c.a,0);if(s!=Status::complete)return s;
            s=c.player_counter(c.d,214,500,false,"epic_invincible");if(s!=Status::complete)return s;
            s=c.debug("isTracingChar_Attack");if(s!=Status::complete)return s;flags=attack->outcomes&0xffu;
        }
        if(flags&0x10u){s=c.status(Operation::hurt,c.d,c.a,special);if(s!=Status::complete)return s;
            s=c.debug("isTracingChar_Attack");if(s!=Status::complete)return s;flags=attack->outcomes&0xffu;}
        if(flags&0x80u){s=c.status(Operation::push,c.d,c.a,(attack->mask>>20)&1,special);if(s!=Status::complete)return s;
            s=c.player_counter(c.d,222,49,true,"knocked_down_50_times");if(s!=Status::complete)return s;
            s=c.debug("isTracingChar_Attack");if(s!=Status::complete)return s;flags=attack->outcomes&0xffu;}
        if(flags&0x40u){s=c.invoke(Operation::read_property,c.a,reply,0,(attack->mask&0x1000u)?185u:140u);if(s!=Status::complete)return s;
            s=c.status(Operation::stun,c.d,c.a,asr8(reply.word),1,0);if(s!=Status::complete)return s;
            s=c.debug("isTracingChar_Attack");if(s!=Status::complete)return s;flags=attack->outcomes&0xffu;}
        if(flags&0x20u){s=c.invoke(Operation::read_property,c.a,reply,0,(attack->mask&0x4000u)?187u:143u);if(s!=Status::complete)return s;
            const auto duration_word=asr8(reply.word);
            if(duration_word){s=c.status(Operation::fear,c.d,c.a,duration_word,1,special);if(s!=Status::complete)return s;}
            s=c.debug("isTracingChar_Attack");if(s!=Status::complete)return s;
        }
        if(attack->outcomes&0x100u){s=c.invoke(Operation::read_property,c.a,reply,0,(attack->mask&0x10000u)?189u:146u);if(s!=Status::complete)return s;
            s=c.status(Operation::slow,c.d,0,asr8(reply.word));if(s!=Status::complete)return s;
            s=c.debug("isTracingChar_Attack");if(s!=Status::complete)return s;}
    }
    s=c.effect(Operation::cancel_sneaking,c.d);if(s!=Status::complete)return s;++report->notifications;
    s=c.effect(Operation::combat_text,0);if(s!=Status::complete)return s;++report->notifications;
    s=c.effect(Operation::combat_sound,0);if(s!=Status::complete)return s;++report->notifications;
    if(!(attack->mask&0x20000000u)){
        s=c.effect(Operation::ai_combat_result,c.a);if(s!=Status::complete)return s;++report->ai_callbacks;
        s=c.effect(Operation::ai_combat_result,c.d);if(s!=Status::complete)return s;++report->ai_callbacks;
    }
    bool player_defender=false;s=c.is_player(c.d,player_defender);if(s!=Status::complete)return s;
    if(player_defender){s=c.player_stat(c.d,3);if(s!=Status::complete)return s;}
    bool player_attacker=false;s=c.is_player(c.a,player_attacker);if(s!=Status::complete)return s;
    if(!player_attacker)return Status::complete;
    // PlayerManager::GetPlayerByCharacter(attacker, false), followed by the
    // source PlayerStatManager tail. Read the mutable result only at each
    // branch, preserving callbacks that changed outcomes or amount earlier.
    s=c.invoke(Operation::player_by_character,globals->application,reply,c.a,0);if(s!=Status::complete)return s;
    const auto player_id=reply.word;
    if(attack->outcomes&0x160u){
        s=c.effect(Operation::player_stat_increment,globals->application,0,2u,player_id);if(s!=Status::complete)return s;
    }
    s=c.invoke(Operation::is_dead,c.d,reply);if(s!=Status::complete)return s;
    if(reply.word){
        s=c.effect(Operation::player_stat_increment,globals->application,0,0u,player_id);if(s!=Status::complete)return s;
    }
    return c.effect(Operation::player_stat_increase,globals->application,0,1u,
                    asr8(std::uint32_t(attack->amount)),player_id);
}
} // namespace dh2::character_apply_result
