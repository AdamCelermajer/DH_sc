
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0047875c <ConditionList::DBG_TraceDetailedConditionListInformation(__sFILE*) const>:
  47875c: e92d4070     	push	{r4, r5, r6, lr}
  478760: e5903000     	ldr	r3, [r0]
  478764: e1a05000     	mov	r5, r0
  478768: e1a06001     	mov	r6, r1
  47876c: e3530000     	cmp	r3, #0
  478770: da00000b     	ble	0x4787a4 <ConditionList::DBG_TraceDetailedConditionListInformation(__sFILE*) const+0x48> @ imm = #0x2c
  478774: e3a04000     	mov	r4, #0
  478778: e5953004     	ldr	r3, [r5, #0x4]
  47877c: e1a01006     	mov	r1, r6
  478780: e7933104     	ldr	r3, [r3, r4, lsl #2]
  478784: e2844001     	add	r4, r4, #1
  478788: e1a00003     	mov	r0, r3
  47878c: e5933000     	ldr	r3, [r3]
  478790: e1a0e00f     	mov	lr, pc
  478794: e593f010     	ldr	pc, [r3, #0x10]
  478798: e5953000     	ldr	r3, [r5]
  47879c: e1530004     	cmp	r3, r4
  4787a0: cafffff4     	bgt	0x478778 <ConditionList::DBG_TraceDetailedConditionListInformation(__sFILE*) const+0x1c> @ imm = #-0x30
  4787a4: e8bd8070     	pop	{r4, r5, r6, pc}
