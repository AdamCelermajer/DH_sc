#include "../runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"check failed at line %d: %s\n",__LINE__,#x);return 2; } } while(0)
int main(int argc,char **argv) {
    char error[512];dh2_lua *runtime=dh2_lua_create(2*1024*1024);CHECK(runtime);
    const char *basic="assert(_VERSION=='Lua 5.1'); assert(math.floor(1.9)==1);"
        "assert(string.upper('dh2')=='DH2'); local t={3,1,2};table.sort(t);assert(t[1]==1);"
        "assert(io==nil and os==nil and debug==nil and package==nil);"
        "assert(dofile==nil and loadfile==nil and print==nil);"
        "assert(GetPyCst==nil and GetPyOID==nil and PlayAnim==nil);"
        "assert(getfenv~=nil and setfenv~=nil);";
    CHECK(dh2_lua_execute(runtime,basic,strlen(basic),100,error,sizeof(error))==0);
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
    if (argc==1)return 0;
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
