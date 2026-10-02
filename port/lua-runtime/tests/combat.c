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
    dh2_lua_destroy(r);return 0;
}
static int file(dh2_lua *r,const char *path,int kind) {
    FILE *f=fopen(path,"rb");CHECK(f);CHECK(!fseek(f,0,SEEK_END));long n=ftell(f);CHECK(n>=0 && n<=4*1024*1024);rewind(f);
    unsigned char *bytes=malloc(n?n:1);CHECK(bytes);CHECK(fread(bytes,1,n,f)==(size_t)n);CHECK(!fclose(f));char error[512];int rc;
    if(kind==0)rc=dh2_lua_import_character_properties(r,bytes,n,error,sizeof(error));
    else if(kind==1)rc=dh2_lua_import_loot_tables(r,bytes,n,error,sizeof(error));
    else if(kind==2)rc=dh2_lua_import_constants(r,bytes,n,error,sizeof(error));
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
