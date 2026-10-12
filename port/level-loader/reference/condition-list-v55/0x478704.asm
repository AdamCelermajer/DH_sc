
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00478704 <ConditionList::Eval()>:
  478704: e92d4070     	push	{r4, r5, r6, lr}
  478708: e5903000     	ldr	r3, [r0]
  47870c: e1a05000     	mov	r5, r0
  478710: e3530000     	cmp	r3, #0
  478714: da00000e     	ble	0x478754 <ConditionList::Eval()+0x50> @ imm = #0x38
  478718: e3a04000     	mov	r4, #0
  47871c: ea000002     	b	0x47872c <ConditionList::Eval()+0x28> @ imm = #0x8
  478720: e5953000     	ldr	r3, [r5]
  478724: e1530004     	cmp	r3, r4
  478728: da000009     	ble	0x478754 <ConditionList::Eval()+0x50> @ imm = #0x24
  47872c: e5953004     	ldr	r3, [r5, #0x4]
  478730: e7933104     	ldr	r3, [r3, r4, lsl #2]
  478734: e2844001     	add	r4, r4, #1
  478738: e1a00003     	mov	r0, r3
  47873c: e5933000     	ldr	r3, [r3]
  478740: e1a0e00f     	mov	lr, pc
  478744: e593f008     	ldr	pc, [r3, #0x8]
  478748: e3500000     	cmp	r0, #0
  47874c: 1afffff3     	bne	0x478720 <ConditionList::Eval()+0x1c> @ imm = #-0x34
  478750: e8bd8070     	pop	{r4, r5, r6, pc}
  478754: e3a00001     	mov	r0, #1
  478758: e8bd8070     	pop	{r4, r5, r6, pc}
