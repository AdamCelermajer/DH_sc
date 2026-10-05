
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00485a0c <rnd::RoomPool::~RoomPool()>:
  485a0c: e92d4010     	push	{r4, lr}
  485a10: e5903004     	ldr	r3, [r0, #0x4]
  485a14: e5902008     	ldr	r2, [r0, #0x8]
  485a18: e1a04000     	mov	r4, r0
  485a1c: e1530002     	cmp	r3, r2
  485a20: 15803008     	strne	r3, [r0, #0x8]
  485a24: e2800004     	add	r0, r0, #4
  485a28: ebffffe0     	bl	0x4859b0 <std::vector<rnd::RPElem, std::allocator<rnd::RPElem>>::~vector()> @ imm = #-0x80
  485a2c: e1a00004     	mov	r0, r4
  485a30: e8bd8010     	pop	{r4, pc}
