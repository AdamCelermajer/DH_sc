
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048e274 <rnd::Rule::GetBlock(char const*) const>:
  48e274: e5903004     	ldr	r3, [r0, #0x4]
  48e278: e593008c     	ldr	r0, [r3, #0x8c]
  48e27c: eaffffc0     	b	0x48e184 <rnd::RandomGenerator::GetBlock(char const*) const> @ imm = #-0x100
