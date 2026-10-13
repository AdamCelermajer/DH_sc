"""Integrate conservative source decoders without changing inventory identities."""
from collections import Counter
from pathlib import Path
from common import read_json,write_json


def run(ctx):
    import script_links,table_payloads,campaign_links
    tables=read_json(ctx.out/'data/tables.json',[])
    refs=read_json(ctx.out/'data/references.json',[])
    decoded=table_payloads.decode(ctx,tables)
    updates={r['id']:r for r in decoded['rows']}
    for filename in ('tables.json','tables-extra.json'):
        values=tables if filename=='tables.json' else read_json(ctx.out/'data'/filename,[])
        for row in values:
            if row['id'] in updates:
                row.update(updates[row['id']])
                if row.get('status')=='RESOLVED':row.update(reason=None,candidate_note=None,candidates=[],work_status='DONE')
        write_json(ctx.out/'data'/filename,values)
    by_id={r['id']:r for r in refs}
    import references as reference_resolver
    assets=read_json(ctx.out/'data/assets.json',[])
    for row in decoded['references']:
        by_id[row['id']]=reference_resolver.resolve_reference(ctx,row,tables,assets)
    refs=script_links.resolve(ctx,list(by_id.values()),tables,
                              read_json(ctx.out/'data/calls.json',[]),read_json(ctx.out/'data/scripts.json',[]))
    campaign_refs=[r for r in refs if r.get('scene_id') and r.get('command_index') is not None]
    resolved,relations=campaign_links.resolve(ctx,campaign_refs,tables,read_json(ctx.out/'data/assets.json',[]))
    by_id={r['id']:r for r in refs};by_id.update({r['id']:r for r in resolved})
    global_refs,global_relations=campaign_links.global_references(ctx,tables)
    by_id.update({r['id']:r for r in global_refs})
    relations.extend(global_relations)
    import packed_sites
    packed=packed_sites.extract(ctx,assets,tables)
    for ref in packed['references']:
        # Package sites keep their own numeric/name namespace. Cross-platform
        # file presence is useful evidence but never proves native iPad IDs.
        ref.update(resolves_to=[],status='UNRESOLVED',work_status='DONE',reason='OTHER',
                   candidate_note=ref.get('candidate_note') or 'iPad source name/ID is preserved; its native consumer has not been recovered.')
        name=str(ref.get('name') or '')
        if ref.get('reference_kind')=='script_name':
            candidates=[a for a in assets if a.get('source')=='ios-packed' and any(
                Path(p).stem.casefold()==Path(name).stem.casefold()
                and Path(p).suffix.lower() in ('.lua','.luac') for p in a.get('logical_paths',[]))]
            if len(candidates)==1:
                ref.update(status='RESOLVED',reason=None,resolves_to=[{'type':'source asset file',
                    'asset_files':reference_resolver._asset_file_metadata(candidates)}],confidence='LIKELY')
        elif Path(name).suffix.lower() in ('.bdae','.wav','.vxn','.ogg','.mp3','.lua','.luac',
                '.mlx','.mgp','.mgx','.mvx','.mvp','.png','.dds','.tga','.jpg','.jpeg','.swf','.fnt','.ttf','.otf','.xml','.bin'):
            status,found,candidates,reason=reference_resolver._resolve_file(name,assets)
            ref.update(status=status,reason=reason,candidates=reference_resolver._asset_file_metadata(candidates))
            if status=='RESOLVED':ref['resolves_to']=[{'type':'source asset file','path':name,
                'asset_files':reference_resolver._asset_file_metadata(found)}]
        by_id[ref['id']]=ref
    write_json(ctx.out/'data/packed-sites.json',packed)
    write_json(ctx.out/'counts/packed-sites.json',packed['coverage'])
    refs=list(by_id.values())
    # Final semantic joins see the complete source union, while native Lua
    # contracts remain scoped to the Android call/script ledgers.
    import lua_semantics,file_semantics,skill_slots,ios_semantics,xml_semantics,resource_semantics,character_template_semantics
    calls=read_json(ctx.out/'data/calls.json',[])
    scripts=read_json(ctx.out/'data/scripts.json',[])
    refs=lua_semantics.resolve(ctx,refs,tables,calls,scripts)
    refs=file_semantics.resolve(ctx,refs,tables,assets)
    refs=skill_slots.resolve(ctx,refs,tables,calls,scripts)
    refs=ios_semantics.resolve(ctx,refs,tables,assets,packed)
    refs=xml_semantics.resolve(ctx,refs,tables,assets)
    refs=resource_semantics.resolve(ctx,refs,tables,assets)
    refs=character_template_semantics.resolve(ctx,refs,tables,assets)
    # Update target snapshots so callers see the same payload as the canonical row.
    table_index={(r['table'],r['index']):r for r in tables}
    for ref in refs:
        for target in ref.get('resolves_to') or []:
            if (target.get('table'),target.get('index')) in table_index and target.get('source_row'):
                row=table_index[target['table'],target['index']]
                target['source_row']={k:row[k] for k in ('table','index','name','fields')}
        ref.setdefault('work_status','DONE');ref.setdefault('candidates',[])
        if ref.get('status')=='UNRESOLVED':
            reason=ref.get('reason') or ref.get('reason_code') or 'Source relationship remains unproven.'
            if reason not in ('FILE_MISSING','NAME_NO_FILE','PATH_CASE','ID_OUT_OF_RANGE','NOT_IN_CACHE','OTHER'):
                ref['candidate_note']=ref.get('candidate_note') or reason
                ref['reason']='OTHER'
            ref.setdefault('reason_code',ref.get('reason'))
    write_json(ctx.out/'data/references.json',refs)
    write_json(ctx.out/'data/payload-references.json',decoded['references'])
    write_json(ctx.out/'data/campaign-relations.json',relations)
    write_json(ctx.out/'data/enrichment.json',{'payloads':decoded['coverage'],'payload_gaps':decoded['gaps'],
        'symbolic_edges':sum(len(r.get('derived_edges',[])) for r in refs),'campaign_relations':len(relations),
        'semantic_reference_statuses':dict(Counter(r['status'] for r in refs)),
        'non_resource_reference_sites':sum(r['status']=='NOT-NEEDED' for r in refs),
        'file_path_joins':sum(bool(r.get('file_resolution')) for r in refs),
        'actor_skill_slot_joins':sum((r.get('semantic_resolution') or {}).get('resolver')=='skill_slots.v1' for r in refs),
        'ios_source_associations':sum(bool(r.get('ios_semantic_resolution')) for r in refs),
        'xml_source_associations':xml_semantics.summarize(refs),
        'resource_roles':resource_semantics.summarize(refs),
        'character_template_references':{
            attribute:dict(Counter(r['status'] for r in refs
                if (r.get('context') or {}).get('attribute')==attribute))
            for attribute in ('char_template','char_template_pydata','charpropsname_pydata')}})
    counts=read_json(ctx.out/'counts/references.json',{})
    counts.update(references=len(refs),table_rows_unresolved=sum(r['status']=='UNRESOLVED' for r in tables),
                  references_by_kind=dict(Counter(r.get('kind','') for r in refs)))
    write_json(ctx.out/'counts/references.json',counts)
    import references
    references._write_report(ctx,counts,refs,tables)
    write_json(ctx.out/'counts/enrichment.json',read_json(ctx.out/'data/enrichment.json'))
