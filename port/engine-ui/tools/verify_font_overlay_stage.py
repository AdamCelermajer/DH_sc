"""Read-only receipt for a current core with the frozen font-v1 overlay.

This binds current bytes separately from historical façade/source proof. It
does not execute the new connected HUD, rebind old reports, or build anything.
"""
import argparse,hashlib,json,pathlib,zipfile,re

ROOT=pathlib.Path(__file__).resolve().parents[3]
def sha(data):return hashlib.sha256(data).hexdigest()
def file_sha(path):return sha(path.read_bytes())
def check_members(stock,current):
    assert set(stock)==set(current) and len(stock)==95,'core member list changed'
    changed=[n for n in stock if stock[n]!=current[n]]
    assert changed==['gameswf_font.cpp.o'],('expected exactly font TU change',changed)
    return changed
def paired_archive(members):
    blob=bytearray(b'!<arch>\n')
    for name,data in members.items():
        # BSD extended names avoid truncation, duplicate aliases and a GNU
        # string-table dependency. This comparison archive needs no link index.
        encoded=name.encode('utf8');body=encoded+data
        header=(f'{"#1/"+str(len(encoded)):<16}{0:<12}{0:<6}{0:<6}{"100644":<8}{len(body):<10}`\n').encode('ascii')
        assert len(header)==60
        blob.extend(header);blob.extend(body)
        if len(body)&1:blob.extend(b'\n')
    return bytes(blob)
