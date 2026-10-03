"""Exercise the genuine native actor bridge on an explicitly named emulator.

No build or installation by default. --install explicitly enables adb install.
Uses actual touch controls and phase/body logs; does not assert a fixed stride,
obstacle force, original FSM, original camera or physical ARM64 phone parity.
"""
import argparse
import hashlib
import json
import math
import re
import struct
import subprocess
import time
import xml.etree.ElementTree as ET
import zipfile
from pathlib import Path
from PIL import Image, ImageChops
from emulator_smoke import launch_fresh

PACKAGE = 'com.example.dh2'
FLOAT = r'[-+]?(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][-+]?\d+)?'
READY = re.compile(r'Native actor ready \| genuine bodies (\d+) \| decor colliders (\d+) \| radius ('+FLOAT+r') \| source bounds ('+FLOAT+r') ('+FLOAT+r') ('+FLOAT+r') ('+FLOAT+r') \| flags ([0-9a-fA-F]+) \| scene then Step then actor')
FRAME = re.compile(r'Native actor frame \| scene (\d+) \| Step (\d+) \| actor (\d+) \| source phase (\d+) \| clip (-?\d+) \| ms (-?\d+) \| replays (\d+) \| body ('+FLOAT+r') ('+FLOAT+r') \| contacts (\d+) (\d+)(?: \| state -?\d+ \| body present \d+ \| timeline scale ('+FLOAT+r'))?')
POSITION = re.compile(r'Player position ('+FLOAT+r') ('+FLOAT+r') ('+FLOAT+r') \| moved (\d+) \| blocked (\d+)')
WORLD = re.compile(r'World ready .*?position ('+FLOAT+r') ('+FLOAT+r') ('+FLOAT+r')')
BAD = re.compile(r'FATAL EXCEPTION|Fatal signal|Native frame failed|World load failed|GL error|Shader failed|Link failed|(?:World|Model|Asset|Animation|Skin|Object|Actor|Combat).*?(?:load|sample|draw|route|dispatch|completion|animation) failed',re.I)
CLIP_ENDS = {1040:1066,1041:2333,1126:1066,1114:800}

def require(value, message):
    if not value: raise AssertionError(message)

def digest(path):
    with Path(path).open('rb') as f: return hashlib.file_digest(f,'sha256').hexdigest()

def inspect_apk(apk):
    """Inspect current libraries without changing the historical smoke allowlist."""
    libraries=[]
    with zipfile.ZipFile(apk) as archive:
        for name in archive.namelist():
            if not name.startswith('lib/') or not name.endswith('.so'): continue
            abi=name.split('/')[1];raw=archive.read(name)
            require(abi in ('arm64-v8a','x86_64') and raw[:6]==b'\x7fELF\x02\x01', 'Unsupported ELF/ABI: '+name)
            require(Path(name).name!='libDungeonHunter2.so','Legacy oracle packaged: '+name)
            require(struct.unpack_from('<H',raw,18)[0]==(183 if abi=='arm64-v8a' else 62),'ELF machine mismatch: '+name)
            phoff=struct.unpack_from('<Q',raw,32)[0];phsize,phcount=struct.unpack_from('<HH',raw,54)
            require(phsize>=56 and phoff+phsize*phcount<=len(raw),'Malformed program headers: '+name)
            aligns=[struct.unpack_from('<Q',raw,phoff+i*phsize+48)[0] for i in range(phcount) if struct.unpack_from('<I',raw,phoff+i*phsize)[0]==1]
            require(aligns and min(aligns)>=16384,'Insufficient native load alignment: '+name)
            libraries.append({'path':name,'sha256':hashlib.sha256(raw).hexdigest(),'minimum_load_alignment':min(aligns)})
        require({r['path'].split('/')[1] for r in libraries}=={'arm64-v8a','x86_64'},'Both native ABIs required')
        for abi in ('arm64-v8a','x86_64'):
            require({'libdh2_native.so','libdh2_level_world.so','libdh2_game_data.so','libdh2_engine_animation.so','libdh2_engine_skinning.so','libdh2_scene_materials.so'}<={Path(r['path']).name for r in libraries if r['path'].split('/')[1]==abi},'Actor dependencies missing: '+abi)
        raw=archive.read('assets/player-locomotion-provenance.json');provenance=json.loads(raw)
        for clip in provenance['clips']:
            require(hashlib.sha256(archive.read('assets/'+clip['asset'])).hexdigest()==clip['sha256'],'Authored locomotion asset differs: '+clip['asset'])
        require({1040,1041,1114,1126}<={clip['clip_id'] for clip in provenance['clips']},'Default locomotion clips absent')
    return libraries,hashlib.sha256(raw).hexdigest()

