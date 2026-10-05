"""Execute original ARM32 TinyXML constructor/parser; no parsed tree fixtures.

Heap allocation and imported byte/string operations are explicit services.
The projection offsets are taken from original node/attribute accessors. This
does not establish the level caller's handling of parser errors.
"""
import argparse, hashlib, json, pathlib, struct, sys
ROOT = pathlib.Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT/'port/game-data/tests'))
from items_differential import Original
from unicorn import UC_HOOK_CODE

class Parser(Original):
    def __init__(self, path, manifest):
        super().__init__(path, manifest)
        self.heap = self.data+0x200000
        # Imported Bionic C-locale byte classification table, not an XML result.
        # Slot zero is EOF; high bytes have no ASCII classification bits.
        table = bytearray(257)
        for c in range(128):
            table[c+1] = ((1 if 65<=c<=90 else 0) | (2 if 97<=c<=122 else 0)
                | (4 if 48<=c<=57 else 0) | (8 if c in (9,10,11,12,13,32) else 0)
                | (16 if 33<=c<=126 and not chr(c).isalnum() else 0)
                | (32 if c<32 or c==127 else 0)
                | (64 if chr(c) in '0123456789ABCDEFabcdef' else 0)
                | (128 if c in (9,32) else 0))
        p = self.data+0x8000; self.uc.mem_write(p,bytes(table)); self.pointer(p+512,p)
        self.pointer(0x996874,p+512)
        for offset, got, upper in ((0x9000,0x9950c0,True),(0xa000,0x998178,False)):
            values = [-1]+[(c-32 if upper and 97<=c<=122 else
                           c+32 if not upper and 65<=c<=90 else c) for c in range(256)]
            p = self.data+offset
            self.uc.mem_write(p,struct.pack('<257h',*values));self.pointer(p+1024,p)
            self.pointer(got,p+1024)
    def text(self, p):
        if not p: return ''
        raw = bytearray()
        while len(raw) < 1048576:
            b = bytes(self.uc.mem_read(p+len(raw), 1))
            if b == b'\0': return raw.decode('utf-8')
            raw.extend(b)
        raise AssertionError('Unterminated original string')
    def allocation(self, uc, addr, size, unused):
        if addr in (0x310570,0x3106d4,0x5341ac,0x53419c):
            count = self.reg(0); assert count < 0x1000000
            p = self.heap; self.heap += (count+15)&~15
            assert self.heap < self.data+0x2000000
            if count: uc.mem_write(p, bytes(count))
            self.returned(p)
    def external(self, uc, addr, size, unused):
        name = self.imports.get(addr)
        if name == 'strlen': self.returned(len(self.text(self.reg(0)).encode()))
        elif name in ('strcmp','strncmp'):
            a,b = self.text(self.reg(0)).encode(),self.text(self.reg(1)).encode()
            if name == 'strncmp': a,b = a[:self.reg(2)],b[:self.reg(2)]
            self.returned((a>b)-(a<b))
        elif name in ('memmove','memcmp'):
            a,b,n = [self.reg(i) for i in range(3)]; assert n <= 0x1000000
            rb = bytes(uc.mem_read(b,n))
            if name == 'memmove':
                if n: uc.mem_write(a, rb)
                self.returned(a)
            else:
                ra = bytes(uc.mem_read(a,n)); self.returned((ra>rb)-(ra<rb))
        elif name in ('pthread_mutex_lock','pthread_mutex_unlock'):
            # Single-threaded caller: no competing lock owner or XML outcomes.
            self.returned(0)
        elif name in ('_Znwj','_Znaj','malloc'):
            count=self.reg(0);assert count<0x1000000
            p=self.heap;self.heap+=(count+15)&~15
            assert self.heap<self.data+0x2000000
            if count:uc.mem_write(p,bytes(count))
            self.returned(p)
        elif name in ('_ZdlPv','_ZdaPv','free'):self.returned()
        elif name in ('__aeabi_uidiv','__aeabi_idiv'):
            a,b=self.reg(0),self.reg(1)
            if name=='__aeabi_idiv':
                a=a if a<0x80000000 else a-0x100000000
                b=b if b<0x80000000 else b-0x100000000
            assert b;self.returned((abs(a)//abs(b))*(-1 if (a<0)!=(b<0) else 1))
        else: super().external(uc,addr,size,unused)
    def element(self, p):
        assert self.word(p+0x14) == 1
        attrs = {}; sentinel = p+0x40; a = self.word(sentinel+0x48); count = 0
        while a != sentinel:
            assert a and count < 10000; count += 1
            name,value = self.text(self.word(a+0x28)),self.text(self.word(a+0x40))
            assert name not in attrs; attrs[name] = value; a = self.word(a+0x48)
        children = []; c = self.word(p+0x18)
        while c:
            if self.word(c+0x14) == 1: children.append(self.element(c))
            c = self.word(c+0x3c)
        return dict(tag=self.text(self.word(p+0x34)),attributes=attrs,children=children)
    def parse(self, raw):
        assert b'\0' not in raw and len(raw)<0x100000
        doc = self.data+0x10000; inp = self.data+0x100000
        self.uc.mem_write(doc,bytes(112)); self.uc.mem_write(inp,raw+b'\0')
        self.invoke(0x516ec4,[doc]); self.invoke(0x51a734,[doc,inp,0,0],budget=30000000)
        root = self.word(doc+0x18)
        while root and self.word(root+0x14) != 1: root = self.word(root+0x3c)
        return dict(xml_error=self.word(doc+0x44),error_description=self.text(self.word(doc+0x5c)),
                    root=self.element(root) if root else None)

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--engine',type=pathlib.Path,required=True)
    ap.add_argument('--input',type=pathlib.Path,required=True);a=ap.parse_args()
    p=Parser(a.engine,{'functions':[]});p.uc.hook_add(UC_HOOK_CODE,p.allocation)
    print(json.dumps(p.parse(a.input.read_bytes())))

if __name__=='__main__': main()
