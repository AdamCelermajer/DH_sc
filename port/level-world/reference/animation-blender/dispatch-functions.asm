
# _ZN6glitch7collada18ISceneNodeAnimator19setCompatibleTargetERKNS0_8SChannelEPv
00669574: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00669578: ldr      r3, [r1, #8]
0066957c: ldr      r5, [pc, #0x1a0]
00669580: sub      sp, sp, #0x14
00669584: str      r2, [sp, #0xc]
00669588: cmp      r3, #0xe
0066958c: add      r5, pc, r5
00669590: mov      r6, r1
00669594: mov      r4, r0
00669598: ldr      r8, [r1, #4]
0066959c: beq      #0x669690
006695a0: ldr      r3, [r0]
006695a4: mov      lr, pc
006695a8: ldr      pc, [r3, #0x70]
006695ac: subs     sl, r0, #0
006695b0: ble      #0x669684
006695b4: ldr      r3, [pc, #0x16c]
006695b8: ldr      sb, [pc, #0x16c]
006695bc: mov      r7, #0
006695c0: add      r3, pc, r3
006695c4: str      r3, [sp, #8]
006695c8: mov      fp, #0xc
006695cc: b        #0x6695dc
006695d0: add      r7, r7, #1
006695d4: cmp      r7, sl
006695d8: beq      #0x669684
006695dc: mov      r1, r7
006695e0: ldr      r3, [r4]
006695e4: mov      r0, r4
006695e8: mov      lr, pc
006695ec: ldr      pc, [r3, #0x6c]
006695f0: mov      r1, r8
006695f4: bl       #0x30e31c
006695f8: cmp      r0, #0
006695fc: bne      #0x6695d0
00669600: mov      r1, r7
00669604: ldr      r3, [r4]
00669608: mov      r0, r4
0066960c: mov      lr, pc
00669610: ldr      pc, [r3, #0x54]
00669614: ldr      r1, [r5, sb]
00669618: ldr      r3, [r6, #8]
0066961c: ldr      r2, [r0, #8]
00669620: ldr      r1, [r1]
00669624: cmp      r3, #0x5b
00669628: mla      r2, fp, r2, r1
0066962c: bls      #0x669648
00669630: ldr      r0, [sp, #8]
00669634: str      r2, [sp, #4]
00669638: str      r3, [sp]
0066963c: bl       #0x708eb0
00669640: ldr      r3, [sp]
00669644: ldr      r2, [sp, #4]
00669648: lsr      r1, r3, #5
0066964c: ldr      r2, [r2, r1, lsl #2]
00669650: and      r3, r3, #0x1f
00669654: mov      r1, #1
00669658: ands     r2, r2, r1, lsl r3
0066965c: beq      #0x6695d0
00669660: mov      r0, r4
00669664: mov      r1, r7
00669668: ldr      r2, [sp, #0xc]
0066966c: ldr      ip, [r4]
00669670: mov      r3, #0
00669674: mov      lr, pc
00669678: ldr      pc, [ip, #0x68]
0066967c: mov      r0, #1
00669680: b        #0x669688
00669684: mov      r0, #0
00669688: add      sp, sp, #0x14
0066968c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00669690: ldr      r3, [r0]
00669694: ldrb     sl, [r1, #0xc]
00669698: mov      lr, pc
0066969c: ldr      pc, [r3, #0x70]
006696a0: subs     r7, r0, #0
006696a4: ble      #0x669684
006696a8: mov      r6, #0
006696ac: b        #0x6696bc
006696b0: add      r6, r6, #1
006696b4: cmp      r6, r7
006696b8: beq      #0x669684
006696bc: mov      r1, r6
006696c0: ldr      r3, [r4]
006696c4: mov      r0, r4
006696c8: mov      lr, pc
006696cc: ldr      pc, [r3, #0x6c]
006696d0: mov      r1, r8
006696d4: bl       #0x30e31c
006696d8: subs     r5, r0, #0
006696dc: bne      #0x6696b0
006696e0: ldr      r3, [r4]
006696e4: mov      r1, r6
006696e8: mov      r0, r4
006696ec: mov      lr, pc
006696f0: ldr      pc, [r3, #0x54]
006696f4: ldrb     r3, [r0, #0xc]
006696f8: cmp      r3, sl
006696fc: bne      #0x6696b0
00669700: mov      r0, r4
00669704: mov      r1, r6
00669708: ldr      r2, [sp, #0xc]
0066970c: mov      r3, r5
00669710: ldr      ip, [r4]
00669714: mov      lr, pc
00669718: ldr      pc, [ip, #0x68]
0066971c: mov      r0, #1
00669720: b        #0x669688
00669724: eorseq   fp, r2, r4, lsl #10
00669728: eoreq    r8, r5, r8, lsl #14
0066972c: andeq    r4, r0, ip, asr #10

# _ZN11AnimatorSet20applyAnimationValuesEj
0036737c: ldr      r3, [r0, #0x98]
00367380: cmp      r3, #0
00367384: ldrne    r3, [r3, #0x20]
00367388: str      r3, [r0, #0x50]
0036738c: b        #0x65f418

# _ZN11AnimatorSet22computeAnimationValuesEj
00367368: ldr      r3, [r0, #0x98]
0036736c: cmp      r3, #0
00367370: ldrne    r3, [r3, #0x20]
00367374: str      r3, [r0, #0x50]
00367378: b        #0x65f5dc

# _ZN15AnimatorBlender7compileEPSt6vectorIhN6glitch4core10SAllocatorIhLNS1_6memory13E_MEMORY_HINTE0EEEE
00366764: b        #0x65ed98
