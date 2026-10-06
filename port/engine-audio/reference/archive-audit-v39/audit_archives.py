"""Read-only exact-filename inventory of existing DH2 archives and assets."""
from pathlib import Path
import hashlib, json, subprocess, zipfile, zlib, tempfile

ROOT=Path(__file__).resolve().parents[4]
OUT=Path(__file__).resolve().parent
DOWNLOADS=Path(r'C:/Users/adamc/Downloads')
ledger=json.loads((ROOT/'port/engine-audio/reference/source-bindings-v38/ledger.json').read_text())
missing=sorted({a['filename'] for r in ledger for a in r.get('assets',[]) if not a['exists']})
priority=['sfx_chest_opening.wav','sfx_drop_armor.wav','sfx_drop_gold.wav','sfx_drop_potion.wav','sfx_drop_weapon.wav','sfx_pickup_armor.wav','sfx_pickup_gold.wav','sfx_pickup_potion.wav','sfx_pickup_weapon.wav']
targets=set(missing)
def sha(path):
    digest=hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda:stream.read(1024*1024),b''):digest.update(block)
    return digest.hexdigest()
def inventory(roots):
    args=['rg','--files','--hidden','--no-ignore',*map(str,roots)]
    for glob in ('*.zip','*.apk','*.obb','*.7z','*.rar','*.wav','*.vxn'):args+=['-g',glob]
    result=subprocess.run(args,cwd=ROOT,capture_output=True,text=True,timeout=30)
    assert result.returncode in (0,1,2),result.stderr
    return [Path(n) if Path(n).is_absolute() else ROOT/n for n in result.stdout.splitlines()]

downloads=inventory([DOWNLOADS])
# Only provided DH2-named files. Do not inspect other user archive contents.
downloads=[p for p in downloads if any(x in str(p).lower() for x in ('dungeonhunter2','dungeon-hunter-2','gloftd2'))]
workspace=inventory([ROOT/'.local-inputs',ROOT/'port',ROOT/'session-contributions'] if (ROOT/'session-contributions').exists() else [ROOT/'.local-inputs',ROOT/'port'])
excluded=('zip-asset-pack-v1','irrlicht-lineage','ui-swf-discovery','physics-backend-discovery')
files=sorted(set(downloads+[p for p in workspace if not any(x in str(p).lower() for x in excluded)]))
archives=[p for p in files if p.suffix.lower() in ('.zip','.apk','.obb','.7z','.rar')]
loose=[p for p in files if p.name in targets]
matches=[];records=[];nested=[];errors=[]
for archive in archives:
    row=dict(path=str(archive),bytes=archive.stat().st_size,version_hint=archive.name)
    if archive.suffix.lower() in ('.7z','.rar'):
        row['status']='not_zip_format_not_inspected';records.append(row);continue
    try:
        with zipfile.ZipFile(archive) as z:
            infos=z.infolist()
            listing=[dict(path=i.filename,bytes=i.file_size,compressed_bytes=i.compress_size,crc32=f'{i.CRC:08x}',compression=i.compress_type) for i in infos]
            row['entries']=len(infos)
            row['central_listing_sha256']=hashlib.sha256(json.dumps(listing,sort_keys=True,separators=(',',':')).encode()).hexdigest()
            row['sound_members']=sum(Path(i.filename).suffix.lower() in ('.wav','.vxn') for i in infos)
            row['exact_matches']=[]
            # Original user archives receive a whole-file SHA. Derived native
            # packages receive an exact central inventory digest; hashing all
            # repeated hundreds-MB APK payloads would be unrelated I/O work.
            if archive in downloads or archive.stat().st_size<16*1024*1024:row['archive_sha256']=sha(archive)
            for info in infos:
                name=Path(info.filename).name
                if name in targets:
                    assert info.file_size<64*1024*1024,info.filename
                    data=z.read(info)
                    match=dict(archive=str(archive),member=info.filename,filename=name,bytes=len(data),sha256=hashlib.sha256(data).hexdigest(),archive_sha256=row.get('archive_sha256'),central_listing_sha256=row['central_listing_sha256'])
                    matches.append(match);row['exact_matches'].append(match)
                if name.lower().endswith(('.zip','.obb')) and any(x in name.lower() for x in ('sounds','original-cache','dungeon','gloftd2')):
                    nested.append(dict(archive=str(archive),member=info.filename,bytes=info.file_size,crc32=f'{info.CRC:08x}',scope='Plausible nested asset container; derived package copy is inventoried, not assumed a new original version.'))
            row['status']='central_inventory_complete'
    except (zipfile.BadZipFile,OSError,RuntimeError) as error:
        row['status']='error';row['error']=str(error);errors.append(row)
    records.append(row)
