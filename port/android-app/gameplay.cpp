#include "gameplay.h"
#include "../lua-runtime/runtime.h"
#include "../character-properties/properties.h"
#include "../persistence/binary.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef __ANDROID__
#include <jni.h>
#include <pthread.h>
#endif

extern "C" bool dh2_world_walkable(float x,float y);

/* Authored orchestration around reconstructed source components. The stat
 * overrides deliberately make a short, testable encounter; enemy placement,
 * controls and AI are not claims about original level behavior. */
static const char encounter_lua[]=R"lua(
local enemies,player,quest,record,enemyCount,matchId,questRow,objectiveIndex
local deathEvents,lootRequests,lastDamage=0,0,0
local policy={target_dead=false,target_monster=true,local_player_alive=true,
 online=false,manager_present=true,manager_mode=0,monster_invincible=false,
 force_kill_config=false,force_kill_switch=false,target_network=false}
local deathPolicy={forced=false,loot_manager_present=false,
 kill_enemies=GetPyCst('v2QuestObjectiveType','KillXEnemies'),
 clear_enemies=GetPyCst('v2QuestObjectiveType','ClearEnemies'),
 kill_template=GetPyCst('v2QuestObjectiveType','KillEnemyTemplate'),
 clear_template=GetPyCst('v2QuestObjectiveType','ClearEnemyTemplate')}
assert(GetPyCst('CombatAttackTypes','Melee')==0)
assert(GetPyCst('CombatAttackTypes','Range')==1)
assert(GetPyCst('AIStates','Stunned')==9)
assert(GetPyCst('Elemental','none')==-1)
assert(deathPolicy.kill_template==10 and deathPolicy.clear_template==11)
local function health(a) local hp=a:GetHP();return hp end
local function balanced(a,hp,damage,name)
 -- Base stats are explicit authored rows appended to an owned table. Original
 -- SetProp follows each field's composition flags and cannot overwrite all
 -- base fields. SetHP remains the recovered property write operation.
 a:SetHP(hp)
 a:SetCombatContext(0,0,name)
end
function DH2EncounterReset()
 objectiveIndex=nil
 -- Choose an actual small property-kill objective. Only the condition/reward
 -- and original level dispatch are omitted in this development scene.
 for row=0,DH2GetQuestCount()-1 do
  local r=DH2GetQuestRecord(row)
  for index,o in ipairs(r.objectives)do
   if o.common[1]==0 and o.args[3]>=2 and o.args[3]<=3 and o.args[1]>=2 and o.args[1]<448 then
    record=r;questRow=row;objectiveIndex=index-1;matchId=o.args[1];enemyCount=o.args[3];break
   end
  end
  if objectiveIndex then break end
 end
 assert(objectiveIndex,'no suitable original encounter objective')
 enemies={};local population={}
 player=DH2CreatePropertyState(DH2EncounterPlayerRow);balanced(player,80,7,'Prince')
 for i=1,enemyCount do
  local a=DH2CreatePropertyState(DH2EncounterEnemyRow);balanced(a,18,3,'Sentry '..i)
  a:SetDeathContext{dead=false,network=false,suppress_events=false,
   target_id=({1001,1002,1003})[i],property_id=matchId,template_id=-1}
  enemies[i]=a;population[i]={property_id=matchId,template_id=-1}
 end
 local level=record.objectives[objectiveIndex+1].args[2]
 quest=DH2CreateCompiledQuestObjective(questRow,objectiveIndex,0,false,false,
  DH2CreateQuestWorld(level==-1 and 0 or level,population,{}))
 assert(quest:GetProgress().active)
 deathEvents=0;lootRequests=0;lastDamage=0;DH2SeedRandom(177)
 collectgarbage('collect')
 return enemyCount
