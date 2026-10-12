
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00320678 <Application::IsUsingUncompiledData(char const*) const>:
  320678: e92d4070     	push	{r4, r5, r6, lr}
  32067c: e59030d0     	ldr	r3, [r0, #0xd0]
  320680: e59020cc     	ldr	r2, [r0, #0xcc]
  320684: e1a05001     	mov	r5, r1
  320688: e1520003     	cmp	r2, r3
  32068c: 0a00000a     	beq	0x3206bc <Application::IsUsingUncompiledData(char const*) const+0x44> @ imm = #0x28
  320690: e59f6034     	ldr	r6, [pc, #0x34]         @ 0x3206cc <Application::IsUsingUncompiledData(char const*) const+0x54>
  320694: e3a04000     	mov	r4, #0
  320698: e08f6006     	add	r6, pc, r6
  32069c: e7961004     	ldr	r1, [r6, r4]
  3206a0: e1a00005     	mov	r0, r5
  3206a4: ebffb94a     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x11ad8
  3206a8: e3500000     	cmp	r0, #0
  3206ac: e2844004     	add	r4, r4, #4
  3206b0: 1a000003     	bne	0x3206c4 <Application::IsUsingUncompiledData(char const*) const+0x4c> @ imm = #0xc
  3206b4: e3540010     	cmp	r4, #16
  3206b8: 1afffff7     	bne	0x32069c <Application::IsUsingUncompiledData(char const*) const+0x24> @ imm = #-0x24
  3206bc: e3a00000     	mov	r0, #0
  3206c0: e8bd8070     	pop	{r4, r5, r6, pc}
  3206c4: e3a00001     	mov	r0, #1
  3206c8: e8bd8070     	pop	{r4, r5, r6, pc}
  3206cc: 00 63 63 00  	.word	0x00636300
