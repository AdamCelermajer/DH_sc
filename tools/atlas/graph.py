"""One typed, evidence-preserving forward/reverse index of the original corpus."""
from collections import Counter, defaultdict, deque
from pathlib import Path
import base64
import gzip
import html
import json
import re

from common import read_json, write_json, stable_id, markdown_table
import mobs


TABLE_KINDS = {
 'character_properties#group0':('Actors','actor'), 'skills#group0':('Skills','skill_list'),
 'skills#group1':('Skills','skill'), 'loot_table#group3':('Items','item'),
 'item_powers#group0':('Items','item_power_list'), 'item_powers#group1':('Items','item_power'),
 'effect_sets':('Effects','effect_set'), 'effects_dictionary#group0':('Effects','effect_asset'),
 'character_effects':('Effects','character_effects'), 'footstep_effects':('Effects','footstep_effects'),
 'animations_dictionary#group0':('Animations','animation_clip'),
 'animation_sequences':('Animations','animation_sequence'), 'character_animations':('Animations','animation_states'),
 'sdd_dungeon_hunter_2_iphone':('Audio','sound'), 'sounds#group0':('Audio','character_sounds'),
 'projectiles#group0':('Effects','projectile'), 'projectiles_dictionary#group0':('Effects','projectile_model'),
 'faeries#group1':('Actors','faery'), 'levels#group0':('Levels','level_row'),
 'levels#group1':('Levels','level_row')}
QUERY_KINDS = {'mob':'actor','boss':'actor','actor':'actor','skill':'skill','item':'item',
 'sound':'sound','effect':'effect_set','clip':'animation_clip','anim':'animation_clip',
 'sequence':'animation_sequence','projectile':'projectile','script':'script','function':'engine_function'}


def source_file(site):
    site = str(site or '').replace('\\','/')
    return re.split(r'\[|:(?:\d|script\[|string@|event@)', site, maxsplit=1)[0]