for path in loose:matches.append(dict(loose=str(path),filename=path.name,bytes=path.stat().st_size,sha256=sha(path)))

# Inspect standalone generated cache ZIP containers found by rg as well as both
# user cache versions. A nested repack is not a substitute for an original
# sample. Record matching size/CRC with an inspected loose container without
# asserting byte identity from CRC alone.
known_containers=[]
for row in records:
    if row.get('status')=='central_inventory_complete' and any(x in Path(row['path']).name.lower() for x in ('original-cache','dungeon-hunter-2')):
        crc=0
        with Path(row['path']).open('rb') as stream:
            for block in iter(lambda:stream.read(1024*1024),b''):crc=zlib.crc32(block,crc)
        known_containers.append(dict(path=row['path'],bytes=row['bytes'],crc32=f'{crc&0xffffffff:08x}',archive_sha256=row.get('archive_sha256'),central_listing_sha256=row['central_listing_sha256']))
for n in nested:
    n['matching_inspected_standalone_metadata']=[r for r in known_containers if r['bytes']==n['bytes'] and r['crc32']==n['crc32']]
    assert n['bytes'] <= 512*1024*1024,n
    digest=hashlib.sha256();actual_size=0
    with zipfile.ZipFile(n['archive']) as outer:
        with outer.open(n['member']) as payload:
            for block in iter(lambda:payload.read(1024*1024),b''):
                actual_size+=len(block);assert actual_size<=512*1024*1024;digest.update(block)
        assert actual_size==n['bytes']
        n['payload_sha256']=digest.hexdigest()
        n['identical_inspected_archives']=[r['path'] for r in known_containers if r.get('archive_sha256')==n['payload_sha256']]
        if n['identical_inspected_archives']:
            n['status']='cryptographically_identical_to_inspected_original_cache'
        else:
            # Only an actually different plausible nested asset container is
            # staged for central-directory reading, with bounded disk/memory.
            with tempfile.TemporaryFile(dir=OUT) as staging:
                with outer.open(n['member']) as payload:
                    for block in iter(lambda:payload.read(1024*1024),b''):staging.write(block)
                staging.seek(0)
                with zipfile.ZipFile(staging) as inner:
                    n['status']='distinct_nested_central_inventory_complete'
                    n['entries']=len(inner.infolist())
                    for info in inner.infolist():
                        name=Path(info.filename).name
                        if name in targets:
                            assert info.file_size<64*1024*1024
                            data=inner.read(info)
                            matches.append(dict(archive=n['archive'],container=n['member'],member=info.filename,filename=name,bytes=len(data),sha256=hashlib.sha256(data).hexdigest(),container_sha256=n['payload_sha256']))

report=dict(validation='inventory_complete' if not errors else 'partial',scope='Existing provided DH2 archives and workspace DH2 derivative archives/loose assets only. Exact basename match; no suffix/typo substitution, no asset binding/cache edits/APK packaging/network/device.',targets=len(targets),priorities=priority,missing_filenames=missing,archives=records,nested_containers=nested,loose_matches=[str(p) for p in loose],matches=matches,errors=errors,remaining_exact_missing=[name for name in missing if not any(m['filename']==name for m in matches)],nested_boundary='Each plausible nested asset container was streamed in1MiB chunks and SHA256 compared with inspected original cache archives. Cryptographically identical payloads use the same already inspected central directory; distinct payloads receive a bounded staged central-directory inspection.')
(OUT/'report.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(dict(archives=len(records),targets=len(targets),exact_matches=len(matches),nested_containers=len(nested),errors=len(errors),original_archives=[dict(path=r['path'],bytes=r['bytes'],sha256=r.get('archive_sha256'),sound_members=r.get('sound_members')) for r in records if Path(r['path']) in downloads])))
