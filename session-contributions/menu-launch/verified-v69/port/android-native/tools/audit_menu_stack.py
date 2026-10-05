"""Preserve source evidence for the unconnected native menu stack.

Reads original ELF32 ARM load segments and checked PC-relative literals.
This is an evidence extractor, not a navigation implementation or parity test.
"""
import argparse
import hashlib
import json
import struct
import subprocess
from pathlib import Path

def main():
    p=argparse.ArgumentParser()
    p.add_argument('--elf',type=Path,required=True)
    p.add_argument('--objdump',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();a.output.mkdir(parents=True,exist_ok=True)
    data=a.elf.read_bytes()
    assert data[:7]==b'\x7fELF\x01\x01\x01'
    assert struct.unpack_from('<H',data,18)[0]==40
    phoff=struct.unpack_from('<I',data,28)[0]
    entry,count=struct.unpack_from('<HH',data,42)
    segments=[]
    for i in range(count):
        kind,offset,va,_,size,_,_,_=struct.unpack_from('<8I',data,phoff+i*entry)
        if kind==1:segments.append((va,offset,size))
    def read(va,size):
        for base,offset,length in segments:
            if base<=va and va+size<=base+length:return data[offset+va-base:offset+va-base+size]
        raise ValueError('Unmapped ELF address '+hex(va))
    def word(va):return struct.unpack('<I',read(va,4))[0]
    refs=[]
    for label,pc,literal in (
        ('previous_state_callback',0x438384,0x438b70),
        ('pushed_state_callback',0x43847c,0x438b74),
        ('pushed_state_animation',0x438604,0x438b9c),
        ('postload_state_name_filter',0x42efec,0x42f2f0),
        ('manager_push_debug_switch',0x4318cc,0x431920),
        ('push_special_name_0',0x4382c0,0x438b58),
        ('push_special_name_1',0x4382d8,0x438b5c),
        ('push_special_name_2',0x4382f0,0x438b60),
        ('push_special_name_3',0x438308,0x438b64),
        ('push_special_name_4',0x4384d8,0x438b78),
        ('push_special_name_5',0x4384f0,0x438b7c),
        ('option_maximum_member',0x44a384,0x44a5a8),
        ('option_current_member',0x44a3fc,0x44a5ac),
        ('option_string_member',0x44a478,0x44a5b0),
        ('option_language_key',0x44a4f0,0x44a5b4),
        ('option_empty_string',0x44a54c,0x44a5c0),
    ):
        address=(pc+8+word(literal))&0xffffffff
        value=read(address,100).split(b'\0')[0].decode('ascii')
        refs.append(dict(label=label,pc=hex(pc),literal=hex(literal),address=hex(address),value=value))
    # mov r1,#132 immediately before SetInputBehavior in LoadMainMenu.
    assert word(0x4325ac)==0xe3a01084
    assert word(0x431f10)==0xe3a01084
    for name,start,end in (
        ('original-multi-menu-push',0x438278,0x438c14),
        ('original-load-main-menu',0x4324dc,0x432984),
        ('original-multi-load-swf',0x437d68,0x437e24),
        ('original-set-input-behavior',0x7a7c98,0x7a7ca0),
        ('original-menu-postload',0x42efb8,0x42f304),
        ('original-register-menu-state',0x7adf50,0x7adff0),
        ('original-get-option-parameters',0x44a298,0x44a5c4),
        ('original-option-string-and-max',0x46d2b8,0x46d378),
        ('original-renderfx-load',0x7ab784,0x7ab924),
        ('original-load-menu',0x431ea4,0x4324dc),
        ('original-menu-manager-update',0x42ea04,0x42ee94),
        ('original-native-pop-and-push',0x43b158,0x43b1e4),
        ('original-renderfx-invoke-character',0x7abe0c,0x7abf34),
        ('original-renderfx-invoke-name',0x7ad7e8,0x7ad81c),
        ('original-menu-manager-push',0x4317e8,0x431948),
        ('original-menu-manager-get-by-name',0x42d1f0,0x42d234),
        ('original-menu-manager-pop-named',0x42e2b0,0x42e2d4),
        ('original-menu-base-show',0x425450,0x425728),
    ):
        asm=subprocess.check_output([str(a.objdump),'-d','--demangle',
             '--start-address='+hex(start),'--stop-address='+hex(end),str(a.elf)],text=True)
        (a.output/(name+'.asm')).write_text(asm,encoding='utf-8')
    report=dict(status='SOURCE EVIDENCE; navigation not implemented',
        elf_sha256=hashlib.sha256(data).hexdigest(),
        main_input_flags=0x84,main_input_flags_pc='0x4325ac',
        loaded_menu_input_flags=0x84,loaded_menu_input_flags_pc='0x431f10',
        renderer_update_order='MenuManager.Update iterates loaded slots 0..3; source dt in ms, advance flag false',
        main_renderer_slot=2,renderer_slot_evidence='LoadSWFFile stores at this+0x134+slot*4; LoadMainMenu reads 0x13c',
        callback_references=refs)
    (a.output/'menu-stack-source.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))

if __name__=='__main__':main()
