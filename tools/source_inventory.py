"""Inventory maintained source separately from recovered text and dependencies.

Uses the Git index so it also covers the consolidated pending source snapshot.
Counts are repository size metrics, never a percentage of the original game.
"""
import argparse, collections, hashlib, json, pathlib, subprocess

ROOT=pathlib.Path(__file__).resolve().parents[1]
EXT={'.cpp','.c','.hpp','.h','.java','.py','.lua','.luac'}
THIRD={'upstream','vendor','external','3rdparty','dependencies'}
ADAM={'engine-textures','scene-materials','engine-animation','engine-skinning','game-data','physics-backend','level-world','adam-script-runtime','android-native','engine-ui'}

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=pathlib.Path,default=ROOT/'reports/combined-source-inventory.json');a=p.parse_args()
    entries=subprocess.check_output(['git','ls-files','--stage','-z'],cwd=ROOT).decode().split('\0')
    paths={}
    for entry in entries:
        if not entry:continue
        metadata,name=entry.split('\t',1)
        _,object_id,stage=metadata.split()
        if stage!='0':raise RuntimeError('Resolve unmerged index entries before inventory')
        paths[name]=object_id
    # Read staged blobs so concurrent work on tracked files cannot silently
    # enter an inventory of a different, reviewable source checkpoint.
    selected=[]
    for name in paths:
        path=pathlib.PurePosixPath(name)
        if len(path.parts)<3 or path.parts[0]!='port' or path.suffix not in EXT:continue
        if any(v in THIRD for v in path.parts[2:]) or any('box2d' in v.lower() or v.startswith('lua-5.') for v in path.parts):continue
        if path.parts[1]=='adam-script-runtime' and path.parts[2]=='lua':continue
        if any(v in {'reference','fixtures','build','reports','assets'} for v in path.parts[2:]):continue
        selected.append(name)
    batch=subprocess.run(['git','cat-file','--batch'],cwd=ROOT,input=('\n'.join(paths[name] for name in selected)+'\n').encode(),capture_output=True,check=True).stdout
    blobs={};cursor=0
    for name in selected:
        end=batch.index(b'\n',cursor);object_id,kind,size=batch[cursor:end].split();size=int(size)
        assert object_id.decode()==paths[name] and kind==b'blob'
        cursor=end+1;blobs[name]=batch[cursor:cursor+size];cursor+=size
        assert batch[cursor:cursor+1]==b'\n';cursor+=1
    assert cursor==len(batch)
    groups=collections.defaultdict(lambda:{'source_files':0,'source_lines':0,'source_bytes':0,'test_files':0})
    files=[]
    for name in selected:
        path=pathlib.PurePosixPath(name)
        if len(path.parts)<3 or path.parts[0]!='port' or path.suffix not in EXT:continue
        group=path.parts[1]
        if any(v in THIRD for v in path.parts[2:]) or any('box2d' in v.lower() or v.startswith('lua-5.') for v in path.parts):continue
        if group=='adam-script-runtime' and path.parts[2]=='lua':continue
        if any(v in {'reference','fixtures','build','reports','assets'} for v in path.parts[2:]):continue
        raw=blobs[name];lines=len(raw.splitlines())
        tests=any(v in {'tests','tools','test','androidTest'} for v in path.parts[2:]) or path.name.startswith('test_')
        row=groups[group];row['test_files']+=int(tests)
        if not tests:row['source_files']+=1;row['source_lines']+=lines;row['source_bytes']+=len(raw)
        files.append({'path':name,'bytes':len(raw),'lines':lines,'kind':'test_or_tool' if tests else 'source','sha256':hashlib.sha256(raw).hexdigest()})
    result={'schema':'dh2-maintained-source-inventory/v2','scope':'Git-index paths and staged blob bytes; excludes unstaged drafts, recovered evidence, vendor/upstream code, reference corpora, fixtures and packaged assets (including unchanged recovered scripts). Includes repaired/decompiled Java in its own category. Files/lines are not function implementation or game-completion counts.','adam_pinned_commit':'45c5348e807607a2825211bb8f26248067ba9106','modules':[{'module':k,'origin':'Adam import with local adaptations' if k in ADAM else 'Existing reconstruction','source_category':'repaired_decompiled_Java' if k=='android-java' else 'reconstruction_and_port',**v} for k,v in sorted(groups.items())],'totals':{key:sum(v[key] for v in groups.values()) for key in ('source_files','source_lines','source_bytes','test_files')},'reconstruction_and_port_totals':{key:sum(v[key] for k,v in groups.items() if k!='android-java') for key in ('source_files','source_lines','source_bytes','test_files')},'files':files}
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({'totals':result['totals'],'modules':len(groups),'report':str(a.output)},indent=2))
if __name__=='__main__':main()