def frames(text):
    out=[]
    for m in FRAME.finditer(text):
        r={'scene_ms':int(m[1]),'physics_steps':int(m[2]),'actor_frames':int(m[3]),'phase':int(m[4]),'clip':int(m[5]),'clip_ms':int(m[6]),'replays':int(m[7]),'body_xy':[float(m[8]),float(m[9])],'contacts':[int(m[10]),int(m[11])]}
        if m[12] is not None:r['timeline_scale']=float(m[12]);require(math.isfinite(r['timeline_scale']),'Nonfinite applied timeline scale')
        require(r['phase']==5 and r['physics_steps']==r['actor_frames'] and r['actor_frames']>0,'Incomplete or mismatched native phases: '+str(r))
        require(r['clip'] in CLIP_ENDS and 0<=r['clip_ms']<=CLIP_ENDS[r['clip']],'Unexpected locomotion timeline: '+str(r))
        require(all(math.isfinite(v) for v in r['body_xy']),'Nonfinite native body: '+str(r))
        if out:
            require(r['scene_ms']>=out[-1]['scene_ms'] and r['actor_frames']>out[-1]['actor_frames'] and r['replays']>=out[-1]['replays'],'Native frame counters regressed: '+str(r))
        out.append(r)
    return out

def position(text):
    found=POSITION.findall(text);require(found,'Player release coordinate marker missing')
    row=found[-1];point=list(map(float,row[:3]));require(all(map(math.isfinite,point)),'Nonfinite player coordinates')
    return {'game_xyz':point,'moved_frames':int(row[3]),'blocked_frames':int(row[4])}

