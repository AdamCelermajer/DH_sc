#!/usr/bin/env python3
"""Compare reconstructed behavior against executed original ARM instructions.

Dependencies: unicorn and pyelftools. Inputs are the original user-supplied ELF
and a host build of nativeinterface.c. The original library is not loaded into
the host process; its ELF segments execute in a memory-only ARM32 emulator.
Libc calls are modeled explicitly. All identity pairs in this test are synthetic.
"""
import argparse
import ctypes
import hashlib
import io
import json
import random
import struct
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3, UC_ARM_REG_SP, UC_ARM_REG_LR, UC_ARM_REG_PC

BASE, STACK, HEAP, STOP = 0x10000, 0x200000, 0x400000, 0x900000

class Original:
    def __init__(self, data):
        self.uc = Uc(UC_ARCH_ARM, UC_MODE_ARM)
        self.uc.mem_map(BASE, 0x10000)
        self.uc.mem_map(STACK, 0x10000)
        self.uc.mem_map(HEAP, 0x100000)
        self.uc.mem_map(STOP, 0x1000)
        elf = ELFFile(io.BytesIO(data))
        for segment in elf.iter_segments():
            if segment['p_type'] == 'PT_LOAD':
                self.uc.mem_write(BASE + segment['p_vaddr'], segment.data())
        for relocation in elf.get_section_by_name('.rel.dyn').iter_relocations():
            at = BASE + relocation['r_offset']
            if relocation['r_info_type'] == 23:  # R_ARM_RELATIVE
                value = struct.unpack('<I', self.uc.mem_read(at, 4))[0] + BASE
                self.uc.mem_write(at, struct.pack('<I', value))
            elif relocation['r_info_type'] == 21:  # __stack_chk_guard
                self.uc.mem_write(at, struct.pack('<I', HEAP + 0xff000))
                self.uc.mem_write(HEAP + 0xff000, b'\x12\x34\x56\x78')
        self.next = HEAP
        self.file_bytes = None
        self.file_offset = 0
        self.array_lengths = {}
        self.jni_results = []
        self.env = self.bytes(b'\0' * 4)
        self.jni_table = self.bytes(b'\0' * (201 * 4))
        self.uc.mem_write(self.env, struct.pack('<I', self.jni_table))
        for index in (167, 169, 170, 171, 184, 192):
            self.uc.mem_write(self.jni_table + index * 4, struct.pack('<I', STOP + 0x100 + index * 4 + 1))
        self.uc.hook_add(UC_HOOK_CODE, self._hook)

    def bytes(self, value):
        at = self.next
        self.next += (len(value) + 31) & ~15
        self.uc.mem_write(at, value + b'\0\0\0\0')
        return at

    def string(self, at):
        result = bytearray()
        while True:
            b = bytes(self.uc.mem_read(at, 1))
            if b == b'\0': return bytes(result)
            result.extend(b); at += 1

    def _hook(self, uc, address, size, unused):
        if address == STOP:
            uc.emu_stop(); return
        if address >= STOP + 0x100:
            index = (address - STOP - 0x100) // 4
            a, b, c = [uc.reg_read(r) for r in (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2)]
            assert a == self.env
            if index in (169, 184): result = b  # GetStringUTFChars / GetByteArrayElements
            elif index == 171: result = self.array_lengths[b]
            elif index in (170, 192): result = 0  # ReleaseStringUTFChars / ReleaseByteArrayElements
            elif index == 167:  # NewStringUTF
                value = self.string(b); self.jni_results.append(value); result = self.bytes(value)
            else: raise AssertionError(index)
            uc.reg_write(UC_ARM_REG_R0, result)
            uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))
            return
        offset = address - BASE
        supported = {0x824, 0x830, 0x83c, 0x848, 0x854, 0x860, 0x86c,
                     0x878, 0x884, 0x890, 0x89c, 0x8a8, 0x8b4, 0x8c0, 0x8cc, 0x8d8}
        if offset not in supported: return
        a, b, c, d = [uc.reg_read(r) for r in (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3)]
        result = 0
        if offset == 0x824:  # wcslen; explicit padded buffers only
            while bytes(uc.mem_read(a + 4 * result, 4)) != b'\0' * 4: result += 1
        elif offset == 0x830: result = ord(chr(a).lower()) if a < 128 else a
        elif offset == 0x83c: result = self.bytes(b'\0' * a)
        elif offset == 0x848: raise RuntimeError('original __stack_chk_fail')
        elif offset == 0x854:
            value = self.string(a) + self.string(b)
            uc.mem_write(a, value + b'\0'); result = a
        elif offset == 0x860:
            value = self.string(b)[:c]
            uc.mem_write(a, value.ljust(c, b'\0')); result = a
        elif offset == 0x86c:
            aa, bb = bytes(uc.mem_read(a, c)), bytes(uc.mem_read(b, c))
            result = (aa > bb) - (aa < bb)
        elif offset == 0x878:
            value = (self.file_bytes or b'')[self.file_offset:self.file_offset+b*c]
            if value: uc.mem_write(a, value)
            self.file_offset += len(value); result = len(value) // b
        elif offset == 0x884:
            self.file_offset = 0; result = 0xf17e if self.file_bytes is not None else 0
        elif offset == 0x890:
            uc.mem_write(a, bytes([b & 255]) * c); result = a
        elif offset == 0x89c: result = 0
        elif offset == 0x8a8:
            assert self.string(b) == b'%02x'
            value = ('%02x' % c).encode(); uc.mem_write(a, value + b'\0'); result = len(value)
        elif offset == 0x8b4:
            result = a // b; uc.reg_write(UC_ARM_REG_R1, a % b)
        elif offset == 0x8c0:
            value = bytes(uc.mem_read(a, b*c))
            self.file_bytes = (self.file_bytes or b'') + value; result = c
        elif offset == 0x8cc: result = len(self.string(a))
        elif offset == 0x8d8: result = 0
        uc.reg_write(UC_ARM_REG_R0, result & 0xffffffff)
        uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))

    def call(self, offset, *arguments):
        self.uc.reg_write(UC_ARM_REG_SP, STACK + 0xff00)
        for reg, arg in zip((UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3), arguments):
            self.uc.reg_write(reg, arg)
        for index, arg in enumerate(arguments[4:]):
            self.uc.mem_write(STACK + 0xff00 + 4 * index, struct.pack('<I', arg))
        self.uc.reg_write(UC_ARM_REG_LR, STOP | 1)
        self.uc.emu_start((BASE + offset) | 1, STOP + 4, count=1000000)
        assert self.uc.reg_read(UC_ARM_REG_PC) == STOP
        return self.uc.reg_read(UC_ARM_REG_R0)

    def digest(self, value):
        context = self.bytes(b'\0' * 104)
        source = self.bytes(value)
        self.call(0xedc, context)
        self.call(0x1128, context, source, len(value))
        assert self.call(0x123c, context) == 1
        words = struct.unpack('<5I', self.uc.mem_read(context, 20))
        return struct.pack('>5I', *words)

    def passphrase(self, first, second):
        slot = self.bytes(b'\0' * 4)
        length = self.call(0xb48, self.bytes(first), self.bytes(second), slot)
        pointer = struct.unpack('<I', self.uc.mem_read(slot, 4))[0]
        return bytes(self.uc.mem_read(pointer, length))

    def check_license(self, contents, first, second):
        self.file_bytes = contents
        fields = [self.bytes(b'synthetic-license-file'), self.bytes(first), self.bytes(second)]
        self.uc.mem_write(BASE + 0x3888, struct.pack('<3I', *fields))
        return self.call(0xd20)

    def store_license(self, key, metadata):
        self.file_bytes = b''
        array = self.bytes(key)
        self.array_lengths[array] = len(key)
        result = self.call(0x9d0, self.env, 0, self.bytes(b'synthetic-license-file'), array, self.bytes(metadata))
        return result, self.file_bytes

    def jni_passphrase(self, first, second):
        pointer = self.call(0xc60, self.env, 0, self.bytes(first), self.bytes(second))
        return self.string(pointer)

    def jni_check_license(self, contents, first, second):
        self.file_bytes = contents
        return self.call(0xe2c, self.env, 0, self.bytes(b'synthetic-license-file'), self.bytes(first), self.bytes(second))

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('original', type=Path)
    parser.add_argument('host_library', type=Path)
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    data = args.original.read_bytes()
    original = Original(data)
    library = ctypes.CDLL(str(args.host_library.resolve()))
    library.dh2_sha1.argtypes = [ctypes.c_char_p, ctypes.c_size_t, ctypes.c_void_p]
    library.dh2_passphrase.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.POINTER(ctypes.c_void_p)]
    library.dh2_passphrase.restype = ctypes.c_size_t
    library.dh2_check_license_file.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p]
    library.dh2_store_license_key.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_size_t, ctypes.c_char_p]
    rng = random.Random(2011)
    digests = [b'', b'abc'] + [bytes(rng.randrange(256) for _ in range(n))
                for n in (1, 3, 31, 55, 56, 57, 63, 64, 65, 127, 128, 129, 1024)]
    for message in digests:
        recovered = (ctypes.c_uint8 * 20)()
        library.dh2_sha1(message, len(message), recovered)
        assert original.digest(message) == bytes(recovered) == hashlib.sha1(message).digest()
    pairs = [(b'', b''), (b'synthetic-test-identity', b'synthetic-test-product'),
             (b'1234567890123456789012345678901EXTRA', b'product'),
             (b'caf\xc3\xa9', b'\xe4\xb8\x96\xe7\x95\x8c')]
    pairs += [(bytes(rng.randrange(33, 127) for _ in range(rng.randrange(1, 100))),
               bytes(rng.randrange(33, 127) for _ in range(rng.randrange(1, 100))))
              for _ in range(96)]
    for first, second in pairs:
        slot = ctypes.c_void_p()
        length = library.dh2_passphrase(first, second, ctypes.byref(slot))
        recovered = ctypes.string_at(slot.value, length)
        assert original.passphrase(first, second) == recovered
        if (first, second) in pairs[:4]:
            assert original.jni_passphrase(first, second) == recovered
    license_cases = 0
    import tempfile
    with tempfile.TemporaryDirectory() as directory:
        path = Path(directory) / 'synthetic-license'
        for first, second in pairs[:12]:
            digest = hashlib.sha1(original.passphrase(first, second) + first + second).digest()
            for contents, expected in [(None, 0), (b'', 0), (digest[:19], 0),
                                       (digest, 1), (digest + b'ignored metadata', 1),
                                       (bytes([digest[0] ^ 1]) + digest[1:], 0)]:
                if contents is None:
                    path.unlink(missing_ok=True)
                else: path.write_bytes(contents)
                actual = original.check_license(contents, first, second)
                recovered = library.dh2_check_license_file(str(path).encode(), first, second)
                assert actual == recovered == expected
                assert original.jni_check_license(contents, first, second) == expected
                license_cases += 1
        store_cases = 0
        for metadata in (b'', b'abc', b'abcd', b'abcde', b'metadata', b'long-synthetic-file-path',
                         b'caf\xc3\xa9', b'\xe4\xb8\x96\xe7\x95\x8c'):
            key = bytes(range(20))  # Synthetic test bytes; no real entitlement
            assert len(key) == 20
            expected = key + hashlib.sha1(metadata[:(len(metadata)+3)//4]).digest()
            result, stored = original.store_license(key, metadata)
            assert result == 1 and stored == expected
            assert library.dh2_store_license_key(str(path).encode(), key, len(key), metadata) == 1
            assert path.read_bytes() == expected
            store_cases += 1
    # Independent original symbol check for the intentionally unconditional JNI method.
    assert data[0x920:0x924] == bytes.fromhex('01207047')
    assert original.call(0x920) == 1
    report = {'original_sha256': hashlib.sha256(data).hexdigest(),
              'original_arm_instructions_executed': True,
              'sha1_cases': len(digests), 'passphrase_cases': len(pairs),
              'license_file_cases': license_cases,
              'original_jni_passphrase_cases': 4,
              'original_jni_check_license_cases': license_cases,
              'original_jni_store_license_padded_buffer_cases': store_cases,
              'original_checkLicenseFile2_returns_true': True,
              'result': 'pass', 'limits': ['modeled libc and JNI dispatch rather than original Android runtime',
                  'original store metadata pointer deliberately zero-padded; unsafe allocation-slack behavior excluded',
                  'device runtime not exercised by this emulator']}
    if args.report: args.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report, indent=2))

if __name__ == '__main__':
    main()
