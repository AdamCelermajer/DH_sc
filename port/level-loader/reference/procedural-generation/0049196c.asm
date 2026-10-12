
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0049196c <_ZN3rnd4Tile7NewRootERNS_15RandomGeneratorERNS_5BlockERNS_8ListElemE>:
  49196c: e92d40f0     	push	{r4, r5, r6, r7, lr}
  491970: e1a07000     	mov	r7, r0
  491974: e24dd00c     	sub	sp, sp, #12
  491978: e1a06001     	mov	r6, r1
  49197c: e3a00090     	mov	r0, #144
  491980: e3a01000     	mov	r1, #0
  491984: e1a05002     	mov	r5, r2
  491988: ebf9faf8     	bl	0x310570 <_Znwj15MemoryHintState> @ imm = #-0x181420
  49198c: e1a01007     	mov	r1, r7
  491990: e1a04000     	mov	r4, r0
  491994: e1a02006     	mov	r2, r6
  491998: e3a03000     	mov	r3, #0
  49199c: e58d5000     	str	r5, [sp]
  4919a0: ebffff94     	bl	0x4917f8 <_ZN3rnd4TileC1ERNS_15RandomGeneratorERNS_5BlockEPS0_RNS_8ListElemE> @ imm = #-0x1b0
  4919a4: e1a00004     	mov	r0, r4
  4919a8: e28dd00c     	add	sp, sp, #12
  4919ac: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
