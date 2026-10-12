
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0033e7c8 <ConditionData::Clear()>:
  33e7c8: e92d4070     	push	{r4, r5, r6, lr}
  33e7cc: e590401c     	ldr	r4, [r0, #0x1c]
  33e7d0: e1a05000     	mov	r5, r0
  33e7d4: e3540000     	cmp	r4, #0
  33e7d8: 0a000005     	beq	0x33e7f4 <ConditionData::Clear()+0x2c> @ imm = #0x14
  33e7dc: e1a00004     	mov	r0, r4
  33e7e0: eb04e9b1     	bl	0x478eac <ConditionList::~ConditionList()> @ imm = #0x13a6c4
  33e7e4: e1a00004     	mov	r0, r4
  33e7e8: ebff4714     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x2e3b0
  33e7ec: e3a03000     	mov	r3, #0
  33e7f0: e585301c     	str	r3, [r5, #0x1c]
  33e7f4: e8bd8070     	pop	{r4, r5, r6, pc}
