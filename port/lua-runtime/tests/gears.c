#include "../runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do {if(!(x)) {fprintf(stderr,"gear Lua check line %d: %s\n",__LINE__,#x);return 2;}}while(0)
static void word(unsigned char *p,unsigned v) {for(unsigned i=0;i<4;++i)p[i]=(unsigned char)(v>>(8*i));}
static int execute(dh2_lua *r,const char *s) {char error[512];int rc=dh2_lua_execute(r,s,strlen(s),10000,error,sizeof(error));if(rc)fprintf(stderr,"gear script: %s\n",error);return rc;}
static void fixture(unsigned char props[2700],unsigned char items[479],unsigned char powers[90]) {
    memset(props,0,2700);word(props,3);for(unsigned i=0;i<224;++i)word(props+900+i*4,4);
    word(props+1796+79*4,7);word(props+1796+80*4,8);
    memset(items,0,479);word(items+12,3);
    unsigned char *tail=items+85;word(tail+4,0);word(tail+14*4,10);word(tail+15*4,20);
    tail+=149;word(tail+4,6);word(tail+14*4,5);word(tail+15*4,2);
    tail+=149;word(tail+4,7);word(tail+14*4,3);
    memset(powers,0,90);word(powers+4,2);
    for(unsigned i=0;i<2;++i) {unsigned char *p=powers+8+i*41;word(p+5,1);word(p+9,i?9:28);word(p+13,i?256:4);}
}
int dh2_lua_gear_tests(void) {
    unsigned char props[2700],items[479],powers[90],classes[8]={1,0,0,0,0,0,0,0};fixture(props,items,powers);char error[512];
    dh2_lua *r=dh2_lua_create(2*1024*1024);CHECK(r);
    CHECK(!dh2_lua_import_character_properties(r,props,sizeof(props),error,sizeof(error)));
    CHECK(!execute(r,"beforeClass=DH2CreatePropertyState(2);assert(pcall(function()beforeClass:UpdateBaseProperties(2)end)==false)"));
    CHECK(!dh2_lua_import_character_classes(r,classes,sizeof(classes),error,sizeof(error)));
    CHECK(!dh2_lua_import_loot_tables(r,items,sizeof(items),error,sizeof(error)));
    CHECK(!execute(r,"before=DH2CreatePropertyState(2);before:EquipItem(0,1,0);assert(pcall(function()before:UpdateGearsProperties()end)==false);"
        "assert(pcall(function()before:SetItemPowers(0,1,0)end)==false);assert(pcall(function()beforeClass:UpdateBaseProperties(2)end)==false)"));
    CHECK(!dh2_lua_import_item_powers(r,powers,sizeof(powers),error,sizeof(error)));memset(powers,0,sizeof(powers));memset(items,0,sizeof(items));
    CHECK(!execute(r,"old=DH2CreatePropertyState(2);old:EquipItem(0,1,0);old:EquipGear(0,2,1);old:EquipGear(0,8,2);"
        "old:SetItemPowers(0,1,0,1);old:UpdateGearsProperties();"
        "assert(old:GetProp(79,false)==21 and old:GetProp(80,false)==32 and old:GetProp(97,false)==256);"
        "assert(old:GetProp(71,false)==8 and old:GetProp(61,false)==2 and old:HasShield());"
        "old:UpdateGearsProperties();assert(old:GetProp(79,false)==21);old:RecalculateProperties(false);assert(old:GetProp(79,false)==21);"
        "assert(pcall(function()old:SetItemPowers(0,1,0,2)end)==false);old:UpdateGearsProperties();assert(old:GetProp(79,false)==21);"
        "assert(pcall(function()old:SetItemPowers(0,1,'0')end)==false);assert(pcall(function()old:SetItemPowers(0,1,0/0)end)==false);"
        "assert(pcall(function()old:EquipGear(0,16,0)end)==false);assert(pcall(function()old:RecalculateProperties(1)end)==false);"
        "assert(pcall(function()old:UpdateBaseProperties(0/0)end)==false);assert(pcall(function()old:UpdateGearsProperties(1)end)==false);"
        "assert(old:GetProp(79,false)==21);old:UpdateBaseProperties(-1);assert(old:GetProp(79,false)==14);old:UpdateBaseProperties(2);assert(old:GetProp(79,false)==21);"
        "old:EquipGear(1,1,0);old:SelectEquipmentSet(1);old:UpdateGearsProperties();assert(old:GetProp(79,false)==17 and old:GetProp(71,false)==3);"
        "old:SelectEquipmentSet(0);old:SetItemPowers(0,1);old:UpdateGearsProperties();assert(old:GetProp(79,false)==17);"
        "old:SetItemPowers(0,1,0);old:UpdateGearsProperties();assert(old:GetProp(79,false)==21)"));
    CHECK(dh2_lua_import_item_powers(r,powers,1,error,sizeof(error))!=0);
    fixture(props,items,powers);word(powers+21,12);CHECK(!dh2_lua_import_item_powers(r,powers,sizeof(powers),error,sizeof(error)));memset(powers,0,sizeof(powers));
    CHECK(!execute(r,"fresh=DH2CreatePropertyState(2);fresh:EquipGear(0,1,0);fresh:SetItemPowers(0,1,0);fresh:UpdateGearsProperties();assert(fresh:GetProp(79,false)==29);"
        "old:UpdateGearsProperties();assert(old:GetProp(79,false)==21);collectgarbage('collect');old:UpdateGearsProperties();assert(old:GetProp(79,false)==21);"
        "assert(pcall(function()before:UpdateGearsProperties()end)==false);assert(pcall(function()fresh:EquipGear(0,1,3)end)==false);"
        "fresh:UpdateGearsProperties();assert(fresh:GetProp(79,false)==29);fresh:EquipItem(0,1,0);fresh:UpdateGearsProperties();assert(fresh:GetProp(79,false)==17)"));
    dh2_lua_destroy(r);r=dh2_lua_create(256*1024);CHECK(r);fixture(props,items,powers);
    CHECK(!dh2_lua_import_character_properties(r,props,sizeof(props),error,sizeof(error)));
    CHECK(!dh2_lua_import_character_classes(r,classes,sizeof(classes),error,sizeof(error)));
    CHECK(!dh2_lua_import_loot_tables(r,items,sizeof(items),error,sizeof(error)));
    CHECK(!dh2_lua_import_item_powers(r,powers,sizeof(powers),error,sizeof(error)));
    size_t n=8+10000*29;unsigned char *big=calloc(1,n);CHECK(big);word(big+4,10000);
    CHECK(dh2_lua_import_item_powers(r,big,n,error,sizeof(error))!=0 && strstr(error,"memory"));free(big);
    CHECK(!execute(r,"collectgarbage('collect');local c=DH2CreatePropertyState(2);c:EquipItem(0,1,0);c:SetItemPowers(0,1,0);c:UpdateGearsProperties();assert(c:GetProp(79,false)==21)"));
    CHECK(dh2_lua_memory_used(r)<=256*1024);dh2_lua_destroy(r);return 0;
}
static int import_file(dh2_lua *r,const char *path,int kind) {
    FILE *f=fopen(path,"rb");CHECK(f);CHECK(!fseek(f,0,SEEK_END));long n=ftell(f);CHECK(n>=0 && n<=4*1024*1024);rewind(f);
    unsigned char *p=malloc(n?n:1);CHECK(p);CHECK(fread(p,1,n,f)==(size_t)n);CHECK(!fclose(f));char error[512];
    int rc=kind==0?dh2_lua_import_character_properties(r,p,n,error,sizeof(error)):kind==1?dh2_lua_import_character_classes(r,p,n,error,sizeof(error)):kind==2?dh2_lua_import_loot_tables(r,p,n,error,sizeof(error)):dh2_lua_import_item_powers(r,p,n,error,sizeof(error));
    free(p);if(rc)fprintf(stderr,"gear import: %s\n",error);CHECK(!rc);return 0;
}
int dh2_lua_gear_corpus(const char *props,const char *classes,const char *items,const char *powers,const char *listing) {
    dh2_lua *r=dh2_lua_create(8*1024*1024);CHECK(r);const char *paths[]={props,classes,items,powers};
    for(int i=0;i<4;++i)CHECK(!import_file(r,paths[i],i));
    FILE *f=fopen(listing,"rb");CHECK(f);char path[2048],error[512];unsigned count=0;
    while(fgets(path,sizeof(path),f)) {
        size_t n=strlen(path);CHECK(n && path[n-1]=='\n');path[--n]=0;if(n && path[n-1]=='\r')path[--n]=0;
        FILE *src=fopen(path,"rb");CHECK(src);CHECK(!fseek(src,0,SEEK_END));long size=ftell(src);CHECK(size>=0 && size<=1024*1024);rewind(src);
        char *bytes=malloc(size?size:1);CHECK(bytes);CHECK(fread(bytes,1,size,src)==(size_t)size);CHECK(!fclose(src));
        int rc=dh2_lua_execute(r,bytes,size,10000,error,sizeof(error));free(bytes);if(rc)fprintf(stderr,"gear corpus %u: %s\n",count,error);CHECK(!rc);++count;
    }
    CHECK(!ferror(f));CHECK(!fclose(f));dh2_lua_destroy(r);printf("GEAR CORPUS PASS %u\n",count);return 0;
}
