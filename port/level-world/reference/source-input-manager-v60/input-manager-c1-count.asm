
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034d6bc <InputManager::InputManager(int, int, int)>:
  34d6bc: e92d0070     	push	{r4, r5, r6}
  34d6c0: e59f402c     	ldr	r4, [pc, #0x2c]         @ 0x34d6f4 <InputManager::InputManager(int, int, int)+0x38>
  34d6c4: e59f502c     	ldr	r5, [pc, #0x2c]         @ 0x34d6f8 <InputManager::InputManager(int, int, int)+0x3c>
  34d6c8: e3a06001     	mov	r6, #1
  34d6cc: e08f4004     	add	r4, pc, r4
  34d6d0: e7945005     	ldr	r5, [r4, r5]
  34d6d4: e5c06010     	strb	r6, [r0, #0x10]
  34d6d8: e5801004     	str	r1, [r0, #0x4]
  34d6dc: e2855008     	add	r5, r5, #8
  34d6e0: e5805000     	str	r5, [r0]
  34d6e4: e5802008     	str	r2, [r0, #0x8]
  34d6e8: e580300c     	str	r3, [r0, #0xc]
  34d6ec: e8bd0070     	pop	{r4, r5, r6}
  34d6f0: e12fff1e     	bx	lr
  34d6f4: c4 73 64 00  	.word	0x006473c4
  34d6f8: b4 06 00 00  	.word	0x000006b4

0034d6fc <InputManager::InputManager(int, int, int)>:
  34d6fc: e92d0070     	push	{r4, r5, r6}
  34d700: e59f402c     	ldr	r4, [pc, #0x2c]         @ 0x34d734 <InputManager::InputManager(int, int, int)+0x38>
  34d704: e59f502c     	ldr	r5, [pc, #0x2c]         @ 0x34d738 <InputManager::InputManager(int, int, int)+0x3c>
  34d708: e3a06001     	mov	r6, #1
  34d70c: e08f4004     	add	r4, pc, r4
  34d710: e7945005     	ldr	r5, [r4, r5]
  34d714: e5c06010     	strb	r6, [r0, #0x10]
  34d718: e5801004     	str	r1, [r0, #0x4]
  34d71c: e2855008     	add	r5, r5, #8
  34d720: e5805000     	str	r5, [r0]
  34d724: e5802008     	str	r2, [r0, #0x8]
  34d728: e580300c     	str	r3, [r0, #0xc]
  34d72c: e8bd0070     	pop	{r4, r5, r6}
  34d730: e12fff1e     	bx	lr
  34d734: 84 73 64 00  	.word	0x00647384
  34d738: b4 06 00 00  	.word	0x000006b4

0034d73c <InputManager::~InputManager()>:
  34d73c: e12fff1e     	bx	lr

0034d740 <InputManager::~InputManager()>:
  34d740: e12fff1e     	bx	lr

0034d744 <InputManager::GetNumMouses() const>:
  34d744: e5900004     	ldr	r0, [r0, #0x4]
  34d748: e12fff1e     	bx	lr

0034d74c <InputManager::GetNumKeyboards() const>:
  34d74c: e5900008     	ldr	r0, [r0, #0x8]
  34d750: e12fff1e     	bx	lr

0034d754 <InputManager::GetNumGamepads() const>:
  34d754: e590000c     	ldr	r0, [r0, #0xc]
  34d758: e12fff1e     	bx	lr