class Builder:
    def __init__(self, ctx):
        self.ctx=ctx
        self.index=mobs.Index(ctx)
        self.nodes={}; self.edges={}; self.profiles={}
        self.table_ids={(r['table'],r['index']):'table:'+r['id'] for r in self.index.tables}
        self.script_sites={s['site']:'script:'+s['id'] for s in self.index.scripts}
        self.row_owner={ref['id']:'table:'+row['id'] for key,refs in self.index.row_refs.items()
                        if (row:=self.index.rows.get(key)) for ref in refs}
        self.ref_targets={}; self.ref_sources={}

    def node(self, id, name, category, kind, **data):
        if id not in self.nodes:
            self.nodes[id]={'id':id,'name':str(name or id),'category':category,'kind':kind,**data}
        else:
            self.nodes[id].update(data)
        return id

    def edge(self, source, target, kind, label=None, **data):
        if source is None or target is None:return
        identity=['graph-edge',source,target,kind,data.get('reference_id'),data.get('call_id'),data.get('site'),label]
        if data.get('source_edge_id'):identity.append(data['source_edge_id'])
        eid=stable_id(*identity)
        self.edges[eid]={'id':eid,'from':source,'to':target,'kind':kind,'label':label or kind,**data}

    def files(self,path):
        return self.index.files(path)

    def targets(self,target):
        result=[]
        key=mobs.target_key(target)
        if key in self.table_ids:result.append(self.table_ids[key])
        if target.get('type')=='native-string-member' and target.get('namespace')=='Sounds':
            rows=[r for r in self.index.tables if r['table']=='sdd_dungeon_hunter_2_iphone' and r['name']==target.get('name')]
            if len(rows)==1:result.append('table:'+rows[0]['id'])
            else:
                result.append(self.node('symbol:'+stable_id('Sounds',target.get('name')),target.get('name'),'Audio','sound_member_name',
                                        status='UNRESOLVED',reason='Native Sounds member lookup is established; exact unique catalog row is unavailable.',fields=target))
        if target.get('type')=='campaign_script':
            id=f"campaign:{target['scene_id']}:{target['index']}"
            if id in self.nodes:return [id]
        if target.get('type')=='global_campaign_script':
            id=f"campaign:global:{target['index']}"
            if id in self.nodes:return [id]
        tid=target.get('id')
        if tid and key not in self.table_ids and target.get('type') not in ('table-row','source-table-cell','source_script'):
            id='object:'+tid
            category={'skill_icon_symbol':'UI','itemdrop_scene_object_symbol':'Items'}.get(target.get('type'),
                        'Levels' if 'object' in target.get('type','') else 'Tables')
            result.append(self.node(id,target.get('name'), category,
                                    target.get('type','source_target'),status=target.get('status','SOURCE_LINKED'),
                                    fields=target,site=target.get('source_file') or target.get('module')))
            cp=target.get('actor_id') or target.get('character_properties_id')
            if cp and 'table:'+cp in self.nodes:
                self.edge(id,'table:'+cp,'character_definition',status='INFERENCE',confidence='LIKELY',
                          rule='Static object definition association; runtime creation is not asserted.')
        path=target.get('path')
        if not path and str(target.get('table','')).lower().startswith(('data/','q:/')):path=target['table']
        paths=[path]+[f.get('path') for f in target.get('asset_files') or []]
        for path in paths:
            for asset in self.files(path):
                result.append('asset:'+asset['id'])
                if asset['path'] in self.script_sites:result.append(self.script_sites[asset['path']])
        return sorted(set(result))

    def add_sources(self):
        actors={a['id']:a for a in read_json(self.ctx.out/'data/actors.json',[])}
        bosses={b['id']:b for b in read_json(self.ctx.out/'data/bosses.json',[])}
        for row in self.index.tables:
            category,kind=TABLE_KINDS.get(row['table'],('Tables','table_row'))
            actor=actors.get(row['id'],{})
            aliases=['Mud Lord','MudLord','Swamp King'] if row['name']=='SwampKing' and kind=='actor' else []
            self.node('table:'+row['id'],row['name'],category,kind,table=row['table'],index=row['index'],
                      fields=mobs.fields(row),status=row.get('status'),confidence=row.get('confidence'),
                      aliases=aliases,reason=row.get('candidate_note') or row.get('reason'),
                      site=row.get('source_file') or row.get('source_path'),schema_evidence=row.get('schema_evidence'),
                      role=('BOSS_FLAGGED' if bosses.get(row['id'],{}).get('boss_flag_status')=='PROVEN'
                            else 'MOB' if actor.get('ai_type')==4 else 'ACTOR') if actor else None,
                      act=actor.get('scene_membership') or actor.get('act'))
        for a in self.index.assets:
            logical=a.get('logical_paths') or [a.get('relative',a['path'])]
            path=logical[0]; suffix=Path(path).suffix.lower()
            category=('Audio' if suffix in ('.wav','.vxn','.ogg','.mp3') else 'UI' if suffix in ('.swf','.fnt','.ttf','.otf')
                      or a['source']=='ios-glive' else 'Text' if suffix in ('.english','.german','.french','.italian',
                      '.spanish','.japanese','.korean','.srt','.strings') else 'Assets')
            self.node('asset:'+a['id'],Path(path).name,category,'source_asset',**{k:v for k,v in a.items() if k not in ('id','referrers')},
                      aliases=logical,fields={'logical_paths':logical,'decode_recipe':a.get('decode_recipe','identity'),
                      'package_version':a.get('package_version'),'mapping_confidence':a.get('mapping_confidence')})
            if a.get('source')=='ios-packed' and a.get('decode_confidence')=='UNRESOLVED':
                self.nodes['asset:'+a['id']]['gaps']=[{'reason':'Source bytes and logical aliases are indexed; no bounded decoder has validated this payload.',
                    'format':a.get('format'),'logical_paths':logical}]
        for script in self.index.scripts:
            sid='script:'+script['id']
            self.node(sid,script['name'],'Scripts','script',site=script['site'],status=script['status'],
                      fields={'readiness':script['readiness'],'callbacks':script['classification']['script_functions'],
                              'imports':script.get('imports',{})})
            for a in self.files(script['site']):self.edge(sid,'asset:'+a['id'],'source_file',status='RESOLVED')
            # These are recorded definitions/candidates, not a claim of VM load order.
            for call in self.index.script_calls[script['id']]:
                fid=call.get('function_id')
                if fid in self.index.functions:
                    self.edge(sid,'function:'+fid,'engine_call',label=call['name'],site=script['site']+':'+str(call['line']),
                              status=call.get('status'),port_status=call.get('port_status'),confidence=call.get('confidence'),
                              arguments=call.get('arguments'),call_id=call['id'])
                for origin in call.get('imported_from') or []:
                    if origin in self.script_sites:
                        self.edge(sid,self.script_sites[origin],'helper_definition',label=call['name'],
                                  site=script['site']+':'+str(call['line']),status='INFERENCE',confidence='LIKELY')
            closure=self.index.script_closure([script['name']])
            for include in closure['include_edges']:
                source=self.script_sites.get(include['source'])
                matches=self.index.script_names.get(Path(include['target']).stem.casefold(),[])
                if len(matches)==1:self.edge(source,'script:'+matches[0]['id'],'include',label=include['target'],
                                            status='RESOLVED',confidence='PROVEN',site=include['source']+':'+str(include['line']))
        for f in self.index.functions.values():
            self.node('function:'+f['id'],f['name'],'Functions','engine_function',status=f['status'],
                      port_status=f.get('port_status'),fields={k:v for k,v in f.items() if k not in ('id','name')})
        packed=read_json(self.ctx.out/'data/packed-sites.json',{})
        for gap in packed.get('gaps',[]):
            asset=self.nodes.get('asset:'+gap.get('source_asset_id',''))
            if asset:asset.setdefault('gaps',[]).append(gap)
        for script in packed.get('script_records',[]):
            sid='script:ios:'+script['id'];self.script_sites[script['site']]=sid
            self.node(sid,script['name'],'Scripts','script',site=script['site'],platform='ios',
                      status='SOURCE_DECODED',confidence=script['decode_confidence'],
                      fields={k:script.get(k) for k in ('logical_path','classification','source_sha256','decoded_sha256','package_version','candidate_note')})
            self.edge(sid,'asset:'+script['source_asset_id'],'source_file',status='RESOLVED')
        for function in packed.get('script_nodes',[]):
            self.node('function:ios:'+function['id'],function['name'],'Functions','ios_call_target',
                      status='UNRESOLVED',reason=function['candidate_note'],platform='ios',fields=function)
        for call in packed.get('script_calls',[]):
            self.edge('script:ios:'+call['script_id'],'function:ios:'+call['function_id'],'source_call',
                      label=call['name'],call_id=call['id'],status='SOURCE_DECODED',confidence='PROVEN',
                      site=call['site']+':'+str(call['line']),arguments=call['arguments'])
        levels=read_json(self.ctx.out/'data/levels.json',[])
        for level in levels:
            if level.get('record_type')!='scene_file':continue
            sid=self.node('scene:'+level['id'],level['name'],'Levels','scene',site=level.get('source'),
                          status=level['status'],act=level.get('act'),fields=level.get('config'))
            for a in self.files(level.get('source')):self.edge(sid,'asset:'+a['id'],'scene_file',status='RESOLVED')
            for cp in level.get('module_spawn_candidates',[]):
                actorid='table:'+str(cp.get('actor_id'))
                if actorid in self.nodes:
                    self.edge(sid,actorid,'authored_spawn_candidate',label=cp.get('object_name'),status='INFERENCE',
                              confidence='LIKELY',site=cp.get('module'),rule=cp.get('reason'))
        for package in read_json(self.ctx.out/'data/campaign.json',[]):
            sceneid='scene:'+package['scene_id']
            for script in package.get('scripts',[]):
                sid=f"campaign:{package['scene_id']}:{script['index']}"
                self.node(sid,script['name'],'Campaign','campaign_script',status=script['status'],
                          site=package.get('source_commands'),index=script['index'],fields={'scene':package['scene'],'commands':script['command_count']})
                self.edge(sceneid,sid,'campaign_script',status='RESOLVED',confidence='PROVEN')
                for command in script.get('commands',[]):
                    cid=f"command:{package['scene_id']}:{script['index']}:{command['index']}"
                    self.node(cid,command['name'],'Campaign','campaign_command',index=command['index'],status=command.get('decode_status'),
                              fields=command['fields'],native_mapping=command.get('native_mapping'),site=package.get('source_commands'))
                    self.edge(sid,cid,'command',label=str(command['index'])+' '+command['name'],status='RESOLVED',confidence='PROVEN')
        import campaign_links,package_index
        for obj in campaign_links.object_records(self.ctx,self.index.tables):
            self.targets(obj)
            oid='object:'+obj['id']
            sceneid='scene:'+obj['scene_id']
            if sceneid in self.nodes:self.edge(sceneid,oid,'authored_definition',status='SOURCE_DECODED',confidence='PROVEN')
            for field in ('character_properties','character_template'):
                relation=obj.get(field) or {}
                target=self.table_ids.get((relation.get('target_table'),relation.get('target_index')))
                if target:self.edge(oid,target,field,status='INFERENCE',confidence='LIKELY',rule=relation.get('evidence'))
        global_scripts=campaign_links.global_records(self.ctx)
        write_json(self.ctx.out/'data/global-campaign.json',global_scripts)
        for script in global_scripts:
            sid=self.node(script['graph_id'],script['name'],'Campaign','global_campaign_script',
                          status='SOURCE_DECODED',site=script['site'],index=script['index'],
                          fields={'commands':script['command_count'],'manager_script_index':script['manager_script_index']})
            for a in self.files(script['source_commands']):self.edge(sid,'asset:'+a['id'],'source_file',status='RESOLVED')
            for command in script['commands']:
                cid=self.node(command['graph_id'],command['name'],'Campaign','campaign_command',
                              index=command['index'],status='SOURCE_DECODED',site=command['site'],fields=command['fields'])
                self.edge(sid,cid,'command',label=str(command['index'])+' '+command['name'],status='RESOLVED')
        for a in self.index.assets:
            if a.get('source')!='ios-packed' or a.get('name_group_count') is None:continue
            groups=package_index.read_name_groups(self.ctx,a)
            for group,names in enumerate(groups):
                for index,name in enumerate(names):
                    id=self.node(f"ios-name:{a['id']}:{group}:{index}",name,'Tables','ios_name_row',
                                 platform='ios',status='UNRESOLVED',index=index,
                                 site=a['path'],reason='Names and stored group/ordinal decoded; native iPad table/ID semantics are unverified.',
                                 fields={'group':group,'ordinal':index,'logical_paths':a['logical_paths'],
                                         'package_version':a['package_version']})
                    self.edge('asset:'+a['id'],id,'stored_name',status='SOURCE_DECODED',confidence='PROVEN')

    def add_references(self):
        for ref in self.index.refs:
            rid='ref:'+ref['id']
            self.node(rid,ref.get('name'),'References','reference',status=ref['status'],site=ref.get('site'),
                      reference_kind=ref.get('reference_kind',ref.get('kind')),confidence=ref.get('confidence'),
                      reason=ref.get('candidate_note') or ref.get('reason'),rule=ref.get('rule'),
                      fields={'context':ref.get('context'),'candidates':[
                          {k:v for k,v in candidate.items() if k in ('id','name','path','table','index','source','label','status')}
                          if isinstance(candidate,dict) else candidate for candidate in ref.get('candidates') or []],
                              'symbolic_trace':ref.get('symbolic_trace'),'field_semantics':ref.get('field_semantics')})
            context=ref.get('context') or {}
            owner=self.row_owner.get(ref['id']) or self.script_sites.get(context.get('script'))
            if ref.get('source_definition_id'):
                owner='object:'+ref['source_definition_id']
                if owner not in self.nodes:self.node(owner,ref.get('source_definition_id'),'Levels','scene_object_definition',
                    site=ref.get('source_file'),fields={'ordinal':ref.get('object_ordinal')},status='SOURCE_DECODED')
            elif ref.get('scene_id'):
                if ref.get('command_index') is not None:
                    owner=f"command:{ref['scene_id']}:{ref['script_index']}:{ref['command_index']}"
                else:owner='scene:'+ref['scene_id']
            sources=[owner] if owner in self.nodes else []
            if not sources:
                path=context.get('file') or source_file(ref.get('site'))
                sources=['asset:'+a['id'] for a in self.files(path)]
            if not sources:
                owner=self.node('source:'+stable_id(ref.get('site')),ref.get('site'),'References','unclassified_source_site',
                                status='UNRESOLVED',reason='Reference source exists in ledger; entity ownership has not been decoded.',site=ref.get('site'))
                sources=[owner]
            self.ref_sources[ref['id']]=sources
            for owner in sources:
                self.edge(owner,rid,'reference_site',label=ref.get('reference_kind',ref.get('kind')),status=ref['status'],
                          confidence=ref.get('confidence'),site=ref.get('site'),reference_id=ref['id'],rule=ref.get('rule'))
            targets=[]
            for target in ref.get('resolves_to') or []:targets.extend(self.targets(target))
            for target in sorted(set(targets)):
                self.edge(rid,target,'resolution',status=ref['status'],confidence=ref.get('confidence'),
                          site=ref.get('site'),reference_id=ref['id'],rule=ref.get('rule'))
            self.ref_targets[ref['id']]=sorted(set(targets))
            for derived in ref.get('derived_edges') or []:
                for target in self.targets(derived.get('target') or derived):
                    self.edge(rid,target,'static_binding',status='INFERENCE',confidence=derived.get('confidence','LIKELY'),
                              reference_id=ref['id'],rule=derived.get('rule') or derived.get('evidence'),site=ref.get('site'))

    def add_table_rules(self):
        for row in self.index.tables:
            id='table:'+row['id']; values=mobs.fields(row)
            for a in self.files(row.get('source_file') or row.get('source_path')):
                self.edge(id,'asset:'+a['id'],'source_file',status='RESOLVED',confidence='PROVEN')
            if row['table']=='skills#group0':
                for position,index in enumerate(values.get('list_entries',[])):
                    self.edge(id,self.table_ids.get(('skills#group1',index)),'skill_list_member',label=f'List[{position}]',
                              status='RESOLVED',confidence='PROVEN',rule='Source SkillListTable list_entries -> SkillTable local row index.')
            if isinstance(values,dict):
                for field in ('Script','SpellScript'):
                    name=values.get(field)
                    if not isinstance(name,str) or not name:continue
                    matches=self.index.script_names.get(Path(name).stem.casefold(),[])
                    if len(matches)==1:self.edge(id,'script:'+matches[0]['id'],'script',label=field,status='RESOLVED',
                                                confidence='PROVEN',rule='Exact source script stem in a decoded consumer field.')

    def add_profiles(self):
        import entity_profiles
        profiles=entity_profiles.build(self.ctx,self.index)
        write_json(self.ctx.out/'data/entity-profiles.json',profiles)
        for p in profiles:
            root=self.table_ids.get((p['source'].get('table'),p['source'].get('index'))) or p['id']
            self.profiles[root]={'kind':p['kind'],'coverage':p['coverage'],
                'field_explanations':p.get('field_explanations',{}),
                'related_entities':sorted({'table:'+i for i in p['joined']['table_ids']+p['script_resources']['table_ids']}),
                'assets':sorted({'asset:'+i for i in p['joined']['asset_ids']+p['script_resources']['asset_ids']}),
                'scripts':['script:'+i for i in p['scripts']['script_ids']],
                'audio':['asset:'+i for i in p.get('audio_asset_ids',[])],
                'gaps':p['joined']['gaps']+p['script_resources']['gaps']+p['scripts']['gaps']}
        # Actor overview uses direct graph edges and the same canonical rows,
        # rather than copying the large legacy loot dossiers into this index.
        for row in self.index.tables:
            if row['table']!='character_properties#group0':continue
            id='table:'+row['id']
            self.profiles[id]={'kind':'actor','coverage':'SOURCE_JOINED_PARTIAL','fields':row['fields'],
                              'note':'Use dependencies for authored resources and Used by for levels/owners. Branch execution remains explicit evidence work.'}

    def add_ui_text(self):
        import ui_text_index
        data=ui_text_index.build(self.ctx,self.index.assets,self.index.tables)
        write_json(self.ctx.out/'data/ui-text-index.json',data)
        write_json(self.ctx.out/'counts/ui-text.json',data['coverage'])
        def canonical(id):return 'table:'+id[len('table-row:'):] if id.startswith('table-row:') else id
        for n in data['nodes']:
            id=canonical(n['id'])
            if id in self.nodes:continue
            category=('Text' if 'localization' in n['kind'] else 'Scripts' if any(k in n['kind'] for k in ('action','bytecode','script')) else 'UI')
            self.node(id,n['name'],category,n['kind'],status=n.get('status','SOURCE_DECODED'),source=n.get('source'),
                      fields={k:v for k,v in n.items() if k not in ('id','name','kind')},confidence=n.get('confidence','PROVEN'),
                      reason=n.get('reason') or n.get('candidate_note'),gaps=n.get('gaps'),platform=n.get('platform'))
            source=n.get('source') or ''
            candidates=self.files(source[5:] if source.startswith('ios7/') else source)
            if source.startswith('ios7/'):candidates=[a for a in candidates if a.get('source')=='ios-packed']
            for a in candidates:
                self.edge(id,'asset:'+a['id'],'source_file',status='RESOLVED',confidence='PROVEN')
        for e in data['edges']:
            self.edge(canonical(e['source_id']),canonical(e['target_id']),e['kind'],status='RESOLVED',
                      confidence='PROVEN',rule=e.get('evidence'))
        for alias in data['aliases']:
            id=('table:'+alias['table_id']) if alias.get('table_id') else canonical(alias['source_id'])
            if id not in self.nodes:continue
            node=self.nodes[id]
            node['aliases']=sorted(set(node.get('aliases',[])+[alias['alias']]))
            if alias.get('human_text'):
                node.setdefault('display_names',[])
                if alias['human_text'] not in node['display_names']:node['display_names'].append(alias['human_text'])
                if alias.get('kind') in ('actor-name','skill-name','item-name'):
                    node['display_name']=alias['human_text']

    def add_item_resources(self):
        import entity_profiles
        data=entity_profiles.resolve_item_resources(self.ctx,self.index.refs,self.index.tables,self.index.assets)
        write_json(self.ctx.out/'data/item-resource-links.json',data)
        write_json(self.ctx.out/'counts/item-resources.json',data['coverage'])
        for e in data['derived_edges']:
            source=self.table_ids.get((e['source']['table'],e['source']['index']))
            target=e.get('target') or {}
            ids=[]
            for path in (target.get('uri'),target.get('asset_uri'),target.get('template_uri')):
                if path==target.get('template_uri') and target.get('template_asset'):continue
                ids.extend('asset:'+a['id'] for a in self.files(path))
            for a in target.get('assets') or []:
                if 'asset:'+str(a.get('id')) in self.nodes:ids.append('asset:'+a['id'])
            template=target.get('template_asset') or {}
            if 'asset:'+str(template.get('id')) in self.nodes:ids.append('asset:'+template['id'])
            if target.get('sprite_id') is not None:
                # Connect to the scoped SWF sprite when the independently
                # decoded UI index supplies the same movie and character ID.
                for n in self.nodes.values():
                    fields=n.get('fields') or {}
                    if n['kind']=='swf_character' and fields.get('character_id')==target['sprite_id']:
                        source=str(fields.get('source') or n.get('source') or '')
                        if Path(source).name.casefold()==Path(str(target.get('asset_uri') or '')).name.casefold():
                            ids.append(n['id'])
            for model in target.get('models') or []:
                mid=model.get('model',{}).get('id')
                if 'table:'+str(mid) in self.nodes:ids.append('table:'+mid)
                aid=model.get('asset',{}).get('id')
                if 'asset:'+str(aid) in self.nodes:ids.append('asset:'+aid)
            for id in sorted(set(ids)):
                self.edge(source,id,e['kind'],label=e['field'],status=e['status'],confidence='LIKELY',
                          rule=e['evidence'],reason=e.get('note'),source_edge_id=e['id'])

    def build(self):
        self.add_sources();self.add_references();self.add_table_rules();self.add_profiles();self.add_ui_text();self.add_item_resources()
        # Never silently create a link to a node absent from the indexed corpus.
        errors=[e['id'] for e in self.edges.values() if e['from'] not in self.nodes or e['to'] not in self.nodes]
        if errors:raise ValueError('Graph contains dangling nodes: '+str(errors[:5]))
        counts={'nodes':len(self.nodes),'edges':len(self.edges),'by_category':dict(Counter(n['category'] for n in self.nodes.values())),
                'reference_sites':len(self.index.refs),'unresolved_reference_sites':sum(r['status']=='UNRESOLVED' for r in self.index.refs),
                'profiles':len(self.profiles),'scope':'All supplied corpus rows and files; evidence/inference/unknown distinctions are preserved.'}
        return {'schema':'dh2-atlas-graph-v1','nodes':sorted(self.nodes.values(),key=lambda n:n['id']),
                'edges':sorted(self.edges.values(),key=lambda e:e['id']),'profiles':self.profiles,'counts':counts}


