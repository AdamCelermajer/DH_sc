
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00421fe0 <_ZN8MenuBase6UpdateEv>:
  421fe0: e92d4070     	push	{r4, r5, r6, lr}
  421fe4: e590104c     	ldr	r1, [r0, #0x4c]
  421fe8: e1a04000     	mov	r4, r0
  421fec: e5905004     	ldr	r5, [r0, #0x4]
  421ff0: e3510000     	cmp	r1, #0
  421ff4: 0a000003     	beq	0x422008 <_ZN8MenuBase6UpdateEv+0x28> @ imm = #0xc
  421ff8: e5900048     	ldr	r0, [r0, #0x48]
  421ffc: e5d03004     	ldrb	r3, [r0, #0x4]
  422000: e3530000     	cmp	r3, #0
  422004: 0a000006     	beq	0x422024 <_ZN8MenuBase6UpdateEv+0x44> @ imm = #0x18
  422008: e1a00005     	mov	r0, r5
  42200c: eb0e1770     	bl	0x7a7dd4 <_ZN8RenderFX10IsAnimOverEPN7gameswf9characterE> @ imm = #0x385dc0
  422010: e3500000     	cmp	r0, #0
  422014: 15943078     	ldrne	r3, [r4, #0x78]
  422018: 12833001     	addne	r3, r3, #1
  42201c: 15843078     	strne	r3, [r4, #0x78]
  422020: e8bd8070     	pop	{r4, r5, r6, pc}
  422024: e5901000     	ldr	r1, [r0]
  422028: e2411001     	sub	r1, r1, #1
  42202c: e3510000     	cmp	r1, #0
  422030: e5801000     	str	r1, [r0]
  422034: 1a000000     	bne	0x42203c <_ZN8MenuBase6UpdateEv+0x5c> @ imm = #0x0
  422038: eb0cc2be     	bl	0x752b38 <_ZN7gameswf13free_internalEPvj> @ imm = #0x330af8
  42203c: e3a01000     	mov	r1, #0
  422040: e5841048     	str	r1, [r4, #0x48]
  422044: e584104c     	str	r1, [r4, #0x4c]
  422048: eaffffee     	b	0x422008 <_ZN8MenuBase6UpdateEv+0x28> @ imm = #-0x48
