#include "../character_apply_result.hpp"
#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
namespace ar=dh2::character_apply_result;
using ar::Operation;
struct Trace {std::array<std::uint64_t,9> w;};
std::uint32_t bits(float f){std::uint32_t x;std::memcpy(&x,&f,4);return x;}
struct Fixture {
    ar::Actor a{101,65535,0,0,-1},d{202,100,0,1,-1};
    ar::Arguments args{&a,&d,0};ar::Globals globals{11,77,88};
    dh2::data::CombatResult attack;ar::Result report{};
    std::uint32_t dead=0,no_damage=0,god=0,saved_god=0,online=0,coop=1,
        per_damage=bits(2.0f),aggro_return=0,player=0,player_id=73,mutation=0,fail=0,throw_at=0;
    std::array<std::uint32_t,224> sheet{};
    std::uint32_t idle=0,local=1,trophy_id=35,unlocked=0;
    std::int32_t gold=10,gold_limit=0x7fffffff;
    bool monster=false;std::uint32_t monster_factor=0x3f000000u,player_factor=0x3f000000u;
    std::vector<Trace> trace;std::uint32_t strings=0,hits=0,notifications=0,phase=0;
    bool nested=false,nested_ok=false;
    Fixture(){attack.amount=256;attack.mask=0x20080000;sheet[140]=sheet[143]=sheet[146]=768;sheet[185]=sheet[187]=sheet[189]=128;}
    static unsigned key(const char* text){
        if(!text)return 0;
        if(std::string(text)=="NoDamages")return 1;
        if(std::string(text)=="GOD")return 2;
        if(std::string(text)=="isTracingThreatChange")return 3;
        if(std::string(text)=="isTracingChar_Attack")return 4;
        if(std::string(text)=="epic_invisible")return 5;
        if(std::string(text)=="epic_invincible")return 6;
        if(std::string(text)=="knocked_down_50_times")return 7;
        assert(false);return 0;
    }
    static std::int32_t invoke(void* p,const ar::Request* q,ar::Reply* r){
        auto& f=*static_cast<Fixture*>(p);
        f.trace.push_back({{std::uint32_t(q->operation),q->subject,q->peer,q->identity,q->word0,q->word1,q->word2,q->word3,key(q->text)}});
        const auto index=f.trace.size();if(index==f.throw_at)throw std::runtime_error("named provider exception");if(index==f.fail)return 7;
        switch(q->operation){
        case Operation::online_mode:r->word=f.online;break;
        case Operation::is_player:r->word=(q->subject==f.player)?7:0;break;
        case Operation::debug_load:++f.phase;if(f.mutation==1&&f.phase==1)f.globals.debug_switches=22;break;
        case Operation::string_construct:r->identity=++f.strings;break;
        case Operation::debug_query:r->word=key(q->text)==1?f.no_damage:key(q->text)==2?f.god:0;break;
        case Operation::string_destroy:break;
        case Operation::saved_option:r->word=f.saved_god;break;
        case Operation::coop_player_count:r->word=f.coop;break;
        case Operation::is_monster:r->word=f.monster;break;
        case Operation::effective_threat:r->word=f.per_damage;if(f.mutation==2)f.attack.amount=512;break;
        case Operation::add_aggro:r->word=f.aggro_return;
            if(f.mutation==3){f.d.remote_update_word_110=99;f.attack.outcomes=128;f.attack.mask|=0x100000;}
            if(f.mutation==4){f.attack.outcomes=128;f.attack.mask|=0x100000;}
            break;
        case Operation::hit_for:++f.hits;if(f.mutation==5)f.dead=7;break;
        case Operation::is_dead:r->word=f.dead;break;
        case Operation::blood_death_fx:r->word=51;break;
        case Operation::blood_fx:r->word=52;break;
        case Operation::target_position:r->identity=777;if(f.mutation==6)f.globals.visual_fx=99;break;
        case Operation::play_anim_fx_set:assert(q->identity==777);break;
        case Operation::regen_hp:if(f.mutation==7){f.attack.mp_leech=513;f.dead=0;}break;
        case Operation::regen_mp:if(f.mutation==8){f.attack.dot_duration=1025;f.attack.dot_amount=2048;f.attack.dot_element=4;}break;
        case Operation::apply_dot:if(f.mutation==9)f.attack.mask|=0x18000000;break;
        case Operation::dodge:if(f.mutation==10)f.attack.outcomes=16;break;
        case Operation::block:break;
        case Operation::hurt:if(f.mutation==11)f.attack.outcomes=128|64|32|256;break;
        case Operation::push:break;
        case Operation::read_property:r->word=f.sheet[q->word0];if(f.mutation==12&&q->word0==140)f.attack.mask|=0x14000;break;
        case Operation::stun:break;
        case Operation::fear:break;
        case Operation::slow:break;
        case Operation::cancel_sneaking:++f.notifications;if(f.mutation==13)f.attack.mask&=~0x20000000u;break;
        case Operation::combat_text:++f.notifications;if(f.mutation==14)f.attack.mask|=0x20000000u;break;
        case Operation::combat_sound:++f.notifications;break;
        case Operation::ai_combat_result:break;
        case Operation::critical_camera:break;
        case Operation::player_by_character:assert(q->subject==f.globals.application&&q->word0==0);r->word=f.player_id;break;
        case Operation::player_stat_increment:assert(q->subject==f.globals.application);break;
        case Operation::player_stat_increase:assert(q->subject==f.globals.application);break;
        case Operation::state_machine_is_idle:assert(q->subject==f.d.identity&&q->word0==0);r->word=f.idle;break;
        case Operation::player_property_add_int:assert(q->subject==f.d.identity);f.sheet[q->word0]+=q->word1;break;
        case Operation::player_property_get_int:assert(q->subject==f.d.identity&&q->word1==0);r->word=f.sheet[q->word0];break;
        case Operation::player_is_local:assert(q->subject==f.globals.application&&q->peer==f.d.identity);r->word=f.local;break;
        case Operation::trophy_lookup:
            assert(q->text);
            if(std::string(q->text)=="epic_invisible")r->word=35;
            else if(std::string(q->text)=="epic_invincible")r->word=34;
            else if(std::string(q->text)=="knocked_down_50_times")r->word=55;
            else assert(false);
            f.trophy_id=r->word;break;
        case Operation::trophy_unlock:assert(q->subject==f.globals.application&&q->word0==f.trophy_id);++f.unlocked;break;
        case Operation::inventory_add_gold:{
            assert(q->subject==f.a.identity);
            auto amount=std::int32_t(q->word0);
            if(amount<0&&f.gold< -amount)amount=-f.gold;
            else if(amount>0){auto room=f.gold_limit-f.gold;if(amount>room)amount=room<0?0:room;}
            auto next=f.gold+amount;f.gold=next>f.gold_limit?f.gold_limit:next;break;
        }
        }
        if(f.nested&&q->operation==Operation::saved_option){Fixture inner;ar::Services s{&inner,invoke};f.nested_ok=ar::execute(&inner.args,&inner.globals,&s,&inner.attack,&inner.report)==ar::Status::complete;}
        return 0;
    }
    static std::int32_t read_gold(void* p,std::uintptr_t character,std::int32_t* value){
        auto& f=*static_cast<Fixture*>(p);if(character!=f.a.identity||!value)return 1;*value=f.gold;return 0;
    }
    static std::int32_t read_factor(void* p,std::uint32_t offset,std::uint32_t* value){
        auto& f=*static_cast<Fixture*>(p);if(!value)return 1;
        if(offset==52u)*value=f.monster_factor;else if(offset==56u)*value=f.player_factor;else return 1;return 0;
    }
    ar::Status run(){ar::Services s{this,invoke,read_gold,read_factor};return ar::execute(&args,&globals,&s,&attack,&report);}
};
void print(const Fixture& f,ar::Status status){
    auto actor=[](const ar::Actor& a){std::cout<<'['<<a.identity<<','<<a.combo_14d0<<','<<unsigned(a.invulnerable_14f0)<<','<<unsigned(a.push_death_53b)<<','<<std::uint32_t(a.remote_update_word_110)<<']';};
    std::cout<<"{\"status\":"<<int(status)<<",\"report\":[";
    std::array<std::uint32_t,sizeof(ar::Result)/4> report{};std::memcpy(report.data(),&f.report,sizeof(f.report));
    for(unsigned i=0;i<report.size();++i){if(i)std::cout<<',';std::cout<<report[i];}
    std::cout<<"],\"attacker\":";actor(f.a);std::cout<<",\"defender\":";actor(f.d);
    std::cout<<",\"dead\":"<<f.dead<<",\"debug\":"<<f.globals.debug_switches<<",\"fx\":"<<f.globals.visual_fx<<",\"result\":[";
    std::array<std::uint32_t,10> words{};std::memcpy(words.data(),&f.attack,40);
    for(unsigned i=0;i<10;++i){if(i)std::cout<<',';std::cout<<words[i];}
    std::cout<<"],\"gold\":"<<f.gold<<",\"trace\":[";for(unsigned i=0;i<f.trace.size();++i){if(i)std::cout<<',';std::cout<<'[';for(unsigned j=0;j<9;++j){if(j)std::cout<<',';std::cout<<f.trace[i].w[j];}std::cout<<']';}std::cout<<"]}\n";
}
int main(int argc,char** argv){
    if(argc>1){assert(argc==24);Fixture f;std::array<std::uint32_t,23>x{};for(unsigned i=0;i<23;++i)x[i]=std::uint32_t(std::stoull(argv[i+1]));
        std::memcpy(&f.attack.amount,&x[0],4);f.attack.outcomes=x[1];f.attack.mask=x[2];std::memcpy(&f.attack.dot_duration,&x[3],4);std::memcpy(&f.attack.dot_amount,&x[4],4);
        std::memcpy(&f.attack.hp_leech,&x[5],4);std::memcpy(&f.attack.mp_leech,&x[6],4);std::memcpy(&f.attack.element,&x[7],4);f.attack.dot_element=3;
        std::memcpy(&f.d.remote_update_word_110,&x[8],4);f.dead=x[9];f.no_damage=x[10];f.god=x[11];f.saved_god=x[12];f.d.invulnerable_14f0=std::uint8_t(x[13]);f.online=x[14];f.coop=x[15];f.per_damage=x[16];f.aggro_return=x[17];f.player=x[18];f.mutation=x[19];
        f.gold=std::int32_t(x[22]);f.monster=x[19]==17;f.sheet[140]=f.sheet[143]=f.sheet[146]=x[20];f.sheet[185]=f.sheet[187]=f.sheet[189]=x[20]^0x80000100u;
        f.sheet[215]=f.sheet[214]=f.sheet[222]=x[20];f.idle=x[19]>=15;f.local=x[19]!=16;
        if(x[21])f.args.attacker=&f.d;
        const auto s=f.run();print(f,s);return 0;
    }
    unsigned behavior=0,guards=0,failures=0;
    for(unsigned mode=0;mode<20;++mode){Fixture f;
        switch(mode){
        case 1:f.args.attacker=&f.d;f.args.defender=&f.d;break;
        case 2:f.attack.amount=0;break;case 3:f.attack.amount=-1;break;
        case 4:f.no_damage=7;break;case 5:f.god=1;break;case 6:f.saved_god=9;break;
        case 7:f.d.invulnerable_14f0=1;break;case 8:f.d.remote_update_word_110=23;break;
        case 9:f.dead=0x80000000u;break;case 10:f.attack.outcomes=511;break;
        case 11:f.attack.mask|=0x200000;break;case 12:f.attack.mask|=0x200000;f.attack.element=-1;break;
        case 13:f.aggro_return=bits(1.f);break;case 14:f.aggro_return=0x7fc00123;break;
        case 15:f.attack.mask=0;break;case 16:f.attack.dot_duration=255;f.attack.dot_amount=1;break;
        case 17:f.attack.outcomes=32;f.sheet[143]=0;break;
        case 18:f.nested=true;break;case 19:f.per_damage=0x80000000;break;
        }
        assert(f.run()==ar::Status::complete);assert(f.report.regenerations==2&&f.report.notifications==3);
        assert(f.trace[f.trace.size()-2].w[0]==unsigned(Operation::is_player));if(f.nested)assert(f.nested_ok);++behavior;
    }
    for(unsigned mutation=1;mutation<=14;++mutation){Fixture f;f.mutation=mutation;f.attack.outcomes=511;f.attack.mask|=0x200000;f.aggro_return=bits(1.f);f.attack.dot_duration=513;f.attack.dot_amount=1024;assert(f.run()==ar::Status::complete);++behavior;}
    {Fixture f;f.player=101;assert(f.run()==ar::Status::complete);assert(f.trace[f.trace.size()-3].w[0]==unsigned(Operation::player_by_character));assert(f.trace.back().w[0]==unsigned(Operation::player_stat_increase));++behavior;}
    {Fixture f;f.player=101;f.attack.amount=0;f.attack.outcomes=0x160;f.dead=1;assert(f.run()==ar::Status::complete);unsigned increments=0;for(const auto& t:f.trace)if(t.w[0]==unsigned(Operation::player_stat_increment)){++increments;assert(t.w[4]==(increments==1?2u:0u)&&t.w[5]==f.player_id);}assert(increments==2);++behavior;}
    {Fixture f;f.player=101;f.attack.outcomes=8;assert(f.run()==ar::Status::complete);unsigned cameras=0;for(const auto& t:f.trace)cameras+=t.w[0]==unsigned(Operation::critical_camera);assert(cameras==1);++behavior;}
    {Fixture f;f.player=202;f.idle=1;f.attack.amount=0;f.attack.outcomes=0;f.sheet[215]=499;assert(f.run()==ar::Status::complete);assert(f.attack.outcomes&2);assert(f.sheet[215]==500&&f.unlocked==1);assert(f.trace[f.trace.size()-2].w[0]==unsigned(Operation::player_stat_increment));assert(f.trace[f.trace.size()-2].w[4]==3);++behavior;}
    {Fixture f;f.player=202;f.idle=1;f.local=0;f.attack.amount=0;f.attack.outcomes=0x80;f.sheet[222]=49;assert(f.run()==ar::Status::complete);assert(f.sheet[222]==50&&f.unlocked==0);++behavior;}
    {Fixture f;f.coop=3;f.monster=true;f.attack.amount=256;assert(f.run()==ar::Status::complete);assert(f.attack.amount==512&&f.report.captured_hit_amount==512);++behavior;}
    {Fixture f;f.coop=3;f.player=f.a.identity;f.player_factor=0x3f400000u;f.attack.amount=512;assert(f.run()==ar::Status::complete);assert(f.attack.amount==512&&f.report.captured_hit_amount==256);++behavior;}
    {Fixture f;f.coop=3;f.attack.amount=256;assert(f.run()==ar::Status::complete);assert(f.attack.amount==256&&f.report.captured_hit_amount==256);++behavior;}
    {Fixture f;f.attack.mask|=0x400000;f.attack.amount=1024;f.gold=3;assert(f.run()==ar::Status::complete);assert(f.attack.amount==768&&f.gold==0&&f.report.captured_hit_amount==768);++behavior;}
    {Fixture f;f.attack.mask|=0x400000;f.attack.amount=512;f.gold=10;assert(f.run()==ar::Status::complete);assert(f.attack.amount==512&&f.gold==8);++behavior;}
    {Fixture f;f.attack.mask|=0x400000;f.attack.amount=-256;f.gold=10;assert(f.run()==ar::Status::complete);assert(f.attack.amount==-256&&f.gold==11);++behavior;}
    {Fixture f;f.attack.mask|=0x400000;f.attack.amount=1024;f.gold=3;assert(f.run()==ar::Status::complete);std::uint32_t at=0;for(std::uint32_t i=0;i<f.trace.size();++i)if(f.trace[i].w[0]==unsigned(Operation::inventory_add_gold))at=i+1;assert(at);Fixture failed;failed.attack.mask|=0x400000;failed.attack.amount=1024;failed.gold=3;failed.fail=at;assert(failed.run()==ar::Status::service_failed);assert(failed.attack.amount==768&&failed.gold==3&&failed.trace.back().w[0]==unsigned(Operation::inventory_add_gold));++behavior;}
    {Fixture f;f.attack.mask|=0x400000;f.attack.amount=1024;f.gold=3;ar::Services missing{&f,Fixture::invoke};assert(ar::execute(&f.args,&f.globals,&missing,&f.attack,&f.report)==ar::Status::service_unavailable);assert(f.attack.amount==1024&&f.gold==3);++behavior;}
    Fixture full;full.attack.outcomes=511;full.attack.mask|=0x200000;full.aggro_return=bits(1.f);full.attack.dot_duration=513;full.attack.dot_amount=1024;assert(full.run()==ar::Status::complete);
    for(std::uint32_t index=1;index<=full.trace.size();++index)for(unsigned throwing=0;throwing<2;++throwing){Fixture f;f.attack=full.attack;f.attack.outcomes=511;f.aggro_return=bits(1.f);if(throwing)f.throw_at=index;else f.fail=index;assert(f.run()==ar::Status::service_failed);assert(f.trace.size()==index&&f.report.calls==index);++failures;}
    auto unchanged=[&](const ar::Arguments* a,const ar::Globals* g,const ar::Services* s,dh2::data::CombatResult* r,ar::Result* out){assert(ar::execute(a,g,s,r,out)==ar::Status::invalid_argument);++guards;};
    Fixture f;ar::Services s{&f,Fixture::invoke};
    unchanged(nullptr,&f.globals,&s,&f.attack,&f.report);unchanged(&f.args,nullptr,&s,&f.attack,&f.report);unchanged(&f.args,&f.globals,nullptr,&f.attack,&f.report);unchanged(&f.args,&f.globals,&s,nullptr,&f.report);unchanged(&f.args,&f.globals,&s,&f.attack,nullptr);
    for(unsigned which=0;which<7;++which){alignas(16)std::array<unsigned char,128>raw{};auto* bad=raw.data()+1;auto a=f.args;const auto* ap=&a;const auto* gp=&f.globals;const auto* sp=&s;auto* rp=&f.attack;auto* op=&f.report;
        switch(which){case 0:ap=reinterpret_cast<const ar::Arguments*>(bad);break;case 1:gp=reinterpret_cast<const ar::Globals*>(bad);break;case 2:sp=reinterpret_cast<const ar::Services*>(bad);break;case 3:rp=reinterpret_cast<dh2::data::CombatResult*>(bad);break;case 4:op=reinterpret_cast<ar::Result*>(bad);break;case 5:a.attacker=reinterpret_cast<ar::Actor*>(bad);break;case 6:a.defender=reinterpret_cast<ar::Actor*>(bad);break;}unchanged(ap,gp,sp,rp,op);}
    auto args=f.args;args.attacker=nullptr;unchanged(&args,&f.globals,&s,&f.attack,&f.report);args=f.args;args.defender=nullptr;unchanged(&args,&f.globals,&s,&f.attack,&f.report);
    f.a.identity=0;unchanged(&f.args,&f.globals,&s,&f.attack,&f.report);f.a.identity=101;
    unchanged(&f.args,&f.globals,&s,&f.attack,reinterpret_cast<ar::Result*>(&f.attack));
    for(unsigned i=0;i<5;++i)for(unsigned j=i+1;j<5;++j){
        const void* controls[]={&f.args,&f.globals,&s,&f.attack,&f.report};controls[j]=controls[i];
        unchanged(static_cast<const ar::Arguments*>(controls[0]),static_cast<const ar::Globals*>(controls[1]),static_cast<const ar::Services*>(controls[2]),const_cast<dh2::data::CombatResult*>(static_cast<const dh2::data::CombatResult*>(controls[3])),const_cast<ar::Result*>(static_cast<const ar::Result*>(controls[4])));
    }
    f.d.identity=f.a.identity;unchanged(&f.args,&f.globals,&s,&f.attack,&f.report);f.d.identity=202;
    {alignas(ar::Actor)std::array<unsigned char,128>memory{};ar::Actor a{101,0,0,0,-1},d{202,0,0,0,-1};std::memcpy(memory.data(),&a,sizeof(a));std::memcpy(memory.data()+alignof(ar::Actor),&d,sizeof(d));args={reinterpret_cast<ar::Actor*>(memory.data()),reinterpret_cast<ar::Actor*>(memory.data()+alignof(ar::Actor)),0};unchanged(&args,&f.globals,&s,&f.attack,&f.report);}
    ar::Services missing{nullptr,nullptr};assert(ar::execute(&f.args,&f.globals,&missing,&f.attack,&f.report)==ar::Status::service_unavailable);assert(f.report.calls==1);++guards;
    // SM_SetKnockBackState(sm,great,attacker,direct): the special (skill/spell) bool is the
    // push request's word2 -- the slot the Character status closure routes through _SetState --
    // and word1 stays zero; a melee hit leaves word2 clear.
    for(unsigned special=0;special<2;++special){Fixture p;p.attack.outcomes=0x80;p.attack.mask|=(special?0x08100000u:0x00100000u);assert(p.run()==ar::Status::complete);
        unsigned pushes=0;for(const auto& t:p.trace)if(t.w[0]==unsigned(Operation::push)){++pushes;assert(t.w[4]==1u&&t.w[5]==0u&&t.w[6]==special);}assert(pushes==1);}
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<behavior<<",\"guard_cases\":"<<guards<<",\"failure_cases\":"<<failures<<",\"mismatches\":0}\n";
}
