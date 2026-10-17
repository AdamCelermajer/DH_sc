#include "../runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Test harness paths are staged and hash checked externally. No script resolver. */
static int execute_file(dh2_lua *runtime,const char *path) {
    FILE *file=fopen(path,"rb");if(!file)return 2;
    if(fseek(file,0,SEEK_END)) { fclose(file);return 2; }
    long bytes=ftell(file);if(bytes<0 || bytes>1024*1024) { fclose(file);return 2; }
    rewind(file);char *source=malloc(bytes?(size_t)bytes:1);
    if(!source) { fclose(file);return 2; }
    if(fread(source,1,(size_t)bytes,file)!=(size_t)bytes) { free(source);fclose(file);return 2; }
    if(fclose(file)) { free(source);return 2; }
    char error[512];int status=dh2_lua_execute(runtime,source,(size_t)bytes,10000,error,sizeof(error));
    free(source);if(status)fprintf(stderr,"execute %s: %s\n",path,error);return status;
}
int dh2_lua_execution_corpus(const char *listing,const char *assertions) {
    dh2_lua *runtime=dh2_lua_create(8*1024*1024);if(!runtime)return 2;
    FILE *list=fopen(listing,"rb");if(!list) { dh2_lua_destroy(runtime);return 2; }
    char path[2048];unsigned index=0;int status=0;
    while(fgets(path,sizeof(path),list)) {
        size_t length=strlen(path);if(!length || path[length-1]!='\n') { status=2;break; }
        path[--length]=0;if(length && path[length-1]=='\r')path[--length]=0;
        status=execute_file(runtime,path);printf("EXECUTED %u %d\n",index++,status);if(status)break;
    }
    if(ferror(list))status=2;if(fclose(list))status=2;
    if(!status && index!=3)status=2;
    if(!status)status=execute_file(runtime,assertions);
    dh2_lua_destroy(runtime);if(!status)puts("ORIGINAL SHARED SCRIPTS PASS 3");return status;
}
