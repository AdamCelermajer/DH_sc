
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040b7cc <LightPoint::GetIdsFromName(char const*, int&, int&, char&)>:
  40b7cc: e92d4070     	push	{r4, r5, r6, lr}
  40b7d0: e1a04000     	mov	r4, r0
  40b7d4: e1a00001     	mov	r0, r1
  40b7d8: e1a05002     	mov	r5, r2
  40b7dc: e1a06003     	mov	r6, r3
  40b7e0: ebfc099b     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0xfd994
  40b7e4: e5942044     	ldr	r2, [r4, #0x44]
  40b7e8: e19230d0     	ldrsb	r3, [r2, r0]
  40b7ec: e3a00001     	mov	r0, #1
  40b7f0: e3530053     	cmp	r3, #83
  40b7f4: 03a03004     	moveq	r3, #4
  40b7f8: 05863000     	streq	r3, [r6]
  40b7fc: 12433030     	subne	r3, r3, #48
  40b800: 03a03000     	moveq	r3, #0
  40b804: 15863000     	strne	r3, [r6]
  40b808: 05853000     	streq	r3, [r5]
  40b80c: 15853000     	strne	r3, [r5]
  40b810: 12832030     	addne	r2, r3, #48
  40b814: e59d3010     	ldr	r3, [sp, #0x10]
  40b818: 03a02030     	moveq	r2, #48
  40b81c: 120220ff     	andne	r2, r2, #255
  40b820: e5c32000     	strb	r2, [r3]
  40b824: e8bd8070     	pop	{r4, r5, r6, pc}
