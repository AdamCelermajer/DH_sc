#include "../runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"property check line %d: %s\n",__LINE__,#x);return 2; } } while(0)
static void put(unsigned char *p,unsigned value) { for(unsigned i=0;i<4;++i)p[i]=(unsigned char)(value>>(8*i)); }
static int execute(dh2_lua *r,const char *source) {
    char error[512];int status=dh2_lua_execute(r,source,strlen(source),10000,error,sizeof(error));
    if(status)fprintf(stderr,"property script: %s\n",error);return status;
}
int dh2_lua_property_tests(void) {
    unsigned char raw[2700]={0};put(raw,3);put(raw+900+36*4,8);put(raw+900+19*4,16);put(raw+1796+19*4,512);
    char error[512];dh2_lua *r=dh2_lua_create(2*1024*1024);CHECK(r);
    CHECK(!execute(r,"assert(pcall(DH2CreatePropertyState,2)==false)"));
    CHECK(!dh2_lua_import_character_properties(r,raw,sizeof(raw),error,sizeof(error)));
    memset(raw,0,sizeof(raw)); /* Dataset must be owned after caller release. */
    CHECK(!execute(r,"old=DH2CreatePropertyState(2);assert(type(old)=='userdata');assert(getmetatable(old)==false);"
                     "assert(old:GetProp(19)==512);assert(old:GetProp(36)==0);old:SetProp(36,257.9);assert(old:GetProp(36)==257);"
                     "assert(old:GetProp(36,true)==0);old:SetProp(19,77);assert(old:GetProp(19)==512);"
                     "assert(select('#',old:GetProp())==0);assert(select('#',old:GetProp('36'))==0);"
                     "assert(select('#',old:GetProp(224))==0);assert(old:GetProp(-1.2)==0);"
                     "assert(pcall(DH2CreatePropertyState,-1)==false);assert(pcall(DH2CreatePropertyState,1.5)==false);"
                     "assert(pcall(DH2CreatePropertyState,3)==false);assert(pcall(DH2CreatePropertyState,0/0)==false);"
                     "assert(pcall(function()old:SetProp(36,1/0)end)==false);assert(old:GetProp(36)==257);"
                     "assert(pcall(function()old:GetProp(36,old)end)==false);assert(old:GetProp(36)==257);"));
    CHECK(dh2_lua_import_character_properties(r,raw,1,error,sizeof(error))!=0);
    CHECK(!execute(r,"assert(DH2CreatePropertyState(2):GetProp(19)==512);assert(old:GetProp(36)==257)"));
    put(raw,3);put(raw+4+36*4,42);put(raw+900+36*4,8);put(raw+900+19*4,16);put(raw+1796+19*4,768);
    CHECK(!dh2_lua_import_character_properties(r,raw,sizeof(raw),error,sizeof(error)));memset(raw,0,sizeof(raw));
    CHECK(!execute(r,"new=DH2CreatePropertyState(2);assert(new:GetProp(19)==768);assert(new:GetProp(36)==42);"
                     "assert(new:GetProp(36,true)==42);assert(old:GetProp(36,true)==0);old:SetProp(36,500);"
                     "assert(old:GetProp(36)==500);assert(new:GetProp(36)==42);collectgarbage('collect');assert(old:GetProp(19)==512)"));
    dh2_lua_destroy(r);
    r=dh2_lua_create(256*1024);CHECK(r);put(raw,3);put(raw+900+19*4,16);put(raw+1796+19*4,512);
    CHECK(!dh2_lua_import_character_properties(r,raw,sizeof(raw),error,sizeof(error)));
    size_t bytes=4096*896+12;unsigned char *large=calloc(1,bytes);CHECK(large);put(large,4096);
    CHECK(dh2_lua_import_character_properties(r,large,bytes,error,sizeof(error))!=0 && strstr(error,"memory"));free(large);
    CHECK(!execute(r,"collectgarbage('collect');assert(DH2CreatePropertyState(2):GetProp(19)==512)"));
    CHECK(dh2_lua_memory_used(r)<=256*1024);dh2_lua_destroy(r);return 0;
}
int dh2_lua_property_corpus(const char *data,const char *listing) {
    char error[512];FILE *f=fopen(data,"rb");CHECK(f);CHECK(!fseek(f,0,SEEK_END));long size=ftell(f);CHECK(size>=0 && size<=4*1024*1024);rewind(f);
    unsigned char *bytes=malloc(size?size:1);CHECK(bytes);CHECK(fread(bytes,1,size,f)==(size_t)size);CHECK(!fclose(f));
    dh2_lua *r=dh2_lua_create(8*1024*1024);CHECK(r);CHECK(!dh2_lua_import_character_properties(r,bytes,size,error,sizeof(error)));free(bytes);
    f=fopen(listing,"rb");CHECK(f);char path[2048];unsigned count=0;
    while(fgets(path,sizeof(path),f)) {
        size_t n=strlen(path);CHECK(n && path[n-1]=='\n');path[--n]=0;if(n && path[n-1]=='\r')path[--n]=0;
        FILE *source=fopen(path,"rb");CHECK(source);CHECK(!fseek(source,0,SEEK_END));long length=ftell(source);CHECK(length>=0 && length<=1024*1024);rewind(source);
        char *text=malloc(length?length:1);CHECK(text);CHECK(fread(text,1,length,source)==(size_t)length);CHECK(!fclose(source));
        int status=dh2_lua_execute(r,text,length,10000,error,sizeof(error));free(text);if(status)fprintf(stderr,"properties %u: %s\n",count,error);CHECK(!status);++count;
    }
    CHECK(!ferror(f));CHECK(!fclose(f));dh2_lua_destroy(r);printf("PROPERTY CORPUS PASS %u\n",count);return 0;
}
