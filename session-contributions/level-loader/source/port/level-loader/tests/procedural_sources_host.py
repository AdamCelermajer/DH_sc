"""Source-bind rule roots and file-listed MGX graphs for all level rows.

Independent XML reads compare complete selected element trees. For malformed
rule files, the original ARM parser supplies the retained first root; parser
errors stay explicit. This does not execute typed rule/layout generation,
select gameplay/visual placement variants, or instantiate runtime objects.
"""
import argparse,collections,hashlib,json,os,pathlib,subprocess,sys,zipfile,xml.etree.ElementTree as ET
ap=argparse.ArgumentParser()
ap.add_argument('--cache',type=pathlib.Path,required=True)
ap.add_argument('--probe',type=pathlib.Path,required=True)
ap.add_argument('--inventory',type=pathlib.Path,required=True)
ap.add_argument('--original-lists',type=pathlib.Path,required=True)
ap.add_argument('--engine',type=pathlib.Path,required=True)
ap.add_argument('--dependency-root',type=pathlib.Path,required=True)
ap.add_argument('--out',type=pathlib.Path,required=True)
ap.add_argument('--swamp2-out',type=pathlib.Path)
ap.add_argument('--sanitizers',action='store_true')
a=ap.parse_args();root=pathlib.Path(__file__).resolve().parents[1]
inventory=json.loads(a.inventory.read_text());lists=json.loads(a.original_lists.read_text())
cache_sha=hashlib.sha256(a.cache.read_bytes()).hexdigest()
assert cache_sha==inventory['cache_sha256']==lists['cache_sha256']=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
assert lists['original_execution_complete']
assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==lists['engine_sha256']=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
sys.path.insert(0,str(a.dependency_root));sys.path.insert(0,str(root/'tests'))
from xml_original_probe import Parser
from unicorn import UC_HOOK_CODE
list_rows={row['name']:row for row in lists['cases'] if row['name'].startswith('data/')}
original=None;original_calls=0;source_hashes={};partial_sources={}
def tree(e):return {'tag':e.tag,'attributes':e.attrib,'children':[tree(c) for c in e]}
def projection(pack,key,tag):
    global original,original_calls
    raw=pack.read(prefix+key);source_hashes[key]=hashlib.sha256(raw).hexdigest()
    try:return tree(ET.fromstring(raw)),True
    except ET.ParseError:
        if key not in partial_sources:
            if original is None:
                original=Parser(a.engine,{'functions':[]});original.uc.hook_add(UC_HOOK_CODE,original.allocation)
            parsed=original.parse(raw);original_calls+=1
            assert parsed['root'] and parsed['root']['tag']==tag
            partial_sources[key]={'tree':parsed['root'],'original_xml_error':parsed['xml_error'],
                'original_error_description':parsed['error_description']}
        return partial_sources[key]['tree'],not bool(partial_sources[key]['original_xml_error'])
cmd=[str(a.probe.resolve())];cache_path=str(a.cache.resolve())
if os.name=='nt':
    absolute=a.probe.resolve();cmd=['wsl','-d','Ubuntu','--','/mnt/'+absolute.drive[0].lower()+absolute.as_posix()[2:]]
    absolute=a.cache.resolve();cache_path='/mnt/'+absolute.drive[0].lower()+absolute.as_posix()[2:]
if a.sanitizers:
    prefix_cmd=['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1']
    cmd=cmd[:4]+prefix_cmd+cmd[4:] if os.name=='nt' else prefix_cmd+cmd
prefix='com.gameloft.android.GAND.GloftD2SS/files/'
rows=[];block_occurrences=0
with zipfile.ZipFile(a.cache) as pack:
    for level in inventory['levels']:
        row={k:level[k] for k in ('name','file','random')}
        if not level['file'].endswith('.rule.xml'):
            row['procedural_sources']='fixed_definition';rows.append(row);continue
        run=subprocess.run(cmd+[cache_path,level['name'],level['file']],text=True,capture_output=True)
        if run.returncode:
            row.update(procedural_sources='dependency_blocked',reason=run.stderr.strip());rows.append(row);continue
        native=json.loads(run.stdout)
        rule_uri=('data/scene/'+level['file'] if '/' not in level['file'] else level['file']).replace('\\','/').lower()
        expected,parsed=projection(pack,rule_uri,'rules')
        assert native['validation']=='PASS' and native['rule']['tree']==expected
        assert native['rule']['uri']==rule_uri and native['rule']['parsed']==parsed, (level['name'],native['rule']['uri'],rule_uri,native['rule']['parsed'],parsed,native['rule']['diagnostic'])
        folder=expected['attributes'].get('folder','')
        list_uri=(folder+'/mgx/mgxlist.txt').replace('\\','/').lower()
        raw=pack.read(prefix+list_uri);source_hashes[list_uri]=hashlib.sha256(raw).hexdigest()
        assert source_hashes[list_uri]==list_rows[list_uri]['input_sha256']
        assert native['folder']==folder and native['file_list_uri']==list_uri and native['file_list_bytes']==len(raw)
        assert native['filenames']==list_rows[list_uri]['rows']
        filenames=[name for name in list_rows[list_uri]['rows'] if '.mgx' in name]
        assert len(native['blocks'])==len(filenames)
        document_keys={rule_uri}
        for actual,filename in zip(native['blocks'],filenames):
            uri=(folder+'/mgx/'+filename).replace('\\','/').lower()
            expected_block,parsed_block=projection(pack,uri,'Module')
            at=filename.rfind('.mgx');name=filename[:at]+filename[at+4:]
            assert actual['filename']==filename and actual['name']==name and actual['uri']==uri
            assert actual['tree']==expected_block and actual['parsed']==parsed_block
            document_keys.add(uri)
        assert native['document_count']==len(document_keys) and native['ownership_checks']
        assert not native['layout_generation_verified'] and not native['selected_placement_graph_verified']
        row.update(procedural_sources='source_compared',blocks=len(filenames),documents=len(document_keys),
            rule_parsed=parsed,rule_diagnostic=native['rule']['diagnostic'])
        rows.append(row);block_occurrences+=len(filenames)
        if level['name']=='SWAMP_02' and a.swamp2_out:a.swamp2_out.write_text(json.dumps(native,indent=2)+'\n')
paths=[root/'procedural_sources_v1.cpp',root/'procedural_sources_v1.hpp',root/'procedural_file_list_v1.cpp',
    root/'procedural_file_list_v1.hpp',root/'tests/procedural_sources_probe.cpp',root/'CMakeLists.txt',pathlib.Path(__file__)]
report={'validation':'PASS','scope':__doc__,'cache_sha256':cache_sha,'engine_sha256':lists['engine_sha256'],
    'inventory_sha256':hashlib.sha256(a.inventory.read_bytes()).hexdigest(),
    'original_lists_sha256':hashlib.sha256(a.original_lists.read_bytes()).hexdigest(),
    'original_parser_script_sha256':hashlib.sha256((root/'tests/xml_original_probe.py').read_bytes()).hexdigest(),
    'sources_sha256':{str(p.relative_to(root)):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},
    'probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),'authored_sources_sha256':source_hashes,
    'levels':rows,'summary':dict(collections.Counter(r['procedural_sources'] for r in rows)),
    'block_occurrences_compared':block_occurrences,'original_partial_xml_calls':original_calls,
    'partial_xml_sources':partial_sources,'sanitizers':a.sanitizers,
    'layout_generation_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','summary':report['summary'],'block_occurrences_compared':block_occurrences,
    'original_partial_xml_calls':original_calls,'dependency_blocked':[r for r in rows if r['procedural_sources']=='dependency_blocked']}))
