import json,sys,re,collections
c=collections.Counter(); sz=collections.Counter()
for line in open(sys.argv[1],encoding='utf8'):
    r=json.loads(line); d=r.get('demangled') or r['name']
    m=re.match(r'([A-Za-z_][\w]*)::',d)
    k=m.group(1) if m else ('<free>' if not d.startswith('sub_') else '<sub>')
    c[k]+=1; sz[k]+=int(r['end'],16)-int(r['address'],16)
for k,v in sorted(c.items(),key=lambda x:-sz[x[0]])[:450]: print(k,v,sz[k])
