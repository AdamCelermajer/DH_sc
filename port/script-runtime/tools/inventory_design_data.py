"""Inventory genuine constant inputs required by the Application design backend."""
import argparse,hashlib,json,struct,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
EXPECTED='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'

def constants(raw):
    # Inventory serialized records, including authored empty identifiers. Do
    # not impose the narrower animation preparation tool's nonempty policy.
    offset=0
    def integer():
        nonlocal offset
        if offset+4>len(raw):raise ValueError('Truncated constant word')
        n=struct.unpack_from('<i',raw,offset)[0];offset+=4;return n
    def count():
        n=integer()
        if n<0 or n>100000:raise ValueError('Invalid serialized count')
        return n
    def text():
        nonlocal offset
        n=count()
        if n>4096 or offset+n>len(raw):raise ValueError('Invalid constant text')
        value=raw[offset:offset+n].decode('ascii');offset+=n;return value
    out={}
    for _ in range(count()):
        group=text();values={}
        for _ in range(count()):
            name=text();values[name]=integer()
        if group in out:raise ValueError('Repeated group needs ordered inventory')
        out[group]=values
    if offset!=len(raw):raise ValueError('Unexpected constants suffix')
    return out

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--cache',type=Path,default=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip'))
    p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    if a.output.exists():raise RuntimeError('Refusing to replace design inventory')
    with a.cache.open('rb') as f:assert hashlib.file_digest(f,'sha256').hexdigest()==EXPECTED
    sources=[];groups={};total=0;unparsed=[]
    with zipfile.ZipFile(a.cache) as z:
        for entry in z.infolist():
            if not entry.filename.lower().endswith('_pycst.bin'):continue
            raw=z.read(entry)
            try:decoded=constants(raw)
            except (ValueError,UnicodeError,struct.error) as error:
                unparsed.append(dict(entry=entry.filename,bytes=len(raw),
                    sha256=hashlib.sha256(raw).hexdigest(),error=str(error)))
                continue
            item=dict(entry=entry.filename,bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest(),
                      groups={name:len(values) for name,values in decoded.items()})
            sources.append(item)
            for name,values in decoded.items():
                total+=len(values)
                groups.setdefault(name,[]).append(dict(entry=entry.filename,values=values))
    status='PARTIAL' if unparsed else 'PASS'
    report=dict(validation=status,scope=__doc__,cache_sha256=EXPECTED,
        tool_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        sources=sources,constant_files=len(sources),group_names=len(groups),
        total_constants=total,unparsed_inputs=unparsed,
        all_constant_files_decoded=not unparsed,
        duplicate_group_sources={k:v for k,v in groups.items() if len(v)>1},
        ai_states=groups.get('AIStates',[]),
        original_loader_order_proved=False,all_constant_values=groups)
    a.output.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(dict(validation=status,files=len(sources),groups=len(groups),unparsed=unparsed,
                         constants=total,ai_states_sources=[x['entry'] for x in groups.get('AIStates',[])],
                         duplicate_groups=[k for k,v in groups.items() if len(v)>1])))

if __name__=='__main__':main()