end
function DH2EncounterHit(index)
 assert(index==math.floor(index) and index~=0 and math.abs(index)<=enemyCount)
 local sentry=enemies[math.abs(index)]
 if sentry:IsDead() or health(player)<=0 then return 0,0 end
 local attacker,target
 if index>0 then attacker=player;target=sentry else attacker=sentry;target=player end
 CF_ClearCombatants();CF_SetCombatants(attacker,target,-1,false,false)
 local raw=CF_CalcDamage(0,GetPyCst('CombatAttackTypes','Melee'))
 CF_ClearCombatants()
 policy.target_dead=index>0 and target:IsDead() or health(target)<=0
 policy.target_monster=index>0;policy.local_player_alive=health(player)>0
 local processed,death,whole=target:ApplyNonplayerHit(raw,policy,3)
 lastDamage=processed and whole or 0
 if index>0 and processed and death then
  local result=target:KillNonplayer(deathPolicy)
  if result.drop_loot_requested then lootRequests=lootRequests+1 end
  deathEvents=deathEvents+#result.events
  for _,event in ipairs(result.events)do
   -- Source receiver semantics: inactive/completed objectives do not dispatch.
   local p=quest:GetProgress()
   if p.active and not p.completed then quest:ConsumeKillEvent(event)end
  end
 end
 return lastDamage,(index>0 and target:IsDead() or health(target)<=0)and 1 or 0
end
function DH2EncounterState()
 local q=quest:GetProgress()
 local function eh(i)return enemies[i]and health(enemies[i])or 0 end
 return health(player),80,eh(1),eh(2),eh(3),q.current,q.required,
  q.completed and 1 or 0,deathEvents,lootRequests,questRow,matchId,lastDamage
end
local function pack(n)
 assert(n>=0 and n<=16777215 and n==math.floor(n),'invalid encounter counter')
 return string.char(n%256,math.floor(n/256)%256,math.floor(n/65536)%256,0)
end
local function unpack32(s,p)
 local a,b,c,d=string.byte(s,p,p+3);assert(d==0,'encounter counter outside exact range')
 return a+b*256+c*65536
