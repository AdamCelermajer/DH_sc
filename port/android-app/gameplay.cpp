#include "gameplay.h"
#include "../lua-runtime/runtime.h"
#include "../character-properties/properties.h"
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
local enemies,player,quest,record,enemyCount,matchId,questRow
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
 local objectiveIndex
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
   target_id=1000+i,property_id=matchId,template_id=-1}
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
)lua";

struct Enemy {float x,y,heading,pulse,cooldown;};
struct dh2_gameplay {
    dh2_lua *lua;
    float x,y,heading,attack_pulse,cooldown;
    Enemy enemies[3];
    float state[13];
    unsigned count;
    char error[256];
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
    if(g) {dh2_lua_destroy(g->lua);std::free(g);}
}
extern "C" int dh2_gameplay_reset(dh2_gameplay *g) {
    if(!g)return 0;
    float count;
    if(!call(g,"DH2EncounterReset",nullptr,0,&count,1) || count<1 || count>3 || std::floor(count)!=count ||
       !update_state(g))return 0;
    g->count=static_cast<unsigned>(count);g->x=g->y=g->heading=g->attack_pulse=g->cooldown=0;
    const float placement[][2]={{1.4f,0.0f},{0.5f,0.4f},{0.9f,-0.3f}};
    for(unsigned i=0;i<3;++i)g->enemies[i]={placement[i][0],placement[i][1],0,0,0.8f+0.4f*i};
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

#ifdef __ANDROID__
static pthread_mutex_t session_guard=PTHREAD_MUTEX_INITIALIZER;
static dh2_gameplay *session=nullptr;
extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_GameplayActivity_sessionInit(JNIEnv *env,jclass,
        jbyteArray properties,jbyteArray classes,jbyteArray loot,jbyteArray powers,
        jbyteArray quests,jbyteArray constants,jbyteArray combat) {
    jbyteArray inputs[]={properties,classes,loot,powers,quests,constants,combat};
    dh2_gameplay_bytes assets[7]{};jbyte *held[7]{};char error[256]{};bool valid=true;
    for(unsigned i=0;i<7 && valid;++i) {
        const jsize length=inputs[i]?env->GetArrayLength(inputs[i]):0;
        if(length<=0 || length>4*1024*1024) {valid=false;break;}
        held[i]=env->GetByteArrayElements(inputs[i],nullptr);
        if(!held[i]) {valid=false;break;}
        assets[i]={held[i],static_cast<size_t>(length)};
    }
    dh2_gameplay *candidate=valid?dh2_gameplay_create(assets,error,sizeof(error)):nullptr;
    for(unsigned i=0;i<7;++i)if(held[i])env->ReleaseByteArrayElements(inputs[i],held[i],JNI_ABORT);
    if(env->ExceptionCheck())return nullptr;
    if(!candidate)return env->NewStringUTF(error[0]?error:"Source encounter assets rejected");
    pthread_mutex_lock(&session_guard);dh2_gameplay_destroy(session);session=candidate;
    dh2_gameplay_status(session,error,sizeof(error));pthread_mutex_unlock(&session_guard);
    return env->NewStringUTF(error);
}
extern "C" JNIEXPORT jfloatArray JNICALL
Java_local_dh2_sourceviewer_GameplayActivity_sessionStep(JNIEnv *env,jclass,jfloat mx,jfloat my,jfloat dt,jboolean attack) {
    float snapshot[21];pthread_mutex_lock(&session_guard);
    const size_t size=dh2_gameplay_step(session,mx,my,dt,attack?1:0,snapshot);
    pthread_mutex_unlock(&session_guard);
    if(!size)return nullptr;
    jfloatArray result=env->NewFloatArray(static_cast<jsize>(size));
    if(result)env->SetFloatArrayRegion(result,0,static_cast<jsize>(size),snapshot);
    return result;
}
extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_GameplayActivity_sessionStatus(JNIEnv *env,jclass) {
    char text[256];pthread_mutex_lock(&session_guard);dh2_gameplay_status(session,text,sizeof(text));
    pthread_mutex_unlock(&session_guard);return env->NewStringUTF(text);
}
extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_GameplayActivity_sessionReset(JNIEnv *env,jclass) {
    char text[256];pthread_mutex_lock(&session_guard);
    if(session)dh2_gameplay_reset(session);
    dh2_gameplay_status(session,text,sizeof(text));
    pthread_mutex_unlock(&session_guard);return env->NewStringUTF(text);
}
#endif