def archive_members(path,contents=False):
    blob=path.read_bytes()
    if not blob.startswith(b'!<arch>\n'):raise ValueError('not a normal Unix archive: '+str(path))
    offset=8;strings=b'';members={}
    while offset<len(blob):
        header=blob[offset:offset+60]
        if len(header)!=60 or header[58:60]!=b'`\n':raise ValueError('malformed archive header')
        length=int(header[48:58].strip());name=header[:16].decode('ascii').strip();offset+=60
        data=blob[offset:offset+length]
        if len(data)!=length:raise ValueError('short archive member')
        offset+=length+(length&1)
        if name=='//':strings=data;continue
        if name in ('/','/SYM64/'):continue
        if name.startswith('#1/'):
            count=int(name[3:]);name=data[:count].rstrip(b'\0').decode('utf8');data=data[count:]
        elif name.startswith('/'):
            begin=int(name[1:]);end=strings.index(b'/\n',begin);name=strings[begin:end].decode('utf8')
        else:name=name.removesuffix('/')
        if not name.endswith('.o') or name in members:raise ValueError('unexpected/duplicate core member: '+name)
        members[name]=data if contents else sha(data)
    if offset!=len(blob):raise ValueError('archive padding mismatch')
    return members

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--core',type=pathlib.Path,required=True,help='Current libdh2_gameswf_core.a')
    p.add_argument('--ui-library',type=pathlib.Path,help='Optional same-stage current UI DSO, binding only')
    p.add_argument('--stock-core',type=pathlib.Path,default=ROOT/'.local-inputs/font-text-discovery/font-overlay-v1/libgameswf-core-stock.a')
    p.add_argument('--retained-stock-font-object',type=pathlib.Path,help='Same central build retained stock font object; reconstructs paired stock archive using unchanged current members. Requires exact historical vendor source comparison and matching compiler producer.')
    p.add_argument('--freeze',type=pathlib.Path,default=ROOT/'port/engine-ui/reference/gameswf-font-overlay-v1/freeze-manifest.json')
    p.add_argument('--historical-capture',type=pathlib.Path,default=ROOT/'.local-inputs/native-original-hud-packaged-build-capture.zip')
    p.add_argument('--historical-host-report',type=pathlib.Path,default=ROOT/'port/engine-ui/reports/gameswf-font-overlay-v1-host-audit.json')
    p.add_argument('--output',type=pathlib.Path,required=True)
    args=p.parse_args()
    current_facade=[ROOT/'port/engine-ui'/n for n in ('swf_movie.hpp','swf_movie.cpp')]
    tracked=[args.core,args.freeze,args.historical_capture,args.historical_host_report,*current_facade]
    tracked.append(args.retained_stock_font_object or args.stock_core)
    if args.ui_library:tracked.append(args.ui_library)
    before={str(x):file_sha(x) for x in tracked}
    freeze=json.loads(args.freeze.read_text());assert freeze['validation']=='PASS'
    for name,h in freeze['source_sha256'].items():assert file_sha(ROOT/name)==h,('frozen overlay source changed',name)
    for name,h in freeze['proof_sha256'].items():assert file_sha(ROOT/name)==h,('historical proof changed',name)
    current=archive_members(args.core);paired=None;vendor={}
    if args.retained_stock_font_object:
        raw=archive_members(args.core,True)
        original_font=args.retained_stock_font_object.read_bytes()
        producer=lambda data:re.findall(rb'GNU C\+\+[^\0]*',data)
        a=producer(original_font);b=producer(raw['gameswf_font.cpp.o'])
        assert a and a==b,'retained stock/current font compiler producers differ or missing'
        with zipfile.ZipFile(args.historical_capture) as z:
            source_list='port/engine-ui/gameswf_sources.cmake'
            old_list=z.read('source/'+source_list)
            assert (ROOT/source_list).read_bytes()==old_list,'unknown core source-list change'
            cpp_paths=re.findall(r'"\$\{DH2_GAMESWF_ROOT\}/([^"\n]+\.cpp)"',old_list.decode())
            assert len(cpp_paths)==95 and {pathlib.Path(n).name+'.o' for n in cpp_paths}==set(current),'source list does not cover all95 archived TUs'
            prefix='source/port/engine-ui/vendor/gameswf1714/'
            entries=[n for n in z.namelist() if n.startswith(prefix) and not n.endswith('/')]
            assert entries,'historical capture lacks original core sources'
            for n in entries:
                path=n.removeprefix('source/');expected=sha(z.read(n))
                assert file_sha(ROOT/path)==expected,('unknown core source change',path)
                vendor[path]=expected
            assert all('port/engine-ui/vendor/gameswf1714/'+n in vendor for n in cpp_paths),'historical compiled source missing'
        # This is deliberately a paired central baseline, not a claim that all
        # central object bytes equal the private optimized historical archive.
        stock=dict(current);stock['gameswf_font.cpp.o']=sha(original_font)
        baseline_raw=dict(raw);baseline_raw['gameswf_font.cpp.o']=original_font
        baseline=paired_archive(baseline_raw)
        baseline_path=ROOT/'.local-inputs/connected-hud-stage'/('paired-stock-'+sha(baseline)[:16]+'.a')
        baseline_path.parent.mkdir(parents=True,exist_ok=True)
        if baseline_path.exists():assert baseline_path.read_bytes()==baseline,'paired stock archive changed'
        else:baseline_path.write_bytes(baseline)
        assert archive_members(baseline_path)==stock,'paired stock archive verification failed'
        paired={'retained_stock_font_object':str(args.retained_stock_font_object),'retained_stock_font_sha256':sha(original_font),
          'compiler_producer':a[0].decode('ascii'),'historical_vendor_files_compared':len(vendor),
          'historical_vendor_source_sha256':vendor,'source_list_sha256':sha(old_list),'source_list_TUs':len(cpp_paths),'uses_current_unchanged_94_members':True,
          'cross_build_94_object_byte_identity_claim':False,'paired_stock_archive_path':str(baseline_path),
          'paired_stock_archive_sha256':sha(baseline),'paired_archive_link_execution':False}
    else:stock=archive_members(args.stock_core)
    changed=check_members(stock,current)
    with zipfile.ZipFile(args.historical_capture) as z:
        historical={f'port/engine-ui/{n}':sha(z.read(f'source/port/engine-ui/{n}')) for n in ('swf_movie.hpp','swf_movie.cpp')}
    old_report=json.loads(args.historical_host_report.read_text())
    assert old_report['validation']=='PASS' and old_report['captured_dependency_sources']['capture_sha256']==file_sha(args.historical_capture)
    assert historical==old_report['captured_dependency_sources']['captured_source_sha256']
    now={f'port/engine-ui/{x.name}':file_sha(x) for x in current_facade}
    after={str(x):file_sha(x) for x in tracked};assert after==before,'input bytes changed during read-only stage audit'
    assert all(file_sha(ROOT/n)==h for n,h in vendor.items()),'vendor source changed during read-only audit'
    report={'validation':'PASS','scope':'Read-only current-source/core receipt. Exactly1/95 archived TUs differs from privately frozen stock baseline; current UI DSO binding optional. No current connected-HUD execution, compiler-input completeness, instruction/package/device proof or historical façade report rebinding.',
      'script_sha256':file_sha(pathlib.Path(__file__)),'overlay_freeze_sha256':file_sha(args.freeze),
      'source_sha256':freeze['source_sha256'],'stock_core_sha256':sha(baseline) if paired else file_sha(args.stock_core),'current_core_sha256':file_sha(args.core),
      'members':95,'changed_members':changed,'unchanged_members':94,'archive_member_order_equal':list(stock)==list(current),'font_member_sha256':{'stock':stock['gameswf_font.cpp.o'],'current':current['gameswf_font.cpp.o']},
      'historical_capture_sha256':file_sha(args.historical_capture),'historical_host_report_sha256':file_sha(args.historical_host_report),
      'historical_facade_source_sha256':historical,'current_facade_source_sha256':now,
      'facade_bytes_equal_to_historical':{n:now[n]==historical[n] for n in now},'historical_proof_rebound':False}
    if args.ui_library:report['current_ui_library_sha256']=file_sha(args.ui_library)
    if paired:
        report['paired_central_baseline']=paired
        report['scope']='Read-only paired central core receipt: retained same-compiler stock font object versus overlay object, all historical vendor files unchanged, current other94 members retained. Historical private archive/proof is not rebound. No connected HUD execution or package/device proof.'
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'validation':'PASS','members':95,'changed_members':changed,'historical_proof_rebound':False}))
if __name__=='__main__':main()
