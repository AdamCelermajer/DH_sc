
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048bc94 <rnd::Rule::Unload()>:
  48bc94: e92d4010     	push	{r4, lr}
  48bc98: e1a04000     	mov	r4, r0
  48bc9c: e594200c     	ldr	r2, [r4, #0xc]
  48bca0: e2821004     	add	r1, r2, #4
  48bca4: e0841101     	add	r1, r4, r1, lsl #2
  48bca8: e3520000     	cmp	r2, #0
  48bcac: 0a000009     	beq	0x48bcd8 <rnd::Rule::Unload()+0x44> @ imm = #0x24
  48bcb0: e2422001     	sub	r2, r2, #1
  48bcb4: e584200c     	str	r2, [r4, #0xc]
  48bcb8: e5313004     	ldr	r3, [r1, #-0x4]!
  48bcbc: e3530000     	cmp	r3, #0
  48bcc0: 0afffff8     	beq	0x48bca8 <rnd::Rule::Unload()+0x14> @ imm = #-0x20
  48bcc4: e1a00003     	mov	r0, r3
  48bcc8: e5933000     	ldr	r3, [r3]
  48bccc: e1a0e00f     	mov	lr, pc
  48bcd0: e593f004     	ldr	pc, [r3, #0x4]
  48bcd4: eafffff0     	b	0x48bc9c <rnd::Rule::Unload()+0x8> @ imm = #-0x40
  48bcd8: e1a03002     	mov	r3, r2
  48bcdc: e2822001     	add	r2, r2, #1
  48bce0: e3520010     	cmp	r2, #16
  48bce4: e5843010     	str	r3, [r4, #0x10]
  48bce8: e2844004     	add	r4, r4, #4
  48bcec: 1afffffa     	bne	0x48bcdc <rnd::Rule::Unload()+0x48> @ imm = #-0x18
  48bcf0: e8bd8010     	pop	{r4, pc}
