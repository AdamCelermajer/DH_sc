"""anim-ui slice helper: extract original+recon function text and compare features.

Read-only helper. Usage:
  python scan-anim-ui.py list <prio> [start] [count]
  python scan-anim-ui.py diff <address>            # print both sides with line numbers
  python scan-anim-ui.py fmt <address>             # print the two normalized texts
  python scan-anim-ui.py rank <prio>               # print machine-detectable feature diffs
"""
import csv, os, re, sys

REPO = r"C:\Users\nacworkstation\Documents\Projects\Dungeon Hunter 2 Recomp\DH_sc\.worktrees\track-b"
HEX = os.path.join(REPO, "recovered", "native", "hexrays-all")
RECON = os.path.join(REPO, "port", "decomp-reconstruction")
QUEUE = r"C:\Users\nacworkstation\Documents\deepseek-harness\default-workspace\dh2-deepseek-audit\index\queue_anim-ui.tsv"

HDR = re.compile(r"^// ===== ([0-9a-fA-F]+) (.+?) =====\s*$")


def read_lines(p):
    with open(p, "r", encoding="utf-8", errors="replace") as f:
        return f.read().split("\n")


def rows(cat=None, prio=None):
    out = []
    with open(QUEUE, encoding="utf-8", newline="") as f:
        for r in csv.DictReader(f, delimiter="\t"):
            if cat and r["category"] != cat:
                continue
            if prio and r["prio"] != prio:
                continue
            out.append(r)
    return out


def find_addr(cat, addr):
    a = addr.lower()
    for r in rows():
        if r["category"] == cat and r["address"].lower() == a:
            return r
    for r in rows():
        if r["address"].lower() == a:
            return r
    return None


def orig_fn(rec):
    """Original function text lines (raw, as printed) starting at its address header."""
    lines = read_lines(os.path.join(HEX, rec["orig_chunk"]))
    s = int(rec["orig_span"].split("-")[0]) - 1
    e = int(rec["orig_span"].split("-")[1])
    # widen: from the header line before s, through the next header
    hstart = s
    while hstart > 0 and not HDR.match(lines[hstart].rstrip("\r")):
        hstart -= 1
    hend = e
    while hend < len(lines) - 1 and not HDR.match(lines[hend].rstrip("\r")):
        hend += 1
    return hstart + 1, lines[hstart:hend]


def recon_fn(rec):
    lines = read_lines(os.path.join(RECON, rec["recon_file"]))
    s = int(rec["recon_span"].split("-")[0])
    e = int(rec["recon_span"].split("-")[1])
    # widen backwards to the // 0x<addr> marker of this address if present
    addr = rec["address"].lower()
    i = s - 1
    while i > 1 and not re.match(r"^//\s*0x", lines[i - 1]):
        i -= 1
    while i > 1 and not lines[i - 1].lower().startswith("// 0x" + addr[2:]):
        i -= 1
        break
    return s, e, lines


NUM = re.compile(r"(?<![0-9A-Za-z_.])(0[xX][0-9a-fA-F]+|\d+)(?![0-9A-Za-z_])")
STR = re.compile(r'"((?:[^"\\]|\\.)*)"')
CALL = re.compile(r"([A-Za-z_~][A-Za-z0-9_]*(?:::[A-Za-z_~][A-Za-z0-9_]*)*)\s*\(")
KW = set("""if else switch case default for while do break continue goto return sizeof void int
char short long float double unsigned signed bool const static struct class union enum typedef new
delete this true false nullptr NULL __fastcall __noreturn""".split())


def strip_comments(lines):
    out = []
    blk = False
    for raw in lines:
        s = raw.rstrip("\r")
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
    return out


def is_offset(text, i):
    pre = text[max(0, i - 2):i].rstrip()
    return pre.endswith("+") or pre.endswith("-") or pre.endswith("+")


def ints(lines):
    text = "\n".join(lines)
    res = []
    for m in NUM.finditer(text):
        tok = m.group(1)
        # skip when directly preceded by + or - (offset-like) or inside a 0x marker comment
        if is_offset(text, m.start()):
            continue
        try:
            v = int(tok, 16) if tok.lower().startswith("0x") else int(tok)
        except ValueError:
            continue
        if 0x1000 <= v <= 0x7FFFFFFF and tok.lower().startswith("0x"):
            # hex addresses of functions/globals are code pointers, keep separately
            res.append(("A", v))
        else:
            res.append(("I", v))
    return res


def counts(lines):
    text = "\n".join(lines)
    cf = {w: len(re.findall(r"(?<![A-Za-z0-9_])" + w + r"(?![A-Za-z0-9_])", text))
          for w in ("if", "else", "switch", "case", "default", "for", "while", "do",
                    "break", "continue", "goto", "return")}
    cmp_ = {op: len(re.findall(re.escape(op), text)) for op in ("<=", ">=", "==", "!=", "<", ">")}
    calls = set()
    for m in CALL.finditer(text):
        base = m.group(1).split("::")[-1]
        if base in KW:
            continue
        calls.add(m.group(1))
    strs = set(STR.findall(text))
    floats = set(re.findall(r"(?<![0-9A-Za-z_.])(\d+\.\d+(?:[eE][+\-]?\d+)?f?)(?![0-9A-Za-z_])", text))
    return cf, cmp_, calls, strs, floats


