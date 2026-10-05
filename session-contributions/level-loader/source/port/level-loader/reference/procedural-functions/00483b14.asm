
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00483b14 <rnd::RandomGenerator::Hash(unsigned char*)>:
  483b14: e5d13000     	ldrb	r3, [r1]
  483b18: e3530000     	cmp	r3, #0
  483b1c: 03010505     	movweq	r0, #0x1505
  483b20: 012fff1e     	bxeq	lr
  483b24: e3010505     	movw	r0, #0x1505
  483b28: e0832280     	add	r2, r3, r0, lsl #5
  483b2c: e5f13001     	ldrb	r3, [r1, #0x1]!
  483b30: e0800002     	add	r0, r0, r2
  483b34: e3530000     	cmp	r3, #0
  483b38: 1afffffa     	bne	0x483b28 <rnd::RandomGenerator::Hash(unsigned char*)+0x14> @ imm = #-0x18
  483b3c: e12fff1e     	bx	lr
