"""Lossless binary host replay projection of the original instruction corpus."""
import hashlib,json,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
REF=ROOT/'port/script-runtime/reference/int-bindings'
def words(*v):return struct.pack('<'+'I'*len(v),*v)
def blob(v):return words(len(v))+v
def main():
    source=REF/'int-original-gold.json';data=json.loads(source.read_text());out=bytearray(words(0x31544e49,data['cases']))
    for row in data['rows']:
        op=row['op'];out+=words(op,row['bits'],len(row['arguments']))+blob(bytes.fromhex(row['key_hex']))
        for record in row['arguments']:out+=words(*record)
        result=row['result']
        if op in (0,2,6):out+=words(result)
        elif op in (3,4):out+=words(len(result),*result)
        elif op==5:out+=words(result[0] is not None,result[1])+blob(bytes.fromhex(result[0]) if result[0] is not None else b'')
        for state in row['maps']:
            out+=words(len(state))
            for record in state:out+=words(*record)
        services=[s for s in row['services'] if s[0]!='parse'];out+=words(len(services))
        for name,value in services:out+=words(0 if name=='identity' else 1,value)
    target=REF/'int-original-gold.bin';target.write_bytes(out)
    report=dict(source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),binary_sha256=hashlib.sha256(out).hexdigest(),script_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),cases=data['cases'],numeric_parser_services='Actual host Lua parsing is tested; no host primitive hook is inserted.')
    (REF/'binary-projection.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