def run(ctx):
    data=Builder(ctx).build()
    write_json(ctx.out/'data/graph.json',data)
    write_json(ctx.out/'counts/graph.json',data['counts'])
    render(ctx,data)
    return data['counts']


def render(ctx,data):
    # JSON object keys are strings. Native campaign field offsets can still be
    # integer keys in a fresh decode, so normalize them before sorting. This
    # keeps fresh generation and rendering an existing graph byte-identical.
    normalized=json.loads(json.dumps(data,ensure_ascii=False,separators=(',',':')))
    serialized=json.dumps(normalized,ensure_ascii=False,separators=(',',':'),sort_keys=True).encode('utf-8')
    packed=base64.b64encode(gzip.compress(serialized,mtime=0)).decode('ascii')
    template=(ctx.root/'tools/atlas/templates/explorer.html').read_text(encoding='utf-8')
    (ctx.out/'index.html').write_text(template.replace('__ATLAS_GZIP_BASE64__',packed)
                                  .replace('__ATLAS_TITLE__',html.escape('Dungeon Hunter II · Original Data Atlas')),
                                  encoding='utf-8',newline='\n')


def search(data,query):
    term=query.casefold().strip(); kind=None
    if ':' in term and term.split(':',1)[0] in QUERY_KINDS:
        key,term=term.split(':',1);kind=QUERY_KINDS[key]
    candidates=[n for n in data['nodes'] if not kind or n['kind']==kind]
    exact=[n for n in candidates if term in [n['name'].casefold(),n['id'].casefold(),
             str(n.get('index')),*(str(x).casefold() for x in n.get('aliases',[]))]]
    return exact or [n for n in candidates if term in (' '.join([n['name'],n['id'],n.get('table',''),
             n.get('path',''),*n.get('aliases',[])])).casefold()]


