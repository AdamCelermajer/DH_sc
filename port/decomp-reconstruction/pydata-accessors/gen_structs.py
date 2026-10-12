#!/usr/bin/env python3
"""Generator for Structs::X::read / finalize (pure readAs shapes).

Reads the Ghidra pseudocode of every queued Structs::*::read and ::finalize
(via tools/decomp_extract.py), keeps only bodies whose statements are a plain
sequence of StreamReader::readAs stores, base read/finalize calls, and
CustomFree of a conditionally-set field, and emits:

  include/pydata-accessors/structs_records.hpp  record structs (not in script_cmd.hpp)
  src/structs_records.cpp                       their read/finalize definitions
  src/structs_script_cmd_read.cpp               read/finalize for script_cmd.hpp types
  include/pydata-accessors/script_cmd.hpp       declarations inserted (marked "// gen")

Bodies that do not fit are left out and remain queued (see NOTES.md).
Run from the repository root:  python3 port/decomp-reconstruction/pydata-accessors/gen_structs.py
"""
import csv, os, re, subprocess, sys, collections

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', '..'))
CAT = os.path.join(ROOT, 'port/decomp-reconstruction/pydata-accessors')
QUEUE = os.path.join(CAT, 'queue.csv')

TY = {'a': 'std::int8_t', 'b': 'bool', 's': 'std::int16_t', 'j': 'std::uint32_t',
      'i': 'std::int32_t', 'f': 'float'}
RA = re.compile(r'_ZN12StreamReader6readAsI(\w)EEvP11IStreamBasePT_\(param_2,param_1 \+ ((?:0x)?[0-9a-f]+)\)$')
BASE_READ = re.compile(r'_ZN7Structs\d+(\w+?)4readEP11IStreamBase\(\)$')
BASE_FIN = re.compile(r'_ZN7Structs\d+(\w+?)8finalizeEv\(param_1\)$')
IF_FIELD = re.compile(r'if \(\*\(int \*\)\(param_1 \+ ((?:0x)?[0-9a-f]+)\) != 0\) \{$')
WARN = re.compile(r'^\s*/\* WARNING')


def pseudo(addr):
    out = subprocess.run([sys.executable, 'tools/decomp_extract.py', '--address', '0x' + addr],
                         cwd=ROOT, capture_output=True, text=True).stdout
    lines = [l.rstrip() for l in out.splitlines()
             if not WARN.match(l) and not l.startswith('// =====') and not l.startswith('/* address')]
    txt = '\n'.join(lines)
    m = re.search(r'\)\s*\n\s*\{\n', txt)
    if not m:
        return None
    body = txt[m.end():]
    return body[:body.rfind('}')]


def stmts(body):
    return [s.strip() for s in re.split(r';|\n', body) if s.strip()]


def parse_read(body):
    ops = []
    for s in stmts(body):
        if s in ('return', '}'):
            continue
        m = RA.match(s)
        if m:
            ops.append(('f', int(m.group(2), 0) if m.group(2).startswith('0x') else int(m.group(2), 16)
                        if False else int(m.group(2), 0) if m.group(2).startswith('0x') else int(m.group(2), 16),
                        TY[m.group(1)]))
            continue
        m = BASE_READ.match(s)
        if m:
            ops.append(('base', m.group(1)))
            continue
        return None
    return ops


def parse_fin(body):
    ops = []
    pend = None
    for s in stmts(body):
        if s in ('return', '}'):
            continue
        m = IF_FIELD.match(s + ' {') if not s.endswith('{') else IF_FIELD.match(s)
        if m:
            pend = int(m.group(1), 0) if m.group(1).startswith('0x') else int(m.group(1))
            continue
        if s == '_Z10CustomFreePv()':
            if pend is None:
                return None
            ops.append(('free', pend))
            pend = None
            continue
        m = BASE_FIN.match(s)
        if m:
            ops.append(('base', m.group(1)))
            continue
        return None
    return ops


def off_hex(o):
    return '0x%02x' % o


def main():
    q = list(csv.DictReader(open(QUEUE)))
    ledger = list(csv.DictReader(open(os.path.join(CAT, 'ledger.csv')))) if os.path.exists(os.path.join(CAT, 'ledger.csv')) else []
    done = {r['elf_address'] for r in ledger}
    reads, fins = {}, {}
    for r in q:
        if r['elf_address'] in done:
            continue
        d = r['demangled']
        m = re.match(r'void Structs::(\w+)::(read|finalize)\(', d) or re.match(r'void Structs::(\w+)::(read|finalize)\(', d)
        if not m:
            continue
        cls, kind = m.group(1), m.group(2)
        (reads if kind == 'read' else fins)[cls] = r['elf_address']
    print('queued read', len(reads), 'finalize', len(fins), file=sys.stderr)
    return reads, fins


if __name__ == '__main__':
    reads, fins = main()