end
function DH2EncounterExport()
 local parts={'DHE1',pack(enemyCount),pack(questRow),pack(objectiveIndex),pack(matchId),player:ExportEncounterState()}
 for i=1,enemyCount do parts[#parts+1]=enemies[i]:ExportEncounterState()end
 parts[#parts+1]=quest:ExportEncounterProgress();parts[#parts+1]=DH2ExportRandomState()
 parts[#parts+1]=pack(deathEvents)..pack(lootRequests)..pack(lastDamage)
 return table.concat(parts)
end
function DH2EncounterImport(bytes)
 assert(type(bytes)=='string' and #bytes==20+(enemyCount+1)*932+72,'invalid encounter checkpoint length')
 assert(string.sub(bytes,1,4)=='DHE1' and unpack32(bytes,5)==enemyCount,'invalid encounter checkpoint header')
 assert(unpack32(bytes,9)==questRow and unpack32(bytes,13)==objectiveIndex and unpack32(bytes,17)==matchId,'quest definition changed')
 local p=21;player:ImportEncounterState(string.sub(bytes,p,p+931));p=p+932
 local dead=0
 for i=1,enemyCount do
  enemies[i]:ImportEncounterState(string.sub(bytes,p,p+931));p=p+932
  assert(enemies[i]:IsDead()==(health(enemies[i])==0),'inconsistent sentry health/death')
  if enemies[i]:IsDead()then dead=dead+1 end
 end
 quest:ImportEncounterProgress(string.sub(bytes,p,p+39));p=p+40
 DH2ImportRandomState(string.sub(bytes,p,p+19));p=p+20
 local d,l,last=unpack32(bytes,p),unpack32(bytes,p+4),unpack32(bytes,p+8)
 assert(quest:GetProgress().current==dead and d==dead*2 and l==dead and last<=7,'inconsistent encounter events')
 deathEvents=d;lootRequests=l;lastDamage=last
 return 'ok'
end
)lua";

struct Spawn {const char *id;float x,y;};
static const Spawn spawns[]={{"sentry_a",1.4f,0.0f},{"sentry_b",0.5f,0.4f},{"sentry_c",0.9f,-0.3f}};
struct Enemy {float x,y,heading,pulse,cooldown;};
struct dh2_gameplay {
    dh2_lua *lua;
    float x,y,heading,attack_pulse,cooldown;
    Enemy enemies[3];
    float state[13];
    unsigned count;
    char error[256];
    dh2_gameplay_bytes assets[7];
    unsigned char generation[32];
    bool generation_set;
};
static void copy_text(char *out,size_t capacity,const char *text) {
    if(out && capacity)std::snprintf(out,capacity,"%s",text?text:"");
}
static bool call(dh2_gameplay *g,const char *name,const float *arguments,size_t count,
                  float *out,size_t results) {
    return dh2_lua_call_numbers(g->lua,name,arguments,count,out,results,1000,
                                g->error,sizeof(g->error))==0;
}
static bool update_state(dh2_gameplay *g) {
    return call(g,"DH2EncounterState",nullptr,0,g->state,13);
}
static void store_word(unsigned char *out,unsigned value) {
    out[0]=static_cast<unsigned char>(value);out[1]=static_cast<unsigned char>(value>>8);
    out[2]=static_cast<unsigned char>(value>>16);out[3]=static_cast<unsigned char>(value>>24);
}
static unsigned char *encounter_properties(const dh2_gameplay_bytes &source,size_t &size,unsigned &original_rows) {
    dh2_property_table table{};
    if(!source.data || source.size>4U*1024U*1024U-1792U ||
       dh2_property_open(&table,source.data,static_cast<unsigned>(source.size)))return nullptr;
    original_rows=table.counts[0];size=source.size+1792U;
    auto *copy=static_cast<unsigned char*>(std::malloc(size));if(!copy)return nullptr;
    const auto *raw=static_cast<const unsigned char*>(source.data);
    const size_t after=table.offsets[0]+original_rows*896U;
    std::memcpy(copy,raw,after);store_word(copy,original_rows+2);
    for(unsigned role=0;role<2;++role) {
        unsigned char *profile=copy+after+role*896U;
        std::memcpy(profile,raw+table.offsets[0],896U);
        const unsigned hp=role?18:80,damage=role?3:7;
        store_word(profile+36*4,0);store_word(profile+38*4,hp*256U);
        store_word(profile+19*4,256U);store_word(profile+79*4,damage*256U);
        store_word(profile+80*4,damage*256U);store_word(profile+97*4,static_cast<unsigned>(-256));
        const unsigned cleared[]={71,73,92,93,94,95,96,123,124,125,132,133,198,199};
        for(unsigned id:cleared)store_word(profile+id*4,0);
    }
    std::memcpy(copy+after+1792U,raw+after,source.size-after);return copy;
}
extern "C" dh2_gameplay *dh2_gameplay_create(const dh2_gameplay_bytes assets[7],
                                               char *error,size_t capacity) {
    if(!assets) {copy_text(error,capacity,"Encounter assets are absent");return nullptr;}
    auto *g=static_cast<dh2_gameplay*>(std::calloc(1,sizeof(dh2_gameplay)));
    if(!g) {copy_text(error,capacity,"Encounter allocation failed");return nullptr;}
    g->lua=dh2_lua_create(32U*1024U*1024U);
    if(!g->lua) {copy_text(error,capacity,"Source runtime allocation failed");std::free(g);return nullptr;}
    for(unsigned i=0;i<7;++i) {
        if(!assets[i].data || !assets[i].size || assets[i].size>4U*1024U*1024U) {
            copy_text(error,capacity,"Invalid retained encounter assets");dh2_gameplay_destroy(g);return nullptr;
        }
        void *copy=std::malloc(assets[i].size);
        if(!copy) {copy_text(error,capacity,"Encounter asset retention failed");dh2_gameplay_destroy(g);return nullptr;}
        std::memcpy(copy,assets[i].data,assets[i].size);g->assets[i]={copy,assets[i].size};
    }
    using Import=int(*)(dh2_lua*,const void*,size_t,char*,size_t);
    const Import imports[]={dh2_lua_import_character_properties,dh2_lua_import_character_classes,
        dh2_lua_import_loot_tables,dh2_lua_import_item_powers,dh2_lua_import_quests,dh2_lua_import_constants};
    size_t property_size=0;unsigned original_rows=0;
    unsigned char *profiles=encounter_properties(assets[0],property_size,original_rows);
    if(!profiles || imports[0](g->lua,profiles,property_size,g->error,sizeof(g->error))) {
        std::free(profiles);copy_text(error,capacity,g->error[0]?g->error:"Malformed encounter property source");
        dh2_gameplay_destroy(g);return nullptr;
    }
    std::free(profiles);
    for(unsigned i=1;i<6;++i)if(imports[i](g->lua,assets[i].data,assets[i].size,g->error,sizeof(g->error))) {
        copy_text(error,capacity,g->error);dh2_gameplay_destroy(g);return nullptr;
    }
    char profile_source[160];
    const int profile_length=std::snprintf(profile_source,sizeof(profile_source),
        "DH2EncounterPlayerRow=%u;DH2EncounterEnemyRow=%u",original_rows,original_rows+1);
    if(dh2_lua_execute(g->lua,assets[6].data,assets[6].size,1000,g->error,sizeof(g->error)) ||
       dh2_lua_execute(g->lua,profile_source,static_cast<size_t>(profile_length),1000,g->error,sizeof(g->error)) ||
       dh2_lua_execute(g->lua,encounter_lua,sizeof(encounter_lua)-1,1000,g->error,sizeof(g->error)) ||
       !dh2_gameplay_reset(g)) {
        copy_text(error,capacity,g->error);dh2_gameplay_destroy(g);return nullptr;
    }
    copy_text(error,capacity,"");return g;
}
extern "C" void dh2_gameplay_destroy(dh2_gameplay *g) {
    if(g) {dh2_lua_destroy(g->lua);for(auto &asset:g->assets)std::free(const_cast<void*>(asset.data));std::free(g);}
}
extern "C" int dh2_gameplay_reset(dh2_gameplay *g) {
    if(!g)return 0;
    float count;
    if(!call(g,"DH2EncounterReset",nullptr,0,&count,1) || count<1 || count>3 || std::floor(count)!=count ||
       !update_state(g))return 0;
    g->count=static_cast<unsigned>(count);g->x=g->y=g->heading=g->attack_pulse=g->cooldown=0;
    for(unsigned i=0;i<3;++i)g->enemies[i]={spawns[i].x,spawns[i].y,0,0,0.8f+0.4f*i};
    g->error[0]=0;return 1;
}
static float distance(float x,float y) {return std::sqrt(x*x+y*y);}
static float decay(float value,float dt) {return value>dt?value-dt:0;}
static void move_on_floor(float &x,float &y,float dx,float dy) {
    if(dh2_world_walkable(x+dx,y+dy)) {x+=dx;y+=dy;return;}
    if(dh2_world_walkable(x+dx,y))x+=dx;
    if(dh2_world_walkable(x,y+dy))y+=dy;
}
extern "C" size_t dh2_gameplay_step(dh2_gameplay *g,float mx,float my,float dt,
                                     int attack,float snapshot[21]) {
    if(!g || !snapshot || !std::isfinite(mx) || !std::isfinite(my) || !std::isfinite(dt) ||
       dt<0 || dt>1.0f)return 0;
    if(dt>0.1f)dt=0.1f; // authored anti-tunneling/focus-resume bound
    const float length=distance(mx,my);if(length>1) {mx/=length;my/=length;}
    g->attack_pulse=decay(g->attack_pulse,dt);g->cooldown=decay(g->cooldown,dt);
    const float before_x=g->x,before_y=g->y;
    if(g->state[0]>0 && !g->error[0]) {
        move_on_floor(g->x,g->y,mx*dt*0.95f,my*dt*0.95f);
        if(length>0.05f)g->heading=std::atan2(-mx,my);
        if(attack && !g->cooldown) {
            g->cooldown=0.42f;g->attack_pulse=0.333f;
            unsigned target=3;float nearest=0.85f;
            for(unsigned i=0;i<g->count;++i)if(g->state[2+i]>0) {
                const float d=distance(g->enemies[i].x-g->x,g->enemies[i].y-g->y);
                if(d<nearest) {nearest=d;target=i;}
            }
            if(target<3) {
                float arg=static_cast<float>(target+1),result[2];
                g->heading=std::atan2(g->x-g->enemies[target].x,g->enemies[target].y-g->y);
                if(!call(g,"DH2EncounterHit",&arg,1,result,2) || !update_state(g))return 0;
                g->enemies[target].pulse=0.22f;
            }
        }
        for(unsigned i=0;i<g->count;++i) {
            Enemy &e=g->enemies[i];e.pulse=decay(e.pulse,dt);e.cooldown=decay(e.cooldown,dt);
            if(g->state[2+i]<=0 || g->state[0]<=0)continue;
            const float dx=g->x-e.x,dy=g->y-e.y,d=distance(dx,dy);
            if(d>0.001f)e.heading=std::atan2(-dx,dy);
            if(d>0.34f && d<2.5f)move_on_floor(e.x,e.y,dx/d*dt*0.27f,dy/d*dt*0.27f);
            if(d<0.46f && !e.cooldown) {
                float arg=-static_cast<float>(i+1),result[2];e.cooldown=1.3f;
                if(!call(g,"DH2EncounterHit",&arg,1,result,2) || !update_state(g))return 0;
            }
        }
    }
    snapshot[0]=g->x;snapshot[1]=g->y;snapshot[2]=g->heading;
    snapshot[3]=distance(g->x-before_x,g->y-before_y)>0.0001f?1.0f:0.0f;
    snapshot[4]=g->attack_pulse/0.333f;snapshot[5]=static_cast<float>(g->count);
    for(unsigned i=0;i<g->count;++i) {
        const Enemy &e=g->enemies[i];float *out=snapshot+6+5*i;
        out[0]=e.x;out[1]=e.y;out[2]=e.heading;
        out[3]=g->state[2+i]/18.0f;out[4]=e.pulse/0.22f;
    }
    return 6+g->count*5;
}
extern "C" void dh2_gameplay_status(const dh2_gameplay *g,char *text,size_t capacity) {
    if(!text || !capacity)return;
    if(!g) {copy_text(text,capacity,"Source encounter is loading");return;}
    if(g->error[0]) {copy_text(text,capacity,g->error);return;}
    std::snprintf(text,capacity,"HP %d/80 | Sentries %d/%d%s | Move: left pad  Attack: right button",
        static_cast<int>(g->state[0]),static_cast<int>(g->state[5]),static_cast<int>(g->state[6]),
        g->state[7]>0?"  COMPLETE":g->state[0]<=0?"  DEFEATED - Reset to retry":"");
}
extern "C" int dh2_gameplay_player_hp(const dh2_gameplay *g) {return g?static_cast<int>(g->state[0]):0;}

static constexpr size_t save_header=64,record_size=48+DH2_ACTOR_SAVE_BYTES;
static constexpr size_t lua_header=20,lua_tail=DH2_QUEST_SAVE_BYTES+DH2_RANDOM_SAVE_BYTES+12;
static const char *actor_id(unsigned slot) {return slot?spawns[slot-1].id:"player";}
static void write_float(unsigned char *p,float value) {
    uint32_t bits;std::memcpy(&bits,&value,4);dh2_save_write32(p,bits);
}
static float read_float(const unsigned char *p) {
    uint32_t bits=dh2_save_read32(p);float value;std::memcpy(&value,&bits,4);return value;
}
extern "C" int dh2_gameplay_set_generation(dh2_gameplay *g,const unsigned char generation[32]) {
    if(!g || !generation || g->generation_set)return 0;
    std::memcpy(g->generation,generation,32);g->generation_set=true;return 1;
}
extern "C" size_t dh2_gameplay_save(dh2_gameplay *g,void *output,size_t capacity) {
    if(!g || !output || !g->generation_set || g->error[0] || g->count<1 || g->count>3)return 0;
    const size_t size=save_header+lua_header+(g->count+1)*record_size+lua_tail;
    if(capacity<size)return 0;
    unsigned char lua[8192];size_t lua_size=0;
    if(dh2_lua_call_bytes(g->lua,"DH2EncounterExport",nullptr,0,lua,sizeof(lua),&lua_size,1000,g->error,sizeof(g->error)) ||
       lua_size!=lua_header+(g->count+1)*DH2_ACTOR_SAVE_BYTES+lua_tail)return 0;
    auto *out=static_cast<unsigned char*>(output);std::memset(out,0,size);
    std::memcpy(out,"DH2S",4);dh2_save_write32(out+4,1);dh2_save_write32(out+8,static_cast<uint32_t>(size));
    std::memcpy(out+16,g->generation,32);dh2_save_write32(out+48,0x43525331U); // dev.cross_junction
    dh2_save_write32(out+52,1);dh2_save_write32(out+56,g->count);dh2_save_write32(out+60,0);
    std::memcpy(out+save_header,lua,lua_header);
    for(unsigned slot=0;slot<=g->count;++slot) {
        unsigned char *record=out+save_header+lua_header+slot*record_size;
        std::memcpy(record,actor_id(slot),std::strlen(actor_id(slot)));
        const float position[]={slot?g->enemies[slot-1].x:g->x,slot?g->enemies[slot-1].y:g->y,
            slot?g->enemies[slot-1].heading:g->heading,slot?g->enemies[slot-1].cooldown:g->cooldown};
        for(unsigned i=0;i<4;++i)write_float(record+32+i*4,position[i]);
        std::memcpy(record+48,lua+lua_header+slot*DH2_ACTOR_SAVE_BYTES,DH2_ACTOR_SAVE_BYTES);
    }
    std::memcpy(out+size-lua_tail,lua+lua_size-lua_tail,lua_tail);
    dh2_save_write32(out+12,dh2_save_crc32(out+save_header,size-save_header));return size;
}
extern "C" int dh2_gameplay_restore(dh2_gameplay **slot,const void *input,size_t size,char *error,size_t capacity) {
    auto fail=[&](const char *text){copy_text(error,capacity,text);return 0;};
    if(!slot || !*slot || !input || size<save_header || size>8192)return fail("Save length is invalid");
    dh2_gameplay *old=*slot;const auto *bytes=static_cast<const unsigned char*>(input);
    if(std::memcmp(bytes,"DH2S",4) || dh2_save_read32(bytes+4)!=1 || dh2_save_read32(bytes+8)!=size ||
       dh2_save_read32(bytes+48)!=0x43525331U || dh2_save_read32(bytes+52)!=1 || dh2_save_read32(bytes+60)!=0)
        return fail("Save format or encounter definition is unsupported");
    if(!old->generation_set || std::memcmp(bytes+16,old->generation,32))return fail("Saved encounter uses different assets or rules");
    const unsigned count=dh2_save_read32(bytes+56);
    if(count!=old->count || count<1 || count>3 || size!=save_header+lua_header+(count+1)*record_size+lua_tail ||
       dh2_save_read32(bytes+12)!=dh2_save_crc32(bytes+save_header,size-save_header))return fail("Save checksum or actor count is invalid");
    unsigned char lua[8192];std::memcpy(lua,bytes+save_header,lua_header);
    float positions[4][4]{};bool seen[4]{};
    for(unsigned source=0;source<=count;++source) {
        const unsigned char *record=bytes+save_header+lua_header+source*record_size;unsigned target=count+1;
        for(unsigned i=0;i<=count;++i) {
            char name[32]{};std::memcpy(name,actor_id(i),std::strlen(actor_id(i)));
            if(!std::memcmp(record,name,32)) {target=i;break;}
        }
        if(target>count || seen[target])return fail("Save contains an unknown or duplicate spawn ID");
        seen[target]=true;
        for(unsigned i=0;i<4;++i)positions[target][i]=read_float(record+32+i*4);
        const float *p=positions[target];
        for(unsigned i=0;i<4;++i)if(!std::isfinite(p[i]))return fail("Saved transform or cooldown is not finite");
        if(std::fabs(p[0])>4 || std::fabs(p[1])>4 || std::fabs(p[2])>3.1417f || p[3]<0 ||
           p[3]>(target?1.6001f:0.4201f) || !dh2_world_walkable(p[0],p[1]))return fail("Saved position or cooldown is outside the encounter");
        std::memcpy(lua+lua_header+target*DH2_ACTOR_SAVE_BYTES,record+48,DH2_ACTOR_SAVE_BYTES);
    }
    const size_t lua_size=lua_header+(count+1)*DH2_ACTOR_SAVE_BYTES+lua_tail;
    std::memcpy(lua+lua_size-lua_tail,bytes+size-lua_tail,lua_tail);
    char detail[256]{};dh2_gameplay *candidate=dh2_gameplay_create(old->assets,detail,sizeof(detail));
    if(!candidate)return fail(detail);
    dh2_gameplay_set_generation(candidate,old->generation);
    char result[4]{};size_t result_size=0;
    if(dh2_lua_call_bytes(candidate->lua,"DH2EncounterImport",lua,lua_size,result,sizeof(result),&result_size,1000,
           detail,sizeof(detail)) || result_size!=2 || std::memcmp(result,"ok",2) || !update_state(candidate)) {
        dh2_gameplay_destroy(candidate);return fail(detail[0]?detail:"Saved state did not validate");
    }
    candidate->x=positions[0][0];candidate->y=positions[0][1];candidate->heading=positions[0][2];candidate->cooldown=positions[0][3];
    for(unsigned i=0;i<count;++i) {
        Enemy &e=candidate->enemies[i];e.x=positions[i+1][0];e.y=positions[i+1][1];
        e.heading=positions[i+1][2];e.cooldown=positions[i+1][3];e.pulse=0;
    }
    *slot=candidate;dh2_gameplay_destroy(old);copy_text(error,capacity,"");return 1;
}
extern "C" int dh2_gameplay_reset_transactional(dh2_gameplay **slot,char *error,size_t capacity) {
    if(!slot || !*slot) {copy_text(error,capacity,"No encounter is running");return 0;}
    dh2_gameplay *old=*slot;dh2_gameplay *candidate=dh2_gameplay_create(old->assets,error,capacity);
    if(!candidate)return 0;
    if(old->generation_set)dh2_gameplay_set_generation(candidate,old->generation);
    *slot=candidate;dh2_gameplay_destroy(old);copy_text(error,capacity,"");return 1;
}

#ifdef __ANDROID__
static pthread_mutex_t session_guard=PTHREAD_MUTEX_INITIALIZER;
static dh2_gameplay *session=nullptr;
static jlong session_token=0;
extern "C" JNIEXPORT jlong JNICALL
Java_local_dh2_sourceviewer_GameplayActivity_sessionInit(JNIEnv *env,jclass,
        jbyteArray properties,jbyteArray classes,jbyteArray loot,jbyteArray powers,
        jbyteArray quests,jbyteArray constants,jbyteArray combat,jbyteArray generation) {
    jbyteArray inputs[]={properties,classes,loot,powers,quests,constants,combat};
    dh2_gameplay_bytes assets[7]{};jbyte *held[7]{};char error[256]{};bool valid=true;
    unsigned char fingerprint[32]{};
    if(!generation || env->GetArrayLength(generation)!=32)valid=false;
    else env->GetByteArrayRegion(generation,0,32,reinterpret_cast<jbyte*>(fingerprint));
    for(unsigned i=0;i<7 && valid;++i) {
        const jsize length=inputs[i]?env->GetArrayLength(inputs[i]):0;
        if(length<=0 || length>4*1024*1024) {valid=false;break;}
        held[i]=env->GetByteArrayElements(inputs[i],nullptr);
        if(!held[i]) {valid=false;break;}
        assets[i]={held[i],static_cast<size_t>(length)};
    }
    dh2_gameplay *candidate=valid?dh2_gameplay_create(assets,error,sizeof(error)):nullptr;
    for(unsigned i=0;i<7;++i)if(held[i])env->ReleaseByteArrayElements(inputs[i],held[i],JNI_ABORT);
    if(env->ExceptionCheck()) {dh2_gameplay_destroy(candidate);return 0;}
    if(!candidate) {
        jclass type=env->FindClass("java/lang/IllegalStateException");
        if(type)env->ThrowNew(type,error[0]?error:"Source encounter assets rejected");return 0;
    }
    dh2_gameplay_set_generation(candidate,fingerprint);
    pthread_mutex_lock(&session_guard);dh2_gameplay_destroy(session);session=candidate;
    session_token=session_token==INT64_MAX?1:session_token+1;jlong token=session_token;
    pthread_mutex_unlock(&session_guard);return token;
}
extern "C" JNIEXPORT jfloatArray JNICALL
Java_local_dh2_sourceviewer_GameplayActivity_sessionStep(JNIEnv *env,jclass,jlong token,jfloat mx,jfloat my,jfloat dt,jboolean attack) {
    float snapshot[21];pthread_mutex_lock(&session_guard);
    const size_t size=token==session_token?dh2_gameplay_step(session,mx,my,dt,attack?1:0,snapshot):0;
    pthread_mutex_unlock(&session_guard);
    if(!size)return nullptr;
    jfloatArray result=env->NewFloatArray(static_cast<jsize>(size));
    if(result)env->SetFloatArrayRegion(result,0,static_cast<jsize>(size),snapshot);
    return result;
}
extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_GameplayActivity_sessionStatus(JNIEnv *env,jclass,jlong token) {
    char text[256];pthread_mutex_lock(&session_guard);
    if(token!=session_token)copy_text(text,sizeof(text),"Encounter owner changed; reopen this screen");
    else dh2_gameplay_status(session,text,sizeof(text));
    pthread_mutex_unlock(&session_guard);return env->NewStringUTF(text);
}
extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_GameplayActivity_sessionReset(JNIEnv *env,jclass,jlong token) {
    char text[256];pthread_mutex_lock(&session_guard);
    if(token!=session_token)copy_text(text,sizeof(text),"Encounter owner changed; reopen this screen");
    else if(dh2_gameplay_reset_transactional(&session,text,sizeof(text)))dh2_gameplay_status(session,text,sizeof(text));
    pthread_mutex_unlock(&session_guard);return env->NewStringUTF(text);
}
extern "C" JNIEXPORT jbyteArray JNICALL
Java_local_dh2_sourceviewer_GameplayActivity_sessionSnapshot(JNIEnv *env,jclass,jlong token) {
    unsigned char bytes[8192];pthread_mutex_lock(&session_guard);
    const size_t size=token==session_token?dh2_gameplay_save(session,bytes,sizeof(bytes)):0;
    pthread_mutex_unlock(&session_guard);if(!size)return nullptr;
    jbyteArray result=env->NewByteArray(static_cast<jsize>(size));
    if(result)env->SetByteArrayRegion(result,0,static_cast<jsize>(size),reinterpret_cast<const jbyte*>(bytes));
    return result;
}
extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_GameplayActivity_sessionRestore(JNIEnv *env,jclass,jlong token,jbyteArray save) {
    const jsize size=save?env->GetArrayLength(save):0;
    if(size<=0 || size>8192)return env->NewStringUTF("Save length is invalid");
    unsigned char bytes[8192];env->GetByteArrayRegion(save,0,size,reinterpret_cast<jbyte*>(bytes));
    if(env->ExceptionCheck())return nullptr;
    char text[256];pthread_mutex_lock(&session_guard);
    if(token!=session_token)copy_text(text,sizeof(text),"Encounter owner changed; reopen this screen");
    else if(dh2_gameplay_restore(&session,bytes,static_cast<size_t>(size),text,sizeof(text)))
        copy_text(text,sizeof(text),"Encounter restored.");
    pthread_mutex_unlock(&session_guard);return env->NewStringUTF(text);
}
#endif
