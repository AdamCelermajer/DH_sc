import pathlib,sys
sys.path.insert(0,r'C:\Users\adamc\.codex\worktrees\generic-level-loader\dependencies')
from elftools.elf.elffile import ELFFile
with open(r'C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so','rb') as f:
    elf=ELFFile(f)
    for table in ('.symtab','.dynsym'):
        section=elf.get_section_by_name(table)
        if not section:continue
        for s in section.iter_symbols():
            if s['st_size'] and s['st_info']['type']=='STT_FUNC' and any(x in s.name for x in ('PropertyMap','PropertyTemplate','PropertyManager','ObjectManager')):
                print(hex(s['st_value']),s['st_size'],s.name)
