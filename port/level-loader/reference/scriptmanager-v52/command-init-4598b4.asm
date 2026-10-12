
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004598b4 <Script_PlayEffect::Init()>:
  4598b4: e590200c     	ldr	r2, [r0, #0xc]
  4598b8: e59f3010     	ldr	r3, [pc, #0x10]         @ 0x4598d0 <Script_PlayEffect::Init()+0x1c>
  4598bc: e5921008     	ldr	r1, [r2, #0x8]
  4598c0: e59f200c     	ldr	r2, [pc, #0xc]          @ 0x4598d4 <Script_PlayEffect::Init()+0x20>
  4598c4: e08f3003     	add	r3, pc, r3
  4598c8: e7930002     	ldr	r0, [r3, r2]
  4598cc: ea00f3c5     	b	0x4967e8 <VisualFXManager::RegisterFXSetToLoad(int)> @ imm = #0x3cf14
  4598d0: cc b1 53 00  	.word	0x0053b1cc
  4598d4: 08 1b 00 00  	.word	0x00001b08
