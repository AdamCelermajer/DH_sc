
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00479cac <std::vector<GameEvent*, std::allocator<GameEvent*>>::vector(unsigned int) (.clone.1)>:
  479cac: e92d4010     	push	{r4, lr}
  479cb0: e24dd008     	sub	sp, sp, #8
  479cb4: e1a04000     	mov	r4, r0
  479cb8: e3a01000     	mov	r1, #0
  479cbc: e28d2008     	add	r2, sp, #8
  479cc0: e5221004     	str	r1, [r2, #-0x4]!
  479cc4: e5841000     	str	r1, [r4]
  479cc8: e5841004     	str	r1, [r4, #0x4]
  479ccc: e5a01008     	str	r1, [r0, #0x8]!
  479cd0: ebffffa3     	bl	0x479b64 <std::allocator<GameEvent*>::_M_allocate(unsigned int, unsigned int&)> @ imm = #-0x174
  479cd4: e59d3004     	ldr	r3, [sp, #0x4]
  479cd8: e5840000     	str	r0, [r4]
  479cdc: e5840004     	str	r0, [r4, #0x4]
  479ce0: e0800103     	add	r0, r0, r3, lsl #2
  479ce4: e5840008     	str	r0, [r4, #0x8]
  479ce8: e1a00004     	mov	r0, r4
  479cec: e28dd008     	add	sp, sp, #8
  479cf0: e8bd8010     	pop	{r4, pc}

00479cf4 <GameEventManager::GameEventManager()>:
  479cf4: e92d4010     	push	{r4, lr}
  479cf8: e1a04000     	mov	r4, r0
  479cfc: ebffffea     	bl	0x479cac <std::vector<GameEvent*, std::allocator<GameEvent*>>::vector(unsigned int) (.clone.1)> @ imm = #-0x58
  479d00: e1a00004     	mov	r0, r4
  479d04: e8bd8010     	pop	{r4, pc}

00479d08 <GameEventManager::GameEventManager()>:
  479d08: e92d4010     	push	{r4, lr}
  479d0c: e1a04000     	mov	r4, r0
  479d10: ebffffe5     	bl	0x479cac <std::vector<GameEvent*, std::allocator<GameEvent*>>::vector(unsigned int) (.clone.1)> @ imm = #-0x6c
  479d14: e1a00004     	mov	r0, r4
  479d18: e8bd8010     	pop	{r4, pc}
