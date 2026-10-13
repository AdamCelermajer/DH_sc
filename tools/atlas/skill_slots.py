"""Resolve literal DoSkill slots through the script's declared actor consumers."""
from collections import defaultdict
from copy import deepcopy
from pathlib import Path
import re

RESOLVER='skill_slots.v1'
EVIDENCE={
    'native_argument':'Character::_DoSkill 0x003b8bd8, pseudocode-all.c:134604: numeric argument, unsigned slot, slot < receiver skill-list count',
    'skill_list':'Character::GetCharSkillListId 0x003bc5c0 / GetCharSkillList 0x003bc5fc',
    'slot_to_row':'Character::GetCharSkill 0x003bc784: SkillListTable.list_entries[slot] -> SkillTable row',
    'execution_gate':'CharAI::AI_UseSkill 0x003d8868 / AI_BeginSkill 0x003d86bc: type and usability admission remain separate',
}


def resolve(ctx,refs,tables,calls,scripts):
    result=deepcopy(refs)
    by_call={c['id']:c for c in calls}
    by_script={s['id']:s for s in scripts}
    rows={(r['table'],r['index']):r for r in tables}
    by_id={r['id']:r for r in tables}
    script_ais=defaultdict(list)
    actors_by_ai=defaultdict(list)
    for row in tables:
        fields=row.get('fields') or {}
        if row['table']=='ai#group0' and isinstance(fields.get('Script'),str):
            script_ais[fields['Script']].append(row)
        elif row['table']=='character_properties#group0' and isinstance(fields.get('AI'),int):
            actors_by_ai[fields['AI']].append(row)
    for ref in result:
        context=ref.get('context') or {}
        if context.get('call')!='DoSkill' or context.get('argument_index')!=1:
            continue
        if ref.get('status')=='RESOLVED':continue
        call=by_call.get(context.get('call_id'),{})
        script=by_script.get(call.get('script_id'),{})
        if (call.get('call_mode')!='global' or call.get('name')!='DoSkill'
                or script.get('platform')!='android'):continue
        arg=next((a for a in call.get('arguments',[]) if a.get('index')==1),{})
        text=arg.get('expression','')
        literal=arg.get('literals') or []
        if (not re.fullmatch(r'\d+',text) or len(literal)!=1
                or literal[0].get('type')!='number' or literal[0].get('raw')!=text):continue
        # Only an exact AI-script load path binds the native receiver here.
        stem=Path(script.get('name','')).stem
        if not str(script.get('site','')).endswith('/scripts/ai/'+stem+'.luac'):continue
        owners=[(ai,actor) for ai in script_ais.get(stem,[]) for actor in actors_by_ai[ai['index']]]
        if not owners:continue
        slot=int(text); targets=defaultdict(list); missing=[]
        for ai,actor in owners:
            tree_id=(actor.get('fields') or {}).get('SkillTree')
            tree=rows.get(('skills#group0',tree_id))
            entries=(tree or {}).get('fields',{}).get('list_entries')
            if not isinstance(entries,list) or not 0<=slot<len(entries):
                missing.append({'actor_id':actor['id'],'skill_tree':tree_id,'slot':slot})
                continue
            target=rows.get(('skills#group1',entries[slot]))
            if not target:
                missing.append({'actor_id':actor['id'],'skill_row':entries[slot],'slot':slot})
                continue
            targets[target['id']].append({'actor_id':actor['id'],'actor_index':actor['index'],
                'ai_row_id':ai['id'],'skill_list_id':tree['id'],'skill_list_index':tree_id,'slot':slot})
        proof={'resolver':RESOLVER,'rule':'Declared Android AI.Script consumers -> actor SkillTree -> zero-based SkillList slot -> exact SkillTable row',
               'evidence':EVIDENCE,'call_id':call['id'],'script':script['site'],'slot':slot,
               'scope':'Static declared script consumers; execution and active/usability gates are not established.',
               'missing_consumer_bindings':missing}
        ref['semantic_resolution']=proof
        if missing or not targets:
            ref.update(status='UNRESOLVED',reason='OTHER',reason_code='OTHER',work_status='DONE',
                candidate_note='Not every declared script consumer has a valid indexed skill-list entry at this slot.',
                candidates=[{'row_id':id,'actors':bindings} for id,bindings in sorted(targets.items())])
            continue
        resolutions=[]
        for id,bindings in sorted(targets.items()):
            row=by_id[id]
            resolutions.append({'type':'table-row','table':row['table'],'index':row['index'],
                'name':row['name'],'row_id':id,'actor_skill_slot_bindings':bindings})
        ref.update(status='RESOLVED' if len(resolutions)==1 else 'AMBIGUOUS',work_status='DONE',
            reason=None,reason_code=None,confidence='PROVEN',rule=proof['rule'],
            resolves_to=resolutions,candidates=[] if len(resolutions)==1 else resolutions)
        ref.pop('candidate_note',None)
        ref['reference_kind']='actor_skill_slot'
    return result