def lookup(ctx,query,as_json=False):
    data=read_json(ctx.out/'data/graph.json',{})
    matches=search(data,query)
    if not matches:print('No indexed entity matches '+query);return 1
    # A display name often also matches its localization source row. Open the
    # unique gameplay entity directly while preserving true entity ambiguity.
    entities=[n for n in matches if n['kind'] in ('actor','skill','item')]
    if len(entities)==1:matches=entities
    if len(matches)!=1:
        print(markdown_table(['Entity','Kind','Typed ID','Status'],[[n['name'],n['kind'],n['id'],n.get('status')] for n in matches[:100]]));return 0
    node=matches[0]; out=[e for e in data['edges'] if e['from']==node['id']]; incoming=[e for e in data['edges'] if e['to']==node['id']]
    result={'node':node,'profile':data['profiles'].get(node['id']),'dependencies':out,'used_by':incoming}
    if as_json:print(json.dumps(result,ensure_ascii=False,indent=2))
    else:
        print(node.get('display_name',node['name'])+' · '+node['kind']+' · '+str(node.get('status')))
        print('Typed ID: '+node['id']);print('Direct links: '+str(len(out))+' dependencies, '+str(len(incoming))+' users')
        print('Browser: '+str(ctx.out/'index.html')+'#'+node['id'])
        by_id={n['id']:n for n in data['nodes']};adj=defaultdict(list)
        for e in data['edges']:adj[e['from']].append(e)
        seen={node['id']};queue=deque([(node['id'],0)]);groups=defaultdict(list)
        skip={'source_file','engine_call','source_call','helper_definition','scene_file','authored_spawn_candidate','command'}
        while queue and len(seen)<5000:
            current,depth=queue.popleft()
            if depth>=12:continue
            for edge in adj[current]:
                if edge['kind'] in skip or edge['to'] in seen:continue
                seen.add(edge['to']);target=by_id[edge['to']];queue.append((target['id'],depth+1))
                if target['category']!='References':groups[target['category']].append(target)
        for category,targets in sorted(groups.items()):
            print(category+': '+str(len(targets))+' linked entities')
            for target in targets[:8]:print('  '+target['name']+' · '+target['id'])
        print('Use --json for source fields and direct relationship evidence.')
    return 0


