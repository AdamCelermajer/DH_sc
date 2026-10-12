
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00478648 <Condition::Condition()>:
  478648: e59f301c     	ldr	r3, [pc, #0x1c]         @ 0x47866c <Condition::Condition()+0x24>
  47864c: e59f201c     	ldr	r2, [pc, #0x1c]         @ 0x478670 <Condition::Condition()+0x28>
  478650: e3a0c000     	mov	r12, #0
  478654: e08f3003     	add	r3, pc, r3
  478658: e7932002     	ldr	r2, [r3, r2]
  47865c: e580c004     	str	r12, [r0, #0x4]
  478660: e2822008     	add	r2, r2, #8
  478664: e5802000     	str	r2, [r0]
  478668: e12fff1e     	bx	lr
  47866c: 3c c4 51 00  	.word	0x0051c43c
  478670: ac 3c 00 00  	.word	0x00003cac
