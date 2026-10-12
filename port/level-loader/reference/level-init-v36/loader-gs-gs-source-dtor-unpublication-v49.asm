
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003860bc <GSLevel::Dtor(StateMachine const*)>:
  3860bc: e92d4070     	push	{r4, r5, r6, lr}
  3860c0: e1a05000     	mov	r5, r0
  3860c4: e5900034     	ldr	r0, [r0, #0x34]
  3860c8: e59f4068     	ldr	r4, [pc, #0x68]         @ 0x386138 <GSLevel::Dtor(StateMachine const*)+0x7c>
  3860cc: e3500000     	cmp	r0, #0
  3860d0: e08f4004     	add	r4, pc, r4
  3860d4: 0a000012     	beq	0x386124 <GSLevel::Dtor(StateMachine const*)+0x68> @ imm = #0x48
  3860d8: eb01a96c     	bl	0x3f0690 <Level::Unload()> @ imm = #0x6a5b0
  3860dc: eb029a6a     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #0xa69a8
  3860e0: e59f1054     	ldr	r1, [pc, #0x54]         @ 0x38613c <GSLevel::Dtor(StateMachine const*)+0x80>
  3860e4: e08f1001     	add	r1, pc, r1
  3860e8: eb029c40     	bl	0x42d1f0 <MenuManager::GetMenuByName(char const*)> @ imm = #0xa7100
  3860ec: e2503000     	subs	r3, r0, #0
  3860f0: 0a000002     	beq	0x386100 <GSLevel::Dtor(StateMachine const*)+0x44> @ imm = #0x8
  3860f4: e5933000     	ldr	r3, [r3]
  3860f8: e1a0e00f     	mov	lr, pc
  3860fc: e593f010     	ldr	pc, [r3, #0x10]
  386100: e5953034     	ldr	r3, [r5, #0x34]
  386104: e3530000     	cmp	r3, #0
  386108: 0a000005     	beq	0x386124 <GSLevel::Dtor(StateMachine const*)+0x68> @ imm = #0x14
  38610c: e1a00003     	mov	r0, r3
  386110: e5933000     	ldr	r3, [r3]
  386114: e1a0e00f     	mov	lr, pc
  386118: e593f004     	ldr	pc, [r3, #0x4]
  38611c: e3a03000     	mov	r3, #0
  386120: e5853034     	str	r3, [r5, #0x34]
  386124: e59f3014     	ldr	r3, [pc, #0x14]         @ 0x386140 <GSLevel::Dtor(StateMachine const*)+0x84>
  386128: e3a02000     	mov	r2, #0
  38612c: e7943003     	ldr	r3, [r4, r3]
  386130: e5832000     	str	r2, [r3]
  386134: e8bd8070     	pop	{r4, r5, r6, pc}
  386138: c0 e9 60 00  	.word	0x0060e9c0
  38613c: c4 b6 53 00  	.word	0x0053b6c4
  386140: 64 1d 00 00  	.word	0x00001d64
