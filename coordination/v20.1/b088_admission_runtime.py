"""B088 disclosed source-command fixtures; hardlinked read-only content, isolated saves.
Run with py -3 -I. All mutations are new files beneath b088-repair.
"""
from pathlib import Path
import os, json, shutil, xml.etree.ElementTree as ET
root=Path('C:/Users/adamc/Desktop/workspace/DH_sc')
out=root/'.local-inputs/v20.1/b088-repair'
assets=out/'assets'
original=root/'.local-inputs/v20.1/root/assets-integration'
if not assets.exists():
    for folder,dirs,files in os.walk(original):
        target=assets/Path(folder).relative_to(original)
        target.mkdir(parents=True,exist_ok=True)
        for name in files:os.link(Path(folder)/name,target/name)
# The only altered authored file is a private copy with fixture Dummy waypoints.
relative=Path('original-cache/data/3d/modules/swamp/mgp/obj_2of4_brdwalk_sw_00.mgp')
text=(original/relative).read_text()
waypoints=''
for name,position in [('B088_near','-2326.9,663.128,256.489'),('B088_far','-1326.9,663.128,256.489'),('B088_enemy_front','-2326.9,763.128,256.489'),('B088_enemy_side','-2476.9,663.128,256.489')]:
    waypoints+=f'<GameObject name="{name}" position="{position}" rotation="0,0,0" scale="1,1,1" gametype="Dummy" _templateName="Waypoint" />\n'
temp=assets/relative.with_suffix('.b088.tmp')
temp.write_text(text.replace('</Module>',waypoints+'</Module>'))
os.replace(temp,assets/relative)
doc=ET.Element('originalCampaign',version='1')
bank=ET.SubElement(doc,'scripts',scope='level')
script=ET.SubElement(bank,'script',id='0',name='B088Fixture')
commands=[(43,{12:'_prim_NPC_Mothgiver'}),(42,{12:'_prim_NPC_Mothgiver'}),
          (46,{12:'B088_far',20:'_prim_NPC_Mothgiver'}),(46,{12:'B088_near',20:'_prim_NPC_Mothgiver'}),
          (46,{12:'B088_enemy_front',20:'_prim_MothTemplate_06'}),(46,{12:'B088_enemy_side',20:'_prim_MothTemplate_06'})]
for index,(kind,strings) in enumerate(commands):
    c=ET.SubElement(script,'command',index=str(index),kind=str(kind),className='B088SourceFixture')
    ET.SubElement(c,'scalar',offset='4',bits=str(kind))
    for offset,value in strings.items():ET.SubElement(c,'string',offset=str(offset),value=value)
ET.ElementTree(doc).write(assets/'b088-fixture.xml',encoding='unicode')
lines=(root/'.local-inputs/v20.1/root/initial-runtime/npc/run.args').read_text().splitlines()
removed={'--assets','--position','--capture','--capture-frames','--frames','--game-save','--save','--space-key-interval','--window-size'}
base=[];i=0
while i<len(lines):
    if lines[i] in removed:i+=2
    else:base.append(lines[i]);i+=1
jobs=[]
cases={
    'hide-show':[(0,70),(1,180)],
    'move-far-return':[(2,70),(3,180)],
    'frontal-enemy':[(4,10)],
    'frontal-npc':[(5,10)],
    'hidden-npc-enemy':[(4,10),(0,40)],
    'talk':[],
    'compiled-talk':[],
    'merchant':[],
}
for stage,exe in [('before',root/'.local-inputs/v20.1/root/dh-foundation-merchant-close.exe'),('after',out/'dh-foundation-b088-frozen.exe')]:
    for name,schedule in cases.items():
        if name in ('compiled-talk','merchant') and stage=='before':continue
        d=out/'runtime-verified'/f'{stage}-{name}';d.mkdir(parents=True,exist_ok=True)
        config=base+['--assets',str(assets),'--position','-2326.9,6563.128,257','--frames','360','--capture',str(d/'last.ppm'),
                     '--capture-frames','35,65,100,210,320','--save',str(d/'profile.save'),'--game-save',str(d/'gameplay.save')]
        if name=='merchant':
            config[config.index('--position')+1]='-10900,12000,255'
            config+=['--space-key-interval','100:3']
        elif name=='compiled-talk':config+=['--campaign-triggers','--retain-hidden-actors','--space-key-interval','100:3','--caption-auto-tap-ms','1']
        elif name=='talk':config+=['--space-key-interval','100:3','--space-key-interval','250:3']
        elif schedule:
            config+=['--campaign-triggers','--retain-hidden-actors','--campaign-commands','b088-fixture.xml']
            for index,frame in schedule:config+=['--campaign-command',f'B088Fixture:{index}:{frame}']
            if name in ('frontal-enemy','hidden-npc-enemy'):config[config.index('--position')+1]='-2427,6563.128,257'
        (d/'run.args').write_text('\n'.join(config)+'\n')
        jobs.append(dict(name=f'{stage}-{name}',exe=str(exe),args=['--startup-config',str(d/'run.args')],cwd=str(d),log=str(d/'run.log'),timeoutSec=150))
(out/'jobs.json').write_text(json.dumps(jobs,indent=2))
print(len(jobs),'jobs generated; asset waypoint and XML command fixtures explicitly synthetic')
