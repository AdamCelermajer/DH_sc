"""Original factory dispatch registry and raw cache declaration-type coverage.

The factory table is read from the original GetNewObject instruction sequence.
Declaration counts cover XML documents in the cache once each, not selected
levels, procedural occurrences or active objects. Original registration does
not establish native runtime support; factory callbacks are not executed.
"""
import argparse,collections,hashlib,json,pathlib,struct,sys,zipfile
import xml.etree.ElementTree as ET
ap=argparse.ArgumentParser()
for n in ('engine','cache','dependency-root','xml-reference','out'):ap.add_argument('--'+n,type=pathlib.Path,required=True)
a=ap.parse_args();sys.path.insert(0,str(a.dependency_root))
from elftools.elf.elffile import ELFFile
raw=a.engine.read_bytes();engine_sha=hashlib.sha256(raw).hexdigest()
assert engine_sha=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
with a.engine.open('rb') as stream:
    elf=ELFFile(stream);segments=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
    def read(at,n):
        for seg in segments:
            if seg['p_vaddr']<=at and at+n<=seg['p_vaddr']+seg['p_filesz']:
                start=seg['p_offset']+at-seg['p_vaddr'];return raw[start:start+n]
        raise ValueError(hex(at))
    def word(at):return struct.unpack('<I',read(at,4))[0]
    def string(at):return read(at,128).split(b'\0')[0].decode('ascii')
    # r5 is the registry base, r4 advances 8 bytes, and its limit is 264.
    assert word(0x34b59c)==0xe3540f42
    base=(0x34b588+8+word(0x34b710))&0xffffffff
    names={}
    for s in elf.get_section_by_name('.dynsym').iter_symbols():
        if s['st_shndx']!='SHN_UNDEF':names.setdefault(s['st_value'],[]).append(s.name)
    registry=[]
    for index in range(33):
        type_at,callback=struct.unpack('<II',read(base+index*8,8))
        registry.append({'index':index,'gametype':string(type_at),'type_address':hex(type_at),
                         'factory_address':hex(callback),'factory_symbols':names.get(callback,[]),
                         'native_factory_support':'not_integrated'})
assert len({r['gametype'] for r in registry})==33
with a.cache.open('rb') as stream:cache_sha=hashlib.file_digest(stream,'sha256').hexdigest()
assert cache_sha=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
prefix='com.gameloft.android.GAND.GloftD2SS/files/'
xml_reference=json.loads(a.xml_reference.read_text())
assert xml_reference['all_cached_xml_compared'] and xml_reference['original_sha256']==engine_sha
original={r['uri'].lower():r for r in xml_reference['cases']}
counts=collections.Counter();tags=collections.Counter();documents=[];errors=[];unregistered=[];missing=[]
registered={r['gametype'] for r in registry}
with zipfile.ZipFile(a.cache) as z:
    for entry in z.infolist():
        if entry.is_dir():continue
        assert entry.filename.startswith(prefix)
        uri=entry.filename[len(prefix):]
        if pathlib.PurePosixPath(uri).suffix.lower() not in ('.xml','.mlx','.mgp','.mvp','.mvx','.mgx'):continue
        data=z.read(entry)
        digest=hashlib.sha256(data).hexdigest()
        reference=original.get(uri.lower());original_error=None
        if reference:
            assert reference['input_sha256']==digest and 'original_execution_failure' not in reference
            tree=reference['original']['root'];original_error=reference['original']['xml_error']
            method='original_ARM_first_root';assert tree is not None
        else:
            try:
                def project(node):return {'tag':node.tag,'attributes':node.attrib,'children':[project(c) for c in node]}
                tree=project(ET.fromstring(data));method='standard_XML_inventory_only'
            except ET.ParseError as error:
                errors.append({'uri':uri,'sha256':digest,'error':str(error)});continue
        per=collections.Counter()
        pending=[tree]
        while pending:
            node=pending.pop();pending.extend(reversed(node['children']));attrs=node['attributes']
            if node['tag']=='GameObject' and 'gametype' not in attrs:
                missing.append({'uri':uri,'name':attrs.get('name'),'attributes':attrs})
            if 'gametype' not in attrs:continue
            kind=attrs['gametype'];counts[kind]+=1;per[kind]+=1;tags[node['tag']]+=1
            if kind not in registered:
                unregistered.append({'uri':uri,'root_tag':tree['tag'],'tag':node['tag'],'gametype':kind,'name':attrs.get('name'),'attributes':attrs})
        documents.append({'uri':uri,'sha256':digest,'parsing_evidence':method,
                          'original_xml_error':original_error,'gametype_counts':dict(per)})
for row in registry:row['raw_cache_declarations']=counts[row['gametype']]
report={'validation':'PASS','scope':__doc__,'engine_sha256':engine_sha,'cache_sha256':cache_sha,
        'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),
        'xml_reference_sha256':hashlib.sha256(a.xml_reference.read_bytes()).hexdigest(),
        'registry_address':hex(base),'registry':registry,'raw_cache_type_counts':dict(sorted(counts.items())),
        'gametype_attribute_tags':dict(tags),'xml_documents_parsed':len(documents),'documents':documents,
        'xml_parse_errors':errors,'unregistered_declarations':unregistered,'missing_gametype_declarations':missing,
        'factory_constructor_execution_verified':False,'runtime_factory_contract_agreed':False,
        'runtime_objects_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'registry_address':hex(base),'registered_types':len(registry),'raw_cache_type_counts':dict(counts),
                  'parsed_documents':len(documents),'original_first_root_documents':sum(r['parsing_evidence']=='original_ARM_first_root' for r in documents),
                  'original_error_documents':sum(bool(r['original_xml_error']) for r in documents),
                  'parse_errors':errors,'unregistered_types':sorted({r['gametype'] for r in unregistered}),
                  'missing_gametype':len(missing),'factories':[{k:r[k] for k in ('gametype','factory_address','factory_symbols')} for r in registry]}))
