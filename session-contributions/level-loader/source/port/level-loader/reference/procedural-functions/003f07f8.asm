
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f07f8 <Level::GenerateRandomLevel(StreamBuffer&, unsigned int)>:
  3f07f8: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3f07fc: e590514c     	ldr	r5, [r0, #0x14c]
  3f0800: e1a04000     	mov	r4, r0
  3f0804: e1a06001     	mov	r6, r1
  3f0808: e3550000     	cmp	r5, #0
  3f080c: e1a07002     	mov	r7, r2
  3f0810: 0a000005     	beq	0x3f082c <Level::GenerateRandomLevel(StreamBuffer&, unsigned int)+0x34> @ imm = #0x14
  3f0814: e1a00005     	mov	r0, r5
  3f0818: eb025f39     	bl	0x488504 <rnd::RandomGenerator::~RandomGenerator()> @ imm = #0x97ce4
  3f081c: e1a00005     	mov	r0, r5
  3f0820: ebfc7f06     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xe03e8
  3f0824: e3a03000     	mov	r3, #0
  3f0828: e584314c     	str	r3, [r4, #0x14c]
  3f082c: e3a01000     	mov	r1, #0
  3f0830: e3a00e19     	mov	r0, #400
  3f0834: ebfc7f4d     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0xe02cc
  3f0838: e1a05000     	mov	r5, r0
  3f083c: eb025d90     	bl	0x487e84 <rnd::RandomGenerator::RandomGenerator()> @ imm = #0x97640
  3f0840: e584514c     	str	r5, [r4, #0x14c]
  3f0844: e1a00005     	mov	r0, r5
  3f0848: e594110c     	ldr	r1, [r4, #0x10c]
  3f084c: eb02632d     	bl	0x489508 <rnd::RandomGenerator::LoadRuleFile(char const*)> @ imm = #0x98cb4
  3f0850: e1a01007     	mov	r1, r7
  3f0854: e594014c     	ldr	r0, [r4, #0x14c]
  3f0858: eb025e35     	bl	0x488134 <rnd::RandomGenerator::Generate(int)> @ imm = #0x978d4
  3f085c: e1a01006     	mov	r1, r6
  3f0860: e1a05000     	mov	r5, r0
  3f0864: e594014c     	ldr	r0, [r4, #0x14c]
  3f0868: eb0260c5     	bl	0x488b84 <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)> @ imm = #0x98314
  3f086c: e594614c     	ldr	r6, [r4, #0x14c]
  3f0870: e3560000     	cmp	r6, #0
  3f0874: 0a000005     	beq	0x3f0890 <Level::GenerateRandomLevel(StreamBuffer&, unsigned int)+0x98> @ imm = #0x14
  3f0878: e1a00006     	mov	r0, r6
  3f087c: eb025f20     	bl	0x488504 <rnd::RandomGenerator::~RandomGenerator()> @ imm = #0x97c80
  3f0880: e1a00006     	mov	r0, r6
  3f0884: ebfc7eed     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xe044c
  3f0888: e3a03000     	mov	r3, #0
  3f088c: e584314c     	str	r3, [r4, #0x14c]
  3f0890: e1a00005     	mov	r0, r5
  3f0894: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
