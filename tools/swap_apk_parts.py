import sys,zipfile
src,dex,so,out=sys.argv[1:5]
zi=zipfile.ZipFile(src)
with zipfile.ZipFile(out,'w') as zo:
    for i in zi.infolist():
        n=i.filename
        if n.startswith('META-INF/'): continue
        if n=='classes.dex': zo.write(dex,n,zipfile.ZIP_DEFLATED); continue
        if n=='lib/arm64-v8a/libDungeonHunter2.so': zo.write(so,n,zipfile.ZIP_STORED); continue
        with zi.open(i) as f, zo.open(i,'w',force_zip64=True) as g:
            while True:
                b=f.read(1<<20)
                if not b: break
                g.write(b)
