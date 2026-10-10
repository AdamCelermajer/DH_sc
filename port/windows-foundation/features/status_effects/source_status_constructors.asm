
.local-inputs/libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003c1b58 <_ZN16CharStateMachineC2Ev>:
  3c1b58: e59fc084     	ldr	r12, [pc, #0x84]        @ 0x3c1be4 <_ZN16CharStateMachineC2Ev+0x8c>
  3c1b5c: e52d4004     	str	r4, [sp, #-0x4]!
  3c1b60: e59f4080     	ldr	r4, [pc, #0x80]         @ 0x3c1be8 <_ZN16CharStateMachineC2Ev+0x90>
  3c1b64: e08fc00c     	add	r12, pc, r12
  3c1b68: e3a02000     	mov	r2, #0
  3c1b6c: e79c4004     	ldr	r4, [r12, r4]
  3c1b70: e1a01000     	mov	r1, r0
  3c1b74: e5802004     	str	r2, [r0, #0x4]
  3c1b78: e2844008     	add	r4, r4, #8
  3c1b7c: e5804000     	str	r4, [r0]
  3c1b80: e580200c     	str	r2, [r0, #0xc]
  3c1b84: e3e04000     	mvn	r4, #0
  3c1b88: e5e12008     	strb	r2, [r1, #0x8]!
  3c1b8c: e5801014     	str	r1, [r0, #0x14]
  3c1b90: e5804028     	str	r4, [r0, #0x28]
  3c1b94: e580205c     	str	r2, [r0, #0x5c]
  3c1b98: e5801010     	str	r1, [r0, #0x10]
  3c1b9c: e5802018     	str	r2, [r0, #0x18]
  3c1ba0: e5802020     	str	r2, [r0, #0x20]
  3c1ba4: e5802024     	str	r2, [r0, #0x24]
  3c1ba8: e5802060     	str	r2, [r0, #0x60]
  3c1bac: e580202c     	str	r2, [r0, #0x2c]
  3c1bb0: e5802030     	str	r2, [r0, #0x30]
  3c1bb4: e5802034     	str	r2, [r0, #0x34]
  3c1bb8: e5802038     	str	r2, [r0, #0x38]
  3c1bbc: e580203c     	str	r2, [r0, #0x3c]
  3c1bc0: e5802040     	str	r2, [r0, #0x40]
  3c1bc4: e5802044     	str	r2, [r0, #0x44]
  3c1bc8: e5802048     	str	r2, [r0, #0x48]
  3c1bcc: e580204c     	str	r2, [r0, #0x4c]
  3c1bd0: e5802050     	str	r2, [r0, #0x50]
  3c1bd4: e5802054     	str	r2, [r0, #0x54]
  3c1bd8: e5802058     	str	r2, [r0, #0x58]
  3c1bdc: e8bd0010     	ldm	sp!, {r4}
  3c1be0: e12fff1e     	bx	lr

003cebf0 <_ZN6CharAIC2Ev>:
  3cebf0: e59f314c     	ldr	r3, [pc, #0x14c]        @ 0x3ced44 <_ZN6CharAIC2Ev+0x154>
  3cebf4: e59f214c     	ldr	r2, [pc, #0x14c]        @ 0x3ced48 <_ZN6CharAIC2Ev+0x158>
  3cebf8: e92d4030     	push	{r4, r5, lr}
  3cebfc: e08f3003     	add	r3, pc, r3
  3cec00: e7932002     	ldr	r2, [r3, r2]
  3cec04: e1a04000     	mov	r4, r0
  3cec08: e3a01000     	mov	r1, #0
  3cec0c: e2822008     	add	r2, r2, #8
  3cec10: e5842000     	str	r2, [r4]
  3cec14: e59f2130     	ldr	r2, [pc, #0x130]        @ 0x3ced4c <_ZN6CharAIC2Ev+0x15c>
  3cec18: e3a00001     	mov	r0, #1
  3cec1c: e3e0c000     	mvn	r12, #0
  3cec20: e1a05004     	mov	r5, r4
  3cec24: e5c40055     	strb	r0, [r4, #0x55]
  3cec28: e5841008     	str	r1, [r4, #0x8]
  3cec2c: e584100c     	str	r1, [r4, #0xc]
  3cec30: e5c41018     	strb	r1, [r4, #0x18]
  3cec34: e584101c     	str	r1, [r4, #0x1c]
  3cec38: e5841020     	str	r1, [r4, #0x20]
  3cec3c: e5c41024     	strb	r1, [r4, #0x24]
  3cec40: e5841028     	str	r1, [r4, #0x28]
  3cec44: e5c4102c     	strb	r1, [r4, #0x2c]
  3cec48: e5841030     	str	r1, [r4, #0x30]
  3cec4c: e5841034     	str	r1, [r4, #0x34]
  3cec50: e584103c     	str	r1, [r4, #0x3c]
  3cec54: e5841040     	str	r1, [r4, #0x40]
  3cec58: e5841044     	str	r1, [r4, #0x44]
  3cec5c: e5c41049     	strb	r1, [r4, #0x49]
  3cec60: e5c4004a     	strb	r0, [r4, #0x4a]
  3cec64: e5c4004b     	strb	r0, [r4, #0x4b]
  3cec68: e5c4104c     	strb	r1, [r4, #0x4c]
  3cec6c: e5c4004d     	strb	r0, [r4, #0x4d]
  3cec70: e5841050     	str	r1, [r4, #0x50]
  3cec74: e5c40054     	strb	r0, [r4, #0x54]
  3cec78: e5841058     	str	r1, [r4, #0x58]
  3cec7c: e1a00004     	mov	r0, r4
  3cec80: e5841060     	str	r1, [r4, #0x60]
  3cec84: e584c010     	str	r12, [r4, #0x10]
  3cec88: e584c014     	str	r12, [r4, #0x14]
  3cec8c: e584c038     	str	r12, [r4, #0x38]
  3cec90: e5e5105c     	strb	r1, [r5, #0x5c]!
  3cec94: e5845068     	str	r5, [r4, #0x68]
  3cec98: e5845064     	str	r5, [r4, #0x64]
  3cec9c: e584106c     	str	r1, [r4, #0x6c]
  3ceca0: e5841080     	str	r1, [r4, #0x80]
  3ceca4: e5e0107c     	strb	r1, [r0, #0x7c]!
  3ceca8: e7935002     	ldr	r5, [r3, r2]
  3cecac: e1a02004     	mov	r2, r4
  3cecb0: e5840088     	str	r0, [r4, #0x88]
  3cecb4: e5840084     	str	r0, [r4, #0x84]
  3cecb8: e584108c     	str	r1, [r4, #0x8c]
  3cecbc: e5841098     	str	r1, [r4, #0x98]
  3cecc0: e28400ac     	add	r0, r4, #172
  3cecc4: e5e21094     	strb	r1, [r2, #0x94]!
  3cecc8: e58420a0     	str	r2, [r4, #0xa0]
  3ceccc: e58400b0     	str	r0, [r4, #0xb0]
  3cecd0: e584c0cc     	str	r12, [r4, #0xcc]
  3cecd4: e5c410d1     	strb	r1, [r4, #0xd1]
  3cecd8: e584209c     	str	r2, [r4, #0x9c]
  3cecdc: e58410a4     	str	r1, [r4, #0xa4]
  3cece0: e58400ac     	str	r0, [r4, #0xac]
  3cece4: e58410b4     	str	r1, [r4, #0xb4]
  3cece8: e58410b8     	str	r1, [r4, #0xb8]
  3cecec: e58410bc     	str	r1, [r4, #0xbc]
  3cecf0: e58410c0     	str	r1, [r4, #0xc0]
  3cecf4: e58410c4     	str	r1, [r4, #0xc4]
  3cecf8: e58410c8     	str	r1, [r4, #0xc8]
  3cecfc: e5c410d0     	strb	r1, [r4, #0xd0]
  3ced00: e5951018     	ldr	r1, [r5, #0x18]
  3ced04: e5952010     	ldr	r2, [r5, #0x10]
  3ced08: e24dd00c     	sub	sp, sp, #12
  3ced0c: e2413004     	sub	r3, r1, #4
  3ced10: e1520003     	cmp	r2, r3
  3ced14: e58d4004     	str	r4, [sp, #0x4]
  3ced18: 0a000006     	beq	0x3ced38 <_ZN6CharAIC2Ev+0x148> @ imm = #0x18
  3ced1c: e5824000     	str	r4, [r2]
  3ced20: e5953010     	ldr	r3, [r5, #0x10]
  3ced24: e2833004     	add	r3, r3, #4
  3ced28: e5853010     	str	r3, [r5, #0x10]
  3ced2c: e1a00004     	mov	r0, r4
  3ced30: e28dd00c     	add	sp, sp, #12
  3ced34: e8bd8030     	pop	{r4, r5, pc}
  3ced38: e28d0004     	add	r0, sp, #4
  3ced3c: ebfffeb3     	bl	0x3ce810 <_ZNSt5dequeIP6CharAISaIS1_EE18_M_push_back_aux_vERKS1_.clone.17> @ imm = #-0x534
  3ced40: eafffff9     	b	0x3ced2c <_ZN6CharAIC2Ev+0x13c> @ imm = #-0x1c

003d12b0 <_ZN6CharAI6OnInitEv>:
  3d12b0: e92d40f0     	push	{r4, r5, r6, r7, lr}
  3d12b4: e5903004     	ldr	r3, [r0, #0x4]
  3d12b8: e24dd00c     	sub	sp, sp, #12
  3d12bc: e1a04000     	mov	r4, r0
  3d12c0: e1a00003     	mov	r0, r3
  3d12c4: e5933000     	ldr	r3, [r3]
  3d12c8: e1a0e00f     	mov	lr, pc
  3d12cc: e593f034     	ldr	pc, [r3, #0x34]
  3d12d0: e59f50e8     	ldr	r5, [pc, #0xe8]         @ 0x3d13c0 <_ZN6CharAI6OnInitEv+0x110>
  3d12d4: e3500000     	cmp	r0, #0
  3d12d8: e08f5005     	add	r5, pc, r5
  3d12dc: 1a00002e     	bne	0x3d139c <_ZN6CharAI6OnInitEv+0xec> @ imm = #0xb8
  3d12e0: e5941010     	ldr	r1, [r4, #0x10]
  3d12e4: e3710001     	cmn	r1, #1
  3d12e8: 0a000002     	beq	0x3d12f8 <_ZN6CharAI6OnInitEv+0x48> @ imm = #0x8
  3d12ec: e5940004     	ldr	r0, [r4, #0x4]
  3d12f0: e2800fed     	add	r0, r0, #948
  3d12f4: eb0027f7     	bl	0x3db2d8 <_ZN10CharTimers8TMR_StopEj> @ imm = #0x9fdc
  3d12f8: e59f60c4     	ldr	r6, [pc, #0xc4]         @ 0x3d13c4 <_ZN6CharAI6OnInitEv+0x114>
  3d12fc: e59f10c4     	ldr	r1, [pc, #0xc4]         @ 0x3d13c8 <_ZN6CharAI6OnInitEv+0x118>
  3d1300: e59f20c4     	ldr	r2, [pc, #0xc4]         @ 0x3d13cc <_ZN6CharAI6OnInitEv+0x11c>
  3d1304: e7953006     	ldr	r3, [r5, r6]
  3d1308: e08f1001     	add	r1, pc, r1
  3d130c: e08f2002     	add	r2, pc, r2
  3d1310: e593002c     	ldr	r0, [r3, #0x2c]
  3d1314: e5947004     	ldr	r7, [r4, #0x4]
  3d1318: eb03ce2f     	bl	0x4c4bdc <_ZNK15PyDataConstants11getConstantEPKcS1_> @ imm = #0xf38bc
  3d131c: e2877fed     	add	r7, r7, #948
  3d1320: e1a01000     	mov	r1, r0
  3d1324: e3a0c000     	mov	r12, #0
  3d1328: e1a00007     	mov	r0, r7
  3d132c: e3e02000     	mvn	r2, #0
  3d1330: e3a03033     	mov	r3, #51
  3d1334: e58dc000     	str	r12, [sp]
  3d1338: eb002ab9     	bl	0x3dbe24 <_ZN10CharTimers9TMR_StartEjiiPv> @ imm = #0xaae4
  3d133c: e5941014     	ldr	r1, [r4, #0x14]
  3d1340: e5840010     	str	r0, [r4, #0x10]
  3d1344: e3710001     	cmn	r1, #1
  3d1348: 0a000002     	beq	0x3d1358 <_ZN6CharAI6OnInitEv+0xa8> @ imm = #0x8
  3d134c: e5940004     	ldr	r0, [r4, #0x4]
  3d1350: e2800fed     	add	r0, r0, #948
  3d1354: eb0027df     	bl	0x3db2d8 <_ZN10CharTimers8TMR_StopEj> @ imm = #0x9f7c
  3d1358: e7953006     	ldr	r3, [r5, r6]
  3d135c: e59f106c     	ldr	r1, [pc, #0x6c]         @ 0x3d13d0 <_ZN6CharAI6OnInitEv+0x120>
  3d1360: e59f206c     	ldr	r2, [pc, #0x6c]         @ 0x3d13d4 <_ZN6CharAI6OnInitEv+0x124>
  3d1364: e593002c     	ldr	r0, [r3, #0x2c]
  3d1368: e08f1001     	add	r1, pc, r1
  3d136c: e08f2002     	add	r2, pc, r2
  3d1370: e5945004     	ldr	r5, [r4, #0x4]
  3d1374: eb03ce18     	bl	0x4c4bdc <_ZNK15PyDataConstants11getConstantEPKcS1_> @ imm = #0xf3860
  3d1378: e2855fed     	add	r5, r5, #948
  3d137c: e1a01000     	mov	r1, r0
  3d1380: e3a0c000     	mov	r12, #0
  3d1384: e1a00005     	mov	r0, r5
  3d1388: e3e02000     	mvn	r2, #0
  3d138c: e3a03034     	mov	r3, #52
  3d1390: e58dc000     	str	r12, [sp]
  3d1394: eb002aa2     	bl	0x3dbe24 <_ZN10CharTimers9TMR_StartEjiiPv> @ imm = #0xaa88
  3d1398: e5840014     	str	r0, [r4, #0x14]
  3d139c: e5943020     	ldr	r3, [r4, #0x20]
  3d13a0: e3530000     	cmp	r3, #0
  3d13a4: 0a000003     	beq	0x3d13b8 <_ZN6CharAI6OnInitEv+0x108> @ imm = #0xc
  3d13a8: e1a00003     	mov	r0, r3
  3d13ac: e5933000     	ldr	r3, [r3]
  3d13b0: e1a0e00f     	mov	lr, pc
  3d13b4: e593f008     	ldr	pc, [r3, #0x8]
  3d13b8: e28dd00c     	add	sp, sp, #12
  3d13bc: e8bd80f0     	pop	{r4, r5, r6, r7, pc}

003dba84 <_ZNSt6vectorIN10CharTimers6_TimerESaIS1_EE7reserveEj.clone.2>:
  3dba84: e92d4070     	push	{r4, r5, r6, lr}
  3dba88: e5902000     	ldr	r2, [r0]
  3dba8c: e5903008     	ldr	r3, [r0, #0x8]
  3dba90: e24dd008     	sub	sp, sp, #8
  3dba94: e3a01014     	mov	r1, #20
  3dba98: e0623003     	rsb	r3, r2, r3
  3dba9c: e1a032c3     	asr	r3, r3, #5
  3dbaa0: e3530013     	cmp	r3, #19
  3dbaa4: e1a04000     	mov	r4, r0
  3dbaa8: e58d1004     	str	r1, [sp, #0x4]
  3dbaac: 8a00000f     	bhi	0x3dbaf0 <_ZNSt6vectorIN10CharTimers6_TimerESaIS1_EE7reserveEj.clone.2+0x6c> @ imm = #0x3c
  3dbab0: e5903004     	ldr	r3, [r0, #0x4]
  3dbab4: e3520000     	cmp	r2, #0
  3dbab8: e0625003     	rsb	r5, r2, r3
  3dbabc: e1a052c5     	asr	r5, r5, #5
  3dbac0: 0a00000c     	beq	0x3dbaf8 <_ZNSt6vectorIN10CharTimers6_TimerESaIS1_EE7reserveEj.clone.2+0x74> @ imm = #0x30
  3dbac4: e28d1004     	add	r1, sp, #4
  3dbac8: ebffffc3     	bl	0x3db9dc <_ZNSt6vectorIN10CharTimers6_TimerESaIS1_EE20_M_allocate_and_copyIPS1_EES5_RjT_S7_> @ imm = #-0xf4
  3dbacc: e1a06000     	mov	r6, r0
  3dbad0: e1a00004     	mov	r0, r4
  3dbad4: ebffff72     	bl	0x3db8a4 <_ZNSt6vectorIN10CharTimers6_TimerESaIS1_EE8_M_clearEv> @ imm = #-0x238
  3dbad8: e59d3004     	ldr	r3, [sp, #0x4]
  3dbadc: e0865285     	add	r5, r6, r5, lsl #5
  3dbae0: e5845004     	str	r5, [r4, #0x4]
  3dbae4: e0863283     	add	r3, r6, r3, lsl #5
  3dbae8: e5843008     	str	r3, [r4, #0x8]
  3dbaec: e5846000     	str	r6, [r4]
  3dbaf0: e28dd008     	add	sp, sp, #8
  3dbaf4: e8bd8070     	pop	{r4, r5, r6, pc}
  3dbaf8: e2800008     	add	r0, r0, #8
  3dbafc: e28d2004     	add	r2, sp, #4
  3dbb00: ebffff99     	bl	0x3db96c <_ZNSaIN10CharTimers6_TimerEE11_M_allocateEjRj> @ imm = #-0x19c
  3dbb04: e1a06000     	mov	r6, r0
  3dbb08: eafffff2     	b	0x3dbad8 <_ZNSt6vectorIN10CharTimers6_TimerESaIS1_EE7reserveEj.clone.2+0x54> @ imm = #-0x38

003dbb0c <_ZN10CharTimersC2Ev>:
  3dbb0c: e59f303c     	ldr	r3, [pc, #0x3c]         @ 0x3dbb50 <_ZN10CharTimersC2Ev+0x44>
  3dbb10: e59f203c     	ldr	r2, [pc, #0x3c]         @ 0x3dbb54 <_ZN10CharTimersC2Ev+0x48>
  3dbb14: e3a01000     	mov	r1, #0
  3dbb18: e08f3003     	add	r3, pc, r3
  3dbb1c: e7932002     	ldr	r2, [r3, r2]
  3dbb20: e92d4010     	push	{r4, lr}
  3dbb24: e2822008     	add	r2, r2, #8
  3dbb28: e1a04000     	mov	r4, r0
  3dbb2c: e5801010     	str	r1, [r0, #0x10]
  3dbb30: e5802000     	str	r2, [r0]
  3dbb34: e5801004     	str	r1, [r0, #0x4]
  3dbb38: e5801008     	str	r1, [r0, #0x8]
  3dbb3c: e580100c     	str	r1, [r0, #0xc]
  3dbb40: e2800008     	add	r0, r0, #8
  3dbb44: ebffffce     	bl	0x3dba84 <_ZNSt6vectorIN10CharTimers6_TimerESaIS1_EE7reserveEj.clone.2> @ imm = #-0xc8
  3dbb48: e1a00004     	mov	r0, r4
  3dbb4c: e8bd8010     	pop	{r4, pc}

003def34 <_ZN14CharProperties16_ResetPropertiesERN7Structs19CharacterPropertiesE>:
  3def34: e59f3040     	ldr	r3, [pc, #0x40]         @ 0x3def7c <_ZN14CharProperties16_ResetPropertiesERN7Structs19CharacterPropertiesE+0x48>
  3def38: e59f2040     	ldr	r2, [pc, #0x40]         @ 0x3def80 <_ZN14CharProperties16_ResetPropertiesERN7Structs19CharacterPropertiesE+0x4c>
  3def3c: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3def40: e08f3003     	add	r3, pc, r3
  3def44: e1a08000     	mov	r8, r0
  3def48: e7937002     	ldr	r7, [r3, r2]
  3def4c: e1a06001     	mov	r6, r1
  3def50: e3a04000     	mov	r4, #0
  3def54: e1a01004     	mov	r1, r4
  3def58: e1a00008     	mov	r0, r8
  3def5c: e7975104     	ldr	r5, [r7, r4, lsl #2]
  3def60: ebffffea     	bl	0x3def10 <_ZNK14CharProperties11_GetDefaultEi> @ imm = #-0x58
  3def64: e2844001     	add	r4, r4, #1
  3def68: e2855004     	add	r5, r5, #4
  3def6c: e35400e0     	cmp	r4, #224
  3def70: e7860005     	str	r0, [r6, r5]
  3def74: 1afffff6     	bne	0x3def54 <_ZN14CharProperties16_ResetPropertiesERN7Structs19CharacterPropertiesE+0x20> @ imm = #-0x28
  3def78: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}

003defc4 <_ZN14CharProperties18ResetAllPropertiesEv>:
  3defc4: e92d4010     	push	{r4, lr}
  3defc8: e1a04000     	mov	r4, r0
  3defcc: ebfffffa     	bl	0x3defbc <_ZN14CharProperties19ResetBasePropertiesEv> @ imm = #-0x18
  3defd0: e1a00004     	mov	r0, r4
  3defd4: ebfffff6     	bl	0x3defb4 <_ZN14CharProperties20ResetSavedPropertiesEv> @ imm = #-0x28
  3defd8: e1a00004     	mov	r0, r4
  3defdc: ebfffff2     	bl	0x3defac <_ZN14CharProperties20ResetGearsPropertiesEv> @ imm = #-0x38
  3defe0: e2841ea9     	add	r1, r4, #2704
  3defe4: e1a00004     	mov	r0, r4
  3defe8: e2811004     	add	r1, r1, #4
  3defec: e8bd4010     	pop	{r4, lr}
  3deff0: eaffffcf     	b	0x3def34 <_ZN14CharProperties16_ResetPropertiesERN7Structs19CharacterPropertiesE> @ imm = #-0xc4

003df084 <_ZN14CharPropertiesC2Ev>:
  3df084: e92d4070     	push	{r4, r5, r6, lr}
  3df088: e59f5074     	ldr	r5, [pc, #0x74]         @ 0x3df104 <_ZN14CharPropertiesC2Ev+0x80>
  3df08c: e59f3074     	ldr	r3, [pc, #0x74]         @ 0x3df108 <_ZN14CharPropertiesC2Ev+0x84>
  3df090: e59f1074     	ldr	r1, [pc, #0x74]         @ 0x3df10c <_ZN14CharPropertiesC2Ev+0x88>
  3df094: e08f5005     	add	r5, pc, r5
  3df098: e7953003     	ldr	r3, [r5, r3]
  3df09c: e7951001     	ldr	r1, [r5, r1]
  3df0a0: e2802ee1     	add	r2, r0, #3600
  3df0a4: e2822008     	add	r2, r2, #8
  3df0a8: e2811008     	add	r1, r1, #8
  3df0ac: e283c008     	add	r12, r3, #8
  3df0b0: e3a03000     	mov	r3, #0
  3df0b4: e580c000     	str	r12, [r0]
  3df0b8: e5802e24     	str	r2, [r0, #0xe24]
  3df0bc: e5802e20     	str	r2, [r0, #0xe20]
  3df0c0: e5801a94     	str	r1, [r0, #0xa94]
  3df0c4: e5803e30     	str	r3, [r0, #0xe30]
  3df0c8: e5803004     	str	r3, [r0, #0x4]
  3df0cc: e5801008     	str	r1, [r0, #0x8]
  3df0d0: e580138c     	str	r1, [r0, #0x38c]
  3df0d4: e5801710     	str	r1, [r0, #0x710]
  3df0d8: e5803e1c     	str	r3, [r0, #0xe1c]
  3df0dc: e5c03e18     	strb	r3, [r0, #0xe18]
  3df0e0: e5803e28     	str	r3, [r0, #0xe28]
  3df0e4: e1a04000     	mov	r4, r0
  3df0e8: ebffffb5     	bl	0x3defc4 <_ZN14CharProperties18ResetAllPropertiesEv> @ imm = #-0x12c
  3df0ec: e59f301c     	ldr	r3, [pc, #0x1c]         @ 0x3df110 <_ZN14CharPropertiesC2Ev+0x8c>
  3df0f0: e1a00004     	mov	r0, r4
  3df0f4: e7951003     	ldr	r1, [r5, r3]
  3df0f8: ebffff8d     	bl	0x3def34 <_ZN14CharProperties16_ResetPropertiesERN7Structs19CharacterPropertiesE> @ imm = #-0x1cc
  3df0fc: e1a00004     	mov	r0, r4
  3df100: e8bd8070     	pop	{r4, r5, r6, pc}
