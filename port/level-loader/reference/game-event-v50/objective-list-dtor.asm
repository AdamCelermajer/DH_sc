
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0047a8cc <ObjectiveList::~ObjectiveList()>:
  47a8cc: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  47a8d0: e5902000     	ldr	r2, [r0]
  47a8d4: e1a06000     	mov	r6, r0
  47a8d8: e3520000     	cmp	r2, #0
  47a8dc: d5905004     	ldrle	r5, [r0, #0x4]
  47a8e0: da00000e     	ble	0x47a920 <ObjectiveList::~ObjectiveList()+0x54> @ imm = #0x38
  47a8e4: e5905004     	ldr	r5, [r0, #0x4]
  47a8e8: e3a04000     	mov	r4, #0
  47a8ec: e1a07004     	mov	r7, r4
  47a8f0: e7953104     	ldr	r3, [r5, r4, lsl #2]
  47a8f4: e3530000     	cmp	r3, #0
  47a8f8: 0a000005     	beq	0x47a914 <ObjectiveList::~ObjectiveList()+0x48> @ imm = #0x14
  47a8fc: e1a00003     	mov	r0, r3
  47a900: e5933000     	ldr	r3, [r3]
  47a904: e1a0e00f     	mov	lr, pc
  47a908: e593f004     	ldr	pc, [r3, #0x4]
  47a90c: e7857104     	str	r7, [r5, r4, lsl #2]
  47a910: e8960024     	ldm	r6, {r2, r5}
  47a914: e2844001     	add	r4, r4, #1
  47a918: e1520004     	cmp	r2, r4
  47a91c: cafffff3     	bgt	0x47a8f0 <ObjectiveList::~ObjectiveList()+0x24> @ imm = #-0x34
  47a920: e3550000     	cmp	r5, #0
  47a924: 0a000003     	beq	0x47a938 <ObjectiveList::~ObjectiveList()+0x6c> @ imm = #0xc
  47a928: e1a00005     	mov	r0, r5
  47a92c: ebfa56c3     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x16a4f4
  47a930: e3a03000     	mov	r3, #0
  47a934: e5863004     	str	r3, [r6, #0x4]
  47a938: e1a00006     	mov	r0, r6
  47a93c: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}

0047a940 <ObjectiveList::~ObjectiveList()>:
  47a940: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  47a944: e5902000     	ldr	r2, [r0]
  47a948: e1a06000     	mov	r6, r0
  47a94c: e3520000     	cmp	r2, #0
  47a950: d5905004     	ldrle	r5, [r0, #0x4]
  47a954: da00000e     	ble	0x47a994 <ObjectiveList::~ObjectiveList()+0x54> @ imm = #0x38
  47a958: e5905004     	ldr	r5, [r0, #0x4]
  47a95c: e3a04000     	mov	r4, #0
  47a960: e1a07004     	mov	r7, r4
  47a964: e7953104     	ldr	r3, [r5, r4, lsl #2]
  47a968: e3530000     	cmp	r3, #0
  47a96c: 0a000005     	beq	0x47a988 <ObjectiveList::~ObjectiveList()+0x48> @ imm = #0x14
  47a970: e1a00003     	mov	r0, r3
  47a974: e5933000     	ldr	r3, [r3]
  47a978: e1a0e00f     	mov	lr, pc
  47a97c: e593f004     	ldr	pc, [r3, #0x4]
  47a980: e7857104     	str	r7, [r5, r4, lsl #2]
  47a984: e8960024     	ldm	r6, {r2, r5}
  47a988: e2844001     	add	r4, r4, #1
  47a98c: e1520004     	cmp	r2, r4
  47a990: cafffff3     	bgt	0x47a964 <ObjectiveList::~ObjectiveList()+0x24> @ imm = #-0x34
  47a994: e3550000     	cmp	r5, #0
  47a998: 0a000003     	beq	0x47a9ac <ObjectiveList::~ObjectiveList()+0x6c> @ imm = #0xc
  47a99c: e1a00005     	mov	r0, r5
  47a9a0: ebfa56a6     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x16a568
  47a9a4: e3a03000     	mov	r3, #0
  47a9a8: e5863004     	str	r3, [r6, #0x4]
  47a9ac: e1a00006     	mov	r0, r6
  47a9b0: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}

0047a9b4 <Vector3DFList::setNew(int)>:
  47a9b4: e92d4070     	push	{r4, r5, r6, lr}
  47a9b8: e1a04000     	mov	r4, r0
  47a9bc: e5900000     	ldr	r0, [r0]
  47a9c0: e3a06000     	mov	r6, #0
  47a9c4: e1a05001     	mov	r5, r1
  47a9c8: e1500006     	cmp	r0, r6
  47a9cc: e5841008     	str	r1, [r4, #0x8]
  47a9d0: e584600c     	str	r6, [r4, #0xc]
  47a9d4: 0a000001     	beq	0x47a9e0 <Vector3DFList::setNew(int)+0x2c> @ imm = #0x4
  47a9d8: ebfa5698     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x16a5a0
  47a9dc: e5846000     	str	r6, [r4]
  47a9e0: e5940004     	ldr	r0, [r4, #0x4]
  47a9e4: e3500000     	cmp	r0, #0
  47a9e8: 0a000002     	beq	0x47a9f8 <Vector3DFList::setNew(int)+0x44> @ imm = #0x8
  47a9ec: ebfa5693     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x16a5b4
  47a9f0: e3a03000     	mov	r3, #0
  47a9f4: e5843004     	str	r3, [r4, #0x4]
  47a9f8: e3a0000c     	mov	r0, #12
  47a9fc: e0000590     	mul	r0, r0, r5
  47aa00: ebfa5693     	bl	0x310454 <CustomAlloc(unsigned int)> @ imm = #-0x16a5b4
  47aa04: e3550000     	cmp	r5, #0
  47aa08: 0a000009     	beq	0x47aa34 <Vector3DFList::setNew(int)+0x80> @ imm = #0x24
  47aa0c: e3a01000     	mov	r1, #0
  47aa10: e1a03000     	mov	r3, r0
  47aa14: e3a02000     	mov	r2, #0
  47aa18: e2822001     	add	r2, r2, #1
  47aa1c: e1520005     	cmp	r2, r5
  47aa20: e5831000     	str	r1, [r3]
  47aa24: e5831004     	str	r1, [r3, #0x4]
  47aa28: e5831008     	str	r1, [r3, #0x8]
  47aa2c: e283300c     	add	r3, r3, #12
  47aa30: 1afffff8     	bne	0x47aa18 <Vector3DFList::setNew(int)+0x64> @ imm = #-0x20
  47aa34: e5840000     	str	r0, [r4]
  47aa38: e1a00105     	lsl	r0, r5, #2
  47aa3c: ebfa5684     	bl	0x310454 <CustomAlloc(unsigned int)> @ imm = #-0x16a5f0
  47aa40: e5840004     	str	r0, [r4, #0x4]
  47aa44: e8bd8070     	pop	{r4, r5, r6, pc}