def body_matches(frame,point):
    # Body GetPosition logs meters with six significant digits; game is *100.
    require(math.dist([v*100 for v in frame['body_xy']],point[:2])<.03,'Body/game XY disagree: '+str((frame,point)))

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--adb',required=True);parser.add_argument('--serial',required=True)
    parser.add_argument('--apk',type=Path,required=True);parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--install',action='store_true',help='Explicitly install the supplied APK before testing; never builds')
    args=parser.parse_args();require(args.serial.startswith('emulator-'),'This harness operates on a named development emulator only')
    args.output.mkdir(parents=True,exist_ok=True);report={'validation':'FAIL','result':'FAIL','apk_sha256':digest(args.apk),'serial':args.serial,'install_requested':args.install,'physical_arm64_phone_tested':False,'original_frame_order_proven_by_smoke':False,'full_game_playable':False}
    transcript=[];last_logs='';touch_started=False;started=False
    def adb(*command,missing=False):
        process=subprocess.run([args.adb,'-s',args.serial,*command],capture_output=True,text=True,timeout=45)
        transcript.append({'args':list(command),'returncode':process.returncode,'stdout':process.stdout,'stderr':process.stderr})
        if missing and process.returncode==1 and not (process.stdout+process.stderr).strip(): return ''
        require(process.returncode==0,'adb command failed: '+str(command)+'\n'+process.stdout+process.stderr)
        return process.stdout.strip()
    def logs():
        nonlocal last_logs
        pid=adb('shell','pidof',PACKAGE,missing=True);require(pid,'Native app process exited')
        if report.get('pid'):require(pid==report['pid'],'Native app process changed during smoke')
        else:report['pid']=pid
        last_logs=adb('logcat','-d','--pid='+pid,'-v','brief');require(not BAD.search(last_logs),'Native runtime failure in captured log')
        frames(last_logs);return last_logs
    def wait(predicate,label,timeout=40):
        deadline=time.monotonic()+timeout
        while True:
            text=logs()
            if predicate(text):return text
            require(time.monotonic()<deadline,'Timed out waiting for '+label);time.sleep(.2)
    def hierarchy():
        deadline=time.monotonic()+30
        while True:
            adb('shell','uiautomator','dump','/sdcard/dh2-live-actor-window.xml')
            xml=adb('shell','cat','/sdcard/dh2-live-actor-window.xml');root=ET.fromstring(xml)
            views={node.get('content-desc'):tuple(map(int,re.findall(r'-?\d+',node.get('bounds','')))) for node in root.iter('node') if node.get('content-desc')}
            viewport=views.get('DH2 native texture viewport');pad=views.get('Movement control')
            submitted=re.findall(r'Model frame submitted at (\d+) x (\d+)',logs())
            if viewport and pad and submitted and tuple(map(int,submitted[-1]))==(viewport[2]-viewport[0],viewport[3]-viewport[1]):
                require(viewport[0]<=pad[0]<pad[2]<=viewport[2] and viewport[1]<=pad[1]<pad[3]<=viewport[3],'Movement pad outside native viewport')
                (args.output/'ui-hierarchy.xml').write_text(xml,encoding='utf-8');return views
            require(time.monotonic()<deadline,'Native UI/viewport failed to settle');time.sleep(.2)
    def capture(stem):
        viewport=hierarchy()['DH2 native texture viewport'];path=args.output/(stem+'.png')
        adb('shell','screencap','-p','/sdcard/dh2-live-actor-test.png');adb('pull','/sdcard/dh2-live-actor-test.png',str(path))
        with Image.open(path) as image:crop=image.convert('RGB').crop(viewport)
        mask=ImageChops.difference(crop,Image.new('RGB',crop.size,(20,23,28))).convert('L').point(lambda v:255 if v>8 else 0)
        pixels=mask.histogram()[255];require(pixels>crop.width*crop.height*.2,'Native world screenshot has insufficient rendered geometry')
        report.setdefault('screenshots',[]).append({'path':path.name,'viewport':list(viewport),'rendered_pixels':pixels,'sha256':digest(path)})
    def touch(kind,x,y):adb('shell','input','touchscreen','motionevent',kind,str(x),str(y))
    def zero_touch(x,y):
        # Android may discard an UP with no preceding DOWN. A centered pair
        # dispatches actual zero-axis events without entering Move policy.
        touch('DOWN',x,y);touch('UP',x,y)
    try:
        report['libraries'],report['locomotion_provenance_sha256']=inspect_apk(args.apk)
        if args.install:require('Success' in adb('install','-r',str(args.apk)),'APK installation failed')
        paths=adb('shell','pm','path',PACKAGE).splitlines();require(len(paths)==1 and paths[0].startswith('package:'),'Single installed package APK required')
        report['installed_apk_sha256']=adb('shell','sha256sum',paths[0].removeprefix('package:')).split()[0]
        require(report['installed_apk_sha256']==report['apk_sha256'],'Installed APK differs from supplied artifact; use --install explicitly if intended')
        # Disabling the independent combat AI isolates locomotion from attacks.
        touch('CANCEL',0,0);launch_fresh(adb,'--es','world','crypt01.dwld','--ez','enemy_ai','false');started=True
        text=wait(lambda t:READY.search(t) and WORLD.search(t) and bool(frames(t)),'native actor ready and first completed frame')
        ready=READY.search(text);require((int(ready[1]),int(ready[2]))==(83,82),'Genuine body/collider count differs')
        require(abs(float(ready[3])-113.699707)<.00002,'Original mesh-derived player radius differs')
        require(int(ready[8],16)==0x2380,'Initial Idle flags differ')
        bounds=list(map(float,ready.group(4,5,6,7)));require(all(map(math.isfinite,bounds)) and bounds[0]<bounds[2] and bounds[1]<bounds[3],'Invalid original owner bounds')
        report['ready']={'genuine_bodies':83,'decor_colliders':82,'radius':float(ready[3]),'source_bounds':bounds,'flags':'2380'}
        start=list(map(float,WORLD.findall(text)[-1]));initial=frames(text)[-1]
        require(initial['clip'] in (1040,1041),'Initial authored Idle clip missing');body_matches(initial,start);capture('idle-initial')
        report['initial_frame']=initial;report['movement']=[]
        for label,length,clip in (('Walk',.5,1126),('Run',.95,1114)):
            before=frames(logs())[-1];before_positions=len(POSITION.findall(last_logs))
            # Log a fresh zero-input coordinate before the DOWN command.
            pad=hierarchy()['Movement control'];left,top,right,bottom=pad;x=round((left+right)/2);center_y=round((top+bottom)/2)
            zero_touch(x,center_y);text=wait(lambda t:len(POSITION.findall(t))>before_positions,'pre-move coordinates');origin=position(text)
            # Java pad normalizes by width*.44 and flips screenY into gameY.
            y=round((top+bottom)/2+(right-left)*.44*length)
            touch('DOWN',x,y);touch_started=True
            # Selection can coincide with a periodic marker before the first
            # authored root displacement. Require an actually displaced body.
            def displaced(f):
                return f['clip']==clip and f['actor_frames']>before['actor_frames'] and math.dist(before['body_xy'],f['body_xy'])>.0005
            text=wait(lambda t:any(displaced(f) for f in frames(t)),label+' native moving frame with body displacement')
            moving=[f for f in frames(text) if displaced(f)][-1]
            requested=280 if label=='Walk' else 271
            require(re.search(r'Character animation selected \| state 4 \| flags [0-9a-fA-F]+ \| sequence '+str(requested)+r' \|',text),'Source '+label+' sequence request missing')
            require(abs(moving.get('timeline_scale',float('inf'))-1.3)<.000001,'Applied '+label+' timeline scale missing or wrong')
            capture(label.lower()+'-held');release_count=len(POSITION.findall(logs()));touch('UP',x,y);touch_started=False
            text=wait(lambda t:len(POSITION.findall(t))>release_count,'touch release coordinates');raw_release=position(text)
            text=wait(lambda t:any(f['clip'] in (1040,1041) and f['actor_frames']>moving['actor_frames'] for f in frames(t)),'authored Idle after release')
            idle=frames(text)[-1]
            # moveAxis's UP log precedes the next scene -> Step -> Idle switch.
            # Take an authoritative zero-input marker after that transition.
            settled_count=len(POSITION.findall(text));zero_touch(x,center_y)
            text=wait(lambda t:len(POSITION.findall(t))>settled_count,'post-transition Idle coordinates');stopped=position(text)
            body_matches(idle,stopped['game_xyz'])
            distance=math.dist(origin['game_xyz'][:2],stopped['game_xyz'][:2])
            require(distance>.05 and stopped['moved_frames']>origin['moved_frames'],label+' did not move the native actor')
            require(math.dist(before['body_xy'],moving['body_xy'])>.0005,label+' did not update genuine body coordinates')
            # Wait another periodic Idle frame before requesting a fresh log.
            text=wait(lambda t:frames(t)[-1]['actor_frames']>idle['actor_frames'],'continued released Idle frames')
            stable_frame=frames(text)[-1];require(stable_frame['clip'] in (1040,1041),'Released control resumed movement')
            count_positions=len(POSITION.findall(text));zero_touch(x,center_y)
            text=wait(lambda t:len(POSITION.findall(t))>count_positions,'stationary coordinate marker');stationary=position(text)
            require(math.dist(stopped['game_xyz'],stationary['game_xyz'])<.05,'Actor drifted after touch release');body_matches(stable_frame,stationary['game_xyz'])
            capture(label.lower()+'-released')
            report['movement'].append({'state':label,'ui_axis_length':length,'requested_sequence':requested,'selected_clip':clip,'timeline_speed':moving['timeline_scale'],'speed_observation':'applied actor frame','origin':origin,'release':raw_release,'settled_idle':stopped,'stationary':stationary,'distance_xy':distance,'moving_frame':moving,'idle_frame':idle,'stable_frame':stable_frame})
        final=frames(logs());require(final[-1]['actor_frames']>initial['actor_frames'] and final[-1]['scene_ms']>initial['scene_ms'],'Native phases did not advance')
        require(final[-1]['replays']>initial['replays'],'No authored NewAnim replay observed')
        report.update(validation='PASS',result='PASS',source_phase=5,frame_markers=len(final),final_frame=final[-1],scene_then_step_then_actor_markers=True,live_touch_movement=True,release_returns_to_authored_idle=True,body_coordinates_match_game=True,scope='Native bridge runtime smoke. Touch destination/camera remain development adapters; source instruction parity and frame order are established by separate original-derived audits, not screenshots.')
        report['api']=adb('shell','getprop','ro.build.version.sdk');report['runtime_abi']=adb('shell','getprop','ro.product.cpu.abi')
    except Exception as exc:
        report['error']=str(exc);raise
    finally:
        if touch_started or started:
            try:touch('CANCEL',0,0)
            except Exception as exc:report['cleanup_error']=str(exc)
        (args.output/'live-actor.log').write_text(last_logs+'\n',encoding='utf-8')
        (args.output/'adb-transcript.json').write_text(json.dumps(transcript,indent=2)+'\n',encoding='utf-8')
        (args.output/'live-actor-smoke.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({k:v for k,v in report.items() if k not in ('libraries','screenshots','movement')},indent=2))

if __name__=='__main__':main()
