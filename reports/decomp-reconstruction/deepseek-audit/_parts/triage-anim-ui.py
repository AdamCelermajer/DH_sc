"""anim-ui triage: statement-level alignment between original and reconstruction.

usage: python triage-anim-ui.py <prio> [category]        # rank rows by unmatched-statement score
       python triage-anim-ui.py --addr 0x341bcc          # show the aligned diff for one row
"""
import csv, os, re, sys
from collections import Counter

REPO = r"C:\Users\nacworkstation\Documents\Projects\Dungeon Hunter 2 Recomp\DH_sc\.worktrees\track-b"
HEX = os.path.join(REPO, "recovered", "native", "hexrays-all")
RECON = os.path.join(REPO, "port", "decomp-reconstruction")
QUEUE = r"C:\Users\nacworkstation\Documents\deepseek-harness\default-workspace\dh2-deepseek-audit\index\queue_anim-ui.tsv"
HDR = re.compile(r"^// ===== ([0-9a-fA-F]+) (.+?) =====\s*$")


def read_lines(p):
    with open(p, "r", encoding="utf-8", errors="replace") as f:
        return f.read().split("\n")


def rows():
    with open(QUEUE, encoding="utf-8", newline="") as f:
        return list(csv.DictReader(f, delimiter="\t"))


def spans(rec):
    """(orig_first_line, orig_lines, recon_first_line, recon_lines)"""
    ol = read_lines(os.path.join(HEX, rec["orig_chunk"]))
    s = int(rec["orig_span"].split("-")[0]) - 1
    e = int(rec["orig_span"].split("-")[1])
    hs = s
    while hs > 0 and not HDR.match(ol[hs].rstrip("\r")):
        hs -= 1
    he = e
    while he < len(ol) - 1 and not HDR.match(ol[he].rstrip("\r")):
        he += 1
    rl = read_lines(os.path.join(RECON, rec["recon_file"]))
    rs = int(rec["recon_span"].split("-")[0])
    re_ = int(rec["recon_span"].split("-")[1])
    return hs + 1, ol[hs:he], rs, rl[rs - 1:re_]


IDENT = re.compile(r"[A-Za-z_][A-Za-z0-9_]*")
NUMLIT = re.compile(r"(?<![0-9A-Za-z_.])(0[xX][0-9a-fA-F]+|\d+)(?![0-9A-Za-z_])")
FLOATLIT = re.compile(r"(?<![0-9A-Za-z_.])\d+\.\d+(?:[eE][+\-]?\d+)?f?(?![0-9A-Za-z_])")
STR = re.compile(r'"(?:[^"\\]|\\.)*"')


def strip_comments(text):
    out = []
    blk = False
    for raw in text.split("\n"):
        s = raw
        if blk:
            if "*/" in s:
                s = s.split("*/", 1)[1]
                blk = False
            else:
                s = ""
        while "/*" in s:
            b, rest = s.split("/*", 1)
            if "*/" in rest:
                s = b + " " + rest.split("*/", 1)[1]
            else:
                s = b
                blk = True
                break
        if "//" in s:
            s = s.split("//", 1)[0]
        out.append(s)
    return "\n".join(out)


def canon(text):
    """keep shape: replace identifiers/strings/numbers with placeholders but keep operators"""
    t = strip_comments(text)
    t = STR.sub("S", t)
    t = FLOATLIT.sub("F", t)
    t = NUMLIT.sub("N", t)
    t = IDENT.sub("v", t)
    t = re.sub(r"\s+", " ", t)
    return t


def stmts(text):
    """split into statements/branches; drop pure structural noise, keep (line, canonical)"""
    text = strip_comments(text)
    out = []
    for raw in text.split("\n"):
        s = raw.strip()
        if not s:
            continue
        out.append((raw, canon(raw).strip()))
    return out


def split_args(s):
    """split a statement into ~clauses on commas and operators for finer matching"""
    parts = re.split(r"(,|&&|\|\||[<>!=]=?|\(|\)|\{|\})", s)
    return [p.strip() for p in parts if p.strip()]


def score(rec):
    oh, ol, rh, rl = spans(rec)
    o = [c for _, c in stmts("\n".join(ol)) if c not in ("{", "}", "};", ";")]
    r = [c for _, c in stmts("\n".join(rl)) if c not in ("{", "}", "};", ";")]
    co, cr = Counter(o), Counter(r)
    only_o = co - cr
    only_r = cr - co
    matched = sum((co & cr).values())
    return matched, only_o, only_r, len(o), len(r), oh, ol, rh, rl


def cmd_rank(prio, cat=None, top=None):
    res = []
    for rec in rows():
        if rec["prio"] != prio:
            continue
        if cat and rec["category"] != cat:
            continue
        try:
            m, oo, orr, no, nr, _, _, _, _ = score(rec)
        except Exception as ex:
            print("ERR", rec["address"], ex)
            continue
        res.append((m, len(list(oo.elements())), len(list(orr.elements())), rec, oo, orr, no, nr))
    res.sort(key=lambda t: (-(t[1] + t[2]), -t[0]))
    for m, so, sr, rec, oo, orr, no, nr in (res[:top] if top else res):
        print("=" * 100)
        print("%s %s  %s %s -> %s %s  matched=%d/%d  only_orig=%d only_recon=%d" %
              (rec["address"], rec["symbol"], rec["orig_chunk"], rec["orig_span"], rec["recon_file"],
               rec["recon_span"], m, max(no, nr), so, sr))
        if oo:
            print("  ORIG-ONLY: " + " ; ".join("%s x%d" % (k[:70], v) for k, v in list(oo.items())[:12]))
        if orr:
            print("  RECON-ONLY: " + " ; ".join("%s x%d" % (k[:70], v) for k, v in list(orr.items())[:12]))


def cmd_addr(addr):
    rec = None
    for r in rows():
        if r["address"].lower() == addr.lower():
            rec = r
            break
    if rec is None:
        print("not found", addr)
        return
    m, oo, orr, no, nr, oh, ol, rh, rl = score(rec)
    print("%s %s" % (rec["address"], rec["symbol"]))
    print("---- ORIGINAL %s %d-%d ----" % (rec["orig_chunk"], oh, oh + len(ol) - 1))
    for i, ln in enumerate(ol):
        print("%6d| %s" % (oh + i, ln.rstrip()))
    print("---- RECONSTRUCTION %s %d-%d ----" % (rec["recon_file"], rh, rh + len(rl) - 1))
    for i, ln in enumerate(rl):
        print("%6d| %s" % (rh + i, ln.rstrip()))
    print("---- UNMATCHED ----")
    for k, v in list(oo.items()):
        print("  orig x%d: %s" % (v, k))
    for k, v in list(orr.items()):
        print("  recon x%d: %s" % (v, k))


if __name__ == "__main__":
    a = sys.argv[1:]
    if a[0] == "--addr":
        cmd_addr(a[1])
    else:
        cmd_rank(a[0], a[1] if len(a) > 1 and not a[1].isdigit() else None,
                 int(a[-1]) if a[-1].isdigit() and len(a) > 1 else None)
