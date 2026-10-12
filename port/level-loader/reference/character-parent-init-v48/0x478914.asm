
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00478914 <ConditionList::AssignPyData(Structs::v2ConditionStub*, int)>:
  478914: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  478918: e3520000     	cmp	r2, #0
  47891c: e1a04000     	mov	r4, r0
  478920: e5841008     	str	r1, [r4, #0x8]
  478924: e1a05001     	mov	r5, r1
  478928: e5802000     	str	r2, [r0]
  47892c: da000018     	ble	0x478994 <ConditionList::AssignPyData(Structs::v2ConditionStub*, int)+0x80> @ imm = #0x60
  478930: e1a00102     	lsl	r0, r2, #2
  478934: e3a01000     	mov	r1, #0
  478938: ebfa5f0b     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0x1683d4
  47893c: e5943000     	ldr	r3, [r4]
  478940: e1a07000     	mov	r7, r0
  478944: e5840004     	str	r0, [r4, #0x4]
  478948: e3530000     	cmp	r3, #0
  47894c: da000010     	ble	0x478994 <ConditionList::AssignPyData(Structs::v2ConditionStub*, int)+0x80> @ imm = #0x40
  478950: e59f8040     	ldr	r8, [pc, #0x40]         @ 0x478998 <ConditionList::AssignPyData(Structs::v2ConditionStub*, int)+0x84>
  478954: e3a06000     	mov	r6, #0
  478958: e08f8008     	add	r8, pc, r8
  47895c: ea000000     	b	0x478964 <ConditionList::AssignPyData(Structs::v2ConditionStub*, int)+0x50> @ imm = #0x0
  478960: e5947004     	ldr	r7, [r4, #0x4]
  478964: e5953004     	ldr	r3, [r5, #0x4]
  478968: e1a0e00f     	mov	lr, pc
  47896c: e798f103     	ldr	pc, [r8, r3, lsl #2]
  478970: e7870106     	str	r0, [r7, r6, lsl #2]
  478974: e5943004     	ldr	r3, [r4, #0x4]
  478978: e7933106     	ldr	r3, [r3, r6, lsl #2]
  47897c: e2866001     	add	r6, r6, #1
  478980: e5835004     	str	r5, [r3, #0x4]
  478984: e5943000     	ldr	r3, [r4]
  478988: e2855010     	add	r5, r5, #16
  47898c: e1530006     	cmp	r3, r6
  478990: cafffff2     	bgt	0x478960 <ConditionList::AssignPyData(Structs::v2ConditionStub*, int)+0x4c> @ imm = #-0x38
  478994: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  478998: f8 08 4f 00  	.word	0x004f08f8
