from pathlib import Path
import zipfile,struct,json,hashlib,xml.etree.ElementTree as ET,argparse
ROOT=Path(__file__).resolve().parents[3]
def produce(cache):
 with cache.open('rb')as f:assert hashlib.file_digest(f,'sha256').hexdigest()=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
 with zipfile.ZipFile(cache)as z:
  entry=next(n for n in z.namelist()if n.endswith('/scene/x07_crypt_backup.mlx'));raw=z.read(entry);nodes=[n for n in ET.fromstring(raw).iter('GameObject')if n.get('gametype')=='LevelConfig'];assert len(nodes)==1 and nodes[0].get('name')=='level_config';attrs=nodes[0].attrib;assert attrs['combat_music_enabled']=='1'
 def text(s):b=s.encode();return struct.pack('<I',len(b))+b
 payload=text(entry)+b''.join(text(k)+text(v)for k,v in attrs.items());binary=struct.pack('<4sIII',b'LCM1',1,16+len(payload),len(attrs))+payload
 out=ROOT/'port/level-world/reference/character-player-aggro-v1';out.mkdir(parents=True,exist_ok=True);(out/'crypt01-level-config-music.bin').write_bytes(binary)
 manifest=dict(cache_entry=entry,xml_sha256=hashlib.sha256(raw).hexdigest(),xml_bytes=len(raw),projection_sha256=hashlib.sha256(binary).hexdigest(),projection_bytes=len(binary),attributes=attrs,full_original_XML_factory=False);(out/'crypt01-level-config-music.json').write_text(json.dumps(manifest,indent=2)+'\n');print(json.dumps(manifest))
if __name__=='__main__':p=argparse.ArgumentParser();p.add_argument('--cache',type=Path,required=True);a=p.parse_args();produce(a.cache)
