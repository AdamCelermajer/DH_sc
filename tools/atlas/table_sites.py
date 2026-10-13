"""Source-schema decoders for actors, AI, items, faeries and dictionaries.

These are independent readers of the read-only serialized tables. Bounds,
section names, field order, and consumption are checked before rows are emitted.
"""
from collections import Counter
from pathlib import Path
import struct
from common import stable_id, source_site, write_json, read_json

class Reader:
    def __init__(self,p):self.b=Path(p).read_bytes();self.at=0
    def take(self,n):
        if n<0 or self.at+n>len(self.b):raise ValueError(f'truncated table at {self.at}, size {n}')
        v=self.b[self.at:self.at+n];self.at+=n;return v
    def i(self):return struct.unpack('<i',self.take(4))[0]
    def u(self):return struct.unpack('<I',self.take(4))[0]
    def f(self):return struct.unpack('<f',self.take(4))[0]
    def byte(self):return self.take(1)[0]
    def count(self):
        n=self.u()
        if n>100000:raise ValueError('count outside native bound')
        return n
    def text(self):return self.take(self.count()).decode('latin1').rstrip('\0')
    def strings(self):return [self.text() for _ in range(self.count())]
    def ints(self):return [self.i() for _ in range(self.count())]
    def end(self):
        if self.at!=len(self.b):raise ValueError(f'unconsumed suffix {len(self.b)-self.at}')

def groups(p):
    r=Reader(p);out=[]
    while r.at<len(r.b):out.append(r.strings())
    return out

