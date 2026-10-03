
# _ZN6glitch4core8CMatrix4IfEC1ERKS2_NS2_12eConstructorE.clone.0
00586a14: mov      r3, #0
00586a18: push     {r4, lr}
00586a1c: mov      r2, #0x41
00586a20: mov      r4, r0
00586a24: strb     r3, [r0, #0x40]
00586a28: bl       #0x30e868
00586a2c: mov      r0, r4
00586a30: pop      {r4, pc}

# _ZNK6glitch4core8CMatrix4IfE12transformBoxERNS0_8aabbox3dIfEE
00587398: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0058739c: ldrb     r3, [r0, #0x40]
005873a0: mov      r4, r0
005873a4: mov      r5, r1
005873a8: cmp      r3, #0
005873ac: bne      #0x587600
005873b0: ldr      sl, [r1]
005873b4: ldr      r7, [r1, #4]
005873b8: ldr      r1, [r0]
005873bc: mov      r0, sl
005873c0: bl       #0x30ed6c
005873c4: ldr      r1, [r4, #0x10]
005873c8: mov      r8, r0
005873cc: mov      r0, r7
005873d0: bl       #0x30ed6c
005873d4: mov      r1, r0
005873d8: mov      r0, r8
005873dc: bl       #0x30eba4
005873e0: ldr      r6, [r5, #8]
005873e4: ldr      r1, [r4, #0x20]
005873e8: mov      r8, r0
005873ec: mov      r0, r6
005873f0: bl       #0x30ed6c
005873f4: mov      r1, r0
005873f8: mov      r0, r8
005873fc: bl       #0x30eba4
00587400: ldr      r1, [r4, #0x30]
00587404: bl       #0x30eba4
00587408: ldr      r1, [r4, #4]
0058740c: mov      sb, r0
00587410: mov      r0, sl
00587414: bl       #0x30ed6c
00587418: ldr      r1, [r4, #0x14]
0058741c: mov      r8, r0
00587420: mov      r0, r7
00587424: bl       #0x30ed6c
00587428: mov      r1, r0
0058742c: mov      r0, r8
00587430: bl       #0x30eba4
00587434: ldr      r1, [r4, #0x24]
00587438: mov      r8, r0
0058743c: mov      r0, r6
00587440: bl       #0x30ed6c
00587444: mov      r1, r0
00587448: mov      r0, r8
0058744c: bl       #0x30eba4
00587450: ldr      r1, [r4, #0x34]
00587454: bl       #0x30eba4
00587458: ldr      r1, [r4, #8]
0058745c: mov      r8, r0
00587460: mov      r0, sl
00587464: bl       #0x30ed6c
00587468: ldr      r1, [r4, #0x18]
0058746c: mov      sl, r0
00587470: mov      r0, r7
00587474: bl       #0x30ed6c
00587478: mov      r1, r0
0058747c: mov      r0, sl
00587480: bl       #0x30eba4
00587484: ldr      r1, [r4, #0x28]
00587488: mov      r7, r0
0058748c: mov      r0, r6
00587490: bl       #0x30ed6c
00587494: mov      r1, r0
00587498: mov      r0, r7
0058749c: bl       #0x30eba4
005874a0: ldr      r1, [r4, #0x38]
005874a4: bl       #0x30eba4
005874a8: str      sb, [r5]
005874ac: ldr      r6, [r5, #0xc]
005874b0: str      r0, [r5, #8]
005874b4: str      r8, [r5, #4]
005874b8: ldr      r1, [r4]
005874bc: mov      r7, r0
005874c0: mov      r0, r6
005874c4: bl       #0x30ed6c
005874c8: ldr      r1, [r4, #0x10]
005874cc: mov      sl, r0
005874d0: ldr      r0, [r5, #0x10]
005874d4: bl       #0x30ed6c
005874d8: mov      r1, r0
005874dc: mov      r0, sl
005874e0: bl       #0x30eba4
005874e4: ldr      r1, [r4, #0x20]
005874e8: mov      sl, r0
005874ec: ldr      r0, [r5, #0x14]
005874f0: bl       #0x30ed6c
005874f4: mov      r1, r0
005874f8: mov      r0, sl
005874fc: bl       #0x30eba4
00587500: ldr      r1, [r4, #0x30]
00587504: bl       #0x30eba4
00587508: ldr      r1, [r4, #4]
0058750c: mov      fp, r0
00587510: mov      r0, r6
00587514: bl       #0x30ed6c
00587518: ldr      r1, [r4, #0x14]
0058751c: mov      sl, r0
00587520: ldr      r0, [r5, #0x10]
00587524: bl       #0x30ed6c
00587528: mov      r1, r0
0058752c: mov      r0, sl
00587530: bl       #0x30eba4
00587534: ldr      r1, [r4, #0x24]
00587538: mov      sl, r0
0058753c: ldr      r0, [r5, #0x14]
00587540: bl       #0x30ed6c
00587544: mov      r1, r0
00587548: mov      r0, sl
0058754c: bl       #0x30eba4
00587550: ldr      r1, [r4, #0x34]
00587554: bl       #0x30eba4
00587558: ldr      r1, [r4, #8]
0058755c: mov      sl, r0
00587560: mov      r0, r6
00587564: bl       #0x30ed6c
00587568: ldr      r1, [r4, #0x18]
0058756c: mov      r6, r0
00587570: ldr      r0, [r5, #0x10]
00587574: bl       #0x30ed6c
00587578: mov      r1, r0
0058757c: mov      r0, r6
00587580: bl       #0x30eba4
00587584: ldr      r1, [r4, #0x28]
00587588: mov      r6, r0
0058758c: ldr      r0, [r5, #0x14]
00587590: bl       #0x30ed6c
00587594: mov      r1, r0
00587598: mov      r0, r6
0058759c: bl       #0x30eba4
005875a0: ldr      r1, [r4, #0x38]
005875a4: bl       #0x30eba4
005875a8: str      fp, [r5, #0xc]
005875ac: mov      r4, r0
005875b0: str      r0, [r5, #0x14]
005875b4: mov      r1, fp
005875b8: str      sl, [r5, #0x10]
005875bc: mov      r0, sb
005875c0: bl       #0x30e2f8
005875c4: cmp      r0, #0
005875c8: strne    fp, [r5]
005875cc: strne    sb, [r5, #0xc]
005875d0: mov      r1, sl
005875d4: mov      r0, r8
005875d8: bl       #0x30e2f8
005875dc: cmp      r0, #0
005875e0: strne    sl, [r5, #4]
005875e4: strne    r8, [r5, #0x10]
005875e8: mov      r0, r7
005875ec: mov      r1, r4
005875f0: bl       #0x30e2f8
005875f4: cmp      r0, #0
005875f8: strne    r7, [r5, #0x14]
005875fc: strne    r4, [r5, #8]
00587600: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch4core6detail12CMatrix4BaseIfE6multEqERKS3_
0050f6d8: push     {r4, r5, r6, lr}
0050f6dc: ldrb     r3, [r1, #0x40]
0050f6e0: sub      sp, sp, #0x48
0050f6e4: mov      r5, r1
0050f6e8: cmp      r3, #0
0050f6ec: mov      r4, r0
0050f6f0: bne      #0x50f728
0050f6f4: ldrb     r3, [r0, #0x40]
0050f6f8: cmp      r3, #0
0050f6fc: bne      #0x50f734
0050f700: add      r6, sp, #4
0050f704: mov      r1, r0
0050f708: mov      r2, #0x41
0050f70c: mov      r0, r6
0050f710: bl       #0x30e868
0050f714: mov      r0, r4
0050f718: mov      r1, r6
0050f71c: mov      r2, r5
0050f720: bl       #0x40ea54
0050f724: mov      r4, r0
0050f728: mov      r0, r4
0050f72c: add      sp, sp, #0x48
0050f730: pop      {r4, r5, r6, pc}
0050f734: mov      r2, #0x41
0050f738: bl       #0x30e868
0050f73c: mov      r4, r0
0050f740: b        #0x50f728

# _ZNK6glitch5scene17CTriangleSelector16getTriangleCountEv
00590f20: ldr      r2, [r0, #0x10]
00590f24: ldr      r3, [r0, #0xc]
00590f28: rsb      r3, r3, r2
00590f2c: asr      r3, r3, #2
00590f30: lsl      r2, r3, #3
00590f34: rsb      r2, r3, r2
00590f38: add      r2, r2, r2, lsl #6
00590f3c: add      r2, r3, r2, lsl #3
00590f40: lsl      r1, r2, #0xf
00590f44: rsb      r2, r2, r1
00590f48: add      r0, r3, r2, lsl #3
00590f4c: bx       lr

# _ZNK6glitch4core8CMatrix4IfE10getInverseERS2_
003232c0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003232c4: ldrb     r2, [r0, #0x40]
003232c8: sub      sp, sp, #0x2c
003232cc: mov      r4, r0
003232d0: cmp      r2, #0
003232d4: mov      r5, r1
003232d8: bne      #0x323ac8
003232dc: ldr      r6, [r0, #0x3c]
003232e0: ldr      sl, [r0, #0x28]
003232e4: ldr      sb, [r0, #0x2c]
003232e8: ldr      fp, [r0, #0x38]
003232ec: mov      r1, r6
003232f0: mov      r0, sl
003232f4: str      r2, [sp]
003232f8: bl       #0x30ed6c
003232fc: mov      r1, fp
00323300: mov      r7, r0
00323304: mov      r0, sb
00323308: bl       #0x30ed6c
0032330c: mov      r1, r0
00323310: mov      r0, r7
00323314: bl       #0x30e3ac
00323318: str      r0, [sp, #0x10]
0032331c: ldr      r1, [r4, #0x18]
00323320: mov      r0, r6
00323324: bl       #0x30ed6c
00323328: ldr      r8, [r4, #0x1c]
0032332c: mov      r7, r0
00323330: mov      r0, fp
00323334: mov      r1, r8
00323338: bl       #0x30ed6c
0032333c: mov      r1, r0
00323340: mov      r0, r7
00323344: bl       #0x30e3ac
00323348: str      r0, [sp, #0x14]
0032334c: ldr      r1, [r4, #0x18]
00323350: mov      r0, sb
00323354: bl       #0x30ed6c
00323358: mov      r1, r8
0032335c: mov      r7, r0
00323360: mov      r0, sl
00323364: bl       #0x30ed6c
00323368: mov      r1, r0
0032336c: mov      r0, r7
00323370: bl       #0x30e3ac
00323374: str      r0, [sp, #0x18]
00323378: ldr      r7, [r4, #8]
0032337c: mov      r0, r6
00323380: ldr      r6, [r4, #0xc]
00323384: mov      r1, r7
00323388: bl       #0x30ed6c
0032338c: mov      r1, r6
00323390: mov      r3, r0
00323394: mov      r0, fp
00323398: str      r3, [sp, #4]
0032339c: bl       #0x30ed6c
003233a0: ldr      r3, [sp, #4]
003233a4: mov      r1, r0
003233a8: mov      r0, r3
003233ac: bl       #0x30e3ac
003233b0: mov      r1, r7
003233b4: str      r0, [sp, #0x1c]
003233b8: mov      r0, sb
003233bc: bl       #0x30ed6c
003233c0: mov      r1, r6
003233c4: mov      sb, r0
003233c8: mov      r0, sl
003233cc: bl       #0x30ed6c
003233d0: mov      r1, r0
003233d4: mov      r0, sb
003233d8: bl       #0x30e3ac
003233dc: mov      r1, r7
003233e0: str      r0, [sp, #0x20]
003233e4: mov      r0, r8
003233e8: bl       #0x30ed6c
003233ec: mov      r1, r6
003233f0: mov      r7, r0
003233f4: ldr      r0, [r4, #0x18]
003233f8: bl       #0x30ed6c
003233fc: mov      r1, r0
00323400: mov      r0, r7
00323404: bl       #0x30e3ac
00323408: str      r0, [sp, #8]
0032340c: ldr      sb, [r4, #0x20]
00323410: ldr      sl, [r4, #0x34]
00323414: ldr      fp, [r4, #0x24]
00323418: mov      r0, sb
0032341c: mov      r1, sl
00323420: bl       #0x30ed6c
00323424: ldr      r8, [r4, #0x30]
00323428: mov      r6, r0
0032342c: mov      r0, fp
00323430: mov      r1, r8
00323434: bl       #0x30ed6c
00323438: mov      r1, r0
0032343c: mov      r0, r6
00323440: bl       #0x30e3ac
00323444: str      r0, [sp, #0x24]
00323448: ldr      r1, [r4, #0x10]
0032344c: mov      r0, sl
00323450: bl       #0x30ed6c
00323454: ldr      r1, [r4, #0x14]
00323458: mov      r6, r0
0032345c: mov      r0, r8
00323460: bl       #0x30ed6c
00323464: mov      r1, r0
00323468: mov      r0, r6
0032346c: bl       #0x30e3ac
00323470: ldr      r1, [r4, #0x10]
00323474: mov      r7, r0
00323478: mov      r0, fp
0032347c: bl       #0x30ed6c
00323480: ldr      r1, [r4, #0x14]
00323484: mov      r6, r0
00323488: mov      r0, sb
0032348c: bl       #0x30ed6c
00323490: mov      r1, r0
00323494: mov      r0, r6
00323498: bl       #0x30e3ac
0032349c: ldr      r1, [r4]
003234a0: mov      r6, r0
003234a4: mov      r0, sl
003234a8: bl       #0x30ed6c
003234ac: ldr      sl, [r4, #4]
003234b0: mov      r3, r0
003234b4: mov      r0, r8
003234b8: mov      r1, sl
003234bc: str      r3, [sp, #4]
003234c0: bl       #0x30ed6c
003234c4: ldr      r3, [sp, #4]
003234c8: mov      r1, r0
003234cc: mov      r0, r3
003234d0: bl       #0x30e3ac
003234d4: ldr      r1, [r4]
003234d8: mov      r8, r0
003234dc: mov      r0, fp
003234e0: bl       #0x30ed6c
003234e4: mov      r1, sl
003234e8: mov      fp, r0
003234ec: mov      r0, sb
003234f0: bl       #0x30ed6c
003234f4: mov      r1, r0
003234f8: mov      r0, fp
003234fc: bl       #0x30e3ac
00323500: ldr      r1, [r4]
00323504: mov      sb, r0
00323508: ldr      r0, [r4, #0x14]
0032350c: bl       #0x30ed6c
00323510: mov      r1, sl
00323514: mov      fp, r0
00323518: ldr      r0, [r4, #0x10]
0032351c: bl       #0x30ed6c
00323520: mov      r1, r0
00323524: mov      r0, fp
00323528: bl       #0x30e3ac
0032352c: mov      sl, r0
00323530: mov      r1, sl
00323534: ldr      r0, [sp, #0x10]
00323538: bl       #0x30ed6c
0032353c: mov      r1, sb
00323540: mov      fp, r0
00323544: ldr      r0, [sp, #0x14]
00323548: bl       #0x30ed6c
0032354c: mov      r1, r0
00323550: mov      r0, fp
00323554: bl       #0x30e3ac
00323558: mov      r1, r8
0032355c: mov      fp, r0
00323560: ldr      r0, [sp, #0x18]
00323564: bl       #0x30ed6c
00323568: mov      r1, r0
0032356c: mov      r0, fp
00323570: bl       #0x30eba4
00323574: mov      r1, r6
00323578: mov      fp, r0
0032357c: ldr      r0, [sp, #0x1c]
00323580: bl       #0x30ed6c
00323584: mov      r1, r0
00323588: mov      r0, fp
0032358c: bl       #0x30eba4
00323590: mov      r1, r7
00323594: mov      fp, r0
00323598: ldr      r0, [sp, #0x20]
0032359c: bl       #0x30ed6c
003235a0: mov      r1, r0
003235a4: mov      r0, fp
003235a8: bl       #0x30e3ac
003235ac: ldr      r1, [sp, #0x24]
003235b0: mov      fp, r0
003235b4: ldr      r0, [sp, #8]
003235b8: bl       #0x30ed6c
003235bc: mov      r1, r0
003235c0: mov      r0, fp
003235c4: bl       #0x30eba4
003235c8: str      r0, [sp, #0xc]
003235cc: ldr      r3, [sp, #0xc]
003235d0: movw     r1, #0x37bd
003235d4: movt     r1, #0x3586
003235d8: bic      r0, r3, #0x80000000
003235dc: bl       #0x30e9ac
003235e0: ldr      r2, [sp]
003235e4: cmp      r0, #0
003235e8: movne    r0, r2
003235ec: bne      #0x323ac0
003235f0: strb     r2, [r5, #0x40]
003235f4: ldr      r1, [r4, #0x14]
003235f8: ldr      r0, [sp, #0x10]
003235fc: str      r2, [sp]
00323600: bl       #0x30ed6c
00323604: ldr      r1, [r4, #0x24]
00323608: mov      fp, r0
0032360c: ldr      r0, [sp, #0x14]
00323610: bl       #0x30ed6c
00323614: mov      r1, r0
00323618: mov      r0, fp
0032361c: bl       #0x30e3ac
00323620: ldr      r1, [r4, #0x34]
00323624: mov      fp, r0
00323628: ldr      r0, [sp, #0x18]
0032362c: bl       #0x30ed6c
00323630: mov      r1, r0
00323634: mov      r0, fp
00323638: bl       #0x30eba4
0032363c: str      r0, [r5]
00323640: ldr      r1, [r4, #0x24]
00323644: ldr      r0, [sp, #0x1c]
00323648: bl       #0x30ed6c
0032364c: ldr      r1, [r4, #4]
00323650: mov      fp, r0
00323654: ldr      r0, [sp, #0x10]
00323658: bl       #0x30ed6c
0032365c: mov      r1, r0
00323660: mov      r0, fp
00323664: bl       #0x30e3ac
00323668: ldr      r1, [r4, #0x34]
0032366c: mov      fp, r0
00323670: ldr      r0, [sp, #0x20]
00323674: bl       #0x30ed6c
00323678: mov      r1, r0
0032367c: mov      r0, fp
00323680: bl       #0x30e3ac
00323684: str      r0, [r5, #4]
00323688: ldr      r1, [r4, #4]
0032368c: ldr      r0, [sp, #0x14]
00323690: bl       #0x30ed6c
00323694: ldr      r1, [r4, #0x14]
00323698: mov      fp, r0
0032369c: ldr      r0, [sp, #0x1c]
003236a0: bl       #0x30ed6c
003236a4: mov      r1, r0
003236a8: mov      r0, fp
003236ac: bl       #0x30e3ac
003236b0: ldr      r1, [r4, #0x34]
003236b4: mov      fp, r0
003236b8: ldr      r0, [sp, #8]
003236bc: bl       #0x30ed6c
003236c0: mov      r1, r0
003236c4: mov      r0, fp
003236c8: bl       #0x30eba4
003236cc: str      r0, [r5, #8]
003236d0: ldr      r1, [r4, #0x14]
003236d4: ldr      r0, [sp, #0x20]
003236d8: bl       #0x30ed6c
003236dc: ldr      r1, [r4, #4]
003236e0: mov      fp, r0
003236e4: ldr      r0, [sp, #0x18]
003236e8: bl       #0x30ed6c
003236ec: mov      r1, r0
003236f0: mov      r0, fp
003236f4: bl       #0x30e3ac
003236f8: ldr      r1, [r4, #0x24]
003236fc: mov      fp, r0
00323700: ldr      r0, [sp, #8]
00323704: bl       #0x30ed6c
00323708: mov      r1, r0
0032370c: mov      r0, fp
00323710: bl       #0x30e3ac
00323714: str      r0, [r5, #0xc]
00323718: ldr      r1, [r4, #0x20]
0032371c: ldr      r0, [sp, #0x14]
00323720: bl       #0x30ed6c
00323724: ldr      r1, [r4, #0x10]
00323728: mov      fp, r0
0032372c: ldr      r0, [sp, #0x10]
00323730: bl       #0x30ed6c
00323734: mov      r1, r0
00323738: mov      r0, fp
0032373c: bl       #0x30e3ac
00323740: ldr      r1, [r4, #0x30]
00323744: mov      fp, r0
00323748: ldr      r0, [sp, #0x18]
0032374c: bl       #0x30ed6c
00323750: mov      r1, r0
00323754: mov      r0, fp
00323758: bl       #0x30e3ac
0032375c: str      r0, [r5, #0x10]
00323760: ldr      r1, [r4]
00323764: ldr      r0, [sp, #0x10]
00323768: bl       #0x30ed6c
0032376c: ldr      r1, [r4, #0x20]
00323770: mov      fp, r0
00323774: ldr      r0, [sp, #0x1c]
00323778: bl       #0x30ed6c
0032377c: mov      r1, r0
00323780: mov      r0, fp
00323784: bl       #0x30e3ac
00323788: ldr      r1, [r4, #0x30]
0032378c: mov      fp, r0
00323790: ldr      r0, [sp, #0x20]
00323794: bl       #0x30ed6c
00323798: mov      r1, r0
0032379c: mov      r0, fp
003237a0: bl       #0x30eba4
003237a4: str      r0, [r5, #0x14]
003237a8: ldr      r1, [r4, #0x10]
003237ac: ldr      r0, [sp, #0x1c]
003237b0: bl       #0x30ed6c
003237b4: ldr      r1, [r4]
003237b8: mov      fp, r0
003237bc: ldr      r0, [sp, #0x14]
003237c0: bl       #0x30ed6c
003237c4: mov      r1, r0
003237c8: mov      r0, fp
003237cc: bl       #0x30e3ac
003237d0: ldr      r1, [r4, #0x30]
003237d4: mov      fp, r0
003237d8: ldr      r0, [sp, #8]
003237dc: bl       #0x30ed6c
003237e0: mov      r1, r0
003237e4: mov      r0, fp
003237e8: bl       #0x30e3ac
003237ec: str      r0, [r5, #0x18]
003237f0: ldr      r1, [r4]
003237f4: ldr      r0, [sp, #0x18]
003237f8: bl       #0x30ed6c
003237fc: ldr      r1, [r4, #0x10]
00323800: mov      fp, r0
00323804: ldr      r0, [sp, #0x20]
00323808: bl       #0x30ed6c
0032380c: mov      r1, r0
00323810: mov      r0, fp
00323814: bl       #0x30e3ac
00323818: ldr      r1, [r4, #0x20]
0032381c: mov      fp, r0
00323820: ldr      r0, [sp, #8]
00323824: bl       #0x30ed6c
00323828: mov      r1, r0
0032382c: mov      r0, fp
00323830: bl       #0x30eba4
00323834: str      r0, [r5, #0x1c]
00323838: ldr      r1, [r4, #0x1c]
0032383c: ldr      r0, [sp, #0x24]
00323840: bl       #0x30ed6c
00323844: ldr      r1, [r4, #0x2c]
00323848: mov      fp, r0
0032384c: mov      r0, r7
00323850: bl       #0x30ed6c
00323854: mov      r1, r0
00323858: mov      r0, fp
0032385c: bl       #0x30e3ac
00323860: ldr      r1, [r4, #0x3c]
00323864: mov      fp, r0
00323868: mov      r0, r6
0032386c: bl       #0x30ed6c
00323870: mov      r1, r0
00323874: mov      r0, fp
00323878: bl       #0x30eba4
0032387c: str      r0, [r5, #0x20]
00323880: ldr      r1, [r4, #0x2c]
00323884: mov      r0, r8
00323888: bl       #0x30ed6c
0032388c: ldr      r1, [r4, #0xc]
00323890: mov      fp, r0
00323894: ldr      r0, [sp, #0x24]
00323898: bl       #0x30ed6c
0032389c: mov      r1, r0
003238a0: mov      r0, fp
003238a4: bl       #0x30e3ac
003238a8: ldr      r1, [r4, #0x3c]
003238ac: mov      fp, r0
003238b0: mov      r0, sb
003238b4: bl       #0x30ed6c
003238b8: mov      r1, r0
003238bc: mov      r0, fp
003238c0: bl       #0x30e3ac
003238c4: str      r0, [r5, #0x24]
003238c8: ldr      r1, [r4, #0xc]
003238cc: mov      r0, r7
003238d0: bl       #0x30ed6c
003238d4: ldr      r1, [r4, #0x1c]
003238d8: mov      fp, r0
003238dc: mov      r0, r8
003238e0: bl       #0x30ed6c
003238e4: mov      r1, r0
003238e8: mov      r0, fp
003238ec: bl       #0x30e3ac
003238f0: ldr      r1, [r4, #0x3c]
003238f4: mov      fp, r0
003238f8: mov      r0, sl
003238fc: bl       #0x30ed6c
00323900: mov      r1, r0
00323904: mov      r0, fp
00323908: bl       #0x30eba4
0032390c: str      r0, [r5, #0x28]
00323910: ldr      r1, [r4, #0x1c]
00323914: mov      r0, sb
00323918: bl       #0x30ed6c
0032391c: ldr      r1, [r4, #0xc]
00323920: mov      fp, r0
00323924: mov      r0, r6
00323928: bl       #0x30ed6c
0032392c: mov      r1, r0
00323930: mov      r0, fp
00323934: bl       #0x30e3ac
00323938: ldr      r1, [r4, #0x2c]
0032393c: mov      fp, r0
00323940: mov      r0, sl
00323944: bl       #0x30ed6c
00323948: mov      r1, r0
0032394c: mov      r0, fp
00323950: bl       #0x30e3ac
00323954: str      r0, [r5, #0x2c]
00323958: ldr      r1, [r4, #0x28]
0032395c: mov      r0, r7
00323960: bl       #0x30ed6c
00323964: ldr      r1, [r4, #0x18]
00323968: mov      fp, r0
0032396c: ldr      r0, [sp, #0x24]
00323970: bl       #0x30ed6c
00323974: mov      r1, r0
00323978: mov      r0, fp
0032397c: bl       #0x30e3ac
00323980: ldr      r1, [r4, #0x38]
00323984: mov      fp, r0
00323988: mov      r0, r6
0032398c: bl       #0x30ed6c
00323990: mov      r1, r0
00323994: mov      r0, fp
00323998: bl       #0x30e3ac
0032399c: str      r0, [r5, #0x30]
003239a0: ldr      r1, [r4, #8]
003239a4: ldr      r0, [sp, #0x24]
003239a8: bl       #0x30ed6c
003239ac: ldr      r1, [r4, #0x28]
003239b0: mov      fp, r0
003239b4: mov      r0, r8
003239b8: bl       #0x30ed6c
003239bc: mov      r1, r0
003239c0: mov      r0, fp
003239c4: bl       #0x30e3ac
003239c8: ldr      r1, [r4, #0x38]
003239cc: mov      fp, r0
003239d0: mov      r0, sb
003239d4: bl       #0x30ed6c
003239d8: mov      r1, r0
003239dc: mov      r0, fp
003239e0: bl       #0x30eba4
003239e4: str      r0, [r5, #0x34]
003239e8: ldr      r1, [r4, #0x18]
003239ec: mov      r0, r8
003239f0: bl       #0x30ed6c
003239f4: ldr      r1, [r4, #8]
003239f8: mov      r8, r0
003239fc: mov      r0, r7
00323a00: bl       #0x30ed6c
00323a04: mov      r1, r0
00323a08: mov      r0, r8
00323a0c: bl       #0x30e3ac
00323a10: ldr      r1, [r4, #0x38]
00323a14: mov      r7, r0
00323a18: mov      r0, sl
00323a1c: bl       #0x30ed6c
00323a20: mov      r1, r0
00323a24: mov      r0, r7
00323a28: bl       #0x30e3ac
00323a2c: str      r0, [r5, #0x38]
00323a30: ldr      r1, [r4, #8]
00323a34: mov      r0, r6
00323a38: bl       #0x30ed6c
00323a3c: ldr      r1, [r4, #0x18]
00323a40: mov      r6, r0
00323a44: mov      r0, sb
00323a48: bl       #0x30ed6c
00323a4c: mov      r1, r0
00323a50: mov      r0, r6
00323a54: bl       #0x30e3ac
00323a58: ldr      r1, [r4, #0x28]
00323a5c: mov      r6, r0
00323a60: mov      r0, sl
00323a64: bl       #0x30ed6c
00323a68: mov      r1, r0
00323a6c: mov      r0, r6
00323a70: bl       #0x30eba4
00323a74: str      r0, [r5, #0x3c]
00323a78: ldr      r2, [sp]
00323a7c: ldr      r1, [sp, #0xc]
00323a80: mov      r0, #0x3f800000
00323a84: mov      r6, r2
00323a88: bl       #0x30ec94
00323a8c: mov      r7, r0
00323a90: ldr      r0, [r5, r6]
00323a94: mov      r1, r7
00323a98: bl       #0x30ed6c
00323a9c: str      r0, [r5, r6]
00323aa0: add      r6, r6, #4
00323aa4: cmp      r6, #0x40
00323aa8: bne      #0x323a90
00323aac: mov      r3, #0
00323ab0: strb     r3, [r5, #0x40]
00323ab4: ldrb     r3, [r4, #0x40]
00323ab8: mov      r0, #1
00323abc: strb     r3, [r5, #0x40]
00323ac0: add      sp, sp, #0x2c
00323ac4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00323ac8: mov      r0, r1
00323acc: mov      r2, #0x41
00323ad0: mov      r1, r4
00323ad4: bl       #0x30e868
00323ad8: mov      r0, #1
00323adc: b        #0x323ac0

# _ZN6glitch4core8CMatrix4IfE11makeInverseEv
005822f8: push     {r4, r5, lr}
005822fc: ldrb     r3, [r0, #0x40]
00582300: sub      sp, sp, #0x4c
00582304: mov      r4, r0
00582308: cmp      r3, #0
0058230c: beq      #0x58231c
00582310: mov      r0, #1
00582314: add      sp, sp, #0x4c
00582318: pop      {r4, r5, pc}
0058231c: add      r5, sp, #4
00582320: mov      r1, r5
00582324: strb     r3, [sp, #0x44]
00582328: bl       #0x3232c0
0058232c: cmp      r0, #0
00582330: beq      #0x582314
00582334: mov      r0, r4
00582338: mov      r1, r5
0058233c: mov      r2, #0x41
00582340: bl       #0x30e868
00582344: b        #0x582310

# _ZNK6glitch5scene17CTriangleSelector5SetupEPKNS_4core8CMatrix4IfEE
00586a34: push     {r4, r5, r6, lr}
00586a38: mov      r3, #0
00586a3c: add      r5, r0, #0x5c
00586a40: mov      r4, r0
00586a44: strb     r3, [r0, #0x9c]
00586a48: mov      r6, r1
00586a4c: mov      r2, #0x40
00586a50: mov      r1, r3
00586a54: mov      r0, r5
00586a58: bl       #0x30e460
00586a5c: mov      r3, #0x3f800000
00586a60: mov      r2, #1
00586a64: cmp      r6, #0
00586a68: str      r3, [r4, #0x98]
00586a6c: strb     r2, [r4, #0x9c]
00586a70: str      r3, [r4, #0x5c]
00586a74: str      r3, [r4, #0x70]
00586a78: str      r3, [r4, #0x84]
00586a7c: beq      #0x586a90
00586a80: mov      r1, r6
00586a84: mov      r0, r5
00586a88: mov      r2, #0x41
00586a8c: bl       #0x30e868
00586a90: ldr      r3, [r4, #8]
00586a94: cmp      r3, #0
00586a98: beq      #0x586aa8
00586a9c: ldrb     r2, [r4, #0x18]
00586aa0: cmp      r2, #0
00586aa4: beq      #0x586aac
00586aa8: pop      {r4, r5, r6, pc}
00586aac: mov      r0, r3
00586ab0: ldr      r3, [r3]
00586ab4: mov      lr, pc
00586ab8: ldr      pc, [r3, #0x38]
00586abc: mov      r1, r0
00586ac0: mov      r0, r5
00586ac4: pop      {r4, r5, r6, lr}
00586ac8: b        #0x50f6d8

# _ZNK6glitch5scene24COctTreeTriangleSelector12getTrianglesEPNS_4core10triangle3dIfEEiRiRKNS2_8aabbox3dIfEEPKNS2_8CMatrix4IfEE
00587694: push     {r4, r5, r6, lr}
00587698: str      r2, [r0, #0xa4]
0058769c: mov      r2, #0
005876a0: str      r2, [r0, #0xa8]
005876a4: str      r1, [r0, #0xa0]
005876a8: mov      r4, r0
005876ac: ldr      r1, [sp, #0x14]
005876b0: mov      r5, r3
005876b4: bl       #0x586a34
005876b8: ldr      r1, [sp, #0x10]
005876bc: mov      r0, r4
005876c0: bl       #0x587604
005876c4: ldr      r1, [r4, #0xac]
005876c8: cmp      r1, #0
005876cc: beq      #0x5876d8
005876d0: mov      r0, r4
005876d4: bl       #0x5870fc
005876d8: ldr      r3, [r4, #0xa8]
005876dc: str      r3, [r5]
005876e0: pop      {r4, r5, r6, pc}

# _ZN6glitch4core6detail12CMatrix4BaseIfE20setbyproduct_nocheckERKS3_S5_
0040ea54: push     {r4, r5, r6, r7, r8, lr}
0040ea58: mov      r4, r1
0040ea5c: mov      r6, r0
0040ea60: ldr      r1, [r2]
0040ea64: ldr      r0, [r4]
0040ea68: mov      r5, r2
0040ea6c: bl       #0x30ed6c
0040ea70: ldr      r1, [r5, #4]
0040ea74: mov      r7, r0
0040ea78: ldr      r0, [r4, #0x10]
0040ea7c: bl       #0x30ed6c
0040ea80: mov      r1, r0
0040ea84: mov      r0, r7
0040ea88: bl       #0x30eba4
0040ea8c: ldr      r1, [r5, #8]
0040ea90: mov      r7, r0
0040ea94: ldr      r0, [r4, #0x20]
0040ea98: bl       #0x30ed6c
0040ea9c: mov      r1, r0
0040eaa0: mov      r0, r7
0040eaa4: bl       #0x30eba4
0040eaa8: ldr      r1, [r5, #0xc]
0040eaac: mov      r7, r0
0040eab0: ldr      r0, [r4, #0x30]
0040eab4: bl       #0x30ed6c
0040eab8: mov      r1, r0
0040eabc: mov      r0, r7
0040eac0: bl       #0x30eba4
0040eac4: str      r0, [r6]
0040eac8: ldr      r1, [r5]
0040eacc: ldr      r0, [r4, #4]
0040ead0: bl       #0x30ed6c
0040ead4: ldr      r1, [r5, #4]
0040ead8: mov      r7, r0
0040eadc: ldr      r0, [r4, #0x14]
0040eae0: bl       #0x30ed6c
0040eae4: mov      r1, r0
0040eae8: mov      r0, r7
0040eaec: bl       #0x30eba4
0040eaf0: ldr      r1, [r5, #8]
0040eaf4: mov      r7, r0
0040eaf8: ldr      r0, [r4, #0x24]
0040eafc: bl       #0x30ed6c
0040eb00: mov      r1, r0
0040eb04: mov      r0, r7
0040eb08: bl       #0x30eba4
0040eb0c: ldr      r1, [r5, #0xc]
0040eb10: mov      r7, r0
0040eb14: ldr      r0, [r4, #0x34]
0040eb18: bl       #0x30ed6c
0040eb1c: mov      r1, r0
0040eb20: mov      r0, r7
0040eb24: bl       #0x30eba4
0040eb28: str      r0, [r6, #4]
0040eb2c: ldr      r1, [r5]
0040eb30: ldr      r0, [r4, #8]
0040eb34: bl       #0x30ed6c
0040eb38: ldr      r1, [r5, #4]
0040eb3c: mov      r7, r0
0040eb40: ldr      r0, [r4, #0x18]
0040eb44: bl       #0x30ed6c
0040eb48: mov      r1, r0
0040eb4c: mov      r0, r7
0040eb50: bl       #0x30eba4
0040eb54: ldr      r1, [r5, #8]
0040eb58: mov      r7, r0
0040eb5c: ldr      r0, [r4, #0x28]
0040eb60: bl       #0x30ed6c
0040eb64: mov      r1, r0
0040eb68: mov      r0, r7
0040eb6c: bl       #0x30eba4
0040eb70: ldr      r1, [r5, #0xc]
0040eb74: mov      r7, r0
0040eb78: ldr      r0, [r4, #0x38]
0040eb7c: bl       #0x30ed6c
0040eb80: mov      r1, r0
0040eb84: mov      r0, r7
0040eb88: bl       #0x30eba4
0040eb8c: str      r0, [r6, #8]
0040eb90: ldr      r1, [r5]
0040eb94: ldr      r0, [r4, #0xc]
0040eb98: bl       #0x30ed6c
0040eb9c: ldr      r1, [r5, #4]
0040eba0: mov      r7, r0
0040eba4: ldr      r0, [r4, #0x1c]
0040eba8: bl       #0x30ed6c
0040ebac: mov      r1, r0
0040ebb0: mov      r0, r7
0040ebb4: bl       #0x30eba4
0040ebb8: ldr      r1, [r5, #8]
0040ebbc: mov      r7, r0
0040ebc0: ldr      r0, [r4, #0x2c]
0040ebc4: bl       #0x30ed6c
0040ebc8: mov      r1, r0
0040ebcc: mov      r0, r7
0040ebd0: bl       #0x30eba4
0040ebd4: ldr      r1, [r5, #0xc]
0040ebd8: mov      r7, r0
0040ebdc: ldr      r0, [r4, #0x3c]
0040ebe0: bl       #0x30ed6c
0040ebe4: mov      r1, r0
0040ebe8: mov      r0, r7
0040ebec: bl       #0x30eba4
0040ebf0: str      r0, [r6, #0xc]
0040ebf4: ldr      r1, [r5, #0x10]
0040ebf8: ldr      r0, [r4]
0040ebfc: bl       #0x30ed6c
0040ec00: ldr      r1, [r5, #0x14]
0040ec04: mov      r7, r0
0040ec08: ldr      r0, [r4, #0x10]
0040ec0c: bl       #0x30ed6c
0040ec10: mov      r1, r0
0040ec14: mov      r0, r7
0040ec18: bl       #0x30eba4
0040ec1c: ldr      r1, [r5, #0x18]
0040ec20: mov      r7, r0
0040ec24: ldr      r0, [r4, #0x20]
0040ec28: bl       #0x30ed6c
0040ec2c: mov      r1, r0
0040ec30: mov      r0, r7
0040ec34: bl       #0x30eba4
0040ec38: ldr      r1, [r5, #0x1c]
0040ec3c: mov      r7, r0
0040ec40: ldr      r0, [r4, #0x30]
0040ec44: bl       #0x30ed6c
0040ec48: mov      r1, r0
0040ec4c: mov      r0, r7
0040ec50: bl       #0x30eba4
0040ec54: str      r0, [r6, #0x10]
0040ec58: ldr      r1, [r5, #0x10]
0040ec5c: ldr      r0, [r4, #4]
0040ec60: bl       #0x30ed6c
0040ec64: ldr      r1, [r5, #0x14]
0040ec68: mov      r7, r0
0040ec6c: ldr      r0, [r4, #0x14]
0040ec70: bl       #0x30ed6c
0040ec74: mov      r1, r0
0040ec78: mov      r0, r7
0040ec7c: bl       #0x30eba4
0040ec80: ldr      r1, [r5, #0x18]
0040ec84: mov      r7, r0
0040ec88: ldr      r0, [r4, #0x24]
0040ec8c: bl       #0x30ed6c
0040ec90: mov      r1, r0
0040ec94: mov      r0, r7
0040ec98: bl       #0x30eba4
0040ec9c: ldr      r1, [r5, #0x1c]
0040eca0: mov      r7, r0
0040eca4: ldr      r0, [r4, #0x34]
0040eca8: bl       #0x30ed6c
0040ecac: mov      r1, r0
0040ecb0: mov      r0, r7
0040ecb4: bl       #0x30eba4
0040ecb8: str      r0, [r6, #0x14]
0040ecbc: ldr      r1, [r5, #0x10]
0040ecc0: ldr      r0, [r4, #8]
0040ecc4: bl       #0x30ed6c
0040ecc8: ldr      r1, [r5, #0x14]
0040eccc: mov      r7, r0
0040ecd0: ldr      r0, [r4, #0x18]
0040ecd4: bl       #0x30ed6c
0040ecd8: mov      r1, r0
0040ecdc: mov      r0, r7
0040ece0: bl       #0x30eba4
0040ece4: ldr      r1, [r5, #0x18]
0040ece8: mov      r7, r0
0040ecec: ldr      r0, [r4, #0x28]
0040ecf0: bl       #0x30ed6c
0040ecf4: mov      r1, r0
0040ecf8: mov      r0, r7
0040ecfc: bl       #0x30eba4
0040ed00: ldr      r1, [r5, #0x1c]
0040ed04: mov      r7, r0
0040ed08: ldr      r0, [r4, #0x38]
0040ed0c: bl       #0x30ed6c
0040ed10: mov      r1, r0
0040ed14: mov      r0, r7
0040ed18: bl       #0x30eba4
0040ed1c: str      r0, [r6, #0x18]
0040ed20: ldr      r1, [r5, #0x10]
0040ed24: ldr      r0, [r4, #0xc]
0040ed28: bl       #0x30ed6c
0040ed2c: ldr      r1, [r5, #0x14]
0040ed30: mov      r7, r0
0040ed34: ldr      r0, [r4, #0x1c]
0040ed38: bl       #0x30ed6c
0040ed3c: mov      r1, r0
0040ed40: mov      r0, r7
0040ed44: bl       #0x30eba4
0040ed48: ldr      r1, [r5, #0x18]
0040ed4c: mov      r7, r0
0040ed50: ldr      r0, [r4, #0x2c]
0040ed54: bl       #0x30ed6c
0040ed58: mov      r1, r0
0040ed5c: mov      r0, r7
0040ed60: bl       #0x30eba4
0040ed64: ldr      r1, [r5, #0x1c]
0040ed68: mov      r7, r0
0040ed6c: ldr      r0, [r4, #0x3c]
0040ed70: bl       #0x30ed6c
0040ed74: mov      r1, r0
0040ed78: mov      r0, r7
0040ed7c: bl       #0x30eba4
0040ed80: str      r0, [r6, #0x1c]
0040ed84: ldr      r1, [r5, #0x20]
0040ed88: ldr      r0, [r4]
0040ed8c: bl       #0x30ed6c
0040ed90: ldr      r1, [r5, #0x24]
0040ed94: mov      r7, r0
0040ed98: ldr      r0, [r4, #0x10]
0040ed9c: bl       #0x30ed6c
0040eda0: mov      r1, r0
0040eda4: mov      r0, r7
0040eda8: bl       #0x30eba4
0040edac: ldr      r1, [r5, #0x28]
0040edb0: mov      r7, r0
0040edb4: ldr      r0, [r4, #0x20]
0040edb8: bl       #0x30ed6c
0040edbc: mov      r1, r0
0040edc0: mov      r0, r7
0040edc4: bl       #0x30eba4
0040edc8: ldr      r1, [r5, #0x2c]
0040edcc: mov      r7, r0
0040edd0: ldr      r0, [r4, #0x30]
0040edd4: bl       #0x30ed6c
0040edd8: mov      r1, r0
0040eddc: mov      r0, r7
0040ede0: bl       #0x30eba4
0040ede4: str      r0, [r6, #0x20]
0040ede8: ldr      r1, [r5, #0x20]
0040edec: ldr      r0, [r4, #4]
0040edf0: bl       #0x30ed6c
0040edf4: ldr      r1, [r5, #0x24]
0040edf8: mov      r7, r0
0040edfc: ldr      r0, [r4, #0x14]
0040ee00: bl       #0x30ed6c
0040ee04: mov      r1, r0
0040ee08: mov      r0, r7
0040ee0c: bl       #0x30eba4
0040ee10: ldr      r1, [r5, #0x28]
0040ee14: mov      r7, r0
0040ee18: ldr      r0, [r4, #0x24]
0040ee1c: bl       #0x30ed6c
0040ee20: mov      r1, r0
0040ee24: mov      r0, r7
0040ee28: bl       #0x30eba4
0040ee2c: ldr      r1, [r5, #0x2c]
0040ee30: mov      r7, r0
0040ee34: ldr      r0, [r4, #0x34]
0040ee38: bl       #0x30ed6c
0040ee3c: mov      r1, r0
0040ee40: mov      r0, r7
0040ee44: bl       #0x30eba4
0040ee48: str      r0, [r6, #0x24]
0040ee4c: ldr      r1, [r5, #0x20]
0040ee50: ldr      r0, [r4, #8]
0040ee54: bl       #0x30ed6c
0040ee58: ldr      r1, [r5, #0x24]
0040ee5c: mov      r7, r0
0040ee60: ldr      r0, [r4, #0x18]
0040ee64: bl       #0x30ed6c
0040ee68: mov      r1, r0
0040ee6c: mov      r0, r7
0040ee70: bl       #0x30eba4
0040ee74: ldr      r1, [r5, #0x28]
0040ee78: mov      r7, r0
0040ee7c: ldr      r0, [r4, #0x28]
0040ee80: bl       #0x30ed6c
0040ee84: mov      r1, r0
0040ee88: mov      r0, r7
0040ee8c: bl       #0x30eba4
0040ee90: ldr      r1, [r5, #0x2c]
0040ee94: mov      r7, r0
0040ee98: ldr      r0, [r4, #0x38]
0040ee9c: bl       #0x30ed6c
0040eea0: mov      r1, r0
0040eea4: mov      r0, r7
0040eea8: bl       #0x30eba4
0040eeac: str      r0, [r6, #0x28]
0040eeb0: ldr      r1, [r5, #0x20]
0040eeb4: ldr      r0, [r4, #0xc]
0040eeb8: bl       #0x30ed6c
0040eebc: ldr      r1, [r5, #0x24]
0040eec0: mov      r7, r0
0040eec4: ldr      r0, [r4, #0x1c]
0040eec8: bl       #0x30ed6c
0040eecc: mov      r1, r0
0040eed0: mov      r0, r7
0040eed4: bl       #0x30eba4
0040eed8: ldr      r1, [r5, #0x28]
0040eedc: mov      r7, r0
0040eee0: ldr      r0, [r4, #0x2c]
0040eee4: bl       #0x30ed6c
0040eee8: mov      r1, r0
0040eeec: mov      r0, r7
0040eef0: bl       #0x30eba4
0040eef4: ldr      r1, [r5, #0x2c]
0040eef8: mov      r7, r0
0040eefc: ldr      r0, [r4, #0x3c]
0040ef00: bl       #0x30ed6c
0040ef04: mov      r1, r0
0040ef08: mov      r0, r7
0040ef0c: bl       #0x30eba4
0040ef10: str      r0, [r6, #0x2c]
0040ef14: ldr      r1, [r5, #0x30]
0040ef18: ldr      r0, [r4]
0040ef1c: bl       #0x30ed6c
0040ef20: ldr      r1, [r5, #0x34]
0040ef24: mov      r7, r0
0040ef28: ldr      r0, [r4, #0x10]
0040ef2c: bl       #0x30ed6c
0040ef30: mov      r1, r0
0040ef34: mov      r0, r7
0040ef38: bl       #0x30eba4
0040ef3c: ldr      r1, [r5, #0x38]
0040ef40: mov      r7, r0
0040ef44: ldr      r0, [r4, #0x20]
0040ef48: bl       #0x30ed6c
0040ef4c: mov      r1, r0
0040ef50: mov      r0, r7
0040ef54: bl       #0x30eba4
0040ef58: ldr      r1, [r5, #0x3c]
0040ef5c: mov      r7, r0
0040ef60: ldr      r0, [r4, #0x30]
0040ef64: bl       #0x30ed6c
0040ef68: mov      r1, r0
0040ef6c: mov      r0, r7
0040ef70: bl       #0x30eba4
0040ef74: str      r0, [r6, #0x30]
0040ef78: ldr      r1, [r5, #0x30]
0040ef7c: ldr      r0, [r4, #4]
0040ef80: bl       #0x30ed6c
0040ef84: ldr      r1, [r5, #0x34]
0040ef88: mov      r7, r0
0040ef8c: ldr      r0, [r4, #0x14]
0040ef90: bl       #0x30ed6c
0040ef94: mov      r1, r0
0040ef98: mov      r0, r7
0040ef9c: bl       #0x30eba4
0040efa0: ldr      r1, [r5, #0x38]
0040efa4: mov      r7, r0
0040efa8: ldr      r0, [r4, #0x24]
0040efac: bl       #0x30ed6c
0040efb0: mov      r1, r0
0040efb4: mov      r0, r7
0040efb8: bl       #0x30eba4
0040efbc: ldr      r1, [r5, #0x3c]
0040efc0: mov      r7, r0
0040efc4: ldr      r0, [r4, #0x34]
0040efc8: bl       #0x30ed6c
0040efcc: mov      r1, r0
0040efd0: mov      r0, r7
0040efd4: bl       #0x30eba4
0040efd8: str      r0, [r6, #0x34]
0040efdc: ldr      r1, [r5, #0x30]
0040efe0: ldr      r0, [r4, #8]
0040efe4: bl       #0x30ed6c
0040efe8: ldr      r1, [r5, #0x34]
0040efec: mov      r7, r0
0040eff0: ldr      r0, [r4, #0x18]
0040eff4: bl       #0x30ed6c
0040eff8: mov      r1, r0
0040effc: mov      r0, r7
0040f000: bl       #0x30eba4
0040f004: ldr      r1, [r5, #0x38]
0040f008: mov      r7, r0
0040f00c: ldr      r0, [r4, #0x28]
0040f010: bl       #0x30ed6c
0040f014: mov      r1, r0
0040f018: mov      r0, r7
0040f01c: bl       #0x30eba4
0040f020: ldr      r1, [r5, #0x3c]
0040f024: mov      r7, r0
0040f028: ldr      r0, [r4, #0x38]
0040f02c: bl       #0x30ed6c
0040f030: mov      r1, r0
0040f034: mov      r0, r7
0040f038: bl       #0x30eba4
0040f03c: str      r0, [r6, #0x38]
0040f040: ldr      r1, [r5, #0x30]
0040f044: ldr      r0, [r4, #0xc]
0040f048: bl       #0x30ed6c
0040f04c: ldr      r1, [r5, #0x34]
0040f050: mov      r7, r0
0040f054: ldr      r0, [r4, #0x1c]
0040f058: bl       #0x30ed6c
0040f05c: mov      r1, r0
0040f060: mov      r0, r7
0040f064: bl       #0x30eba4
0040f068: ldr      r1, [r5, #0x38]
0040f06c: mov      r7, r0
0040f070: ldr      r0, [r4, #0x2c]
0040f074: bl       #0x30ed6c
0040f078: mov      r1, r0
0040f07c: mov      r0, r7
0040f080: bl       #0x30eba4
0040f084: ldr      r1, [r5, #0x3c]
0040f088: mov      r7, r0
0040f08c: ldr      r0, [r4, #0x3c]
0040f090: bl       #0x30ed6c
0040f094: mov      r1, r0
0040f098: mov      r0, r7
0040f09c: bl       #0x30eba4
0040f0a0: mov      r3, #0
0040f0a4: str      r0, [r6, #0x3c]
0040f0a8: strb     r3, [r6, #0x40]
0040f0ac: mov      r0, r6
0040f0b0: pop      {r4, r5, r6, r7, r8, pc}

# _ZNK6glitch5scene17CTriangleSelector5SetupERKNS_4core8aabbox3dIfEE
00587604: push     {r4, r5, lr}
00587608: ldr      r2, [r1]
0058760c: ldr      r3, [r0, #8]
00587610: sub      sp, sp, #0x4c
00587614: str      r2, [r0, #0x44]
00587618: ldr      r2, [r1, #4]
0058761c: cmp      r3, #0
00587620: mov      r4, r0
00587624: str      r2, [r0, #0x48]
00587628: ldr      r2, [r1, #8]
0058762c: str      r2, [r0, #0x4c]
00587630: ldr      r2, [r1, #0xc]
00587634: str      r2, [r0, #0x50]
00587638: ldr      r2, [r1, #0x10]
0058763c: str      r2, [r0, #0x54]
00587640: ldr      r2, [r1, #0x14]
00587644: str      r2, [r0, #0x58]
00587648: beq      #0x58768c
0058764c: ldrb     r2, [r0, #0x18]
00587650: cmp      r2, #0
00587654: bne      #0x58768c
00587658: mov      r0, r3
0058765c: ldr      r3, [r3]
00587660: mov      lr, pc
00587664: ldr      pc, [r3, #0x38]
00587668: add      r5, sp, #4
0058766c: mov      r1, r0
00587670: mov      r0, r5
00587674: bl       #0x586a14
00587678: mov      r0, r5
0058767c: bl       #0x5822f8
00587680: mov      r0, r5
00587684: add      r1, r4, #0x44
00587688: bl       #0x587398
0058768c: add      sp, sp, #0x4c
00587690: pop      {r4, r5, pc}

# _ZNK6glitch5scene17CTriangleSelector9AddResultERKNS_4core10triangle3dIfEE
00586acc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00586ad0: ldr      r3, [r0, #0xa8]
00586ad4: mov      r5, #0x24
00586ad8: ldr      ip, [r1]
00586adc: mul      r3, r5, r3
00586ae0: mov      r4, r0
00586ae4: ldr      r0, [r0, #0xa0]
00586ae8: sub      sp, sp, #0xc
00586aec: str      ip, [r0, r3]
00586af0: add      r2, r0, r3
00586af4: ldr      r3, [r1, #4]
00586af8: str      r3, [r2, #4]
00586afc: ldr      r3, [r1, #8]
00586b00: str      r3, [r2, #8]
00586b04: ldr      r3, [r1, #0xc]
00586b08: str      r3, [r2, #0xc]
00586b0c: ldr      r3, [r1, #0x10]
00586b10: str      r3, [r2, #0x10]
00586b14: ldr      r3, [r1, #0x14]
00586b18: str      r3, [r2, #0x14]
00586b1c: ldr      r3, [r1, #0x18]
00586b20: str      r3, [r2, #0x18]
00586b24: ldr      r3, [r1, #0x1c]
00586b28: str      r3, [r2, #0x1c]
00586b2c: ldr      r3, [r1, #0x20]
00586b30: str      r3, [r2, #0x20]
00586b34: ldrb     r3, [r4, #0x9c]
00586b38: cmp      r3, #0
00586b3c: bne      #0x586ea4
00586b40: ldr      r7, [r4, #0xa8]
00586b44: ldr      r8, [r4, #0xa0]
00586b48: ldr      r1, [r4, #0x60]
00586b4c: mul      r7, r5, r7
00586b50: ldr      fp, [r8, r7]
00586b54: add      r6, r8, r7
00586b58: ldr      sb, [r6, #4]
00586b5c: mov      r0, fp
00586b60: bl       #0x30ed6c
00586b64: ldr      r1, [r4, #0x70]
00586b68: mov      r3, r0
00586b6c: mov      r0, sb
00586b70: ldr      sl, [r6, #8]
00586b74: str      r3, [sp, #4]
00586b78: bl       #0x30ed6c
00586b7c: ldr      r3, [sp, #4]
00586b80: mov      r1, r0
00586b84: mov      r0, r3
00586b88: bl       #0x30eba4
00586b8c: ldr      r1, [r4, #0x80]
00586b90: mov      r3, r0
00586b94: mov      r0, sl
00586b98: str      r3, [sp, #4]
00586b9c: bl       #0x30ed6c
00586ba0: ldr      r3, [sp, #4]
00586ba4: mov      r1, r0
00586ba8: mov      r0, r3
00586bac: bl       #0x30eba4
00586bb0: ldr      r1, [r4, #0x90]
00586bb4: bl       #0x30eba4
00586bb8: ldr      r1, [r4, #0x64]
00586bbc: mov      r2, r0
00586bc0: mov      r0, fp
00586bc4: str      r2, [sp]
00586bc8: bl       #0x30ed6c
00586bcc: ldr      r1, [r4, #0x74]
00586bd0: mov      r3, r0
00586bd4: mov      r0, sb
00586bd8: str      r3, [sp, #4]
00586bdc: bl       #0x30ed6c
00586be0: ldr      r3, [sp, #4]
00586be4: mov      r1, r0
00586be8: mov      r0, r3
00586bec: bl       #0x30eba4
00586bf0: ldr      r1, [r4, #0x84]
00586bf4: mov      r3, r0
00586bf8: mov      r0, sl
00586bfc: str      r3, [sp, #4]
00586c00: bl       #0x30ed6c
00586c04: ldr      r3, [sp, #4]
00586c08: mov      r1, r0
00586c0c: mov      r0, r3
00586c10: bl       #0x30eba4
00586c14: ldr      r1, [r4, #0x94]
00586c18: bl       #0x30eba4
00586c1c: ldr      r1, [r4, #0x5c]
00586c20: mov      r3, r0
00586c24: mov      r0, fp
00586c28: str      r3, [sp, #4]
00586c2c: bl       #0x30ed6c
00586c30: ldr      r1, [r4, #0x6c]
00586c34: mov      fp, r0
00586c38: mov      r0, sb
00586c3c: bl       #0x30ed6c
00586c40: mov      r1, r0
00586c44: mov      r0, fp
00586c48: bl       #0x30eba4
00586c4c: ldr      r1, [r4, #0x7c]
00586c50: mov      sb, r0
00586c54: mov      r0, sl
00586c58: bl       #0x30ed6c
00586c5c: mov      r1, r0
00586c60: mov      r0, sb
00586c64: bl       #0x30eba4
00586c68: ldr      r1, [r4, #0x8c]
00586c6c: bl       #0x30eba4
00586c70: str      r0, [r8, r7]
00586c74: ldr      r3, [sp, #4]
00586c78: str      r3, [r6, #8]
00586c7c: ldr      r2, [sp]
00586c80: str      r2, [r6, #4]
00586c84: ldr      r3, [r4, #0xa0]
00586c88: ldr      r6, [r4, #0xa8]
00586c8c: ldr      r1, [r4, #0x60]
00586c90: mla      r6, r5, r6, r3
00586c94: ldr      sl, [r6, #0xc]
00586c98: ldr      r8, [r6, #0x10]
00586c9c: ldr      r7, [r6, #0x14]
00586ca0: mov      r0, sl
00586ca4: bl       #0x30ed6c
00586ca8: ldr      r1, [r4, #0x70]
00586cac: mov      sb, r0
00586cb0: mov      r0, r8
00586cb4: bl       #0x30ed6c
00586cb8: mov      r1, r0
00586cbc: mov      r0, sb
00586cc0: bl       #0x30eba4
00586cc4: ldr      r1, [r4, #0x80]
00586cc8: mov      sb, r0
00586ccc: mov      r0, r7
00586cd0: bl       #0x30ed6c
00586cd4: mov      r1, r0
00586cd8: mov      r0, sb
00586cdc: bl       #0x30eba4
00586ce0: ldr      r1, [r4, #0x90]
00586ce4: bl       #0x30eba4
00586ce8: ldr      r1, [r4, #0x64]
00586cec: mov      sb, r0
00586cf0: mov      r0, sl
00586cf4: bl       #0x30ed6c
00586cf8: ldr      r1, [r4, #0x74]
00586cfc: mov      fp, r0
00586d00: mov      r0, r8
00586d04: bl       #0x30ed6c
00586d08: mov      r1, r0
00586d0c: mov      r0, fp
00586d10: bl       #0x30eba4
00586d14: ldr      r1, [r4, #0x84]
00586d18: mov      fp, r0
00586d1c: mov      r0, r7
00586d20: bl       #0x30ed6c
00586d24: mov      r1, r0
00586d28: mov      r0, fp
00586d2c: bl       #0x30eba4
00586d30: ldr      r1, [r4, #0x94]
00586d34: bl       #0x30eba4
00586d38: ldr      r1, [r4, #0x5c]
00586d3c: mov      fp, r0
00586d40: mov      r0, sl
00586d44: bl       #0x30ed6c
00586d48: ldr      r1, [r4, #0x6c]
00586d4c: mov      sl, r0
00586d50: mov      r0, r8
00586d54: bl       #0x30ed6c
00586d58: mov      r1, r0
00586d5c: mov      r0, sl
00586d60: bl       #0x30eba4
00586d64: ldr      r1, [r4, #0x7c]
00586d68: mov      r8, r0
00586d6c: mov      r0, r7
00586d70: bl       #0x30ed6c
00586d74: mov      r1, r0
00586d78: mov      r0, r8
00586d7c: bl       #0x30eba4
00586d80: ldr      r1, [r4, #0x8c]
00586d84: bl       #0x30eba4
00586d88: str      r0, [r6, #0xc]
00586d8c: str      sb, [r6, #0x10]
00586d90: str      fp, [r6, #0x14]
00586d94: ldr      r2, [r4, #0xa8]
00586d98: ldr      r3, [r4, #0xa0]
00586d9c: ldr      r1, [r4, #0x60]
00586da0: mla      r5, r5, r2, r3
00586da4: ldr      r8, [r5, #0x18]
00586da8: ldr      r7, [r5, #0x1c]
00586dac: ldr      r6, [r5, #0x20]
00586db0: mov      r0, r8
00586db4: bl       #0x30ed6c
00586db8: ldr      r1, [r4, #0x70]
00586dbc: mov      sl, r0
00586dc0: mov      r0, r7
00586dc4: bl       #0x30ed6c
00586dc8: mov      r1, r0
00586dcc: mov      r0, sl
00586dd0: bl       #0x30eba4
00586dd4: ldr      r1, [r4, #0x80]
00586dd8: mov      sl, r0
00586ddc: mov      r0, r6
00586de0: bl       #0x30ed6c
00586de4: mov      r1, r0
00586de8: mov      r0, sl
00586dec: bl       #0x30eba4
00586df0: ldr      r1, [r4, #0x90]
00586df4: bl       #0x30eba4
00586df8: ldr      r1, [r4, #0x64]
00586dfc: mov      sl, r0
00586e00: mov      r0, r8
00586e04: bl       #0x30ed6c
00586e08: ldr      r1, [r4, #0x74]
00586e0c: mov      sb, r0
00586e10: mov      r0, r7
00586e14: bl       #0x30ed6c
00586e18: mov      r1, r0
00586e1c: mov      r0, sb
00586e20: bl       #0x30eba4
00586e24: ldr      r1, [r4, #0x84]
00586e28: mov      sb, r0
00586e2c: mov      r0, r6
00586e30: bl       #0x30ed6c
00586e34: mov      r1, r0
00586e38: mov      r0, sb
00586e3c: bl       #0x30eba4
00586e40: ldr      r1, [r4, #0x94]
00586e44: bl       #0x30eba4
00586e48: ldr      r1, [r4, #0x5c]
00586e4c: mov      sb, r0
00586e50: mov      r0, r8
00586e54: bl       #0x30ed6c
00586e58: ldr      r1, [r4, #0x6c]
00586e5c: mov      r8, r0
00586e60: mov      r0, r7
00586e64: bl       #0x30ed6c
00586e68: mov      r1, r0
00586e6c: mov      r0, r8
00586e70: bl       #0x30eba4
00586e74: ldr      r1, [r4, #0x7c]
00586e78: mov      r7, r0
00586e7c: mov      r0, r6
00586e80: bl       #0x30ed6c
00586e84: mov      r1, r0
00586e88: mov      r0, r7
00586e8c: bl       #0x30eba4
00586e90: ldr      r1, [r4, #0x8c]
00586e94: bl       #0x30eba4
00586e98: str      r0, [r5, #0x18]
00586e9c: str      sb, [r5, #0x20]
00586ea0: str      sl, [r5, #0x1c]
00586ea4: ldr      r3, [r4, #0xa8]
00586ea8: ldr      r0, [r4, #0xa4]
00586eac: add      r3, r3, #1
00586eb0: cmp      r3, r0
00586eb4: movne    r0, #0
00586eb8: moveq    r0, #1
00586ebc: str      r3, [r4, #0xa8]
00586ec0: add      sp, sp, #0xc
00586ec4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
