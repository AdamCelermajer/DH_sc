"""Create a simple fan spawn override from this project's source-built APK.

The app performs the final floor/room validation when loading the override.
"""
import argparse, hashlib, json, math, pathlib, re, struct, zipfile

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--apk',type=pathlib.Path,required=True)
    p.add_argument('--world',default='crypt01.dwld')
    p.add_argument('--output',type=pathlib.Path,required=True,help='Mod folder to create')
    for axis in 'xyz':p.add_argument('--shift-'+axis,type=float,default=0)
    a=p.parse_args()
    if not re.fullmatch(r'[A-Za-z0-9_-]+\.dwld',a.world):p.error('Use an existing world file name without a directory')
    with zipfile.ZipFile(a.apk) as apk:raw=bytearray(apk.read('assets/worlds/'+a.world))
    if len(raw)<24 or raw[:4]!=b'DWLD':p.error('Not a port world descriptor')
    version,count=struct.unpack_from('<II',raw,4)
    if version!=1 or not 1<=count<=512 or len(raw)!=24+128*count:p.error('Unsupported world descriptor')
    original=list(struct.unpack_from('<3f',raw,12))
    target=[v+d for v,d in zip(original,(a.shift_x,a.shift_y,a.shift_z))]
    if not all(math.isfinite(v) and abs(v)<=10_000_000 for v in target):p.error('Spawn coordinate exceeds native limits')
    struct.pack_into('<3f',raw,12,*target)
    destination=a.output/'worlds'/a.world;destination.parent.mkdir(parents=True,exist_ok=True)
    destination.write_bytes(raw)
    print(json.dumps({'file':str(destination),'original_spawn':original,'modified_spawn':list(struct.unpack_from('<3f',raw,12)),'sha256':hashlib.sha256(raw).hexdigest(),'note':'The native app will reject a spawn without a compatible floor.'},indent=2))

if __name__=='__main__':main()
