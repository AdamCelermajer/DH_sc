
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00318fe4 <UserProperties::_ParseLine(char*)>:
  318fe4: e92d4070     	push	{r4, r5, r6, lr}
  318fe8: e1a05001     	mov	r5, r1
  318fec: e1a06000     	mov	r6, r0
  318ff0: e3a0103d     	mov	r1, #61
  318ff4: e1a00005     	mov	r0, r5
  318ff8: ebffd70a     	bl	0x30ec28 <.plt+0xeb4>   @ imm = #-0xa3d8
  318ffc: e2504000     	subs	r4, r0, #0
  319000: 0a000008     	beq	0x319028 <UserProperties::_ParseLine(char*)+0x44> @ imm = #0x20
  319004: e3a03000     	mov	r3, #0
  319008: e1a02004     	mov	r2, r4
  31900c: e4c23001     	strb	r3, [r2], #1
  319010: e1a00006     	mov	r0, r6
  319014: e1a01005     	mov	r1, r5
  319018: ebffff8f     	bl	0x318e5c <UserProperties::_ParseKeyValue(char*, char*)> @ imm = #-0x1c4
  31901c: e3a0303d     	mov	r3, #61
  319020: e5c43000     	strb	r3, [r4]
  319024: e8bd8070     	pop	{r4, r5, r6, pc}
  319028: e1a00006     	mov	r0, r6
  31902c: e1a01005     	mov	r1, r5
  319030: e1a02004     	mov	r2, r4
  319034: e8bd4070     	pop	{r4, r5, r6, lr}
  319038: eaffff87     	b	0x318e5c <UserProperties::_ParseKeyValue(char*, char*)> @ imm = #-0x1e4