def norm_text(lines):
    """Crude C normalization for token-set comparison."""
    t = "\n".join(strip_comments(lines))
    t = re.sub(r"\b(v\d+|a\d+|result|this|_DWORD|_BYTE|_WORD|__fastcall|__noreturn|const|unsigned|signed|int|float|double|char|short|long|void|bool)\b", " ", t)
    return t


def toks(lines):
    t = norm_text(lines)
    return set(re.findall(r"[A-Za-z_][A-Za-z0-9_]*", t))


def cmd_list(prio, start=0, count=25):
    rs = [r for r in rows(prio=prio)]
    rs.sort(key=lambda r: (r["category"], r["address"]))
    for r in rs[start:start + count]:
        print("\t".join([r["category"], r["address"], r["symbol"], r["orig_chunk"], r["orig_span"],
                         r["recon_file"], r["recon_span"], r["lead"]]))


def cmd_diff(addr):
    rec = find_addr(None, addr)
    if not rec:
        print("not found", addr)
        return
    hs, ol = orig_fn(rec)
    for i, ln in enumerate(ol):
        print("%6d| %s" % (hs + i, ln.rstrip()))
    print("-" * 100)
    s, e, rl = recon_fn(rec)
    for i in range(max(1, s - 4), min(len(rl), e + 1) + 1):
        print("%6d| %s" % (i, rl[i - 1].rstrip()))


def cmd_fmt(addr):
    rec = find_addr(None, addr)
    hs, ol = orig_fn(rec)
    s, e, rl = recon_fn(rec)
    print("=== ORIG tokens-only ===")
    print(norm_text(ol))
    print("=== RECON tokens-only ===")
    print(norm_text(rl[s - 1:e]))


def cmd_rank(prio, cat=None):
    rs = rows(cat=cat, prio=prio)
    rs.sort(key=lambda r: (r["category"], r["address"]))
    for r in rs:
        try:
            hs, ol = orig_fn(r)
            s, e, rl = recon_fn(r)
            rl2 = rl[s - 1:e]
        except Exception as ex:
            print("ERR", r["address"], ex)
            continue
        ocf, ocmp, ocall, ostr, ofl = counts(ol)
        rcf, rcmp, rcall, rstr, rfl = counts(rl2)
        oi = ints(ol)
        ri = ints(rl2)
        oiv = {}
        riv = {}
        for k, v in oi:
            oiv[v] = oiv.get(v, 0) + 1
        for k, v in ri:
            riv[v] = riv.get(v, 0) + 1
        ohex = {v for k, v in oi if k == "A"}
        flags = []
        dcf = {w: (ocf[w], rcf[w]) for w in ocf if ocf[w] != rcf[w]}
        if dcf:
            flags.append("cf" + str(dcf))
        dcmp = {w: (ocmp[w], rcmp[w]) for w in ocmp if ocmp[w] != rcmp[w]}
        if dcmp:
            flags.append("cmp" + str(dcmp))
        ocb = {c.split("::")[-1] for c in ocall}
        rcb = {c.split("::")[-1] for c in rcall}
        if ocb - rcb:
            flags.append("orig-call-only=" + ",".join(sorted(ocb - rcb)))
        if rcb - ocb:
            flags.append("recon-call-only=" + ",".join(sorted(rcb - ocb)))
        if ostr - rstr:
            flags.append("orig-str-only=" + ",".join(sorted(ostr - rstr)))
        if rstr - ostr:
            flags.append("recon-str-only=" + ",".join(sorted(rstr - ostr)))
        # hex-code-pointer constants (function/global addresses)
        if ohex - {v for k, v in ri if k == "A"}:
            flags.append("orig-hex-only=" + ",".join(hex(v) for v in sorted(ohex - {v for k, v in ri if k == "A"})))
        # small integer multiset diff (ignore offsets)
        small = [v for k, v in oi if k == "I" and v < 0x10000]
        smallr = [v for k, v in ri if k == "I" and v < 0x10000]
        from collections import Counter
        cs, cr = Counter(small), Counter(smallr)
        onlyo = cs - cr
        onlyr = cr - cs
        if onlyo:
            flags.append("orig-int-only=" + ",".join(str(v) for v in sorted(onlyo)))
        if onlyr:
            flags.append("recon-int-only=" + ",".join(str(v) for v in sorted(onlyr)))
        if ofl - rfl:
            flags.append("orig-float-only=" + ",".join(sorted(ofl - rfl)))
        if rfl - ofl:
            flags.append("recon-float-only=" + ",".join(sorted(rfl - ofl)))
        ot = toks(ol)
        rt = toks(rl2)
        share = len(ot & rt) / max(1, len(ot | rt))
        if share < 0.55:
            flags.append("lowtoken=%.2f" % share)
        if flags:
            print("%-9s %-46s %s" % (r["address"], r["symbol"][:46], " | ".join(flags)))


if __name__ == "__main__":
    c = sys.argv[1]
    if c == "list":
        cmd_list(sys.argv[2], int(sys.argv[3]) if len(sys.argv) > 3 else 0,
                 int(sys.argv[4]) if len(sys.argv) > 4 else 25)
    elif c == "diff":
        cmd_diff(sys.argv[2])
    elif c == "fmt":
        cmd_fmt(sys.argv[2])
    elif c == "rank":
        cmd_rank(sys.argv[2], sys.argv[3] if len(sys.argv) > 3 else None)
