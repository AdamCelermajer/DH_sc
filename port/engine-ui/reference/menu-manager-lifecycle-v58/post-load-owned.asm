
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0042efb8 <_ZN11MenuManager8PostLoadEv>:
  42f270: e1d730d0     	ldrsb	r3, [r7]
  42f274: e2855001     	add	r5, r5, #1
  42f278: e3730001     	cmn	r3, #1
  42f27c: 12877001     	addne	r7, r7, #1
  42f280: 0597700c     	ldreq	r7, [r7, #0xc]
  42f284: ebfb84b9     	bl	0x310570 <_Znwj15MemoryHintState> @ imm = #-0x11ed1c
  42f288: e1a01007     	mov	r1, r7
  42f28c: e1a04000     	mov	r4, r0
  42f290: ebffddd1     	bl	0x4269dc <_ZN8MenuBaseC1EPKc> @ imm = #-0x88bc
  42f294: e3a02001     	mov	r2, #1
  42f298: e5c4207d     	strb	r2, [r4, #0x7d]
  42f29c: ebfff5fa     	bl	0x42ca8c <_ZN11MenuManager11GetInstanceEv> @ imm = #-0x2818
  42f2a0: e1a01004     	mov	r1, r4
  42f2a4: ebfffefa     	bl	0x42ee94 <_ZN11MenuManager12RegisterMenuEP8MenuBase> @ imm = #-0x418
  42f2a8: e59d3038     	ldr	r3, [sp, #0x38]
  42f2ac: e1550003     	cmp	r5, r3
