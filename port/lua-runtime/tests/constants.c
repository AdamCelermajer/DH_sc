#include "../runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* The list and authored assertion file are staged/hash-checked by Python QA. */
int dh2_lua_constant_corpus(const char *listing,const char *assertions) {
    char error[512],path[2048];unsigned index=0,passed=0,rejected=0;
    dh2_lua *runtime=dh2_lua_create(8*1024*1024);if(!runtime)return 2;
    FILE *list=fopen(listing,"rb");if(!list)return 2;
    while(fgets(path,sizeof(path),list)) {
        size_t length=strlen(path);if(!length || path[length-1]!='\n')return 2;
        path[--length]=0;if(length && path[length-1]=='\r')path[--length]=0;
        FILE *file=fopen(path,"rb");if(!file || fseek(file,0,SEEK_END))return 2;
        long bytes=ftell(file);if(bytes<0 || bytes>16*1024*1024)return 2;rewind(file);
        void *source=malloc(bytes?(size_t)bytes:1);if(!source)return 2;
        if(fread(source,1,(size_t)bytes,file)!=(size_t)bytes || fclose(file))return 2;
        int status=dh2_lua_import_constants(runtime,source,(size_t)bytes,error,sizeof(error));
        printf("CONSTANT %u %d\n",index++,status);if(status)++rejected;else ++passed;
        /* Caller buffers are released: subsequent Lua queries must use owned data. */
        memset(source,0,(size_t)bytes);free(source);
    }
    if(ferror(list) || fclose(list) || passed!=26 || rejected!=1 || index!=27)return 2;
    FILE *file=fopen(assertions,"rb");if(!file || fseek(file,0,SEEK_END))return 2;
    long bytes=ftell(file);if(bytes<0 || bytes>1024*1024)return 2;rewind(file);
    char *source=malloc(bytes?(size_t)bytes:1);if(!source)return 2;
    if(fread(source,1,(size_t)bytes,file)!=(size_t)bytes || fclose(file))return 2;
    int status=dh2_lua_execute(runtime,source,(size_t)bytes,10000,error,sizeof(error));free(source);
    if(status) { fprintf(stderr,"constant assertions: %s\n",error);return 2; }
    if(dh2_lua_memory_used(runtime)>8*1024*1024)return 2;
    dh2_lua_destroy(runtime);puts("CONSTANT LOOKUPS PASS 5608");return 0;
}
