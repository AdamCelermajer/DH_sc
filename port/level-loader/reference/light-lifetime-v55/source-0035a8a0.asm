
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0035a8a0 <virtual thunk to SceneManager::~SceneManager()>:
  35a8a0: e5903000     	ldr	r3, [r0]
  35a8a4: e5133018     	ldr	r3, [r3, #-0x18]
  35a8a8: e0800003     	add	r0, r0, r3
  35a8ac: eafff0fa     	b	0x356c9c <SceneManager::~SceneManager()> @ imm = #-0x3c18
