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
