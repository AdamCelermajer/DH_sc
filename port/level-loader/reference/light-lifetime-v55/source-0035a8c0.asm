
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0035a8c0 <virtual thunk to SceneManager::~SceneManager()>:
  35a8c0: e5903000     	ldr	r3, [r0]
  35a8c4: e5133018     	ldr	r3, [r3, #-0x18]
  35a8c8: e0800003     	add	r0, r0, r3
  35a8cc: eafff0c6     	b	0x356bec <SceneManager::~SceneManager()> @ imm = #-0x3ce8
