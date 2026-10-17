#include "../runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"class Lua check line %d: %s\n",__LINE__,#x);return 2; } }while(0)
static void put(unsigned char *p,unsigned value) { for(unsigned i=0;i<4;++i)p[i]=(unsigned char)(value>>(8*i)); }
static int execute(dh2_lua *r,const char *source) {
    char error[512];int status=dh2_lua_execute(r,source,strlen(source),10000,error,sizeof(error));
    if(status)fprintf(stderr,"class script: %s\n",error);return status;
}
int dh2_lua_class_tests(void) {
    unsigned char props[2700]={0},raw[28]={0};put(props,3);
    put(props+900+19*4,16);put(props+900+20*4,16);put(props+1796+19*4,512);put(props+1796+20*4,1536);
    put(raw,1);put(raw+4,1);put(raw+8,20);put(raw+12,1);put(raw+16,-666u);put(raw+20,19);put(raw+24,256);
    dh2_lua *r=dh2_lua_create(2*1024*1024);CHECK(r);char error[512];
    CHECK(!dh2_lua_import_character_properties(r,props,sizeof(props),error,sizeof(error)));
    CHECK(!execute(r,"before=DH2CreatePropertyState(2);assert(pcall(function()before:ApplyClass(0)end)==false)"));
    CHECK(!dh2_lua_import_character_classes(r,raw,sizeof(raw),error,sizeof(error)));memset(raw,0,sizeof(raw));
    CHECK(!execute(r,"old=DH2CreatePropertyState(2);old:ApplyClass(0);assert(old:GetProp(20)==2048);"
                     "assert(pcall(function()before:ApplyClass(0)end)==false);"
                     "assert(pcall(function()old:ApplyClass(1)end)==false);assert(pcall(function()old:ApplyClass(0,'true')end)==false);"
                     "assert(pcall(function()old:ApplyClass(0/0)end)==false);assert(old:GetProp(20)==2048)"));
    CHECK(dh2_lua_import_character_classes(r,raw,1,error,sizeof(error))!=0);
    CHECK(!execute(r,"assert(DH2CreatePropertyState(2):GetProp(20)==1536);old:ApplyClass(0,true);assert(old:GetProp(20)==2560)"));
    put(raw,1);put(raw+4,1);put(raw+8,20);put(raw+12,1);put(raw+16,-666u);put(raw+20,19);put(raw+24,512);
    CHECK(!dh2_lua_import_character_classes(r,raw,sizeof(raw),error,sizeof(error)));memset(raw,0,sizeof(raw));
    CHECK(!execute(r,"new=DH2CreatePropertyState(2);new:ApplyClass(0);assert(new:GetProp(20)==2560);"
                     "old:ApplyClass(0);assert(old:GetProp(20)==3072);collectgarbage('collect')"));
    /* Cyclic classes are structurally valid; guarded execution must be atomic. */
    put(raw,1);put(raw+4,1);put(raw+8,-1u);put(raw+12,0);put(raw+16,0);put(raw+20,-1u);put(raw+24,-1u);
    CHECK(!dh2_lua_import_character_classes(r,raw,sizeof(raw),error,sizeof(error)));
    CHECK(!execute(r,"cyclic=DH2CreatePropertyState(2);assert(pcall(function()cyclic:ApplyClass(0)end)==false);"
                     "assert(cyclic:GetProp(20)==1536);assert(new:GetProp(20)==2560)"));
    dh2_lua_destroy(r);
    r=dh2_lua_create(256*1024);CHECK(r);put(raw+8,20);put(raw+12,9);put(raw+16,777);
    CHECK(!dh2_lua_import_character_properties(r,props,sizeof(props),error,sizeof(error)));
    CHECK(!dh2_lua_import_character_classes(r,raw,sizeof(raw),error,sizeof(error)));
    size_t bytes=8+50000*20;unsigned char *large=calloc(1,bytes);CHECK(large);put(large,1);put(large+4,50000);
    CHECK(dh2_lua_import_character_classes(r,large,bytes,error,sizeof(error))!=0 && strstr(error,"memory"));free(large);
    CHECK(!execute(r,"collectgarbage('collect');local c=DH2CreatePropertyState(2);c:ApplyClass(0);assert(c:GetProp(20)==777)"));
    CHECK(dh2_lua_memory_used(r)<=256*1024);dh2_lua_destroy(r);return 0;
}
static int import_file(dh2_lua *r,const char *path,int classes) {
    FILE *f=fopen(path,"rb");CHECK(f);CHECK(!fseek(f,0,SEEK_END));long size=ftell(f);CHECK(size>=0 && size<=4*1024*1024);rewind(f);
    unsigned char *bytes=malloc(size?size:1);CHECK(bytes);CHECK(fread(bytes,1,size,f)==(size_t)size);CHECK(!fclose(f));char error[512];
    int status=classes?dh2_lua_import_character_classes(r,bytes,size,error,sizeof(error)):dh2_lua_import_character_properties(r,bytes,size,error,sizeof(error));
    free(bytes);if(status)fprintf(stderr,"class import: %s\n",error);CHECK(!status);return 0;
}
int dh2_lua_class_corpus(const char *properties,const char *classes,const char *listing) {
    dh2_lua *r=dh2_lua_create(8*1024*1024);CHECK(r);CHECK(!import_file(r,properties,0));CHECK(!import_file(r,classes,1));
    FILE *f=fopen(listing,"rb");CHECK(f);char path[2048],error[512];unsigned count=0;
    while(fgets(path,sizeof(path),f)) {
        size_t n=strlen(path);CHECK(n && path[n-1]=='\n');path[--n]=0;if(n && path[n-1]=='\r')path[--n]=0;
        FILE *source=fopen(path,"rb");CHECK(source);CHECK(!fseek(source,0,SEEK_END));long size=ftell(source);CHECK(size>=0 && size<=1024*1024);rewind(source);
        char *bytes=malloc(size?size:1);CHECK(bytes);CHECK(fread(bytes,1,size,source)==(size_t)size);CHECK(!fclose(source));
        int status=dh2_lua_execute(r,bytes,size,10000,error,sizeof(error));free(bytes);if(status)fprintf(stderr,"class corpus %u: %s\n",count,error);CHECK(!status);++count;
    }
    CHECK(!ferror(f));CHECK(!fclose(f));dh2_lua_destroy(r);printf("CLASS CORPUS PASS %u\n",count);return 0;
}
