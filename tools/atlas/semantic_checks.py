"""Source examples and failure branches for the reported atlas reference gaps."""
from copy import deepcopy
from collections import Counter
from common import read_json
import skill_slots
import file_semantics


def _slot_fixture():
    script={'id':'script','platform':'android','name':'boss.luac',
            'site':'.local-inputs/assets-extra/android/data/scripts/ai/boss.luac'}
    call={'id':'call','name':'DoSkill','call_mode':'global','script_id':'script',
          'arguments':[{'index':1,'expression':'0','literals':[{'type':'number','raw':'0'}]}]}
    tables=[{'id':'ai','table':'ai#group0','index':1,'fields':{'Script':'boss'}},
            {'id':'actor','table':'character_properties#group0','index':1,'fields':{'AI':1,'SkillTree':3}},
            {'id':'list','table':'skills#group0','index':3,'fields':{'list_entries':[10]}},
            {'id':'skill','table':'skills#group1','index':10,'name':'BossSkill','fields':{}}]
    ref={'id':'ref','kind':'symbolic_reference','name':'0','status':'UNRESOLVED',
         'reason':'OTHER','candidate_note':'Original unproven source site.','candidates':[],
         'context':{'call':'DoSkill','argument_index':1,'call_id':'call'}}
    return [ref],tables,[call],[script]


