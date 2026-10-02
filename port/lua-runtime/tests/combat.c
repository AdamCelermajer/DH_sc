#include "../runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do {if(!(x)) {fprintf(stderr,"combat check line %d: %s\n",__LINE__,#x);return 2;}}while(0)
static int execute(dh2_lua *r,const char *s) {char error[512];int rc=dh2_lua_execute(r,s,strlen(s),10000,error,sizeof(error));if(rc)fprintf(stderr,"combat: %s\n",error);return rc;}
static void word(unsigned char *p,unsigned v) {for(unsigned i=0;i<4;++i)p[i]=(unsigned char)(v>>(8*i));}
int dh2_lua_combat_tests(void) {
    dh2_lua *r=dh2_lua_create(2*1024*1024);CHECK(r);unsigned char props[2700]={0};word(props,3);
    for(unsigned i=0;i<224;++i)word(props+900+i*4,8);
    char error[512];CHECK(!dh2_lua_import_character_properties(r,props,sizeof(props),error,sizeof(error)));
    CHECK(!execute(r,"DH2SeedRandom(0);assert(Rand(100)==49);DH2SeedRandom(0);assert(Rand(10,110)==59);"
        "DH2SeedRandom(0);assert(Rand(0)==0 and Rand(100)==49);"
        "DH2SeedRandom(0);assert(select('#',Rand('100'))==0 and Rand(100)==49);"
        "DH2SeedRandom(0);assert(Rand('unused',nil,false)==49);"
        "DH2SeedRandom(0);assert(pcall(Rand,0/0)==false and Rand(100)==49);"
        "DH2SeedRandom(0);assert(pcall(DH2SeedRandom,1,'bad')==false and Rand(100)==49);"
        "local p=DH2CreatePropertyState(2);assert(p:GetState()==-1 and p:GetHitCount()==0 and p:GetName()=='source character');"
        "p:SetCombatContext(17,65535,'warrior');collectgarbage('collect');"
        "assert(p:GetState()==17 and p:GetHitCount()==65535 and p:GetName()=='warrior');"
        "for _,v in ipairs({-1,65536,0/0,1.5})do assert(pcall(function()p:SetCombatContext(0,v,'changed')end)==false)end;"
        "assert(pcall(function()p:SetCombatContext(0,0,'bad\\000name')end)==false);"
        "assert(pcall(function()p:SetCombatContext(0/0,0,'changed')end)==false);"
        "assert(p:GetState()==17 and p:GetHitCount()==65535 and p:GetName()=='warrior');"
        "assert(p:GetState('ignored')==17 and p:GetHitCount(nil)==65535 and p:GetName(false)=='warrior')"));
    CHECK(!execute(r,"local p=DH2CreatePropertyState(2);p:SetProp(38,25600);p:SetProp(43,5120);p:SetHP(50);p:SetMP(10);"
        "local hp,max,percent=p:GetHP();assert(hp==50 and max==100 and percent==50);"
        "assert(p:GetHPFraction()==.5 and p:GetMPFraction()==.5 and p:GetMP()==10 and p:GetTotalMP()==20);"
        "assert(select('#',p:RegenHP(256))==0);assert(p:GetHP()==51);p:RegenHP(-1);assert(p:GetHP()==100);"
        "p:RegenMP(1);assert(p:GetProp(41)==2561);assert(p:HasMana(2561) and not p:HasMana(2562));"
        "assert(not p:UseMana(2562) and p:GetProp(41)==2561);assert(p:UseMana(256) and p:GetProp(41)==2305);"
        "assert(p:UseMana(-1) and p:GetProp(41)==2306);p:RegenMP(-1);assert(p:GetProp(41)==5120);"
        "p:SetHP(101);p:SetMP(21);p:ValidateHPMP();assert(p:GetHP()==100 and p:GetMP()==20);"
        "p:SetHP(-1);p:ValidateHPMP();assert(p:GetHP()==-1);"
        "assert(select('#',p:RegenHP())==0 and select('#',p:RegenMP('1'))==0);"
        "assert(select('#',p:HasMana(false))==0 and select('#',p:UseMana())==0);"
        "local before=p:GetProp(36);assert(not pcall(function()p:RegenHP(0/0)end));assert(p:GetProp(36)==before);"
        "assert(not pcall(function()p:SetHP(2147483648)end));assert(p:GetProp(36)==before);"
        "p:SetProp(38,1);assert(not pcall(function()return p:GetHP()end));assert(p:GetProp(36)==before);"
        "p:SetProp(38,0);hp,max,percent=p:GetHP();assert(hp==0 and max==0 and percent==0);"
        "assert(p:GetHPFraction()==-1/0);p:SetHP(0);assert(p:GetHPFraction()~=p:GetHPFraction());"
        "p:SetProp(38,25600);p:SetHP(1);assert(p:GetHP()==1 and p:GetTotalHP()==100)"));
    CHECK(!execute(r,"local p=DH2CreatePropertyState(2);p:SetProp(38,25600);p:SetHP(100);"
        "local policy={target_dead=false,target_monster=true,local_player_alive=true,online=false,"
        "manager_present=true,manager_mode=0,monster_invincible=false,force_kill_config=false,force_kill_switch=false,target_network=false};"
        "local processed,death,damage,reason=p:ApplyNonplayerHit(2560,policy,17);"
        "assert(processed and not death and damage==10 and reason==17 and p:GetHP()==90);"
        "processed,death,damage,reason=p:ApplyNonplayerHit(25600,policy,17);"
        "assert(processed and death and damage==100 and reason==3 and p:GetHP()==0);"
        "policy.target_dead=true;p:SetHP(90);processed,death,damage,reason=p:ApplyNonplayerHit(25600,policy,17);"
        "assert(not processed and not death and damage==0 and reason==17 and p:GetHP()==90);"
        "policy.target_dead=false;policy.local_player_alive=false;processed,death,damage,reason=p:ApplyNonplayerHit(2560,policy,17);"
        "assert(processed and not death and damage==0 and reason==17 and p:GetHP()==90);"
        "policy.local_player_alive=true;policy.monster_invincible=true;p:ApplyNonplayerHit(2560,policy,17);assert(p:GetHP()==90);"
        "policy.monster_invincible=false;policy.online=true;policy.manager_mode=2;p:ApplyNonplayerHit(2560,policy,17);assert(p:GetHP()==90);"
        "policy.manager_mode=5;p:ApplyNonplayerHit(2560,policy,17);assert(p:GetHP()==80);"
        "policy.target_network=true;policy.force_kill_config=true;processed,death,damage,reason=p:ApplyNonplayerHit(0,policy,17);"
        "assert(processed and death and damage==0 and reason==17 and p:GetHP()==0);"
        "p:SetHP(50);local before=p:GetProp(36);"
        "for _,v in ipairs({-1,4294967296,1/0,0/0,'10'})do assert(not pcall(function()p:ApplyNonplayerHit(v,policy,17)end));assert(p:GetProp(36)==before)end;"
        "policy.manager_mode=1.5;assert(not pcall(function()p:ApplyNonplayerHit(0,policy,17)end));assert(p:GetProp(36)==before);"
        "policy.manager_mode=0;policy.target_dead=0;assert(not pcall(function()p:ApplyNonplayerHit(0,policy,17)end));assert(p:GetProp(36)==before);"
        "policy.target_dead=false;assert(not pcall(function()p:ApplyNonplayerHit(0,policy,1.5)end));assert(p:GetProp(36)==before);"
        "assert(not pcall(function()p:ApplyNonplayerHit(0,{},17)end));assert(p:GetProp(36)==before);"
        "policy.online=false;policy.force_kill_config=false;policy.target_network=false;p:ApplyNonplayerHit(256,policy,17);assert(p:GetHP()==49)"));
    CHECK(!execute(r,"local p=DH2CreatePropertyState(2);assert(not p:IsDead());p:SetProp(38,25600);p:SetHP(10);p:SetProp(9,123);"
        "local ctx={dead=false,network=false,suppress_events=false,target_id=734,property_id=5,template_id=7};"
        "local policy={forced=false,loot_manager_present=false,kill_enemies=0,clear_enemies=1,kill_template=10,clear_template=11};"
        "p:SetDeathContext(ctx);local r=p:KillNonplayer(policy);assert(r.processed and r.dead and r.drop_loot_requested and r.drop_loot_id==123);"
        "assert(p:IsDead() and p:GetHP()==0 and #r.events==4);"
        "for i,e in ipairs(r.events)do assert(e.kind==i-1 and e.target_id==734 and e.match_id==(i<3 and 5 or 7))end;"
        "assert(r.events[1].objective_id==0 and r.events[2].objective_id==1 and r.events[3].objective_id==10 and r.events[4].objective_id==11);"
        "p:SetHP(10);r=p:KillNonplayer(policy);assert(not r.processed and r.dead and not r.drop_loot_requested and #r.events==0 and p:GetHP()==10);"
        "ctx.template_id=-1;p:SetDeathContext(ctx);policy.loot_manager_present=true;r=p:KillNonplayer(policy);"
        "assert(r.processed and r.dead and not r.drop_loot_requested and #r.events==2 and p:GetHP()==0);"
        "ctx.network=true;p:SetDeathContext(ctx);r=p:KillNonplayer(policy);assert(r.processed and #r.events==0);"
        "ctx.network=false;ctx.suppress_events=true;p:SetDeathContext(ctx);r=p:KillNonplayer(policy);assert(r.processed and #r.events==0);"
        "ctx.suppress_events=false;p:SetDeathContext(ctx);policy.forced=true;policy.loot_manager_present=false;"
        "r=p:KillNonplayer(policy);assert(r.processed and r.drop_loot_requested and #r.events==0);"
        "ctx.target_id=4294967040;p:SetDeathContext(ctx);policy.forced=false;r=p:KillNonplayer(policy);assert(r.events[1].target_id==4294967040);"
        "ctx.target_id=734;p:SetDeathContext(ctx);p:SetHP(10);local before=p:GetProp(36);"
        "ctx.property_id=32768;assert(not pcall(function()p:SetDeathContext(ctx)end));assert(not p:IsDead() and p:GetProp(36)==before);"
        "ctx.property_id=5;ctx.target_id=-1;assert(not pcall(function()p:SetDeathContext(ctx)end));assert(not p:IsDead() and p:GetProp(36)==before);"
        "ctx.target_id=734;policy.kill_enemies=0/0;assert(not pcall(function()p:KillNonplayer(policy)end));assert(not p:IsDead() and p:GetProp(36)==before);"
        "policy.kill_enemies=0;policy.forced=0;assert(not pcall(function()p:KillNonplayer(policy)end));assert(not p:IsDead() and p:GetProp(36)==before);"
        "policy.forced=false;assert(not pcall(function()p:KillNonplayer({})end));assert(not p:IsDead() and p:GetProp(36)==before);"
        "collectgarbage('collect');r=p:KillNonplayer(policy);assert(r.processed and p:IsDead() and p:GetHP()==0)"));
    CHECK(!execute(r,"local p=DH2CreatePropertyState(2);p:SetProp(38,25600);"
        "local ctx={dead=false,network=false,suppress_events=false,target_id=734,property_id=5,template_id=7};"
        "local policy={forced=false,loot_manager_present=true,kill_enemies=0,clear_enemies=1,kill_template=10,clear_template=11};local qs={};"
        "for k=0,3 do qs[k+1]=DH2CreateKillObjective{kind=k,match_id=k<2 and 5 or 7,current=0,required=2,completed=false}end;"
        "for kill=1,3 do p:SetDeathContext(ctx);p:SetHP(10);local d=p:KillNonplayer(policy);"
        "for _,q in ipairs(qs)do for _,e in ipairs(d.events)do local r=q:ConsumeKillEvent(e);"
        "if r.matched then assert(r.changed and r.progress.current==kill and r.event.quantity==kill and r.event.outbound);"
        "assert(r.completion_requested==(kill>=2) and r.newly_completed==(kill==2))end end;"
        "local v=q:GetProgress();assert(v.current==kill and v.completed==(kill>=2))end;"
        "assert(#p:KillNonplayer(policy).events==0)end;"
        "local q=qs[1];local e={kind=0,match_id=5,synchronized=true,quantity=2,outbound=false};"
        "local v=q:ConsumeKillEvent(e);assert(v.matched and not v.changed and not v.completion_requested and v.progress.current==3);"
        "e.quantity=8;v=q:ConsumeKillEvent(e);assert(v.changed and v.completion_requested and not v.newly_completed and v.progress.current==8 and not v.event.outbound);"
        "assert(e.quantity==8 and e.outbound==false);"
        "e.match_id=6;v=q:ConsumeKillEvent(e);assert(not v.matched and not v.changed and v.progress.current==8);e.match_id=5;e.kind=1;"
        "v=q:ConsumeKillEvent(e);assert(not v.matched and q:GetProgress().current==8);e.kind=0;"
        "for _,bad in ipairs({0/0,1/0,1.5,2147483648,'8'})do e.quantity=bad;assert(not pcall(function()q:ConsumeKillEvent(e)end));assert(q:GetProgress().current==8)end;"
        "e.quantity=9;e.synchronized=0;assert(not pcall(function()q:ConsumeKillEvent(e)end));assert(q:GetProgress().current==8);"
        "assert(not pcall(function()q:ConsumeKillEvent({})end));assert(not pcall(function()DH2CreateKillObjective{}end));"
        "local thrown=false;local raw=setmetatable({kind=0,match_id=5},{__index=function()thrown=true;error('metamethod')end});"
        "v=q:ConsumeKillEvent(raw);assert(v.changed and v.progress.current==9 and not thrown);collectgarbage('collect');assert(q:GetProgress().current==9)"));
    unsigned char quests[209]={0};word(quests,1);word(quests+24,1);
    word(quests+32,0xffffffffu);word(quests+36,0xffffffffu);
    word(quests+48,5);word(quests+52,0xffffffffu);word(quests+56,2);
    CHECK(!dh2_lua_import_quests(r,quests,sizeof(quests),error,sizeof(error)));
    CHECK(!execute(r,"assert(DH2GetQuestCount()==1);local row=DH2GetQuestRecord(0);"
        "assert(#row.ids==4 and #row.conditions==0 and #row.objectives==1 and #row.scripts==14);"
        "assert(row.objectives[1].common[1]==0 and row.objectives[1].args[1]==5 and row.objectives[1].args[3]==2);"
        "assert(row.objectives[1].strings[1]=='' and row.scripts[14]=='');"
        "quest_generation_keeper=DH2CreateQuestKillObjective(0,0,0,false);"
        "local q=quest_generation_keeper;local r=q:ConsumeKillEvent{kind=0,match_id=5};assert(r.changed and r.progress.current==1 and not r.progress.completed);"
        "assert(not pcall(function()DH2GetQuestRecord(1)end));assert(not pcall(function()DH2GetQuestRecord('0')end));"
        "assert(not pcall(function()DH2CreateQuestKillObjective(0,1,0,false)end));assert(not pcall(function()DH2CreateQuestKillObjective(0,0,0,0)end))"));
    word(quests+48,9);word(quests+56,3);
    CHECK(!dh2_lua_import_quests(r,quests,sizeof(quests),error,sizeof(error)));
    CHECK(dh2_lua_import_quests(r,quests,sizeof(quests)-1,error,sizeof(error)));
    CHECK(!execute(r,"collectgarbage('collect');local row=DH2GetQuestRecord(0);assert(row.objectives[1].args[1]==9);"
        "local q=quest_generation_keeper;local r=q:ConsumeKillEvent{kind=0,match_id=5};"
        "assert(r.changed and r.newly_completed and r.progress.current==2 and r.progress.required==2);"
        "local fresh=DH2CreateQuestKillObjective(0,0,0,false);assert(fresh:GetProgress().match_id==9 and fresh:GetProgress().required==3);"
        "assert(not fresh:ConsumeKillEvent{kind=0,match_id=5}.matched);"
        "assert(fresh:ConsumeKillEvent{kind=0,match_id=9}.progress.current==1)"));
    word(quests+28,1);CHECK(!dh2_lua_import_quests(r,quests,sizeof(quests),error,sizeof(error)));
    CHECK(!execute(r,"assert(not pcall(function()DH2CreateQuestKillObjective(0,0,0,false)end));"
        "assert(quest_generation_keeper:GetProgress().current==2)"));
    CHECK(!execute(r,"local chars={{property_id=5,template_id=7},{present=false,property_id=5,template_id=7}};"
        "local cached={{property_id=5,quantity=9},{property_id=8,quantity=-2}};"
        "quest_world_keeper=DH2CreateQuestWorld(7,chars,cached);local w=quest_world_keeper;"
        "assert(w:GetLevel()==7 and w:GetPopulation(0,5)==1 and w:GetPopulation(1,7)==1);"
        "assert(w:GetPopulation(0,8)==-2 and w:GetPopulation(1,8)==0);"
        "chars[1].property_id=8;cached[1].quantity=1;collectgarbage('collect');assert(w:GetPopulation(0,5)==1);"
        "assert(not pcall(function()DH2CreateQuestWorld(0,{},{{property_id=1,quantity=0},{property_id=1,quantity=2}})end));"
        "assert(not pcall(function()DH2CreateQuestWorld(0,{{property_id=32768,template_id=1}},{})end));"
        "assert(not pcall(function()DH2CreateQuestWorld(0,{{present=1,property_id=5,template_id=7}},{})end));"
        "assert(not pcall(function()w:GetPopulation(2,5)end));assert(not pcall(function()w:GetPopulation(0,0/0)end));"
        "assert(w:GetPopulation(0,5)==1)"));
    for(unsigned kind=0;kind<4;++kind) {
        word(quests+28,kind<2?kind:kind+8);word(quests+48,kind&1?7:5);word(quests+52,kind&1?5:7);word(quests+56,2);
        CHECK(!dh2_lua_import_quests(r,quests,sizeof(quests),error,sizeof(error)));
        char source[4096];snprintf(source,sizeof(source),
            "local kind=%u;local w=DH2CreateQuestWorld(7,{{property_id=5,template_id=5},{property_id=5,template_id=5}},{});"
            "local q,c=DH2CreateCompiledQuestObjective(0,0,2,false,false,w);compiled_generation_keeper=q;"
            "assert(c.eligible and c.required_updated and c.completion_requested and c.newly_completed);"
            "local p=q:GetProgress();assert(p.compiled_record and p.active and p.completed and p.required==2 and p.kind==kind and p.match_id==5);"
            "local mismatch=DH2CreateQuestWorld(9,{},{});c=q:CompileAgainst(mismatch);"
            "assert(not c.eligible and not c.completion_requested and c.required_updated==(kind%%2==0));"
            "assert(c.progress.active==(kind%%2==1) and c.progress.required==2 and c.progress.completed);"
            "local empty=DH2CreateQuestWorld(7,{},{});c=q:CompileAgainst(empty);"
            "assert(not c.eligible and c.required_updated and c.progress.required==(kind%%2==0 and 2 or 0));"
            "assert(c.progress.active==(kind%%2==1));c=q:CompileAgainst(w);"
            "assert(c.eligible and c.progress.active and c.progress.required==2 and c.completion_requested and not c.newly_completed);"
            "local e={kind=kind,match_id=5};local v=q:ConsumeKillEvent(e);assert(v.changed and v.progress.current==3 and v.completion_requested);"
            "assert(not pcall(function()q:CompileAgainst({})end));assert(q:GetProgress().current==3);"
            "assert(not pcall(function()DH2CreateCompiledQuestObjective(0,0,0,0,false,w)end));"
            "assert(not pcall(function()DH2CreateCompiledQuestObjective(0,0,0,false,false,{})end));"
            "collectgarbage('collect');assert(q:GetProgress().current==3)",kind);
        CHECK(!execute(r,source));
    }
    word(quests+28,4);CHECK(!dh2_lua_import_quests(r,quests,sizeof(quests),error,sizeof(error)));
    CHECK(!execute(r,"assert(not pcall(function()DH2CreateCompiledQuestObjective(0,0,0,false,false,quest_world_keeper)end));"
        "collectgarbage('collect');local r=compiled_generation_keeper:CompileAgainst(DH2CreateQuestWorld(7,{{property_id=5,template_id=5}},{}));"
        "assert(r.eligible and r.progress.kind==3 and r.progress.required==1 and r.progress.current==3 and r.completion_requested and not r.newly_completed)"));
    dh2_lua_destroy(r);return 0;
}
static int file(dh2_lua *r,const char *path,int kind) {
    FILE *f=fopen(path,"rb");CHECK(f);CHECK(!fseek(f,0,SEEK_END));long n=ftell(f);CHECK(n>=0 && n<=4*1024*1024);rewind(f);
    unsigned char *bytes=malloc(n?n:1);CHECK(bytes);CHECK(fread(bytes,1,n,f)==(size_t)n);CHECK(!fclose(f));char error[512];int rc;
    if(kind==0)rc=dh2_lua_import_character_properties(r,bytes,n,error,sizeof(error));
    else if(kind==1)rc=dh2_lua_import_loot_tables(r,bytes,n,error,sizeof(error));
    else if(kind==2)rc=dh2_lua_import_constants(r,bytes,n,error,sizeof(error));
    else if(kind==4)rc=dh2_lua_import_quests(r,bytes,n,error,sizeof(error));
    else rc=dh2_lua_execute(r,bytes,n,10000,error,sizeof(error));
    free(bytes);if(rc)fprintf(stderr,"combat file %s: %s\n",path,error);CHECK(!rc);return 0;
}
static int listing(dh2_lua *r,const char *path,int kind,unsigned *count) {
    FILE *f=fopen(path,"rb");CHECK(f);char line[2048];*count=0;
    while(fgets(line,sizeof(line),f)) {
        size_t n=strlen(line);CHECK(n && line[n-1]=='\n');line[--n]=0;if(n && line[n-1]=='\r')line[--n]=0;
        CHECK(n && !file(r,line,kind));++*count;
    }
    CHECK(!ferror(f));CHECK(!fclose(f));return 0;
}
int dh2_lua_combat_corpus(const char *props,const char *loot,const char *constants,const char *scripts) {
    dh2_lua *r=dh2_lua_create(8*1024*1024);CHECK(r);CHECK(!file(r,props,0) && !file(r,loot,1));unsigned imported,executed;
    CHECK(!listing(r,constants,2,&imported));CHECK(!listing(r,scripts,3,&executed));dh2_lua_destroy(r);
    printf("COMBAT CORPUS PASS %u %u\n",imported,executed);return 0;
}
int dh2_lua_quest_corpus(const char *props,const char *loot,const char *constants,const char *quests,const char *scripts) {
    dh2_lua *r=dh2_lua_create(8*1024*1024);CHECK(r);CHECK(!file(r,props,0) && !file(r,loot,1));unsigned imported,executed;
    CHECK(!listing(r,constants,2,&imported));CHECK(!file(r,quests,4));CHECK(!listing(r,scripts,3,&executed));dh2_lua_destroy(r);
    printf("QUEST CORPUS PASS %u %u\n",imported,executed);return 0;
}
