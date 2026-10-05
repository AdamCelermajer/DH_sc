
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048aa84 <rnd::MgxBlock::LoadFromXmlStream(IFileStream*)>:
  48aa84: e92d4070     	push	{r4, r5, r6, lr}
  48aa88: e1a04000     	mov	r4, r0
  48aa8c: e24dd008     	sub	sp, sp, #8
  48aa90: e5913000     	ldr	r3, [r1]
  48aa94: e1a00001     	mov	r0, r1
  48aa98: e1a05001     	mov	r5, r1
  48aa9c: e1a0e00f     	mov	lr, pc
  48aaa0: e593f008     	ldr	pc, [r3, #0x8]
  48aaa4: e1a06000     	mov	r6, r0
  48aaa8: ebfa0f11     	bl	0x30e6f4 <.plt+0x980>   @ imm = #-0x17c3bc
  48aaac: e58409c0     	str	r0, [r4, #0x9c0]
  48aab0: e1a02006     	mov	r2, r6
  48aab4: e1a03fc2     	asr	r3, r2, #31
  48aab8: e595c000     	ldr	r12, [r5]
  48aabc: e1a01000     	mov	r1, r0
  48aac0: e1a00005     	mov	r0, r5
  48aac4: e1a0e00f     	mov	lr, pc
  48aac8: e59cf018     	ldr	pc, [r12, #0x18]
  48aacc: e3a01000     	mov	r1, #0
  48aad0: e3a00070     	mov	r0, #112
  48aad4: ebfa16a5     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x17a56c
  48aad8: e1a05000     	mov	r5, r0
  48aadc: eb0230f8     	bl	0x516ec4 <TiXmlDocument::TiXmlDocument()> @ imm = #0x8c3e0
  48aae0: e1a02006     	mov	r2, r6
  48aae4: e58459c4     	str	r5, [r4, #0x9c4]
  48aae8: e59419c0     	ldr	r1, [r4, #0x9c0]
  48aaec: e1a00005     	mov	r0, r5
  48aaf0: e3a03000     	mov	r3, #0
  48aaf4: eb022e6b     	bl	0x5164a8 <TiXmlDocument::LoadFromBuffer(void const*, int, TiXmlEncoding)> @ imm = #0x8b9ac
  48aaf8: e59f2030     	ldr	r2, [pc, #0x30]         @ 0x48ab30 <rnd::MgxBlock::LoadFromXmlStream(IFileStream*)+0xac>
  48aafc: e594c9c4     	ldr	r12, [r4, #0x9c4]
  48ab00: e1a0000d     	mov	r0, sp
  48ab04: e28d1004     	add	r1, sp, #4
  48ab08: e08f2002     	add	r2, pc, r2
  48ab0c: e3a03000     	mov	r3, #0
  48ab10: e58dc004     	str	r12, [sp, #0x4]
  48ab14: eb0228af     	bl	0x514dd8 <TiXmlHandle::Child(char const*, int) const> @ imm = #0x8a2bc
  48ab18: e1a00004     	mov	r0, r4
  48ab1c: e1a0100d     	mov	r1, sp
  48ab20: e1a0500d     	mov	r5, sp
  48ab24: ebffff78     	bl	0x48a90c <rnd::MgxBlock::LoadFromXml(TiXmlHandle&)> @ imm = #-0x220
  48ab28: e28dd008     	add	sp, sp, #8
  48ab2c: e8bd8070     	pop	{r4, r5, r6, pc}
  48ab30: 28 59 43 00  	.word	0x00435928
