from pathlib import Path
import subprocess,xml.etree.ElementTree as ET,re,time,sys
r=Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc\port\level-loader\reports')
a=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-P','5039','-s','emulator-5590']
def run(args):
 assert subprocess.check_output(a+['emu','avd','name'],text=True,timeout=15).replace('\r','').splitlines()[0]=='DH2_Loader_API37'
 return subprocess.check_output(a+args,timeout=30)
run(['shell','uiautomator','dump','/sdcard/loader-ui.xml'])
xml=run(['shell','cat','/sdcard/loader-ui.xml']);tree=ET.fromstring(xml)
nodes=[n for n in tree.iter('node') if n.attrib.get('text','').lower()=='next object'];assert len(nodes)==1,[(n.attrib.get('text'),n.attrib.get('bounds')) for n in tree.iter('node') if n.attrib.get('text')]
coords=list(map(int,re.findall(r'\d+',nodes[0].attrib['bounds'])));x=(coords[0]+coords[2])//2;y=(coords[1]+coords[3])//2
for _ in range(int(sys.argv[1])):run(['shell','input','tap',str(x),str(y)]);time.sleep(.5)
time.sleep(1);png=run(['exec-out','screencap','-p']);assert png.startswith(b'\x89PNG')
p=r/('visible-entities-v32-'+sys.argv[2]+'.png');p.write_bytes(png);print(p)
