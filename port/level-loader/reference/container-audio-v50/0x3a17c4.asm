
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003a17c4 <_ZNK17OpenableContainer8GetSoundEv>:
  3a17c4: e92d4010     	push	{r4, lr}
  3a17c8: e590038c     	ldr	r0, [r0, #0x38c]
  3a17cc: ebffffcd     	bl	0x3a1708 <_ZN6Arrays19GetMemberIDByStringINS_18OpenableContainersEEEiPKc> @ imm = #-0xcc
  3a17d0: e59f4024     	ldr	r4, [pc, #0x24]         @ 0x3a17fc <_ZNK17OpenableContainer8GetSoundEv+0x38>
  3a17d4: e3700001     	cmn	r0, #1
  3a17d8: e08f4004     	add	r4, pc, r4
  3a17dc: 0a000005     	beq	0x3a17f8 <_ZNK17OpenableContainer8GetSoundEv+0x34> @ imm = #0x14
  3a17e0: e59f3018     	ldr	r3, [pc, #0x18]         @ 0x3a1800 <_ZNK17OpenableContainer8GetSoundEv+0x3c>
  3a17e4: e3a02028     	mov	r2, #40
  3a17e8: e7943003     	ldr	r3, [r4, r3]
  3a17ec: e5933000     	ldr	r3, [r3]
  3a17f0: e0203092     	mla	r0, r2, r0, r3
  3a17f4: e5900008     	ldr	r0, [r0, #0x8]
  3a17f8: e8bd8010     	pop	{r4, pc}
  3a17fc: b8 32 5f 00  	.word	0x005f32b8
  3a1800: dc 33 00 00  	.word	0x000033dc
