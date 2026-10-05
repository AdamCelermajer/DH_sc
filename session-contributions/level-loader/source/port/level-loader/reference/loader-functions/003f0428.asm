
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f0428 <Level::LoadCheckpoint()>:
  3f0428: e92d4070     	push	{r4, r5, r6, lr}
  3f042c: e1a04000     	mov	r4, r0
  3f0430: e59000ec     	ldr	r0, [r0, #0xec]
  3f0434: e59f5070     	ldr	r5, [pc, #0x70]         @ 0x3f04ac <Level::LoadCheckpoint()+0x84>
  3f0438: e3a03001     	mov	r3, #1
  3f043c: e3500000     	cmp	r0, #0
  3f0440: e5c430f4     	strb	r3, [r4, #0xf4]
  3f0444: e08f5005     	add	r5, pc, r5
  3f0448: 0a000005     	beq	0x3f0464 <Level::LoadCheckpoint()+0x3c> @ imm = #0x14
  3f044c: e5941114     	ldr	r1, [r4, #0x114]
  3f0450: e5942040     	ldr	r2, [r4, #0x40]
  3f0454: e594303c     	ldr	r3, [r4, #0x3c]
  3f0458: eb01c84f     	bl	0x46259c <LevelSavegame::LoadCheckPoint(int, int, int)> @ imm = #0x7213c
  3f045c: e5940194     	ldr	r0, [r4, #0x194]
  3f0460: eb022531     	bl	0x47992c <GameEventManager::ReInit()> @ imm = #0x894c4
  3f0464: e59f3044     	ldr	r3, [pc, #0x44]         @ 0x3f04b0 <Level::LoadCheckpoint()+0x88>
  3f0468: e3a01000     	mov	r1, #0
  3f046c: e3a02001     	mov	r2, #1
  3f0470: e7953003     	ldr	r3, [r5, r3]
  3f0474: e5930040     	ldr	r0, [r3, #0x40]
  3f0478: ebfdf7fe     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x82008
  3f047c: e5905660     	ldr	r5, [r0, #0x660]
  3f0480: e3550000     	cmp	r5, #0
  3f0484: 0a000005     	beq	0x3f04a0 <Level::LoadCheckpoint()+0x78> @ imm = #0x14
  3f0488: e3a01010     	mov	r1, #16
  3f048c: e1a00005     	mov	r0, r5
  3f0490: ebff3009     	bl	0x3bc4bc <Character::SG_LoadCheckpoint(int)> @ imm = #-0x33fdc
  3f0494: e1a00005     	mov	r0, r5
  3f0498: e3a01001     	mov	r1, #1
  3f049c: ebff2ff7     	bl	0x3bc480 <Character::SG_Update(bool)> @ imm = #-0x34024
  3f04a0: e3a03000     	mov	r3, #0
  3f04a4: e5c430f4     	strb	r3, [r4, #0xf4]
  3f04a8: e8bd8070     	pop	{r4, r5, r6, pc}
  3f04ac: 4c 46 5a 00  	.word	0x005a464c
  3f04b0: f4 37 00 00  	.word	0x000037f4