def run(ctx):
    tests=[]
    def record(name,passed,detail=None):
        tests.append({'name':name,'passed':bool(passed),'detail':detail})
    fixture=_slot_fixture()
    output=skill_slots.resolve(ctx,*fixture)
    record('literal receiver slot resolves through the actor list',
           output[0]['status']=='RESOLVED' and output[0]['resolves_to'][0]['index']==10)
    record('skill-slot resolver preserves inputs and reference IDs',
           fixture==_slot_fixture() and output[0]['id']=='ref')
    record('skill-slot annotations are idempotent',
           output==skill_slots.resolve(ctx,output,*fixture[1:]))
    for label,mutate,expected in [
        ('dynamic slot remains unproven',lambda x:x[2][0]['arguments'][0].update(expression='slot'), 'UNRESOLVED'),
        ('object receiver is not assigned the AI owner',lambda x:x[2][0].update(call_mode='object_method'),'UNRESOLVED'),
        ('iOS native slot semantics remain separate',lambda x:x[3][0].update(platform='ios'),'UNRESOLVED'),
        ('out-of-range slot remains a gap',lambda x:x[2][0]['arguments'][0].update(expression='1',literals=[{'type':'number','raw':'1'}]),'UNRESOLVED'),
        ('missing selected skill row remains a gap',lambda x:x[1].pop(),'UNRESOLVED'),
        ('a script with no declared actor is not assigned a skill',lambda x:x[1][1]['fields'].update(AI=2),'UNRESOLVED'),
    ]:
        f=deepcopy(fixture);mutate(f)
        record(label,skill_slots.resolve(ctx,*f)[0]['status']==expected)
    f=deepcopy(fixture)
    f[1].extend([{'id':'other','table':'character_properties#group0','index':2,'fields':{'AI':1,'SkillTree':4}},
                 {'id':'list2','table':'skills#group0','index':4,'fields':{'list_entries':[11]}},
                 {'id':'skill2','table':'skills#group1','index':11,'name':'OtherSkill','fields':{}}])
    ambiguous=skill_slots.resolve(ctx,*f)[0]
    record('shared script with divergent lists remains ambiguous',
           ambiguous['status']=='AMBIGUOUS' and {t['index'] for t in ambiguous['resolves_to']}=={10,11})
    refs=read_json(ctx.out/'data/references.json',[]);by_id={r['id']:r for r in refs}
    reported={
        '0d186b6ccfc617e9d39a':('swampking_core AI script','RESOLVED'),
        '743f8745636ccbb0499c':('swampking_fall duplicate literal','NOT-NEEDED'),
        'c5a58ff1eb28ca067fde':('OnDied callback key','NOT-NEEDED'),
        '8c36a97d971353b03f42':('swampking_emerge animation expression','RESOLVED'),
        'b714a3a5f083a9907255':('AIStates Skill enum','NOT-NEEDED'),
        'ac3c663b281a71a82e15':('OnEndOfAnim callback key','NOT-NEEDED'),
        'df000b32e8334c45e104':('swampking_intimidate duplicate literal','NOT-NEEDED'),
        'd523815fb8b44427da67':('CharacterProperties duplicate schema token','NOT-NEEDED'),
        '38b8bbb0677e188f2373':('PlayMusic numeric duration','NOT-NEEDED'),
        '97dc7f1627198ca64046':('PlaySound numeric control','NOT-NEEDED'),
        '6a83569c58692faa7eaa':('StopSound exact sound member','RESOLVED'),
    }
    for id,(label,expected) in reported.items():
        row=by_id.get(id,{})
        record('reported '+label,row.get('status')==expected,
               {'reference_id':id,'actual':row.get('status')})
    emerge=by_id.get('8c36a97d971353b03f42',{})
    record('emerge joins the exact animation dictionary row 1321',
           any(t.get('table')=='animations_dictionary#group0' and t.get('index')==1321
               for t in emerge.get('resolves_to',[])))
    enum=by_id.get('b714a3a5f083a9907255',{}).get('semantic_resolution',{}).get('evidence',{})
    record('Skill is AIStates value 6 rather than SkillTable row 6',
           enum.get('namespace')=='AIStates' and enum.get('signed_value')==6)
    core_refs=[r for r in refs if r.get('context',{}).get('script','').endswith('/ai/swampking_core.luac')]
    for namespace,name,value in [('AITargetList_CharacterFilter','Player',128),
                                  ('AITargetList_ObjectFilter','NONE',2),
                                  ('AITargetList_Sorting','ClosestFirst',1)]:
        matching=[r for r in core_refs if r.get('context',{}).get('call')=='GetPyCst'
                  and r.get('name')=='"'+name+'"']
        record('target-list constant '+namespace+'.'+name,
            bool(matching) and all(r.get('status')=='NOT-NEEDED'
                and r.get('semantic_resolution',{}).get('evidence',{}).get('namespace')==namespace
                and r.get('semantic_resolution',{}).get('evidence',{}).get('signed_value')==value
                for r in matching))
    for name in ('OnInitFinal','OnTargetInCloseRange','OnTargetDied','OnTerminate'):
        matching=[r for r in core_refs if r.get('context',{}).get('call')=='AddToVFTable'
                  and r.get('name')=='"'+name+'"']
        record('native callback binding '+name,bool(matching) and all(
            r.get('status')=='NOT-NEEDED' and r.get('semantic_resolution',{}).get('evidence',{}).get('native_dispatch')
            for r in matching))
    slot_refs=[r for r in refs if r.get('context',{}).get('call')=='DoSkill'
               and r.get('context',{}).get('script','').endswith('/ai/swampking_core.luac')]
    record('all 12 Mud Lord literal skill calls reach the expected slot rows',
        len({r['context']['call_id'] for r in slot_refs})==12 and all(
            r.get('status')=='RESOLVED' and any(t.get('table')=='skills#group1'
                and t.get('index')==106+int(r.get('name')) for t in r.get('resolves_to',[]))
            for r in slot_refs),{'source_calls':len({r['context']['call_id'] for r in slot_refs}),'source_reference_sites':len(slot_refs)})
    record('every non-resource classification retains an evidence rule and reason',all(
        r.get('reason') and (r.get('semantic_resolution') or {}).get('evidence')
        for r in refs if r.get('status')=='NOT-NEEDED'))
    core=by_id.get('0d186b6ccfc617e9d39a',{})
    tables=read_json(ctx.out/'data/tables.json',[]);assets=read_json(ctx.out/'data/assets.json',[])
    ios_assets=[a for a in assets if a.get('source')=='ios-packed']
    fallback=file_semantics.resolve(ctx,[core],tables,ios_assets)[0]
    record('an exact iOS-only script remains available under the cache-union policy',
        fallback.get('status')=='RESOLVED' and any(a.get('platform')=='ios'
            for t in fallback.get('resolves_to',[]) for a in t.get('asset_files',[])))
    missing=file_semantics.resolve(ctx,[core],tables,[])[0]
    record('removing all exact script assets produces an explicit gap',
           missing.get('status')=='UNRESOLVED' and bool(missing.get('candidate_note')))
    record('file resolver annotations are idempotent',
        file_semantics.resolve(ctx,[core],tables,assets)==[core])
    tex=[r for r in refs if r.get('name') in ('atlas_weapons_dh2.tga','envmap_swamp.tga')
         and r.get('file_resolution',{}).get('resolver')=='file_semantics.v1']
    record('both reported texture names reach exact source URIs with role uncertainty retained',
        {r.get('name') for r in tex}=={'atlas_weapons_dh2.tga','envmap_swamp.tga'} and all(
            r.get('status')=='RESOLVED' and r.get('confidence')=='LIKELY'
            and r['file_resolution'].get('native_equivalence')=='not-asserted'
            for r in tex),{'reference_sites':len(tex)})
    return {'passed':all(t['passed'] for t in tests),'tests':tests,
            'scope':'Static atlas source joins and rejection cases; no gameplay execution.',
            'reference_statuses':dict(Counter(r.get('status') for r in refs))}
