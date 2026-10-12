
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0035a8d0 <virtual thunk to SceneManager::~SceneManager()>:
  35a8d0: e5903000     	ldr	r3, [r0]
  35a8d4: e513300c     	ldr	r3, [r3, #-0xc]
  35a8d8: e0800003     	add	r0, r0, r3
  35a8dc: eafff0c2     	b	0x356bec <SceneManager::~SceneManager()> @ imm = #-0x3cf8
