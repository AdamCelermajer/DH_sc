
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003ef18c <DBG_GetLoadingStepName(int)>:
  3ef18c: e3500000     	cmp	r0, #0
  3ef190: ba000003     	blt	0x3ef1a4 <DBG_GetLoadingStepName(int)+0x18> @ imm = #0xc
  3ef194: e59f3014     	ldr	r3, [pc, #0x14]         @ 0x3ef1b0 <DBG_GetLoadingStepName(int)+0x24>
  3ef198: e08f3003     	add	r3, pc, r3
  3ef19c: e7930100     	ldr	r0, [r3, r0, lsl #2]
  3ef1a0: e12fff1e     	bx	lr
  3ef1a4: e59f0008     	ldr	r0, [pc, #0x8]          @ 0x3ef1b4 <DBG_GetLoadingStepName(int)+0x28>
  3ef1a8: e08f0000     	add	r0, pc, r0
  3ef1ac: e12fff1e     	bx	lr
  3ef1b0: 14 78 56 00  	.word	0x00567814
  3ef1b4: b8 72 4d 00  	.word	0x004d72b8

003ef1b8 <Color255To01(Point3D<float>&)>:
  3ef1b8: e92d4010     	push	{r4, lr}
  3ef1bc: e3a01443     	mov	r1, #1124073472
  3ef1c0: e1a04000     	mov	r4, r0
  3ef1c4: e281187f     	add	r1, r1, #8323072
  3ef1c8: e5900000     	ldr	r0, [r0]
  3ef1cc: ebfc7eb0     	bl	0x30ec94 <.plt+0xf20>   @ imm = #-0xe0540
  3ef1d0: e3a01443     	mov	r1, #1124073472
  3ef1d4: e5840000     	str	r0, [r4]
  3ef1d8: e281187f     	add	r1, r1, #8323072
  3ef1dc: e5940004     	ldr	r0, [r4, #0x4]
  3ef1e0: ebfc7eab     	bl	0x30ec94 <.plt+0xf20>   @ imm = #-0xe0554
  3ef1e4: e3a01443     	mov	r1, #1124073472
  3ef1e8: e5840004     	str	r0, [r4, #0x4]
  3ef1ec: e281187f     	add	r1, r1, #8323072
  3ef1f0: e5940008     	ldr	r0, [r4, #0x8]
  3ef1f4: ebfc7ea6     	bl	0x30ec94 <.plt+0xf20>   @ imm = #-0xe0568
  3ef1f8: e5840008     	str	r0, [r4, #0x8]
  3ef1fc: e8bd8010     	pop	{r4, pc}

003ef200 <Level::_SpawnLoadingThread()>:
  3ef200: e12fff1e     	bx	lr

003ef204 <Level::_CheckLoadProcess()>:
  3ef204: e12fff1e     	bx	lr

003ef208 <Level::_ThreadLoadingProcess()>:
  3ef208: e12fff1e     	bx	lr

003ef20c <Level::LoadScriptFile()>:
  3ef20c: e3a00001     	mov	r0, #1
  3ef210: e12fff1e     	bx	lr

003ef214 <Level::_LoadResumeBatching()>:
  3ef214: e12fff1e     	bx	lr
