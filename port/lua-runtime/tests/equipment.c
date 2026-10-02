#include "../runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"equipment Lua check line %d: %s\n",__LINE__,#x);return 2; } }while(0)
static void put(unsigned char *p,unsigned value) { for(unsigned i=0;i<4;++i)p[i]=(unsigned char)(value>>(8*i)); }
static int execute(dh2_lua *r,const char *source) {
    char error[512];int status=dh2_lua_execute(r,source,strlen(source),10000,error,sizeof(error));
    if(status)fprintf(stderr,"equipment script: %s\n",error);return status;
}
static void properties(unsigned char props[2700]) {
    memset(props,0,2700);put(props,3);
    for(unsigned field=0x33;field<0x5c;++field)put(props+900+4*field,16);
    for(unsigned kind=0;kind<7;++kind) {
        put(props+1796+4*(0x33+kind),(kind+1)*256);put(props+1796+4*(0x40+kind),(kind+1)*512);put(props+1796+4*(0x53+kind),(kind+1)*1024);
    }
    put(props+1796+4*0x3a,2048);put(props+1796+4*0x5a,4096);put(props+1796+4*0x5b,8192);
}
static void loot(unsigned char raw[479]) {
    memset(raw,0,479);put(raw+12,3);
    for(unsigned i=0;i<3;++i) {
        unsigned char *tail=raw+16+i*149+69;
        put(tail+4,i==1?6:i==2?4:0);put(tail+20,i==2?-4u:i==1?2:1);put(tail+64,i);
    }
}
int dh2_lua_equipment_tests(void) {
    unsigned char props[2700],raw[479];properties(props);loot(raw);char error[512];
    dh2_lua *r=dh2_lua_create(2*1024*1024);CHECK(r);
    CHECK(!dh2_lua_import_character_properties(r,props,sizeof(props),error,sizeof(error)));
    CHECK(!execute(r,"before=DH2CreatePropertyState(2);assert(before:HasShield()==false);"
        "assert(select('#',before:GetDamageBonus())==0);assert(before:GetDamageBonus(false)==0);"
        "assert(pcall(function()before:EquipItem(0,1,0)end)==false)"));
    CHECK(!dh2_lua_import_loot_tables(r,raw,sizeof(raw),error,sizeof(error)));memset(raw,0,sizeof(raw));
    CHECK(!execute(r,"old=DH2CreatePropertyState(2);old:EquipItem(0,1,0);old:EquipItem(0,2,1);"
        "assert(old:HasShield()==true and old:HasShield(999)==true);"
        "assert(old:GetAttackRatingBonus(false)==256 and old:GetAttackRatingBonus(true)==512);"
        "assert(old:GetAttackRatingBonus(0)==256 and old:GetAttackRatingBonus(-1)==512);"
        "assert(old:GetAttackRatingBonus(nil)==256 and old:GetAttackRatingBonus(false,'ignored')==256);"
        "assert(old:GetAttackRatingBonus(0/0)==512);assert(old:GetCritRatingBonus(false)==512);"
        "assert(old:GetDamageBonus(false)==1024 and old:GetDamageBonus(true)==2048);"
        "assert(pcall(function()old:GetDamageBonus('false')end)==false);"
        "old:EquipItem(1,1,2);old:EquipItem(1,2,0);old:SelectEquipmentSet(1);"
        "assert(old:HasShield()==false);assert(old:GetAttackRatingBonus(false)==2816);"
        "assert(old:GetDamageBonus(false)==15360 and old:GetDamageBonus(true)==13312);"
        "assert(pcall(function()before:EquipItem(0,1,0)end)==false);"
        "assert(pcall(function()old:EquipItem(0,1,3)end)==false);"
        "assert(pcall(function()old:EquipItem(0,1,0/0)end)==false);"
        "assert(pcall(function()old:EquipItem(0,3,0)end)==false);"
        "assert(pcall(function()old:EquipItem('0',1,0)end)==false);"
        "assert(pcall(function()old:SelectEquipmentSet(2)end)==false);"
        "assert(pcall(function()old:SelectEquipmentSet(0/0)end)==false);"
        "assert(old:GetDamageBonus(false)==15360);old:EquipItem(1,2,-1);"
        "assert(old:GetDamageBonus(false)==11264 and old:GetDamageBonus(true)==0)"));
    CHECK(dh2_lua_import_loot_tables(r,raw,1,error,sizeof(error))!=0);
    CHECK(!execute(r,"old:EquipItem(1,2,0);assert(old:GetDamageBonus(false)==15360)"));
    loot(raw);put(raw+16+69+4,6);CHECK(!dh2_lua_import_loot_tables(r,raw,sizeof(raw),error,sizeof(error)));memset(raw,0,sizeof(raw));
    CHECK(!execute(r,"new=DH2CreatePropertyState(2);new:EquipItem(1,1,2);new:EquipItem(1,2,0);new:SelectEquipmentSet(1);"
        "assert(new:HasShield()==true and new:GetDamageBonus(false)==11264);"
        "old:EquipItem(1,2,0);assert(old:HasShield()==false and old:GetDamageBonus(false)==15360);"
        "collectgarbage('collect');old:SelectEquipmentSet(0);assert(old:HasShield()==true)"));
    dh2_lua_destroy(r);
    r=dh2_lua_create(256*1024);CHECK(r);loot(raw);
    CHECK(!dh2_lua_import_character_properties(r,props,sizeof(props),error,sizeof(error)));
    CHECK(!dh2_lua_import_loot_tables(r,raw,sizeof(raw),error,sizeof(error)));
    size_t size=32+5000*149;unsigned char *large=calloc(1,size);CHECK(large);put(large+12,5000);
    CHECK(dh2_lua_import_loot_tables(r,large,size,error,sizeof(error))!=0 && strstr(error,"memory"));free(large);
    CHECK(!execute(r,"collectgarbage('collect');local c=DH2CreatePropertyState(2);c:EquipItem(0,2,1);assert(c:HasShield()==true)"));
    CHECK(dh2_lua_memory_used(r)<=256*1024);dh2_lua_destroy(r);return 0;
}
static int import_file(dh2_lua *r,const char *path,int items) {
    FILE *f=fopen(path,"rb");CHECK(f);CHECK(!fseek(f,0,SEEK_END));long size=ftell(f);CHECK(size>=0 && size<=4*1024*1024);rewind(f);
    unsigned char *bytes=malloc(size?size:1);CHECK(bytes);CHECK(fread(bytes,1,size,f)==(size_t)size);CHECK(!fclose(f));char error[512];
    int status=items?dh2_lua_import_loot_tables(r,bytes,size,error,sizeof(error)):dh2_lua_import_character_properties(r,bytes,size,error,sizeof(error));
    free(bytes);if(status)fprintf(stderr,"equipment import: %s\n",error);CHECK(!status);return 0;
}
int dh2_lua_equipment_corpus(const char *properties_path,const char *items,const char *listing) {
    dh2_lua *r=dh2_lua_create(8*1024*1024);CHECK(r);CHECK(!import_file(r,properties_path,0));CHECK(!import_file(r,items,1));
    FILE *f=fopen(listing,"rb");CHECK(f);char path[2048],error[512];unsigned count=0;
    while(fgets(path,sizeof(path),f)) {
        size_t n=strlen(path);CHECK(n && path[n-1]=='\n');path[--n]=0;if(n && path[n-1]=='\r')path[--n]=0;
        FILE *source=fopen(path,"rb");CHECK(source);CHECK(!fseek(source,0,SEEK_END));long size=ftell(source);CHECK(size>=0 && size<=1024*1024);rewind(source);
        char *bytes=malloc(size?size:1);CHECK(bytes);CHECK(fread(bytes,1,size,source)==(size_t)size);CHECK(!fclose(source));
        int status=dh2_lua_execute(r,bytes,size,10000,error,sizeof(error));free(bytes);if(status)fprintf(stderr,"equipment corpus %u: %s\n",count,error);CHECK(!status);++count;
    }
    CHECK(!ferror(f));CHECK(!fclose(f));dh2_lua_destroy(r);printf("EQUIPMENT CORPUS PASS %u\n",count);return 0;
}
