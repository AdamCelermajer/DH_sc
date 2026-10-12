
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003eac00 <ItemManager::ItemManager()>:
  3eac00: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x3eac2c <ItemManager::ItemManager()+0x2c>
  3eac04: e59fc024     	ldr	r12, [pc, #0x24]        @ 0x3eac30 <ItemManager::ItemManager()+0x30>
  3eac08: e3a01000     	mov	r1, #0
  3eac0c: e08f2002     	add	r2, pc, r2
  3eac10: e792c00c     	ldr	r12, [r2, r12]
  3eac14: e580100c     	str	r1, [r0, #0xc]
  3eac18: e5801004     	str	r1, [r0, #0x4]
  3eac1c: e28cc008     	add	r12, r12, #8
  3eac20: e580c000     	str	r12, [r0]
  3eac24: e5801008     	str	r1, [r0, #0x8]
  3eac28: e12fff1e     	bx	lr
  3eac2c: 84 9e 5a 00  	.word	0x005a9e84
  3eac30: ec 08 00 00  	.word	0x000008ec
