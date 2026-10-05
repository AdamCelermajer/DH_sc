
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004865f0 <rnd::RandomGenerator::LoadListRules(TiXmlNode*)>:
  4865f0: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  4865f4: e59fa0bc     	ldr	r10, [pc, #0xbc]        @ 0x4866b8 <rnd::RandomGenerator::LoadListRules(TiXmlNode*)+0xc8>
  4865f8: e24dd024     	sub	sp, sp, #36
  4865fc: e1a07000     	mov	r7, r0
  486600: e08fa00a     	add	r10, pc, r10
  486604: e1a00001     	mov	r0, r1
  486608: e1a0100a     	mov	r1, r10
  48660c: eb0239e1     	bl	0x514d98 <TiXmlNode::FirstChild(char const*) const> @ imm = #0x8e784
  486610: e2505000     	subs	r5, r0, #0
  486614: 0a000024     	beq	0x4866ac <rnd::RandomGenerator::LoadListRules(TiXmlNode*)+0xbc> @ imm = #0x90
  486618: e59f909c     	ldr	r9, [pc, #0x9c]         @ 0x4866bc <rnd::RandomGenerator::LoadListRules(TiXmlNode*)+0xcc>
  48661c: e28d300c     	add	r3, sp, #12
  486620: e58d3004     	str	r3, [sp, #0x4]
  486624: e28d3014     	add	r3, sp, #20
  486628: e2878054     	add	r8, r7, #84
  48662c: e08f9009     	add	r9, pc, r9
  486630: e28db01c     	add	r11, sp, #28
  486634: e58d3000     	str	r3, [sp]
  486638: e3a06000     	mov	r6, #0
  48663c: e3a01000     	mov	r1, #0
  486640: e3a0002c     	mov	r0, #44
  486644: ebfa27c9     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x1760dc
  486648: e1a0200b     	mov	r2, r11
  48664c: e1a01009     	mov	r1, r9
  486650: e1a04000     	mov	r4, r0
  486654: ebfa36a4     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x172570
  486658: e3a03001     	mov	r3, #1
  48665c: e1a01005     	mov	r1, r5
  486660: e5c43018     	strb	r3, [r4, #0x18]
  486664: e1a00004     	mov	r0, r4
  486668: e584601c     	str	r6, [r4, #0x1c]
  48666c: e5846020     	str	r6, [r4, #0x20]
  486670: e5846024     	str	r6, [r4, #0x24]
  486674: e5847028     	str	r7, [r4, #0x28]
  486678: eb0021ab     	bl	0x48ed2c <rnd::ListRule::LoadFromXml(TiXmlNode*)> @ imm = #0x86ac
  48667c: e5943014     	ldr	r3, [r4, #0x14]
  486680: e59d2000     	ldr	r2, [sp]
  486684: e59d0004     	ldr	r0, [sp, #0x4]
  486688: e1a01008     	mov	r1, r8
  48668c: e58d3014     	str	r3, [sp, #0x14]
  486690: e58d4018     	str	r4, [sp, #0x18]
  486694: ebffff6c     	bl	0x48644c <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)> @ imm = #-0x250
  486698: e1a00005     	mov	r0, r5
  48669c: e1a0100a     	mov	r1, r10
  4866a0: eb023988     	bl	0x514cc8 <TiXmlNode::NextSibling(char const*) const> @ imm = #0x8e620
  4866a4: e2505000     	subs	r5, r0, #0
  4866a8: 1affffe3     	bne	0x48663c <rnd::RandomGenerator::LoadListRules(TiXmlNode*)+0x4c> @ imm = #-0x74
  4866ac: e3a00001     	mov	r0, #1
  4866b0: e28dd024     	add	sp, sp, #36
  4866b4: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  4866b8: 18 6c 46 00  	.word	0x00466c18
  4866bc: dc 51 44 00  	.word	0x004451dc
