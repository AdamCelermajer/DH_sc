"""API37/16KiB source AI death composition; raw fatal-hit fixture, not full Kill."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time
import xml.etree.ElementTree as ET
import zipfile
from emulator_smoke import inspect, launch_fresh

PACKAGE='com.example.dh2'
BAD=re.compile(r'FATAL EXCEPTION|Fatal signal|Native frame failed|World load failed|Model draw GL error|Native Player AIS failure retained|Native Player AI timer failed|Native Player death fixture failed')
SNAP=re.compile(r'Native Player dead snapshot \| phase (\w+) \| state (\d+) \| HP (-?\d+) \| dead (\d+) \| buffs (\d+) \| groups (\d+) \| update attempts (\d+) \| VM (\S+) \| AIS (\S+) \| source timer33 (\d+) \| source timer34 (\d+)')
TIMERS=re.compile(r'Native Player dead timer \| phase (\w+) \| slot (\d+) \| event ([0-9a-f]+) \| active (\d+) \| paused (\d+) \| elapsed (\d+) \| ref (\d+)')
ANIMATION=re.compile(r'Prince retained animation inspection \| state (-?\d+) \| frozen (\d+) \| scene (\d+) \| clip (-?\d+) \| engine (-?\d+) \| ms (-?\d+) \| playback ([0-9a-f]+) \| pose ([0-9a-f]+)')

def main():
    p=argparse.ArgumentParser(description=__doc__)
    for name in ('adb','serial'):p.add_argument('--'+name,required=True)
    for name in ('apk','output'):p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();out=a.output.resolve();out.mkdir(parents=True,exist_ok=True)
    report={'validation':'FAIL','apk_sha256':hashlib.sha256(a.apk.read_bytes()).hexdigest(),
            'scope':'Live native CharAI::OnDied -> AI_SetDead after existing raw F_ApplyResult health/Kill prefix. Full Character::Kill trophies/online/event2 forwarding, rewards, AI frame and campaign combat remain unbound.'}
    transcript=[];last='';pid='';since='';prior=[]
    def adb(*args):
        r=subprocess.run([a.adb,'-s',a.serial,*args],capture_output=True,text=True,timeout=45)
        transcript.append({'args':list(args),'returncode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
        if r.returncode:raise RuntimeError(repr(args)+': '+r.stdout+r.stderr)
        return r.stdout.strip()
    def logs():
        nonlocal last
        assert adb('shell','pidof',PACKAGE)==pid,'process lost or changed'
        last=adb('logcat','-d','-T',since,'--pid='+pid,'-v','brief')
        assert not BAD.search(last),'native failure in logs'
        return last
    def wait(predicate,label,seconds=35):
        deadline=time.monotonic()+seconds
        while time.monotonic()<deadline:
            text=logs()
            if predicate(text):return text
            time.sleep(.25)
        raise AssertionError(label+' not observed')
    def command(action,*extras):return adb('shell','am','broadcast','-a',PACKAGE+'.DEBUG_'+action,'-p',PACKAGE,*extras)
    def freeze(label):
        prior=logs().count('Animation time command applied | time 0')
        command('ANIMATION_TIME','--ei','time_ms','0')
        text=wait(lambda t:t.count('Animation time command applied | time 0')>prior,label)
        row=ANIMATION.findall(text)[-1]
        assert row[1]=='1',row
        return text,row
    def select_model_preview():
        def nodes():
            adb('shell','uiautomator','dump','/sdcard/dh2-player-preview-window.xml')
            return list(ET.fromstring(adb('shell','cat','/sdcard/dh2-player-preview-window.xml')).iter('node'))
        def tap(node):
            left,top,right,bottom=map(int,re.findall(r'-?\d+',node.get('bounds')))
            assert right>left and bottom>top
            adb('shell','input','tap',str((left+right)//2),str((top+bottom)//2))
        spinner=next(node for node in nodes() if node.get('class')=='android.widget.Spinner')
        tap(spinner)
        # The continuously rendered popup can prevent uiautomator idle. Navigate
        # the bundled picker order instead, then require the actual selected model.
        time.sleep(.5)
        with zipfile.ZipFile(a.apk) as archive:
            worlds=sorted(n.removeprefix('assets/') for n in archive.namelist() if n.startswith('assets/worlds/') and n.endswith('.dwld'))
            models=sorted(n.removeprefix('assets/') for n in archive.namelist() if n.startswith('assets/models/') and n.count('/')==2)
        choices=['ui/original-main-menu']+worlds+models
        steps=choices.index('models/prince_modular.bdae')-choices.index('worlds/crypt01.dwld')
        assert steps>0
        adb('shell','input','keyevent',*(['20']*steps),'23')
    def image(name):
        raw=subprocess.check_output([a.adb,'-s',a.serial,'exec-out','screencap','-p'],timeout=30)
        assert raw.startswith(b'\x89PNG');(out/(name+'.png')).write_bytes(raw)
        report.setdefault('screenshots',[]).append({'file':name+'.png','sha256':hashlib.sha256(raw).hexdigest()})
    try:
        report['api']=int(adb('shell','getprop','ro.build.version.sdk'))
        report['page_size']=int(adb('shell','getconf','PAGE_SIZE'))
        report['abi']=adb('shell','getprop','ro.product.cpu.abi')
        assert (report['api'],report['page_size'],report['abi'])==(37,16384,'x86_64')
        report['libraries']=inspect(a.apk)
        assert 'Success' in adb('install','-r',str(a.apk.resolve()))
        prior=adb('shell','cmd','window','user-rotation').split()
        adb('shell','cmd','window','user-rotation','lock','0')
        since=adb('shell','date','+%s.%N')
        launch_fresh(adb,'--es','world','crypt01.dwld','--ez','enemy_ai','false')
        pid=adb('shell','pidof',PACKAGE)
        text=wait(lambda t:'Native Player skill update complete | attempt 1 | callbacks 13' in t,'source InitProcess')
        initial=re.search(r'Native Player AIS initialized .*? \| VM (\S+)',text)
        assert initial;vm=initial[1]
        assert 'Native Player buff snapshot | phase initial | count 1 | groups 1 | id 49' in text
        freeze('frozen source clocks')
        image('alive-before-fixture')
        command('PLAYER_DEATH')
        text=wait(lambda t:'Native Player source death complete |' in t,'source death/cleanup')
        assert 'payload null | target 0 | last target 0 | timer stops 2 | timer33 4294967295 | timer34 4294967295' in text
        assert 'outgoing 1 | incoming 1 | skill 1 | faery 1 | buffs 0 | groups 0' in text
        cleanup=re.findall(r'Native Player source cleanup \| list (\d+) \| examined (\d+) \| callbacks (\d+) \| completed (\d+) \| lua errors (\d+)',text)
        assert [tuple(map(int,x)) for x in cleanup]==[(1,16,8,8,0),(2,5,5,5,0)],cleanup
        report['cleanup_receipts']=cleanup
        before_counts={event:len(re.findall(r'Native Player AI timer delivered \| event '+event+r' \|',text)) for event in ('33','34')}
        command('ANIMATION_TIME','--ei','time_ms','-1')
        wait(lambda t:'Character physical object removed | state 12 | source event 22' in t,'authored death animation/body removal',45)
        text,final_pose=freeze('frozen final death pose')
        assert final_pose[:2]==('12','1'),final_pose
        image('dead-after-authored-animation')
        # Reach normal frames for more than both retired timer durations.
        command('ANIMATION_TIME','--ei','time_ms','-1')
        deadline=time.monotonic()+3.5
        while time.monotonic()<deadline:logs();time.sleep(.3)
        text,before_reload=freeze('frozen pose before reload')
        event_count=text.count('Blended character event |')
        command('RELOAD_WORLD')
        text=wait(lambda t:'World reload command applied |' in t and 'Native Player dead snapshot | phase restore |' in t,'same dead owners on reload')
        text,after_reload=freeze('retained final pose after reload')
        assert before_reload==after_reload,('reload changed CPU playback or rendered pose',before_reload,after_reload)
        assert text.count('Blended character event |')==event_count,'reload replayed source animation events'
        assert 'Player blended sequence restarted |' not in text
        image('dead-after-reload')
        restore_count=text.count('Native Player dead snapshot | phase restore |')
        adb('shell','cmd','window','user-rotation','lock','1')
        text=wait(lambda t:t.count('Native Player dead snapshot | phase restore |')>restore_count,'dead Player Activity/GL recreation')
        text,after_recreation=freeze('retained pose after Activity recreation')
        assert before_reload==after_recreation,('Activity recreation changed playback or pose',before_reload,after_recreation)
        assert text.count('Blended character event |')==event_count,'Activity recreation replayed source animation events'
        image('dead-after-activity-recreation')
        snapshots=[]
        for row in SNAP.findall(text):
            phase,*data=row
            state,hp,dead,buffs,groups,attempts=map(int,data[:6]);observed_vm,ais=data[6:8]
            timer33,timer34=map(int,data[8:])
            assert (state,hp,dead,buffs,groups,attempts,timer33,timer34)==(12,0,1,0,0,1,0xffffffff,0xffffffff),row
            assert observed_vm==vm
            snapshots.append({'phase':phase,'VM':observed_vm,'AIS':ais,'HP':hp,'buffs':buffs,'update_attempts':attempts})
        assert [r['phase'] for r in snapshots]==['dead','restore','restore'],snapshots
        assert all(row['AIS']==snapshots[0]['AIS'] for row in snapshots)
        retired=[(phase,int(slot),int(event,16),int(active),int(ref)) for phase,slot,event,active,paused,elapsed,ref in TIMERS.findall(text) if int(event,16) in (0x33,0x34)]
        assert len(retired)==6 and all(row[3:]==(0,0) for row in retired),retired
        assert [r[2] for r in retired]==[0x33,0x34]*3
        assert all(len(re.findall(r'Native Player AI timer delivered \| event '+event+r' \|',text))==count for event,count in before_counts.items()),'retired source timer delivered again'
        assert text.count('Native Player AIS initialized |')==1 and text.count('Native Player source death complete |')==1
        assert text.count('Native Player source cleanup |')==2,'cleanup replayed on reload'
        assert text.count('Native Player buff snapshot |')==1,'faery buff recreated'
        # A standalone model preview is a terminal development-world boundary,
        # including after reset_context has moved actors into saved copies.
        select_model_preview()
        text=wait(lambda t:'Native Player preview owners retired | bank 0 | retained pose 0 | skill owner 0 | AI owner 0' in t,'terminal model-preview retirement')
        preview=re.findall(r'(models/\S+\.bdae): 3D upload OK',text)
        assert preview,'Terminal boundary did not load a standalone model'
        report['terminal_preview_asset']=preview[-1]
        ghost_retired=[(int(event,16),int(slot),int(status)) for event,slot,status in re.findall(r'Native Ghost timer retired before VM close \| event ([0-9a-f]+) \| slot (\d+) \| status (-?\d+)',text)]
        assert ghost_retired and all(status>=0 for _,_,status in ghost_retired),ghost_retired
        assert sum(event==0x33 for event,_,_ in ghost_retired)==sum(event==0x34 for event,_,_ in ghost_retired)
        assert re.search(r'Native terminal world discard \| Ghost references \d+ \| remaining 0 \| groups 0 \| saved actors 0 \| catalogue 0',text)
        image('terminal-model-preview')
        report.update(validation='PASS',snapshots=snapshots,retired_ai_timers=retired,source_update_call_count=1,retained_VM=vm,no_post_death_regeneration=True,no_buff_recreation=True,same_process_reload=True,
                      retained_animation={'before_reload':before_reload,'after_reload':after_reload,'after_activity_recreation':after_recreation,'source_event_count':event_count,'exact_pose_and_playback_match':True},
                      terminal_model_preview_retires_world_CPU_owners=True,ghost_timers_cancelled_before_VM_close=ghost_retired)
        print(json.dumps({'validation':'PASS','scope':report['scope'],'apk_sha256':report['apk_sha256']}))
    except Exception as e:
        report['error']=repr(e);raise
    finally:
        (out/'runtime.log').write_text(last,encoding='utf-8')
        (out/'commands.json').write_text(json.dumps(transcript,indent=2)+'\n',encoding='utf-8')
        (out/'validation.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
        if prior and prior[0]=='free':adb('shell','cmd','window','user-rotation','free')
        elif len(prior)>1:adb('shell','cmd','window','user-rotation','lock',prior[1])

if __name__=='__main__':main()
