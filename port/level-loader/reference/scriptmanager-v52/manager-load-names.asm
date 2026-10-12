
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
