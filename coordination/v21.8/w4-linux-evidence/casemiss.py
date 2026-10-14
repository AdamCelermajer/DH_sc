import os, re, sys
d = sys.argv[1]
lim = int(sys.argv[2]) if len(sys.argv) > 2 else 40
pat = re.compile(r'^(?:\d+\s+)?(\w+)\((?:AT_FDCWD, )?"([^"]*)".*= (-1 ENOENT|\d+|0)')
def ci_find(p):
    cur = '/' if p.startswith('/') else '.'
    for part in p.split('/'):
        if part in ('', '.'): continue
        nxt = os.path.join(cur, part)
        if os.path.lexists(nxt): cur = nxt; continue
        try: names = os.listdir(cur)
        except OSError: return None
        m = [n for n in names if n.lower() == part.lower()]
        if not m: return None
        cur = os.path.join(cur, m[0])
    return cur
def norm(p):
    p = p if p.startswith('/') else os.path.join(d, p)
    return os.path.normpath(p)
ok = set(); miss = {}
for line in open(os.path.join(d, 'trace.txt'), errors='replace'):
    m = pat.match(line)
    if not m: continue
    call, path, res = m.groups()
    n = norm(path)
    if not re.search(r'/(assets|audio-assets|ui-assets)(/|$)', n + '/'): continue
    if res == '-1 ENOENT':
        if call == 'readlink': continue
        miss.setdefault(n, [0, call])[0] += 1
    else:
        ok.add(os.path.realpath(n).lower()); ok.add(n.lower())
cm, ab = [], []
for n, (c, call) in miss.items():
    hit = ci_find(n)
    (cm if hit else ab).append((n, c, call, hit))
rp = lambda p: os.path.relpath(p, d)
print('CASE-MISS: %d' % len(cm))
for n, c, call, hit in sorted(cm):
    st = 'OPENED-LATER' if (os.path.realpath(hit).lower() in ok or hit.lower() in ok) else 'NEVER-OPENED'
    print('  x%d %s %s -> %s [%s]' % (c, call, rp(n), os.path.basename(hit), st))
print('TRULY ABSENT: %d' % len(ab))
for n, c, call, hit in sorted(ab)[:lim]: print('  x%d %s %s' % (c, call, rp(n)))
