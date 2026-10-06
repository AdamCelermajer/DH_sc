
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00340f2c <ObjectBase* GetNewInstance<CheckpointZone>()>:
  340f2c: e92d4010     	push	{r4, lr}
  340f30: e3a01000     	mov	r1, #0
  340f34: e3a00e3a     	mov	r0, #928
  340f38: ebff3d8c     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x309d0
  340f3c: e3a0100c     	mov	r1, #12
  340f40: e1a04000     	mov	r4, r0
  340f44: eb015280     	bl	0x39594c <CheckpointZone::CheckpointZone(ObjectBase::GO_IDS)> @ imm = #0x54a00
  340f48: e1a00004     	mov	r0, r4
  340f4c: e8bd8010     	pop	{r4, pc}


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039594c <CheckpointZone::CheckpointZone(ObjectBase::GO_IDS)>:
  39594c: e3a02001     	mov	r2, #1
  395950: e92d4070     	push	{r4, r5, r6, lr}
  395954: e1a03002     	mov	r3, r2
  395958: e59f504c     	ldr	r5, [pc, #0x4c]         @ 0x3959ac <CheckpointZone::CheckpointZone(ObjectBase::GO_IDS)+0x60>
  39595c: e1a04000     	mov	r4, r0
  395960: eb0008ce     	bl	0x397ca0 <Zone::Zone(ObjectBase::GO_IDS, bool, bool)> @ imm = #0x2338
  395964: e59f3044     	ldr	r3, [pc, #0x44]         @ 0x3959b0 <CheckpointZone::CheckpointZone(ObjectBase::GO_IDS)+0x64>
  395968: e08f5005     	add	r5, pc, r5
  39596c: e3a01000     	mov	r1, #0
  395970: e7953003     	ldr	r3, [r5, r3]
  395974: e1a02004     	mov	r2, r4
  395978: e584138c     	str	r1, [r4, #0x38c]
  39597c: e28300f4     	add	r0, r3, #244
  395980: e283c008     	add	r12, r3, #8
  395984: e28330e8     	add	r3, r3, #232
  395988: e5840024     	str	r0, [r4, #0x24]
  39598c: e584c000     	str	r12, [r4]
  395990: e5843004     	str	r3, [r4, #0x4]
  395994: e5e21388     	strb	r1, [r2, #0x388]!
  395998: e5842394     	str	r2, [r4, #0x394]
  39599c: e5841398     	str	r1, [r4, #0x398]
  3959a0: e5842390     	str	r2, [r4, #0x390]
  3959a4: e1a00004     	mov	r0, r4
  3959a8: e8bd8070     	pop	{r4, r5, r6, pc}
  3959ac: 28 f1 5f 00  	.word	0x005ff128
  3959b0: 64 2e 00 00  	.word	0x00002e64


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003959b4 <CheckpointZone::CheckpointZone(ObjectBase::GO_IDS)>:
  3959b4: e3a02001     	mov	r2, #1
  3959b8: e92d4070     	push	{r4, r5, r6, lr}
  3959bc: e1a03002     	mov	r3, r2
  3959c0: e59f504c     	ldr	r5, [pc, #0x4c]         @ 0x395a14 <CheckpointZone::CheckpointZone(ObjectBase::GO_IDS)+0x60>
  3959c4: e1a04000     	mov	r4, r0
  3959c8: eb0008b4     	bl	0x397ca0 <Zone::Zone(ObjectBase::GO_IDS, bool, bool)> @ imm = #0x22d0
  3959cc: e59f3044     	ldr	r3, [pc, #0x44]         @ 0x395a18 <CheckpointZone::CheckpointZone(ObjectBase::GO_IDS)+0x64>
  3959d0: e08f5005     	add	r5, pc, r5
  3959d4: e3a01000     	mov	r1, #0
  3959d8: e7953003     	ldr	r3, [r5, r3]
  3959dc: e1a02004     	mov	r2, r4
  3959e0: e584138c     	str	r1, [r4, #0x38c]
  3959e4: e28300f4     	add	r0, r3, #244
  3959e8: e283c008     	add	r12, r3, #8
  3959ec: e28330e8     	add	r3, r3, #232
  3959f0: e5840024     	str	r0, [r4, #0x24]
  3959f4: e584c000     	str	r12, [r4]
  3959f8: e5843004     	str	r3, [r4, #0x4]
  3959fc: e5e21388     	strb	r1, [r2, #0x388]!
  395a00: e5842394     	str	r2, [r4, #0x394]
  395a04: e5841398     	str	r1, [r4, #0x398]
  395a08: e5842390     	str	r2, [r4, #0x390]
  395a0c: e1a00004     	mov	r0, r4
  395a10: e8bd8070     	pop	{r4, r5, r6, pc}
  395a14: c0 f0 5f 00  	.word	0x005ff0c0
  395a18: 64 2e 00 00  	.word	0x00002e64
