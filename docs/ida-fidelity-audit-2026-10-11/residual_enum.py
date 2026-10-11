"""Enumerate gameplay IDA functions NOT matched by partition.py, family = normalized class or free-fn name.
usage: py -I resid2.py <functions.jsonl> <partition.py> <out.json>"""
import json,re,sys,collections
src=open(sys.argv[2],encoding='utf8').read().split("out=collections")[0].replace("import json,sys,re,collections,os","import re")
ns={}; exec(src,ns); B,F=ns['B'],ns['F']

def norm(d):
    """strip leading backtick-thunk prefix and return type; return (class or None, name)"""
    d=re.sub(r"^`[^']*'","",d)
    depth=0; cut=len(d); lastsp=-1
    for i,ch in enumerate(d):
        if ch=='<': depth+=1
        elif ch=='>': depth-=1
        elif ch=='(' and depth==0: cut=i;break
        elif ch==' ' and depth==0: lastsp=i
    head=d[lastsp+1:cut]
    return head

def klass(head):
    # split on '::' at depth 0
    parts=[];depth=0;cur=''
    i=0
    while i<len(head):
        ch=head[i]
        if ch=='<':depth+=1
        elif ch=='>':depth-=1
        if depth==0 and head.startswith('::',i):
            parts.append(cur);cur='';i+=2;continue
        cur+=ch;i+=1
    parts.append(cur)
    return parts

fam=collections.defaultdict(lambda:[0,0,[]])
tot=0;matched=0
for line in open(sys.argv[1],encoding='utf8'):
    r=json.loads(line); d=r.get('demangled') or r['name']; tot+=1
    # partition's own logic (verbatim)
    m=re.match(r'(?:`[^\']*\')?([A-Za-z_]\w*)::',d); cls=m.group(1) if m else None
    if d.startswith('Structs::') or d.startswith('Arrays::'):
        cls=re.match(r'(?:Structs|Arrays)::(\w+)',d).group(1)
    hit=None
    for b,rx in B.items():
        if cls and re.search(rx,cls): hit=b;break
    if not hit and not cls:
        for b,rx in F.items():
            if re.search(rx,d): hit=b;break
    if not hit and cls=='Character': hit='c'
    if hit: matched+=1;continue
    head=norm(d); parts=klass(head)
    if parts[0] in('Structs','Arrays') and len(parts)>1: key=parts[0]+'::'+re.sub(r'<.*','',parts[1])
    elif len(parts)>1: key=re.sub(r'<.*','',parts[0])+'::'
    else:
        n=parts[0]
        if n.startswith('GetNewInstance') or n.startswith('GetNewState') or n.startswith('GetObjectiveImpl') or n.startswith('GetNewScriptCmdDataInstance'): key='<factory:'+n.split('<')[0]+'>'
        elif n.startswith('global constructor') or d.startswith('`global constructor'): key='<global-ctor>'
        elif r['name'].startswith('sub_'): key='<sub>'
        else: key='<free>'+n
    # re-test partition class regex on normalized class (functions whose return type hid the class)
    if len(parts)>1:
        c2=re.sub(r'<.*','',parts[0]) if parts[0] not in('Structs','Arrays') else parts[1]
        c2=re.sub(r'<.*','',c2)
        for b,rx in B.items():
            if re.search(rx,c2): key='HIDDEN-BY-RETTYPE:'+b+':'+key;break
    sz=int(r['end'],16)-int(r['address'],16)
    f=fam[key];f[0]+=1;f[1]+=sz;f[2].append((r['address'],sz,d[:140]))
print('total',tot,'matched',matched,'unmatched',tot-matched,'families',len(fam))
json.dump(fam,open(sys.argv[3],'w'))
