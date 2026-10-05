"""Recover menu sound IDs/files from original serialized sound arrays.

No filenames are guessed. The complete Arrays::Sounds name order is retained
so the native wrapper distinguishes a genuine name miss from a missing backend.
"""
import argparse
import hashlib
import json
import struct
import zipfile
from pathlib import Path


class Reader:
    def __init__(self, data): self.data, self.pos = data, 0
    def u32(self):
        value = struct.unpack_from('<I', self.data, self.pos)[0]
        self.pos += 4
        return value
    def string(self):
        size = self.u32()
        value = self.data[self.pos:self.pos+size].decode('utf-8').rstrip('\0')
        self.pos += size
        return value


def main():
    p = argparse.ArgumentParser()
    p.add_argument('--cache', type=Path, required=True)
    p.add_argument('--android', type=Path, required=True)
    p.add_argument('--receipt', type=Path, required=True)
    a = p.parse_args()
    prefix = 'com.gameloft.android.GAND.GloftD2SS/files/'
    with zipfile.ZipFile(a.cache) as z:
        names = z.read(prefix+'data/pydata/sounds_pyarraynames.bin')
        records = z.read(prefix+'data/pydata/sounds_pyarray.bin')
        nr = Reader(names)
        groups = []
        for _ in range(5): groups.append([nr.string() for _ in range(nr.u32())])
        assert nr.pos == len(names)
        r = Reader(records)
        # CharSounds: four variable sound-ID lists, then two byte booleans.
        assert r.u32() == len(groups[0])
        for _ in groups[0]:
            for _ in range(4):
                size = r.u32()
                r.pos += size*4
            r.pos += 2
        # Listeners: six words (Anchor, MaxDistance, Orientation, RefDistance,
        # RolloffFactor, UpVector); Groups: two words; Types: string + enum.
        assert r.u32() == len(groups[1])
        r.pos += len(groups[1])*24
        assert r.u32() == len(groups[2])
        r.pos += len(groups[2])*8
        assert r.u32() == len(groups[3])
        for _ in groups[3]: r.string(); r.u32()
        assert r.u32() == len(groups[4])
        sounds = []
        for index, name in enumerate(groups[4]):
            bank, channel, filename = r.u32(), r.u32(), r.string()
            fmt, group, load, priority, repeat, volume = [r.u32() for _ in range(6)]
            sounds.append(dict(id=index, name=name, file=filename, volume=volume,
                               channel=channel, format=fmt, repeat=repeat))
        assert r.pos == len(records)
        menu = [s for s in sounds if s['file'].startswith('sfx_menu_')]
        assert menu and all(s['volume']==100 and s['format']==0 and s['repeat']==0 for s in menu)
        # Only WAV assets genuinely present in this cache become a backend.
        delivered = set()
        for s in menu:
            uri = prefix+'data/sounds/'+s['file']
            if uri not in z.namelist(): continue
            data = z.read(uri)
            assert data[:4] == b'RIFF' and data[8:12] == b'WAVE'
            (a.android/'app/src/main/assets/original-media'/s['file']).write_bytes(data)
            delivered.add(s['file'])
        header = ['// Generated from original cache by recover_menu_sounds.py.\n',
                  '#pragma once\nnamespace dh2::android_ui {\n',
                  'struct OriginalSoundRecord { const char* name; const char* file; bool menu_backend; };\n',
                  'inline constexpr OriginalSoundRecord original_sounds[] = {\n']
        for s in sounds:
            header.append('  {%s,%s,%s},\n' % (json.dumps(s['name']),json.dumps(s['file']),
                          'true' if s['file'] in delivered else 'false'))
        header.append('};\n}\n')
        (a.android/'app/src/main/cpp/original_menu_sound_data.hpp').write_text(''.join(header),encoding='utf-8')
        a.receipt.write_text(json.dumps(dict(names_sha256=hashlib.sha256(names).hexdigest(),
            records_sha256=hashlib.sha256(records).hexdigest(), records_consumed=r.pos,
            sound_count=len(sounds), menu=menu, delivered_files=sorted(delivered)),indent=2)+'\n')


if __name__ == '__main__': main()