def extract(ctx):
    py=ctx.inputs/'assets-extra/android/data/pydata';rows=[];refs=[];errors=[];consumption=[]
    def row(stem,gi,i,name,fields,evidence,status='RESOLVED',reason=None):
        table=f'{stem}#group{gi}'
        result={'id':stable_id('table',table,i,name),'table':table,'source_table':stem,'section':gi,
                'index':i,'name':name,'fields':fields,'status':status,'work_status':'DONE',
                'reason':reason,'confidence':'PROVEN','schema_evidence':evidence,
                'source_file':source_site(ctx,py/f'{stem}_pyarray.bin')}
        rows.append(result);return result
    def edge(rr,field,value,kind,target=None,path=None,rule=None):
        if isinstance(value,int) and value<0:return
        if value=='':return
        site=f'{rr["source_file"]}[section={rr["section"]},row={rr["index"]}].{field}'
        result={'id':stable_id('cell-ref',site,value,kind),'site':site,'name':str(value),'kind':kind,
                'rule':rule or f'{rr["schema_evidence"]}; field {field}',
                'status':'TODO','work_status':'TODO','reason':None,'candidates':[],
                'confidence':'PROVEN','port_status':'PARTIAL','implementation':rr['schema_evidence'],
                'context':{'file':rr['source_file'],'table':rr['table'],'index':rr['index'],'field':field},
                'resolves_to':[]}
        if target:
            result.update(referenced_table=target,referenced_index=value)
            result['resolves_to']=[{'type':'table-row','table':target,'index':value}]
        elif path:result['resolves_to']=[{'type':'file','path':path}]
        refs.append(result)
    def names(stem):return groups(py/f'{stem}_pyarraynames.bin')
    def record_used(stem,r,partial=False):
        consumption.append({'table':stem,'consumed':r.at,'size':len(r.b),'suffix':len(r.b)-r.at,'partial':partial})
        if not partial:r.end()
    def attempt(stem,fn):
        start=len(rows);refstart=len(refs)
        try:fn()
        except Exception as e:
            del rows[start:];del refs[refstart:]
            errors.append({'table':stem,'error':str(e)})
    # Exact count + 224 signed words per CharacterTable record, data.cpp:25.
    def characters():
        nn=names('character_properties');fields=groups(py/'character_properties_pystructnames.bin')[0]
        if len(fields)!=224:raise ValueError('CharacterTable must have 224 fields')
        r=Reader(py/'character_properties_pyarray.bin');count=r.count()
        if count!=len(nn[0]):raise ValueError('CharacterTable names/count mismatch')
        roles={'AIFaction':('faction','ai_factions#group0'),'AI':('script','ai#group0'),
               'AnimTable':('anim','character_animations'),'ModelFile':('model','character_models_dictionary#group0'),
               'Effects':('effect set','character_effects'),'Sounds':('sound','sounds#group4'),
               'Loot':('loot','loot_table#group5'),'ClassID':('class','character_classes#group0'),
               'SkillTree':('skill','skills#group0'),'FaeryList':('faery','faeries#group0'),
               'RangeProjectileID':('projectile','projectiles#group0')}
        for i,name in enumerate(nn[0]):
            fields_values=dict(zip(fields,[r.i() for _ in fields]))
            rr=row('character_properties',0,i,name,fields_values,'port/game-data/data.cpp:25')
            for field,(kind,target) in roles.items():edge(rr,field,fields_values[field],kind,target)
        record_used('character_properties',r,partial=True)
        # Additional names belong to source class/stat auto-assignment streams.
        # Their raw payload is retained as unresolved; never counted as actors.
        for gi,gg in enumerate(nn[1:],1):
            for i,name in enumerate(gg):row('character_properties',gi,i,name,{},'port/game-data/data.cpp:25',
                'UNRESOLVED','OTHER')
    attempt('character_properties',characters)
    # Generic dictionary = equal-length named/string value arrays (data.cpp:35).
    for stem in ('character_models_dictionary','animations_dictionary','effects_dictionary','projectiles_dictionary','game_objects_dictionary'):
        def dictionary(stem=stem):
            nn=names(stem);r=Reader(py/f'{stem}_pyarray.bin');paths=r.strings();r.end()
            if len(nn)!=1 or len(paths)!=len(nn[0]):raise ValueError('dictionary dimensions mismatch')
            for i,(name,path) in enumerate(zip(nn[0],paths)):
                rr=row(stem,0,i,name,{'Path':path},'port/game-data/data.cpp:35')
                kind='anim' if stem.startswith('animations') else 'projectile' if stem.startswith('projectiles') else 'animated effect' if stem.startswith('effects') else 'model'
                edge(rr,'Path',path,kind,path=path)
            record_used(stem,r)
        attempt(stem,dictionary)
    def ai():
        nn=names('ai')[0];schema=groups(py/'ai_pystructnames.bin')[0]
        layout=[('AttackDelay','i'),('CombatBeat','i'),('CombatMusic','i'),('DelayedLoad','byte'),('Flags','i'),
                ('InteractRadius','f'),('LeashDistance','f'),('MeleeRadius','f'),('OnAggroSFX','i'),('Script','text'),
                ('SelfFX','i'),('Trophy','i'),('Type','i'),('ViewRadius','f'),('ViewRadiusNoAggro','f')]
        if schema!=[f for f,t in layout]:raise ValueError('AI field order differs')
        r=Reader(py/'ai_pyarray.bin');n=r.count()
        if n!=len(nn):raise ValueError('AI names/count mismatch')
        for i,name in enumerate(nn):
            fields={f:getattr(r,t)() for f,t in layout};rr=row('ai',0,i,name,fields,'port/game-data/ai.cpp:22')
            for f in ('CombatMusic','OnAggroSFX'):edge(rr,f,fields[f],'sound','sdd_dungeon_hunter_2_iphone')
            edge(rr,'SelfFX',fields['SelfFX'],'effect set','effect_sets')
            # Special built-in AI tokens are resolved as engine policies, not files.
            edge(rr,'Script',fields['Script'],'script',path=None,rule='CharAI::SetScriptByName; scripts/ai/<name>.luac or native __policy__')
        record_used('ai',r)
    attempt('ai',ai)
    def factions():
        nn=names('ai_factions')[0];r=Reader(py/'ai_factions_pyarray.bin');n=r.count()
        if n!=len(nn):raise ValueError('faction dimensions')
        for i,name in enumerate(nn):
            rel=[{'Id':r.i(),'Value':r.i()} for _ in range(r.count())];rr=row('ai_factions',0,i,name,{'factions':rel},'port/game-data/ai.cpp:36')
            for j,v in enumerate(rel):edge(rr,f'factions[{j}].Id',v['Id'],'faction','ai_factions#group0')
        record_used('ai_factions',r)
    attempt('ai_factions',factions)
    def faeries():
        nn=names('faeries');r=Reader(py/'faeries_pyarray.bin');n=r.count()
        if n!=len(nn[0]):raise ValueError('faery list count')
        for i,name in enumerate(nn[0]):
            vals=r.ints();rr=row('faeries',0,i,name,{'List':vals},'port/game-data/faery_tables.cpp:56')
            for j,v in enumerate(vals):edge(rr,f'List[{j}]',v,'faery','faeries#group1')
        n=r.count()
        if n!=len(nn[1]):raise ValueError('faery count')
        for i,name in enumerate(nn[1]):
            fields={f:r.i() for f in ('Description','Elemental','ModelFile','Name')};fields['SpellScript']=r.text()
            fields.update(SpellType=r.i(),Type=r.i());rr=row('faeries',1,i,name,fields,'port/game-data/faery_tables.cpp:24')
            edge(rr,'ModelFile',fields['ModelFile'],'model','character_models_dictionary#group0')
            edge(rr,'SpellScript',fields['SpellScript'],'script',path='scripts/skills/'+fields['SpellScript']+'.luac')
        record_used('faeries',r)
    attempt('faeries',faeries)
    def items():
        stem='loot_table';nn=names(stem);sf=groups(py/'loot_table_pystructnames.bin');r=Reader(py/'loot_table_pyarray.bin')
        n=r.count()
        if n!=len(nn[0]):raise ValueError('priority names')
        for i,name in enumerate(nn[0]):row(stem,0,i,name,{'Priorities':r.ints()},'port/game-data/items.cpp:62')
        n=r.count()
        if n!=len(nn[1]):raise ValueError('item pickup type names')
        for i,name in enumerate(nn[1]):row(stem,1,i,name,{'Type':r.byte(),'Visual':r.byte()},'port/game-data/items.cpp:63')
        n=r.count()
        if n!=len(nn[2]):raise ValueError('item list names')
        for i,name in enumerate(nn[2]):
            vals=[]
            for j in range(r.count()):vals.append({'Item':r.i(),'Prob':struct.unpack('<h',r.take(2))[0],'Quantity':r.byte()})
            rr=row(stem,2,i,name,{'List':vals},'port/game-data/loot_tables_v2.cpp:34')
            for j,v in enumerate(vals):edge(rr,f'List[{j}].Item',v['Item'],'item','loot_table#group3')
        fields=sf[1]
        if len(fields)!=38:raise ValueError('item fields dimensions')
        n=r.count()
        if n!=len(nn[3]):raise ValueError('item count')
        for i,name in enumerate(nn[3]):
            vals={}
            for field in fields:
                vals[field]=r.text() if field in ('IconName','ModularModule') else r.byte() if field=='Stackable' else r.f() if field.endswith('EquipValue') else r.i()
            rr=row(stem,3,i,name,vals,'port/game-data/items.cpp:43')
            for f,kind,target in [('SwooshSoundFX','sound','sdd_dungeon_hunter_2_iphone'),('SwooshFX','effect set','effect_sets'),
                 ('AudioVisualID','model','loot_audiovisual#group0')]:edge(rr,f,vals[f],kind,target)
            for f,kind in [('ModularModule','model component'),('IconName','image')]:
                edge(rr,f,vals[f],kind,rule='Item metadata stores a named modular node/icon; a matching file name alone is insufficient to prove its consumer.')
        n=r.count()
        if n!=len(nn[4]):raise ValueError('loot string count')
        for i,name in enumerate(nn[4]):row(stem,4,i,name,{'Value':r.text()},'port/game-data/loot_tables_v2.cpp:35')
        n=r.count()
        if n!=len(nn[5]):raise ValueError('loot count')
        for i,name in enumerate(nn[5]):
            vals={'RollType':r.i(),'NumRandomItemProbs':r.i()}
            rr=row(stem,5,i,name,vals,'port/game-data/loot_tables_v2.cpp:22')
            for field in ('RandomLootEntries','FixedLootEntries'):
                entries=[]
                for j in range(r.count()):
                    fields8=['ItemListID','ItemPowerList','MProb','NumPowerProbs','Pct','Prob','RProb','WProb']
                    v={f:r.i() for f in fields8};entries.append(v)
                    edge(rr,f'{field}[{j}].ItemListID',v['ItemListID'],'item','loot_table#group2')
                    edge(rr,f'{field}[{j}].ItemPowerList',v['ItemPowerList'],'item power','item_powers#group0')
                vals[field]=entries
            vals['SubLoots']=r.ints()
            for j,v in enumerate(vals['SubLoots']):edge(rr,f'SubLoots[{j}]',v,'loot','loot_table#group5')
        record_used(stem,r,partial=True)
    attempt('loot_table',items)
    def powers():
        nn=names('item_powers');r=Reader(py/'item_powers_pyarray.bin');n=r.count()
        if n!=len(nn[0]):raise ValueError('power lists count')
        for i,name in enumerate(nn[0]):
            vals=[{'Power':r.i(),'Prob':struct.unpack('<b',r.take(1))[0]} for _ in range(r.count())];rr=row('item_powers',0,i,name,{'List':vals},'port/game-data/item_power_tables_v5.cpp:30')
            for j,v in enumerate(vals):edge(rr,f'List[{j}].Power',v['Power'],'item power','item_powers#group1')
        n=r.count()
        if n!=len(nn[1]):raise ValueError('powers count')
        for i,name in enumerate(nn[1]):
            vals={'Palette':r.byte(),'SpecialEffect':r.i()}
            vals['AttrBonusList']=[{'Attr':r.i(),'Bonus':r.i(),'Flags':r.i()} for _ in range(r.count())]
            vals.update({f:r.i() for f in ('Description','GoldBonusMultiplier','SortingOrder','GoldBonus','AttrMonopoly')})
            rr=row('item_powers',1,i,name,vals,'port/game-data/item_power_tables_v5.cpp:19')
            # SpecialEffect is an authored effect-set index; leave exact target
            # type evidence visible so a consumer can audit version differences.
            edge(rr,'SpecialEffect',vals['SpecialEffect'],'effect set','effect_sets')
        record_used('item_powers',r)
    attempt('item_powers',powers)
    # These dictionaries and visual triplets have independent recovered readers.
    def loot_av():
        nn=names('loot_audiovisual')[0];r=Reader(py/'loot_audiovisual_pyarray.bin');n=r.count()
        if n!=len(nn):raise ValueError('loot audiovisual count')
        for i,name in enumerate(nn):
            vals={'AudioDrop':r.i(),'AudioPickup':r.i(),'Visual':r.text()};rr=row('loot_audiovisual',0,i,name,vals,'port/game-data/loot_audiovisual_v8.cpp:10')
            for f in ('AudioDrop','AudioPickup'):edge(rr,f,vals[f],'sound','sdd_dungeon_hunter_2_iphone')
            edge(rr,'Visual',vals['Visual'],'model',path=vals['Visual'])
        record_used('loot_audiovisual',r)
    attempt('loot_audiovisual',loot_av)
    def levels():
        nn=names('levels');schemas=groups(py/'levels_pystructnames.bin');r=Reader(py/'levels_pyarray.bin')
        layout0=[('DescriptionId','i'),('EntryPointId','i'),('LevelName','text'),('LocationType','i'),('StringId','i')]
        layout1=[('Dbg_IsStable','byte'),('DynamicBusRouting','text'),('Hub','i'),('IsRandom','byte'),
                 ('LevelDescription','i'),('LevelFile','text'),('LevelName','i'),('LevelState','i'),('MapName','i'),
                 ('MonsterLvlMax','i'),('MonsterLvlMaxHard','i'),('MonsterLvlMaxNightmare','i'),
                 ('MonsterLvlMin','i'),('MonsterLvlMinHard','i'),('MonsterLvlMinNightmare','i')]
        for gi,layout in enumerate((layout0,layout1)):
            if schemas[gi]!=[f for f,t in layout] or r.count()!=len(nn[gi]):raise ValueError('level schema/count differs')
            for i,name in enumerate(nn[gi]):
                vals={f:getattr(r,t)() for f,t in layout};rr=row('levels',gi,i,name,vals,'port/game-data/level_tables.cpp:44')
                if gi==0:
                    # The original fast-travel operand is a level row NAME.
                    edge(rr,'LevelName',vals['LevelName'],'level',rule='FastTravel.LevelName is the exact Levels table row name (level_tables.cpp:44).')
                else:
                    edge(rr,'LevelFile',vals['LevelFile'],'level',path=vals['LevelFile'])
        record_used('levels',r)
    attempt('levels',levels)
    return rows,refs,{'rows':len(rows),'reference_cells':len(refs),'by_table':dict(Counter(r['table'] for r in rows)),
                       'by_kind':dict(Counter(r['kind'] for r in refs)),'consumption':consumption,'decode_errors':errors}

def run(ctx,inventory_only=False):
    tables,refs,counts=extract(ctx)
    if inventory_only:
        for r in tables:r.update(status='TODO',work_status='TODO')
    write_json(ctx.out/'data/tables-extra.json',tables)
    write_json(ctx.out/'data/reference-cells-extra.json',refs)
    write_json(ctx.out/'counts/table-sites.json',counts)
    return counts

def coverage_check(ctx):
    tables,refs,counts=extract(ctx);have_t={r['id'] for r in read_json(ctx.out/'data/tables.json',[])}
    have_r={r['id'] for r in read_json(ctx.out/'data/references.json',[])}
    return ['missing decoded table row: '+r['table']+':'+str(r['index']) for r in tables if r['id'] not in have_t] + \
           ['missing source cell reference: '+r['site'] for r in refs if r['id'] not in have_r] + \
           ['table decode failed: '+e['table']+' '+e['error'] for e in counts['decode_errors']]
