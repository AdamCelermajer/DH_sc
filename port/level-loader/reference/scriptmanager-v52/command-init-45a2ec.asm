
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0045a2ec <Script_PlayActorAnim::Init()>:
  45a2ec: e92d4070     	push	{r4, r5, r6, lr}
  45a2f0: e59fc084     	ldr	r12, [pc, #0x84]        @ 0x45a37c <Script_PlayActorAnim::Init()+0x90>
  45a2f4: e59f3084     	ldr	r3, [pc, #0x84]         @ 0x45a380 <Script_PlayActorAnim::Init()+0x94>
  45a2f8: e590500c     	ldr	r5, [r0, #0xc]
  45a2fc: e08fc00c     	add	r12, pc, r12
  45a300: e79c3003     	ldr	r3, [r12, r3]
  45a304: e24dd018     	sub	sp, sp, #24
  45a308: e28d600c     	add	r6, sp, #12
  45a30c: e5931038     	ldr	r1, [r3, #0x38]
  45a310: e5952018     	ldr	r2, [r5, #0x18]
  45a314: e3a04000     	mov	r4, #0
  45a318: e3e03000     	mvn	r3, #0
  45a31c: e1a00006     	mov	r0, r6
  45a320: e58d4000     	str	r4, [sp]
  45a324: e58d4004     	str	r4, [sp, #0x4]
  45a328: ebfbc25c     	bl	0x34aca0 <ObjectManager::GetObjectByName(char const*, int, bool, char const*)> @ imm = #-0x10f690
  45a32c: e1a00006     	mov	r0, r6
  45a330: e1a01004     	mov	r1, r4
  45a334: ebfb96a1     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0x11a57c
  45a338: e1500004     	cmp	r0, r4
  45a33c: 1a000001     	bne	0x45a348 <Script_PlayActorAnim::Init()+0x5c> @ imm = #0x4
  45a340: e28dd018     	add	sp, sp, #24
  45a344: e8bd8070     	pop	{r4, r5, r6, pc}
  45a348: e1a00006     	mov	r0, r6
  45a34c: ebfb9700     	bl	0x33ff54 <ObjectHandle::operator Character*()> @ imm = #-0x11a400
  45a350: e2504000     	subs	r4, r0, #0
  45a354: 0afffff9     	beq	0x45a340 <Script_PlayActorAnim::Init()+0x54> @ imm = #-0x1c
  45a358: e2844e49     	add	r4, r4, #1168
  45a35c: e284400c     	add	r4, r4, #12
  45a360: e5951008     	ldr	r1, [r5, #0x8]
  45a364: e1a00004     	mov	r0, r4
  45a368: ebfdbde9     	bl	0x3c9b14 <CharAnimator::ANIM_AddAnimDictToSet(int)> @ imm = #-0x9085c
  45a36c: e1a00004     	mov	r0, r4
  45a370: e595100c     	ldr	r1, [r5, #0xc]
  45a374: ebfdbde6     	bl	0x3c9b14 <CharAnimator::ANIM_AddAnimDictToSet(int)> @ imm = #-0x90868
  45a378: eafffff0     	b	0x45a340 <Script_PlayActorAnim::Init()+0x54> @ imm = #-0x40
  45a37c: 94 a7 53 00  	.word	0x0053a794
  45a380: f4 37 00 00  	.word	0x000037f4
