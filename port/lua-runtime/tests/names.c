#include "../runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int dh2_lua_name_corpus(const char *listing,const char *assertions) {
    char error[512],line[2304];unsigned index=0;
    dh2_lua *runtime=dh2_lua_create(16*1024*1024);if(!runtime)return 2;
    FILE *list=fopen(listing,"rb");if(!list)return 2;
    while(fgets(line,sizeof(line),list)) {
        size_t length=strlen(line);if(!length || line[length-1]!='\n')return 2;
        line[--length]=0;if(length && line[length-1]=='\r')line[--length]=0;
        char *path=strchr(line,'\t');if(!path)return 2;*path++=0;
        size_t class_length=strlen(line);if(!class_length || class_length>255)return 2;
        FILE *file=fopen(path,"rb");if(!file || fseek(file,0,SEEK_END))return 2;
        long bytes=ftell(file);if(bytes<0 || bytes>16*1024*1024)return 2;rewind(file);
        void *source=malloc(bytes?(size_t)bytes:1);if(!source)return 2;
        if(fread(source,1,(size_t)bytes,file)!=(size_t)bytes || fclose(file))return 2;
        int status=dh2_lua_import_names(runtime,line,class_length,source,(size_t)bytes,error,sizeof(error));
        printf("NAMES %u %d\n",index++,status);memset(source,0,(size_t)bytes);free(source);
        if(status) { fprintf(stderr,"name import: %s\n",error);return 2; }
    }
    if(ferror(list) || fclose(list) || index!=71)return 2;
    FILE *file=fopen(assertions,"rb");if(!file || fseek(file,0,SEEK_END))return 2;
    long bytes=ftell(file);if(bytes<0 || bytes>1024*1024)return 2;rewind(file);
    char *source=malloc(bytes?(size_t)bytes:1);if(!source)return 2;
    if(fread(source,1,(size_t)bytes,file)!=(size_t)bytes || fclose(file))return 2;
    int status=dh2_lua_execute(runtime,source,(size_t)bytes,10000,error,sizeof(error));free(source);
    if(status) { fprintf(stderr,"name assertions: %s\n",error);return 2; }
    if(dh2_lua_memory_used(runtime)>16*1024*1024)return 2;
    dh2_lua_destroy(runtime);puts("NAME LOOKUPS PASS 8863");return 0;
}
