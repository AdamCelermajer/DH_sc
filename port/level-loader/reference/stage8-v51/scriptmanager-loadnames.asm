
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0045ad40 <ScriptManager::LoadScriptFileNames(char const*, bool)>:
  45ad40: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  45ad44: e59f3210     	ldr	r3, [pc, #0x210]        @ 0x45af5c <ScriptManager::LoadScriptFileNames(char const*, bool)+0x21c>
  45ad48: e3520000     	cmp	r2, #0
  45ad4c: e24dd054     	sub	sp, sp, #84
  45ad50: e1a07000     	mov	r7, r0
  45ad54: e08f3003     	add	r3, pc, r3
  45ad58: 0a00000b     	beq	0x45ad8c <ScriptManager::LoadScriptFileNames(char const*, bool)+0x4c> @ imm = #0x2c
  45ad5c: e590001c     	ldr	r0, [r0, #0x1c]
  45ad60: e5972018     	ldr	r2, [r7, #0x18]
  45ad64: e597c008     	ldr	r12, [r7, #0x8]
  45ad68: e0622000     	rsb	r2, r2, r0
  45ad6c: e1a02142     	asr	r2, r2, #2
  45ad70: e0820102     	add	r0, r2, r2, lsl #2
  45ad74: e0800200     	add	r0, r0, r0, lsl #4
  45ad78: e0800400     	add	r0, r0, r0, lsl #8
  45ad7c: e0800800     	add	r0, r0, r0, lsl #16
  45ad80: e0822080     	add	r2, r2, r0, lsl #1
  45ad84: e15c0002     	cmp	r12, r2
  45ad88: ba000060     	blt	0x45af10 <ScriptManager::LoadScriptFileNames(char const*, bool)+0x1d0> @ imm = #0x180
  45ad8c: e59f01cc     	ldr	r0, [pc, #0x1cc]        @ 0x45af60 <ScriptManager::LoadScriptFileNames(char const*, bool)+0x220>
  45ad90: e3a02000     	mov	r2, #0
  45ad94: e7934000     	ldr	r4, [r3, r0]
  45ad98: e1a03002     	mov	r3, r2
  45ad9c: e5940010     	ldr	r0, [r4, #0x10]
  45ada0: e590c034     	ldr	r12, [r0, #0x34]
  45ada4: e1a0000c     	mov	r0, r12
  45ada8: e59cc000     	ldr	r12, [r12]
  45adac: e1a0e00f     	mov	lr, pc
  45adb0: e59cf088     	ldr	pc, [r12, #0x88]
  45adb4: e2503000     	subs	r3, r0, #0
  45adb8: 0a000054     	beq	0x45af10 <ScriptManager::LoadScriptFileNames(char const*, bool)+0x1d0> @ imm = #0x150
  45adbc: e28d8008     	add	r8, sp, #8
  45adc0: e1a01003     	mov	r1, r3
  45adc4: e1a00008     	mov	r0, r8
  45adc8: e58d304c     	str	r3, [sp, #0x4c]
  45adcc: ebfaf141     	bl	0x3172d8 <StreamBuffer::StreamBuffer(IStreamBase*)> @ imm = #-0x143afc
  45add0: e5943010     	ldr	r3, [r4, #0x10]
  45add4: e28d104c     	add	r1, sp, #76
  45add8: e28d4048     	add	r4, sp, #72
  45addc: e5933034     	ldr	r3, [r3, #0x34]
  45ade0: e1a00003     	mov	r0, r3
  45ade4: e5933000     	ldr	r3, [r3]
  45ade8: e1a0e00f     	mov	lr, pc
  45adec: e593f078     	ldr	pc, [r3, #0x78]
  45adf0: e3a03000     	mov	r3, #0
  45adf4: e1a00008     	mov	r0, r8
  45adf8: e1a01004     	mov	r1, r4
  45adfc: e3a02004     	mov	r2, #4
  45ae00: ebfaeda3     	bl	0x316494 <StreamBuffer::read(void*, unsigned long long)> @ imm = #-0x144974
  45ae04: e3a03001     	mov	r3, #1
  45ae08: e3530000     	cmp	r3, #0
  45ae0c: e58d303c     	str	r3, [sp, #0x3c]
  45ae10: 0a000040     	beq	0x45af18 <ScriptManager::LoadScriptFileNames(char const*, bool)+0x1d8> @ imm = #0x100
  45ae14: e1a00007     	mov	r0, r7
  45ae18: e5b03024     	ldr	r3, [r0, #0x24]!
  45ae1c: e5975028     	ldr	r5, [r7, #0x28]
  45ae20: e59d9048     	ldr	r9, [sp, #0x48]
  45ae24: e28d2050     	add	r2, sp, #80
  45ae28: e0635005     	rsb	r5, r3, r5
  45ae2c: e1a05145     	asr	r5, r5, #2
  45ae30: e0859009     	add	r9, r5, r9
  45ae34: e3a04000     	mov	r4, #0
  45ae38: e522400c     	str	r4, [r2, #-0xc]!
  45ae3c: e1a01009     	mov	r1, r9
  45ae40: ebffffad     	bl	0x45acfc <std::vector<char*, std::allocator<char*>>::resize(unsigned int, char* const&)> @ imm = #-0x14c
  45ae44: e1590005     	cmp	r9, r5
  45ae48: da00002e     	ble	0x45af08 <ScriptManager::LoadScriptFileNames(char const*, bool)+0x1c8> @ imm = #0xb8
  45ae4c: e28db040     	add	r11, sp, #64
  45ae50: e3a0a001     	mov	r10, #1
  45ae54: e08b200a     	add	r2, r11, r10
  45ae58: e28b3002     	add	r3, r11, #2
  45ae5c: e1a06105     	lsl	r6, r5, #2
  45ae60: e88d000c     	stm	sp, {r2, r3}
  45ae64: e1a00008     	mov	r0, r8
  45ae68: e1a0100b     	mov	r1, r11
  45ae6c: ebfe10cb     	bl	0x3df1a0 <void StreamReader::readAs<unsigned int>(IStreamBase*, unsigned int*)> @ imm = #-0x7bcd4
  45ae70: e35a0000     	cmp	r10, #0
  45ae74: e58da03c     	str	r10, [sp, #0x3c]
  45ae78: 1a00000f     	bne	0x45aebc <ScriptManager::LoadScriptFileNames(char const*, bool)+0x17c> @ imm = #0x3c
  45ae7c: e59d3000     	ldr	r3, [sp]
  45ae80: e59d2004     	ldr	r2, [sp, #0x4]
  45ae84: e5d20001     	ldrb	r0, [r2, #0x1]
  45ae88: e5531001     	ldrb	r1, [r3, #-0x1]
  45ae8c: e1520003     	cmp	r2, r3
  45ae90: e0201001     	eor	r1, r0, r1
  45ae94: e5431001     	strb	r1, [r3, #-0x1]
  45ae98: e5d20001     	ldrb	r0, [r2, #0x1]
  45ae9c: e0211000     	eor	r1, r1, r0
  45aea0: e5c21001     	strb	r1, [r2, #0x1]
  45aea4: e5530001     	ldrb	r0, [r3, #-0x1]
  45aea8: e2422001     	sub	r2, r2, #1
  45aeac: e0211000     	eor	r1, r1, r0
  45aeb0: e5431001     	strb	r1, [r3, #-0x1]
  45aeb4: e2833001     	add	r3, r3, #1
  45aeb8: 8afffff1     	bhi	0x45ae84 <ScriptManager::LoadScriptFileNames(char const*, bool)+0x144> @ imm = #-0x3c
  45aebc: e59d0040     	ldr	r0, [sp, #0x40]
  45aec0: e3a01000     	mov	r1, #0
  45aec4: e2855001     	add	r5, r5, #1
  45aec8: e2800001     	add	r0, r0, #1
  45aecc: ebfad5a6     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0x14a968
  45aed0: e1a04000     	mov	r4, r0
  45aed4: e59d2040     	ldr	r2, [sp, #0x40]
  45aed8: e3a03000     	mov	r3, #0
  45aedc: e1a00008     	mov	r0, r8
  45aee0: e1a01004     	mov	r1, r4
  45aee4: ebfaf15a     	bl	0x317454 <StreamReader::readStringEx(IStreamBase*, char*, unsigned long long)> @ imm = #-0x143a98
  45aee8: e59d3040     	ldr	r3, [sp, #0x40]
  45aeec: e3a02000     	mov	r2, #0
  45aef0: e1550009     	cmp	r5, r9
  45aef4: e7c42003     	strb	r2, [r4, r3]
  45aef8: e5973024     	ldr	r3, [r7, #0x24]
  45aefc: e7834006     	str	r4, [r3, r6]
  45af00: e2866004     	add	r6, r6, #4
  45af04: 1affffd6     	bne	0x45ae64 <ScriptManager::LoadScriptFileNames(char const*, bool)+0x124> @ imm = #-0xa8
  45af08: e1a00008     	mov	r0, r8
  45af0c: ebfaeeac     	bl	0x3169c4 <StreamBuffer::~StreamBuffer()> @ imm = #-0x144550
  45af10: e28dd054     	add	sp, sp, #84
  45af14: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  45af18: e2843002     	add	r3, r4, #2
  45af1c: e2844001     	add	r4, r4, #1
  45af20: e5d31001     	ldrb	r1, [r3, #0x1]
  45af24: e5542001     	ldrb	r2, [r4, #-0x1]
  45af28: e1530004     	cmp	r3, r4
  45af2c: e0212002     	eor	r2, r1, r2
  45af30: e5442001     	strb	r2, [r4, #-0x1]
  45af34: e5d31001     	ldrb	r1, [r3, #0x1]
  45af38: e0222001     	eor	r2, r2, r1
  45af3c: e5c32001     	strb	r2, [r3, #0x1]
  45af40: e5541001     	ldrb	r1, [r4, #-0x1]
  45af44: e2433001     	sub	r3, r3, #1
  45af48: e0222001     	eor	r2, r2, r1
  45af4c: e5442001     	strb	r2, [r4, #-0x1]
  45af50: e2844001     	add	r4, r4, #1
  45af54: 8afffff1     	bhi	0x45af20 <ScriptManager::LoadScriptFileNames(char const*, bool)+0x1e0> @ imm = #-0x3c
  45af58: eaffffad     	b	0x45ae14 <ScriptManager::LoadScriptFileNames(char const*, bool)+0xd4> @ imm = #-0x14c
  45af5c: 3c 9d 53 00  	.word	0x00539d3c
  45af60: f4 37 00 00  	.word	0x000037f4

0045af64 <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::_M_fill_insert(ScriptManager::ScriptCmds*, unsigned int, ScriptManager::ScriptCmds const&)>:
  45af64: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  45af68: e2527000     	subs	r7, r2, #0
  45af6c: e24dd010     	sub	sp, sp, #16
  45af70: e1a05000     	mov	r5, r0
  45af74: e1a04001     	mov	r4, r1
  45af78: e1a06003     	mov	r6, r3
  45af7c: 0a00008a     	beq	0x45b1ac <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::_M_fill_insert(ScriptManager::ScriptCmds*, unsigned int, ScriptManager::ScriptCmds const&)+0x248> @ imm = #0x228
  45af80: e9905000     	ldmib	r0, {r12, lr}
  45af84: e06cc00e     	rsb	r12, r12, lr
  45af88: e1a0c14c     	asr	r12, r12, #2
  45af8c: e08ce10c     	add	lr, r12, r12, lsl #2
  45af90: e08ee20e     	add	lr, lr, lr, lsl #4
  45af94: e08ee40e     	add	lr, lr, lr, lsl #8
  45af98: e08ee80e     	add	lr, lr, lr, lsl #16
  45af9c: e08cc08e     	add	r12, r12, lr, lsl #1
  45afa0: e157000c     	cmp	r7, r12
  45afa4: 9a000082     	bls	0x45b1b4 <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::_M_fill_insert(ScriptManager::ScriptCmds*, unsigned int, ScriptManager::ScriptCmds const&)+0x250> @ imm = #0x208
  45afa8: e1a01007     	mov	r1, r7
  45afac: ebfff7e0     	bl	0x458f34 <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::_M_compute_next_size(unsigned int)> @ imm = #-0x2080
  45afb0: e28d2010     	add	r2, sp, #16
  45afb4: e1a01000     	mov	r1, r0
  45afb8: e5220008     	str	r0, [r2, #-0x8]!
  45afbc: e2850008     	add	r0, r5, #8
  45afc0: ebfffdaa     	bl	0x45a670 <std::allocator<ScriptManager::ScriptCmds>::_M_allocate(unsigned int, unsigned int&)> @ imm = #-0x958
  45afc4: e595c000     	ldr	r12, [r5]
  45afc8: e1a08000     	mov	r8, r0
  45afcc: e06c3004     	rsb	r3, r12, r4
  45afd0: e1a03143     	asr	r3, r3, #2
  45afd4: e083a103     	add	r10, r3, r3, lsl #2
  45afd8: e08aa20a     	add	r10, r10, r10, lsl #4
  45afdc: e08aa40a     	add	r10, r10, r10, lsl #8
  45afe0: e08aa80a     	add	r10, r10, r10, lsl #16
  45afe4: e083a08a     	add	r10, r3, r10, lsl #1
  45afe8: e35a0000     	cmp	r10, #0
  45afec: d1a0a000     	movle	r10, r0
  45aff0: da000010     	ble	0x45b038 <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::_M_fill_insert(ScriptManager::ScriptCmds*, unsigned int, ScriptManager::ScriptCmds const&)+0xd4> @ imm = #0x40
  45aff4: e1a0000a     	mov	r0, r10
  45aff8: e3a03000     	mov	r3, #0
  45affc: e79c2003     	ldr	r2, [r12, r3]
  45b000: e08c1003     	add	r1, r12, r3
  45b004: e2811004     	add	r1, r1, #4
  45b008: e7882003     	str	r2, [r8, r3]
  45b00c: e4919004     	ldr	r9, [r1], #4
  45b010: e0882003     	add	r2, r8, r3
  45b014: e2822004     	add	r2, r2, #4
  45b018: e4829004     	str	r9, [r2], #4
  45b01c: e5911000     	ldr	r1, [r1]
  45b020: e2500001     	subs	r0, r0, #1
  45b024: e283300c     	add	r3, r3, #12
  45b028: e5821000     	str	r1, [r2]
  45b02c: 1afffff2     	bne	0x45affc <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::_M_fill_insert(ScriptManager::ScriptCmds*, unsigned int, ScriptManager::ScriptCmds const&)+0x98> @ imm = #-0x38
  45b030: e3a0300c     	mov	r3, #12
  45b034: e02a8a93     	mla	r10, r3, r10, r8
  45b038: e3570001     	cmp	r7, #1
  45b03c: 0a000067     	beq	0x45b1e0 <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::_M_fill_insert(ScriptManager::ScriptCmds*, unsigned int, ScriptManager::ScriptCmds const&)+0x27c> @ imm = #0x19c
  45b040: e3a0300c     	mov	r3, #12
  45b044: e027a793     	mla	r7, r3, r7, r10
  45b048: e06a3007     	rsb	r3, r10, r7
  45b04c: e1a03143     	asr	r3, r3, #2
  45b050: e0831103     	add	r1, r3, r3, lsl #2
  45b054: e0811201     	add	r1, r1, r1, lsl #4
  45b058: e0811401     	add	r1, r1, r1, lsl #8
  45b05c: e0811801     	add	r1, r1, r1, lsl #16
  45b060: e0831081     	add	r1, r3, r1, lsl #1
  45b064: e3510000     	cmp	r1, #0
  45b068: da00000d     	ble	0x45b0a4 <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::_M_fill_insert(ScriptManager::ScriptCmds*, unsigned int, ScriptManager::ScriptCmds const&)+0x140> @ imm = #0x34
  45b06c: e286c004     	add	r12, r6, #4
  45b070: e3a02000     	mov	r2, #0
  45b074: e28c9004     	add	r9, r12, #4
  45b078: e5960000     	ldr	r0, [r6]
  45b07c: e08a3002     	add	r3, r10, r2
  45b080: e2833004     	add	r3, r3, #4
  45b084: e78a0002     	str	r0, [r10, r2]
  45b088: e59c0000     	ldr	r0, [r12]
  45b08c: e2511001     	subs	r1, r1, #1
  45b090: e282200c     	add	r2, r2, #12
  45b094: e4830004     	str	r0, [r3], #4
  45b098: e5990000     	ldr	r0, [r9]
  45b09c: e5830000     	str	r0, [r3]
  45b0a0: 1afffff4     	bne	0x45b078 <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::_M_fill_insert(ScriptManager::ScriptCmds*, unsigned int, ScriptManager::ScriptCmds const&)+0x114> @ imm = #-0x30
  45b0a4: e5953004     	ldr	r3, [r5, #0x4]
  45b0a8: e0642003     	rsb	r2, r4, r3
  45b0ac: e1a02142     	asr	r2, r2, #2
  45b0b0: e0826102     	add	r6, r2, r2, lsl #2
  45b0b4: e0866206     	add	r6, r6, r6, lsl #4
  45b0b8: e0866406     	add	r6, r6, r6, lsl #8
  45b0bc: e0866806     	add	r6, r6, r6, lsl #16
  45b0c0: e0826086     	add	r6, r2, r6, lsl #1
  45b0c4: e3560000     	cmp	r6, #0
  45b0c8: da000011     	ble	0x45b114 <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::_M_fill_insert(ScriptManager::ScriptCmds*, unsigned int, ScriptManager::ScriptCmds const&)+0x1b0> @ imm = #0x44
  45b0cc: e1a00006     	mov	r0, r6
  45b0d0: e3a03000     	mov	r3, #0
  45b0d4: e7942003     	ldr	r2, [r4, r3]
  45b0d8: e0841003     	add	r1, r4, r3
  45b0dc: e2811004     	add	r1, r1, #4
  45b0e0: e7872003     	str	r2, [r7, r3]
  45b0e4: e491c004     	ldr	r12, [r1], #4
  45b0e8: e0872003     	add	r2, r7, r3
  45b0ec: e2822004     	add	r2, r2, #4
  45b0f0: e482c004     	str	r12, [r2], #4
  45b0f4: e5911000     	ldr	r1, [r1]
  45b0f8: e2500001     	subs	r0, r0, #1
  45b0fc: e283300c     	add	r3, r3, #12
  45b100: e5821000     	str	r1, [r2]
  45b104: 1afffff2     	bne	0x45b0d4 <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::_M_fill_insert(ScriptManager::ScriptCmds*, unsigned int, ScriptManager::ScriptCmds const&)+0x170> @ imm = #-0x38
  45b108: e3a0300c     	mov	r3, #12
  45b10c: e0277693     	mla	r7, r3, r6, r7
  45b110: e5953004     	ldr	r3, [r5, #0x4]
  45b114: e5950000     	ldr	r0, [r5]
  45b118: e1500003     	cmp	r0, r3
  45b11c: 0a00000e     	beq	0x45b15c <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::_M_fill_insert(ScriptManager::ScriptCmds*, unsigned int, ScriptManager::ScriptCmds const&)+0x1f8> @ imm = #0x38
  45b120: e243200c     	sub	r2, r3, #12
  45b124: e0602002     	rsb	r2, r0, r2
  45b128: e1a02122     	lsr	r2, r2, #2
  45b12c: e0821102     	add	r1, r2, r2, lsl #2
  45b130: e0811281     	add	r1, r1, r1, lsl #5
  45b134: e0821081     	add	r1, r2, r1, lsl #1
  45b138: e0811281     	add	r1, r1, r1, lsl #5
  45b13c: e1a0c781     	lsl	r12, r1, #15
  45b140: e061100c     	rsb	r1, r1, r12
  45b144: e0822081     	add	r2, r2, r1, lsl #1
  45b148: e3c22103     	bic	r2, r2, #-1073741824
  45b14c: e3e0100b     	mvn	r1, #11
  45b150: e0020291     	mul	r2, r1, r2
  45b154: e0822001     	add	r2, r2, r1
  45b158: e0833002     	add	r3, r3, r2
  45b15c: e3530000     	cmp	r3, #0
  45b160: e5952008     	ldr	r2, [r5, #0x8]
  45b164: 0a00000b     	beq	0x45b198 <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::_M_fill_insert(ScriptManager::ScriptCmds*, unsigned int, ScriptManager::ScriptCmds const&)+0x234> @ imm = #0x2c
  45b168: e0633002     	rsb	r3, r3, r2
  45b16c: e1a03143     	asr	r3, r3, #2
  45b170: e0831103     	add	r1, r3, r3, lsl #2
  45b174: e0811201     	add	r1, r1, r1, lsl #4
  45b178: e0811401     	add	r1, r1, r1, lsl #8
  45b17c: e0811801     	add	r1, r1, r1, lsl #16
  45b180: e0833081     	add	r3, r3, r1, lsl #1
  45b184: e3a0100c     	mov	r1, #12
  45b188: e0010391     	mul	r1, r1, r3
  45b18c: e3510080     	cmp	r1, #128
  45b190: 8a00000b     	bhi	0x45b1c4 <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::_M_fill_insert(ScriptManager::ScriptCmds*, unsigned int, ScriptManager::ScriptCmds const&)+0x260> @ imm = #0x2c
  45b194: eb0ab759     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x2add64
  45b198: e59d3008     	ldr	r3, [sp, #0x8]
  45b19c: e3a0200c     	mov	r2, #12
  45b1a0: e5858000     	str	r8, [r5]
  45b1a4: e0288392     	mla	r8, r2, r3, r8
  45b1a8: e9850180     	stmib	r5, {r7, r8}
  45b1ac: e28dd010     	add	sp, sp, #16
  45b1b0: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  45b1b4: e28dc00c     	add	r12, sp, #12
  45b1b8: e58dc000     	str	r12, [sp]
  45b1bc: ebffeabf     	bl	0x455cc0 <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::_M_fill_insert_aux(ScriptManager::ScriptCmds*, unsigned int, ScriptManager::ScriptCmds const&, std::__false_type const&)> @ imm = #-0x5504
  45b1c0: eafffff9     	b	0x45b1ac <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::_M_fill_insert(ScriptManager::ScriptCmds*, unsigned int, ScriptManager::ScriptCmds const&)+0x248> @ imm = #-0x1c
  45b1c4: ebfad49d     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x14ad8c
  45b1c8: e59d3008     	ldr	r3, [sp, #0x8]
  45b1cc: e3a0200c     	mov	r2, #12
  45b1d0: e5858000     	str	r8, [r5]
  45b1d4: e0288392     	mla	r8, r2, r3, r8
  45b1d8: e9850180     	stmib	r5, {r7, r8}
  45b1dc: eafffff2     	b	0x45b1ac <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::_M_fill_insert(ScriptManager::ScriptCmds*, unsigned int, ScriptManager::ScriptCmds const&)+0x248> @ imm = #-0x38
  45b1e0: e1a02006     	mov	r2, r6
  45b1e4: e4921004     	ldr	r1, [r2], #4
  45b1e8: e1a0300a     	mov	r3, r10
  45b1ec: e28a700c     	add	r7, r10, #12
  45b1f0: e4831004     	str	r1, [r3], #4
  45b1f4: e5961004     	ldr	r1, [r6, #0x4]
  45b1f8: e58a1004     	str	r1, [r10, #0x4]
  45b1fc: e5922004     	ldr	r2, [r2, #0x4]
  45b200: e5832004     	str	r2, [r3, #0x4]
  45b204: eaffffa6     	b	0x45b0a4 <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::_M_fill_insert(ScriptManager::ScriptCmds*, unsigned int, ScriptManager::ScriptCmds const&)+0x140> @ imm = #-0x168

0045b208 <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::resize(unsigned int, ScriptManager::ScriptCmds const&)>:
  45b208: e92d0070     	push	{r4, r5, r6}
  45b20c: e5904004     	ldr	r4, [r0, #0x4]
  45b210: e5905000     	ldr	r5, [r0]
  45b214: e1a03002     	mov	r3, r2
  45b218: e0652004     	rsb	r2, r5, r4
  45b21c: e1a02142     	asr	r2, r2, #2
  45b220: e0826102     	add	r6, r2, r2, lsl #2
  45b224: e0866206     	add	r6, r6, r6, lsl #4
  45b228: e0866406     	add	r6, r6, r6, lsl #8
  45b22c: e0866806     	add	r6, r6, r6, lsl #16
  45b230: e0822086     	add	r2, r2, r6, lsl #1
  45b234: e1510002     	cmp	r1, r2
  45b238: 2a000005     	bhs	0x45b254 <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::resize(unsigned int, ScriptManager::ScriptCmds const&)+0x4c> @ imm = #0x14
  45b23c: e3a0300c     	mov	r3, #12
  45b240: e0255193     	mla	r5, r3, r1, r5
  45b244: e1550004     	cmp	r5, r4
  45b248: 15805004     	strne	r5, [r0, #0x4]
  45b24c: e8bd0070     	pop	{r4, r5, r6}
  45b250: e12fff1e     	bx	lr
  45b254: e0622001     	rsb	r2, r2, r1
  45b258: e1a01004     	mov	r1, r4
  45b25c: e8bd0070     	pop	{r4, r5, r6}
  45b260: eaffff3f     	b	0x45af64 <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::_M_fill_insert(ScriptManager::ScriptCmds*, unsigned int, ScriptManager::ScriptCmds const&)> @ imm = #-0x304
