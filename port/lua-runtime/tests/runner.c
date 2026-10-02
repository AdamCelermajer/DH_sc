#include "../runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"check failed at line %d: %s\n",__LINE__,#x);return 2; } } while(0)
int dh2_lua_numeric_tests(void);
int dh2_lua_constant_corpus(const char *,const char *);
int dh2_lua_name_corpus(const char *,const char *);
int dh2_lua_execution_corpus(const char *,const char *);
int dh2_lua_property_tests(void);
int dh2_lua_property_corpus(const char *,const char *);
static void put32(unsigned char *p,unsigned value) {
    for(unsigned i=0;i<4;++i)p[i]=(unsigned char)(value>>(8*i));
}
int main(int argc,char **argv) {
    char error[512];dh2_lua *runtime=dh2_lua_create(2*1024*1024);CHECK(runtime);
    const char *basic="assert(_VERSION=='Lua 5.1'); assert(math.floor(1.9)==1);"
        "assert(string.upper('dh2')=='DH2'); local t={3,1,2};table.sort(t);assert(t[1]==1);"
        "assert(io==nil and os==nil and debug==nil and package==nil);"
        "assert(dofile==nil and loadfile==nil and print==nil);"
        "assert(type(GetPyCst)=='function' and type(GetPyOID)=='function' and type(GetPyStruct)=='function' and PlayAnim==nil);"
        "assert(GetPyStruct('CharacterProperties','HP')==36);assert(GetPyStruct('CharAnim','Attack')==0);"
        "assert(GetPyStruct('CharAnim','Attack2H')==-1);assert(GetPyStruct('Item','')==23);"
        "assert(GetPyOID('CharacterProperties','HP')==36);assert(GetPyStruct('missing','HP')==-1);"
        "assert(select('#',GetPyStruct('CharacterProperties'))==0);assert(select('#',GetPyStruct(1,'HP'))==0);"
        "assert(GetPyStruct('CharacterProperties\\000suffix','HP\\000suffix','ignored')==36);"
        "assert(getfenv~=nil and setfenv~=nil);";
    CHECK(dh2_lua_execute(runtime,basic,strlen(basic),100,error,sizeof(error))==0);
    const char *bridge="assert(ToFixed(-1.9)==-256);assert(ToFixed(8388608)==-2147483648);"
        "assert(ToFixed(16777216)==0);local whole,fractional=FromFixed(-1);"
        "assert(whole==-1 and fractional==-1/256);assert(select('#',FromFixed(256))==2);"
        "assert(MulFixed(256,128)==128);assert(MulFixed(65536,65536)==0);"
        "assert(DivFixed(512,384)==512);assert(BitNot(0)==-1);"
        "assert(BitAnd(15,7,3)==3);assert(BitOr(1,2,4)==7);assert(BitXOr(15,3)==12);"
        "assert(select('#',BitOr(1))==0);assert(select('#',BitNot('1'))==0);"
        "assert(select('#',BitXOr(1))==0);assert(select('#',Trace('no output'))==0);"
        "assert(pcall(DivFixed,1,1)==false);assert(pcall(ToFixed,0/0)==false);"
        "assert(pcall(ToFixed,2147483648)==false);";
    CHECK(dh2_lua_execute(runtime,bridge,strlen(bridge),100,error,sizeof(error))==0);
    const char *table_edges="local t={}; local keys={-2147483648,2147483648,1/0,-1/0,1,2};"
        "for i,k in ipairs(keys) do t[k]=i;assert(t[k]==i)end;"
        "local n=0;for k,v in pairs(t)do n=n+1;assert(t[k]==v)end;assert(n==6);"
        "local nan=0/0;assert(t[nan]==nil);assert(pcall(function()t[nan]=1 end)==false);";
    CHECK(dh2_lua_execute(runtime,table_edges,strlen(table_edges),100,error,sizeof(error))==0);
    unsigned char constants[]={1,0,0,0,1,0,0,0,'G',2,0,0,0,
        1,0,0,0,'K',1,0,0,0,1,0,0,0,'K',0xff,0xff,0xff,0xff};
    CHECK(dh2_lua_import_constants(runtime,constants,sizeof(constants),error,sizeof(error))==0);
    memset(constants,0,sizeof(constants)); /* Installed strings/values must be owned. */
    const char *lookup="assert(GetPyCst('G','K')==-1);assert(GetPyCst('G','K','ignored')==-1);"
        "assert(GetPyCst('G','missing')==0);assert(GetPyCst('missing','K')==0);"
        "assert(select('#',GetPyCst('G'))==0);assert(select('#',GetPyCst(1,'K'))==0);"
        "assert(GetPyCst('G\\000suffix','K\\000suffix')==-1);";
    CHECK(dh2_lua_execute(runtime,lookup,strlen(lookup),100,error,sizeof(error))==0);
    unsigned char names[]={3,0,0,0,3,0,0,0,'d','u','p',5,0,0,0,'o','t','h','e','r',3,0,0,0,'d','u','p'};
    char classname[]="Names";
    CHECK(dh2_lua_import_names(runtime,classname,5,names,sizeof(names),error,sizeof(error))==0);
    memset(names,0,sizeof(names));memset(classname,0,sizeof(classname));
    const char *ids="assert(GetPyOID('Names','dup')==0);assert(GetPyOID('Names','other')==1);"
        "assert(GetPyOID('Names','missing')==-1);assert(GetPyOID('missing','dup')==-1);"
        "assert(select('#',GetPyOID('Names'))==0);assert(select('#',GetPyOID(1,'dup'))==0);"
        "assert(GetPyOID('Names\\000suffix','dup\\000suffix','ignored')==0);";
    CHECK(dh2_lua_execute(runtime,ids,strlen(ids),100,error,sizeof(error))==0);
    const unsigned char replacement[]={1,0,0,0,3,0,0,0,'n','e','w'};
    CHECK(dh2_lua_import_names(runtime,"Empty",5,replacement,sizeof(replacement),error,sizeof(error))==0);
    const char *replaced="assert(GetPyStruct('Empty','new')==0);assert(GetPyStruct('Empty','missing')==-1);"
        "assert(GetPyStruct('CharacterProperties','HP')==36);";
    CHECK(dh2_lua_execute(runtime,replaced,strlen(replaced),100,error,sizeof(error))==0);
    CHECK(dh2_lua_import_names(runtime,"Names",5,names,1,error,sizeof(error))!=0);
    CHECK(dh2_lua_execute(runtime,ids,strlen(ids),100,error,sizeof(error))==0);
    const unsigned char empty_names[4]={0};
    CHECK(dh2_lua_import_names(runtime,"Empty",5,empty_names,4,error,sizeof(error))==0);
    CHECK(dh2_lua_import_names(runtime,"Empty",5,replacement,sizeof(replacement),error,sizeof(error))==0);
    CHECK(dh2_lua_execute(runtime,replaced,strlen(replaced),100,error,sizeof(error))==0);
    CHECK(dh2_lua_execute(runtime,ids,strlen(ids),100,error,sizeof(error))==0);
    CHECK(dh2_lua_import_constants(runtime,constants,1,error,sizeof(error))!=0);
    CHECK(dh2_lua_execute(runtime,lookup,strlen(lookup),100,error,sizeof(error))==0);
    dh2_lua *small=dh2_lua_create(256*1024);CHECK(small);
    const unsigned char baseline[]={1,0,0,0,1,0,0,0,'G',1,0,0,0,1,0,0,0,'K',7,0,0,0};
    CHECK(dh2_lua_import_constants(small,baseline,sizeof(baseline),error,sizeof(error))==0);
    const unsigned char name_baseline[]={1,0,0,0,3,0,0,0,'d','u','p'};
    CHECK(dh2_lua_import_names(small,"Names",5,name_baseline,sizeof(name_baseline),error,sizeof(error))==0);
    size_t big_size=13+6000*24;unsigned char *big=malloc(big_size);CHECK(big);
    put32(big,1);put32(big+4,1);big[8]='H';put32(big+9,6000);
    for(unsigned i=0;i<6000;++i) {
        unsigned char *p=big+13+i*24;put32(p,16);
        char key[17];CHECK(snprintf(key,sizeof(key),"missing-%08u",i)==16);
        memcpy(p+4,key,16);put32(p+20,i);
    }
    CHECK(dh2_lua_import_constants(small,big,big_size,error,sizeof(error))!=0);
    CHECK(strstr(error,"memory"));free(big);
    big_size=4+6000*20;big=malloc(big_size);CHECK(big);put32(big,6000);
    for(unsigned i=0;i<6000;++i) {
        unsigned char *p=big+4+i*20;put32(p,16);char key[17];
        CHECK(snprintf(key,sizeof(key),"missing-%08u",i)==16);memcpy(p+4,key,16);
    }
    CHECK(dh2_lua_import_names(small,"Names",5,big,big_size,error,sizeof(error))!=0);
    CHECK(strstr(error,"memory"));free(big);
    const char *retained="assert(GetPyCst('G','K')==7);assert(GetPyCst('H','missing-00000000')==0);";
    CHECK(dh2_lua_execute(small,retained,strlen(retained),100,error,sizeof(error))==0);
    const char *retained_ids="assert(GetPyOID('Names','dup')==0);assert(GetPyOID('Names','missing-00000000')==-1);";
    CHECK(dh2_lua_execute(small,retained_ids,strlen(retained_ids),100,error,sizeof(error))==0);
    CHECK(dh2_lua_memory_used(small)<=256*1024);dh2_lua_destroy(small);
    const char *non_string="error({})";
    CHECK(dh2_lua_execute(runtime,non_string,strlen(non_string),100,error,sizeof(error))!=0);
    CHECK(strcmp(error,"non-string Lua error")==0);
    const char *loop="while true do end";
    CHECK(dh2_lua_execute(runtime,loop,strlen(loop),3,error,sizeof(error))!=0);
    CHECK(strstr(error,"instruction budget exhausted"));
    CHECK(dh2_lua_execute(runtime,basic,strlen(basic),100,error,sizeof(error))==0);
    const char *memory="local t={};while true do t[#t+1]=string.rep('a',1024)end";
    CHECK(dh2_lua_execute(runtime,memory,strlen(memory),10000,error,sizeof(error))!=0);
    CHECK(strstr(error,"memory"));
    CHECK(dh2_lua_memory_used(runtime)<=2*1024*1024);
    const char *collect="collectgarbage('collect');assert(1+1==2)";
    CHECK(dh2_lua_execute(runtime,collect,strlen(collect),100,error,sizeof(error))==0);
    CHECK(dh2_lua_compile(runtime,"assert(false)",13,error,sizeof(error))==0);
    CHECK(dh2_lua_compile(runtime,"local =",7,error,sizeof(error))!=0);
    CHECK(dh2_lua_compile(runtime,"\033Lua",4,error,sizeof(error))==-1);
    CHECK(dh2_lua_compile(runtime,NULL,1,error,sizeof(error))==-1);
    CHECK(dh2_lua_compile(runtime,"",0,error,sizeof(error))==0);
    CHECK(dh2_lua_create(1)==NULL);
    CHECK(dh2_lua_create(65*1024*1024)==NULL);
    dh2_lua_destroy(runtime);
    printf("SELFTEST PASS\n");
    CHECK(dh2_lua_numeric_tests()==0);
    CHECK(dh2_lua_property_tests()==0);
    if (argc==1)return 0;
    if (argc==4 && strcmp(argv[1],"--constants")==0)return dh2_lua_constant_corpus(argv[2],argv[3]);
    if (argc==4 && strcmp(argv[1],"--names")==0)return dh2_lua_name_corpus(argv[2],argv[3]);
    if (argc==4 && strcmp(argv[1],"--execute")==0)return dh2_lua_execution_corpus(argv[2],argv[3]);
    if (argc==4 && strcmp(argv[1],"--properties")==0)return dh2_lua_property_corpus(argv[2],argv[3]);
    CHECK(argc==2);FILE *list=fopen(argv[1],"rb");CHECK(list);char path[2048];unsigned index=0;
    while (fgets(path,sizeof(path),list)) {
        size_t length=strlen(path);CHECK(length && path[length-1]=='\n');
        path[--length]='\0';if(length && path[length-1]=='\r')path[--length]='\0';CHECK(length);
        FILE *file=fopen(path,"rb");CHECK(file);CHECK(fseek(file,0,SEEK_END)==0);
        long bytes=ftell(file);CHECK(bytes>=0 && bytes<=1024*1024);rewind(file);
        char *source=(char *)malloc(bytes? (size_t)bytes:1);CHECK(source);
        CHECK(fread(source,1,(size_t)bytes,file)==(size_t)bytes);CHECK(fclose(file)==0);
        runtime=dh2_lua_create(2*1024*1024);CHECK(runtime);
        int status=dh2_lua_compile(runtime,source,(size_t)bytes,error,sizeof(error));
        printf("SOURCE %u %d\n",index++,status);
        free(source);dh2_lua_destroy(runtime);
    }
    CHECK(!ferror(list));CHECK(fclose(list)==0);printf("FILES %u\n",index);return 0;
}
