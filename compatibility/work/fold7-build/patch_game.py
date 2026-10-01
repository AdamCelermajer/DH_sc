#!/usr/bin/env python3
"""Prepare the existing compatibility tree for in-process ARM translation."""
from pathlib import Path
import shutil,re,json,subprocess,sys

ROOT=Path(__file__).resolve().parent
source=ROOT.parent/'patched'
target=ROOT/'game-tree'
if target.exists(): shutil.rmtree(target)
shutil.copytree(source,target,ignore=shutil.ignore_patterns('build','dist'))
changes=[]
pattern=re.compile(r'^(\s*const-string(?:/jumbo)?\s+([vp]\d+),\s+"(?:/sdcard/Android/data/com\.gameloft\.android\.GAND\.GloftD2SS/files|/data/data/com\.gameloft\.android\.GAND\.GloftD2SS)[^"]*"\s*)$',re.M)
for p in (target/'smali').rglob('*.smali'):
    original=p.read_text()
    def change(m):
        r=m.group(2)
        return m.group(1)+'\n    invoke-static/range {'+r+' .. '+r+'}, Llocal/dh2/compat/GamePaths;->resolve(Ljava/lang/String;)Ljava/lang/String;\n    move-result-object '+r+'\n'
    new,count=pattern.subn(change,original)
    if count: p.write_text(new);changes.append({'file':str(p.relative_to(target)),'path_lookups':count})
p=target/'smali/local/dh2/compat/CompatApplication.smali'
s=p.read_text();needle='    invoke-super {p0}, Landroid/app/Application;->onCreate()V\n'
assert needle in s
s=s.replace(needle,needle+'    invoke-static {p0}, Llocal/dh2/compat/GamePaths;->initialize(Landroid/content/Context;)V\n')
p.write_text(s)
# getSDFolder used Java reference equality for the empty string. Preserve its
# fallback while testing content, so a persisted empty value works correctly.
p=target/'smali/com/gameloft/android/GAND/GloftD2SS/GLUtils/SUtils.smali'
s=p.read_text();start=s.index('.method public static getSDFolder()');end=s.index('.end method',start)
part=s[start:end];old='    if-eq v0, v1, :cond_0';assert old in part
part=part.replace(old,'    invoke-virtual {v1, v0}, Ljava/lang/String;->equals(Ljava/lang/Object;)Z\n    move-result v1\n    if-nez v1, :cond_0')
p.write_text(s[:start]+part+s[end:])
# The first Fold7 report reached the real game Activity, then MediaProvider
# rejected initMediaList's projection {"*"}. Replace only that query and keep
# the legacy playlist setup and nativeInitplayer call after it.
p=target/'smali/com/gameloft/android/GAND/GloftD2SS/Musicplayer.smali'
s=p.read_text();start=s.index('.method public static initMediaList()V');end=s.index('.end method',start)
part=s[start:end]
query_end=part.index('    move-result-object v0')+len('    move-result-object v0')
assert 'const-string v1, "*"' in part[:query_end]
replacement='''.method public static initMediaList()V
    .locals 2

    sget-object v0, Lcom/gameloft/android/GAND/GloftD2SS/DungeonHunter2;->o:Lcom/gameloft/android/GAND/GloftD2SS/DungeonHunter2;
    invoke-static {v0}, Llocal/dh2/compat/MediaQueries;->queryPlaylists(Landroid/content/Context;)Landroid/database/Cursor;
    move-result-object v0'''
part=replacement+part[query_end:]
assert '->nativeInitplayer()V' in part
p.write_text(s[:start]+part+s[end:])
subprocess.run([sys.executable,str(ROOT/'patch_storm.py'),'--output',str(target/'lib/armeabi-v7a/libStormGLOFT.so')],check=True)
(ROOT/'game-patch-report.json').write_text(json.dumps({'path_changes':changes,'total':sum(x['path_lookups'] for x in changes),'media_playlist_query_fixed':True,'native_engine_modified':False,'storm_patch_library_modified':True,'licensing_decisions_modified':False},indent=2)+'\n')
print('Patched',sum(x['path_lookups'] for x in changes),'Java path lookups and Storm private-linker ABI use; engine unchanged.')
