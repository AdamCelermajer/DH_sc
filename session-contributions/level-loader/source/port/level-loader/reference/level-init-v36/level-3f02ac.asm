
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f02ac <Level::AssignSteamToLoadDataFile(StreamBuffer&)>:
  3f02ac: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3f02b0: e1a06000     	mov	r6, r0
  3f02b4: e1a07001     	mov	r7, r1
  3f02b8: e3a00050     	mov	r0, #80
  3f02bc: e3a01000     	mov	r1, #0
  3f02c0: ebfc80aa     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0xdfd58
  3f02c4: e59f503c     	ldr	r5, [pc, #0x3c]         @ 0x3f0308 <Level::AssignSteamToLoadDataFile(StreamBuffer&)+0x5c>
  3f02c8: e59f303c     	ldr	r3, [pc, #0x3c]         @ 0x3f030c <Level::AssignSteamToLoadDataFile(StreamBuffer&)+0x60>
  3f02cc: e1a04000     	mov	r4, r0
  3f02d0: e08f5005     	add	r5, pc, r5
  3f02d4: e7953003     	ldr	r3, [r5, r3]
  3f02d8: e1a01007     	mov	r1, r7
  3f02dc: e2833008     	add	r3, r3, #8
  3f02e0: e4803008     	str	r3, [r0], #8
  3f02e4: ebfc9bfb     	bl	0x3172d8 <StreamBuffer::StreamBuffer(IStreamBase*)> @ imm = #-0xd9014
  3f02e8: e3a03000     	mov	r3, #0
  3f02ec: e5c43048     	strb	r3, [r4, #0x48]
  3f02f0: e5843038     	str	r3, [r4, #0x38]
  3f02f4: e584303c     	str	r3, [r4, #0x3c]
  3f02f8: e5843040     	str	r3, [r4, #0x40]
  3f02fc: e5843044     	str	r3, [r4, #0x44]
  3f0300: e5864140     	str	r4, [r6, #0x140]
  3f0304: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  3f0308: c0 47 5a 00  	.word	0x005a47c0
  3f030c: 20 22 00 00  	.word	0x00002220
