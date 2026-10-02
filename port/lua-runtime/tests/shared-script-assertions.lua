-- Authored calls into exact original shared Lua scripts; no native objects.
assert(type(CF_SetCombatants)=='function' and type(CF_CalcDamage)=='function')
CF_ClearCombatants()
local miss,dodge=CF_CalcMissOrDodge(0,0)
assert(miss==false and dodge==false)
for _,name in ipairs({'CF_CalcDodge','CF_CalcBlock','CF_CalcCrit','CF_CalcHurt',
                     'CF_CalcPush','CF_CalcStun','CF_CalcFear','CF_CalcSlow','CF_CalcDamage'}) do
    assert(_G[name](0,0)==false)
end
assert(PlayAnim==nil and GetProp==nil and Rand==nil and AddToVFTable==nil)
-- Original animation event dispatch, detach and missing-event behavior.
local event_count=0
local ref={value=7}
AttachToAnimEvent('qa-hit',function(name,data)
    assert(name=='qa-hit' and data==ref)
    event_count=event_count+data.value
end,ref)
OnAnimEvent('qa-hit');assert(event_count==7)
DetachFromAnimEvent('qa-hit');OnAnimEvent('qa-hit');OnAnimEvent('missing')
assert(event_count==7)
-- Original default skill and registration/select/unselect behavior.
local usable,active=OnSkillCheck();assert(usable==false and active==false)
assert(OnPreSkill()==false and OnSkill()==false)
local total=0
DeclareSkill('qa-skill',42)
RegisterSkillEx({update=function(delta)total=total+delta end,
    check=function()return true,false end,pre=function(arg)return arg==5 end,
    skill=function(arg)total=total+arg;return true end})
SetSkill('qa-skill');OnSkillUpdate(3);assert(total==3)
usable,active=OnSkillCheck();assert(usable==true and active==false)
assert(OnPreSkill(5)==true and OnPreSkill(6)==false)
assert(OnSkill(7)==true and total==10)
OnSkillCleanUp();OnPostSkill();OnDelayedSkill();OnSkillInfo()
-- Duplicate registration and incomplete registration retain earlier/default code.
RegisterSkillEx({update=function()error('duplicate replaced')end})
SetSkill('qa-skill');OnSkillUpdate(2);assert(total==12)
DeclareSkill('incomplete',43);RegisterSkillEx({update=function()end})
SetSkill('incomplete');usable,active=OnSkillCheck();assert(usable==false and active==false)
SetSkill('qa-skill');UnsetSkill();usable,active=OnSkillCheck();assert(usable==false and active==false)
-- No combatants, timers, entity state or game loop are modeled here.
