import json,struct
from pathlib import Path
R=Path(__file__).resolve().parents[3];ref=R/'port/engine-skinning/reference/visual-skin-owner-v6'
W=lambda *v:struct.pack('<'+'I'*len(v),*(int(x)&0xffffffff for x in v))
def S(s):b=(s or '').encode();return W(len(b))+b
def trace(rows):
 out=b'';codes={'construct_module':1,'release':2,'update_buffers':3,'visibility':4,'construct_weapon':5,'search':6,'detach':7,'attach':8}
 for row in rows:
  code=codes[row[0]];out+=W(code)
  if code==1:out+=S(row[1])
  elif code in(2,3,7,8):out+=W(row[1])
  elif code in(5,6):out+=S(row[1])+W(row[2])
 return out
def main():
 data=json.loads((ref/'selection-fixtures.json').read_text());out=b'SVK6'+W(len(data['catalog']))
 for c in data['catalog']:out+=S(c['name'])+S(c['default'])+W(len(c['modules']))+b''.join(S(m['uri'])for m in c['modules'])
 out+=W(len(data['cases']))
 for c in data['cases']:
  t=trace(c['trace']);out+=W(('category','module','set','weapon').index(c['kind']),c['old_id'],c['old_present'],c['flags'],c['success'],c['update_or_mode'],c['found'],c['slot'],c.get('mutation',False),c['argument']is not None,c['return'])+S(str(c['argument'])if c['argument']is not None else None)+W(len(t))+t
 (ref/'selection-fixtures.bin').write_bytes(out);print(len(out))
if __name__=='__main__':main()
