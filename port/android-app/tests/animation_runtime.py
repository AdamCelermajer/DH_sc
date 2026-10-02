#!/usr/bin/env python3
"""Repeat the source walk-preview check on a running Android emulator.

Uses adb, UI hierarchy selectors and privately supplied cache files. It installs
the selected source APK, stages three fixtures in Downloads, and records local
evidence. It does not execute or package the original game engine.
"""
import argparse
import base64
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time
import xml.etree.ElementTree as ET

PACKAGE = 'local.dh2.sourceviewer'
FIXTURES = [
    ('IMPORT BRES SCENE', 'data/3d/characters/prince/prince_low_poly_warrior.bdae', 'dh2qa_warrior.bdae'),
    ('IMPORT PVRTC TEXTURE', 'data/3d/textures/prince-warrior.tga', 'dh2qa_texture.tga'),
    ('IMPORT CHARACTER ANIMATION', 'data/3d/characters/prince/animations/prince_walk_1hand.bdae', 'dh2qa_walk.bdae'),
]

class Check:
    def __init__(self, args):
        self.a = args
        self.a.evidence.mkdir(parents=True, exist_ok=True)
        self.fixtures = [*FIXTURES[:2], (FIXTURES[2][0], args.animation, FIXTURES[2][2])]

    def run(self, *args):
        return subprocess.check_output([str(self.a.adb), '-s', self.a.serial, *args],
            text=True, encoding='utf-8', errors='replace', timeout=60)

    def state(self):
        for _ in range(3):
            if self.a.ui_helper:
                output=self.run('shell','am','instrument','-w','local.dh2.uitest/.Snapshot')
                match=re.search(r'^INSTRUMENTATION_RESULT: hierarchy=(.+)$',output,re.MULTILINE)
                if match:
                    return list(ET.fromstring(base64.b64decode(match.group(1))).iter('node'))
                time.sleep(0.4)
                continue
            self.run('shell','rm','-f','/sdcard/dh2-source-qa.xml')
            output=self.run('shell', 'uiautomator', 'dump', '/sdcard/dh2-source-qa.xml')
            if 'dumped to:' in output:
                return list(ET.fromstring(self.run('shell', 'cat', '/sdcard/dh2-source-qa.xml')).iter('node'))
            time.sleep(0.4)
        raise RuntimeError('Android did not return a fresh UI hierarchy: '+output[-1000:])

    def find(self, **attributes):
        for _ in range(3):
            nodes = self.state()
            matches = [n for n in nodes if all(n.get(k) == v for k,v in attributes.items())]
            if len(matches)==1:return matches[0]
            time.sleep(0.4)
        raise RuntimeError(f'Expected one visible selector {attributes}; found {len(matches)}')

    def click(self, node):
        x1,y1,x2,y2 = map(int,re.findall(r'\d+',node.get('bounds')))
        self.run('shell','input','tap',str((x1+x2)//2),str((y1+y2)//2))

    def import_file(self, button, filename):
        self.click(self.find(text=button))
        self.click(self.find(**{'content-desc':'Search'}))
        self.run('shell','input','text',Path(filename).stem)
        self.run('shell','input','keyevent','66')
        self.click(self.find(text=filename))
        return [n.get('text') for n in self.state() if n.get('class') == 'android.widget.TextView' and n.get('text')]

    def capture(self, name):
        self.state()
        self.run('shell','screencap','-p','/sdcard/dh2-source-qa.png')
        path = self.a.evidence/name
        self.run('pull','/sdcard/dh2-source-qa.png',str(path))
        return {'file':name, 'sha256':hashlib.sha256(path.read_bytes()).hexdigest()}

    def position(self):
        text=[n.get('text') for n in self.state() if n.get('text')]
        values=[int(m.group(1)) for s in text
            if (m:=re.search(r'Animation preview: (\d+) / (\d+) ms',s))
            and int(m.group(2))==self.a.duration]
        assert len(values)==1 and 0<=values[0]<=self.a.duration,text
        return values[0]

    def check(self):
        assert self.run('shell','getprop','ro.kernel.qemu').strip() == '1', 'This check requires an emulator'
        if self.a.ui_helper:self.run('install','-r',str(self.a.ui_helper.resolve()))
        for _,relative,_ in self.fixtures:
            if not (self.a.cache/relative).is_file(): raise FileNotFoundError(self.a.cache/relative)
        self.run('install','-r',str(self.a.apk.resolve()))
        self.run('shell','mkdir','-p','/sdcard/Download/dh2-source-qa')
        fixture_hashes = {}
        for _,relative,name in self.fixtures:
            path = self.a.cache/relative
            self.run('push',str(path.resolve()),f'/sdcard/Download/dh2-source-qa/{name}')
            fixture_hashes[relative] = hashlib.sha256(path.read_bytes()).hexdigest()
        start = self.run('shell','date','+%m-%d_%H:%M:%S.000').strip().replace('_',' ')
        self.run('shell','am','force-stop',PACKAGE)
        self.run('shell','am','start','-n',PACKAGE+'/.MainActivity')
        statuses = [self.import_file(button,name) for button,_,name in self.fixtures]
        assert any('335 vertices, 1092 indices, 18 bones' in s for s in statuses[0]), statuses[0]
        assert any('256 x 256' in s for s in statuses[1]), statuses[1]
        assert any(f'{self.a.tracks} tracks, {self.a.duration} ms' in s for s in statuses[2]), statuses[2]
        shots = [self.capture('start.png')]
        self.click(self.find(**{'class':'android.widget.SeekBar'}))
        text = [n.get('text') for n in self.state() if n.get('text')]
        midpoints = [int(m.group(1)) for s in text
            if (m := re.search(r'Animation preview: (\d+) / (\d+) ms', s))
            and int(m.group(2)) == self.a.duration]
        assert any(abs(ms - self.a.duration / 2) <= 1 for ms in midpoints), text
        shots.append(self.capture('middle.png'))
        self.click(self.find(text='PLAY ANIMATION')); self.find(text='PAUSE ANIMATION')
        positions=[self.position() for _ in range(4)]
        assert len(set(positions))>1, ('Playback clock did not advance',positions)
        shots.append(self.capture('playing.png'))
        self.click(self.find(text='PAUSE ANIMATION')); self.find(text='PLAY ANIMATION')
        paused=[self.position(),self.position()]
        assert paused[0]==paused[1], ('Paused clock moved',paused)
        pid = self.run('shell','pidof',PACKAGE).strip(); assert pid
        log = self.run('logcat','-d','-T',start,'--pid='+pid)
        assert not any(s in log for s in ('FATAL EXCEPTION','Fatal signal','animated frame rejected','animation pose rejected'))
        (self.a.evidence/'runtime.log').write_text(log,encoding='utf-8')
        installed = self.run('shell','pm','path',PACKAGE).strip().removeprefix('package:')
        pulled = self.a.evidence/'installed.apk'; self.run('pull',installed,str(pulled))
        sha = lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
        assert sha(pulled) == sha(self.a.apk)
        result = {'serial':self.a.serial, 'android_release':self.run('shell','getprop','ro.build.version.release').strip(),
            'sdk':self.run('shell','getprop','ro.build.version.sdk').strip(),
            'page_size':self.run('shell','getconf','PAGE_SIZE').strip(),
            'apk_sha256':sha(self.a.apk), 'installed_apk_sha256':sha(pulled),
            'fixture_sha256':fixture_hashes, 'statuses':statuses, 'seek_status':text,
            'play_and_pause':True, 'pid':pid, 'fatal_in_run':False, 'log_start_emulator_gmt':start,
            'playing_positions_ms':positions,'paused_positions_ms':paused,
            'ui_helper_sha256':sha(self.a.ui_helper) if self.a.ui_helper else None,
            'test_sha256':sha(Path(__file__)),
            'screenshots':shots, 'visual_inspection_required':True,
            'complete_game':False, 'original_animator_equivalence':False}
        (self.a.evidence/'runtime.json').write_text(json.dumps(result,indent=2)+'\n')
        print(json.dumps(result))

def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--adb',required=True,type=Path)
    p.add_argument('--serial',required=True)
    p.add_argument('--apk',required=True,type=Path)
    p.add_argument('--cache',required=True,type=Path)
    p.add_argument('--evidence',required=True,type=Path)
    p.add_argument('--ui-helper',type=Path,help='Built snapshot instrumentation APK; required for a live updating playback UI')
    p.add_argument('--animation',default=FIXTURES[2][1],help='Animation path relative to the private cache')
    p.add_argument('--tracks',type=int,default=27,help='Expected imported track count')
    p.add_argument('--duration',type=int,default=799,help='Expected imported duration in milliseconds')
    Check(p.parse_args()).check()

if __name__ == '__main__': main()
