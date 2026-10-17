#include "../gameplay.h"
#include "../../persistence/binary.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

/* Serializer/encounter integration test, not a claim about original floor
 * geometry. Emulator UI QA separately exercises the real cached floor. */
extern "C" bool dh2_world_walkable(float x,float y) {
    return std::isfinite(x) && std::isfinite(y) && std::fabs(x)<2 && std::fabs(y)<1;
}
static void require(bool value,const char *message) {
    if(!value) {std::fprintf(stderr,"FAIL: %s\n",message);std::exit(1);}
}
static size_t save(dh2_gameplay *g,unsigned char bytes[8192]) {
    size_t size=dh2_gameplay_save(g,bytes,8192);require(size>0,"checkpoint capture");return size;
}
static unsigned quantity(const unsigned char *bytes,size_t size) {return dh2_save_read32(bytes+size-72+12);}
static void checksum(unsigned char *bytes,size_t size) {dh2_save_write32(bytes+12,dh2_save_crc32(bytes+64,size-64));}
static void reject(dh2_gameplay **g,const unsigned char *bytes,size_t size) {
    unsigned char before[8192],after[8192];size_t n=save(*g,before);dh2_gameplay *original=*g;char error[256];
    require(!dh2_gameplay_restore(g,bytes,size,error,sizeof(error)),"invalid restore rejected");
    require(*g==original && save(*g,after)==n && !std::memcmp(before,after,n),"rejection preserves exact live session");
}
int main(int argc,char **argv) {
    require(argc==8,"supply seven original encounter input paths");dh2_gameplay_bytes assets[7]{};
    for(unsigned i=0;i<7;++i) {
        FILE *file=std::fopen(argv[i+1],"rb");require(file!=nullptr,"open asset");
        require(!std::fseek(file,0,SEEK_END),"seek asset");long length=std::ftell(file);std::rewind(file);
        require(length>0 && length<4*1024*1024,"asset bound");void *bytes=std::malloc(static_cast<size_t>(length));
        require(bytes && std::fread(bytes,1,static_cast<size_t>(length),file)==static_cast<size_t>(length),"read asset");
        std::fclose(file);assets[i]={bytes,static_cast<size_t>(length)};
    }
    char error[256]{};dh2_gameplay *g=dh2_gameplay_create(assets,error,sizeof(error));
    if(!g)std::fprintf(stderr,"creation: %s\n",error);require(g!=nullptr,"real-data encounter creation");
    unsigned char generation[32];for(unsigned i=0;i<32;++i)generation[i]=static_cast<unsigned char>(i+1);
    require(dh2_gameplay_set_generation(g,generation),"generation set once");
    require(!dh2_gameplay_set_generation(g,generation),"generation immutable");
    unsigned char fresh[8192],checkpoint[8192],changed[8192],after[8192];size_t fresh_size=save(g,fresh);
    float frame[21];for(unsigned i=0;i<5;++i)require(dh2_gameplay_step(g,1,0,.1f,0,frame)>0,"move");
    size_t size=save(g,checkpoint);
    for(unsigned i=0;i<100 && quantity(checkpoint,size)==0;++i) {
        require(dh2_gameplay_step(g,0,0,.1f,1,frame)>0,"combat step");size=save(g,checkpoint);
    }
    require(quantity(checkpoint,size)==1,"one sentry killed, quest advanced once");
    dh2_gameplay *restored=dh2_gameplay_create(assets,error,sizeof(error));require(restored!=nullptr,"second session creation");
    require(dh2_gameplay_set_generation(restored,generation),"second generation");
    require(dh2_gameplay_restore(&restored,checkpoint,size,error,sizeof(error)),"mid-combat restore");
    require(save(restored,after)==size && !std::memcmp(checkpoint,after,size),"all persistent bits restored exactly");
    const size_t record=48+DH2_ACTOR_SAVE_BYTES;
    std::memcpy(changed,checkpoint,size);unsigned char temporary[980];
    std::memcpy(temporary,changed+84+record,record);std::memcpy(changed+84+record,changed+84+2*record,record);
    std::memcpy(changed+84+2*record,temporary,record);checksum(changed,size);
    require(dh2_gameplay_restore(&restored,changed,size,error,sizeof(error)),"stable spawn IDs survive record reordering");
    require(save(restored,after)==size && !std::memcmp(checkpoint,after,size),"canonical record order restored");
    for(size_t i=0;i<size;++i) {std::memcpy(changed,checkpoint,size);changed[i]^=1;reject(&restored,changed,size);}
    for(size_t i=0;i<size;++i)reject(&restored,checkpoint,i);
    std::memcpy(changed,checkpoint,size);dh2_save_write32(changed+84+32,0x7fc00000U);checksum(changed,size);reject(&restored,changed,size);
    std::memcpy(changed,checkpoint,size);std::memcpy(changed+84+record,changed+84,32);checksum(changed,size);reject(&restored,changed,size);
    std::memcpy(changed,checkpoint,size);dh2_save_write32(changed+84+48+4+36*4,81*256);checksum(changed,size);reject(&restored,changed,size);
    for(unsigned i=0;i<120;++i) {
        require(dh2_gameplay_step(g,0,0,.1f,1,frame)>0 && dh2_gameplay_step(restored,0,0,.1f,1,frame)>0,"continued combat");
        size_t a=save(g,checkpoint),b=save(restored,after);
        require(a==b && !std::memcmp(checkpoint,after,a),"cooldown/RNG/death/quest continuation deterministic");size=a;
    }
    require(quantity(checkpoint,size)==dh2_save_read32(checkpoint+56),"victory reached");
    require(dh2_gameplay_restore(&restored,checkpoint,size,error,sizeof(error)),"victory restore");
    require(dh2_gameplay_reset_transactional(&restored,error,sizeof(error)),"transactional reset");
    require(save(restored,after)==fresh_size && !std::memcmp(fresh,after,fresh_size),"reset exact fresh state");
    for(unsigned i=0;i<600 && dh2_gameplay_player_hp(restored)>0;++i)
        require(dh2_gameplay_step(restored,0,0,.1f,0,frame)>0,"enemy combat until defeat");
    require(dh2_gameplay_player_hp(restored)==0,"defeat reached");size=save(restored,checkpoint);
    require(dh2_gameplay_restore(&restored,checkpoint,size,error,sizeof(error)) && dh2_gameplay_player_hp(restored)==0,"defeat restore");
    dh2_gameplay_destroy(g);dh2_gameplay_destroy(restored);
    for(auto &asset:assets)std::free(const_cast<void*>(asset.data));
    std::printf("PASS: %zu corruptions + %zu truncations; real-data encounter round trip; named spawn reorder; semantic validation/transactional rollback; 120 deterministic continuation steps; victory, reset and defeat.\n",size,size);
}
