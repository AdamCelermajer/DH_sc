"""Read private campaign metadata on API37/16KiB; no gameplay profile claim."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import struct
import subprocess
import tempfile
import time
from emulator_smoke import inspect, launch_fresh

PACKAGE='com.example.dh2'
FILE='files/dh2_000.savegame'
META=re.compile(r'Native campaign metadata \| slot (\d+) \| class (-?\d+) \| level (-?\d+) \| difficulty (-?\d+) \| level ID (\d+) \| sections (\d+) \| reads (\d+) \| file opens (\d+) \| Save (\d+) \| profile (\d+)')
OWNERS=re.compile(r'Native campaign Save owners \| metadata (\d+) \| gameplay (\d+) \| metadata slot (-?\d+) \| gameplay slot (-?\d+) \| gameplay Character (\d+)')
LOCAL=re.compile(r'Native Player locality \| route (\d+) \| registered (\d+) \| local (-?\d+) \| selected fallback (\d+) \| Character660 (\d+) \| member1a0 (-?\d+) \| matching member (-?\d+) \| matching server (-?\d+) \| active (\d+) \| null result (-?\d+) \| null calls (\d+)')
BAD=re.compile(r'FATAL EXCEPTION|Fatal signal|Native frame failed|Model draw GL error')

def main():
    p=argparse.ArgumentParser(description=__doc__)
    for name in ('adb','serial'):p.add_argument('--'+name,required=True)
    for name in ('apk','campaign','output'):p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();out=a.output.resolve();out.mkdir(parents=True,exist_ok=True)
    report={'validation':'FAIL','apk_sha256':hashlib.sha256(a.apk.read_bytes()).hexdigest(),'campaign_sha256':hashlib.sha256(a.campaign.read_bytes()).hexdigest(),'serial':a.serial,'scope':'Native read-only campaign import transport uses original filename/index/SG_Load mask1 and seven metadata readers. Distinct metadata/gameplay Saves and exact locality fallback are tested. Actual PlayerManager registration, Character association/InitAll, mask4/GEAR restoration, writes and backup recovery remain unbound.'}
    transcript=[];last='';pid='';since='';previous=None;existed=False;backup_checked=False;rotation=[]
    def adb(*args,missing=False):
        r=subprocess.run([a.adb,'-s',a.serial,*args],capture_output=True,text=True,timeout=45)
        transcript.append({'args':list(args),'returncode':r.returncode,'stdout':r.stdout if args[0]!='logcat' else '[stored separately]','stderr':r.stderr})
        if r.returncode and not missing:raise RuntimeError(repr(args)+': '+r.stdout+r.stderr)
        return r.stdout.strip()
    def upload(raw):
        # adb push preserves arbitrary binary bytes; shell stdin may truncate.
        remote='/data/local/tmp/dh2-metadata-smoke-'+str(os.getpid())+'.bin'
        with tempfile.NamedTemporaryFile(prefix='dh2-metadata-',suffix='.bin',delete=False) as temp:
            temp.write(raw);local=Path(temp.name)
        try:
            adb('push',str(local),remote)
            adb('shell','run-as',PACKAGE,'cp',remote,FILE)
            installed=subprocess.check_output([a.adb,'-s',a.serial,'exec-out','run-as',PACKAGE,'cat',FILE],timeout=45)
            transcript.append({'private_copy_bytes':len(installed),'private_copy_sha256':hashlib.sha256(installed).hexdigest(),'matches_input':installed==raw})
            assert installed==raw,'private campaign copy differs'
        finally:
            local.unlink(missing_ok=True)
            adb('shell','rm','-f',remote)
    def logs():
        nonlocal last
        assert adb('shell','pidof',PACKAGE)==pid,'process changed'
        last=adb('logcat','-d','-T',since,'--pid='+pid,'-v','brief')
        assert not BAD.search(last),'native failure'
        return last
    def wait(predicate,label,seconds=35):
        end=time.monotonic()+seconds
        while time.monotonic()<end:
            s=logs()
            if predicate(s):return s
            time.sleep(.25)
        raise AssertionError(label+' not observed')
    def start():
        nonlocal pid,since
        since=adb('shell','date','+%s.%N')
        launch_fresh(adb,'--es','world','crypt01.dwld','--ei','profile_slot','0','--ei','time_ms','0','--ez','enemy_ai','false')
        pid=adb('shell','pidof',PACKAGE)
        report['process_id']=pid;report['log_since']=since
    try:
        report['api']=int(adb('shell','getprop','ro.build.version.sdk'));report['page_size']=int(adb('shell','getconf','PAGE_SIZE'))
        report['abi']=adb('shell','getprop','ro.product.cpu.abi')
        assert (report['api'],report['page_size'],report['abi'])==(37,16384,'x86_64')
        report['libraries']=inspect(a.apk)
        assert 'Success' in adb('install','-r',str(a.apk.resolve()))
        backup=subprocess.run([a.adb,'-s',a.serial,'exec-out','run-as',PACKAGE,'cat',FILE],capture_output=True,timeout=45)
        existed=backup.returncode==0
        if not existed:
            assert b'No such file or directory' in backup.stderr+backup.stdout, 'Cannot determine previous private campaign state'
        backup_checked=True
        if existed:previous=backup.stdout
        rotation=adb('shell','cmd','window','user-rotation').split()
        adb('shell','cmd','window','user-rotation','lock','0')
        # A corruption marker must not silently turn into an empty character.
        upload(struct.pack('<I',0xffffffff));start()
        text=wait(lambda s:'World load failed: Campaign metadata import failed:' in s,'corrupt primary rejection')
        assert not META.search(text) and 'Native Player AIS initialized |' not in text
        (out/'corrupt-primary.log').write_text(text,encoding='utf-8')
        report['corrupt_primary_rejected_without_empty_profile']=True
        raw=a.campaign.read_bytes();upload(raw);start()
        text=wait(lambda s:bool(OWNERS.search(s)) and 'Native Player skill update complete | attempt 1 | callbacks 13' in s,'real campaign metadata')
        first=tuple(map(int,META.findall(text)[-1]))
        assert first[:4]==(0,263,1,0) and first[5:8]==(15,7,1),first
        assert first[8] and first[9]
        adb('shell','am','start','-W','-f','0x20000000','-n',PACKAGE+'/.MainActivity','--ei','profile_slot','1')
        text=wait(lambda s:'Campaign metadata slot change rejected | requested 1 | active 0 | relaunch required' in s,'active profile slot change rejection')
        assert all(tuple(map(int,row))[:6]==first[:6] and tuple(map(int,row))[7:]==first[7:] for row in META.findall(text)),'rejected selection imported another profile'
        baseline=len(META.findall(text))
        adb('shell','am','broadcast','-a',PACKAGE+'.DEBUG_RELOAD_WORLD','-p',PACKAGE)
        text=wait(lambda s:len(META.findall(s))>baseline and len(OWNERS.findall(s))>baseline,'same-process metadata reload')
        baseline=len(META.findall(text))
        adb('shell','cmd','window','user-rotation','lock','1')
        text=wait(lambda s:len(META.findall(s))>baseline and len(OWNERS.findall(s))>baseline,'metadata Activity recreation')
        rows=[tuple(map(int,row)) for row in META.findall(text)]
        assert len(rows)>=3
        for i,row in enumerate(rows):
            assert row[:6]==first[:6] and row[6]==7*(i+1) and row[7:]==first[7:],row
        owners=[tuple(map(int,row)) for row in OWNERS.findall(text)]
        assert len(owners)==len(rows) and all(row==owners[0] for row in owners)
        assert owners[0][0]==first[8] and owners[0][0]!=owners[0][1] and owners[0][2:4]==(0,-1) and owners[0][4]
        local=[tuple(map(int,row)) for row in LOCAL.findall(text)]
        assert len(local)==len(rows) and all(row==(2,0,1,1,0,-1,-1,-2,0,0,0) for row in local),local
        host=[tuple(map(int,row)) for row in re.findall(r'Native managed host \| Level (-?\d+) \| difficulty (-?\d+) \| property reads (\d+) \| member writes (\d+) \| skipped unbound (\d+)',text)]
        assert len(host)==len(rows) and all(row==(-1,0,0,0,1) for row in host),host
        assert text.count('Native Player AIS initialized |')==1 and text.count('Native Player skill preparation |')==1
        assert 'Campaign metadata import failed' not in text
        image=subprocess.check_output([a.adb,'-s',a.serial,'exec-out','screencap','-p'],timeout=30)
        assert image.startswith(b'\x89PNG');(out/'metadata-retained.png').write_bytes(image)
        report.update(validation='PASS',actual_private_campaign_bytes=len(raw),metadata=rows,distinct_Save_owners=owners,source_locality_fallback=local,unregistered_host_reconcile_skipped=host,active_slot_change_rejected=True,same_process_reload_and_rotation=True,same_metadata_profile_and_gameplay_Save=True,private_file_contents_published=False)
        print(json.dumps({'validation':'PASS','metadata_reads':rows[-1][6],'file_opens':rows[-1][7],'native_profile_gameplay_complete':False}))
    except Exception as e:
        report['error']=repr(e);raise
    finally:
        try:
            if backup_checked:
                if existed and previous is not None:upload(previous)
                else:adb('shell','run-as',PACKAGE,'rm','-f',FILE)
            if rotation and rotation[0]=='free':adb('shell','cmd','window','user-rotation','free')
            elif len(rotation)>1:adb('shell','cmd','window','user-rotation','lock',rotation[1])
            report['private_campaign_restored']=backup_checked
        except Exception as cleanup_error:
            report['cleanup_error']=repr(cleanup_error)
            report['validation']='FAIL'
            if 'error' not in report:raise
        finally:
            (out/'runtime.log').write_text(last,encoding='utf-8');(out/'commands.json').write_text(json.dumps(transcript,indent=2)+'\n',encoding='utf-8');(out/'validation.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')

if __name__=='__main__':main()
