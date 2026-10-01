/* Reconstructed equivalent of Storm's hook_import_function, using public ELF
 * data instead of the removed private bionic soinfo layout. Exact DH2 pair only.
 */
#include <elf.h>
#include <stdint.h>
#include <stddef.h>

extern void* storm_dlopen(const char*,int);
extern void* storm_dlsym(void*,const char*);

static int same(const char* a,const char* b) {
    while(*a && *a==*b){a++;b++;}return *a==*b;
}

__attribute__((visibility("default")))
void* storm_import_fix(const char* library,const char* name,void* replacement) {
    if(!library||!name||!replacement)return 0;
    void* handle=storm_dlopen(library,1);
    if(!handle)return 0;
    uintptr_t anchor=(uintptr_t)storm_dlsym(handle,"JNI_OnLoad");
    if(!anchor)return 0;
    uintptr_t base=anchor-0x0053224c;
    const Elf32_Ehdr* eh=(const Elf32_Ehdr*)base;
    if(eh->e_ident[0]!=0x7f||eh->e_ident[1]!='E'||eh->e_ident[2]!='L'||eh->e_ident[3]!='F'||eh->e_machine!=EM_ARM)return 0;
    const Elf32_Phdr* ph=(const Elf32_Phdr*)(base+eh->e_phoff);
    const Elf32_Dyn* dynamic=0;size_t count=0;
    for(unsigned i=0;i<eh->e_phnum;i++)if(ph[i].p_type==PT_DYNAMIC){dynamic=(const Elf32_Dyn*)(base+ph[i].p_vaddr);count=ph[i].p_memsz/sizeof(Elf32_Dyn);break;}
    if(!dynamic)return 0;
    const Elf32_Sym* symbols=0;const char* strings=0;const Elf32_Rel* relocs=0;size_t bytes=0;
    for(size_t i=0;i<count && dynamic[i].d_tag!=DT_NULL;i++){
        uintptr_t value=dynamic[i].d_un.d_ptr;
        switch(dynamic[i].d_tag){
            case DT_SYMTAB:symbols=(const Elf32_Sym*)(base+value);break;
            case DT_STRTAB:strings=(const char*)(base+value);break;
            case DT_JMPREL:relocs=(const Elf32_Rel*)(base+value);break;
            case DT_PLTRELSZ:bytes=value;break;
        }
    }
    if(!symbols||!strings||!relocs||bytes>1024*1024)return 0;
    for(size_t i=0;i<bytes/sizeof(Elf32_Rel);i++){
        if(ELF32_R_TYPE(relocs[i].r_info)!=R_ARM_JUMP_SLOT)continue;
        const Elf32_Sym* symbol=&symbols[ELF32_R_SYM(relocs[i].r_info)];
        if(same(strings+symbol->st_name,name)){
            /* The pinned DH2 ELF has no GNU_RELRO segment; its GOT is writable. */
            void** slot=(void**)(base+relocs[i].r_offset);
            void* old=*slot;*slot=replacement;return old;
        }
    }
    return 0;
}
