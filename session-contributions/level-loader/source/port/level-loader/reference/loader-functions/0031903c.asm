
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0031903c <UserProperties::_ParseProperties(char const*)>:
  31903c: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  319040: e59f40f4     	ldr	r4, [pc, #0xf4]         @ 0x31913c <UserProperties::_ParseProperties(char const*)+0x100>
  319044: e59f50f4     	ldr	r5, [pc, #0xf4]         @ 0x319140 <UserProperties::_ParseProperties(char const*)+0x104>
  319048: e24dd02c     	sub	sp, sp, #44
  31904c: e08f4004     	add	r4, pc, r4
  319050: e7943005     	ldr	r3, [r4, r5]
  319054: e2512000     	subs	r2, r1, #0
  319058: e1a06000     	mov	r6, r0
  31905c: e5933000     	ldr	r3, [r3]
  319060: e58d3024     	str	r3, [sp, #0x24]
  319064: 0a00001e     	beq	0x3190e4 <UserProperties::_ParseProperties(char const*)+0xa8> @ imm = #0x78
  319068: e28d700c     	add	r7, sp, #12
  31906c: e1a00007     	mov	r0, r7
  319070: e28d2008     	add	r2, sp, #8
  319074: ebffec1c     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x4f90
  319078: e3a0b00a     	mov	r11, #10
  31907c: e59da020     	ldr	r10, [sp, #0x20]
  319080: e3a09000     	mov	r9, #0
  319084: ea000005     	b	0x3190a0 <UserProperties::_ParseProperties(char const*)+0x64> @ imm = #0x14
  319088: e1a0100a     	mov	r1, r10
  31908c: e5c89000     	strb	r9, [r8]
  319090: e1a00006     	mov	r0, r6
  319094: ebffffd2     	bl	0x318fe4 <UserProperties::_ParseLine(char*)> @ imm = #-0xb8
  319098: e288a001     	add	r10, r8, #1
  31909c: e5c8b000     	strb	r11, [r8]
  3190a0: e1a0000a     	mov	r0, r10
  3190a4: e3a0100a     	mov	r1, #10
  3190a8: ebffd6de     	bl	0x30ec28 <.plt+0xeb4>   @ imm = #-0xa488
  3190ac: e2508000     	subs	r8, r0, #0
  3190b0: 1afffff4     	bne	0x319088 <UserProperties::_ParseProperties(char const*)+0x4c> @ imm = #-0x30
  3190b4: e1a00006     	mov	r0, r6
  3190b8: e1a0100a     	mov	r1, r10
  3190bc: ebffffc8     	bl	0x318fe4 <UserProperties::_ParseLine(char*)> @ imm = #-0xe0
  3190c0: e1a00007     	mov	r0, r7
  3190c4: ebfffc62     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0xe78
  3190c8: e7943005     	ldr	r3, [r4, r5]
  3190cc: e59d2024     	ldr	r2, [sp, #0x24]
  3190d0: e5933000     	ldr	r3, [r3]
  3190d4: e1520003     	cmp	r2, r3
  3190d8: 1a000016     	bne	0x319138 <UserProperties::_ParseProperties(char const*)+0xfc> @ imm = #0x58
  3190dc: e28dd02c     	add	sp, sp, #44
  3190e0: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3190e4: e59f3058     	ldr	r3, [pc, #0x58]         @ 0x319144 <UserProperties::_ParseProperties(char const*)+0x108>
  3190e8: e7943003     	ldr	r3, [r4, r3]
  3190ec: e5933000     	ldr	r3, [r3]
  3190f0: e3530002     	cmp	r3, #2
  3190f4: 05822000     	streq	r2, [r2]
  3190f8: 0afffff2     	beq	0x3190c8 <UserProperties::_ParseProperties(char const*)+0x8c> @ imm = #-0x38
  3190fc: e3530001     	cmp	r3, #1
  319100: 1afffff0     	bne	0x3190c8 <UserProperties::_ParseProperties(char const*)+0x8c> @ imm = #-0x40
  319104: e59f003c     	ldr	r0, [pc, #0x3c]         @ 0x319148 <UserProperties::_ParseProperties(char const*)+0x10c>
  319108: e59f103c     	ldr	r1, [pc, #0x3c]         @ 0x31914c <UserProperties::_ParseProperties(char const*)+0x110>
  31910c: e59f203c     	ldr	r2, [pc, #0x3c]         @ 0x319150 <UserProperties::_ParseProperties(char const*)+0x114>
  319110: e7940000     	ldr	r0, [r4, r0]
  319114: e59f3038     	ldr	r3, [pc, #0x38]         @ 0x319154 <UserProperties::_ParseProperties(char const*)+0x118>
  319118: e3a0c025     	mov	r12, #37
  31911c: e08f1001     	add	r1, pc, r1
  319120: e08f2002     	add	r2, pc, r2
  319124: e08f3003     	add	r3, pc, r3
  319128: e28000a8     	add	r0, r0, #168
  31912c: e58dc000     	str	r12, [sp]
  319130: ebffd3b3     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xb134
  319134: eaffffe3     	b	0x3190c8 <UserProperties::_ParseProperties(char const*)+0x8c> @ imm = #-0x74
  319138: ebffd474     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xae30
  31913c: 44 ba 67 00  	.word	0x0067ba44
  319140: ac 40 00 00  	.word	0x000040ac
  319144: c0 39 00 00  	.word	0x000039c0
  319148: c0 19 00 00  	.word	0x000019c0
  31914c: bc 52 5a 00  	.word	0x005a52bc
  319150: 58 56 5a 00  	.word	0x005a5658
  319154: 5c 56 5a 00  	.word	0x005a565c