def coverage_check(ctx):
    data=read_json(ctx.out/'data/graph.json',{})
    if not data:return ['graph: missing universal index']
    nodes={n['id']:n for n in data.get('nodes',[])}; errors=[]
    if len(nodes)!=len(data.get('nodes',[])):errors.append('graph: duplicate typed node IDs')
    for dataset,prefix in [('tables','table:'),('assets','asset:'),('scripts','script:'),('functions','function:'),('references','ref:')]:
        expected={prefix+r['id'] for r in read_json(ctx.out/f'data/{dataset}.json',[])}
        missing=expected-nodes.keys()
        if missing:errors.append(f'graph: missing {dataset} entities ({len(missing)})')
    for e in data.get('edges',[]):
        if e['from'] not in nodes or e['to'] not in nodes:errors.append('graph: dangling relationship '+e['id'])
    edges=data.get('edges',[])
    if len({e['id'] for e in edges})!=len(edges):errors.append('graph: duplicate relationship IDs')
    incoming=defaultdict(list);outgoing=defaultdict(list)
    for e in edges:
        incoming[e['to']].append(e);outgoing[e['from']].append(e)
    for ref in read_json(ctx.out/'data/references.json',[]):
        rid='ref:'+ref['id']
        if not any(e['kind']=='reference_site' for e in incoming[rid]):
            errors.append('graph: reference has no source owner '+ref['id'])
        targets=ref.get('resolves_to') or []
        expected=any(mobs.target_key(t) or t.get('path') or t.get('asset_files') or
                     t.get('type') in ('campaign_script','global_campaign_script','scene_object_definition') for t in targets)
        if ref.get('status')=='RESOLVED' and expected and not any(e['kind']=='resolution' for e in outgoing[rid]):
            errors.append('graph: resolved reference has no target '+ref['id'])
    profiles=data.get('profiles',{})
    for row in read_json(ctx.out/'data/tables.json',[]):
        if row['table'] in ('character_properties#group0','skills#group1','loot_table#group3') and 'table:'+row['id'] not in profiles:
            errors.append('graph: missing entity profile '+row['id'])
    packed=read_json(ctx.out/'data/packed-sites.json',{})
    for s in packed.get('script_records',[]):
        if 'script:ios:'+s['id'] not in nodes:errors.append('graph: missing packed script '+s['id'])
    call_ids={e.get('call_id') for e in edges}
    for c in packed.get('script_calls',[]):
        if c['id'] not in call_ids:errors.append('graph: missing packed call '+c['id'])
    ui=read_json(ctx.out/'data/ui-text-index.json',{})
    for n in ui.get('nodes',[]):
        id='table:'+n['id'][len('table-row:'):] if n['id'].startswith('table-row:') else n['id']
        if id not in nodes:errors.append('graph: missing UI/text entity '+id)
    for script in read_json(ctx.out/'data/global-campaign.json',[]):
        if script['graph_id'] not in nodes:errors.append('graph: missing global campaign script '+script['graph_id'])
        for command in script['commands']:
            if command['graph_id'] not in nodes:errors.append('graph: missing global command '+command['graph_id'])
    name_counts=Counter(id.split(':',2)[1] for id in nodes if id.startswith('ios-name:'))
    for a in read_json(ctx.out/'data/assets.json',[]):
        if a.get('source')=='ios-packed' and a.get('name_count') is not None:
            count=name_counts[a['id']]
            if count!=a['name_count']:errors.append('graph: incomplete package name stream '+a['id'])
    return errors
