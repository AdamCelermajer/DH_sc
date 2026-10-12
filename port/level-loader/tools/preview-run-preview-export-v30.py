from pathlib import Path
import subprocess,json,hashlib,xml.etree.ElementTree as ET
root=Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc');build=root.parent/'build/receiver-transport-host';loader=root/'port/level-loader'
out=root/'port/android-native/app/src/main/assets/loader-objects/SWAMP.xml';out.parent.mkdir(parents=True,exist_ok=True)
linux=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
args=['wsl.exe','-d','Ubuntu','--',linux(build/'dh2_loader_preview_entity_export_v30'),linux(Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')),linux(loader/'tests/native-ctor-v4/design.bin'),linux(build/'native-ctor-missing-saves'),linux(out)]
subprocess.run(args,check=True)
tree=ET.parse(out).getroot();entities=tree.findall('Entity');blocked=tree.findall('Blocked');coverage=tree.find('Coverage').attrib
result={'scope':'canonical constructed-prefix object asset inspection, not gameplay activation or full scene acceptance','entities':len(entities),'coverage':coverage,'assets':sorted(set(e.attrib['asset'] for e in entities)),'blocked':[e.attrib for e in blocked],'manifest_sha256':hashlib.sha256(out.read_bytes()).hexdigest()}
(loader/'reports/visible-entity-export-v30.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
