
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00478674 <Condition::Condition()>:
  478674: e59f301c     	ldr	r3, [pc, #0x1c]         @ 0x478698 <Condition::Condition()+0x24>
  478678: e59f201c     	ldr	r2, [pc, #0x1c]         @ 0x47869c <Condition::Condition()+0x28>
  47867c: e3a0c000     	mov	r12, #0
  478680: e08f3003     	add	r3, pc, r3
  478684: e7932002     	ldr	r2, [r3, r2]
  478688: e580c004     	str	r12, [r0, #0x4]
  47868c: e2822008     	add	r2, r2, #8
  478690: e5802000     	str	r2, [r0]
  478694: e12fff1e     	bx	lr
  478698: 10 c4 51 00  	.word	0x0051c410
  47869c: ac 3c 00 00  	.word	0x00003cac
