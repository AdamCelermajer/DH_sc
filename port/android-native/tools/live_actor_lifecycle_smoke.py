"""Rotate/pause an already running Crypt after live_actor_smoke completes.

Does not install, build, fresh-launch or reset spawn. Starts from authored Idle.
Checks GL-context counters and development Activity lifecycle behavior, without
claiming original Application/Character FSM or physical ARM64 phone parity.
"""
import argparse
import json
import math
import re
import subprocess
import time
import xml.etree.ElementTree as ET
from pathlib import Path
from PIL import Image,ImageChops
from live_actor_smoke import PACKAGE,READY,WORLD,POSITION,BAD,frames,position,body_matches,digest,require
from prince_bank_smoke import radius_facts

def contexts(text):
    """Counter resets are legitimate only at a new Native actor ready marker."""
    starts=list(READY.finditer(text));out=[]
    for i,start in enumerate(starts):
        ready=start;require((int(ready[1]),int(ready[2]))==(83,82),'Lifecycle body ownership count differs')
        if i==0:require(abs(float(ready[3])-113.699707)<.00002,'Initial lifecycle spawn radius differs')
        radius=radius_facts(ready,starts[0])
        require(int(ready[8],16)==0x2380,'Lifecycle Idle flags differ')
        chunk=text[start.end():starts[i+1].start() if i+1<len(starts) else len(text)]
        current=frames(chunk)
        if current:require(current[0]['actor_frames']==1 and current[0]['physics_steps']==1,'Context counters did not restart together')
        worlds=WORLD.findall(chunk)
        out.append({'index':i+1,'frames':current,'position':list(map(float,worlds[-1])) if worlds else None,'radius_validation':radius})
    return out

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--adb',required=True);parser.add_argument('--serial',required=True)
    parser.add_argument('--apk',type=Path,required=True);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();require(args.serial.startswith('emulator-'),'Named development emulator required')
    args.output.mkdir(parents=True,exist_ok=True)
    report={'validation':'FAIL','apk_sha256':digest(args.apk),'serial':args.serial,'fresh_launch':False,'original_app_fsm_parity_verified':False,'physical_arm64_phone_tested':False}
    commands=[];last_logs='';prior=[];touched=False;deadline=time.monotonic()+65
    def adb(*command,cleanup=False,missing=False):
        remaining=deadline-time.monotonic();require(cleanup or remaining>0,'Lifecycle smoke exceeded65-second budget')
        timeout=5 if cleanup else min(12,max(.1,remaining))
        try:run=subprocess.run([args.adb,'-s',args.serial,*command],capture_output=True,text=True,timeout=timeout)
        except subprocess.TimeoutExpired:
            commands.append({'args':list(command),'timeout_seconds':timeout});raise
        commands.append({'args':list(command),'returncode':run.returncode,'stdout':run.stdout,'stderr':run.stderr})
        if missing and run.returncode==1 and not (run.stdout+run.stderr).strip():return ''
        require(run.returncode==0,'ADB lifecycle command failed: '+str(command)+'\n'+run.stdout+run.stderr);return run.stdout.strip()
    def logs():
        nonlocal last_logs
        pid=adb('shell','pidof',PACKAGE,missing=True);require(pid,'Crypt app process exited')
        if report.get('pid'):require(pid==report['pid'],'Activity lifecycle replaced the native process')
        else:report['pid']=pid
        last_logs=adb('logcat','-d','--pid='+pid,'-v','brief');require(not BAD.search(last_logs),'Native runtime failure during lifecycle smoke')
        contexts(last_logs);return last_logs
    def latest(text):
        all_contexts=contexts(text);require(all_contexts,'Current Crypt native context missing');return all_contexts[-1]
    def wait(predicate,label,timeout=16):
        until=min(deadline,time.monotonic()+timeout)
        while True:
            text=logs()
            if predicate(text):return text
            require(time.monotonic()<until,'Timed out waiting for '+label);time.sleep(.15)
    def idle(context):return bool(context['frames']) and context['frames'][-1]['clip'] in (1040,1041)
    def hierarchy(shape=None):
        until=min(deadline,time.monotonic()+12)
        while True:
            adb('shell','uiautomator','dump','/sdcard/dh2-live-actor-lifecycle-window.xml')
            xml=adb('shell','cat','/sdcard/dh2-live-actor-lifecycle-window.xml');root=ET.fromstring(xml)
            views={n.get('content-desc'):tuple(map(int,re.findall(r'-?\d+',n.get('bounds','')))) for n in root.iter('node') if n.get('content-desc')}
            viewport=views.get('DH2 native texture viewport');pad=views.get('Movement control');text=logs()
            draws=re.findall(r'Model frame submitted at (\d+) x (\d+)',text)
            loaded=any('Crypt | 8 rooms | 11 monsters | 84 scenery objects' in n.get('text','') for n in root.iter('node'))
            if loaded and viewport and pad and draws:
                width,height=viewport[2]-viewport[0],viewport[3]-viewport[1]
                shaped=shape is None or (width>height if shape=='landscape' else height>width)
                if shaped and tuple(map(int,draws[-1]))==(width,height):
                    require(viewport[0]<=pad[0]<pad[2]<=viewport[2] and viewport[1]<=pad[1]<pad[3]<=viewport[3],'Movement control outside restored viewport')
                    return views,xml
            require(time.monotonic()<until,'Lifecycle viewport/control did not settle');time.sleep(.15)
    def capture(stem,shape=None):
        views,xml=hierarchy(shape);viewport=views['DH2 native texture viewport'];path=args.output/(stem+'.png')
        (args.output/(stem+'.xml')).write_text(xml,encoding='utf-8');adb('shell','screencap','-p','/sdcard/dh2-live-actor-lifecycle-test.png');adb('pull','/sdcard/dh2-live-actor-lifecycle-test.png',str(path))
        with Image.open(path) as image:crop=image.convert('RGB').crop(viewport)
        mask=ImageChops.difference(crop,Image.new('RGB',crop.size,(20,23,28))).convert('L').point(lambda v:255 if v>8 else 0)
        pixels=mask.histogram()[255];require(pixels>crop.width*crop.height*.2,'Blank restored native world viewport')
        report.setdefault('screenshots',[]).append({'path':path.name,'sha256':digest(path),'viewport':list(viewport),'movement_control_bounds':list(views['Movement control']),'rendered_pixels':pixels});return views
    def touch(kind,x,y):adb('shell','input','touchscreen','motionevent',kind,str(x),str(y))
    def center(views):
        left,top,right,bottom=views['Movement control'];return round((left+right)/2),round((top+bottom)/2)
    def zero_marker(views):
        # An unmatched UP may be ignored by Android; centered DOWN+UP is zero.
        count=len(POSITION.findall(logs()));x,y=center(views);touch('DOWN',x,y);touch('UP',x,y)
        return position(wait(lambda t:len(POSITION.findall(t))>count,'authoritative zero-input position'))
    try:
        paths=adb('shell','pm','path',PACKAGE).splitlines();require(len(paths)==1 and paths[0].startswith('package:'),'Single installed APK required')
        report['installed_apk_sha256']=adb('shell','sha256sum',paths[0].removeprefix('package:')).split()[0]
        require(report['installed_apk_sha256']==report['apk_sha256'],'Installed APK differs from lifecycle artifact')
        text=logs();require(idle(latest(text)),'Lifecycle smoke must begin after completed movement test in authored Idle')
        prior=adb('shell','cmd','window','user-rotation').split();require(prior and prior[0] in ('free','lock'),'Cannot preserve original rotation policy')
        report['original_rotation_policy']=prior
        # Establish portrait without resetting the running game.
        adb('shell','cmd','window','user-rotation','lock','0');views=capture('portrait-start','portrait');text=logs();require(idle(latest(text)),'Portrait setup is not Idle')
        baseline=zero_marker(views);report['initial_position']=baseline;report['rotation_cases']=[]
        for rotation,shape in ((1,'landscape'),(0,'portrait')):
            before=latest(logs());adb('shell','cmd','window','user-rotation','lock',str(rotation))
            text=wait(lambda t:latest(t)['index']>before['index'] and idle(latest(t)) and latest(t)['position'] is not None,'new '+shape+' native context')
            restored=latest(text);require(math.dist(baseline['game_xyz'],restored['position'])<.05,'Rotation changed game position')
            body_matches(restored['frames'][-1],restored['position']);views=capture(shape+'-restored',shape)
            point=zero_marker(views);require(math.dist(baseline['game_xyz'],point['game_xyz'])<.05,'Restored Idle moved game position')
            report['rotation_cases'].append({'orientation':shape,'previous_context':before['index'],'restored_context':restored['index'],'first_frame':restored['frames'][0],'position':point})
        # Keep the finger held through HOME. Activity.onPause must stop its axis;
        # no UP/CANCEL or zero-marker is sent until resumed Idle is observed.
        before=latest(logs());x,y=center(views);pad=views['Movement control'];held_y=round(y+(pad[2]-pad[0])*.44*.5)
        touch('DOWN',x,held_y);touched=True
        text=wait(lambda t:latest(t)['index']==before['index'] and any(f['clip']==1126 and f['actor_frames']>before['frames'][-1]['actor_frames'] for f in latest(t)['frames']),'held Walk before pause')
        moving=latest(text)['frames'][-1];count_positions=len(POSITION.findall(text));paused_context=latest(text)['index']
        adb('shell','input','keyevent','KEYCODE_HOME')
        text=wait(lambda t:len(POSITION.findall(t))>count_positions,'Activity.onPause zero-axis marker');paused=position(text)
        adb('shell','am','start','-W','-n',PACKAGE+'/.MainActivity')
        text=wait(lambda t:idle(latest(t)) and (latest(t)['index']>paused_context or latest(t)['frames'][-1]['actor_frames']>moving['actor_frames']),'resumed authored Idle without releasing held input')
        resumed=latest(text);first_idle=resumed['frames'][-1];views=capture('resume-idle','portrait')
        # The native Idle marker above proves onPause stopped the game axis
        # before any injected release. ADB's global held touch may survive HOME;
        # clear it now so Android accepts the fresh centered DOWN/UP pair.
        touch('CANCEL',0,0);touched=False
        # Capture AFTER resumed Idle. The onPause marker precedes any final
        # queued scene tick and is retained separately, without hiding that tail.
        settled=zero_marker(views);body_matches(first_idle,settled['game_xyz']);touched=False
        text=wait(lambda t:latest(t)['index']==resumed['index'] and latest(t)['frames'][-1]['actor_frames']>first_idle['actor_frames'],'continued unattended Idle')
        stable=latest(text)['frames'][-1];require(stable['clip'] in (1040,1041),'Held input survived pause/resume')
        stationary=zero_marker(views);require(math.dist(settled['game_xyz'],stationary['game_xyz'])<.05,'Unattended movement after resume');body_matches(stable,stationary['game_xyz'])
        report['pause_case']={'held_walk_frame':moving,'on_pause_coordinate':paused,'resumed_context':resumed['index'],'resumed_idle_frame':first_idle,'injected_touch_cancel_after_resumed_idle':True,'settled_idle':settled,'stable_idle_frame':stable,'stationary':stationary,'queued_transition_distance_xyz':math.dist(paused['game_xyz'],settled['game_xyz']),'pause_tail_scope':'onPause zero-axis log precedes the final queued scene/actor transition; stability is checked after native Idle, preserving the raw earlier coordinate'}
        report.update(validation='PASS',rotation_position_preserved=True,context_counter_resets_verified=True,pause_cancels_held_input=True,unattended_resume_movement=False,context_radius_checks=[{'context':r['index'],**r['radius_validation']} for r in contexts(text)],scope='Development Activity/GL-context lifecycle and genuine native bridge smoke. Original Application pause/FSM and GPU parity remain separate source reconstruction work.')
    except Exception as exc:report['error']=str(exc);raise
    finally:
        if touched or prior:
            try:adb('shell','input','touchscreen','motionevent','CANCEL','0','0',cleanup=True)
            except Exception as exc:report['touch_cleanup_error']=str(exc)
        if prior:
            try:adb('shell','cmd','window','user-rotation',*prior,cleanup=True)
            except Exception as exc:report['rotation_cleanup_error']=str(exc)
        if report.get('touch_cleanup_error') or report.get('rotation_cleanup_error'):report['validation']='FAIL'
        (args.output/'live-actor-lifecycle.log').write_text(last_logs+'\n',encoding='utf-8')
        (args.output/'adb-transcript.json').write_text(json.dumps(commands,indent=2)+'\n',encoding='utf-8')
        (args.output/'live-actor-lifecycle-smoke.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    require(report['validation']=='PASS','Lifecycle cleanup failed');print(json.dumps(report,indent=2))

if __name__=='__main__':main()
