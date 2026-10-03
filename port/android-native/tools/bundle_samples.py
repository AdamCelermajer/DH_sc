"""Bundle owner-supplied texture and static-scene fixtures, with provenance."""
import argparse
import hashlib
import json
from pathlib import Path
import zipfile

EXPECTED='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
SAMPLES={'pvr2_env_darktemple_alpha.tga','skybox_wind.tga','godparticle_face.tga','fx_smoke_03.tga','menugraphics03.tga'}
MODELS={'candle_flame.bdae','main_menu_charactere_swamp.bdae','prince_modular.bdae'}
ANIMATIONS={'prince_menu_idle_knight.bdae','prince_idle_shield.bdae','prince_walk_1hand.bdae'}
SAMPLES|={'env_crypt.tga','env_crypt_spec.tga','env_selectorcharactere_solide.tga','skybox_selector.tga','env_darkwoods_alpha.tga','pvr2_env_darkwoods_alpha_alpha.tga'}
SAMPLES.add('atlas_modular_warrior.tga')
SAMPLES.add('pvr2_env_crypt_alpha.tga')

def main():
    p=argparse.ArgumentParser();p.add_argument('--cache',type=Path,required=True)
    p.add_argument('--project',type=Path,default=Path(__file__).resolve().parents[1]);a=p.parse_args()
    with a.cache.open('rb') as source: digest=hashlib.file_digest(source,'sha256').hexdigest()
    if digest!=EXPECTED: raise SystemExit('Cache hash differs from the verified DH2 HD 1.0.2 archive')
    records=[];payloads={}
    with zipfile.ZipFile(a.cache) as archive:
        for e in archive.infolist():
            name=Path(e.filename).name
            if name not in SAMPLES|MODELS|ANIMATIONS: continue
            if name in payloads: raise ValueError('Duplicate texture fixture: '+name)
            raw=archive.read(e);payloads[name]=raw
            records.append({'name':name,'archive_entry':e.filename,'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()})
    if payloads.keys()!=SAMPLES|MODELS|ANIMATIONS:raise ValueError('Missing asset fixtures')
    assets=a.project/'app/src/main/assets';folder=assets/'textures';folder.mkdir(parents=True,exist_ok=True)
    (assets/'models').mkdir(parents=True,exist_ok=True)
    (assets/'animations').mkdir(parents=True,exist_ok=True)
    for name,raw in payloads.items():((assets/'models' if name in MODELS else assets/'animations' if name in ANIMATIONS else folder)/name).write_bytes(raw)
    (assets/'texture-provenance.json').write_text(json.dumps({'cache_sha256':digest,'samples':records},indent=2)+'\n')
    print(f'Bundled {len(MODELS)} scenes and {len(SAMPLES)} owner-supplied textures in {assets}')
if __name__=='__main__':main()
