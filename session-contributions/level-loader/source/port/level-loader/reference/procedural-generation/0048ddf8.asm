
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048ddf8 <_ZN3rnd4PathC1ERNS_8RootRuleEPNS_4RuleE>:
  48ddf8: e92d4070     	push	{r4, r5, r6, lr}
  48ddfc: e59f5038     	ldr	r5, [pc, #0x38]         @ 0x48de3c <_ZN3rnd4PathC1ERNS_8RootRuleEPNS_4RuleE+0x44>
  48de00: e1a04000     	mov	r4, r0
  48de04: ebffffa1     	bl	0x48dc90 <_ZN3rnd4RuleC2ERNS_8RootRuleEPS0_> @ imm = #-0x17c
  48de08: e59f3030     	ldr	r3, [pc, #0x30]         @ 0x48de40 <_ZN3rnd4PathC1ERNS_8RootRuleEPNS_4RuleE+0x48>
  48de0c: e08f5005     	add	r5, pc, r5
  48de10: e1a00004     	mov	r0, r4
  48de14: e7953003     	ldr	r3, [r5, r3]
  48de18: e2833008     	add	r3, r3, #8
  48de1c: e5843000     	str	r3, [r4]
  48de20: e3a03001     	mov	r3, #1
  48de24: e5c4308c     	strb	r3, [r4, #0x8c]
  48de28: e3a03000     	mov	r3, #0
  48de2c: e5c4308d     	strb	r3, [r4, #0x8d]
  48de30: e3a03004     	mov	r3, #4
  48de34: e5843088     	str	r3, [r4, #0x88]
  48de38: e8bd8070     	pop	{r4, r5, r6, pc}
  48de3c: 84 6c 50 00  	.word	0x00506c84
  48de40: 34 2c 00 00  	.word	0x00002c34
